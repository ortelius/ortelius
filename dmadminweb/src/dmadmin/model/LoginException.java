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
