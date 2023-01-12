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
