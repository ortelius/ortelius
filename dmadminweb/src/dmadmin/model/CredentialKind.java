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