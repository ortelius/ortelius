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
package com.openmakesoftware.XDR;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

/**
 * This class deals with XDR input/output. Copyright 2005-2006 Trinem Consulting
 * Limited ( www.trinem.com ) All rights reserved.
 * 
 * @author <a href="mailto:akeilhauer@trinem.com">Andreas Keilhauer</a>
 * 
 */
public class XDR
{

    /**
     * The length of a int in an xdr.
     */
    public final static int INT   = 4;

    /**
     * The byte array encoding.
     * Comment to Test CI
     */
    protected byte[]        buf;

    protected byte[][]      bufs;
    protected int[]         counts;
    protected int           currbuf;

    /**
     * Internal buffer length.
     */
    protected int           len;

    protected int           elementCount;

    /**
     * Current index in the encoding. Serializing and deserialzing automatically
     * increment this offset.
     */
    protected int           index = 0;

    /**
     * Creates a new xdr with a buffer of len bytes for encoding.
     * 
     * @param len
     *                the size of the buffer to create.
     */
    public XDR(int len)
    {
        this.buf = new byte[len];
        this.len = len;
        this.elementCount = 0;

        this.currbuf = -1;
        this.bufs = null;
        this.counts = null;
    }

    /**
     * Create a new xdr using the given buffer. Does NOT copy the given buf.
     * <p>
     * XXX probably shouldn't be public.
     * 
     * @param buf
     *                the buffer to use.
     */
    public XDR(byte[] buf)
    {
        this.buf = buf;
        this.len = buf.length;
        this.elementCount = 0;

        this.currbuf = -1;
        this.bufs = null;
        this.counts = null;
    }

    public XDR(byte[] buf, int count)
    {
        this.buf = buf;
        this.len = buf.length;
        this.elementCount = count;

        this.currbuf = -1;
        this.bufs = null;
        this.counts = null;
    }

    public XDR(byte[][] bufs, int[] counts)
    {
        this.bufs = bufs;
        this.counts = counts;

        this.currbuf = 0;
        this.buf = this.bufs[this.currbuf];
        this.elementCount = this.counts[this.currbuf];

        this.len = this.buf.length;
    }

    public byte[] getBuffer()
    {
        return this.buf;
    }

    public int length()
    {
        return this.len;
    }

    public int getElementCount()
    {
        return this.elementCount;
    }

    /**
     * Reset an xdr stream.
     * <p>
     * Package-access only.
     * 
     * @return For convenience, return Xdr stream that was reset.
     */
    XDR reset()
    {
        index = 0;
        len = buf.length;
        return this;
    }

    /**
     * Return the current index in the xdr.
     */
    int index()
    {
        return index;
    }

    /**
     * Return the number of bytes required t encode x in the xdr. Does not
     * actually encode the string. Handles null strings just fine.
     * <p>
     * The string had best be less than 256 characters long.
     * <p>
     * The string had best be in ASCII. (Or some other one-to-one encoding of
     * characters into bytes.)
     * 
     * @param x
     *                the string to check
     * @return the number of bytes required in the xdr
     */
    public final static int STRING(String x)
    {
        if (x == null)
            return INT;

        int offset = 0;
        switch ((INT + x.length()) % 4)
        {
        case 0:
            break;
        case 1:
            offset = 3;
            break;
        case 2:
            offset = 2;
            break;
        case 3:
            offset = 1;
            break;
        }

        return INT + x.length() + offset;
    }

    public void addChar(char x)
    {
        debug("XDR.addChar( " + (byte) x + " )");

        if (index + 1 > len)
            this.extendBuffer();

        buf[index++] = (byte) x;
        this.elementCount++;
    }

    public void addLong(int x)
    {
        debug("XDR.addLong( " + x + " )");

        while (index + INT > len)
            this.extendBuffer();

        int j = index;
        buf[j++] = (byte) (x >>> 24);
        buf[j++] = (byte) (x >>> 16);
        buf[j++] = (byte) (x >>> 8);
        buf[j] = (byte) x;
        this.index += INT;
        this.elementCount++;
    }

