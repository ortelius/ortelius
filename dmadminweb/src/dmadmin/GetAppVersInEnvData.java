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
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Environment;

/**
 * Servlet implementation class GetAppVersInEnvData
 */
public class GetAppVersInEnvData
	extends JSONServletBase
{
	private static final long serialVersionUID = 1L;

    public GetAppVersInEnvData() {
        super();
    }

	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int envid = getIntParameter(request, "envid", 0);
		int appid = getIntParameter(request, "appid", 0);
		int delenvid = -1;
		if((envid == 0) && (appid == 0)) {
			return new JSONObject().add("result", false).add("error", "envid or appid must be specified");
		}

		String reason = request.getParameter("reason");
		String oldenv = request.getParameter("oldenv");

		if (oldenv != null && !oldenv.isEmpty())
		 delenvid = new Integer(oldenv).intValue();

		System.out.println("GetAppVersInEnvData - reason="+reason);

		if(reason != null) {
			if (reason.equalsIgnoreCase("envlist")) {
				return getEnvironmentList(session);
			} else if (reason.equalsIgnoreCase("tgtenvlist")) {
				int tid = getIntParameter(request, "tid", 0);
				Application app = session.getApplication(appid, true);
				List<Environment> envs = app.getPermittedEnvironments(tid);
				TableDataSet data = session.envListToTableData(envs);
				System.out.println("Done, data="+data.getJSON());
				JSONObject obj = new JSONObject();
				obj.add("data", data);
				return obj;
			} else if (reason.equalsIgnoreCase("depenvlist")) {
				int tid = getIntParameter(request, "tid", 0);
				Application app = session.getApplication(appid, false);
				List<Environment> envs = app.getDeployedEnvironments(tid);
				TableDataSet data = session.envListToTableData(envs);
				System.out.println("Done, data="+data.getJSON());
				JSONObject obj = new JSONObject();
				obj.add("data", data);
				return obj;
			} else if(reason.equalsIgnoreCase("add")) {
				if((envid == 0) || (appid == 0)) {
					return new JSONObject().add("result", false).add("error", "Both envid and appid must be specified");
				}
				String note = request.getParameter("note");
				boolean isVersion = Boolean.parseBoolean(request.getParameter("isversion"));
				if (delenvid > 0)
				{
				 Application app = session.getApplication(appid, false);
				 Environment env = session.getEnvironment(delenvid, false);
				 session.clearAppInEnv(env, app);
				}
				setAppInEnv(session, envid, appid, isVersion, note);
				return addToAppsAllowedInEnv(session, envid, appid);
			} else if(reason.equalsIgnoreCase("set")) {
				if((envid == 0) || (appid == 0)) {
					return new JSONObject().add("result", false).add("error", "Both envid and appid must be specified");
				}
				String note = request.getParameter("note");
				boolean isVersion = Boolean.parseBoolean(request.getParameter("isversion"));
				System.out.println("isVersion="+isVersion);
				setAppInEnv(session, envid, appid, isVersion, note);
				return addToAppsAllowedInEnv(session, envid, appid);
			} else if(reason.equalsIgnoreCase("rem")) {
				if((envid == 0) || (appid == 0)) {
					return new JSONObject().add("result", false).add("error", "Both envid and appid must be specified");
				}
				return removeFromAppsAllowedInEnv(session, envid, appid);
			} else if(reason.equalsIgnoreCase("clr")) {
				if((envid == 0) || (appid == 0)) {
					return new JSONObject().add("result", false).add("error", "Both envid and appid must be specified");
				}
				return clearAppInEnv(session, envid, appid);
			} else {
				return new JSONObject().add("result", false).add("error", "Invalid reason");
			}
		}

		JSONObject obj = new JSONObject();

		if(envid > 0) {
			Environment env = session.getEnvironment(envid, false);

			obj.add("readOnly", !env.isUpdatable());

			TableDataSet data = session.getAppVersInEnvData(env);
			obj.add("data", data);
		} else if(appid > 0) {
			Application app = session.getApplication(appid, false);

			obj.add("readOnly", !app.isUpdatable());

			System.out.println("Calling getAppVersInEnvData for app "+app.getId());
			TableDataSet data = session.getAppVersInEnvData(app);
			System.out.println("Done, data="+data.getJSON());
			obj.add("data", data);
		}

		return obj;
	}

	private IJSONSerializable getEnvironmentList(DMSession session)
	{
		List<Environment> envs =  session.getEnvironmentsInDomain();
		JSONArray domarr = new JSONArray();
		JSONArray envarr = new JSONArray();

		Environment LastEnvironment = null;
		for(Environment e : envs) {
			// list is ordered by domain first
			System.out.println("Got environment "+e.getName()+", domain="+session.getDomainName(e.getDomainId()));
			if (LastEnvironment != null && (e.getDomainId() != LastEnvironment.getDomainId())) {
				// Domain Changed
					System.out.println("************************");
					System.out.println("Adding domain "+session.getDomainName(LastEnvironment.getDomainId()));
					System.out.println("envarr="+envarr.getJSON());
					System.out.println("************************");
					domarr.add(new JSONObject()
					.add("id",LastEnvironment.getDomainId())
					.add("name",session.getDomainName(LastEnvironment.getDomainId()))
					.add("env",envarr));
					envarr = new JSONArray();
			}
			System.out.println("Adding environment "+e.getName()+" to envarr");
			envarr.add(new JSONObject()
					.add("id", e.getId())
					.add("name",e.getName())
					.add("summary",e.getSummary()));
			LastEnvironment = e;
		}
		// End of environment list
		System.out.println("******************** END OF LOOP THROUGH ENVIRONMENTS **********************");
		if (LastEnvironment != null) {
			System.out.println("END: envarr="+envarr.getJSON());
			System.out.println("END: adding envarr and domain "+session.getDomainName(LastEnvironment.getDomainId()));
			domarr.add(new JSONObject()
			.add("id",LastEnvironment.getDomainId())
			.add("name",session.getDomainName(LastEnvironment.getDomainId()))
			.add("env",envarr));
		}
		JSONObject obj = new JSONObject();
		obj.add("data", domarr);
		return obj;
	}

	private IJSONSerializable addToAppsAllowedInEnv(DMSession session, int envid, int appid)
	{
		System.out.println("GetAppVersInEnvData.addToAppsAllowedInEnv - " + envid + ", " + appid);
		try {
			Environment env = session.getEnvironment(envid, false);
			Application app = session.getApplication(appid, false);
			if(!env.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result = session.addToAppsAllowedInEnv(env, app);
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}

	private IJSONSerializable setAppInEnv(DMSession session, int envid, int appid, boolean isVersion, String note)
	{
		System.out.println("GetAppVersInEnvData.setAppInEnv - " + envid + ", " + appid + ", " + isVersion + ", " + note);
		try {
			Environment env = session.getEnvironment(envid, false);
			Application app = session.getApplication(appid, false);
			if(!env.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result;
			if (isVersion) {
				result = session.setAppInEnv(env, app, note, -1);
			} else {
				System.out.println("*** adding app="+appid+" env="+envid+" to apps allowed in env");
				result = session.addToAppsAllowedInEnv(env, app);
				System.out.println("*** result="+result);
			}
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}

	private IJSONSerializable removeFromAppsAllowedInEnv(DMSession session, int envid, int appid)
	{
		System.out.println("GetAppVersInEnvData.removeFromAppsAllowedInEnv - " + envid + ", " + appid);
		try {
			Environment env = session.getEnvironment(envid, false);
			Application app = session.getApplication(appid, false);
			if(!env.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result = session.removeFromAppsAllowedInEnv(env, app);
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}

	private IJSONSerializable clearAppInEnv(DMSession session, int envid, int appid)
	{
		System.out.println("GetAppVersInEnvData.clearAppInEnv - " + envid + ", " + appid);
		try {
			Environment env = session.getEnvironment(envid, false);
			Application app = session.getApplication(appid, false);
			if(!env.isUpdatable()) {
				return new JSONObject().add("result", false).add("error", "Permision denied");
			}
			boolean result = session.clearAppInEnv(env, app);
			return new JSONObject().add("result", result);
		} catch(Exception e) {
			e.printStackTrace();
			return new JSONObject().add("result", false).add("error", e);
		}
	}
}
