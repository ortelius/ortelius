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

/**
 * Magic class to allow a all-string array or object to explicitly output boolean values
 * @author Robert
 */
public class JSONBoolean
	implements IJSONSerializable
{
	private boolean m_value;

	public JSONBoolean(boolean value)
	{
		m_value = value;
	}

	@Override
	public String getJSON()
	{
		return m_value ? "true" : "false";
	}

	@Override
	public String toString()
	{
		return getJSON();
	}
}
