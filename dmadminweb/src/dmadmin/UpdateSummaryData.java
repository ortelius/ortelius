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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;




import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.ActionKind;
import dmadmin.model.CredentialKind;
import dmadmin.model.DMObject;
import dmadmin.model.Domain;
import dmadmin.model.Engine;

/**
 * Servlet implementation class UpdateSummaryData
 */
public class UpdateSummaryData
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public UpdateSummaryData() {
        super();
    }
    
    @Override
   	public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	int ot = ServletUtils.getIntParameter(request, "objtype");
		int id = ServletUtils.getIntParameter(request, "id");		
		ObjectType objtype = ObjectType.fromInt(ot);
		if(objtype == null) {
			throw new RuntimeException("Invalid object type " + ot);
		}
		
		System.out.println("In UpdateSummaryData");
		
		DMObject dmobj = session.getDetailedObject(objtype, id);
		
		SummaryChangeSet changes = new SummaryChangeSet();
		
		String errtext = null;
		
		for(Object oparam : request.getParameterMap().keySet()) {
			String param = (String) oparam;
			if(param.startsWith("change_") && !param.equalsIgnoreCase("change_undefined")) {
				String value = request.getParameter(param);
				System.out.println("'" + param + "' = '" + value + "'");
				try {
					int sf = Integer.parseInt(param.substring(7));
					SummaryField field = SummaryField.fromInt(sf);
					if(field != null) {
						errtext = processField(session, dmobj, field, value, changes);
						if (errtext != null) {
							System.err.println(errtext);
							break;
						}
					} else {
						errtext = "ERROR: Unknown summary field " + sf;
						System.err.println(errtext);
						break;
					}
				} catch(NumberFormatException e) {
					e.printStackTrace();
				}
			}
		}
		
		System.out.println("no exception thrown yet");
		
		response.setContentType("application/json");
		PrintWriter out = response.getWriter();
		JSONObject obj = new JSONObject();
		
		System.out.println("errtext=["+errtext+"]");
		
		if (errtext != null) {
			obj.add("saved",false);
			obj.add("error",errtext);
		} else {
			try {
				if(dmobj.isUpdatable()) {
					boolean res = dmobj.updateSummary(changes);
					obj.add("saved", res);
				} else {
					obj.add("saved", false);
					obj.add("error", "You do not have permission to update this object");				
				}
			} catch(Exception e) {
				System.out.println("exception thrown: "+e.getMessage());
				e.printStackTrace();
				obj.add("saved", false);
				obj.add("error", e.getMessage());
			}
		}
		
		String ret = obj.getJSON();
		System.out.println(ret);
		out.println(ret);
   	}

	private String processField(DMSession so, DMObject dmobj, SummaryField field, String value, SummaryChangeSet changes)
	{
		ObjectType ot = dmobj.getObjectType();
		ObjectType type = field.type();
		System.out.println("ot="+ot+" field.name()=["+field.name()+"] value=["+value+"]");
		if (ot == ObjectType.ACTION || ot == ObjectType.FUNCTION || ot == ObjectType.PROCEDURE) {
			Action act = (Action)dmobj;
			if (act.getKind() != ActionKind.GRAPHICAL) ot = ObjectType.PROCEDURE;
			if (act.isFunction()) ot = ObjectType.FUNCTION;
			if (type == null && field.name().equalsIgnoreCase("name")) {
				// Actions, Functions and Procedures should not allow spaces in names
				if (value.indexOf(" ") >= 0) {
					return ot.toString().substring(0,1)+ot.toString().substring(1).toLowerCase() +
							" name cannot include spaces";
				}
				String testname = value.replaceAll("[0-9A-Za-z_]","");
				if (testname.length()>0) {
					return ot.toString().substring(0,1)+ot.toString().substring(1).toLowerCase() +
							" name can only include alpha-numeric characters or underscores";
				}
				if (value.charAt(0)>='0' && value.charAt(0)<='9') {
					return ot.toString().substring(0,1)+ot.toString().substring(1).toLowerCase() +
							" name cannot start with a digit";
				}
			}
		}
		
		System.out.println("type="+type+" field="+field.name());
		
		if(type == ObjectType.ENCRYPTED) {
			// Encrypted value - use engine to encrypt
			Domain dom = dmobj.getDomain();
			Engine eng = (dom != null) ? dom.findNearestEngine() : null;
			if(eng == null) {
				System.err.println("ERROR: Could not find engine to encrypt data for object "
					+ dmobj.getObjectType() + " " + dmobj.getId());
			}
			try {
				String encValue = eng.encryptValue(value, so.GetUserName());
				changes.add(field, encValue);
			} catch(RuntimeException e) {
				e.printStackTrace();
			}
			return null;
		}

		if((type == null) || (type == ObjectType.PASS_HASH)) {
			// Simple string
			changes.add(field, value);
			return null;
		}
		
		if((value == null) || (value.length() == 0)) {
			// Null value
			changes.add(field, null);
			return null;			
		}

		// Owner can be either a user or a group so resolve into actual type
		switch(type) {
		case OWNER:
			if(value.startsWith(ObjectType.USER.getTypeString())) {
				type = ObjectType.USER;
			} else if(value.startsWith(ObjectType.USERGROUP.getTypeString())) {
				type = ObjectType.USERGROUP;
			} else {
				return "ERROR: Invalid object type for field " + field;
			}
			break;
		case TRANSFER:			// Simple protocol string for the moment
		case FILTER_ITEMS:		// Simple string "Components" or "Items"
		case COMPONENT_FILTER:	// Simple string "OFF", "ON", "ALL"
		case SERVERCOMPTYPE: 
			changes.add(field, value);
			return null;

		case BOOLEAN:
			changes.add(field, value.equalsIgnoreCase("true"));
			return null;
		default:
			break;
		}
		
		// Parse out the oid (from "u123") and lookup the object - we guarantee that we will pass an object that exists
		String prefix = type.getTypeString();
		if (prefix == "ar") prefix = "re";	// (a)ction (r)epository maps to (re)pository
		System.out.println("prefix="+prefix);
		System.out.println("value="+value);
		if(value.startsWith(prefix) || (prefix.equalsIgnoreCase("ac") && value.startsWith("fn"))) {
			String soid = value.substring(prefix.length());
			try {
				int oid = Integer.parseInt(soid);
				Object obj = null;
				// These are just the objects that don't derive from DMObject
				switch(type) {
				case SERVER_TYPE: obj = so.getServerType(oid); break;
				case PROVIDERDEF: obj = so.getProviderDefinition(oid); break;
				case CREDENTIAL_KIND: obj = CredentialKind.fromInt(oid); break;
				case ACTION_KIND: obj = ActionKind.fromInt(oid); break;
				case ACTION_REPO: obj = so.getObject(ObjectType.REPOSITORY, oid); break;
				case ACTION_CATEGORY: obj = so.getCategory(oid); break;
				default:          obj = so.getObject(type, oid); break;
				}
				if(obj != null) {
					changes.add(field, obj);
				} else {
					return "ERROR: Object " + type + " " + oid + " not found";
				}
			} catch(Exception e) {
				e.printStackTrace();
			}
		} else {
			return "ERROR: Incorrect object type for field " + field + " (expecting " + type + ")";
		}
		return null;
	}
}
