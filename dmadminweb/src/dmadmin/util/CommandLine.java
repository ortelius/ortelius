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
package dmadmin.util;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import dmadmin.DMSession;
import dmadmin.model.Engine;

public class CommandLine
{
	private List<String> m_cmd = new ArrayList<String>();
	private StringBuffer m_output;
	private InputMuncherThread m_muncher;
	private Engine m_engine;
	private String m_password = "";

	
	public CommandLine()
	{}
	
	public CommandLine(Engine engine)
	{
		m_engine = engine;
	}

	public CommandLine(String cmd) {
		m_cmd.add(cmd);
	}

	public CommandLine add(String arg) {
		m_cmd.add(arg);
		return this;
	}
	
	public CommandLine add(int arg) {
		m_cmd.add("" + arg);
		return this;
	}
	
	public CommandLine add(Map<String,String> map) {
		if(map != null) {
			for(String param : map.keySet()) {
				m_cmd.add(param + "=" + map.get(param));
			}
		}
		return this;
	}
	
	public CommandLine pw(String pw)
	{
	 m_password = pw;	
	 return this;
	}
	
	public String getOutput() {
		return (m_output != null) ? m_output.toString() : null;
	}

	public String getLastOutputLine() {
		if(m_muncher != null) { return m_muncher.getLastOutputLine(); }
		if(m_output != null) { return m_output.toString(); }
		return null;
	}

	public int run(boolean waitFor) {
		return run(waitFor, null, false);
	}

	public int run(boolean waitFor, String input) {
		return run(waitFor, input, false);
	}
	
	public int run(boolean waitFor, String input, boolean capture)
	{
		// Create an environment for the process copying all environment vars from the system
		//Map<String,String> myenv = System.getenv();
		//Hashtable<String,String> env = new Hashtable<String,String>();
		//for(String key: myenv.keySet()) {
		//	env.put(key, myenv.get(key));
		//}	

		////env.put("TRIREASON", "SCRIPT");
		////env.put("TRIFIELD1", envName);
		////env.put("TRIFIELD2", "Robert");
		////env.put("TRIFIELD3", "password");
		////env.put("TRIFIELD4", appName);
		
		//List<String> envl = new ArrayList<String>();
		//for(String key: env.keySet()) {
		//	envl.add(key + "=" + env.get(key));
		//}
		
		// DEBUG
		System.out.print("Running: ");
		for(String s : m_cmd) {
			System.out.print("\"" + s + "\" ");
		}
		System.out.println();
		// END DEBUG
		
		String[] args = new String[m_cmd.size()];
		//String[] envp = new String[env.size()];
		try {
			ProcessBuilder pb = new ProcessBuilder(m_cmd.toArray(args));
			Map<String, String> env = pb.environment();
			env.put("TRIFIELD1",m_password);
			pb.redirectErrorStream(true);
			
			Process p = pb.start(); //Runtime.getRuntime().exec(m_cmd.toArray(args) /*, envl.toArray(envp)*/);
			
			// If input is given, send it to the process and close the stream
			if(input != null) {
				try {					
					OutputStreamWriter stdin = new OutputStreamWriter(p.getOutputStream());
					stdin.write(input);
					stdin.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
			if(capture) {
				try {					
					System.out.println("Capturing output");
					m_output = new StringBuffer();
					String line;
					BufferedReader stdout = new BufferedReader(new InputStreamReader(p.getInputStream()));
					while((line = stdout.readLine()) != null) {
						System.out.println(line);
						m_output.append(line).append("\n");
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			} else {
				m_muncher = new InputMuncherThread(p.getInputStream());
				new Thread(m_muncher).start();
			}

			if(waitFor) {
				try {					
					System.out.println("Waiting for command to complete");
					int ec = p.waitFor();
					System.out.println("Exit Code: " + ec);
					//if(capture) {
					//	m_output.append("Exit Code: " + ec + "\n");
					//}
					return ec;
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			return -1;
		}
		return 0;
	}
	
	public int runWithTrilogy(boolean waitFor, String input)
	{
		// DEBUG
		System.out.print("Running: ");
		for(String s : m_cmd) {
			System.out.print("\"" + s + "\" ");
		}
		System.out.println();
		// END DEBUG
		
        String server = (m_engine != null) ? m_engine.getHostname() : "localhost";
        String clientid = (m_engine != null) ? m_engine.getClientID() : null;
        
        if (clientid != null) {
        	System.out.println("Inserting request into dm.dm_queue for client id "+clientid);
        	DMSession so = m_engine.getSession();
        	if (so != null) {
        		int queueid = so.insertIntoRunQueue(clientid,input,m_cmd,waitFor);
        		if (waitFor) {
        			m_output = new StringBuffer();
        			int ec = so.waitForRunQueue(queueid,clientid,m_output);
        			System.out.println("waitfor=true ec="+ec+" m_output="+m_output);
        		}
        		return 0;
        	}
        	System.out.println("No session associated with engine");
        	return -1;
        } else {
        	return run(waitFor, input, true);
       }
	}
	
	/**
	 * Sneaky little class that implements a reader thread for a process we
	 * have started.  Rather than trying to buffer the entire output, it
	 * remembers just the last line.  If the process does not start normally we
	 * can come back and read this last line to get an idea what went wrong.
	 */
	private class InputMuncherThread
		implements Runnable
	{
		private InputStream m_stream;
		private String m_output;
		
		public InputMuncherThread(InputStream stream) {
			m_stream = stream;
		}
		
		public String getLastOutputLine()  { return m_output; }
		
		@Override
		public void run() {
			System.out.println("Munching output");
			try {
				String line;
				BufferedReader stdout = new BufferedReader(new InputStreamReader(m_stream));
				while((line = stdout.readLine()) != null) {
					//System.out.println(line);
					m_output = line;
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}	
	}
}	
