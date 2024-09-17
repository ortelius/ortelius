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

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
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

/**
 * Servlet implementation class Home
 */
public class Home extends HttpServletBase
{
 private static final long serialVersionUID = 1L;
 private final SAML2ClientBuilder saml2ClientBuilder = new SAML2ClientBuilder();

 /**
  * @see HttpServlet#HttpServlet()
  */
 public Home()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 @Override
 public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  HomeTabsList ht = session.getHomeTabs();
  request.setAttribute("ht", ht);
  request.setAttribute("username", session.GetUserName());
  request.setAttribute("firstinstall", session.firstInstall());
  request.setAttribute("admin", session.getAclOverride() ? "Y" : "N");
  System.out.println("admin=" + (session.getAclOverride() ? "Y" : "N"));

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
   HttpSession sess = request.getSession();
   sess.setAttribute("redirecturl", request.getRequestURL().toString());
   RequestDispatcher dispatcher = getServletContext().getRequestDispatcher("/sso");
   dispatcher.forward(request, response);
   return;
  }

  request.getRequestDispatcher("/WEB-INF/Home.jsp").forward(request, response);
 }
}
