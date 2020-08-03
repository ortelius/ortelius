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
package dmadmin.model;

//import java.util.Map;

import java.util.Enumeration;
import java.util.Hashtable;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;

public class Task
	extends DMObject
	implements IPrePostAction
{
	private static final long serialVersionUID = 1327862378913381548L;

	private String m_TaskType;
	private Action m_preAction;
	private Action m_postAction;
	private boolean m_showOutput;
	private boolean m_subDomains;
	private NotifyTemplate m_successTemplate;
	private NotifyTemplate m_failureTemplate;
	protected Hashtable<String,String> m_aps;
	
	public enum TaskType {
		UNKNOWN       (""),
		APPROVE       ("Approve"),
		MOVE          ("Move"),
		REMOVE        ("Remove"),
		CREATE_VERSION("CreateVersion"),
		DEPLOY        ("Deploy"),
		REQUEST       ("Request"),
		RUN_ACTION	  ("RunAction");
		
		private String m_str;
		
		private TaskType(String str)  { m_str = str; }
		
		public static TaskType fromString(String str) {
			for(TaskType tt : TaskType.values()) {
				if(tt.m_str.equalsIgnoreCase(str)) {
					return tt;
				}
			}
			return UNKNOWN;
		}
		
		public String getTaskTypeString() {
			return m_str;
		}
	};
	
	public Task() {
		m_TaskType="";
	}
	
	public Task(DMSession sess, int id, String name) {
		super(sess, id, name);
		m_aps=null;
		m_TaskType="";
	}
	
	public void setTaskType(String tasktype)  { m_TaskType = tasktype; }
	public String getTaskTypeAsString()  { return m_TaskType; }
	
	public Action getPreAction()  { return m_preAction; }
	public void setPreAction(Action action)  { m_preAction = action; }

	public Action getPostAction()  { return m_postAction; }
	public void setPostAction(Action action)  { m_postAction = action; }
	
	public boolean getShowOutput()  { return m_showOutput; }
	public void setShowOutput(boolean showOutput)  { m_showOutput = showOutput; }
	
	public boolean getSubDomains() { System.out.println("getSubDomains returning "+m_subDomains);return m_subDomains; }
	public void setSubDomains(boolean subDomains) { System.out.println("setSubDomains("+subDomains+")");m_subDomains = subDomains; }
	
	public TaskType getTaskType()
	{
		return TaskType.fromString(m_TaskType);
	}

	public NotifyTemplate getSuccessTemplate()  { return m_successTemplate; }
	public void setSuccessTemplate(NotifyTemplate template)  { m_successTemplate = template; }
	
	public NotifyTemplate getFailureTemplate()  { return m_failureTemplate; }
	public void setFailureTemplate(NotifyTemplate template)  { m_failureTemplate = template; }
	
	public void setAdditionalParameters(Hashtable<String,String> aps) { m_aps = aps; }
	public Hashtable<String,String> getAdditionalParameters() { return m_aps; }
	public String getAddParamsCmdLine() {
		String res="";
		String sep="";
		Enumeration<String> keys = m_aps.keys();
		while (keys.hasMoreElements()) {
		    String key = keys.nextElement();
		    String val = m_aps.get(key);
		    res+=sep+key+"="+val;
		    sep=" ";
		}
		return res;
	}
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.TASK;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_task";
	}

	@Override
	public String getForeignKey() {
		return "taskid";
	}
	
	public boolean run() {
		return false;
	}
	
	public String getOutput() {
		return null;
	}
	
	public String getFormattedOutput() {
		String output = getOutput();
		if(output != null) {
			return output.replace("\n", "<br>");
		}
		return null;
	}

	// Question: is this even used? Isn't getSummaryJSON defined in individual action classes?
	@Override
	public IJSONSerializable getSummaryJSON() {
		System.out.println("m_TaskType="+m_TaskType);
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "Name", getName());
		addCreatorModifier(ds);
		ds.addProperty(SummaryField.PRE_ACTION, "Pre-action", ((m_preAction != null) ? m_preAction.getLinkJSON() : null));
		ds.addProperty(SummaryField.POST_ACTION, "Post-action", ((m_postAction != null) ? m_postAction.getLinkJSON() : null));
		if (m_TaskType.equalsIgnoreCase("approve")) {
			System.out.println("m_successTemplate="+((m_successTemplate == null)?"null":"not null"));
			ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Approved Template",  ((m_successTemplate != null) ? m_successTemplate.getLinkJSON() : null));
			ds.addProperty(SummaryField.FAILURE_TEMPLATE, "Rejected Template",  ((m_failureTemplate != null) ? m_failureTemplate.getLinkJSON() : null));
		}
		else
		if (m_TaskType.equalsIgnoreCase("move")) {
			ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Notify Template",  ((m_successTemplate != null) ? m_successTemplate.getLinkJSON() : null));
		}
		ds.addProperty(SummaryField.TASK_SHOWOUTPUT, "Show Output", new BooleanField(m_showOutput));	
		ds.addProperty(SummaryField.TASK_AVAILABLE_TO_SUBDOMAINS, "Available in SubDomains", new BooleanField(m_subDomains));
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		System.out.println("updateSummary called (in Task.java)");
		return m_session.updateTask(this, changes);
	}
	
	@Override
	public boolean hasReadWrite() {
		return false;
	}
}
