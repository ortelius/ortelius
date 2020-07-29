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

import java.util.List;

import dmadmin.ACDChangeSet;
import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONArray;
import dmadmin.json.JSONBoolean;
import dmadmin.json.LinkField;

public class Repository extends ProviderObject
{
	private static final long serialVersionUID = -1569186922421732870L;
 
	public Repository(DMSession sess, int id, String name)
	{
		super(sess, id, name);
	}
	
	public Repository(DMSession sess, int id, String name, int domainid)
	{
		super(sess, id, name);
		this.setDomainId(domainid);
	}
	
 public String toString()
 {
  Domain d = m_session.getDomain(this.getDomainId());
  String name = m_name;
  if (d!=null) 
  {
   String fd = d.getFullDomain();
   if (fd.length()>0) name = fd+"."+m_name;
  }
  return name;
 }
 
	@Override
	public ObjectType getObjectType() {
		return ObjectType.REPOSITORY;
	}
	
	@Override
	public String getDatabaseTable() {
		return "dm_repository";
	}

	@Override
	public String getForeignKey() {
		return "repositoryid";
	}

	@Override
	public boolean hasReadWrite() {
		return true;
	}

	@Override
	public String getReadTitle() {
		return "Check-out Access";
	}

	@Override
	public String getWriteTitle() {
		return "Check-in Access";
	}
	
	public class TextPattern
		implements IJSONSerializable
	{
		private String m_path;
		private String m_pattern;
		private boolean m_isText;
		private String m_key;
		
		public TextPattern(String path, String pattern, boolean isText, String key) {
			m_path = path;
			m_pattern = pattern;
			m_isText = isText;
			m_key = key;
		}
		
		public String getKey()
		{
			return m_key;
		}
		
		public TextPattern(String path, String pattern, boolean isText)
		{
			this(path, pattern, isText, "");
		}
		public String getPath()  { return m_path; }
		public String getPattern()  { return m_pattern; }
		public boolean isText()  { return m_isText; }

		private JSONArray toJSONArray() {
			return new JSONArray()
				.add(new JSONBoolean(false))	// dirty
				.add(new JSONBoolean(false)) 	// added
				.add(new JSONBoolean(false))	// deleted
				.add(m_path)
				.add(m_pattern)
				.add(m_isText);		
		}

		@Override
		public String getJSON() {
			return toJSONArray().toString();
		}
	}
	
	public List<TextPattern> getTextPatterns() {
		return m_session.getRepositoryTextPatterns(this);
	}
	
	public boolean updateTextPatterns(ACDChangeSet<TextPattern> changes) {
		return m_session.updateRepositoryTextPatterns(this, changes);
	}
	
	public IJSONSerializable getLinkJSON() {
		if (getDomainId()==0 && getId()>0) {
		 // Repository not fully configured
		 Repository repo = m_session.getRepository(getId(),true);
		 this.setDomainId(repo.getDomainId());
	}
	Domain d = m_session.getDomain(getDomainId());
	String name = m_name;
	if (d!=null) {
		String fd = d.getFullDomain();
		if (fd.length()>0) name = fd+"."+m_name;
	}
	System.out.println("getLinkJSON for repository, domain="+getDomainId());
	boolean showLink=m_session.ValidDomain(getDomainId());
	System.out.println("showlink="+showLink);
	return new LinkField(getObjectType(), m_id, name , showLink);
 }
}
