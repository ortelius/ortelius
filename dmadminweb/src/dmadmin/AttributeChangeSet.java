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

import dmadmin.model.DMAttribute;

public class AttributeChangeSet
{
	private List<DMAttribute> m_added   = new ArrayList<DMAttribute>();
	private List<DMAttribute> m_deleted = new ArrayList<DMAttribute>();
	private List<DMAttribute> m_changed = new ArrayList<DMAttribute>();
	private Hashtable<Integer, ArrayList<DMAttribute>> m_addedElements   = new Hashtable<Integer, ArrayList<DMAttribute>>();
	private Hashtable<Integer, ArrayList<DMAttribute>> m_deletedElements = new Hashtable<Integer, ArrayList<DMAttribute>>();
	private Hashtable<Integer, ArrayList<DMAttribute>> m_changedElements = new Hashtable<Integer, ArrayList<DMAttribute>>();
	
	public class AttributeIteratorDelegator
		implements Iterable<DMAttribute>
	{
		private Iterator<DMAttribute> m_iter;
		public AttributeIteratorDelegator(List<DMAttribute> list) {
			m_iter = list.iterator();
		}

		@Override
		public Iterator<DMAttribute> iterator() {
			return m_iter;
		}
	}
	
	public AttributeChangeSet()
	{}
	
	public void addAdded(DMAttribute attr)
	{
		m_added.add(attr);
	}
	
	public void addDeleted(DMAttribute attr)
	{
		m_deleted.add(attr);
	}
	
	public void addChanged(DMAttribute attr)
	{
		m_changed.add(attr);
	}

	public void addAddedElement(int arrayid, DMAttribute attr)
	{
		ArrayList<DMAttribute> list = m_addedElements.get(arrayid);
		if(list == null) {
			m_addedElements.put(arrayid, list = new ArrayList<DMAttribute>());
		}
		list.add(attr);
	}
	
	public void addDeletedElement(int arrayid, DMAttribute attr)
	{
		ArrayList<DMAttribute> list = m_deletedElements.get(arrayid);
		if(list == null) {
			m_deletedElements.put(arrayid, list = new ArrayList<DMAttribute>());
		}
		list.add(attr);
	}
	
	public void addChangedElement(int arrayid, DMAttribute attr)
	{
		ArrayList<DMAttribute> list = m_changedElements.get(arrayid);
		if(list == null) {
			m_changedElements.put(arrayid, list = new ArrayList<DMAttribute>());
		}
		list.add(attr);
	}

	public AttributeIteratorDelegator added() {
		return new AttributeIteratorDelegator(m_added);
	}

	public AttributeIteratorDelegator deleted() {
		return new AttributeIteratorDelegator(m_deleted);
	}

	public AttributeIteratorDelegator changed() {
		return new AttributeIteratorDelegator(m_changed);
	}

	public AttributeIteratorDelegator addedElements(int arrayid) {
		ArrayList<DMAttribute> list = m_addedElements.get(arrayid);
		return new AttributeIteratorDelegator((list != null) ? list : new ArrayList<DMAttribute>());
	}

	public AttributeIteratorDelegator deletedElements(int arrayid) {
		ArrayList<DMAttribute> list = m_deletedElements.get(arrayid);
		return new AttributeIteratorDelegator((list != null) ? list : new ArrayList<DMAttribute>());
	}

	public AttributeIteratorDelegator changedElements(int arrayid) {
		ArrayList<DMAttribute> list = m_changedElements.get(arrayid);
		return new AttributeIteratorDelegator((list != null) ? list : new ArrayList<DMAttribute>());
	}
	
	public boolean isEmpty()
	{
		return (m_added.isEmpty() && m_deleted.isEmpty() && m_changed.isEmpty());
	}

 public void removeAllDeleted(List<DMAttribute> found)
 {
  m_deleted.removeAll(found);
 }
 
 public void removeAllAdded(List<DMAttribute> found)
 {
  m_added.removeAll(found);
 }
 
 public void removeAllUpdated(List<DMAttribute> found)
 {
  m_changed.removeAll(found);
 }
}
