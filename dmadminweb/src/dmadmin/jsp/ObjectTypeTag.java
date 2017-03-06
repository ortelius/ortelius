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
package dmadmin.jsp;

import java.io.IOException;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspWriter;
import javax.servlet.jsp.tagext.SimpleTagSupport;

import dmadmin.ObjectType;

public class ObjectTypeTag
	extends SimpleTagSupport
{
	private String m_valueOf;
	private String m_nameOf;
	
	public void setValueOf(String value) {
		m_valueOf = value;
	}
	
	public void setNameOf(String value) {
		m_nameOf = value;
	}
	
	@Override
	public void doTag()
		throws JspException, IOException
	{
		// Check attributes - we need one or the other not both
		if((m_valueOf == null) && (m_nameOf == null)) {
			throw new JspException("Either valueOf or nameOf must be specified");
		}
		if((m_valueOf != null) && (m_nameOf != null)) {
			throw new JspException("Only one of valueOf or nameOf must be specified");
		}
		// Lookup the value
		String value = (m_valueOf != null) ? m_valueOf : m_nameOf;
		ObjectType match = null;
		for(ObjectType ot : ObjectType.values()) {
			if(value.equals(ot.name())) {
				match = ot;
				break;
			}
		}
		// If not found report an error
		if(match == null) {
			throw new JspException("Not a valid ObjectType: " + value);			
		}
		// Output either the integer value or the type string (for OTID)
		JspWriter out = getJspContext().getOut();
        System.out.println("ObjectTypeTag: " + value + " -> " + ((m_valueOf != null) ? match.value() : match.getTypeString()));
        out.println((m_valueOf != null) ? match.value() : ("'" + match.getTypeString() + "'"));
	}
}
