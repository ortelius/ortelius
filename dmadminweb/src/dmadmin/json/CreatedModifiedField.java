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

package dmadmin.json;

import dmadmin.ObjectType;
import dmadmin.model.User;

public class CreatedModifiedField
	extends LinkField
{
	private String m_when;

	public CreatedModifiedField(String when, User user) {
		super(ObjectType.MODIFIED, user.getId(), user.getName(), user.getRealName());
		m_when = when;
	}

	@Override
	public String getJSON() {
		JSONObject obj = super.getJSONObject();
		obj.add("when", m_when);
		obj.add("readonly", true);
		return obj.getJSON();
	}
}
