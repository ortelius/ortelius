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
 */package dmadmin.pro;

import java.util.ArrayList;
import java.util.List;

import dmadmin.DMSession;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Application;
import dmadmin.model.Component;

public class DefectList
{
 private ArrayList<Defect> m_defects = new ArrayList<Defect>();
 private String m_errorMessage;
 private boolean m_readOnly;
 private DMSession m_session;

 public DefectList(DMSession so)
 {
  m_session = so;
 }

 public void setDefects(ArrayList<Defect> defects)
 {
  m_defects = defects;
 }

 public void setErrorMessage(String errmsg)
 {
  m_errorMessage = errmsg;
 }

 public void setReadOnly(boolean readonly)
 {
  m_readOnly = readonly;
 }

 public List<Defect> getDefects()
 {
  return m_defects;
 }

 public boolean getStatus()
 {
  return (m_errorMessage == null);
 }

 public String getErrorMessage()
 {
  return m_errorMessage;
 }

 public IJSONSerializable getJSON()
 {
  JSONObject obj = new JSONObject();
  JSONArray arr = new JSONArray();
  obj.add("status", (m_errorMessage == null)); // if error message is null, status is "true" (success)
  obj.add("errorMessage", m_errorMessage);
  obj.add("readOnly", m_readOnly);
  if (m_defects != null)
  {
   for (Defect d : m_defects)
   {
    JSONObject dobj = new JSONObject();
    dobj.add("defectid", d.getId());
    dobj.add("deploymentid", d.getDeploymentId());
    dobj.add("timestr", d.getModifiedAsLocale());
    dobj.add("title", d.getTitle());
    dobj.add("apiurl", d.getApiUrl());
    dobj.add("htmlurl", d.getHtmlUrl());
    dobj.add("status", d.getStatus());
    dobj.add("colour", d.getColour());
    if (d.getCompId() > 0)
    {
     Component comp = m_session.getComponent(d.getCompId(), true);
     JSONObject cobj = new JSONObject();
     cobj.add("id", comp.getId());
     cobj.add("name", comp.getName());
     cobj.add("typestr", (comp.getParentId() > 0) ? "cv" : "co");
     dobj.add("component", cobj);
    }
    if (d.getAppId() > 0)
    {
     Application app = m_session.getApplication(d.getAppId(), true);
     JSONObject aobj = new JSONObject();
     aobj.add("id", app.getId());
     aobj.add("name", app.getName());
     aobj.add("typestr", (app.getParentId() > 0) ? "av" : "ap");
     dobj.add("application", aobj);
    }
    arr.add(dobj);
   }
  }
  obj.add("DefectCount", arr.length());
  obj.add("Defects", arr);
  return obj;
 }
}
