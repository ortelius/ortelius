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
import java.io.InputStream;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.fileupload.FileItem;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONObject;
import dmadmin.model.Attachment;


/**
 * Servlet implementation class History New Feed
 */
public class NewsFeed
	extends JSONMultipartServletBase
{
	private static final long serialVersionUID = 8314171259294636898L;


    public NewsFeed() {
        super();
    }


	@Override
	public IJSONSerializable handleRequest(DMSession session, boolean isPost,
			HttpServletRequest requestNotUsed, HttpServletResponse response,
			HttpParameters params, List<FileItem> files)
		throws ServletException, IOException
	{
		// Either result or data go in here
		JSONObject obj = null;

		//for(Object name : request.getParameterMap().keySet()) {
		//	System.out.println("PARAM: " + name + " = '" + request.getParameter((String) name) + "'");
		//}
		for(String name : params.keySet()) {
			System.out.println("PARAM: " + name + " = '" + params.get(name) + "'");
		}

		String sotid = params.get("otid");
		ObjectTypeAndId otid = ((sotid != null) && (sotid.length() > 0)) ? new ObjectTypeAndId(sotid) : null;

		String reason = params.get("reason");
		if(reason != null) {
			if(sotid == null) {
				throw new RuntimeException("otid is required");
			}
			if(reason.equalsIgnoreCase("comments")) {
				obj = doGetComments(session, otid);
			} else if(reason.equalsIgnoreCase("attachments")) {
				obj = doGetAttachments(session, otid);
			} else if(reason.equalsIgnoreCase("add")) {
				obj = doAddNote(session, otid, params, files);
			} else if(reason.equalsIgnoreCase("addcmnt")) {
				obj = doAddComment(session, otid, params);
			} else if(reason.equalsIgnoreCase("subscribe")) {
				obj = doSubscribe(session, otid, false);
			} else if(reason.equalsIgnoreCase("unsubscribe")) {
				obj = doSubscribe(session, otid, true);
   } else if(reason.equalsIgnoreCase("applogs")) {
    obj = doAppLogs(session, otid);
			} else {
				throw new RuntimeException("Bad reason '" + reason + "' given to news feed");
			}
		} else {
			obj = doGetNotes(session, otid, params);
		}

		return obj;
	}

	private JSONObject doGetComments(DMSession session, ObjectTypeAndId otid)
	{
		JSONObject obj = new JSONObject();
		try {
			NewsFeedDataSet data = session.getCommentsForObject(otid);
			obj.add("result", (data != null));
			if(data != null) {
				obj.add("data", data.getJSON());
			}
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}

 private JSONObject doAppLogs(DMSession session, ObjectTypeAndId otid)
 {
  JSONObject obj = new JSONObject();
  try {
   JSONArray data = session.getApplicationDeployments(otid);
   obj.add("result", (data != null));
   if(data != null) {
    obj.add("data", data);
   }
  } catch(Exception e) {
   e.printStackTrace();
   obj.add("result", false);
   obj.add("error", e.getMessage());
  }
  return obj;
 }

	private JSONObject doGetAttachments(DMSession session, ObjectTypeAndId otid)
	{
		JSONObject obj = new JSONObject();
		try {
			List<Attachment> attaches = session.getAttachmentsForObject(otid);
			JSONArray data = new JSONArray();
			for(Attachment attach: attaches) {
				data.add(attach.getJSONObject());
			}
			obj.add("result", true);
			obj.add("data", data);
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}

	private JSONObject doAddNote(DMSession session, ObjectTypeAndId otid, HttpParameters params, List<FileItem> files)
	{
		JSONObject obj = new JSONObject();
		try {
			String text = params.get("text");
			String icon = params.get("icon");
			System.out.println("addnews: otid = " + otid + "; text = '" + text + "'; icon = '" + icon + "'");

			PropertyDataSet data = session.addNewsToObject(otid, text, icon);
			obj.add("result", (data != null));
			if(data == null) {
				return obj;
			}

			if(files != null) {
				int attachCount = 0;
				for(FileItem file : files) {
					InputStream is = file.getInputStream();
					int attachid = session.addAttachmentToObject(data.getNewObject(), file.getName(), (int) file.getSize(), is);
					if(attachid != 0) {
						attachCount++;
					}
				}
				data.addProperty("attach", attachCount);
			}

			obj.add("data", data.getNewsJSON());
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}

	private JSONObject doAddComment(DMSession session, ObjectTypeAndId otid, HttpParameters params)
	{
		JSONObject obj = new JSONObject();
		try {
			String text = params.get("text");
			PropertyDataSet data = session.addCommentToObject(otid, text);
			obj.add("result", (data != null));
			if(data != null) {
				obj.add("data", data.getNewsJSON());
			}
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}

	private JSONObject doSubscribe(DMSession session, ObjectTypeAndId otid, boolean unsubscribe)
	{
		JSONObject obj = new JSONObject();
		try {
			if(unsubscribe) {
				session.newsUnsubscribeObject(otid);
			} else {
				session.newsSubscribeObject(otid);
			}
			obj.add("result", true);
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}

	private JSONObject doGetNotes(DMSession session, ObjectTypeAndId otid, HttpParameters params)
	{
		JSONObject obj = new JSONObject();
		try {
			int from = params.getIntParameter("from", 0);
			int to = params.getIntParameter("to", 0);
			int pending = params.getIntParameter("pending", 0);

			System.out.println("goGetNotes: from="+from+" to="+to);

			if(from == 0) {
				from = (int)(System.currentTimeMillis()/1000) - (31*24*60*60);
			}

			if (to == 0)
			 to = (int)(System.currentTimeMillis()/1000)+10000;

			System.out.println("doGetNotes 2: from="+from+" to="+to);

			if(pending == 1) {
				System.out.println("getpending: otid = " + otid + "; from = " + from + "; to = " + to);
				NewsFeedDataSet data = session.getPendingNewsForObject(otid, from, to);
				obj.add("data", data.getJSON());
			} else {
				System.out.println("gethistory: otid = " + otid + "; from = " + from + "; to = " + to);
				NewsFeedDataSet data = session.getHistoryNewsForObject(otid, from, to);
				obj.add("data", data.getJSON());
			}
			obj.add("result", true);
		} catch(Exception e) {
			e.printStackTrace();
			obj.add("result", false);
			obj.add("error", e.getMessage());
		}
		return obj;
	}
}
