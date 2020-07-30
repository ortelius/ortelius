/*
 *
 *  Ortelius for Microservice Configuration Mapping
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

import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;

public class SummaryChangeSet
	implements Iterable<SummaryField>
{
	private List<SummaryField> m_fields = new ArrayList<SummaryField>();
	private Hashtable<SummaryField,Object> m_values = new Hashtable<SummaryField,Object>();
	
	public SummaryChangeSet()
	{}
	
	public void add(SummaryField field, Object value)
	{
		m_fields.add(field);
		if(value != null) {
			m_values.put(field, value);
		}
	}
	
	public Object get(SummaryField field)
	{
		return m_values.get(field);
	}

	public String getBoolean(SummaryField field)
	{
		boolean ret = false;
		Object obj = m_values.get(field);
		if((obj != null) && (obj instanceof String)) {
			System.out.println("getBoolean - String - " + obj);
			ret = ((String) obj).equalsIgnoreCase("true"); 
		} else if(obj instanceof Boolean) {
			System.out.println("getBoolean - Boolean - " + obj);
			ret = ((Boolean) obj).booleanValue(); 
		}
		return (ret ? "Y" : "N");
	}
	
	public boolean isEmpty()
	{
		return m_fields.isEmpty();
	}

	@Override
	public Iterator<SummaryField> iterator() {
		return m_fields.iterator();
	}
}
