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

package dmadmin;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.URLEncoder;
import java.util.Date;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
// import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.pac4j.core.config.Config;
import org.pac4j.core.context.WebContext;
import org.pac4j.core.context.session.SessionStore;
import org.pac4j.core.profile.ProfileManager;
import org.pac4j.core.util.FindBest;
import org.pac4j.jee.context.JEEContextFactory;
import org.pac4j.jee.context.session.JEESessionStore;
import org.pac4j.saml.client.SAML2Client;

import dmadmin.json.JSONArray;
import dmadmin.model.Domain;
import dmadmin.model.JWTGenerateValidateRSA;
import dmadmin.model.LoginException;
import dmadmin.model.LoginException.LoginExceptionType;
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;

/**
 * Servlet implementation class Login
 */
public class Login extends HttpServlet
{
 private final SAML2ClientBuilder saml2ClientBuilder = new SAML2ClientBuilder();

 private static final long serialVersionUID = 1L;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public Login()
 {
  super();
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  try (DMSession so = DMSession.getInstance(request))
  {

   // Enumeration<String> parameterNames = request.getParameterNames();
   // while (parameterNames.hasMoreElements()) {
   // String paramName = parameterNames.nextElement();
   // String value = request.getParameter(paramName);
   // System.out.println("XXX");
   // }
   String IdpMetadata = System.getenv("SamlIdpMetadata");
   ProfileManager profileManager = null;

   if (IdpMetadata != null)
   {
    String dhurl = saml2ClientBuilder.getCurrentUrl(request);
    SAML2Client client = saml2ClientBuilder.build(dhurl);
    Config config = new Config(client);
    SessionStore bestSessionStore = FindBest.sessionStore(null, config, JEESessionStore.INSTANCE);

    WebContext context = FindBest.webContextFactory(null, config, JEEContextFactory.INSTANCE).newContext(request, response);

    profileManager = new ProfileManager(context, bestSessionStore);
   }

   if (IdpMetadata != null && !IdpMetadata.trim().isEmpty() && profileManager != null && !profileManager.isAuthenticated())
   {
    HttpSession session = request.getSession();
    session.setAttribute("redirecturl", request.getRequestURL().toString());
    RequestDispatcher dispatcher = getServletContext().getRequestDispatcher("/sso");
    dispatcher.forward(request, response);
    return;
   }

   request.setAttribute("firstinstall", so.firstInstall());
   request.getRequestDispatcher("/WEB-INF/Home.jsp").forward(request, response);
  }
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  response.setHeader("Content-Type", "application/json");

  try (DMSession so = DMSession.getInstance(request))
  {
   HttpSession hs = request.getSession();

   String isSaas = "N";
   String username = request.getParameter("username");
   String password = request.getParameter("password");
   String provider = request.getParameter("provider");

   if (provider == null)
    provider = "";

   // Cookie loggedinUser = new Cookie("p1",URLEncoder.encode(username));
   // Cookie loggedinPw = new Cookie("p2",password);
   //
   // loggedinUser.setDomain("deployhub.com"); loggedinUser.setPath("/");
   // loggedinPw.setDomain("deployhub.com"); loggedinPw.setPath("/");
   //
   // response.addCookie(loggedinUser);
   // response.addCookie(loggedinPw);

   String initial = request.getParameter("initial");
   boolean initialInstall = false;
   if (initial != null && initial.equalsIgnoreCase("Y"))
   {
    // Initial admin password creation.
    initialInstall = true;
   }

   if (so.isSaaS(so.GetConnection()))
    isSaas = "Y";

   if (provider.length() > 0)
    password = provider + ":" + password;

   boolean authorized = false;
   String jwt = ServletUtils.GetCookie(request, "token");

   try
   {
    if (jwt != null && jwt.trim().length() > 0)
    {
     Jws<Claims> token = JWTGenerateValidateRSA.parseJwt(jwt);
     String uuid = token.getBody().getId();
     String userid = token.getBody().getSubject();
     authorized = so.validateAuth(Integer.parseInt(userid), uuid);
    }
   }
   catch (Exception e)
   {

   }

   LoginException loginres = new LoginException(LoginExceptionType.LOGIN_OKAY,"");

   if (!authorized)
    loginres = initialInstall ? so.InitialLogin(password) : so.Login(username, password, null);

   String userdatefmt = so.GetUserDateFormat();
   String usertimefmt = so.GetUserTimeFormat();
   String userdom = "";
   int userdomid = 0;
   int userid = so.GetUserID();
   try
   {
    Domain dom = so.getUserDomain();
    userdom = dom.getFullDomain();
    userdomid = dom.getId();
   }
   catch (RuntimeException re)
   {

   }
   System.out.println("login returns " + loginres.getExceptionType());
   switch (loginres.getExceptionType())
   {
    case LOGIN_OKAY:
     hs.setAttribute("session", so);
     hs.setAttribute("pwchange", false);
     so.setWebHostName(request);

     String TargetPage;
     if (ServletUtils.GetCookie(request, "loggedin") != null)
     {
      // We were already logged in and have come back in via a session timeout.
      // The explorer tree(s) will refresh from their own cookies and restore
      // the main panes so just redirect to Home and let that happen
      TargetPage = "Home";
     }
     else
     {
      // We've not previously been logged in - is "origurl" cookie set? If so, then
      // we've been redirected to here from a direct link
      TargetPage = ServletUtils.GetCookie(request, "origurl");
      if (TargetPage == null)
      {
       // no original URL - just redirect to Home (must be initial login)
       TargetPage = "Home";
      }
     }
     // Set the "loggedin" cookie
     // Cookie loggedinCookie = new Cookie("loggedin","Y");
     // Cookie loggedinSession = new Cookie("session",so.GetSessionId());
     // loggedinUser = new Cookie("p1", URLEncoder.encode(username));
     // loggedinPw = new Cookie("p2",password);
     // String srv = "console.deployhub.com";
     //
     // loggedinCookie.setDomain(null);
     // loggedinSession.setDomain(null);
     // loggedinUser.setDomain(srv);
     // loggedinPw.setDomain(srv);
     //
     // loggedinCookie.setPath("/");
     // loggedinSession.setPath("/");
     // loggedinUser.setPath("/");
     // loggedinPw.setPath("/");
     //
     // response.addCookie(loggedinCookie);
     // response.addCookie(loggedinSession);
     // response.addCookie(loggedinUser);
     // response.addCookie(loggedinPw);
     //

     // And redirect to the correct page
     hs.setAttribute("logininfo", "Login OK");

     jwt = so.authUser();
     if (username == null)
      username = "";

     Cookie loggedinUser = new Cookie("p1", URLEncoder.encode(username));
     Cookie loggedinPw = new Cookie("p2", "");
     Cookie loggedinTime = new Cookie("p3", new Long(new Date().getTime()).toString());
     Cookie jwt_token = new Cookie("token", jwt);

     loggedinUser.setPath("/");
     loggedinPw.setPath("/");
     loggedinTime.setPath("/");
     jwt_token.setPath("/");

     response.addCookie(loggedinUser);
     response.addCookie(loggedinPw);
     response.addCookie(loggedinTime);
     response.addCookie(jwt_token);

     if (so.getAclOverride())
     {
      // Administrator login
      response.getWriter().write("{\"token\": \"" + jwt + "\", \"Msg\": \"Login Admin\", \"domainid\": \"" + userdomid + "\", \"domain\": \"" + userdom + "\", \"datefmt\": \"" + userdatefmt + "\", \"timefmt\": \"" + usertimefmt + "\", \"issaas\": \"" + isSaas + "\", \"lictype\": \""
        + so.getLicType() + "\", \"liccnt\": \"" + so.getLicCnt() + "\", \"newuser\": \"" + so.getNewUser() + "\", \"domlist\": \"" + so.getDomainList() + "\", \"userid\": \"" + userid +"\"}");
     }
     else
     {
      // Normal (non admin) login
      response.getWriter().write("{\"token\": \"" + jwt + "\", \"Msg\": \"Login OK\", \"domainid\": \"" + userdomid + "\", \"domain\": \"" + userdom + "\",\"datefmt\": \"" + userdatefmt + "\", \"timefmt\": \"" + usertimefmt + "\", \"issaas\": \"" + isSaas + "\", \"lictype\": \"" + so.getLicType()
        + "\", \"liccnt\": \"" + so.getLicCnt() + "\", \"newuser\": \"" + so.getNewUser() + "\", \"domlist\": \"" + so.getDomainList() + "\", \"userid\": \"" + userid +"\"}");
     }
     break;
    case LOGIN_BAD_PASSWORD:
     System.out.println("BAD PASSWORD");
     hs.setAttribute("logininfo", "Invalid Username/Password");
     response.getWriter().write("{\"Msg\":\"Invalid Username/Password\", \"datefmt\": \"\", \"timefmt\":\"\"}");
     break;
    case LOGIN_BAD_USER:
     System.out.println("BAD USER");
     hs.setAttribute("logininfo", "Invalid Username/Password");
     response.getWriter().write("{\"Msg\":\"Invalid Username/Password\", \"datefmt\": \"\", \"timefmt\":\"\"}");
     break;
    case LOGIN_USER_LOCKED:
     hs.setAttribute("logininfo", "User Account is Locked");
     response.getWriter().write("{\"Msg\": \"User Account is Locked\", \"domainid\": \"" + userdomid + "\", \"domain\": \"" + userdom + "\", \"datefmt\": \"" + userdatefmt + "\", \"issaas\": \"" + isSaas + "\", \"lictype\": \"" + so.getLicType() + "\", \"liccnt\": \"" + so.getLicCnt()
       + "\", \"timefmt\": \"" + usertimefmt + "\"}");
     break;
    case LOGIN_DATABASE_FAILURE:
     hs.setAttribute("logininfo", "Failed to connect to Database<BR>" + loginres.getMessage());
     response.getWriter().write("{\"Msg\": \"Failed to connect to Database<BR>" + loginres.getMessage() + "\", \"datefmt\": \"\", \"timefmt\":\"\"}");
     break;
    case LOGIN_CHANGE_PASSWORD:
     hs.setAttribute("session", so);
     hs.setAttribute("pwchange", true);
     so.setWebHostName(request);

     response.getWriter().write("{\"Msg\": \"Password must be changed\", \"domainid\": \"" + userdomid + "\", \"domain\": \"" + userdom + "\", \"datefmt\": \"" + userdatefmt + "\", \"timefmt\": \"" + usertimefmt + "\", \"issaas\": \"" + isSaas + "\", \"lictype\": \"" + so.getLicType()
       + "\", \"liccnt\": \"" + so.getLicCnt() + "\", \"newuser\": \"" + so.getNewUser() + "\"}");
     break;
    case LDAP_ERROR:
     System.out.println("LDAP ERROR: loginres.getMessage()=" + loginres.getMessage());
     hs.setAttribute("logininfo", "LDAP Error<BR>" + loginres.getMessage());
     response.getWriter().write("{\"Msg\": \"LDAP Error<BR>" + loginres.getMessage() + "\", \"datefmt\": \"\", \"timefmt\":\"\"}");
     break;
   }
  }
 }
}
