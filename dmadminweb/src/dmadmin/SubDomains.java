/*
 *
 *  DeployHub is an Agile Application Release Automation Solution
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
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;






import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.DeployedApplication;
import dmadmin.model.Domain;
import dmadmin.model.Environment;
import dmadmin.model.TreeObject;

/**
 * Servlet implementation class DomainDetails
 */
public class SubDomains extends HttpServletBase {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public SubDomains() {
        super();
        // TODO Auto-generated constructor stub
    }
    
    
    @Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
    	int domainid = ServletUtils.getIntParameter(request, "domainid");
    	
    	String sds = request.getParameter("ds");
    	boolean ds = (sds != null && sds.equalsIgnoreCase("y"));
   		List<TreeObject> subdomains = session.getDomains(domainid);
   		PrintWriter out = response.getWriter();
   		JSONArray list = new JSONArray();
   		
   		for (int i=0;i<subdomains.size();i++) {
   			TreeObject sub = subdomains.get(i);
   			JSONObject o = new JSONObject();
   			o.add("id", sub.getId());
   			o.add("name",sub.getName());
   			if (ds) {
   				// If we're doing a lifecycle summary, pull in the environments
   				Domain domain = session.getDomain(domainid);
   				if (domain.getLifecycle()) {
   					o.add("lifecycle",true);
	   				Hashtable<Integer,String> deployedapps = new Hashtable<Integer,String>();
	   				Domain dom = session.getDomain(sub.getId());
	   				List<Environment> envs = session.getEnvironmentsInDomain(dom);
	   				o.add("envcount", envs.size());
	   				JSONArray ea = new JSONArray();
	   				for (int e=0;e<envs.size();e++) {
	   					Environment env = envs.get(e);
	   					JSONObject eo = new JSONObject();
	   					eo.add("id",env.getId());
	   					eo.add("name", env.getName());
	   					// Now the apps in this environment
	   					JSONArray aa = new JSONArray();
	   					List<DeployedApplication> apps = session.getDeployedApplicationsInEnvironment(env.getId());
	   					int ac=0;
	   					for (int a=0;a<apps.size();a++) {
	   						DeployedApplication app = apps.get(a);
	   						if (app.getVersionID()>0) {
	   							// Deployed application (not simply assigned)
		   						JSONObject ao = new JSONObject();
		   						ao.add("id",app.getVersionID());
		   						ao.add("name",app.getVersionName());
		   						ao.add("pid",app.getApplicationID());
		   						System.out.println("Finding approvals for "+app.getVersionName());
		   						String status = session.getApprovalStatus(app.getVersionID());
		   						ao.add("status",status);
		   						aa.add(ao);
		   						deployedapps.put(app.getVersionID(),"Y");
		   						ac++;
	   						}
	   					}
	   					eo.add("appcount",ac);
	   					eo.add("apps",aa);
	   					ea.add(eo);
	   				}
	   				o.add("envs",ea);
	   				// Now any apps in the domain that are not in an environment
	   				int nac=0;
	   				JSONArray and = new JSONArray();
	   				List<Application> ndapps = session.getApplicationsInDomain(sub.getId());
	   				for (int x=0;x<ndapps.size();x++) {
	   					Application ndapp = ndapps.get(x);
	   					if (!deployedapps.containsKey(ndapp.getId())) {
	   						JSONObject ndo = new JSONObject();
	   						ndo.add("id",ndapp.getId());
	   						ndo.add("name",ndapp.getName());
	   						ndo.add("pid",ndapp.getParentId());
	   						System.out.println("Finding approvals for "+ndapp.getName()+" id="+ndapp.getId());
	   						String status = session.getApprovalStatus(ndapp.getId());
	   						ndo.add("status",status);
	   						and.add(ndo);
	   						nac++;
	   					}
	   				}
	   				o.add("appcount",nac);
	   				o.add("apps",and);
   				} else {
   					o.add("lifecycle",false);
   				}
   			}
   			list.add(o);
   		}
   		
   		out.println(list.toString());
   		
	}
}
