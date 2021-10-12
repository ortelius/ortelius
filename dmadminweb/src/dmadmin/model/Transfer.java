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

import dmadmin.DMSession;
import dmadmin.ObjectType;

public class Transfer
	extends ProviderObject
{
	private static final long serialVersionUID = -6642409618536934477L;

	public Transfer(DMSession sess, int id, String name)
	{
		super(sess, id, name);
	}
	
	@Override
	public ObjectType getObjectType() {
		return ObjectType.TRANSFER;
	}
	
	@Override
	public String getDatabaseTable() {
		return null;
	}

	@Override
	public String getForeignKey() {
		return null;
	}
}
