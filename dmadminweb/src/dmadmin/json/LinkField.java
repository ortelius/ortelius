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

package dmadmin.json;

import dmadmin.ObjectType;
import dmadmin.model.ActionKind;

public class LinkField
	implements IJSONSerializable
{
	private String m_type;
	private int m_objId;
	private int m_deploymentid;
	private String m_name;
	private String m_tooltip;
	private boolean m_showLink;
	private ActionKind m_kind;

	// TODO: change showLink to default to false
	public LinkField(ObjectType type, int objId, String name) {
		this(type.getTypeString(), objId, name, null, true, ActionKind.UNCONFIGURED);
	}

	// TODO: change showLink to default to false
	public LinkField(ObjectType type, int objId, String name, String tooltip) {
		this(type.getTypeString(), objId, name, tooltip, true, ActionKind.UNCONFIGURED);
	}

	public LinkField(ObjectType type, int objId, String name, boolean showLink) {
		this(type.getTypeString(), objId, name, null, showLink, ActionKind.UNCONFIGURED);
	}

	public LinkField(ObjectType type, int objId, String name, ActionKind kind) {
		this(type.getTypeString(), objId, name, null, true, kind);
	}

	public LinkField(ObjectType type, int objId, String name, boolean showLink, ActionKind kind) {
		this(type.getTypeString(), objId, name, null, showLink, kind);
	}

	public LinkField(String type, int objId, String name, String tooltip, boolean showLink, ActionKind kind) {
		if (tooltip == null || tooltip.length()==0) tooltip = name;
		m_type = type;
		m_objId = objId;
		m_name = name;
		m_tooltip = tooltip;
		m_showLink = showLink;
		m_kind = kind;
	}

	public int getId()
	{
	 return m_objId;
	}


	public int getDeploymentid()
 {
  return m_deploymentid;
 }

 public void setDeploymentid(int m_deploymentid)
 {
  this.m_deploymentid = m_deploymentid;
 }

 public LinkField setShowLink(boolean showLink) {
		m_showLink = showLink;
		return this;
	}

	public JSONObject getJSONObject() {
		JSONObject obj = new JSONObject();
		obj.add("type", m_type)
			.add("id", m_objId)
			.add("name", m_name);
		if(m_tooltip != null) {
			obj.add("tooltip", m_tooltip);
		}
		if (m_kind != ActionKind.UNCONFIGURED) {
			obj.add("kind", m_kind.value());
		}
		obj.add("showlink", m_showLink);
		return obj;
	}

	@Override
	public String getJSON() {
		return getJSONObject().getJSON();
	}
}
