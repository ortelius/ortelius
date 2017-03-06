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

public enum CredentialKind {
	UNCONFIGURED(0),	// Only used to indicate no credentials - values is USE_DIALOG in engine
	ENCRYPTED(1),		// Encrypted value in memory
	IN_DATABASE(2),		// Encrypted value in database
	RTI3_DFO_IN_FILESYSTEM(3),
	FROM_VARS(4),		// Values taken from variables
	PPK(5),				// Public Private Key for SSH
	HARVEST_DFO_IN_FILESYSTEM(10);
	
	private int m_value;
	
	private CredentialKind(int value) {
		m_value = value;
	}
	
	public int value()  { return m_value; }
	
	public static CredentialKind fromInt(int value) {
		for(CredentialKind ck : CredentialKind.values()) {
			if(ck.value() == value) {
				return ck;
			}
		}
		return null;
	}
}