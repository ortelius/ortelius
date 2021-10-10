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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * A collection of tabs with the facility to automatically number the tabs as
 * they are added to the collection.  We do not subclass ArrayList as this
 * caused JSTL to treat the class as an EL List and thus firstTab as an
 * integer.  Instead we make the list an internal member so we are treated as
 * an object.  In order to use the forEach loop in JSTL you therefore need to
 * access the list property.
 * @author Robert
 *
 */
public class PanelTabsList
	implements Serializable
{
	private static final long serialVersionUID = 3847864918375923021L;
	
	private List<PanelTabs> m_list = new ArrayList<PanelTabs>();

	/**
	 * Adds a tab to the collection and automatically set its tab number to the
	 * position in the collection.
	 */
	public boolean add(PanelTabs tab) {
		tab.setTabNumber(m_list.size());
		return m_list.add(tab);
	}
	
	/**
	 * Convenience property to retrieve the first tab in the collection.
	 * @return The first tab in the collection.
	 */
	public PanelTabs getFirstTab() {
		return m_list.get(0);
	}
	
	/**
	 * Property to allow access to the list for loops.
	 * @return The underlying list.
	 */
	public List<PanelTabs> getList() {
		return m_list;
	}
}
