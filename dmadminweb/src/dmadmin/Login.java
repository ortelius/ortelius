/*
 *
 *  Ortelius for Microservice Configuration Mapping
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
package dmadmin;

import java.io.IOException;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
// import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.model.Domain;
import dmadmin.model.LoginException;

/**
 * Servlet implementation class Login
 */
public class Login extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public Login() {
        super();
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
  try (DMSession so = DMSession.getInstance(request)) {
//   Enumeration<String> parameterNames = request.getParameterNames();
//   while (parameterNames.hasMoreElements()) {
//       String paramName = parameterNames.nextElement();
//       String value = request.getParameter(paramName);
//       System.out.println("XXX");
//   }
		request.setAttribute("firstinstall",so.firstInstall());
		request.getRequestDispatcher("/WEB-INF/Home.jsp").forward(request, response);
  }	
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	 try (DMSession so = DMSession.getInstance(request)) {
		HttpSession hs = request.getSession();
		
  String isSaas = "N";
		String username=request.getParameter("username");
        String password=request.getParameter("password");
        String provider = request.getParameter("provider");
        
        if (provider == null)
         provider = "";
        
//        Cookie loggedinUser = new Cookie("p1",username);
//        Cookie loggedinPw   = new Cookie("p2",password);
//        
//        loggedinUser.setDomain("deployhub.com");    loggedinUser.setPath("/");
//        loggedinPw.setDomain("deployhub.com");      loggedinPw.setPath("/");
//        
//        response.addCookie(loggedinUser);
//        response.addCookie(loggedinPw);
        
        String initial=request.getParameter("initial");
        boolean initialInstall=false;
        if (initial!=null && initial.equalsIgnoreCase("Y")) {
        	// Initial admin password creation.
        	initialInstall=true;
        }
        
        if (so.isSaaS(so.GetConnection()))
         isSaas = "Y";
        
        if (provider.length() > 0)
         password = provider + ":" + password;
        
        LoginException loginres = initialInstall?so.InitialLogin(password):so.Login(username,password,null);
        String userdatefmt = so.GetUserDateFormat();
    	String usertimefmt = so.GetUserTimeFormat();
     String userdom = "";
     int userdomid = 0;
     try
     {
      Domain dom = so.getUserDomain();
      userdom = dom.getFullDomain();
      userdomid = dom.getId();
     }
     catch (RuntimeException re)
     {
      
     }
    	System.out.println("login returns "+loginres.getExceptionType());
        switch(loginres.getExceptionType()) {
        case LOGIN_OKAY:
        	hs.setAttribute("session", so);
        	hs.setAttribute("pwchange",false);
        	so.setWebHostName(request);
        	
        	String TargetPage;
        	if (ServletUtils.GetCookie(request,"loggedin")!=null) {
        		// We were already logged in and have come back in via a session timeout.
        		// The explorer tree(s) will refresh from their own cookies and restore
        		// the main panes so just redirect to Home and let that happen
        		TargetPage="Home";
        	} else {
        		// We've not previously been logged in - is "origurl" cookie set? If so, then
        		// we've been redirected to here from a direct link
        		TargetPage=ServletUtils.GetCookie(request,"origurl");
        		if (TargetPage == null) {
        			// no original URL - just redirect to Home (must be initial login)
        			TargetPage="Home";
        		}
        	}
        	// Set the "loggedin" cookie
//        	Cookie loggedinCookie = new Cookie("loggedin","Y");
//        	Cookie loggedinSession = new Cookie("session",so.GetSessionId());
//        	loggedinUser = new Cookie("p1",username);
//        	loggedinPw   = new Cookie("p2",password);
//        	String srv = "console.deployhub.com";
//        	
//        	loggedinCookie.setDomain(null); 
//         loggedinSession.setDomain(null); 
//         loggedinUser.setDomain(srv); 
//         loggedinPw.setDomain(srv);  
//        	
//        	loggedinCookie.setPath("/");
//        	loggedinSession.setPath("/");
//        	loggedinUser.setPath("/");  
//        	loggedinPw.setPath("/");
//         
//			response.addCookie(loggedinCookie);
//			response.addCookie(loggedinSession);
//   response.addCookie(loggedinUser);
//   response.addCookie(loggedinPw);
//   

			// And redirect to the correct page
        	hs.setAttribute("logininfo","Login OK");
        	Cookie loggedinTime = new Cookie("p3", new Long(new Date().getTime()).toString());
        	response.addCookie(loggedinTime);
        	
        	if (so.getAclOverride()) {
        		// Administrator login
        		response.getWriter().write("{\"Msg\": \"Login Admin\", \"domainid\": \""+userdomid+"\", \"domain\": \""+userdom+"\", \"datefmt\": \""+userdatefmt+"\", \"timefmt\": \""+usertimefmt+"\", \"issaas\": \""+isSaas+"\", \"lictype\": \""+so.getLicType()+"\", \"liccnt\": \""+so.getLicCnt()+"\", \"newuser\": \""+so.getNewUser()+"\", \"domlist\": \""+so.getDomainList()+"\"}");
        	} else {
        		// Normal (non admin) login
        		response.getWriter().write("{\"Msg\": \"Login OK\", \"domainid\": \""+userdomid+"\", \"domain\": \""+userdom+ "\",\"datefmt\": \""+userdatefmt+"\", \"timefmt\": \""+usertimefmt+"\", \"issaas\": \""+isSaas+"\", \"lictype\": \""+so.getLicType()+"\", \"liccnt\": \""+so.getLicCnt()+"\", \"newuser\": \""+so.getNewUser()+"\", \"domlist\": \""+so.getDomainList()+"\"}");
        	}
        	break;
        case LOGIN_BAD_PASSWORD:
        	System.out.println("BAD PASSWORD");
        	hs.setAttribute("logininfo","Invalid Username/Password");
        	response.getWriter().write("{\"Msg\":\"Invalid Username/Password\", \"datefmt\": \"\", \"timefmt\":\"\"}");
            break;
        case LOGIN_BAD_USER:
         System.out.println("BAD USER");
         hs.setAttribute("logininfo","Invalid Username/Password");
         response.getWriter().write("{\"Msg\":\"Invalid Username/Password\", \"datefmt\": \"\", \"timefmt\":\"\"}");
            break;    
        case LOGIN_USER_LOCKED:
        	hs.setAttribute("logininfo","User Account is Locked");
        	response.getWriter().write("{\"Msg\": \"User Account is Locked\", \"domainid\": \""+userdomid+"\", \"domain\": \""+userdom+"\", \"datefmt\": \""+userdatefmt+"\", \"issaas\": \""+isSaas+"\", \"lictype\": \""+so.getLicType()+"\", \"liccnt\": \""+so.getLicCnt()+"\", \"timefmt\": \""+usertimefmt+"\"}");
            break;
        case LOGIN_DATABASE_FAILURE:
        	hs.setAttribute("logininfo","Failed to connect to Database<BR>" + loginres.getMessage());
        	response.getWriter().write("{\"Msg\": \"Failed to connect to Database<BR>" + loginres.getMessage()+"\", \"datefmt\": \"\", \"timefmt\":\"\"}");
            break;
        case LOGIN_CHANGE_PASSWORD:
        	hs.setAttribute("session", so);
        	hs.setAttribute("pwchange",true);
        	so.setWebHostName(request);
        	
        	response.getWriter().write("{\"Msg\": \"Password must be changed\", \"domainid\": \""+userdomid+"\", \"domain\": \""+userdom+"\", \"datefmt\": \""+userdatefmt+"\", \"timefmt\": \""+usertimefmt+"\", \"issaas\": \""+isSaas+"\", \"lictype\": \""+so.getLicType()+"\", \"liccnt\": \""+so.getLicCnt()+"\", \"newuser\": \""+so.getNewUser()+"\"}");
            break;
        case LDAP_ERROR:
        	System.out.println("LDAP ERROR: loginres.getMessage()="+loginres.getMessage());
        	hs.setAttribute("logininfo","LDAP Error<BR>" + loginres.getMessage());
        	response.getWriter().write("{\"Msg\": \"LDAP Error<BR>" + loginres.getMessage()+"\", \"datefmt\": \"\", \"timefmt\":\"\"}");
        	break;
		}
	 }
	}
}
