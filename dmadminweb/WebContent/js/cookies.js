function SetCookie(name, value) {
	console.log("SetCookie("+name+","+value+")");
	var today = new Date();
	today.setTime( today.getTime() );
	expires = 86400000;	// 1 day in millisecs
	var expires_date = new Date(today.getTime()+(expires));
	// document.cookie = name + "=" + escape(value) + "; expires="+expires_date.toGMTString()+"; path=/";
	document.cookie = name + "=" + escape(value);
	console.log(name + "=" + escape(value) + "; expires="+expires_date.toGMTString()+"; path=/");
}

function GetCookie(check_name) {
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
		console.log("a_all_cookies["+i+"]="+a_all_cookies[i]);
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
				console.log("cookie_value stripping quotes");
				cookie_value = unescape( a_all_cookies[i].substr(e+1).replace(/^\s+|\s+$/g, '') );
				if (cookie_value.charAt(0)=='"') {
					console.log("cookie_value = "+cookie_value);
					cookie_value = cookie_value.substr(1,cookie_value.length-2);
					console.log("cookie_value now "+cookie_value);
				}
			}
			// note that in cases where cookie is initialized but no value, null is returned
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