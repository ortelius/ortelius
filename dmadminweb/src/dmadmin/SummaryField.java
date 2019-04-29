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

public enum SummaryField
{
	READ_ONLY		(0, null),
	
	// DMObject
	NAME			(1, "Name", 	null),						// This value is used in GetSummary.jsp for callback
	SUMMARY			(2, "Summary", 	null),						// This value is used in GetSummary.jsp for callback
	OWNER			(3, "Owner",	ObjectType.OWNER),
	ATTACHMENTS		(4, ObjectType.ATTACHMENT_LIST),
	// Engine
	ENGINE_HOSTNAME (5, null),
 ENGINE_CLIENTID (6, null),
	
	// Environment
	AVAILABILITY	(10,  "Availability",		null),
 
	// Application
	PRE_ACTION		(101, "Pre Action",					ObjectType.ACTION),
	POST_ACTION		(102, "Post Action",				ObjectType.ACTION),
	CUSTOM_ACTION	(103, "Custom Action",				ObjectType.ACTION),
	APP_DATASOURCE	(104, "Change Request Datasource",	ObjectType.DATASOURCE),

	// Server
	SERVER_TYPE		(201, "Type",				ObjectType.SERVER_TYPE),
	SERVER_HOSTNAME	(202, "Hostname",			null),
	SERVER_PROTOCOL	(203, "Protocol",			ObjectType.TRANSFER),
	SERVER_BASEDIR	(204, "Base Directory",		null),
	SERVER_CRED		(205, "Credentials",		ObjectType.CREDENTIALS),
	SERVER_AUTOPING	(206, "Auto Ping",			ObjectType.BOOLEAN),
	SERVER_AUTOMD5	(207, "Auto MD5",			ObjectType.BOOLEAN),
	SERVER_COMPTYPE (208, "Component Types",	ObjectType.SERVERCOMPTYPE),
	SERVER_PINGINTERVAL (209, "Ping Interval",	null),
	SERVER_PINGSTART    (210, "Ping Start",		null),
	SERVER_PINGEND      (211, "Ping End",		null),
	SERVER_PINGTEMPLATE (212, "Ping Template",	ObjectType.TEMPLATE),
	SERVER_MD5TEMPLATE  (213, "MD5 Template",	ObjectType.TEMPLATE),
	SERVER_SSHPORT		(214, "SSH Port",		null),
	
	// Providers
	PROVIDER_TYPE	(301, ObjectType.PROVIDERDEF),	// This is used in GetSummary.jsp for callback
	PROVIDER_CRED	(302, ObjectType.CREDENTIALS),
	
	// User
	USER_REALNAME	(401, "Real Name",		null),					// This is used in GetSummary.jsp for callback
	USER_EMAIL		(402, "email Address",	null),
	USER_PHONE		(403, "Telephone Num",	null),
	USER_PASSWORD   (404, "Password",		ObjectType.PASS_HASH),
	USER_LOCKED     (405, null),
	USER_CHNG_PASS  (406, null),
	USER_DATE_FMT   (407, "Date Format",	null),
	USER_TIME_FMT   (408, "Time Format",	null),
	USER_DATASOURCE (409, "Data Source",	ObjectType.DATASOURCE),
	
	// UserGroup
	GROUP_EMAIL		(501, "email address",	null),
	
	// Components/Component Items
	FILTER_ITEMS    	(701, "Filter Items",			ObjectType.FILTER_ITEMS),
	ROLLUP 				(702, "Rollup flag",			ObjectType.COMPONENT_FILTER),
	ROLLBACK 			(703, "Rollback flag",			ObjectType.COMPONENT_FILTER),
	ITEM_REPOSITORY 	(704, "Repository",				ObjectType.REPOSITORY),
	ITEM_TARGETDIR  	(705, "Target Directory",		null),
	DEPLOY_ALWAYS		(706, "Always Deploy",			ObjectType.BOOLEAN),
	BASE_DIRECTORY		(707, "Base Directory",			null),
	COMPTYPE			(708, "Component Type",			null),
	DEPLOY_SEQUENTIALLY (709, "Deploy Sequentially",	ObjectType.BOOLEAN),
	COMP_BUILDJOB		(711, "Build Job",				ObjectType.BUILDJOB),	
	COMP_LASTBUILDNUMBER(712, "Last Build Number",		null),	
 
