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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import dmadmin.json.JSONObject;
import dmadmin.model.ComponentItem;
import dmadmin.model.DMProperty;


/**
 * Servlet implementation class getfragatt
 */
public class GetComponentItem
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
 DMSession so = null;
 HttpSession session = null;       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public GetComponentItem() {
        super();
    }

	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int ccid = ServletUtils.getIntParameter(request, "ccid");
    	//int compid = ServletUtils.getIntParameter(request, "c");
		String eventcbfn = getStringParameter(request, "eventcb", "");	// callback function - invoked when an event occurs
		ComponentItem ci = session.getComponentItem(ccid, true);

		// Constant values for summary change callbacks - use these in preference to hardcoding the values
		request.setAttribute("SummaryField_ITEM_REPOSITORY", SummaryField.ITEM_REPOSITORY.value());
		request.setAttribute("SummaryField_ITEM_TARGETDIR", SummaryField.ITEM_TARGETDIR.value());

		request.setAttribute("ci", ci);
		request.setAttribute("eventcbfn", eventcbfn);
        request.getRequestDispatcher("/WEB-INF/getcomponentitem.jsp").forward(request, response);
	}
	
	@Override
	protected void doPost(HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		response.setContentType("application/json");

  try (DMSession so = DMSession.getInstance(request)) {
  session = request.getSession();
  session.setAttribute("session", so);
  so.checkConnection(request.getServletContext());
		
		response.setHeader("Content-Disposition", "inline");
		response.setHeader("Cache-Control", "no-cache");

		int ciid = ServletUtils.getIntParameter(request, "ciid");
  String deleteoldrows = getStringParameter(request, "oldrows", "");
  
		SummaryChangeSet schanges = new SummaryChangeSet();
		ACDChangeSet<DMProperty> pchanges = new ACDChangeSet<DMProperty>();
				
		for(Object oparam : request.getParameterMap().keySet()) {
			String param = (String) oparam;
			if(param.startsWith("summ_change_")) {
				// First handle the Summary Field changes - these all start with
				// "summ_change_" - Repository will already have been made into a
				// Summary Field by this point
				String value = request.getParameter(param);
				System.out.println("'" + param + "' = '" + value + "'");
				try {
					int sf = Integer.parseInt(param.substring(12));
					SummaryField field = SummaryField.fromInt(sf);
					if(field != null) {
						so.processField(so, field, value, schanges);
					} else {
						System.err.println("ERROR: Unknown summary field " + sf);
					}
				} catch(NumberFormatException e) {
					e.printStackTrace();
				}
			} else if(param.startsWith("prop_change_")) {
				// Now handle the Property changes
				String prop = param.substring(12);
				String pval = request.getParameter(param);
				System.out.println("change '" + prop + "' = '" + pval + "'");
				pchanges.addChanged(so.processProperty(prop, pval));
			} else if(param.startsWith("prop_add_")) {
				// Now handle the Property additions
				String prop = param.substring(9);
				String pval = request.getParameter(param);
				System.out.println("add '" + prop + "' = '" + pval + "'");
				pchanges.addAdded(so.processProperty(prop, pval));
			} else if(param.startsWith("prop_delete_")) {
				// Now handle the Property removals
				String prop = param.substring(12);
				String pval = request.getParameter(param);
				System.out.println("delete '" + prop + "' = '" + pval + "'");
				pchanges.addDeleted(so.processProperty(prop, pval));
			}
		}
		
		
		PrintWriter out = response.getWriter();
		JSONObject obj = new JSONObject();

		try {
			ComponentItem ci = so.getComponentItem(ciid, true);
			if(ci.isUpdatable()) {			 
				boolean res = true;
				if(!schanges.isEmpty()) {
					ci.updateSummary(schanges);
				}
				if(res && !pchanges.isEmpty()) {
	    if (deleteoldrows.equalsIgnoreCase("Y"))
	     ci.deleteProperties();

					res = ci.updateProperties(pchanges);
				}
				obj.add("saved", res);

				// Send back the updated name, summary and icon suffix
				ComponentItem uci = so.getComponentItem(ciid, false);
				obj.add("name", uci.getName());
				obj.add("summary", uci.getSummary());
				obj.add("suffix", uci.getIconSuffix());
			} else {
				obj.add("saved", false);
				obj.add("error", "You do not have permission to update this object");				
			}
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("saved", false);
			obj.add("error", e.getMessage());
		}
		
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
	}
	}
}

