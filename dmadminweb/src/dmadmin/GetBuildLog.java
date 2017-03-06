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

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.BuildJob;
import dmadmin.model.Component;

/**
 * Servlet implementation class GetDeploymentLog
 */
public class GetBuildLog extends JSONServletBase
{
 private static final long serialVersionUID = 4402903956198244069L;

 /**
  * @see JSONServletBase#JSONServletBase()
  */
 public GetBuildLog()
 {
  super();
 }

 @Override
 public IJSONSerializable handleRequest(DMSession session, boolean isPost, HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
 {
  int buildid = getIntParameter(request, "buildid");
  int compid = getIntParameter(request, "compid", 0);
  int jobid = getIntParameter(request, "jobid", 0);

  System.out.println("deployid = " + buildid + "; compid = " + compid + "; jobid=" + jobid);

  String res = "";
  JSONArray comps = null;
  JSONArray files = null;
  IJSONSerializable details = null;
  BuildJob buildjob = null;
  if (compid > 0)
  {
   Component comp = session.getComponent(compid, true);
   buildjob = comp.getBuildJob();
  }
  else
  {
   buildjob = session.getBuildJob(jobid);
  }
  if (buildjob != null)
  {
   res = buildjob.getBuildLog(buildid);
   comps = buildjob.getBuildTargets(buildid);
   details = buildjob.getBuildDetails(buildid);
   files = buildjob.getBuildFiles(buildid);
  }
  JSONObject ret = new JSONObject();

  ret.add("buildlog", res);
  ret.add("comps", comps);
  ret.add("details", details);
  ret.add("files", files);
  return ret;
 }

}
