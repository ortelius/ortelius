/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package dmadmin.util;

import java.io.BufferedReader;
import java.io.File;
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
			env.put("DMHOME","/opt/deployhub/engine");
			pb.redirectErrorStream(true);
			pb.directory(new File("/opt/deployhub/engine"));
			
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
	
 public int runWithTrilogyNoCapture(boolean waitFor, String input)
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
         return run(waitFor, input, false);
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
