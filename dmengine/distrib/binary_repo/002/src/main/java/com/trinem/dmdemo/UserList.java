package com.trinem.dmdemo;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

public class UserList
{
	public static Collection<User> getUsers(String serverName)
		throws Exception
	{
		ArrayList<User> users = new ArrayList<User>();
		
		try
		{
			Connection conn = getConnection(serverName);
			
			Statement stmt = conn.createStatement();
			
			ResultSet rset = stmt.executeQuery(
				"SELECT firstname, surname, dob FROM users" );
			
			while(rset.next())
			{
				String firstname = rset.getString("firstname");
				String surname = rset.getString("surname");
				Date dob = rset.getDate("dob");
				users.add(new User(firstname, surname, dob));
			}
			
			rset.close();
			stmt.close();
			conn.close();
		}
		catch(Exception e)
		{
			e.printStackTrace(System.err);
			throw e;
		}
		
		return users;
	}
	
	public static java.sql.Connection getConnection(String serverName)
		throws Exception
	{
		ResourceBundle bundle = null;
		try {
			bundle = ResourceBundle.getBundle("dmdemo_" + serverName);
		} catch(MissingResourceException e2) {}
		if(bundle == null) {
			try {
				bundle = ResourceBundle.getBundle("dmdemo");
			} catch(MissingResourceException e2) {}
		}
		if(bundle == null) {
			throw new Exception("Unable to find resource bundle for server " + serverName + ".");
		}
		
		Class.forName(bundle.getString("dmdemo.driverClass"));
		
		return DriverManager.getConnection(
				bundle.getString("dmdemo.connectString"),
				bundle.getString("dmdemo.dbUser"),
				bundle.getString("dmdemo.dbPass")
			 );

	}
}