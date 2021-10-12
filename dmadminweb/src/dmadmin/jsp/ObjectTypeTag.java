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
