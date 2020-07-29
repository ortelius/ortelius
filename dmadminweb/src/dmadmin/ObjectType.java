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
package dmadmin;

import java.util.Hashtable;

public enum ObjectType
{
	/*
	 * Note that the following are also used:
	 * "bb" = (b)oolean field
	 * "oo" = read (o)nly field
	 * "tt" = date (t)ime field
	 */

	ENVIRONMENT     (  2, "en", "environment"),		// (en)vironment
	DOMAIN          (  3, "do", "domain"),			// (do)main
	SERVER          (  4, "se", "server"),			// (se)rver
	APPLICATION     (  5, "ap", "application"),		// (ap)plication
	USER            (  6, "us", "user"),			// (us)er
	USERGROUP       (  7, "gr", "usergroup"),		// user(gr)oup
	REPOSITORY      (  8, "re", "repository"),		// (re)pository
	NOTIFY          (  9, "no", "notify"),			// (no)tify
	DATASOURCE      ( 10, "ds", "datasource"),		// (d)ata(s)ource
	ACTION          ( 11, "ac", "action"),			// (ac)tion
	DROPZONE        ( 12, null, null),
	COMPONENT       ( 13, "co", "component"),		// (co)mponent
	COMPONENTITEM   ( 14, "ci", "componentitem"),	// (c)omponent (i)tem
	DROPZONEFILE    ( 15, null, null),
	TRANSFER        ( 16, "xf", null),				// (xf)er
	APPVERSION      ( 17, "av", "application"),				// (a)pplication (v)ersion
	TASK            ( 18, "ta", "task"),			// (ta)sk
	CREDENTIALS		( 19, "cr", "credentials"),		// (cr)edentials
	TEMPLATE		( 20, "te", "template"),		// Notification (te)mplate
    RELEASE         ( 21, "rl", "release"),         // (ap)plication
    RELVERSION      ( 22, "rv", "release"),              // (ap)plication
    COMPVERSION     ( 23, "cv", "component"),              // (c)omponent (v)ersion
    BUILDID			( 26, "bu", "buildnumber"),		// (bu)ild number
    RPROXY   ( 27, "rp", "rproxy"),  // (rp)roxy
 
	// Never stored in DB and not used by engine
	ATTACHMENT_LIST	( 50, "al", null),		// Only used in web interface - (a)ttachment (l)ist
	PASS_HASH		( 51, "pw", null),		// Only used by web interface - (p)ass(w)ord hash
	SERVER_TYPE		( 52, "st", null),		// Only used by web interface - (s)erver (t)type
	MODIFIED		( 53, "cm", null),		// Only used by web interface - (c)reated/(m)odified - used to indicate user and time
	OWNER			( 54, "ow", null),		// Only used by web interface - (ow)ner - used to indicate either user or group	
	ENGINE          ( 55, "ei", "engine"),		// Only used by web interface - (e)ng(i)ne
	PLUGIN          ( 56, "pi", null),		// Only used by web interface - (p)lug(i)n
	PROVIDERDEF     ( 57, "pd", null),		// Only used by web interface - (p)rovider (d)ef
	CALENDAR_ENTRY	( 58, "ce", "calendar"),	// Only used by web interface - (c)alendar (e)ntry
	COMPONENT_FILTER( 59, "rr", null),		// Only used by web interface - (r)ollup (r)ollback
	FILTER_ITEMS    ( 60, "fi", null),		// Only used by web interface - (f)ilter (i)tems
	CREDENTIAL_KIND ( 61, "ck", null),		// Only used by web interface - (c)redential (k)ind
	ENCRYPTED		( 62, "nc", null),		// Only used by web interface - e(nc)rypted
	BOOLEAN         ( 63, "bb", null),		// Only used by web interface - boolean
	ACTION_KIND     ( 64, "ak", null),		// Only used by web interface - (a)ction (k)ind
	ACTION_REPO     ( 65, "ar", null),		// Only used by web interface - (a)ction (r)pository
	PROCFUNC_CATEGORY ( 66, "cp", "pfcategory"),	// Only used by web interface - (c)tegor(y)
 SERVERCOMPTYPE  ( 67, "ct", "type"),  // Only used by web interface - server (c)omp (t)type
 ACTION_CATEGORY ( 68, "cy", "category"), // Only used by web interface - (c)tegor(y)
 FRAG_CATEGORY ( 69, "cf", "fragcategory"), // Only used by web interface - (c)tegor(y)
 COMP_CATEGORY ( 70, "cc", "compcategory"), // Only used by web interface - (c)tegor(y)
 CATEGORY (71, "ca", "category"),  // Only used for export/import
 XPOS (72,"xp",null), // xpos
 YPOS (73, "yp", null), // ypos
 PREDECESSOR (74,"pc", null), // predecessor
 PARENT (75,"pt", null), // parent
 COMP_KIND ( 76, "ik", null),  // Only used by web interface - (c)redential (k)ind
 
