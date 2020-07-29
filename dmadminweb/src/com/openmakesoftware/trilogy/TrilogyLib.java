package com.openmakesoftware.trilogy;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringReader;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import com.openmakesoftware.XDR.XDR;

public class TrilogyLib
{
    public static class CapturedData
    {
        public static final int OUT = 1;
        public static final int ERR = 2;
        
        private StringBuffer   m_standardOut = new StringBuffer();
        private StringBuffer   m_standardErr = new StringBuffer();
        private BufferedReader m_outReader   = null;
        private BufferedReader m_errReader   = null;

        public BufferedReader getReader(int stream)
        {
            switch(stream)
            {
            case OUT:
                if(m_outReader == null) {
                    m_outReader = new BufferedReader(new StringReader(
                            m_standardOut.toString()));
                }
                return m_outReader;
            case ERR:
                if(m_errReader == null) {
                    m_errReader = new BufferedReader(new StringReader(
                            m_standardOut.toString()));
                }
                return m_errReader;
            default:
                return null;
            }
        }

        public void addToCapture(int stream, byte buf[])
        {
            switch(stream) {
            case OUT:
                for(int n = 0; n < buf.length; n++) {
                    m_standardOut.append((char) buf[n]);
                }
                break;
            case ERR:
                for(int n = 0; n < buf.length; n++) {
                    m_standardErr.append((char) buf[n]);
                }
                break;
            }
        }

        public void addToCapture(int stream, String str)
        {
            switch(stream) {
            case OUT: m_standardOut.append(str); break;
            case ERR: m_standardErr.append(str); break;
            }
        }
    }
    
    private static class StdInThread
    	extends Thread
    {
    	private OutputStream m_stream;
    	private String m_input;
    	
    	public StdInThread(Socket server, String input)
    		throws IOException
    	{
    		m_stream = server.getOutputStream();
    		m_input = input;
    	}
    	
    	public void run() {
    		// Write the data and close the stream by sending 0 length
    		try {
            	byte[] inbuf = m_input.getBytes();
            	System.out.println("Read "+inbuf.length+" bytes from stdin");
            	writeLongNbo(m_stream, inbuf.length);
            	m_stream.write(inbuf);
            	writeLongNbo(m_stream, 0);
    		} catch(IOException e) {
    			e.printStackTrace();    			
    		}
        	System.out.println("StdInThread Finished");
    	}
    }

    // REQUESTTYPE - from triipc.h
    public static final int ECHO_REQUEST             = 0;
    public static final int RUN_SERVER_SCRIPT        = 1;
    public static final int GET_DIALOG_DATA          = 2;
    public static final int RUN_VALIDATION_SCRIPT    = 3;
    public static final int RUN_FIELD_SCRIPT         = 4;
    public static final int RUN_LISTBOX_SCRIPT       = 5;
    public static final int SHUTDOWN_TRILOGY         = 6;
    public static final int DUMP_CLIENT_IP_ADDRESSES = 7;
    public static final int SHOW_HELP_TEXT           = 8;

    // REPLY_STATUS - from triipc.h
    public static final int REQUEST_FAILURE          = 0;
    public static final int REQUEST_SUCCESS          = 1;

    // DISPLAY_OPTION - trilib.h
    public static final int DISCARD                  = 0;
    public static final int DISPLAY                  = 1;
    public static final int POPUP                    = 2;
    public static final int REPORT                   = 3;
    public static final int CHOOSE                   = 4;
    public static final int FILENAME                 = 5;

    private TrilogyLib()
    {
    }

    private static int readLong(InputStream s) throws IOException
    {
        byte[] buf = new byte[4];
        if(s.read(buf) != 4)
        {
            System.out.println("Failed to read long fully.");
        }
        return (buf[0] & 0xff) << 24 | (buf[1] & 0xff) << 16
                | (buf[2] & 0xff) << 8 | buf[3] & 0xff;
    }

//    private static void writeLong(OutputStream s, int val) throws IOException
//    {
//        byte[] buf = new byte[4];
//        buf[0] = (byte) ((val >> 24) & 0xff);
//        buf[1] = (byte) ((val >> 16) & 0xff);
//        buf[2] = (byte) ((val >> 8) & 0xff);
//        buf[0] = (byte) (val & 0xff);
//        s.write(buf);
//    }
        
    /**
     * Write a long in network byte order (equivalent of htonl)
     */
    private static void writeLongNbo(OutputStream s, int val)
    		throws IOException
    {
    	s.write(ByteBuffer.allocate(4).putInt(val).order(ByteOrder.nativeOrder()).array());
    }
    
