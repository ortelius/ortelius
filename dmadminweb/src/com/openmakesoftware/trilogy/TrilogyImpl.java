package com.openmakesoftware.trilogy;

import java.util.ArrayList;

public class TrilogyImpl
	implements ITrilogy
{
    private String                  m_server;
    private int                     m_port;
    private String                  m_jobName;
    private ArrayList<String>       m_params;
    private String                  m_exitCode = "";
    private TrilogyLib.CapturedData m_cd;

    public TrilogyImpl()
    {
        m_server = "localhost";
        m_port = 2305;
    }

    /**
     * Sets the port number of the target Trilogy server.
     * 
     * @param port
     *                The port number.
     */
    public void setServerPort(int port)
    {
        m_port = port;
    }

    /**
     * Sets the hostname of the target Trilogy server.
     * 
     * @param server
     *                The server name.
     */
    public void setServerName(String server)
    {
        m_server = server;
    }

    /**
     * Returns true when the end of the given stream has been reached.
     * 
     * @param stream
     *                id of the stream to check.
     * @return true if at end of given stream.
     */
    public boolean endOfStream(int stream)
    {
        if(m_cd != null) {
            try {
                return !m_cd.getReader(stream).ready();
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
        return true;
    }

    /**
     * Returns a line of text from the given stream.
     * 
     * @param stream
     *                id of the stream to read from.
     * @return the line of text.
     */
    public String getLine(int stream)
    {
        if(m_cd != null) {
            try {
                return m_cd.getReader(stream).readLine();
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
        return "";
    }

    /**
     * Runs the given Trilogy job. The output can be obtained by using
     * {@link endOfStream} and {@link getLine}.
     */
    public void execute()
    	throws TrilogyException
    {
        if(m_jobName == null) {
            m_exitCode = "Must set job name with setProgramID() before calling execute().";
            return;
        }

        int numParams = (m_params != null) ? m_params.size() : 0;
        String[] args = new String[numParams + 2];
        args[0] = "trilogy";
        args[1] = m_jobName;
        for(int n = 0; n < numParams; n++) {
            args[n + 2] = m_params.get(n);
        }

        m_cd = new TrilogyLib.CapturedData();

        try {
            TrilogyLib.IntHolder exitCode = new TrilogyLib.IntHolder();
            TrilogyLib.sendRunRequest(m_server, m_port, args, new String[] {},
                    new TrilogyLib.StringHolder(), 0, 1, 2, true,
                    new TrilogyLib.IntHolder(), new TrilogyLib.IntHolder(),
                    m_cd, new TrilogyLib.IntHolder(), exitCode,
                    new TrilogyLib.StringHolder());
            m_exitCode = Integer.toString(exitCode.value);
        } catch(java.security.AccessControlException ace) {
            m_exitCode = "AccessControlException: " + ace.toString();
        } catch(TrilogyException e) {
        	throw e;
        } catch(Exception e) {
            m_exitCode = "Exception " + e.toString();
            e.printStackTrace();
        }
    }

    public ITrilogy addParameter(String param)
    {
        if(m_params == null) {
            m_params = new ArrayList<String>();
        }
        m_params.add(param);
        return this;
    }

    public ITrilogy setProgramID(String jobName)
    {
        m_jobName = jobName;

        if(m_params != null) {
            m_params.clear();
        }
        return this;
    }

    public String exitCode()
    {
        return m_exitCode;
    }

    /**
     * Lists the available Trilogy jobs on the server. The output can be
     * obtained by using {@link endOfStream} and {@link getLine}.
     */
    public void listJobs()
    {
        m_cd = new TrilogyLib.CapturedData();

        try {
            TrilogyLib.listJobs(m_server, m_port, m_cd);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

//    public static void main(String[] args)
//    {
//        ITrilogy trilogy = new TrilogyImpl();
//        // trilogy.listJobs();
//        trilogy.setProgramID("SDC_REVIEW_FORM");
//        trilogy.addParameter("harvest");
//        trilogy.addParameter("SDC Agile - DSC REQ-5");
//        trilogy.execute();
//        // System.out.println("Exit code: " + trilogy.exitCode());
//        while(!trilogy.endOfStream(1))
//        {
//            String line = trilogy.getLine(1);
//            if(line == null)
//                break;
//            System.out.println(line);
//        }
//    }
}
