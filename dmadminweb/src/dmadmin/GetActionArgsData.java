/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package dmadmin;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Action;
import dmadmin.model.Action.ActionArg;

/**
 * Servlet implementation class GetActionArgsData
 */
public class GetActionArgsData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    public GetActionArgsData() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws PermissionDeniedException, ServletException, IOException
	{
		int actionid = getIntParameter(request, "id");
		Action action = session.getAction(actionid, true, ObjectType.ACTION );
		
		boolean readOnly = !action.isUpdatable();
		if (action.getParentId()>0) readOnly = true;	// archived actions are not editable
		
		if(!isPost) {
			// Gets the arg data
			List<ActionArg> args = action.getInputArgs();
			
			JSONObject ret = new JSONObject().add("result", true);
			ret.add("readonly", readOnly);
			ret.add("filepath", action.getFilepath());
			ret.add("interpreter", action.getInterpreter());
   
			JSONArray data = new JSONArray();
			ret.add("data", data);
			
			for(ActionArg arg : args) {
				data.add(new JSONArray()
					.add(arg.getName())		// id - use the original name
					.add(false)				// dirty
					.add(false)				// added
					.add(false)				// deleted
					.add(arg.getName())		// name
					.add(arg.getType())		// type
					.add(arg.isRequired())	// required
					.add(arg.isPad())		// pad
					.add(arg.getInputPosition() > 0 ? arg.getInputPosition() : null)		// inpos
					.add(arg.getOutputPosition() > 0 ? arg.getOutputPosition() : null)		// outpos
					.add(arg.getSwitchMode().value())
					.add(arg.getSwitch())
					.add(arg.getNegSwitch()));
			}
			return ret;
		} 
		else 
		{
	  String updateOrder = request.getParameter("updateOrder");
   String updateInputOrder = request.getParameter("updateInputOrder");
   String switchAction = request.getParameter("switchAction");
   String removeArg = request.getParameter("removeArg");
   
   if (updateInputOrder != null && updateInputOrder.equalsIgnoreCase("Y"))
   {
    HashMap<String,String> updates = new HashMap<String,String>();
    
    for (Object oparam : request.getParameterMap().keySet())
    {
     String key = (String)oparam;
     if (!key.equalsIgnoreCase("updateInputOrder") && !key.equalsIgnoreCase("id"))
     {
      String val = request.getParameter(key);
      updates.put(key, val);
     } 
    }
    
    session.updateInputOrder(action,updates);
    JSONObject ret = new JSONObject().add("result", true);
    return ret;
   }
   else if (updateOrder != null && updateOrder.equalsIgnoreCase("Y"))
   {
    ArrayList<String> updates = new ArrayList<String>();    
    int poskeyscnt = 0;
    
    Object[] keys = request.getParameterMap().keySet().toArray();
    
    for (int j=0;j<keys.length;j++)
    {
     String key = (String)keys[j];
     if (!key.equalsIgnoreCase("updateOrder") && !key.equalsIgnoreCase("id"))
     {
      poskeyscnt++;
     } 
    }
    
    for (int j=1;j<(poskeyscnt+1);j++)
    {
     String val = request.getParameter("pos_" + j);
     updates.add(val);
    }
    
    session.updateOrder(action,updates);
    JSONObject ret = new JSONObject().add("result", true);
    return ret;
   }
   else if (removeArg != null && removeArg.equalsIgnoreCase("Y"))
   {
    String name = request.getParameter("name");
    session.removeArg(actionid,name);

    JSONObject ret = new JSONObject().add("result", true);
    return ret;
   }
   else if (switchAction != null && (switchAction.equalsIgnoreCase("A") || switchAction.equalsIgnoreCase("U")))
   {
    if (switchAction.equalsIgnoreCase("A"))
    {
     String name = request.getParameter("name");
     String flag = request.getParameter("flag");
     session.addSwitch(actionid,name,flag);
    }
    else
    {
     String name = request.getParameter("name");
     String flag = request.getParameter("flag");
     session.updateSwitch(actionid,name,flag);     
    }
    JSONObject ret = new JSONObject().add("result", true);
    return ret;
   }
   else
   {
    // Write back the arg data
    JSONObject obj = new JSONObject();

    try
    {
     ACDChangeSet<Action.ActionArg> changes = new ACDChangeSet<Action.ActionArg>();

     for (Object oparam : request.getParameterMap().keySet())
     {
      String param = (String) oparam;
      if (param.startsWith("chg_"))
      {
       String key = param.substring(4);
       String val = request.getParameter(param);
       System.out.println("change '" + key + "' = '" + val + "'");
       changes.addChanged(processArg(action, key, val));
      }
      else if (param.startsWith("add_"))
      {
       String key = param.substring(4);
       String val = request.getParameter(param);
       System.out.println("add '" + key + "' = '" + val + "'");
       changes.addAdded(processArg(action, key, val));
      }
      else if (param.startsWith("del_"))
      {
       String key = param.substring(4);
       String val = request.getParameter(param);
       System.out.println("delete '" + key + "' = '" + val + "'");
       changes.addDeleted(processArg(action, key, val));
      }
     }

     if (action.isUpdatable())
     {
      boolean res = action.updateArgs(changes);
      obj.add("saved", res);
     }
     else
     {
      obj.add("saved", false);
      obj.add("error", "You do not have permission to update this object");
     }
    }
    catch (Exception e)
    {
     e.printStackTrace();
     obj.add("saved", false);
     obj.add("error", e.getMessage());
    }
    return obj;
   }
		}
	}
	
	/**
	 * Decodes a value of the form "<reqd><pad><swmode><outpos>&<name>&<sw1>&<sw2>"
	 */
	private Action.ActionArg processArg(Action action, String key, String val)
	{
		// Three single char fields and three separators
		System.out.println("val="+val);
		if(val.length() < 6) {
			throw new RuntimeException("Invalid action arg value: too short");
		}
		
		String[] parts = val.split("&", 6);
		if(parts.length != 6) {
			System.out.println("parts[0]="+parts[0]);
			System.out.println("parts[1]="+parts[1]);
			System.out.println("parts[2]="+parts[2]);
			System.out.println("parts[3]="+parts[3]);
			System.out.println("parts[4]="+parts[4]);
			System.out.println("parts[5]="+parts[5]);
			throw new RuntimeException("Invalid action arg value: wrong number of parts ("+parts.length+")");
		}
		int inpos = Integer.parseInt(parts[1]);
		String name = urlDecode(parts[2]);
		String sw1 = urlDecode(parts[3]);
		String sw2 = urlDecode(parts[4]);
		String type = urlDecode(parts[5]);
		
		boolean reqd = parts[0].charAt(0) == 'Y';
		boolean pad = parts[0].charAt(1) == 'Y';
		Action.SwitchMode mode = Action.SwitchMode.fromString(parts[0].substring(2,3));
		int outpos = 0;
		try {
			outpos = Integer.parseInt(parts[0].substring(3));
		} catch(NumberFormatException e) {
			e.printStackTrace();
			throw new RuntimeException("Invalid action arg value: invalid outpos");
		}
		
		// TODO: need to pass inpos for functions
		Action.ActionArg ret = action.new ActionArg(key, name, type, reqd, pad, inpos, outpos, mode, sw1, sw2);
		System.out.println("arg " + key + " = " + ret.getTextWithMarkup());
		return ret;
	}
}