    public static class ObjectList
    {
        public int        objectID;
        public String     stringValue;
        public String     creatorName;
        // creationTime;
        public String     modifierName;
        // modifiedTime;
        public ObjectList next;

        public ObjectList()
        {
        }
    }

    public static class StringHolder
    {
        public String value;
    }

    public static class IntHolder
    {
        public int value;
    }

    public static class CharHolder
    {
        public char value;
    }

    /*
     * private static ObjectList getObjectListFromServer( Socket server ) { XDR
     * xdrreply = XDR.getIncomingXDRStream( server ); int numOfStrings;
     * ObjectList startOfList = null; ObjectList current = null; // // First
     * value is number of entries in list. // numOfStrings = xdrreply.getLong(); // //
     * Now loop, getting each string value in turn... // for( int i = 0; i <
     * numOfStrings; i++ ) { if( startOfList == null ) { // List not yet
     * initialised. startOfList = new ObjectList(); current = startOfList; }
     * else { current.next = new ObjectList(); current = current.next; }
     * 
     * current.objectID = xdrreply.getLong(); current.stringValue =
     * xdrreply.getString(); current.creatorName = xdrreply.getString();
     * xdrreply.getEncodedDate( /*&(current.CreationTime)
     *//*
         * ); current.modifierName = xdrreply.getString();
         * xdrreply.getEncodedDate( /*&(current.ModifiedTime)
         *//*
         * );
         * 
         * System.out.println( "Object[" + i + "] = ( " + current.objectID + "; " +
         * current.stringValue + "; " + current.creatorName + "; " +
         * current.modifierName + " )" ); }
         * 
         * return startOfList; }
         */

