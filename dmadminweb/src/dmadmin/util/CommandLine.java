/*
 *
 *  DeployHub is an Agile Application Release Automation Solution
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
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.openmakesoftware.trilogy.ITrilogyOutputStream;
import com.openmakesoftware.trilogy.TrilogyException;
import com.openmakesoftware.trilogy.TrilogyLib;

import dmadmin.DMSession;
import dmadmin.model.Engine;

public class CommandLine
{
	private List<String> m_cmd = new ArrayList<String>();
	private StringBuffer m_output;
	private InputMuncherThread m_muncher;
	private Engine m_engine;
	private TrilogyJobMonitor m_monitor;
	
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
	
	public String getOutput() {
		return (m_output != null) ? m_output.toString() : null;
	}

	public String getLastOutputLine() {
		if(m_muncher != null) { return m_muncher.getLastOutputLine(); }
		if(m_monitor != null) { return m_monitor.getLastOutputLine(); }	// TODO: May need to add  + ((m_output != null) ? ("\n" + m_output) : "")
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
		
				/*String line;
				java.io.BufferedReader stderr = new java.io.BufferedReader(new java.io.InputStreamReader(p.getErrorStream()));
				while((line = stderr.readLine()) != null) {
					System.out.println(line);
				}
				java.io.BufferedReader stdout = new java.io.BufferedReader(new java.io.InputStreamReader(p.getInputStream()));
				while((line = stdout.readLine()) != null) {
					System.out.println(line);
				}*/
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
        	System.out.println("Sending request to Trilogy Server on hostname "+server);
        	int port = m_engine.getSession().getTrilogyPort();
	        // int port = 2305;		// TODO: Get this from somewhere
	        
			String[] args = new String[m_cmd.size()];
			
			System.out.println("Setting output stream, waitFor = "+waitFor);
			
			ITrilogyOutputStream output = waitFor ? new TrilogyOutputBuffer() : new TrilogyOutputMuncher();
	
	        try {
	        	// Pro Version - send password through as TRIFIELD1
	        	String[] fields = new String[1];
	        	fields[0] = m_engine.getSession().getPassword();
	        	System.out.println("server is ["+server+"]");
	        	TrilogyLib.TrilogyJob job = TrilogyLib.sendRunRequest(
	        			server, port, m_cmd.toArray(args), fields, input, output);
	        	if(job == null) {
	        		return -1;
	        	}
	        	
	        	if (waitFor) {
	        		System.out.println("Waiting for Completion");
		        	job.waitForCompletion();
		            m_output = output.getBuffer();
		            int ec = job.getExitCode();
		            System.out.println("m_output="+m_output);
					System.out.println("Exit Code: " + ec);
					return ec;
	        	} else {
	        		System.out.println("Not waiting - starting job monitor");
	        		m_monitor = new TrilogyJobMonitor(job);
	        		m_monitor.start();
	        		System.out.println("Job Monitor Started");
	            	return 0;
	        	}
	        } catch(java.security.AccessControlException ace) {
	        	m_output = new StringBuffer("AccessControlException: " + ace.toString());
	        	return -1;
	        } catch(TrilogyException e) {
	        	m_output = new StringBuffer(e.getMessage());
	        	return -1;
	        } catch(Exception e) {
	            e.printStackTrace();
	        	m_output = new StringBuffer(e.toString());
	        	return -1;
	        }
        }
	}
	
	public boolean trilogyJobRunning()
	{
		return (m_monitor != null)?m_monitor.isAlive():false;
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
	
	private class TrilogyOutputBuffer
		implements ITrilogyOutputStream
	{
		private ByteArrayOutputStream m_str;

		public TrilogyOutputBuffer() {
			m_str = new ByteArrayOutputStream();
		}

		@Override
		public void writeToStdOut(byte[] buf) {
			try {
				m_str.write(buf);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		@Override
		public void writeToStdErr(byte[] buf) {
			try {
				//m_str.write("<span style=\"color: #e00\">".getBytes());
				m_str.write(buf);
				//m_str.write("</span>".getBytes());
			} catch (IOException e) {
				e.printStackTrace();
			}			
		}
		
		@Override
		public StringBuffer getBuffer() {
			StringBuffer sb = new StringBuffer();
			try {
				BufferedReader isr = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(m_str.toByteArray())));
				String line;
				while((line = isr.readLine()) != null) {
					sb.append(line).append("\n");
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			return sb;
		}
	}
	
	private class TrilogyOutputMuncher
		implements ITrilogyOutputStream
	{
		private byte[] m_lastbuf;
		private StringBuffer m_sb;
		
		private TrilogyOutputMuncher()
		{
			m_sb = new StringBuffer();
		}
		
		@Override
		public void writeToStdOut(byte[] buf) {
			m_lastbuf = buf;
			if(m_lastbuf != null) {
				try {
					BufferedReader isr = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(m_lastbuf)));
					String line;
					while((line = isr.readLine()) != null) {
						m_sb.append(line).append("\n");
						System.out.println("Appending line="+line);
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		@Override
		public void writeToStdErr(byte[] buf) {
			m_lastbuf = buf;
			writeToStdOut(buf);
		}
		
		@Override
		public StringBuffer getBuffer() {
			//StringBuffer sb = new StringBuffer();
			//if(m_lastbuf != null) {
			//	try {
			//		BufferedReader isr = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(m_lastbuf)));
			//		String line;
			//		while((line = isr.readLine()) != null) {
			//			sb.append(line).append("\n");
			//		}
			//	} catch (IOException e) {
			//		e.printStackTrace();
			//	}
			//}
			// return sb;
			// System.out.println("getBuffer returns "+m_sb);
			return m_sb;
		}
	}
	
	private class TrilogyJobMonitor
		extends Thread
	{
		private TrilogyLib.TrilogyJob m_job;
		private int m_exitCode = -1;
		
		public TrilogyJobMonitor(TrilogyLib.TrilogyJob job) {
			m_job = job;
		}
		
		// public int getExitCode()  { return m_exitCode; }
		public String getLastOutputLine()
		{
			return m_job.getOutput().getBuffer().toString(); 
		}
		
		@Override
		public void run() {
			try {
				m_job.waitForCompletion();
				m_exitCode = m_job.getExitCode();
				System.out.println("exit code="+m_exitCode);
			} catch (TrilogyException e) {
				m_exitCode = -1;
				ITrilogyOutputStream outbuf = m_job.getOutput();				
				outbuf.writeToStdOut(e.toString().getBytes());

    e.printStackTrace();
			}
		}
	}
}
