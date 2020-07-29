package com.openmakesoftware.trilogy;

public interface ITrilogyOutputStream
{
	void writeToStdOut(byte[] buf);
	
	void writeToStdErr(byte[] buf);
	
	StringBuffer getBuffer();
}
