package dmadmin;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;

/**
 * Servlet implementation class ChangePassword
 */
public class ChangePassword extends HttpServlet
{
 private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;

 /**
  * @see HttpServlet#HttpServlet()
  */
 public ChangePassword()
 {
  super();
  // TODO Auto-generated constructor stub
 }

 /**
  * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
 }

 /**
  * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
  */
 protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  System.out.println("In ChangePassword post");

  try (DMSession so = DMSession.getInstance(request))
  {
   session = request.getSession();
   session.setAttribute("session", so);
   so.checkConnection(request.getServletContext());

   PrintWriter out = response.getWriter();
   String oldpass = request.getParameter("oldpw");
   String newpass = request.getParameter("newpw");
   String myuserid = request.getParameter("username");
   System.out.println("newpw=" + newpass);
   JSONObject ret = new JSONObject();

   LoginException le = so.Login(myuserid, oldpass);

   if (le != null && le.exceptionType != LoginExceptionType.LOGIN_OKAY &&  le.exceptionType != LoginExceptionType.LOGIN_CHANGE_PASSWORD)
   {
    ret.add("res", false);
    ret.add("errtext", le.getMessage());
    out.println(ret.getJSON());
   }
   else
   {
    String ResultText = so.ChangePassword(oldpass, newpass);

    if (ResultText != null)
    {
     ret.add("res", false);
     ret.add("errtext", ResultText);
     out.println(ret.getJSON());
    }
    else
    {
     ret.add("res", true);
     ret.add("errtext", "");
     out.println(ret.getJSON());
    }
   }
  }
 }
}
