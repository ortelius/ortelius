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

public class EncryptedXDR extends XDR
{
    private boolean encoding = false;

    private boolean decoding = false;

    private int     seed;

    public EncryptedXDR(int len)
    {
        super(len);
        this.encoding = false;
        this.decoding = false;
    }

    public EncryptedXDR()
    {
        super(1000);
        this.encoding = false;
        this.decoding = false;
    }

    public EncryptedXDR(byte[] buf)
    {
        super(buf);
        this.encoding = false;
        this.decoding = false;
    }

    public EncryptedXDR(byte[][] bufs, int[] counts)
    {
        super(bufs, counts);
        this.encoding = false;
        this.decoding = false;
    }

    public void addString(String x)
    {
        if (this.encoding)
        {
            int oldIndex = this.index;
            this.encoding = false;
            super.addString(x);
            this.encoding = true;
            Encryption.Encrypt(this.buf, oldIndex, this.index);
        }
        else
        {
            super.addString(x);
        }
    }

    public void addLong(int x)
    {
        debug("EncryptedXDR.addLong( " + x + " )");
        if (this.encoding)
        {
            int oldIndex = this.index;
            super.addLong(x);
            Encryption.Encrypt(this.buf, oldIndex, this.index);
        }
        else
        {
            super.addLong(x);
        }
    }

    public void addBytes(byte[] x, int start, int length)
    {
        debug("EncryptedXDR.addBytes( [], " + start + ", " + length + " )");
        if (this.encoding)
        {
            int oldIndex = this.index;
            super.addBytes(x, start, length);
            Encryption.Encrypt(this.buf, oldIndex, this.index);
        }
        else
        {
            super.addBytes(x, start, length);
        }
    }

    // /////////////////////////////////////////////////////////////////////////

    public int getLong()
    {
        if (this.decoding)
        {
            if ((this.currbuf != -1) && (this.elementCount == 0))
                nextBuffer();
            Encryption.Decrypt(this.buf, index, index + INT);
            return super.getLong();
        }
        else
        {
            return super.getLong();
        }
    }

    public String getString()
    {
        if (this.decoding)
        {
            if ((this.currbuf != -1) && (this.elementCount == 0))
                nextBuffer();

            /*
             * Trilogy uses a Tcl/Tk compatible thing with substrings, whereas
             * RTO just outputs a single string.
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

                int l = this.getLong();
                int j = index;

                if (index + l > len)
                    throw new IndexOutOfBoundsException(
                            "No more data left in xdr.");

                this.index += l;
                String result = null;
                if (l == 0)
                    return null;

                pad();

                Encryption.Decrypt(this.buf, j, this.index);

                result = new String(buf, j, l);
                debug("EncryptedXDR.getString() substring " + i + ": \""
                        + result + "\"");
                reply.append(result);
            }
            debug("EncryptedXDR.getString() returning \"" + reply + "\"");
            return reply.toString();
        }
        else
        {
            return super.getString();
        }
    }

    public void getBytes(byte[] x, int start, int length)
    {
        if (this.decoding)
        {
            if ((this.currbuf != -1) && (this.elementCount == 0))
                nextBuffer();
            Encryption.Decrypt(this.buf, index, index + INT);
            super.getBytes(x, start, length);
        }
        else
        {
            super.getBytes(x, start, length);
        }
    }

    public boolean isDecoding()
    {
        return decoding;
    }

    public void startDecoding(int seed)
    {
        debug("EncryptedXDR.startDecoding( " + seed + " )");
        this.seed = seed;
        Encryption.sgenrand(seed);
        this.decoding = true;
    }

    public void endDecoding()
    {
        this.decoding = false;
    }

    public boolean isEncoding()
    {
        return encoding;
    }

    public void startEncoding(int seed)
    {
        debug("EncryptedXDR.startEncoding( " + seed + " )");
        this.seed = seed;
        Encryption.sgenrand(seed);
        this.encoding = true;
    }

    public void endEncoding()
    {
        this.encoding = false;
    }

    protected boolean nextBuffer()
    {
        boolean ret;

        if ((ret = super.nextBuffer()))
        {
            if (this.decoding || this.encoding)
                Encryption.sgenrand(seed);
        }

        return ret;
    }

    // public byte[] getInitialTrinemOutput(int numberOfBuffers, int
    // encryptionKey) {
    // int tmpLength = this.index;
    // XDR tmp = new XDR(tmpLength + 32);
    //
    // tmp.addLong(numberOfBuffers);
    // tmp.addLong(encryptionKey);
    // tmp.addLong(tmpLength);
    // tmp.addLong(this.getElementCount());
    // tmp.skip(16);
    // tmp.addBytes(this.buf, 0, this.index);
    // return tmp.getBuffer();
    // }

    // public byte[] getTrinemOutput() {
    // int tmpLength = this.index;
    // XDR tmp = new XDR(tmpLength + 16);
    // tmp.addLong(tmpLength);
    // tmp.addLong(this.getElementCount());
    // tmp.addBytes(this.buf, 16, this.len);
    // return tmp.getBuffer();
    // }
}