	// Credentials
	CRED_KIND		(801, "Kind",		ObjectType.CREDENTIAL_KIND),// This is used in GetSummary.jsp for callback
	CRED_ENCUSERNAME(802, "Username",	ObjectType.ENCRYPTED),		// This is used in CredDetails.jsp for show/hide
	CRED_ENCPASSWORD(803, "Password",	ObjectType.ENCRYPTED),		// This is used in CredDetails.jsp for show/hide
	CRED_VARUSERNAME(804, "Username",	null),						// This is used in CredDetails.jsp for show/hide
	CRED_VARPASSWORD(805, "Password",	null),						// This is used in CredDetails.jsp for show/hide
	CRED_FILENAME   (806, "Filename",	null),						// This is used in CredDetails.jsp for show/hide
	CRED_USERNAME	(807, "Username",	ObjectType.ENCRYPTED),		// For displaying unencrypted username
	
	// Task
	TASK_SHOWOUTPUT     	 		(901, ObjectType.BOOLEAN),
	TASK_APPROVAL_DOMAIN			(902, null),
	TASK_MOVE_TO_DOMAIN  			(903, null),
	TASK_COPY_TO_DOMAIN  			(904, null),
	TASK_LINKED_TASK     			(905, null),
	TASK_CREATEVERSION_DOMAIN 		(906, null),
	TASK_ACTION						(907, ObjectType.ACTION),
	TASK_AVAILABLE_TO_SUBDOMAINS	(908, ObjectType.BOOLEAN),
 
	// Domain
	DOMAIN_LIFECYCLE(1001,ObjectType.BOOLEAN),
	
	// Action
	ACTION_FILEPATH 	(1101,	"Filepath",			null),						// This is used in actiondetails.jsp for show/hide
	ACTION_RESISEXPR	(1102,	"Result is Expr",	ObjectType.BOOLEAN),
	ACTION_COPYTOREM	(1103,	"Copy to Remote",	ObjectType.BOOLEAN),			// This is used in actiondetails.jsp for show/hide
	ACTION_KIND			(1104,	"Kind",				ObjectType.ACTION_KIND),		// This is used in GetSummary.jsp for callback
	ACTION_REPO     	(1105,	"Repository",		ObjectType.ACTION_REPO),		// This is used in actiondetails.jsp for show/hide
	ACTION_CATEGORY 	(1106,	"Category",			ObjectType.ACTION_CATEGORY),	// This is used in GetSummary.jsp for callback
	ACTION_FRAGNAME 	(1107,	"Display Name",		null),						// This is used in actiondetails.jsp for show/hide
	ACTION_INTERPRETER	(1108,	"Interpreter",		null),
	ACTION_USETTY		(1109,	"Use TTY",			ObjectType.BOOLEAN),
 
	// Notifications
	SUCCESS_TEMPLATE (1201,		"Success Template",	ObjectType.TEMPLATE),
	FAILURE_TEMPLATE (1202,		"Failure Template",	ObjectType.TEMPLATE),
	
	// Server Summary Data
	NAME_RESOLUTION (1301,null),
	PING_SUCCESS (1302,null),
	CONNECT_SUCCESS (1303,null),
	BASEDIR_SUCCESS (1304,null),
	PING_TIME (1305,null),
	IP_ADDRESS (1306,null),
	LAST_PING_TIME(1307,null),
	LAST_ERROR(1308,null),
	
	// Server Component Type
	DATABASE  (1401,ObjectType.BOOLEAN),
 	DELETEDIR (1402,ObjectType.BOOLEAN),
 
 	// Build Engines
 	PROJECTNAME (1501,		"Project Name",		null);
 
	private final int m_value;
	private final ObjectType m_type;
	private final String m_fieldname;

	public static final int OBJECT_MAX = 99;	
	
	SummaryField(int value, ObjectType type) {
		m_value = value;
		m_type = type;
		m_fieldname = null;
	}
	
	SummaryField(int value, String fieldname, ObjectType type) {
		m_value = value;
		m_type = type;
		m_fieldname = fieldname;
	}
	
	public int value()  { return m_value; }

	public ObjectType type()  { return m_type; }
	
	public String fieldname() { return m_fieldname; }
	
	public static SummaryField fromInt(int value) {
		for(SummaryField ot : SummaryField.values()) {
			if(ot.value() == value) {
				return ot;
			}
		}
		return null;
	}
}
