/*
 *
 *  Ortelius for Microservice Configuration Mapping
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package dmadmin;

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;

public class GanttDataSet
{
	private class GanttBar
	{
		private int m_start;
		private int m_end;
		private boolean m_overlaps;
		
		GanttBar(int start, int end) {
			m_start = start;
			m_end = end;
			m_overlaps = false;
		}
		
		/**
		 * Called first on all bars to make the chart start at zero
		 * @param start
		 */
		void makeRelative(int start) {
			m_start -= start;
			m_end -= start;
		}
		
		/**
		 * Used to trim a server bar so that it fits within the step bar
		 * @param start
		 * @param end
		 */
		void trim(GanttBar bar) {
			if(m_start < bar.m_start) {
				System.out.println("Trimming bar from start at " + m_start + " to " + bar.m_start);
				m_start = bar.m_start;
			}
			if(m_end > bar.m_end) {
				//System.out.println("Trimming bar from end at " + m_end + " to " + bar.m_end);
				m_end = bar.m_end;
			}
		}
		
		/**
		 * Used to trim a server bar so that it fits before the next bar
		 * @param start
		 * @param end
		 */
		void trimToNext(GanttBar bar, GanttBar stepBar) {
			if(m_end > bar.m_start) {
				//System.out.println("Trimming server bar from end at " + m_end + " to " + bar.m_start);
				m_end = bar.m_start;
				m_overlaps = true;
				stepBar.m_overlaps = true;
			}
		}
		
		int getStart() {  return m_start; }
		// int getEnd() {  return m_end; }
		boolean isOverlapping()  { return m_overlaps; }
		
		/**
		 * Returns the start of a bar as a cumulative value - will make the blank bar in front
		 * @param start
		 * @return
		 */
		int getStartRelative(GanttBar bar) {
			int rel = m_start - bar.m_start;
			return (rel > 0) ? rel : 0;
		}
		
		/**
		 * Returns the length of the bar
		 * @return
		 */
		int getDuration() {
			return m_end - m_start;
		}
		
		/**
		 * Returns the end of a bar as a cumulative value - will make the blank bar after
		 * @param end
		 * @return
		 */
		int getEndRelative(GanttBar bar) {
			int rel =  bar.m_end - m_end;
			return (rel > 0) ? rel : 0;
		}
	}
	
	private List<Integer> m_stepIds = new ArrayList<Integer>();
	private List<Integer> m_serverIds = new ArrayList<Integer>();
	private Hashtable<Integer, String> m_steps = new Hashtable<Integer, String>();
	private Hashtable<Integer, String> m_servers = new Hashtable<Integer, String>();
	private Hashtable<Integer, GanttBar> m_stepData = new Hashtable<Integer, GanttBar>();
	private Hashtable<Integer, Hashtable<Integer, GanttBar>> m_serverData = new Hashtable<Integer, Hashtable<Integer, GanttBar>>();
	private int m_min = 0;
	private int m_max = 0;
	
	public GanttDataSet()
	{}
	
	public void addStep(int stepId, String stepName, int start, int end)
	{
		System.out.println("addStep(" + stepId + "," + stepName + "," + start + "," + end + ")");
		m_stepIds.add(stepId);
		m_steps.put(stepId, stepName);
		m_stepData.put(stepId, new GanttBar(start, end));
		if((m_min == 0) || (start < m_min)) {
			m_min = start;
		}
		if(end > m_max) {
			m_max = end;
		}
	}
	
	public void addServerStep(int stepId, int serverId, String serverName, int start, int end)
	{	
		if(m_servers.get(serverId) == null) {
			m_servers.put(serverId, serverName);
			m_serverIds.add(serverId);
		}
		
		Hashtable<Integer, GanttBar> data = m_serverData.get(stepId);
		if(data == null) {
			data = new Hashtable<Integer, GanttBar>();
			m_serverData.put(stepId, data);
		}
		
		data.put(serverId, new GanttBar(start, end));
	}
	
	public int getNumberOfServers()
	{
		return m_serverIds.size();
	}
	
	private void calculate()
	{
		for(GanttBar bar : m_stepData.values()) {
			bar.makeRelative(m_min);
		}
		for(int stepId : m_serverData.keySet()) {
			Hashtable<Integer, GanttBar> serverData = m_serverData.get(stepId);
			GanttBar stepBar = m_stepData.get(stepId);
			for(GanttBar serverBar : serverData.values()) {
				serverBar.makeRelative(m_min);
				serverBar.trim(stepBar);				
			}
		}
		// Find the next step for this server and make sure we don't overlap
		for(int serverId: m_serverIds) {
			GanttBar prev = null;
			for(int stepId : m_stepIds) {
				GanttBar stepBar = m_stepData.get(stepId);
				Hashtable<Integer, GanttBar> data = m_serverData.get(stepId);
				if(data != null) {
					GanttBar serverBar = data.get(serverId);
					if(serverBar != null) {
						//System.out.println("Server " + serverId + " has bar start " + serverBar.getStart() + ", end " + serverBar.getEnd() + ", length " + serverBar.getDuration());
						if(prev != null) {
							prev.trimToNext(serverBar, stepBar);
						}
						prev = serverBar;
					}
				}
			}
		}
	}
	
	public IJSONSerializable getDataJSON() {
		calculate();
		
		JSONArray data = new JSONArray();
		
		// This loops through the steps in time order for the series
		//for(int n = m_stepIds.size()-1; n >= 0; n--) {
		for(int n = 0; n < m_stepIds.size(); n++) {
			int stepId = m_stepIds.get(n);
			GanttBar stepBar = m_stepData.get(stepId);
			System.out.println("Step " + stepId + " has bar length " +  stepBar.getDuration());
			JSONArray arr1 = new JSONArray();
			JSONArray arr2 = new JSONArray();
			JSONArray arr3 = new JSONArray();
			
			// Server bars
			Hashtable<Integer, GanttBar> serverData = m_serverData.get(stepId);
			for(int serverId : m_serverIds) {
				GanttBar serverBar = (serverData != null) ? serverData.get(serverId) : null;
				if(serverBar != null) {
					//System.out.println("Server " + serverId + " has bar length " +  stepBar.getDuration() + " for step " + stepId);
					arr1.add(serverBar.getStartRelative(stepBar));
					Object duration = (serverBar.getDuration() == 0) ? new Float(0.01) : new Integer(serverBar.getDuration());
					arr2.add(duration);
					arr3.add(serverBar.isOverlapping() ? 0 : serverBar.getEndRelative(stepBar));
				} else {
					arr1.add(0);
					arr2.add(0);
					arr3.add(stepBar.isOverlapping() ? 0 : stepBar.getDuration());
				}				
			}
			
			// Step bars
			// This loops through the steps in backwards step order - each step
			// has only one bar, so we output a blank bar before it to get the
			// start position and then a bar of the desired length in the step
			// series colour
			for(int m = m_stepIds.size()-1; m >= 0; m--) {
				if(m == n) {
					arr1.add(/*0*/ stepBar.getStart());
					Object duration = (stepBar.getDuration() == 0) ? new Float(0.01) : new Integer(stepBar.getDuration());
					arr2.add(duration);
					arr3.add(0);					
				} else {
					arr1.add(0);
					arr2.add(0);
					arr3.add(0 /*stepBar.getDuration()*/);
				}
			}
			data.add(arr1).add(arr2).add(arr3);
		}
		return data;
	}
	
	public IJSONSerializable getSeriesJSON() {
		JSONObject o1 = new JSONObject();
		o1.add("showLine", false);
		o1.add("fillAlpha", 0.0);
		o1.add("shadow", false);
		
		JSONObject o2 = new JSONObject();
		o2.add("showLine", true);
		
		JSONArray arr = new JSONArray();
		for(int n = 0, max = (m_stepIds.size() + m_serverIds.size()); n < max; n++) {
			arr.add(o1).add(o2).add(o1);
		}
		return arr;
	}
	
	public IJSONSerializable getTicksJSON() {
		JSONArray arr = new JSONArray();
		for(int serverId : m_serverIds) {
			arr.add(m_servers.get(serverId));
		}
		for(int n = m_stepIds.size()-1; n >= 0; n--) {
			int stepId = m_stepIds.get(n);
			arr.add(stepId + ": " + m_steps.get(stepId));
		}
		return arr;
	}
	
	public IJSONSerializable getOverlayJSON()
	{
		JSONObject ret = new JSONObject();
		ret.add("show", true);
		JSONArray objs = new JSONArray();
		ret.add("objects", objs);
		JSONObject line1 = new JSONObject();
		objs.add(line1);
		JSONObject l1 = new JSONObject();
		line1.add("horizontalLine", l1);
		l1.add("name", "divider");
		l1.add("y", ((float) m_serverIds.size()) + 0.5);
		l1.add("lineWidth", 2);
		l1.add("color", "black");
		l1.add("shadow", false);
		return ret;
	}
}