	// Can be stored in DB but not used by engine
	PROCEDURE		( 90, "pr", null),		// Only used in web interface - (pr)ocedure - only stored in create permissions table - DO NOT CHANGE
	FUNCTION		( 91, "fn", null),		// Only used in web interface - (f)unctio(n) - only stored in create permissions table - DO NOT CHANGE
	FRAGMENT		( 92, "fg", null),
	FRAGMENTPROC    ( 93, "fg", null),
	FRAGMENTFUNC    ( 94, "fg", null),
	APPROVAL	    ( 96, "ae", null),		// Only used by web interface - (a)pprov(e)- stored in db - DO NOT CHANGE
	REQUEST			( 97, "rq", null),		// Only used by web interface - (r)e(q)uest - stored in db - DO NOT CHANGE
	HISTORY_NOTE    ( 98, "hn", null),		// Only used by web interface - (h)istory (n)ote - stored in db - DO NOT CHANGE
	DEPLOYMENT      ( 99, "de", null),		// Only used by web interface - (de)ployment - stored in db - DO NOT CHANGE
 DEFECT          ( 102, "df", null),
	
	RESPOSITORY_IMPL(108, null, null),
	NOTIFY_IMPL     (109, null, null),
	DATASOURCE_IMPL (110, null, null),
	STATEMENT_IMPL  (111, null, null),

	PLUGIN_OBJECT	(200, null, null);

	private final int m_value;
	private final String m_kind;
	private final String m_table;
	private static Hashtable<String, ObjectType> s_types;
	private static Hashtable<String, ObjectType> s_tables;
	
	ObjectType(int value, String kind, String table) {
		m_value = value;
		m_kind = kind;
		m_table = table;
	}
	
	public int value()  { return m_value; }
	
	/**
	 * Decode an ObjectType from its integer equivalent, as used in the database
	 */
	public static ObjectType fromInt(int value) {
		for(ObjectType ot : ObjectType.values()) {
			if(ot.value() == value) {
				return ot;
			}
		}
		return null;
	}
	
	/**
	 * Decode an ObjectType from its string equivalent, as used by OTIDs and
	 * the web interface.  The static Hashtable used for lookups is created on
	 * first use. 
	 */
	public static ObjectType fromTypeString(String kind) {
		if(s_types == null) {
			s_types = new Hashtable<String, ObjectType>();
			for(ObjectType ot : ObjectType.values()) {
				String str = ot.getTypeString();
				if(str != null) {
					//System.out.println("Adding '"+str+"' -> '"+ot+"'");
					s_types.put(str, ot);
				}
			}			
		}
		return s_types.get(kind);
	}
	
	/**
	 * Decode an ObjectType from its table name, as used by GetNewID.  The
	 * static Hashtable used for lookups is created on first use. 
	 */
	public static ObjectType fromTableName(String table) {
		if(s_tables == null) {
			s_tables = new Hashtable<String, ObjectType>();
			for(ObjectType ot : ObjectType.values()) {
				String str = ot.getTableName();
				if(str != null) {
					//System.out.println("Adding '"+str+"' -> '"+ot+"'");
					s_tables.put(str, ot);
				}
			}			
		}
		return s_tables.get(table.toLowerCase());
	}

	public String getTypeString()  { return m_kind; }
	public String getTableName()  { return m_table; }
}
