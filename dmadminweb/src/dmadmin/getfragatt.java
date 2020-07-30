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
import java.io.PrintWriter;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.model.FragmentAttributes;
import dmadmin.model.FragmentDetails;
import dmadmin.model.FragmentListValues;

/**
 * Servlet implementation class getfragatt
 */
public class getfragatt extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public getfragatt() {
        super();
        // TODO Auto-generated constructor stub
    }

    //private void ProcessFragmentAttributes(HttpServletRequest request,HttpServletResponse response) throws ServletException, IOException {
    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	response.setContentType("text/html;charset=UTF-8");
 
		PrintWriter out = response.getWriter();
		
    	//String typeid = request.getParameter("typeid");
    	String editmode = request.getParameter("editmode");
    	int actionid = ServletUtils.getIntParameter(request,"actionid");
    	int windowid = ServletUtils.getIntParameter(request,"windowid");
    	boolean useDefault = (request.getParameter("useDefault") != null && request.getParameter("useDefault").equalsIgnoreCase("Y")) ? true : false;
    	String disabled="disabled";
    	if (editmode.compareToIgnoreCase("y")==0)
    	{
    		disabled="";
    	}
    	//
    	//Get existing values (if any)
    	//
    	int pw = ServletUtils.getIntParameter(request,"pw");

    	List<FragmentAttributes> fa = session.getFragmentAttributes(actionid,windowid,pw);
    	FragmentDetails fd = session.getFragmentDetails(actionid,windowid);
    	// Action action = session.getAction(fd.getFragmentAction(),false);
    	// System.out.println("actionid="+actionid+" fd action="+action.getId());
    	
    	out.println("<form id='fragattrs'>");
    	out.println("<div id=\"fragheader\"><h3>"+fd.getTypeName()+"</h3>");
    	    	
    	out.println("<span class=\"summwrap\">" + fd.getTypeSummary()+"</span><br></div>");
    	out.println("<hr><div id=\"attdiv\">");
    		
    	out.println("<table border=0 id=\"atttable\" class=\"atttable\" >");
    	
    	out.print("<tr><td>Title</td><td><input id=\"title\" type=\"text\" value=\""+fd.getFragmentName()+"\"></td>");
    	out.print("<tr><td>Summary</td><td><input id=\"summary\" type=\"text\" value=\""+fd.getFragmentSummary()+"\"></td>");
    	//if (action != null && action.isFunction()) {
    	//	out.print("<tr><td id=\"cresultvar\">Result<sup>*</sup></td><td id=\"rresultvar\"><input id=\"resultvar\" type=\"text\" value=\""+""+"\"></td>");
    	//}
    	String ps="";
    	for (FragmentAttributes a: fa)
		{
    		String str = a.getAttrName();
    		str = str.replaceAll("([A-Z]+)", " $1");
    		str = str.trim();
    		String flag = (a.isRequired() && a.getAttrType().equalsIgnoreCase("entry"))?"<sup>*</sup>":"";
    		String hidden = a.isRequired()?"Y":"N";
    		out.print("<tr><td id=\"c"+a.getAttrId()+"\">"+str+flag+"<input type=\"hidden\" id=\"h"+a.getAttrId()+"\" value=\""+hidden+"\"></td><td id=\"r"+a.getAttrId()+"\">");
    		if (a.getAttrType().equalsIgnoreCase("dropdown"))
    		{
    			out.println("<select id='f"+a.getAttrId()+"' "+disabled+">");
    			if (!a.isRequired()) out.println("<option value=\"\"></option>");
    			for (FragmentListValues d: a.getFragmentListValues())
    			{
    				out.println("<option value=\""+d.getName()+"\" " + d.getSelected() + ">"+d.getName()+"</option>");
    			}
				out.println("</select>");
			}
			else
			if (a.getAttrType().equalsIgnoreCase("entry"))
			{
			 if (useDefault)
				 out.print("<input id='f"+a.getAttrId()+"' type=text "+disabled+" value='" + a.getDefaultValue() + "'>");
			 else
			 	out.print("<input id='f"+a.getAttrId()+"' type=text "+disabled+" value='" + a.getAttrVal() + "'>");
			}
			else
			if (a.getAttrType().equalsIgnoreCase("checkbox"))
			{
				String checked = a.getAttrVal().equalsIgnoreCase("y")?"checked":"";
				System.out.println("checked="+checked+" a.getAttrVal()=["+a.getAttrVal()+"]");
				out.print("<input id='f"+a.getAttrId()+"' type=checkbox value='Y' "+disabled+" "+checked+">");
				System.out.print("<input id='f"+a.getAttrId()+"' type=checkbox value='Y' "+disabled+" "+checked+">");
			}
			else
			{
				out.print("&nbsp;");	// unknown
			}
			out.println("</td></tr>");
			if (a.isRequired()) ps="<sup>*</sup> Required";
		}
    	out.println("</table></div><hr><div id=\"reqdiv\">"+ps+"</div><div class=\"fragmentfielderror\" id=\"errtext\">&nbsp;</div></form>");
    }
}
