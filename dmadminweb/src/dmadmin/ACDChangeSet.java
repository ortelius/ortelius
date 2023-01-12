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
