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

package dmadmin.model;

import java.io.Writer;
import java.util.Hashtable;
import java.util.Map;

import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.SummaryChangeSet;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;

public class Attachment
	extends DMObject
{
	private static final long serialVersionUID = 903806223335945203L;
	
	private static class MimeType
	{
		private String m_contentType;
		private String m_icon;
		
		public MimeType(String ct, String icon) {
			m_contentType = ct;
			m_icon = icon;
		}
		
		public String getContentType()  { return m_contentType; }
		public String getIcon()  { return m_icon; }
	}

	private static Map<String, MimeType> s_mimeTypeMap;
	
	private long m_size;

	public Attachment()
	{}
	
	public Attachment(DMSession session, int attachid, String filename)
	{
		super(session, attachid, filename);
	}
	
	public long getSize()  { return m_size; }
	public void setSize(long size)  { m_size = size; }
	
	public String getExtension()
	{
		if(m_name == null) {
			return "";
		}
		
		int lastdot = m_name.lastIndexOf('.');
		if(lastdot == -1) {
			return "";
		}
		
		return m_name.substring(lastdot+1);
	}
	
	public String getIcon()
	{
		MimeType mt = getMimeTypeMap().get(getExtension().toLowerCase());
		return (mt != null) ? mt.getIcon() : null;
	}
	
	public String getMimeType()
	{			
		MimeType mt = getMimeTypeMap().get(getExtension().toLowerCase());
		return (mt != null) ? mt.getContentType() : null;
	}
	
	public String getAttachmentString()
	{
		return m_session.getAttachmentString(m_id);
		// return new File(s_attachStore, m_id + "_" + m_name);
	}
	
	public void streamOutput(Writer out)
	{}

	@Override
	public ObjectType getObjectType() {
		return null;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_historyattachment";
	}

	@Override
	public String getForeignKey() {
		return null;
	}
	
	public IJSONSerializable getJSONObject() {
		JSONObject obj = new JSONObject();
		obj.add("id", m_id);
		obj.add("filename", m_name);
		obj.add("size", m_size);
		obj.add("icon", getIcon());
		return obj;
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		return null;
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return false;
	}
	
	private static Map<String, MimeType> getMimeTypeMap() {
		if(s_mimeTypeMap == null) {
			s_mimeTypeMap = new Hashtable<String, MimeType>();
			s_mimeTypeMap.put("csv",  new MimeType("text/csv", "xls"));
			s_mimeTypeMap.put("doc",  new MimeType("application/msword", "doc"));
			s_mimeTypeMap.put("docx", new MimeType("application/vnd.openxmlformats-officedocument.wordprocessingml.document", "doc"));
			s_mimeTypeMap.put("dot",  new MimeType("application/msword", "doc"));
			s_mimeTypeMap.put("dotx", new MimeType("application/vnd.openxmlformats-officedocument.wordprocessingml.template", "doc"));
			s_mimeTypeMap.put("gif",  new MimeType("image/gif", "img"));
			s_mimeTypeMap.put("jpg",  new MimeType("image/jpeg", "img"));
			s_mimeTypeMap.put("jpeg", new MimeType("image/jpeg", "img"));
			// TODO: pdf
			s_mimeTypeMap.put("ppt",  new MimeType("application/vnd.ms-powerpoint", "ppt"));
			s_mimeTypeMap.put("pptx", new MimeType("application/vnd.openxmlformats-officedocument.presentationml.presentation", "ppt"));
			s_mimeTypeMap.put("pot",  new MimeType("application/vnd.ms-powerpoint", "ppt"));
			s_mimeTypeMap.put("potx", new MimeType("application/vnd.openxmlformats-officedocument.presentationml.template", "ppt"));
			s_mimeTypeMap.put("png",  new MimeType("image/png", "img"));
			s_mimeTypeMap.put("tif",  new MimeType("image/tiff", "img"));
			s_mimeTypeMap.put("tiff", new MimeType("image/tiff", "img"));
			s_mimeTypeMap.put("txt",  new MimeType("text/plain", "txt"));
			s_mimeTypeMap.put("xla",  new MimeType("application/vnd.ms-excel", "xls"));
			s_mimeTypeMap.put("xls",  new MimeType("application/vnd.ms-excel", "xls"));
			s_mimeTypeMap.put("xlsx", new MimeType("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xls"));
			s_mimeTypeMap.put("xlt",  new MimeType("application/vnd.ms-excel", "xls"));
			s_mimeTypeMap.put("xltx", new MimeType("application/vnd.openxmlformats-officedocument.spreadsheetml.template", "xls"));
		}
		return s_mimeTypeMap;
	}
}
