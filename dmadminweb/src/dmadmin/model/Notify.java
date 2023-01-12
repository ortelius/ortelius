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

public class Notify
	extends ProviderObject
{
	private static final long serialVersionUID = -1569186922421732870L;

	private String m_testrecipient;

	public void setTestRecipient(String rcpt) {m_testrecipient = rcpt;}
	public String getTestRecipient() { return m_testrecipient; }

	public Notify(DMSession sess, int id, String name)
	{
		super(sess, id, name);
	}

	@Override
	public ObjectType getObjectType() {
		return ObjectType.NOTIFY;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_notify";
	}

	@Override
	public String getForeignKey() {
		return "notifyid";
	}

	@Override
	public boolean hasReadWrite() {
		return true;
	}

	@Override
	public String getReadTitle() {
		return "Send Access";
	}

	@Override
	public String getWriteTitle() {
		return null;
	}
}
