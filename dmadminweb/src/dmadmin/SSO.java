package dmadmin;

import java.io.IOException;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Date;
import java.util.Optional;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.pac4j.core.config.Config;
import org.pac4j.core.context.WebContext;
import org.pac4j.core.context.session.SessionStore;
import org.pac4j.core.credentials.Credentials;
import org.pac4j.core.exception.http.HttpAction;
import org.pac4j.core.profile.ProfileManager;
import org.pac4j.core.profile.UserProfile;
import org.pac4j.core.util.FindBest;
import org.pac4j.jee.context.JEEContextFactory;
import org.pac4j.jee.context.session.JEESessionStore;
import org.pac4j.jee.http.adapter.JEEHttpActionAdapter;
import org.pac4j.saml.client.SAML2Client;

import dmadmin.model.Domain;
import dmadmin.model.User;

public class SSO extends HttpServletBase {
    private final SAML2ClientBuilder saml2ClientBuilder = new SAML2ClientBuilder();

    @Override
    public void handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {
     if (isPost)
      doPost(session, request, response);
     else
      doGet(session, request, response);
    }

    // serves GET /dmadminweb/sso
    protected void doGet(DMSession session, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {

           String IdpMetadata = System.getenv("SamlIdpMetadata");

           if (IdpMetadata == null)
            return;

            String dhurl = saml2ClientBuilder.getCurrentUrl(request);
            SAML2Client client = saml2ClientBuilder.build(dhurl);
            Config config = new Config(client);
            SessionStore bestSessionStore = FindBest.sessionStore(null, config, JEESessionStore.INSTANCE);
            WebContext context = FindBest.webContextFactory(null, config, JEEContextFactory.INSTANCE).newContext(request, response);

            HttpAction action;
            try {
                action = (HttpAction)client.getRedirectionAction(context, bestSessionStore).get();
            } catch (final HttpAction e) {
                action = e;
            }
            JEEHttpActionAdapter.INSTANCE.adapt(action, context);

        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    // serves POST /dmadminweb/sso/finish
    protected void doPost(DMSession so, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        try {
            String IdpMetadata = System.getenv("SamlIdpMetadata");

            if (IdpMetadata == null)
             return;

            String dhurl = saml2ClientBuilder.getCurrentUrl(request);
            SAML2Client client = saml2ClientBuilder.build(dhurl);
            Config config = new Config(client);
            SessionStore bestSessionStore = FindBest.sessionStore(null, config, JEESessionStore.INSTANCE);

            WebContext context = FindBest.webContextFactory(null, config, JEEContextFactory.INSTANCE).newContext(request, response);

            Optional<Credentials> credentials = client.getCredentials(context, bestSessionStore);
            Optional<UserProfile> profile = client.getUserProfile(credentials.get(), context, bestSessionStore);
            String username = profile.get().getId();
            ArrayList<String> attrs = (ArrayList<String>) profile.get().getAttribute("domain");

            String domain = null;

            if (attrs != null && attrs.size() > 0)
             domain = attrs.get(0);

            if (domain == null)
             domain = "GLOBAL";

            username = username.replace("@","-");
            username = username.replace(".","-");
            username = username.replace("\\","-");
            username = username.replace("/","-");

            System.out.println("received done message");
            System.out.println("credentials = " + credentials);
            System.out.println("profile = " + profile);
            System.out.println("username = " + domain + "." + username);

            ProfileManager profileManager = new ProfileManager(context, bestSessionStore);
            profileManager.save(true, profile.get(), false);

            so.internalLogin(getServletContext());

            User user = null;

            try
            {
             user = so.getUserByName(domain + "." + username);
            }
            catch (RuntimeException e)
            {

            }

            if (user == null)
            {
              API api = new API();
              api.createDomains4Obj(so, domain + "." + username);

              Domain tgtdomain = so.getDomainByName(domain);

              int newid = so.getID("user");
              so.CreateNewObject("user", username, tgtdomain.getId(), tgtdomain.getId(), newid, -1, -1, "", false);

              so.setUserId(newid);
              user = so.getUser(so.GetUserID());

              if (user == null)
               throw new RuntimeException("Failed to create new user");
            }

            String jwt = so.authUser();

            HttpSession session = request.getSession();
            session.setAttribute("session", so);

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
            response.sendRedirect("/dmadminweb/Home");
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }
}
