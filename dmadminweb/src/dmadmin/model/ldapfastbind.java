package dmadmin.model;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Hashtable;

import javax.naming.Context;
import javax.naming.NamingException;
import javax.naming.ldap.Control;
import javax.naming.ldap.InitialLdapContext;
import javax.naming.ldap.LdapContext;

public class ldapfastbind
{
 public Hashtable<String, String> env = null;
 public LdapContext ctx = null;
 public Control[] connCtls = null;

 public ldapfastbind(String ldapurl)
 {
  env = new Hashtable<String, String>();
  env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.ldap.LdapCtxFactory");
  env.put(Context.SECURITY_AUTHENTICATION, "simple");
  env.put(Context.PROVIDER_URL, ldapurl);
  connCtls = new Control[] { new FastBindConnectionControl() };
  // first time we initialize the context, no credentials are supplied
  // therefore it is an anonymous bind.

  try
  {
   ctx = new InitialLdapContext(env, connCtls);
  }
  catch (Exception ex)
  {
  }
 }

 public boolean Authenticate(String username, String password)
 {
  try
  {
   ctx.addToEnvironment(Context.SECURITY_PRINCIPAL, username);
   ctx.addToEnvironment(Context.SECURITY_CREDENTIALS, password);   
   ctx.reconnect(connCtls);
   System.out.println(username + " is authenticated");
   return true;
  }
  catch (Exception ex)
  {
   try
   {
    StringWriter writer = new StringWriter();
    PrintWriter printWriter = new PrintWriter(writer);
    ex.printStackTrace(printWriter);
   }
   catch (Exception e)
   {
   }
   return false;
  }
 }

 public void finito()
 {
  try
  {
   ctx.close();
   System.out.println("Context is closed");
  }
  catch (NamingException e)
  {
   System.out.println("Context close failure " + e);
  }
 }
}
