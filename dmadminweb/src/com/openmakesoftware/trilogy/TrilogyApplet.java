package com.openmakesoftware.trilogy;

import java.applet.Applet;

public class TrilogyApplet extends Applet implements ITrilogy
{
    private static final long serialVersionUID = 7200216581001923367L;

    private TrilogyImpl m_impl;
    
    public TrilogyApplet()
    {
        m_impl = new TrilogyImpl();
    }
    
    public void init()
    {
        // Nothing to do in here.
    }
    
    public void setServerPort(int port)
    {
        m_impl.setServerPort(port);
    }

    public void setServerName(String server)
    {
        m_impl.setServerName(server);
    }

    public boolean endOfStream(int stream)
    {
        return m_impl.endOfStream(stream);
    }

    public String getLine(int stream)
    {
        return m_impl.getLine(stream);
    }

    public void execute() throws TrilogyException
    {
        m_impl.execute();
    }
    
    public ITrilogy addParameter(String param)
    {
        m_impl.addParameter(param);
        return this;
    }

    public ITrilogy setProgramID(String jobName)
    {
        m_impl.setProgramID(jobName);
        return this;
    };

    public String exitCode()
    {
        return m_impl.exitCode();
    }

    public void listJobs()
    {
        m_impl.listJobs();
    }
}
