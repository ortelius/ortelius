package dmadmin;

import java.io.File;
import java.net.URI;
import java.util.Enumeration;

import javax.servlet.http.HttpServletRequest;

import org.opensaml.saml.common.xml.SAMLConstants;
import org.pac4j.core.http.callback.NoParameterCallbackUrlResolver;
import org.pac4j.saml.client.SAML2Client;
import org.pac4j.saml.config.SAML2Configuration;

public class SAML2ClientBuilder
{

 public SAML2Client build(String dhurl)
 {
  SAML2Configuration config = new SAML2Configuration();

  String SamlIdpMetadataPath = System.getenv("SamlIdpMetadataPath");
  String SamlKeystorePath = System.getenv("SamlKeystorePath");
  String SamlKeystorePassword = System.getenv("SamlKeystorePassword");
  String SamlPrivateKeyPassword = System.getenv("SamlPrivateKeyPassword");
  String SamlKeystoreAlias = System.getenv("SamlKeystoreAlias");

  File file = new File(SamlIdpMetadataPath);
  URI uri = file.toURI();

  config.setIdentityProviderMetadataResourceUrl(uri.toString());
  config.setSpLogoutRequestBindingType(SAMLConstants.SAML2_REDIRECT_BINDING_URI);

  // keytool -genkey -keyalg RSA -alias saml -keypass changeit -keystore trust.keystore -storepass changeit
  config.setKeystorePath(SamlKeystorePath);
  config.setKeystorePassword(SamlKeystorePassword);
  config.setPrivateKeyPassword(SamlPrivateKeyPassword);
  config.setKeystoreAlias(SamlKeystoreAlias);

  SAML2Client saml2Client = new SAML2Client(config);
  saml2Client.setName("DeployHub");
  saml2Client.setCallbackUrl(dhurl + "/dmadminweb/sso/finish");
  saml2Client.setCallbackUrlResolver(new NoParameterCallbackUrlResolver());
  saml2Client.init();

  return saml2Client;
 }

 public String getCurrentUrl(HttpServletRequest request)
 {

  Enumeration headerNames = request.getHeaderNames();
  while (headerNames.hasMoreElements())
  {
   String key = (String) headerNames.nextElement();
   String value = request.getHeader(key);
   System.out.println(key + "=" + value);
  }

  String forwardedHost = request.getHeader("x-forwarded-host");

  if (forwardedHost == null)
  {
   String scheme = "https"; // http
   String serverName = request.getServerName(); // hostname.com
   int serverPort = request.getServerPort(); // 80

   if (serverName.contains("localhost"))
    scheme = "http";

   // Reconstruct original requesting URL
   StringBuilder url = new StringBuilder();
   url.append(scheme).append("://").append(serverName);

   if (serverPort != 80 && serverPort != 443)
   {
    url.append(":").append(serverPort);
   }
   return url.toString();
  }

  // String scheme = request.getHeader("x-forwarded-proto");
  // String prefix = request.getHeader("x-forwarded-prefix");

  if (forwardedHost.contains("localhost"))
   return "http://" + forwardedHost;

  return "https://" + forwardedHost;
 }
}
