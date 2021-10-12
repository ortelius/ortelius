/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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