    public void addEnum(int x)
    {
        debug("XDR.addEnum( " + x + " )");
        this.addLong(x);
    }

    public void setRequestType(int reqtype)
    {
        debug("XDR.setRequestType( " + reqtype + " )");
        this.addLong(reqtype);
    }

    public void addString(String x)
    {
        while (index + STRING(x) > len)
        {
            this.extendBuffer();
        }
        
        this.addLong(1);        // Number of substrings - trilogy specific

        // int j = offset + index;
        if (x == null)
        {
            this.addLong(0);
        }
        else
        {
            int l = x.length();
            this.addLong(l);
            byte[] xbytes = x.getBytes();
            int j = index;
            System.arraycopy(xbytes, 0, buf, j, l);
            index += l;
        }

        this.pad();
        this.elementCount++;
    }

    public void addBytes(byte[] x, int start, int length)
    {
        debug("XDR.addBytes( [], " + start + ", " + length + " )");

        while (index + length > len)
            this.extendBuffer();

        int j = index;
        System.arraycopy(x, start, buf, j, length);
        index += length;
        this.elementCount += length;
    }

    // /////////////////////////////////////////////////////////////////////////

    public char getChar()
    {
        if ((this.currbuf != -1) && (this.elementCount == 0))
            nextBuffer();

        if (this.index + 1 > this.len)
            throw new IndexOutOfBoundsException("No more data left in xdr.");

        char ret = (char) buf[this.index++];
        this.elementCount--;

        debug("XDR.getChar() returning " + (byte) ret);
        return ret;
    }

    public int getLong()
    {
        if ((this.currbuf != -1) && (this.elementCount == 0))
            nextBuffer();

        if (index + INT > len)
            throw new IndexOutOfBoundsException("No more data left in xdr.");

        int j = index;
        this.index += INT;
        this.elementCount--;
        int ret = (buf[j++] & 0xff) << 24 | (buf[j++] & 0xff) << 16
                | (buf[j++] & 0xff) << 8 | buf[j] & 0xff;

        debug("XDR.getLong() returning " + ret);
        return ret;
    }

    public boolean getBool()
    {
        int ret = this.getLong();
        debug("XDR.getBool() returning " + ret);
        return (ret == 1) ? true : false;
    }

    public int getEnum()
    {
        int ret = this.getLong();
        debug("XDR.getEnum() returning " + ret);
        return ret;
    }

    public int getServerStatus()
    {
        int ret = this.getLong();
        debug("XDR.getReplyStatus() returning " + ret);
        return ret;
    }

    public String getString()
    {
        if ((this.currbuf != -1) && (this.elementCount == 0))
            nextBuffer();

        /*
         * Trilogy uses a Tcl/Tk compatible thing with substrings, whereas RTO
         * just outputs a single string.
         */
        if (index + INT /* + 1 */> len)
            throw new IndexOutOfBoundsException("No more data left in xdr.");

        int numSubStrings = getLong();
        debug("XDR.getString() numSubStrings " + numSubStrings);
       
        StringBuffer reply = new StringBuffer();

        for (int i = 0; i < numSubStrings; i++)
        {
            if (index + INT /* + 1 */> len)
                throw new IndexOutOfBoundsException("No more data left in xdr.");

            int l = getLong();
            int j = index;

            if (index + l > len)
                throw new IndexOutOfBoundsException("No more data left in xdr.");

            this.index += l;
            // this.elementCount--;

            if (l == 0)
                return null;

            String result = new String(buf, j, l);
            pad();
            debug("XDR.getString() substring " + i + ": \"" + result + "\"");
            reply.append(result);
        }

        debug("XDR.getString() returning \"" + reply + "\"");
        return reply.toString();
    }

    public void getBytes(byte[] x, int start, int length)
    {
        if ((this.currbuf != -1) && (this.elementCount == 0))
            nextBuffer();

        if (index + length > len)
            throw new IndexOutOfBoundsException("No more data left in xdr.");

        int j = index;
        System.arraycopy(buf, j, x, start, length);
        index += length;
        this.elementCount -= length;
    }

