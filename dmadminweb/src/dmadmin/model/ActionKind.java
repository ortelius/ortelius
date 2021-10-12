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

public enum ActionKind {
	UNCONFIGURED(0),
	SCRIPT(1),
	IN_DB(2),
	LOCAL_EXTERNAL(3),
	REMOTE_EXTERNAL(4),
	PLUGIN(5),
	GRAPHICAL(6);
	
	private int m_value;
	
	private ActionKind(int value)  { m_value = value; }
	
	public int value()  { return m_value; }
	
	public static ActionKind fromInt(int value) {
		for(ActionKind k : values()) {
			if(k.value() == value) {
				return k;
			}
		}
		return UNCONFIGURED;
	}
}