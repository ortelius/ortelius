package com.openmakesoftware.trilogy;

public interface ITrilogy
{
	public static final int OUTPUT_STREAM = 1;
	public static final int ERROR_STREAM = 2;
	
    /**
     * Sets the port number of the target Trilogy server.
     * 
     * @param port
     * CI Test2
     *                The port number.
     */
    public void setServerPort(int port);
    
    /**
     * Sets the hostname of the target Trilogy server.
     * 
     * @param server
     *                The server name.
     */
    public void setServerName(String server);

    /**
     * Returns true when the end of the given stream has been reached.
     * 
     * @param stream
     *                id of the stream to check.
     * @return true if at end of given stream.
     */
    public boolean endOfStream(int stream);
    
    /**
     * Returns a line of text from the given stream.
     * 
     * @param stream
     *                id of the stream to read from.
     * @return the line of text.
     */
    public String getLine(int stream);
    
    /**
     * Runs the given Trilogy job. The output can be obtained by using
     * {@link endOfStream} and {@link getLine}.
     * 
     * @throws TrilogyException 
     */
    public void execute() throws TrilogyException;
    
    public ITrilogy addParameter(String param);
    
    public ITrilogy setProgramID(String jobName);

    public String exitCode();
    
    /**
     * Lists the available Trilogy jobs on the server. The output can be
     * obtained by using {@link endOfStream} and {@link getLine}.
     */
    public void listJobs();
}
