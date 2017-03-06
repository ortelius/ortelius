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
package dmadmin.json;

import dmadmin.ObjectType;
import dmadmin.model.ActionKind;

public class LinkField
	implements IJSONSerializable
{
	private String m_type;
	private int m_objId;
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
