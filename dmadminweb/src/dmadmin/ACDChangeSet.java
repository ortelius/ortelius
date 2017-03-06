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
import java.util.Iterator;
import java.util.List;

/**
 * Generic type for Add/Change/Delete Change Sets.
 * Test CI
 */
public class ACDChangeSet<T>
{
	private List<T> m_added = new ArrayList<T>();
	private List<T> m_deleted = new ArrayList<T>();
	private List<T> m_changed = new ArrayList<T>();
	
	public class IteratorDelegator
		implements Iterable<T>
	{
		private Iterator<T> m_iter;
		public IteratorDelegator(List<T> list) {
			m_iter = list.iterator();
		}

		@Override
		public Iterator<T> iterator() {
			return m_iter;
		}
	}
	
	public ACDChangeSet()
	{}
	
	public void addAdded(T add)
	{
		m_added.add(add);
	}
	
	public void addDeleted(T del)
	{
		m_deleted.add(del);
	}
	
	public void addChanged(T chg)
	{
		m_changed.add(chg);
	}

	public IteratorDelegator added() {
		return new IteratorDelegator(m_added);
	}

	public IteratorDelegator deleted() {
		return new IteratorDelegator(m_deleted);
	}

	public IteratorDelegator changed() {
		return new IteratorDelegator(m_changed);
	}
	
	public boolean isEmpty()
	{
		return (m_added.isEmpty() && m_deleted.isEmpty() && m_changed.isEmpty());
	}
}
