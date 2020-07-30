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