    public void getEncodedDate()
    {
        /* int year = */getLong();
        /* int month = */getLong();
        /* int day = */getLong();
        /* int hours = */getLong();
        /* int mins = */getLong();
        /* int secs = */getLong();
    }

    protected boolean nextBuffer()
    {
        if (this.currbuf == -1)
            return false;

        if ((this.bufs == null) || (this.counts == null)
                || (this.currbuf >= this.bufs.length))
            throw new IndexOutOfBoundsException("No more buffers left in xdr.");

        this.buf = this.bufs[++this.currbuf];
        this.elementCount = this.counts[this.currbuf];
        reset();

        debug("buffer[" + this.currbuf + "] (" + this.length() + "):");
        this.dumpBuffer(this.length());

        return true;
    }

    // /////////////////////////////////////////////////////////////////////////

    public final void skip(int count)
    {
        if (index + count > len)
            throw new IndexOutOfBoundsException("No more data left in xdr.");

        index += count;
    }

    public void pad()
    {
        switch (index % 4)
        {
        case 0:
            skip(0);
            break;
        case 1:
            skip(3);
            break;
        case 2:
            skip(2);
            break;
        case 3:
            skip(1);
            break;
        }
    }

    private void extendBuffer()
    {
        this.len = this.len * 2;
        byte[] newBuffer = new byte[this.len];
        System.arraycopy(this.buf, 0, newBuffer, 0, this.index);
        this.buf = newBuffer;
    }

    public static XDR getIncomingXDRStream(Socket server)
    {
        byte[] replyHeader = new byte[32];

        DataInputStream in = null;
        try
        {
            in = new DataInputStream(server.getInputStream());
        }
        catch (IOException e1)
        {
            // TODO Auto-generated catch block
            e1.printStackTrace();
        }

        try
        {
            in.readFully(replyHeader);
        }
        catch (IOException ioe)
        {
            ioe.printStackTrace();
            return null;
        }

        XDR replyHeaderXDR = new XDR(replyHeader, 4);

        debug("\nReplyHeader (" + replyHeaderXDR.length() + "):");
        replyHeaderXDR.dumpBuffer(replyHeaderXDR.length());

        // header
        // We do not have to deal with more than one buffers, because
        // we just read everything in one go.
        int numberOfBuffers = replyHeaderXDR.getLong(); // numberOfBuffers
        int replySeed = replyHeaderXDR.getLong(); // encryption
        
        if(numberOfBuffers == 0) {
        	System.out.println("WARNING: numberOfBuffers is zero");
        	return null;
        }

        int sizeOfData = replyHeaderXDR.getLong(); // bufferLength
        int numberOfItems = replyHeaderXDR.getLong(); // numberOfItems (not necessary)
        
        replyHeaderXDR.skip(16);

        if (numberOfBuffers != 1)
            System.out.println("WARNING: numberOfBuffers = " + numberOfBuffers);

        if (numberOfBuffers == 1)
        {
            debug("numberOfBuffers = " + numberOfBuffers + "; replySeed = "
                    + replySeed + "; sizeOfData = " + sizeOfData
                    + "; numberOfItems = " + numberOfItems);

            byte[] replyData = new byte[sizeOfData];
            try
            {
                in.readFully(replyData);
            }
            catch (IOException e1)
            {
                // TODO Auto-generated catch block
                e1.printStackTrace();
                return null;
            }

            EncryptedXDR replyDataXDR = new EncryptedXDR(replyData);

            debug("\nReplyData (" + replyDataXDR.length() + "):");
            replyDataXDR.dumpBuffer(replyDataXDR.length());

            replyDataXDR.startDecoding(replySeed);

            return replyDataXDR;
        }

        debug("numberOfBuffers = " + numberOfBuffers + "; replySeed = "
                + replySeed);

        byte[][] buffers = new byte[numberOfBuffers][];
        int[] counts = new int[numberOfBuffers];

        for (int n = 0; n < numberOfBuffers; n++)
        {
            debug("buffer[" + n + "]: " + "sizeOfData = " + sizeOfData
                    + "; numberOfItems = " + numberOfItems);

            buffers[n] = new byte[sizeOfData];
            counts[n] = numberOfItems;
            try
            {
                in.readFully(buffers[n]);
            }
            catch (IOException e1)
            {
                // TODO Auto-generated catch block
                e1.printStackTrace();
                return null;
            }

            if (n + 1 < numberOfBuffers)
            {
                try
                {
                    in.readFully(replyHeader);
                }
                catch (IOException ioe)
                {
                    ioe.printStackTrace();
                    return null;
                }

                replyHeaderXDR = new XDR(replyHeader, 2);

                debug("\nReplyHeader (" + replyHeaderXDR.length() + "):");
                replyHeaderXDR.dumpBuffer(replyHeaderXDR.length());

                sizeOfData = replyHeaderXDR.getLong(); // bufferLength
                numberOfItems = replyHeaderXDR.getLong(); // numberOfItems
                // (not necessary)
                replyHeaderXDR.skip(24);
            }
        }

        EncryptedXDR replyDataXDR = new EncryptedXDR(buffers, counts);

        debug("\nReplyData (" + replyDataXDR.length() + "):");
        replyDataXDR.dumpBuffer(replyDataXDR.length());

        replyDataXDR.startDecoding(replySeed);

        return replyDataXDR;
    }

