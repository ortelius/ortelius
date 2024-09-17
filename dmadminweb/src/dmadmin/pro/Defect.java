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

import java.util.Calendar;
import java.util.Date;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;

public class Defect
{
 private String m_id;
 private int m_compid;
 private int m_appid;
 private String m_title;
 private String m_apiurl;
 private String m_htmlurl;
 private int m_deploymentid;
 private long m_modified;
 private int m_modifierid;
 private String m_status;
 private String m_colour;

 public Defect(String id, String title, String apiurl, String htmlurl)
 {
  m_id = id;
  m_title = title;
  m_apiurl = apiurl;
  m_htmlurl = htmlurl;
  m_compid = 0;
  m_deploymentid = 0;
  m_status = "";
  m_colour = "";
 }

 public String getId()
 {
  return m_id;
 }

 public String getName()
 {
  return m_id;
 }

 public String getTitle()
 {
  return m_title;
 }

 public String getApiUrl()
 {
  return m_apiurl;
 }

 public String getHtmlUrl()
 {
  return m_htmlurl;
 }

 public String getStatus()
 {
  return m_status;
 }

 public String getColour()
 {
  return m_colour;
 }

 public int getAppId()
 {
  return m_appid;
 }

 public int getCompId()
 {
  return m_compid;
 }

 public int getDeploymentId()
 {
  return m_deploymentid;
 }

 public long getModified()
 {
  return m_modified;
 }

 public String getModifiedAsLocale()
 {
  Calendar x = Calendar.getInstance();
  Date d = new Date(m_modified * 1000);
  x.setTime(d);
  return x.get(Calendar.DAY_OF_MONTH) + "/" + (x.get(Calendar.MONTH) + 1) + "/" + x.get(Calendar.YEAR) + " " + x.get(Calendar.HOUR_OF_DAY) + ":" + x.get(Calendar.MINUTE) + ":" + x.get(Calendar.SECOND);
 }

 public int getModifier()
 {
  return m_modifierid;
 }

 public IJSONSerializable getJSON()
 {
  JSONObject dep = new JSONObject();
  dep.add("id", m_id);
  dep.add("title", m_title);
  dep.add("htmlurl", m_htmlurl);
  dep.add("status", m_status);
  return dep;
 }

 public void setId(String id)
 {
  m_id = id;
 }

 public void setTitle(String title)
 {
  m_title = title;
 }

 public void setApiUrl(String url)
 {
  m_apiurl = url;
 }

 public void setHtmlUrl(String url)
 {
  m_htmlurl = url;
 }

 public void setStatus(String status)
 {
  m_status = status;
 }

 public void setColour(String colour)
 {
  m_colour = colour;
 }

 public void setCompId(int compid)
 {
  m_compid = compid;
 }

 public void setAppId(int appid)
 {
  m_appid = appid;
 }

 public void setDeploymentId(int deploymentid)
 {
  m_deploymentid = deploymentid;
 }

 public void setModifierId(int modifierid)
 {
  m_modifierid = modifierid;
 }

 public void setModified(long modified)
 {
  m_modified = modified;
 }
}
