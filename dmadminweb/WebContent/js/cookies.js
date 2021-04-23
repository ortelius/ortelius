function SetCookie(name, value) {
 value = decodeURIComponent(value);
	console.log("SetCookie("+name+","+value+")");
 var d = $(location).attr('hostname');
	$.cookie(name,value, {path: '/', domain: d});
}

function DeleteCookies() {
 DeleteCookie("admin");
 DeleteCookie("p1");
 DeleteCookie("p2");
 DeleteCookie("p3");
 DeleteCookie("token");
 DeleteCookie("loggedin");
 DeleteCookie("logindata");
 window.location = "/dmadminweb/Home";
}

function DeleteCookie(name) {
 console.log("DeleteCookie("+name+")");
 var d = $(location).attr('hostname');
 $.removeCookie(name,{path: '/', domain: d});
 $.removeCookie(name,{path: '/dmadminweb', domain: d});
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
