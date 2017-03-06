package com.trinem.dmdemo;

import java.util.Date;

public class User
{
	private String m_firstname;
	private String m_surname;
	private Date m_dob;
	
	public User(String firstname, String surname, Date dob)
	{
		m_firstname = firstname;
		m_surname = surname;
		m_dob = dob;
	}
	
	public String getFirstname()
	{
		return m_firstname;
	}
	
	public String getSurname()
	{
		return m_surname;
	}
	
	public Date getDOB()
	{
		return m_dob;
	}
}