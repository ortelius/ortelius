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
package dmadmin.model;

public enum ComponentFilter {
	OFF (0),
	ON  (1),
	ALL (2);
	
	private int m_value;
	
	private ComponentFilter(int value) {
		m_value = value;
	}
	
	public int value()  { return m_value; }
	
	public static ComponentFilter fromInt(int value) {
		switch(value) {
		case 1: return ON;
		case 2: return ALL;
		default: return OFF;
		}
	}
}