    public static XDR newOutgoingXDRStream()
    {
        return new XDR(1000);
    }

    public void sendToServer(Socket server)
    {
        EncryptedXDR request;
        int encryptionKey;
        /* int res; */

        encryptionKey = (int) (System.currentTimeMillis() & 0x7fffffff); // 1234;

        //
        // First we have to send a count of the number of XDR encoded
        // streams are about to follow. Just to complicate matters, this
        // needs to be sent as an XDR encoded number...
        //
        //	
        // Allocate enough space to hold 4 encoded "longs"
        // (num buffers + encryption key + buffer size + number of items)
        //
        /* int bufferSize = INT*4; */

        request = new EncryptedXDR(1000 /* bufferSize */);
        request.addLong(1 /* NumberOfBuffers */);
        request.addLong(encryptionKey);
        request.addLong(this.index);
        request.addLong(this.elementCount);
        request.skip(16);
        request.startEncoding(encryptionKey);
        request.addBytes(this.buf, 0, this.index);
        request.endEncoding();

        debug("\nBuffer (" + this.index() + "/" + this.length() + "):");
        this.dumpBuffer(this.index());

        debug("\nRequest (" + request.index() + "/" + request.length() + "):");
        request.dumpBuffer(request.index());

        debug("\nSending " + request.index() + " bytes");

        // SendDataToServer( Server, TransmissionData,
        // BufferSize+xPtr->BufferLength );

        try
        {
            OutputStream out = server.getOutputStream();
            out.write(request.getBuffer(), 0, request.index());
            /**/ out.flush();
        }
        catch (IOException e)
        {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    protected void dumpBuffer(int len)
    {
        if (s_debug)
        {
            for (int y = 0; y < len; y += 16)
            {
                for (int x = 0; x < 16; x++)
                {
                    if (y + x < len)
                        System.out.printf("%02x ", this.buf[y + x]);
                    else
                        System.out.printf("   ");
                }

                System.out.print("    ");

                for (int x = 0; (x < 16) && (y + x < len); x++)
                {
                    if (this.buf[y + x] >= 32 && this.buf[y + x] < 127)
                        System.out.printf("%c", this.buf[y + x]);
                    else
                        System.out.print(".");
                }

                System.out.println();
            }
        }
    }

    private static boolean s_debug = false;

    protected static void debug(String message)
    {
        if(s_debug) {
            System.out.println(message);
        }
    }
}
