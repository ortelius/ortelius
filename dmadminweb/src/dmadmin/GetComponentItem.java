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
		
		HttpSession session = request.getSession();
		DMSession so = (DMSession)session.getAttribute("session");
		if(so == null)  {
			System.out.println("SESSION TIMED OUT");
			return;
		}
		
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
						processField(so, field, value, schanges);
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
				pchanges.addChanged(processProperty(prop, pval));
			} else if(param.startsWith("prop_add_")) {
				// Now handle the Property additions
				String prop = param.substring(9);
				String pval = request.getParameter(param);
				System.out.println("add '" + prop + "' = '" + pval + "'");
				pchanges.addAdded(processProperty(prop, pval));
			} else if(param.startsWith("prop_delete_")) {
				// Now handle the Property removals
				String prop = param.substring(12);
				String pval = request.getParameter(param);
				System.out.println("delete '" + prop + "' = '" + pval + "'");
				pchanges.addDeleted(processProperty(prop, pval));
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
	
	private void processField(DMSession so, SummaryField field, String value, SummaryChangeSet changes)
	{
		ObjectType type = field.type();

		if(type == null) {		
			// Simple string
			changes.add(field, value);
			return;
		}
		
		if((value == null) || (value.length() == 0)) {
			// Null value
			changes.add(field, null);
			return;			
		}

		switch(type) {
		case COMPONENT_FILTER:	// Simple string "OFF", "ON", "ALL"
			changes.add(field, value);
			return;
		default:
			break;
		}
		
		// Parse out the oid (from "u123") and lookup the object - we guarantee that we will pass an object that exists
		String prefix = type.getTypeString();
		if(value.startsWith(prefix)) {
			String soid = value.substring(prefix.length());
			try {
				int oid = Integer.parseInt(soid);
				Object obj = so.getObject(type, oid);
				if(obj != null) {
					changes.add(field, obj);
				} else {
					System.err.println("ERROR: Object " + type + " " + oid + " not found");											
				}
			} catch(Exception e) {
				e.printStackTrace();
			}
		} else {
			System.err.println("ERROR: Incorrect object type for field " + field + " (expecting " + type + ")");
		}
	}

	private DMProperty processProperty(String prop, String pval)
	{
		//if(pval.length() < 3) {
		//	throw new RuntimeException("Invalid property value: too short");
		//}
		//char encr = pval.charAt(0);
		//char over = pval.charAt(1);
		//char apnd = pval.charAt(2);
		//String value = pval.substring(3);
		//return new DMProperty(prop, value, (encr == 'Y'), (over == 'Y'), (apnd == 'Y'));
		return new DMProperty(prop, pval, false, false, false);
	}
}

