package com.trinem.dmdemo;

public class User
{
	private String m_firstname;
	private String m_surname;
	
	public User(String firstname, String surname)
	{
		m_firstname = firstname;
		m_surname = surname;
	}
	
	public String getFirstname()
	{
		return m_firstname;
	}
	
	public String getSurname()
	{
		return m_surname;
	}
}