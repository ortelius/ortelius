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
package dmadmin.model;
@SuppressWarnings("serial")
public class LoginException	extends Exception
{	
 private String msg;
 
	public enum LoginExceptionType {
		LOGIN_OKAY,
		LOGIN_USER_LOCKED,
		LOGIN_CHANGE_PASSWORD,
		LOGIN_BAD_PASSWORD,
  LOGIN_BAD_USER,
		LOGIN_DATABASE_FAILURE,
		LDAP_ERROR
	};
	
	public LoginExceptionType exceptionType;
	
	public LoginException(LoginExceptionType et, String msg) {
		exceptionType = et;
		this.msg = msg;
	}
	
	public LoginExceptionType getExceptionType() {
		return exceptionType;
	}

 public void setMessage(String msg)
 {
  this.msg = msg;
 }

 public String getMessage()
 {
  return this.msg;
 }
}