    public static Socket establishOutgoingConnection(int destination_port, String hostName)
    {
        try {
            System.out.println("Connecting to " + hostName + ":"
                    + destination_port + "...");

            Socket server = new Socket(hostName, destination_port);
            // server.setTcpNoDelay( true );
            return server;
        } catch(java.net.UnknownHostException e) {
            e.printStackTrace();
        } catch(java.io.IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public static int /* REPLY_STATUS */pingServerProcess(Socket server)
    {
        XDR xdrdata = XDR.newOutgoingXDRStream();

        int /* long */testNumber = 12345678;
        int /* REPLY_STATUS */status = REQUEST_FAILURE; // RHT - default to
        // failure - 24/08/2006

        xdrdata.setRequestType(ECHO_REQUEST);
        xdrdata.addLong(testNumber);
        xdrdata.sendToServer(server);

        System.out.println("Echo request sent");

        //
        // Read reply from Server
        //
        XDR replyData;

        // RHT EDIT BEGIN - changed to check result and only set Status if all
        // ok - 28/08/2006
        if((replyData = XDR.getIncomingXDRStream(server)) != null)
        {
            System.out.println("Echo reply received");

            if(replyData.length() == 0) {
                System.out.println("Body of reply is empty!");
                System.exit(1);
            }

            if(replyData.getServerStatus() == REQUEST_SUCCESS) {
                long incomingNumber = replyData.getLong();
                if(incomingNumber == testNumber)
                {
                    status = REQUEST_SUCCESS;
                }
            }
        }
        // RHT EDIT END - 28/08/2006
        return status;
    }

    public static Socket connectToServer(String serverName, int destination_port)
            throws TrilogyException
    {
        Socket server = establishOutgoingConnection(destination_port, serverName);

        if(server == null) {
            throw new TrilogyException(
                    "Connection to TrilogyServer process refused - TrilogyServer process running?");
            		// "Failed to connect to triserv process - unknown error"
        }

        System.out.println("Connected to port " + destination_port);

        //
        // Connection established - issue an ECHO_REQUEST to ensure the server
        // is responding correctly. TODO: This seems to break the connection,
        // so leave it out for now - possibly the server closes the connection
        // after the request.
        //
        // if(pingServerProcess(server) == REQUEST_FAILURE)
        // throw new Exception(
        // "TrilogyServer process is not responding correctly");

        return server;
    }
    
    private static XDR internalCreateRunRequest(String[] args, String[] fields)
    {
        XDR xdrData = XDR.newOutgoingXDRStream();

        xdrData.setRequestType(RUN_SERVER_SCRIPT);
        //
        // Add the Program ID
        //
        xdrData.addString(args[1]);
        //
        // Add client username
        //
        xdrData.addString("fred");		// TODO: find real username or at least pass it in!
        //
        // Add all the arguments.
        //
        xdrData.addLong(args.length - 2);
        for(int k = 2; k < args.length; k++) {
            xdrData.addString(args[k]);
        }
        //
        // Add the field values from any dialog
        //
        xdrData.addLong(fields.length);
        for(int k = 0; k < fields.length; k++) {
            xdrData.addString(fields[k]);
        }
        xdrData.addLong(0); // no selected rows
        xdrData.addLong(0); // no list box values
        xdrData.addLong(0); // no list box columns
        //
        // Passed field values
        //
        xdrData.addLong(0);
        //
        // Passed listbox values
        //
        xdrData.addLong(0); // no selected rows
        xdrData.addLong(0); // no list box values
        //
        // Add the node name
        //
        String nodeName = "node";
        xdrData.addString(nodeName);
        //
        // Client PID
        //
        xdrData.addLong(123);			// TODO: Get this from somewhere or find something unique we can use
        //
        // Standard input filename - this is actually a null string
        //
        xdrData.addString(null);
        
        return xdrData;
    }

    public static int sendRunRequest(
            String machineName,
            int portNumber,
            String[] args,
            String[] fields,
            // LISTBOXVALUES *ListBoxValues,
            StringHolder title, int inStreamNum, int outStreamNum,
            int errStreamNum, boolean scripting,
            IntHolder /* DISPLAY_OPTION */standardOutDisplayOption,
            IntHolder /* DISPLAY_OPTION */standardErrDisplayOption,
            CapturedData cd, IntHolder signalNum, IntHolder exitCode,
            StringHolder failReasonString) throws Exception
    {
        Socket server = connectToServer(machineName, portNumber);
        sendSignature(server);
        
        XDR xdrData = internalCreateRunRequest(args, fields);
        
        System.out.println("Sending run server script request");
        xdrData.sendToServer(server);

        //
        // Now loop, reading the data from the server side script, and
        // passing it our standard input (if required)
        //
        boolean firstTimeThrough = true;

        try
        {
            InputStream in = server.getInputStream();

            // Close stdin before we start as we can never send anything on it
            OutputStream out = server.getOutputStream();
            writeLongNbo(out, 0);

            do
            {
                // Standard Output/Error Streams
                if(firstTimeThrough)
                {
                    //
                    // First get the standard output and
                    // standard error flags
                    // (DISCARD, DISPLAY, POPUP)
                    //
                    int standardOutputFlag = readLong(in);
                    // System.out.println("standardOutputFlag = " + standardOutputFlag);
                    int standardErrorFlag = readLong(in);
                    // System.out.println("standardErrorFlag = " + standardErrorFlag);
                    /* int pid = */readLong(in);
                    // System.out.println("pid = " + pid);
                    /*int systemTray =*/ readLong(in);
                    /*int balloon =*/ readLong(in);
                    if(scripting)
                    {
                        // For scripting clients, override server-side
                        // trilogy.conf stdout= and stderr= directives and make
                        // sure everything gets captured. We don't want to do
                        // this if the standard output and standard error
                        // streams have been diverted to a file (via SetStream)
                        // so check to see if the streams have been diverted....
                        //
                        if(outStreamNum == 1)
                            standardOutputFlag = POPUP;
                        if(errStreamNum == 2)
                            standardErrorFlag = POPUP;
                    }
                    firstTimeThrough = false;
                    standardOutDisplayOption.value = /* (DISPLAY_OPTION) */standardOutputFlag;
                    standardErrDisplayOption.value = /* (DISPLAY_OPTION) */standardErrorFlag;
                    if((standardOutDisplayOption.value == REPORT)
                            || (standardErrDisplayOption.value == REPORT))
                    {
                        // Open up the real-time output
                        // reporting box.

                        // rb = OpenReportWindow();
                    }
                }
                int numBytes = readLong(in);
                // System.out.println("got " + numBytes + " bytes");
                if(numBytes > 0)
                {
                    byte[] buf = new byte[numBytes];
                    int streamNo = readLong(in);
                    // System.out.println("streamNo = " + streamNo);
                    /* int len = */in.read(buf);
                    //System.out.println("Read '" + new String(buf) + "'");
                    // System.out.println("len = " + len);
                    if(streamNo == 0)
                    {
                        // Standard Output
                        switch(standardOutDisplayOption.value)
                        {
                        case REPORT:
                            // WriteToReportWindow(rb,NumBytes,Buf);
                            break;
                        case DISPLAY:
                            // write(outStreamNum,Buf,NumBytes);
                            break;
                        case POPUP:
                            cd.addToCapture(CapturedData.OUT, buf);
                            break;
                        case DISCARD:
                            // Throw away
                            break;
                        }
                    }
                    else if(streamNo == 1)
                    {
                        // Standard Error
                        switch(standardErrDisplayOption.value)
                        {
                        case REPORT:
                            // WriteToReportWindow(rb,NumBytes,Buf);
                            break;
                        case DISPLAY:
                            // write(errStreamNum,Buf,NumBytes);
                            break;
                        case POPUP:
                            cd.addToCapture(CapturedData.ERR, buf);
                            break;
                        case DISCARD:
                            // Throw away
                            break;
                        }
                    }
                }
                else
                {
                    // Num Bytes = 0, no more data
                    break;
                }
                //
                // need to update report window here if its present
                //
            } while(true);
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }

        // System.out.println("Finished reading output.");

        //
        // Read exit codes from server-side job. Note, Windows platforms do
        // this
        // in the StdOutThread.
        //
        //
        XDR replyData = XDR.getIncomingXDRStream(server);

        int /* REPLY_STATUS */s = replyData.getServerStatus();
        if(s != REQUEST_SUCCESS)
        {
            // Get fail indication type
            failReasonString.value = replyData.getString();
            System.out.println("failReasonString = " + failReasonString.value);
            // cd->BytesOfStandardOut=0;
            // cd->BytesOfStandardErr=0;
            throw new TrilogyException(failReasonString.value);
        }
        else
        {
            // Get the exit status of the job
            failReasonString.value = null;
            signalNum.value = replyData.getLong();
            // System.out.println("signalNum = " + signalNum.value);
            exitCode.value = replyData.getLong();
            // System.out.println("exitCode = " + exitCode.value);
            title.value = replyData.getString();
            // System.out.println("title = " + title.value);
        }

        // closeOutgoingConnection(server);
        try
        {
            server.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }

        return s;
    }

    public static TrilogyJob sendRunRequest(
    		String machineName,
    		int portNumber,
    		String[] args,
    		String[] fields,
    		// LISTBOXVALUES *ListBoxValues,
    		String input,
    		ITrilogyOutputStream output)
    	throws Exception
    {
        Socket server = connectToServer(machineName, portNumber);
        sendSignature(server);
        
        XDR xdrData = internalCreateRunRequest(args, fields);
        
        System.out.println("Sending run server script request");
        xdrData.sendToServer(server);

        try
        {                      
            if(input != null) {
            	// Start a thread to feed the input, so we can use blocking reads
            	new StdInThread(server, input).start();
            } else {
            	// If we have no input, close stdin before we start as we never send anything on it
                OutputStream out = server.getOutputStream();
            	writeLongNbo(out, 0);
            }

            InputStream in = server.getInputStream();
            
            // Standard Output/Error Streams
            //
            // First get the standard output and
            // standard error flags
            // (DISCARD, DISPLAY, POPUP)
            //
            int standardOutputFlag = readLong(in);
            // System.out.println("standardOutputFlag = " + standardOutputFlag);
            int standardErrorFlag = readLong(in);
            // System.out.println("standardErrorFlag = " + standardErrorFlag);
            /* int pid = */readLong(in);
            // System.out.println("pid = " + pid);
            /*int systemTray =*/ readLong(in);
            /*int balloon =*/ readLong(in);
            
            return new TrilogyJob(server, standardOutputFlag, standardErrorFlag, output);
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
        
        // Try to close the connection
        try {
        	server.close();
        } catch(IOException e) {
            e.printStackTrace();        	
        }
        
        return null;
     }

    public static void listJobs(String serverName, int serverPort, CapturedData cd)
    	throws Exception
    {
        Socket server = connectToServer(serverName, serverPort);

        if(server != null)
        {
            XDR xdrdata = XDR.newOutgoingXDRStream();
            cd.addToCapture(1, "Jobs available on server " + serverName
                    + " (port " + serverPort + "):\n");

            xdrdata.setRequestType(SHOW_HELP_TEXT);
            xdrdata.sendToServer(server);
            System.out.println("Show help text reqest sent");
            //
            // Get the data back from the server
            //
            XDR replyData;
            if((replyData = XDR.getIncomingXDRStream(server)) != null)
            {
                System.out.println("Show help text reply received");
                String jobName = replyData.getString();
                while((jobName != null) && (jobName.length() > 0))
                {
                    String helpText = replyData.getString();
                    cd.addToCapture(1, jobName + "\n");
                    //
                    // if there is any help text, underline this job name and
                    // print the
                    // help text
                    //
                    if((helpText != null) && (helpText.length() > 0))
                    {
                        StringBuffer underline = new StringBuffer(jobName);
                        for(int n = 0; n < underline.length(); n++)
                        {
                            underline.setCharAt(n, '-');
                        }
                        cd
                                .addToCapture(1, underline + "\n" + helpText
                                        + "\n\n");
                    }
                    jobName = replyData.getString();
                }
                // ShowTextInReportWindow(OutputText);
            }
        }
        else
        {
            // MessageBox(NULL,"Could not connect to server","Trilogy
            // Error",MB_OK);
            System.out.println("Could not connect to server");
        }
    }
    
    private static void sendSignature(Socket server)
    		throws IOException
    {
    	// Send signature before we start
    	OutputStream out = server.getOutputStream();
    	writeLongNbo(out, 0x125abbcd);
    }
    
    public static class TrilogyJob {
    	private Socket m_server;
    	private int m_standardOutputFlag;
    	private int m_standardErrorFlag;
    	private ITrilogyOutputStream m_output;
    	private int m_signalNum;
    	private int m_exitCode;
        private String m_failReason;
    	
    	public TrilogyJob(Socket server, int stdOutFlag, int stdErrFlag, ITrilogyOutputStream output) {
    		m_server = server;
    		m_standardOutputFlag = stdOutFlag;
    		m_standardErrorFlag = stdErrFlag;
    		m_output = output;
    	}
    	
    	public int getSignalNum()  { return m_signalNum; }
    	public int getExitCode()  { return m_exitCode; }
    	public ITrilogyOutputStream getOutput()  { return m_output; }
    	
    	public int waitForCompletion()
    		throws TrilogyException
    	{
            try
            {            
        		InputStream in = m_server.getInputStream();
        		
                //
                // Now loop, reading the data from the server side script
                //
                do
                {
                    int numBytes = readLong(in);
                    /**/ System.out.println("got " + numBytes + " bytes");
                    if(numBytes > 0)
                    {
                        byte[] buf = new byte[numBytes];
                        int streamNo = readLong(in);
                        /**/ System.out.println("streamNo = " + streamNo); 
                        int len = in.read(buf);
                        System.out.println("len = " + len);
                        System.out.println("Read '" + new String(buf) + "'");
                        if(streamNo == 0) {
                            // Standard Output
                            switch(m_standardOutputFlag) {
                            case REPORT:
                            case DISPLAY:
                            case POPUP:
                                m_output.writeToStdOut(buf);
                                break;
                            case DISCARD:
                                // Throw away
                                break;
                            }
                        } else if(streamNo == 1) {
                            // Standard Error
                            switch(m_standardErrorFlag) {
                            case REPORT:
                            case DISPLAY:
                            case POPUP:
                            	m_output.writeToStdErr(buf);
                                break;
                            case DISCARD:
                                // Throw away
                                break;
                            }
                        }
                    } else {
                        // Num Bytes = 0, no more data
                        break;
                    }
                    //
                    // need to update report window here if its present
                    //
                } while(true);
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }

            /**/ System.out.println("Finished reading output.");

            //
            // Read exit codes from server-side job. Note, Windows platforms do
            // this
            // in the StdOutThread.
            //
            //
            XDR replyData = XDR.getIncomingXDRStream(m_server);
            
            if (replyData == null)
             throw new TrilogyException("Could not connect to " + m_server.getInetAddress().getHostName());
            
            int /* REPLY_STATUS */s = replyData.getServerStatus();
            if(s != REQUEST_SUCCESS) {
                // Get fail indication type
                m_failReason = replyData.getString();
                System.out.println("failReasonString = " + m_failReason);
                throw new TrilogyException(m_failReason);
            } else {
                // Get the exit status of the job
                m_signalNum = replyData.getLong();
                // System.out.println("signalNum = " + signalNum.value);
                m_exitCode = replyData.getLong();
                // System.out.println("exitCode = " + exitCode.value);
                /*String title =*/ replyData.getString();
                // System.out.println("title = " + title);
                System.out.println("m_signalNum="+m_signalNum+" m_exitCode="+m_exitCode);
            }

            // closeOutgoingConnection(server);
            try {
                m_server.close();
            } catch(IOException e) {
                e.printStackTrace();
            }

            return s;
    	}
    }
}
