/**
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

function SetCookie(name, value) {
 value = decodeURIComponent(value);
	console.log("SetCookie("+name+","+value+")");
 var d = $(location).attr('hostname');
	$.cookie(name,value, {path: '/', domain: d});
}

function DeleteCookies(redirect) {
 DeleteCookie("admin");
 DeleteCookie("p1");
 DeleteCookie("p2");
 DeleteCookie("p3");
 DeleteCookie("token");
 DeleteCookie("loggedin");
 DeleteCookie("logindata");
 if (redirect == "")
   window.location = "/dmadminweb/Home";
  else
   window.location = redirect;
}

function DeleteCookie(name) {
 console.log("DeleteCookie("+name+")");

 var days = -1;
 var value = "";
 var date = new Date();
 date.setTime(date.getTime()+(days*24*60*60*1000));
 var expires = "; expires="+date.toGMTString();
 document.cookie = name+"="+value+expires+"; path=/";
 document.cookie = name+"="+value+expires+"; path=/dmadminweb";
}

function GetCookie(check_name) {
 console.log("GetCookie("+check_name+")");
 console.log(document.cookie);
	// first we'll split this cookie up into name/value pairs
	// note: document.cookie only returns name=value, not the other components
	var a_all_cookies = document.cookie.split( ';' );
	var a_temp_cookie = '';
	var cookie_name = '';
	var cookie_value = '';
	var b_cookie_found = false; // set boolean t/f default f

	for ( i = 0; i < a_all_cookies.length; i++ )
	{
		// now we'll split apart each name=value pair
		e=a_all_cookies[i].indexOf("=");
		a_temp_cookie = a_all_cookies[i].substr(0,e);
		cookie_name = a_all_cookies[i].substr(0,e).replace(/^\s+|\s+$/g, '');

		// if the extracted name matches passed check_name
		if ( cookie_name == check_name )
		{
			b_cookie_found = true;
			// we need to handle case where cookie has no value but exists (no = sign, that is):
			if ( a_temp_cookie.length > 1 )
			{
				cookie_value = unescape( a_all_cookies[i].substr(e+1).replace(/^\s+|\s+$/g, '') );
				if (cookie_value.charAt(0)=='"') {
					console.log("cookie_value = "+cookie_value);
					cookie_value = cookie_value.substr(1,cookie_value.length-2);
					console.log("cookie_value now "+cookie_value);
				}
			}
			// note that in cases where cookie is initialized but no value, null is returned
			cookie_value = encodeURIComponent(cookie_value);
			console.log("GetCookieRet("+check_name+","+cookie_value+")");
			return cookie_value;
			break;
		}
		a_temp_cookie = null;
		cookie_name = '';
	}
	if ( !b_cookie_found )
	{
		return null;
	}
}
