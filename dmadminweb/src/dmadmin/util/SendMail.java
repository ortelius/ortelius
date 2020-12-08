package dmadmin.util;

import java.io.UnsupportedEncodingException;
import java.util.Properties;

import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

public class SendMail extends Thread
{
 private String from;
 private String password;
 private String to;
 private String subject;
 private String msg;
 private String details;

 public SendMail(String from, String password, String to, String subject, String msg, String details)
 {
  this.from = from;
  this.password = password;
  this.to = to;
  this.subject = subject;
  this.msg = msg;
  this.details = details;
 }
 
 public void run()
 {
  Properties props = new Properties();
  props.put("mail.debug", "true");
  props.put("mail.smtp.host", System.getenv("SMTP_HOST")); // SMTP Host
  props.put("mail.user", from);
  props.put("mail.password", password);
  props.put("mail.smtp.port", "465"); // TLS Port
  props.put("mail.smtp.auth", "true"); // enable authentication
  props.put("mail.smtp.socketFactory.class", "javax.net.ssl.SSLSocketFactory");

  // creating Session instance referenced to
  // Authenticator object to pass in
  // Session.getInstance argument
  Session session = Session.getDefaultInstance(props, new javax.mail.Authenticator()
  {
   protected PasswordAuthentication getPasswordAuthentication()
   {
    return new PasswordAuthentication(from, password);
   }
  });

  try
  {
   Message message = new MimeMessage(session);
   message.setFrom(new InternetAddress(from, "DeployHub Support"));
   message.setRecipients(Message.RecipientType.TO, InternetAddress.parse(to));
   message.setSubject(subject);
   message.setContent(msg, "text/html");

   Transport.send(message);
   System.out.println("Yo it has been sent.."); 
   
   message = new MimeMessage(session);
   message.setFrom(new InternetAddress(from, "DeployHub Signup"));
   message.setRecipients(Message.RecipientType.TO, InternetAddress.parse("steve@deployhub.com,baca@depoyhub.com,tracy@deployhub.com"));
   message.setSubject("DeployHub Signup");
   message.setContent(details, "text/html");

   Transport.send(message);
  }
  catch (MessagingException e)
  {
   e.printStackTrace();
  }
  catch (UnsupportedEncodingException e)
  {
   // TODO Auto-generated catch block
   e.printStackTrace();
  }
 }
}
