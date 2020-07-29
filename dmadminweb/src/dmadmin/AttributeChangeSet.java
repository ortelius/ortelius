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
