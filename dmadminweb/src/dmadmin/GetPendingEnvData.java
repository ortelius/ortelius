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
import java.util.Hashtable;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Domain;
import dmadmin.model.Environment;

/**
 * Servlet implementation class GetAppVersInEnvData
 */
public class GetPendingEnvData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;
       
    public GetPendingEnvData() {
        super();
    }
    
	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int envid = getIntParameter(request, "envid", 0);
		int appid = getIntParameter(request, "appid", 0);
		if((envid == 0) && (appid == 0)) {
			return new JSONObject().add("result", false).add("error", "envid or appid must be specified");
		}
		
		System.out.println("appid="+appid+" envid="+envid);

		JSONObject obj = new JSONObject();

		if(envid > 0) {
			Environment env = session.getEnvironment(envid, false);
			
			obj.add("readOnly", !env.isUpdatable());
			
			TableDataSet data = session.getPendingEnvData(env); 
			obj.add("data", data);
		} else if(appid > 0) {
			//
			// Return where the application (or app version) is due to be deployed
			//
			Application app = session.getApplication(appid, true);
			boolean isRelease = app.getIsRelease().equalsIgnoreCase("y");
			
			obj.add("readOnly", !app.isUpdatable());
			
			TableDataSet data = session.getPendingEnvData(app);
			Hashtable<Integer, Boolean> dc = new Hashtable<Integer, Boolean>();
			JSONArray darr = new JSONArray();
			if (isRelease) {
				// Release timeline is grouped by application version
				System.out.println("row count="+data.getRows());
				for (int x=0;x<=data.getRows();x++) {
					int grpid = (Integer)data.get(x,8);
					String grpname = (String)data.get(x,9);
					Application a = session.getApplication(grpid,false);
					System.out.println(x+") grpid = "+grpid+" grpname="+grpname);
					if (dc.get(grpid) == null) {
						JSONObject dobj = new JSONObject();
						dobj.add("content", grpname);
						dobj.add("id", grpid);
						dobj.add("value",x);
						dobj.add("type",(a.getParentId()>0)?"av":"ap");
						// for now
						// int bopen = (int) Math.abs(Math.random() * 100);
						// int bclosed = (int) Math.abs(Math.random() * 100);
						int res[] = session.getDefectCountsForApplication(a);
						dobj.add("bopen",res[0]);
						dobj.add("bclosed",res[1]);
						darr.add(dobj);
						dc.put(grpid,true);
					}
				}
				// Now add the Release itself
				if (dc.get(app.getId()) == null) {
					JSONObject dobj = new JSONObject();
					dobj.add("content",app.getName());
					dobj.add("id",app.getId());
					dobj.add("value",data.getRows());	// to place it at bottom of list
					dobj.add("type",(app.getParentId()>0)?"rv":"rl");
					darr.add(dobj);
				}
			} else {
				// Application timeline is grouped by domain
				int val=0;
				for (int x=0;x<data.getRows();x++) {
					// If this domain is part of a lifecycle then we need to get the siblings
					Domain d = session.getDomain((Integer)data.get(x,3));
					if (d == null) continue;
					if (d.getLifecycle()) {
						// This domain is a lifecycle domain - include children
						List<Domain> cds = session.getChildDomains(d);
						for (Domain cd: cds) {
							val++;
							if (dc.get(cd.getId()) == null) {
								System.out.println(cd.getId()+" not yet in dc");
								JSONObject dobj = new JSONObject();
								dobj.add("content", cd.getName());
								dobj.add("id", cd.getId());
								dobj.add("value",val);
								darr.add(dobj);
								System.out.println("adding "+cd.getId()+" to dc val="+val);
								dc.put(cd.getId(),true);
							}
						}
					} else {
						// Not a lifecycle domain - what about the parent?
						Domain pardom = d.getDomain();
						if (pardom != null && pardom.getLifecycle()) {
							// This is a lifecycle domain - add the children
							List<Domain> cds = session.getChildDomains(pardom);
							for (Domain cd: cds) {
								val++;
								if (dc.get(cd.getId()) == null) {
									System.out.println(cd.getId()+" not yet in dc");
									JSONObject dobj = new JSONObject();
									dobj.add("content", cd.getName());
									dobj.add("id", cd.getId());
									dobj.add("value",val);
									darr.add(dobj);
									System.out.println("adding "+cd.getId()+" to dc val="+val);
									dc.put(cd.getId(),true);
								}
							}
						} else {
							// We're not in a lifecycle - just include this domain
							if (dc.get(d.getId()) == null) {
								JSONObject dobj = new JSONObject();
								dobj.add("content", d.getName());
								dobj.add("id", d.getId());
								dobj.add("value",x);
								darr.add(dobj);
								System.out.println("adding "+d.getId()+" to dc");
								dc.put(d.getId(),true);
							}
						}
					}
				}
			}
			
			System.out.println("app is "+app.getId());
			obj.add("domains",darr);
			obj.add("data", data);
		}
		
		return obj;
	}
}

