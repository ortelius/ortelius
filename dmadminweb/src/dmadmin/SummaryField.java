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
	APP_DATASOURCE	(104, "Change Request Data Source",	ObjectType.DATASOURCE),

	// Server
	SERVER_TYPE		(201, "Type",				ObjectType.SERVER_TYPE),
	SERVER_HOSTNAME	(202, "Hostname",			null),
	SERVER_PROTOCOL	(203, "Protocol",			ObjectType.TRANSFER),
	SERVER_BASEDIR	(204, "Base Directory",		null),
	SERVER_CRED		(205, "Credentials",		ObjectType.CREDENTIALS),
	SERVER_AUTOPING	(206, "Auto Ping",			ObjectType.BOOLEAN),
	SERVER_AUTOMD5	(207, "Auto MD5",			ObjectType.BOOLEAN),
	SERVER_COMPTYPE (208, "Endpoint Types",	ObjectType.SERVERCOMPTYPE),
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
	COMPTYPE			(708, "Endpoint Type",			null),
	DEPLOY_SEQUENTIALLY (709, "Deploy Sequentially",	ObjectType.BOOLEAN),
	COMP_DATASOURCE		(710, "Change Request Data Source",ObjectType.DATASOURCE),
	COMP_BUILDJOB  (711, "Build Job", null),
	COMP_LASTBUILDNUMBER(712, "Last Build Number",		null),
	DOCKER    (713, "Container Kind",  null),
	XPOS (714,"xpos", null),
	YPOS (715,"ypos", null),
	PREDECESSOR (716,"predecessor", null),
	PARENT (717,"parent", null),
	COMP_KIND   (718, "Kind",   ObjectType.COMP_KIND),

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
	DOMAIN_FULLNAME(1002,"Domain", null),

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
	NOTIFIER (1203, "Notifier", ObjectType.NOTIFY),

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
 PROJECTNAME (1501,		"Project Name",		null),

 // Container Component Item Details
 DOCKER_BUILDID (1601, "Build Id",  null),
 DOCKER_BUILDURL  (1602, "Build URL", null),
 DOCKER_CHART  (1603, "Helm Chart", null),
 DOCKER_OPERATOR  (1604, "Operator",  null),
 DOCKER_BUILDDATE   (1605,"Build Date", null),
 DOCKER_REPO  (1606, "Container Registry", null),
 DOCKER_SHA  (1607, "Container Digest", null),
 DOCKER_GITCOMMIT   (1608, "Git Commit", null),
 DOCKER_GITREPO   (1609, "Git Repo", null),
 DOCKER_GITTAG (1610, "Git Tag", null),
 DOCKER_GITURL (1611, "Git URL", null),
 DOCKER_CHARTVERSION  (1612, "Helm Chart Version", null),
 DOCKER_CHARTNAMESPACE  (1613, "Helm Chart Namespace", null),
 DOCKER_TAG  (1614, "Container Tag", null),
 DOCKER_CHARTREPO  (1615, "Helm Chart Repo", null),
 DOCKER_CHARTREPOURL  (1616, "Helm Chart Repo Url", null),
 COMP_README (1617, "Readme", null),
 SERVICE_OWNER (3, "Service Owner", ObjectType.OWNER),
 SERVICE_OWNER_EMAIL (1619, "Service Owner Email", null),
 SERVICE_OWNER_PHONE (1620, "Service Owner Phone", null),
 SLACK_CHANNEL (1621, "Slack Channel", null),
 DISCORD_CHANNEL (1622, "Discord Channel", null),
 HIPCHAT_CHANNEL (1623, "HipChat Channel", null),
 PAGERDUTY_SERVICE_URL (1624, "PagerDuty Service Url", null),
 PAGERDUTY_BUSINESS_URL (1625, "PagerDuty Business Service Url", null),
 COMP_SWAGGER (1626, "Swagger", null),
 SERVICE_OWNER_ID (1627, "Service Owner Id", null),
 PURL(1650, "Purl", null),
 SCORECARD_SCORECARD_PINNED (1700, "OpenSSF ScoreCard Pinned to Commit", null),
 SCORECARD_SCORE(1701, "OpenSSF ScoreCard Score", null),
 SCORECARD_MAINTAINED(1702, "Maintained", null),
 SCORECARD_CODEREVIEW(1703, "Code Review", null),
 SCORECARD_CIIBESTPRACTICES(1704, "CII Best Practices", null),
 SCORECARD_LICENSE(1705, "License", null),
 SCORECARD_SIGNEDRELEASE(1706, "Signed Releases", null),
 SCORECARD_DANGEROUSWORKFLOW(1707, "Dangerous Workflow", null),
 SCORECARD_PACKAGING(1708, "Packaging", null),
 SCORECARD_TOKENPERMISSIONS(1709,"Token Permissions", null),
 SCORECARD_BRANCHPROTECTION(1710,"Branch Protection", null),
 SCORECARD_BINARYARTIFACTS(1711,"Binary Artifacts", null),
 SCORECARD_PINNEDEPENDENCIES(1712,"Pinned Dependencies", null),
 SCORECARD_SECURITYPOLICY(1713, "Security Policy", null),
 SCORECARD_FUZZING(1714,"Fuzzing", null),
 SCORECARD_SAST(1715, "SAST", null),
 SCORECARD_VULNERABILITES(1716, "Vulnerabilities", null),
 SCORECARD_CITESTS(1717, "CITests", null),
 SCORECARD_CONTRIBUTORS(1718, "Contributors", null),
 SCORECARD_DEPENDENCYUPDATETOOL(1719, "DependencyUpdateTool", null),
 SCORECARD_SBOM(1720, "SBOM", null),
 SCORECARD_WEBHOOOKS(1721, "Webhooks", null);

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
