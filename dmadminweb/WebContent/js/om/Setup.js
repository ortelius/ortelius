/*
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
var wt = 1;
var mp = 9;

function SetupWizard()
{
 SetupRepoCredId = "";
 SetupRepoId = "";
 SetupServerCredId = "";
 SetupServerId = "";
 SetupEnvId = "";
 SetupAppId = "";
 SetupCompId = "";
 SetupBuildEngineId = "";
 SetupBuildJobId = "";
 SetupRepoCredName = "";
 SetupRepoName = "";
 SetupServerCredName = "";
 SetupServerName = "";
 SetupEnvName = "";
 SetupAppName = "";
 SetupCompName = "";
 SetupBuildEngineName = "";
 SetupBuildJobName = "";
 var td = ""; 

 tdedit = "<form id=\"summform\">";
 tdedit += "<div id=\"setuppage1\">";
 tdedit += "<B>Pick a Wizard Type</B>";
 tdedit += "<table style=\"width:100%\"><tr>";
 tdedit += "<td style=\"width:50%;text-align:center;vertical-align:top;border-right: 1px solid #cdd0d4;\">";
 tdedit += "<img src=\"css/images/setupdog.png\">";
 tdedit += "</td>";
 tdedit += "<td style=\"width:50%;text-align:center;vertical-align:top\">";
 tdedit += "<img src=\"css/images/setupjen.png\"><br>";
 tdedit += "</td></tr><tr>";
 tdedit += "<td style=\"text-align:center;border-right: 1px solid #cdd0d4;\"><input type=\"radio\" onclick=\"javascript:wtc();\" id=\"wt1\" name=\"wt\" checked value=\"general\"></td>";
 tdedit += "<td style=\"text-align:center;\"><input type=\"radio\" onclick=\"javascript:wtc();\" id=\"wt2\" name=\"wt\" value=\"jenkins\"></td>";
 tdedit += "</tr><tr><td style=\"text-align:center;border-right: 1px solid #cdd0d4;\"><h2>General DeployHub Setup</h2></td>";
 tdedit += "<td style=\"text-align:center\"><h2>Quick Jenkins Setup</h2></td></tr><tr>";
 tdedit += "<td style=\"text-align:left;vertical-align:top;border-right: 1px solid #cdd0d4;\">";
 tdedit += "Use this Wizard to set up a general DeployHub configuration. This will create all the ";
 tdedit += "objects necessary to perform a simple deployment. You can specify your own names for all ";
 tdedit += "the created objects. You can edit the resulting objects after creation ";
 tdedit += "to customize the deployment for your application and environment configuration.";
 tdedit += "</td><td style=\"text-align:left;vertical-align:top;\">";
 tdedit += "Use this Wizard to set up a simple Jenkins CI/CD Server. This wizard assumes that the Jenkins ";
 tdedit += "build artifacts are archived so all old builds are accessible via the Jenkins WebUI. ";
 tdedit += "This Wizard requires minimal information and will generate its own names for the created ";
 tdedit += "DeployHub objects. You can rename those objects if required after the Wizard has run.<br><br>";
 tdedit += "Note: If the Jenkins build artifacts are published to a separate binary repository you should use the ";
 tdedit += "<I>General DeployHub Setup</I> Wizard.";
 tdedit += "</td></tr></table>";
 tdedit += "</div>";
 tdedit += "<div id =\"setuppage12\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 1 of 8 - Introduction</B><BR>";
 tdedit += "Welcome to the DeployHub Quick Setup Wizard. Over the next few screens you can create all ";
 tdedit += "the initial objects necessary to get DeployHub up and running for a simple first deployment. ";
 tdedit += "Once the wizard is complete, you can chose to edit the resulting objects to add more details.<BR><BR>";
 tdedit += "This Wizard will create:<BR>";
 tdedit += "<UL><LI>An initial <I>Repository</I>. A repository is a source of deployment artifacts. Repositories ";
 tdedit += "can be found under the <I>Connections</I> view.</LI>";
 tdedit += "<LI>An initial <I>Environment</I>. An Environment represents a target for deployment and contains ";
 tdedit += "one or more <I>End Points</I>. Environments can be found under the <I>Data Center</I> view.</LI>";
 tdedit += "<LI>An initial <I>End Point</I>. An End Point receives the deployment artifacts pulled from a ";
 tdedit += "<I>Repository</I>. The artifact(s) an End Point receives will depend on the <I>Component Types</I> ";
 tdedit += "it is set to accept. End Points can be found under the <I>Data Center</I> view.</LI>";
 tdedit += "<LI>An initial <I>Application</I>. An Application represents an object to be deployed to an ";
 tdedit += "<I>Environment</I>. An Application consists of one or more <I>Components</I>. Applications can be ";
 tdedit += "found under the <I>Deploy</I> View.</LI>";
 tdedit += "<LI>An initial <I>Component</I>. A Component pulls one or more files from a <I>Repository</I>. A ";
 tdedit += "Component has a <I>Component Type</I> and this determines which <I>End Point</I> within the targeted ";
 tdedit += "<I>Environment</I> receives the file(s). Components can be found under the <I>Deploy</I> View.</LI>";
 tdedit += "<LI><I>Credentials</I> to allow access to the <I>Repository</I> and <I>End-Point</I>. Credentials ";
 tdedit += "can be found under the <I>Connections</I> View.</LI>";
 tdedit += "<LI>An optional <I>Build Engine</I> and <I>Build Job</I> to allow DeployHub to integrate with a Jenkins CI server. ";
 tdedit += "Build Engines can be found under the <I>Data Center</I> View.</LI>";
 tdedit += "</UL>";
 tdedit += "The Wizard will create the relationships between all the objects automatically to allow you to start ";
 tdedit += "deploying straightaway!<BR><BR>";
 tdedit += "Click \"Next\" and let's get started!";
 tdedit += "</div>";
 // Domain and Owner (both wizards)
 tdedit += "<div id=\"setuppage3\" style=\"display:none\">";
 tdedit += "<img id=\"p3img\" src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 2 of <span id=\"mp\">8</span> - Domain and Owner</B><BR>";
 tdedit += "First, let's decide in which <I>Domain</I> you want to create the initial objects. ";
 tdedit += "Please select the domain you want from the list below.";
 tdedit += "<table id=\"summtab1\" class=\"summtable\">";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Domain:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"domain_val\" name=\"domain_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"domain_field\" value=\"\"/></td>";
 tdedit += "</tr>";
 tdedit += "</table>";
 tdedit += "Every object in DeployHub has an <I>owner</I> which DeployHub uses to determine certain permissions ";
 tdedit += "and to allow notifications to be sent to the appropriate users. An owner can be either an individual ";
 tdedit += "user or a user group. Here, you can select which type the owner should be (user or usergroup) and then ";
 tdedit += "you can select the owner from the drop-down list.";
 tdedit += "<table>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Owner Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"ownertype_val\" name=\"ownertype_val\">";
 tdedit += "<option value=\"User\">User</option>";
 tdedit += "<option value=\"Group\" selected>Group</option>";
 tdedit += "</select></td>";
 tdedit += "<td><input type=\"hidden\" name=\"ownertype_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr id=\"owner_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Users:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"owner_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"owner_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr id=\"group_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Groups:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"groupowner_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"groupowner_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 tdedit += "</table></div>";
 // Jenkins Integration
 tdedit += "<div id=\"setuppage14\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 3 of 8 - Jenkins Integration</B><BR>";
 tdedit += "Do you want DeployHub to integrate with a Jenkins Build Job? Checking this option will ";
 tdedit += "create a <I>Build Engine</I> and a <I>Build Job</I> and will associate the Build Job ";
 tdedit += "with the <I>Component</I> you will create later. This will allow DeployHub to act as a ";
 tdedit += "<I>Continuous Delivery Server</I> and also allows DeployHub to track the builds and where ";
 tdedit += "they have been deployed.<br><br>";
 tdedit += "<input type=\"checkbox\" onclick=\"javascript:ujchecked();\" id=\"use_jenkins\">Create Jenkins Build Engine and Job";
 tdedit += "<div id=\"jenkins_attrs\" style=\"display:none\">";
 tdedit += "<table>";
 tdedit += "<tr><td>*Build Engine Name</td><td><input name=\"bename_val\" style='width:24em' type=\"text\" value=\"\"/></td><td><input type=\"hidden\" name=\"bename_field\" value=\"1\"></td></tr>";
 tdedit += "<tr><td>*Jenkins Build Server URL</td><td><input name=\"jenkinsurl_val\" style='width:24em' type=\"text\" value=\"\"/></td><td><input type=\"hidden\" name=\"betype_field\" value=\"301\"></td></tr>";
 tdedit += "<tr><td>*Jenkins Project Name</td><td><input name=\"jenkinsproj_val\" style='width:24em' type=\"text\" value=\"\"/></td><td><input type=\"hidden\" name=\"bjname_field\" value=\"1\"><input type=\"hidden\" name=\"pnname_field\" value=\"1501\"></td></tr>";
 tdedit += "</table>";
 tdedit += "</div>";
 tdedit += "</div>";

 // Repo
 tdedit += "<div id=\"setuppage15\" style=\"display:none\">"
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 4 of 8- Create a Repository</B><BR>";
 tdedit += "Now we're going to create a <I>Repository</I>. A Repository holds the files which will be ";
 tdedit += "deployed by DeployHub to the <I>End Point(s)</I> within the target <I>Environment</I>.";
 tdedit += "<table id=\"summtab2\" class=\"summtable\">";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Repository Name:</td>";
 tdedit += "<td><input name=\"reponame_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"reponame_field\" value=\"1\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr><td colspan=\"2\">";
 tdedit += "Select the Repository Type. If you select <I>filesystem</I> then the file(s) will be pulled ";
 tdedit += "from a directory that needs to be accessible from the DeployHub server.";
 tdedit += "</td></tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"setuptype_val\" name=\"setuptype_val\"></td>";
 tdedit += "<td><input type=\"hidden\" name=\"setuptype_field\" value=\"301\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the username and password needed to access the repository. If the repository type is ";
 tdedit += "<I>filesystem</I> you may leave these fields blank. If you enter a username and password a ";
 tdedit += "<I>credential</I> is created automatically and linked to the repository to allow it to be ";
 tdedit += "accessed.";
 tdedit += "</td></tr>";
 
 tdedit += "<tr id=\"repousernameenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">User Name:</td>";
 tdedit += "<td><input name=\"repousernameenc_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"repousernameenc_field\" value=\"802\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"repopasswordenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
 tdedit += "<td><input name=\"repopasswordenc_val\" style='width:24em' type=\"password\" /></td>";
 tdedit += "<td><input type=\"hidden\" name=\"repopasswordenc_field\"  value=\"803\"/></td>";
 tdedit += "</tr>";
 tdedit += "</table></div>";
 
 tdedit += "<div id=\"setuppage16\" style=\"display:none\">"
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 5 of 8 - Enter the Repository Attributes</B><BR>";
 tdedit += "Enter the attributes needed to access the <I>Repository</I> here. The fields displayed will depend ";
 tdedit += "on the <I>Repository Type</I> selected on the previous page. ";
 tdedit += "Check the following options for each attribute:<BR>";
 tdedit += "<table><tr><td><I>Encrypted</I></td><td>The value is held encrypted in the database. Use this for sensitive ";
 tdedit += "data.</td></tr>";
 tdedit += "<tr><td><I>Override</I></td><td>The value can be overridden by values specified when creating the <I>Component</I></td></tr>";
 tdedit += "<tr><td><I>Append</I></td><td>The value can be appended by values specified when creating the <I>Component</I></td></tr>";
 tdedit += "<table>";
 tdedit += "Attributes that are checked as being <I>Override</I> or <I>Append</I> can be ";
 tdedit += "specified when the Component is created later."
 tdedit += "<tr><td colspan=\"3\"><table id=\"propdata\" style=\"float:right\">";
 tdedit += "</table></tr>";
 tdedit += "</table></div>";
// Server
 tdedit += "<div id=\"setuppage17\" style=\"display:none\">"
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 6 of 8 - Environment and End Point (part 1)</B><BR>";
 tdedit += "<table id=\"summtab5\" class=\"summtable\">";
 tdedit += "Now we're going to create an Environment and a single End-Point within it.";
 tdedit += "<table>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Environment Name:</td>";
 tdedit += "<td><input name=\"envname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"envname_field\" value=\"1\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*End Point Name:</td>";
 tdedit += "<td><input name=\"servername_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"servername_field\" value=\"1\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the <I>Host Name</I> for this End-Point. This is the name of the host that DeployHub ";
 tdedit += "can use to connect to the End-Point (either via local hosts file or DNS). You can put an ";
 tdedit += "IP Address in here if the hostname cannot be resolved from DeployHub.";
 tdedit += "</td></tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Host Name:</td>";
 tdedit += "<td><input name=\"hostname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"hostname_field\" value=\"202\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the username and password needed to access the End-Point. The values entered here will be used to ";
 tdedit += "create a <I>credential</I> which will be linked to the End-Point to allow it to be accessed.";
 tdedit += "</td></tr>";
 tdedit += "<tr id=\"serverusernameenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">User Name:</td>";
 tdedit += "<td><input name=\"serverusernameenc_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"serverusernameenc_field\" value=\"802\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"serverpasswordenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
 tdedit += "<td><input name=\"serverpasswordenc_val\" style='width:24em' type=\"password\" /></td>";
 tdedit += "<td><input type=\"hidden\" name=\"serverpasswordenc_field\" value=\"803\"/></td>";
 tdedit += "</tr>";
 tdedit += "</table></div>";
 
 tdedit += "<div id=\"setuppage18\" style=\"display:none\">"
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 7 of 8 - End Point (part 2)</B><BR>";
 tdedit += "Enter the End-Point Operating System type and the Protocol to be used when executing commands ";
 tdedit += "or transferring file(s) to this End-Point";
 tdedit += "<table>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">End Point Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"servertype_val\">";
 tdedit += "<option value=\"st3\">as400</option>";
 tdedit += "<option value=\"st4\">mac</option>";
 tdedit += "<option value=\"st1\">unix</option>";
 tdedit += "<option value=\"st2\" selected>windows</option>";
 tdedit += "<option value=\"st5\">Tandem</option>";
 tdedit += "<option value=\"st6\">Stratus</option>";
 tdedit += "<option value=\"st7\">OpenVMS</option>";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"servertype_field\" value=\"201\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Protocol:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"protocol_val\">";
 tdedit += "<option value=\"ftp\">ftp</option>";
 tdedit += "<option value=\"ftps\">ftps</option>";
 tdedit += "<option value=\"sftp\">sftp</option>";
 tdedit += "<option value=\"win\" selected>win</option>";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"protocol_field\" value=\"203\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the <I>Base Directory</I> for this End-Point. This is the directory that file(s) will be ";
 tdedit += "deployed into by default."
 tdedit += "</td></tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Base Directory:</td>";
 tdedit += "<td><input name=\"basedirectory_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"204\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Now select the component type(s) that this End-Point will accept. If you select a single component ";
 tdedit += "type, the component created in the next step will be created with this same type (to allow it to be ";
 tdedit += "deployed to this End-Point). If you select multiple Component Types, the component created in the ";
 tdedit += "next step will be created with the first selected Component Type.";
 tdedit += "</td></tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">End Point Component Types:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select multiple name=\"servercomptype_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"servercomptype_field\" value=\"208\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"servercomptype_oldval\" value=\"\"/></td>";
 tdedit += "</tr></table></div>";
 // Application / Component
 tdedit += "<div id=\"setuppage19\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo.png\"><BR><HR>";
 tdedit += "<B>Page 8 of 8 - Application</B><BR>";
 tdedit += "Now let's create an <I>Application</I> to be Deployed. Initially, this Application will consist ";
 tdedit += "of a single <I>Component</I>. You can add additional Components later by editing the Application. ";
 tdedit += "The Component will pull file(s) from the <I>Repository</I> specified in step 3.";
 tdedit += "<table>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Application Name:</td>";
 tdedit += "<td><input name=\"appname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"appname_field\" value=\"1\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Component Name:</td>";
 tdedit += "<td><input name=\"compname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"compname_field\" value=\"1\"/>";
 tdedit += "<input type=\"hidden\" name=\"compcat_field\" value=\"1106\"/><input type=\"hidden\" name=\"comptype_field\" value=\"708\"/><input type=\"hidden\" name=\"bjtype_field\" value=\"711\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr><td colspan=\"3\"><table id=\"propdata2\" style=\"float:right\">";
 tdedit += "</table></tr>";
 tdedit += "</table></div>";
 // Introduction - Jenkins
 tdedit += "<div id =\"setuppage22\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo2.png\"><BR><HR>";
 tdedit += "<B>Page 1 of 4 - Introduction</B><BR>";
 tdedit += "Welcome to the DeployHub Quick Setup Wizard for Jenkins. Over the next few screens you can create all ";
 tdedit += "the initial objects necessary to get DeployHub up and running as a CD server for your Jenkins installation. ";
 tdedit += "Once the wizard is complete, you can chose to edit the resulting objects to add more details.<BR><BR>";
 tdedit += "This Wizard will create:<BR>";
 tdedit += "<UL><LI>An initial <I>Repository</I>. A repository is a source of deployment artifacts. The repository ";
 tdedit += "will point at your Jenkins Server to allow DeployHub to use http to download the archived file(s) ";
 tdedit += "for each build. Repositories can be found under the <I>Connections</I> view.</LI>";
 tdedit += "<LI>An initial <I>Environment</I>. An Environment represents a target for deployment and contains ";
 tdedit += "one or more <I>End Points</I>. Environments can be found under the <I>Data Center</I> view.</LI>";
 tdedit += "<LI>An initial <I>End Point</I>. This End Point will receives the build artifacts pulled from ";
 tdedit += "the Jenkins <I>Repository</I>.  End Points can be found under the <I>Data Center</I> view.</LI>";
 tdedit += "<LI>A <I>Build Engine</I> and <I>Build Job</I> to allow DeployHub to integrate with Jenkins. ";
 tdedit += "Build Engines can be found under the <I>Data Center</I> View.</LI>";
 tdedit += "<LI>An initial <I>Application</I>. An Application represents an object to be deployed to an ";
 tdedit += "<I>Environment</I>. An Application consists of one or more <I>Components</I>. Applications can be ";
 tdedit += "found under the <I>Deploy</I> View.</LI>";
 tdedit += "<LI>An initial <I>Component</I>. Components are deployed to <I>End Points.</I> This Component will ";
 tdedit += "be associated with the Build Job - this allows DeployHub to track which builds have been deployed to ";
 tdedit += "which End Point. Components can be found under the <I>Deploy</I> View.</LI>";
 tdedit += "<LI><I>Credentials</I> to allow access to the <I>Repository</I> and <I>End-Point</I>. Credentials ";
 tdedit += "can be found under the <I>Connections</I> View.</LI>";
 tdedit += "</UL>";
 tdedit += "The Wizard will create the relationships between all the objects automatically to allow you to start ";
 tdedit += "deploying straightaway!<BR><BR>";
 tdedit += "Click \"Next\" and let's get started!";
 tdedit += "</div>";
 // Jenkins Server Details - Jenkins WIzard
 tdedit += "<div id =\"setuppage24\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo2.png\"><BR><HR>";
 tdedit += "<B>Page 3 of 4 - Jenkins Server</B><BR>";
 tdedit += "Enter the details here for your Jenkins Server. The <I>Jenkins URL</I> should be the same as that shown under ";
 tdedit += "\"Jenkins URL\" under Jenkins -> Manage Jenkins -> Configure System. The <I>Jenkins Project Name</I> ";
 tdedit += "should be the name of the Jenkins Project whose output you want to deploy. The <I>Jenkins Output File ";
 tdedit += "Name</I> should be the file created by Jenkins as build output and represents the file to deploy."
 tdedit += "<table>";
 tdedit += "<tr><td>*Jenkins URL</td><td><input name=\"jenkinsurl2_val\" style='width:24em' type=\"text\" value=\"\"/></td></tr>";
 tdedit += "<tr><td>*Jenkins Project Name</td><td><input name=\"jenkinsproj2_val\" style='width:24em' type=\"text\" value=\"\"/></td></tr>";
 tdedit += "<tr><td>*Jenkins Output File Name</td><td><input name=\"jenkinsname2_val\" style='width:24em' type=\"text\" value=\"\"/></td></tr>";
 tdedit += "</table>";
 tdedit += "</div>"; 
 // Target Server Details - Jenkins WIzard
 tdedit += "<div id =\"setuppage25\" style=\"display:none\">";
 tdedit += "<img src=\"css/images/dhlogo2.png\"><BR><HR>";
 tdedit += "<B>Page 4 of 4 - Target Server</B><BR>";
 tdedit += "<table><tr>";
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the <I>Host Name</I> for this End-Point. This is the name of the host that DeployHub ";
 tdedit += "can use to connect to the End-Point (either via local hosts file or DNS). You can put an ";
 tdedit += "IP Address in here if the hostname cannot be resolved from DeployHub.";
 tdedit += "</td></tr><tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Host Name:</td>";
 tdedit += "<td><input name=\"hostname2_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">End Point Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"servertype2_val\">";
 tdedit += "<option value=\"st3\">as400</option>";
 tdedit += "<option value=\"st4\">mac</option>";
 tdedit += "<option value=\"st1\">unix</option>";
 tdedit += "<option value=\"st2\" selected>windows</option>";
 tdedit += "<option value=\"st5\">Tandem</option>";
 tdedit += "<option value=\"st6\">Stratus</option>";
 tdedit += "<option value=\"st7\">OpenVMS</option>";
 tdedit += "</td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Protocol:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"protocol2_val\">";
 tdedit += "<option value=\"ftp\">ftp</option>";
 tdedit += "<option value=\"ftps\">ftps</option>";
 tdedit += "<option value=\"sftp\">sftp</option>";
 tdedit += "<option value=\"win\" selected>win</option>";
 tdedit += "</td>";
 tdedit += "</tr>";
 
 tdedit += "<tr><td colspan=\"3\">";
 tdedit += "Enter the <I>Base Directory</I> for this End-Point. This is the directory that the build artifacts ";
 tdedit += "pulled from Jenkins will be placed into."
 tdedit += "</td></tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Base Directory:</td>";
 tdedit += "<td><input name=\"basedirectory2_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"204\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr><td colspan=\"2\">";
 tdedit += "Enter the username and password needed to access the End-Point. The values entered here will be used to ";
 tdedit += "create a <I>credential</I> which will be linked to the End-Point to allow it to be accessed.";
 tdedit += "</td></tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">User Name:</td>";
 tdedit += "<td><input name=\"serverusernameenc2_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
 tdedit += "<td><input name=\"serverpasswordenc2_val\" style='width:24em' type=\"password\" /></td>";
 tdedit += "</tr>";
 tdedit += "</table></div>";
 tdedit += "</div>"; 
 
 tdedit += "</form>";
 EditSetupButton();
 wtc();
}

function wtc()
{
	var x = $("#wt2").is(':checked');
	if (x) {
		wt=2;
		mp=5;
		$("#mp").html("4");
		$("#p3img").attr("src","css/images/dhlogo2.png");
	} else {
		wt=1;
		mp=9;
		$("#mp").html("8");
		$("#p3img").attr("src","css/images/dhlogo.png");
	}
	
}

function orchecked(rc)
{
	var t = $("#propoverride_"+rc).is(':checked');
	if (t) {
		$("#cirow"+rc).show();
	} else {
		$("#cirow"+rc).hide();
	}
	
}

function ujchecked()
{
	var t = $("#use_jenkins").is(':checked');
	if (t) {
		$("#jenkins_attrs").show();
	} else {
		$("#jenkins_attrs").hide();
	}
}

function apchecked(rc)
{
	var t = $("#propappend_"+rc).is(':checked');
	if (t) {
		$("#cirow"+rc).show();
	} else {
		$("#cirow"+rc).hide();
	}
}

function LoadPage(pageno)
{
	// alert("Load page "+pageno+" #setuppage"+wt.toString()+pageno.toString());
	$('div[id^="setuppage"]').hide();
	if (pageno==1 || pageno==3) {
		$("#setuppage"+pageno.toString()).show();
	} else {
		$("#setuppage"+wt.toString()+pageno.toString()).show();
	}
}


function EditSetupButton()
{
 var pageno=1
 var pwd = parent.$("#modal");
 
 var buttons1 = [
                 {
                	 text : "Next",
                	 click : function()
                	 {
                		 pageno++;
                		 LoadPage(pageno);
                		 pwd.dialog("option", "buttons", buttons2);
                		 
                	 }
                 },
                 {
                	 text : "Cancel",
                	 click : function()
                	 {
                		 parent.$("#modal").dialog("close");
                	 }
                 } ];
 var buttons2 = [
				{
					 text : "Prev",
					 click: function()
					 {
						 pageno--;
						 LoadPage(pageno);
						 if (pageno == 1) {
							 pwd.dialog("option", "buttons", buttons1);
						 }
					 }
				},
                 {
                	 text : "Next",
                	 click : function()
                	 {
                		 if (pageno==4) {
                			 var url="";
                			 var jpn="";
                			 var cf=false;
                			 if (wt==2) {
                				 url=summform.jenkinsurl2_val.value.trim();
                				 jpn=summform.jenkinsproj2_val.value.trim();
                				 cf=true;
                			 } else {
                				 if ($("#use_jenkins").is(':checked')) {
                					 if (summform.bename_val.value.trim() == "") {
                						 alert("Build Engine Name is required");
                						 return;
                					 }
                					 jpn=summform.jenkinsproj_val.value.trim();
                					 url=summform.jenkinsurl_val.value.trim();
                					 cf=true;
                				 }
	                			 
                			 }
                			 if (cf) {
		                		 if (url == "") {
		                			 alert("Jenkins Server URL is required");
		                			 return;
		                		 }
		                		 if (url.substr(0,8)!="https://" && url.substr(0,7)!="http://") {
		                			 alert("Jenkins Server URL is invalid (must be http[s]://...");
		                			 return;
		                		 }
		                		 if (jpn == "") {
	                				 alert("Jenkins Project Name is required");
	                				 return;
	                			 }
                			 }
                		 }
                		 if (pageno==5 && wt==1) {
                			if (summform.reponame_val.value.trim() == "") {
                				 alert("Repository Name is required");
                				 return;
                			}
                		 }
                		 if (pageno==6 && wt==1) {
                 			// Need to check the required attributes for the specific
                			// repository type.
                			 for (var q in propdefs) {
                				 if (propdefs[q].reqd) {
                					 // This is a required repository field - is it set?
                					 if ($("#idpropval_"+q).val() == "") {
                						 alert(q+" is required");
                						 return;
                					 }
                					 
                				 }
                			 }
                 		 }
                		 if (pageno==7 && wt==1) {
                			 if (summform.envname_val.value.trim() == "")
                			 {
                				 alert("Environment Name is required");
                				 return;
                			 }

                			 if (summform.servername_val.value.trim() == "")
                			 {
                				 alert("End Point Name is required");
                				 return;
                			 }

                			 if (summform.hostname_val.value.trim() == "")
                			 {
                				 alert("Host Name is required");
                				 return;
                			 }
                  		 }
                		 if (pageno==8 && wt==1) {
                			 if (summform.basedirectory_val.value.trim() == "")
                			 {
                				 alert("Base Directory is required");
                				 return;
                			 }
                		 }
                		 pageno++;
                		 LoadPage(pageno);
                		 if (pageno == mp) {
                			 pwd.dialog("option", "buttons", buttons3);
                		 }
                	 }
                 }, 
                 {
                	 text : "Cancel",
                	 click : function()
                	 {
                		 parent.$("#modal").dialog("close");
                	 }
                 } ];
 var buttons3 = [
                 {
                	 text : "Prev",
                	 click: function()
                	 {
                		 pageno--;
                		 LoadPage(pageno);
                		 pwd.dialog("option", "buttons", buttons2);
                	 }
                 },
                 {
                	 text : "Finish",
                	 click : function()
                	 {
                		 if (wt==1) {
	                		 if (summform.appname_val.value.trim() == "")
	            			 {
	            				 alert("Application Name is required");
	            				 return;
	            			 }
	                		 if (summform.compname_val.value.trim() == "")
	            			 {
	            				 alert("Component Name is required");
	            				 return;
	            			 }
                		 }
                		 SaveSetupData(parent.$("#modal"), summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
                	 }
                 },
                 {
                	 text : "Cancel",
                	 click : function()
                	 {
                		 parent.$("#modal").dialog("close");
                	 }
                 } ];

 pwd.empty();
 pwd.html(tdedit);
 

 pwd.dialog(
 {
  modal : true,
  open : function(event, ui)
  {
   var myform = pwd.find("#summform");
   
   if (myform.find(":input[name=\"owner_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetUsers",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var owner = myform.find(":input[name=\"owner_val\"]");

      var userid = save_owner_val;
      
      if (userid == "")
       userid = myuserid;
    
      for (n = 0; n < res.length; n++)
      {
       if (userid == res[n].name)
        owner.append('<option id="owner' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="owner' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
      
      $.ajax(
        {
         url : "GetGroups",
         dataType : 'json',
         type : 'GET',
         success : function(res)
         {

          // res =
          // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
          var myform = pwd.find("#summform");
          var owner = myform.find(":input[name=\"groupowner_val\"]");

          var group = save_groupowner_val;
       
          for (n = 0; n < res.length; n++)
          {
           if (group == res[n].name)
            owner.append('<option id="groupowner' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
           else
            owner.append('<option id="groupowner' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
          }
          $("#ownertype_val").change(ToggleOwner);
          
          ToggleOwner();
         },
         error : function(jqxhr, status, err)
         {
          console.log(status);
          console.log(err);
         }
        });
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"setuptype_val\"]").length > 0)
   {
    var t = obj2Int["re"][0];

    $.ajax(
    {
     url : 'GetProviderDefs',
     data :
     {
      objtype : t
     },
     success : function(res)
     {
      var myform = pwd.find("#summform");
      var typeobj = myform.find(":input[name=\"setuptype_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_type_val == res[n].name)
        typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        typeobj.append('<option id="type' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"domain_val\"]").length > 0)
   {

    $.ajax(
    {
     url : 'GetAllDomains',
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#summform");
      var typeobj = myform.find(":input[name=\"domain_val\"]");

      for (n = 0; n < res.length; n++)
      {
        typeobj.append('<option id="type' + n + '" value=\"' + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   } 
   

   if (myform.find(":input[name=\"servercomptype_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetComponentData?comptype=y",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#summform");
      var comptype = myform.find(":input[name=\"servercomptype_val\"]");
      var cnt = 0;

      for (n = 0; n < res.data.length; n++)
      {
        comptype.append('<option id="servercomptype' + n + '" value=\"' + res.data[n].id + "\">" + res.data[n].name + '</option>');
      }
      
      if (cnt == 0)
       comptype.prop('selectedIndex', 0);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   } 
 
   $("#setuptype_val").change(GetSetupProps);

   GetSetupProps();
  }
 });
 

 
 pwd.dialog("option", "title", "Quick Setup");
 pwd.dialog("option", "height", "660");
 pwd.dialog("option", "width", "600");	// fix width
 pwd.dialog("option", "buttons", buttons1);
 pwd.dialog('open');
}

function GetSetupProps()
{ 
 var pwd = parent.$("#modal");
 var myform = pwd.find("#summform");
 var setuptype = myform.find(":input[name=\"setuptype_val\"]");
 var id = setuptype.val();
 
 if (id == null || id == "")
  id = "pd10";
 
 id = id.substring(2);
 
 $.ajax(
   {
    url : "GetPropertiesData?objtype=8&id=" + id + "&defonly=Y",
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     var propdata = $("#propdata");
     var propdata2 = $("#propdata2");
     console.log(res);
     var td1 = "<tr><th>&nbsp;</th><th>Value</th><th>Encrypted</th><th>Override</th><th>Append</th></tr>";
     var td2 = "<tr><th>&nbsp;</th><th>Value</th></tr>";
     var rowcnt = 0;

     for (a = 0; a < res.defs.length; a++)
     {
      var row = res.defs[a];
      if (row.name == "dsn")
       row.name = "data source name";
      
      console.log("propdefs["+row.name+"].reqd="+row.reqd);
      
      propdefs[row.name] =
      {
       "append" : row.append,
       "reqd" : row.reqd,
       "used" : false
      };
     
       var req = "";
       if (row.reqd)
        req = "*";
        
       console.log("idpropval_"+row.name);
       if (row.append)
        td1 += "<tr><td  align=\"center\" id=\"propname_" + rowcnt + "\">" + req+ row.name + "</td><td align=\"center\"><input id=\"idpropval_"+row.name+"\" name=\"propval_" + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propenc_"  + row.name + "\" /></td><td align=\"center\"><input onclick=\"javascript:orchecked("+rowcnt+");\" type=\"checkbox\" id=\"propoverride_"+rowcnt+"\" name=\"propoverride_" + row.name + "\" /></td><td align=\"center\"><input onclick=\"javascript:apchecked("+rowcnt+");\" type=\"checkbox\" id=\"propappend_"+rowcnt+"\" name=\"propappend_" + row.name + "\" /></td></tr>";
       else
        td1 += "<tr><td  align=\"center\" id=\"propname_" + rowcnt + "\">" + req+ row.name + "</td><td align=\"center\"><input id=\"idpropval_"+row.name+"\" name=\"propval_" + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propenc_"  + row.name + "\" /></td><td align=\"center\"><input onclick=\"javascript:orchecked("+rowcnt+");\" type=\"checkbox\" id=\"propoverride_"+rowcnt+"\" name=\"propoverride_" + row.name + "\" /></td><td align=\"center\">&nbsp;</td></tr>";

      td2 += "<tr style=\"display:none\" id=\"cirow"+rowcnt+"\"><td align=\"center\" id=\"propname2_" + rowcnt + "\">" + req + row.name + "</td><td align=\"center\"><input name=\"propval2_" + row.name + "\" /></td></tr>";
      rowcnt++;
     }

    propdata.html(td1);
    propdata2.html(td2);

    },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
   }); 
}

function GetSetupSummaryData(instance, data, SaveType)
{
	console.log("In GetSetupSummaryData, wt="+wt);
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view = {};

 view["servercomptype_val"] = "";
 
 for ( var i in viewArr)
 {
  if (viewArr[i].name == "servercomptype_val")
   view[viewArr[i].name] += viewArr[i].value + ";";
  else
   view[viewArr[i].name] = viewArr[i].value;
 }

 console.log(view);

 if (SaveType == "Credentials")
 {
  if (view.reponame_val == "") view.reponame_val = "Credential"+SetupRepoCredId;
  data.newname = view.reponame_val;
  console.log('change_' + view.reponame_field + ' = ' + view.reponame_val);
  data['change_' + view.reponame_field] = view.reponame_val;

  console.log('change_801 = ck2');
  data['change_801' ] = 'ck2';
  
  var ownertype = $("#ownertype_val");
  var owt = "User";

  if (ownertype.length > 0)
   owt = ownertype.val();

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  }

  if (wt==1) {
	  console.log('change_' + view.repousernameenc_field + ' = ' + view.repousernameenc_val);
	  data['change_' + view.repousernameenc_field] = view.repousernameenc_val;
	
	  console.log('change_' + view.repopasswordenc_field + ' = ' + view.repopasswordenc_val);
	  data['change_' + view.repopasswordenc_field] = view.repopasswordenc_val;
	  ret = (view.repousernameenc_val!="" || view.repopasswordenc_val!=""); // true if either set, false otherwise
  } else {
	  console.log('change_' + view.repousernameenc_field + ' = ' + view.repousernameenc2_val);
	  data['change_' + view.repousernameenc_field] = view.repousernameenc2_val;
	
	  console.log('change_' + view.repopasswordenc_field + ' = ' + view.repopasswordenc2_val);
	  data['change_' + view.repopasswordenc_field] = view.repopasswordenc2_val;
	  ret = (view.repousernameenc2_val!="" || view.repopasswordenc2_val!=""); // true if either set, false otherwise
  }
 }
 else if (SaveType == "Repository")
 {
  if (view.reponame_val == "") view.reponame_val = "Repository"+SetupRepoId;
  data.newname = view.reponame_val;
  console.log('change_' + view.reponame_field + ' = ' + view.reponame_val);
  data['change_' + view.reponame_field] = view.reponame_val;

  if (wt==2) {
	  console.log('change_' + view.setuptype_field + ' = pd14');
	  data['change_' + view.setuptype_field] = "pd14";	// Provider Def 14 = http repository type.
  } else {
	  console.log('change_' + view.setuptype_field + ' = ' + view.setuptype_val);
	  data['change_' + view.setuptype_field] = view.setuptype_val;
  }

  if (SetupRepoCredId != "") {
	  console.log('change_302 = ' + SetupRepoCredId);
	  data['change_302'] = "cr" + SetupRepoCredId;
  }

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 

  ret = true;
 }
 else if (SaveType == "ServerCredentials")
 {
  if (view.servername_val == "") view.servername_val = "Credentials"+SetupServerCredId;
  data.newname = view.servername_val;
  console.log('change_' + view.servername_field + ' = ' + view.servername_val);
  data['change_' + view.servername_field] = view.servername_val;

  console.log('change_801 = ck2');
  data['change_801' ] = 'ck2';
  
  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  }
  
  if (wt==1) {
	  console.log('change_' + view.serverusernameenc_field + ' = ' + view.serverusernameenc_val);
	  data['change_' + view.serverusernameenc_field] = view.serverusernameenc_val;
	  
	  console.log('change_' + view.serverpasswordenc_field + ' = ' + view.serverpasswordenc_val);
	  data['change_' + view.serverpasswordenc_field] = view.serverpasswordenc_val;
	  
	  ret = (view.serverusernameenc_val!="" || view.serverpasswordenc_val!=""); // true if either set, false otherwise
	  
  } else {
	  console.log('change_' + view.serverusernameenc_field + ' = ' + view.serverusernameenc2_val);
	  data['change_' + view.serverusernameenc_field] = view.serverusernameenc2_val;
	  
	  console.log('change_' + view.serverpasswordenc_field + ' = ' + view.serverpasswordenc2_val);
	  data['change_' + view.serverpasswordenc_field] = view.serverpasswordenc2_val;
	  
	  ret = (view.serverusernameenc2_val!="" || view.serverpasswordenc2_val!=""); // true if either set, false otherwise
  }
 }
 else if (SaveType == "Server")
 {
  if (view.servername_val == "") view.servername_val = "Server"+SetupServerId;
  data.newname = view.servername_val;
  console.log('change_' + view.servername_field + ' = ' + view.servername_val);
  data['change_' + view.servername_field] = view.servername_val;

  if (wt==1) {
	  console.log('change_' + view.servertype_field + ' = ' + view.servertype_val);
	  data['change_' + view.servertype_field] = view.servertype_val;
	
	  console.log('change_' + view.hostname_field + ' = ' + view.hostname_val);
	  data['change_' + view.hostname_field] = view.hostname_val;
	
	  console.log('change_' + view.protocol_field + ' = ' + view.protocol_val);
	  data['change_' + view.protocol_field] = view.protocol_val;
	
	  console.log('change_' + view.basedirectory_field + ' = ' + view.basedirectory_val);
	  data['change_' + view.basedirectory_field] = view.basedirectory_val;
  } else {
	  console.log('change_' + view.servertype_field + ' = ' + view.servertype2_val);
	  data['change_' + view.servertype_field] = view.servertype2_val;
	
	  console.log('change_' + view.hostname_field + ' = ' + view.hostname2_val);
	  data['change_' + view.hostname_field] = view.hostname2_val;
	
	  console.log('change_' + view.protocol_field + ' = ' + view.protocol2_val);
	  data['change_' + view.protocol_field] = view.protocol2_val;
	
	  console.log('change_' + view.basedirectory_field + ' = ' + view.basedirectory2_val);
	  data['change_' + view.basedirectory_field] = view.basedirectory2_val;
  }

  if (view.servercomptype_val != view.servercomptype_oldval)
  {
   console.log('change_' + view.servercomptype_field + "=" + view.servercomptype_val);
   data['change_' + view.servercomptype_field] = view.servercomptype_val;
   ret = true;
  }
  
  console.log('change_205 = ' + SetupServerCredId);
  data['change_205'] = 'cr' + SetupServerCredId;

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 

  ret = true;
 }
 else if (SaveType == "Environment")
 {
  if (view.envname_val == "") view.envname_val = "Environment"+SetupEnvId;
  data.newname = view.envname_val;
  console.log('change_' + view.envname_field + ' = ' + view.envname_val);
  data['change_' + view.envname_field] = view.envname_val;

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 
 }
 else if (SaveType == "Application")
 {
	 if (view.appname_val == "") view.appname_val = "Application"+SetupAppId;
	 data.newname = view.appname_val;
	 console.log('change_' + view.appname_field + ' = ' + view.appname_val);
	 data['change_' + view.appname_field] = view.appname_val;
	 if (owt == "User")
	 { 
		 console.log('change_' + view.owner_field + ' = ' + view.owner_val);
		 data['change_' + view.owner_field] = view.owner_val;
	 }
	 else
	 {
		 console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
		 data['change_' + view.groupowner_field] = view.groupowner_val;
	 }
	 ret = true;
 }
 else if (SaveType == "Component")
 {
	 if (view.compname_val == "") view.compname_val = "Component"+SetupCompId;
	 data.newname = view.compname_val;
	 console.log('change_' + view.compname_field + ' = ' + view.compname_val);
	 data['change_' + view.compname_field] = view.compname_val;
	 console.log('change_' + view.compcat_field + ' = cy10');	// General
	 data['change_' + view.compcat_field] = 'cy10';				// General Category
	 var comptype = view.servercomptype_val.replace(/;.*$/,"");	// first value only
	 console.log('change_' + view.comptype_field + ' = ' + comptype);
	 data['change_' + view.comptype_field] = comptype;
	 if (owt == "User")
	 { 
		 console.log('change_' + view.owner_field + ' = ' + view.owner_val);
		 data['change_' + view.owner_field] = view.owner_val;
	 }
	 else
	 {
		 console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
		 data['change_' + view.groupowner_field] = view.groupowner_val;
	 }
	 if (($("#use_jenkins").is(':checked') && SetupBuildJobId != "") || wt==2) {
		 // Add Build Job to Component
		 console.log('change_' + view.bjtype_field + ' = bj' + SetupBuildJobId);
		 data['change_' + view.bjtype_field] = "bj"+SetupBuildJobId;
	 }
	 ret = true;
 }
 else if (SaveType == "BuildEngine")
 {
	 if (view.bename_val == "") view.bename_val = "BuildEngine"+SetupBuildEngineId;
	 data.newname = view.bename_val;
	 console.log('change_' + view.bename_field + '=' + view.bename_val);
	 data['change_' + view.bename_field] = view.bename_val;
	 console.log('change_' + view.betype_field + '= "pd21"');
	 data['change_' + view.betype_field] = "pd21";
	 if (owt == "User")
	 { 
		 console.log('change_' + view.owner_field + ' = ' + view.owner_val);
		 data['change_' + view.owner_field] = view.owner_val;
	 }
	 else
	 {
		 console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
		 data['change_' + view.groupowner_field] = view.groupowner_val;
	 }
	 ret = true;
 }
 else if (SaveType == "BuildJob")
 {
	 if (wt==1) {
		 data.newname = "Project "+view.jenkinsproj_val;
		 console.log('change_' + view.bjname_field + '= Project ' + view.jenkinsproj_val);
		 data['change_' + view.bjname_field] = "Project " + view.jenkinsproj_val;
		 console.log('change_' + view.pnname_field + '=' + view.jenkinsproj_val);
		 data['change_' + view.pnname_field] = view.jenkinsproj_val;
	 } else {
		 data.newname = "Project "+view.jenkinsproj2_val;
		 console.log('change_' + view.bjname_field + '= Project ' + view.jenkinsproj2_val);
		 data['change_' + view.bjname_field] = "Project " + view.jenkinsproj2_val;
		 console.log('change_' + view.pnname_field + '=' + view.jenkinsproj2_val);
		 data['change_' + view.pnname_field] = view.jenkinsproj2_val;
	 }
	 ret = true;
 }

 return ret;
}

function SaveSetupData(instance)
{
 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view = {};

 for ( var i in viewArr)
 {
  if (viewArr[i].value == "on")
   viewArr[i].value = "Y";

  if (viewArr[i].value == "off")
   viewArr[i].value = "N";
  
  view[viewArr[i].name] = viewArr[i].value;
 }

// Edit Checks - shouldn't be necessary since we've checked on each page next click 
 if (wt==1) {
	 if (view.reponame_val.trim() == "")
	 {
	  alert("Repository Name is required");
	  return;
	 }
	 
	 if (view.envname_val.trim() == "")
	 {
	  alert("Environment Name is required");
	  return;
	 }
	 
	 if (view.servername_val.trim() == "")
	 {
	  alert("End Point Name is required");
	  return;
	 }
	 
	 if (view.hostname_val.trim() == "")
	 {
	  alert("Host Name is required");
	  return;
	 }
	 
	 if (view.basedirectory_val.trim() == "")
	 {
	  alert("Base Directory is required");
	  return;
	 }
 }
 
 $.blockUI();
 var domainid = view.domain_val;
 var domainname=$("#domain_val option:selected").text();

 var SaveList = [ "Credentials", "Repository", "ServerCredentials", "Environment", "Server", "Application", "BuildEngine", "BuildJob", "Component" ];

 for (var k=0; k < SaveList.length; k++)
 {
	 console.log("wt="+wt+" k="+k+" SaveList["+k+"]="+SaveList[k]);
	 var savedata = { objtype : -1, id : "" };

	 if (SaveList[k] == "BuildEngine" && $("#use_jenkins").is(':checked') == false && wt==1) continue;
	 if (SaveList[k] == "BuildJob" && $("#use_jenkins").is(':checked') == false && wt==1) continue;
	 if (SaveList[k] == "Credentials" && view.repousernameenc_val=="" && view.repopasswordenc_val=="") continue;
	 if (SaveList[k] == "ServerCredentials") {
		 if (wt==1 && view.serverusernameenc_val=="" && view.serverpasswordenc_val=="") continue;
		 if (wt==2 && view.serverusernameenc2_val=="" && view.serverpasswordenc2_val=="") continue;
	 }
	 
	 var parentid=domainid;
	 if (SaveList[k] == "BuildJob") parentid = SetupBuildEngineId;
	 if (SaveList[k] == "Server") parentid = SetupEnvId;
	 
	 SetupNewID(SaveList[k], parentid, view);

	 if (SaveList[k] == "Credentials")
	 {
		 savedata = { objtype : 19, id : SetupRepoCredId };
	 }
	 else if (SaveList[k] == "Repository")
	 {
		 savedata = { objtype : 8, id : SetupRepoId };
	 }
	 else if (SaveList[k] == "ServerCredentials")
	 {
		 savedata = { objtype : 19, id : SetupServerCredId };
	 }
	 else if (SaveList[k] == "Server")
	 {
		 savedata = { objtype : 4, id : SetupServerId };
	 }
	 else if (SaveList[k] == "Environment")
	 {
		 savedata = { objtype : 2, id : SetupEnvId };
	 }
	 else if (SaveList[k] == "Application")
	 {
		 savedata = { objtype : 5, id : SetupAppId };
	 }
	 else if (SaveList[k] == "Component")
	 {
		 savedata = { objtype : 13, id : SetupCompId };
	 }
	 else if (SaveList[k] == "BuildEngine")
	 {
		 savedata = { objtype : 24, id : SetupBuildEngineId };
	 }
	 else if (SaveList[k] == "BuildJob")
	 {
		 savedata = { objtype : 25, id : SetupBuildJobId };
	 }

	 if (savedata.id == "")
		 continue;

	 console.log("Calling GetSetupSummaryData");
	 var sumret = GetSetupSummaryData(instance, savedata, SaveList[k]);

	 console.log(savedata);

	 if (sumret)
	 {
		 $.ajax(
				 {
					 url : "UpdateSummaryData",
					 dataType : "json",
					 type : "POST",
					 async : false,
					 data : savedata,
					 success : function(data)
					 {
						 console.log(data);
					 }
				 });
	 }
 }
 
 // Add properties data for Repository
 console.log("**** adding properties for repository");
 var data =
 {
  objtype : 8,
  id : SetupRepoId 
 };
 
 if (wt==1) {
	 for (var x in view)
	 {
		 if (x.indexOf("propval_") == 0)
		 {
			 var name = x.substring(8);
			 console.log("name="+name);


			 if (view['propappend_' + name] != "Y" && view['propappend_' + name] != "N")
				 view['propappend_' + name] = 'N';

			 if (view['propenc_' + name] != "Y" && view['propenc_' + name] != "N")
				 view['propenc_' + name] = 'N';

			 if (view['propoverride_' + name] != "Y" && view['propoverride_' + name] != "N")
				 view['propoverride_' + name] = 'N';

			 var val = view['propenc_' + name] + view['propoverride_' + name] + view['propappend_' + name] + view['propval_' + name];
			 var key = "add_" + name;

			 console.log("data["+key+"]=\""+val+"\"");

			 data[key] = val;
		 } 
	 }
 } else {
	 // Jenkins - repository is http
	 data["add_URI"]="NNYjob/"+view.jenkinsproj2_val;	// Append
	 data["add_pattern"]="NYN";							// Override
	 // data["add_port"]="NNN";
	 // data["add_params"]="NNN";
	 data["add_server"]="NNN"+view.jenkinsurl2_val;
	 data["add_version"]="NYN";							// Override
 }
 
 console.log("**** data is:");
 console.log(data);
 
 $.ajax(
   {
    url : "UpdatePropertiesData",
    type : 'POST',
    async: false,
    dataType : "json",
    data : data,
    success : function(data)
    {
    }
   });
 
 console.log("**** done adding properties for repository");
 
 // Now do the same thing for Build Engine (if adding)
 if ($("#use_jenkins").is(':checked') || wt==2) {
	 data = {
	  objtype: 24,
	  id: SetupBuildEngineId
	 };
	 
	 console.log("Adding Server URL wt="+wt+" view.jenkinsurl2=["+view.jenkinsurl2_val+"]");
	 data["add_Server URL"] = "NNN" + ((wt==1)?view.jenkinsurl_val:view.jenkinsurl2_val);
	 
	 console.log(data);
	 
	 $.ajax(
	   {
	    url : "UpdatePropertiesData",
	    type : 'POST',
	    async: false,
	    dataType : "json",
	    data : data,
	    success : function(data)
	    {
	    	console.log("Success!");
	    	console.log(data);
	    },
	    error: function(data)
	    {
	    	console.log("Failed!");
	    	console.log(data);
	    }
	 });
 }
 
 
 // Add Server to Environment
 console.log("UpdateAttrs?f=sm&e="+ SetupEnvId + "&s=" + SetupServerId + "&xpos=20&ypos=20");
 
 $.ajax(
  {
   url: "UpdateAttrs?f=sm&e="+ SetupEnvId + "&s=" + SetupServerId + "&xpos=20&ypos=20",
   type: 'GET',
   dataType: "text",
   async: false,
   success: function(data)
   {
   }
  });
 
 // Add Component Item to Component
 $.ajax({
	 url: "UpdateAttrs?f=inv&c="+SetupCompId+"&xpos=360&ypos=80",
	 type: 'GET',
	 async: true,
	 success: function(data)
	 {
		 console.log("Created Component item "+data.itemid);
		 // Now update the component item details
		 savedata = { objtype : 14, id : data.itemid };
		 savedata["change_704"] = "re"+SetupRepoId;
		 $.ajax(
				 {
					 url : "UpdateSummaryData",
					 dataType : "json",
					 type : "POST",
					 async : true,
					 data : savedata,
					 success : function()
					 {
						 // Now add any details for the component item that override
						 // the repository settings.
						 var t="";
						 if (wt==1) {
							 for (var x in view) {
								 if (x.indexOf("propval2_") == 0) {
									 console.log("x="+x);
									 var name = x.substring(9);
									 console.log("name="+name);
									 console.log("append = "+view['propappend_'+name]);
									 console.log("override = "+view['propoverride_'+name]);
									 if (view['propappend_'+name]=='Y' || view['propoverride_'+name]=='Y') {
										 console.log("Taking component item data");
										 var cidata = view[x];
										 console.log("cidata=["+cidata+"]");
										 t=t+"&prop_add_"+name+"="+cidata;
										 console.log("t="+t);
									 }
								 }
							 }
						 } else {
							 // Jenkins Wizard - add http overrides here
							 t=t+"&prop_add_pattern="+view.jenkinsname2_val;
							 t=t+"&prop_add_URI=${component.lastbuild}/artifact";
							 t=t+"&prop_add_version=${component.lastbuild}";
						 }
						 if (t!="") {
							 $.ajax({
								 url: "GetComponentItem?ciid="+data.itemid+"&oldrows=Y"+t,
								 type: 'POST',
								 async: true,
								 success: function(data)
								 {
								 }
							 });
						 }
					 }
				 });
	 }
 })
 // Add Component to Application
 $.ajax({
		 url: "UpdateAttrs?f=acvm&a="+SetupAppId+"&c="+SetupCompId+"&xpos=440&ypos=100",
		 type: 'GET',
		 dataType: "text",
		 async: true,
		 success: function(data) {
			 $.ajax({
				 url: "UpdateAttrs?f=cal&a=" + SetupAppId + "&fn=0&tn="+SetupCompId,
				 type: 'GET',
				 dataType: "text",
				 async: true,
				 success: function(data)
				 {
				 }
			 });
		 }
 });
 // Associate Application with Environment
 $.ajax({
	 url : "GetAppVersInEnvData",
	 async : false,
	 data :
	 {
		 reason : 'set',
		 isversion : false,
		 envid : SetupEnvId,
		 appid : SetupAppId,
		 note : 'Associated during setup'
	 },
	 dataType : 'json',
	 type : 'POST',
	 success : function(res)
	 {
	 },
	 error : function(jqxhr, status, err)
	 {
		 console.log(status);
		 console.log(err);
	 }
 });
 
 if (wt==2) {
	 // Add deploy task to specified domain
	 $.ajax({
		 url : "DomainDetails?f=at&domainid="+domainid+"&tasktype=Deploy",
		 async : false,
		 type : 'POST',
		 success : function(res)
		 {
			 console.log("success");
			 console.log(res);
			 SetupDeployTaskName = res.name;
			 SetupDeployTaskId = res.id;
			 $.ajax({
				 url : "DomainDetails?f=ag&gid=1&domainid="+domainid+"&tid="+SetupDeployTaskId,
				 async : false,
				 type : 'POST',
				 success : function(res)
				 {
					 console.log("Added group everyone");
				 }
			 });
		 }
	 });
 }
 
 var pwd = parent.$("#modal");
 pwd.empty();
 var tdedit = (wt==1)?"<img src=\"css/images/dhlogo.png\"><BR><HR>":"<img src=\"css/images/dhlogo2.png\"><BR><HR>";
 tdedit += "<B>Wizard Complete</B><BR>";
 tdedit += "The following objects have been created:<BR>";
 tdedit += "<table class=\"dev_table\">";
 tdedit += "<tr><td>Application:</td><td><img style=\"vertical-align:middle\" src=\"css/images/application_16x.png\">&nbsp;<a href='javascript:SwitchDisplay(\"ap"+SetupAppId+"\");'>"+SetupAppName+"</td></tr>";
 tdedit += "<tr><td>Component:</td><td><img style=\"vertical-align:middle\" src=\"css/images/component_16x.png\">&nbsp;<a href='javascript:SwitchDisplay(\"co"+SetupCompId+"\");'>"+SetupCompName+"</td></tr>";
 tdedit += "<tr><td>Environment:</td><td><img style=\"vertical-align:middle\" src=\"css/images/environment_16x.png\">&nbsp;<a href='javascript:SwitchDisplay(\"en"+SetupEnvId+"\");'>"+SetupEnvName+"</td></tr>";
 tdedit += "<tr><td>End Point:</td><td><img style=\"vertical-align:middle\" src=\"images/server.png\">&nbsp;<a href='javascript:SwitchDisplay(\"se"+SetupServerId+"\");'>"+SetupServerName+"</td></tr>";
 tdedit += "<tr><td>Repository:</td><td><img style=\"vertical-align:middle\" src=\"images/repository.png\">&nbsp;<a href='javascript:SwitchDisplay(\"re"+SetupRepoId+"\");'>"+SetupRepoName+"</td></tr>";
 if (SetupBuildEngineId!="") {
	 tdedit += "<tr><td>Build Engine:</td><td><img style=\"vertical-align:middle\" src=\"css/images/buildserv_16x.png\">&nbsp;<a href='javascript:SwitchDisplay(\"be"+SetupBuildEngineId+"\");'>"+SetupBuildEngineName+"</td></tr>";
	 tdedit += "<tr><td>Build Job:</td><td><img style=\"vertical-align:middle\" src=\"images/buildjob_16x.png\">&nbsp;<a href='javascript:SwitchDisplay(\"bj"+SetupBuildJobId+"\");'>"+SetupBuildJobName+"</td></tr>";
 }
 tdedit += "</table>";
 if (wt==2) {
	 tdedit += "<br>A <I>Deploy Task</I> \""+SetupDeployTaskName+"\" has been created in domain \""+domainname+"\" to allow ";
	 tdedit += "deployments to take place immediately. You way wish to rename or remove this task as required.<br>";
	 tdedit += "<br>If you have not already done so, you should now install the Jenkins plugin for DeployHub. ";
	 tdedit += "Then, within Jenkins configure the project \""+view.jenkinsproj2_val+"\" and add the plugin as ";
	 tdedit += "a post-build step. If you wish to perform continuous deployment, set the plug-in to deploy ";
	 tdedit += "application \""+SetupAppName+"\" to environment \""+SetupEnvName+"\".";
 }
 
// var SetupRepoCredName = "";
// var SetupServerCredName = "";
 pwd.html(tdedit);
 var buttons = [
                {
                	text : "Close",
                	click : function()
                	{
                		parent.$("#modal").dialog("close");
                	}
                } ];
 pwd.dialog("option", "buttons", buttons);
 $.unblockUI();
 return;
}

function SetupNewID(ot, parentid, view)
{
 var objtype = ot;
 
 if (ot == "ServerCredentials")
  objtype = "Credentials";
 
 var name = "";
 var ptype="Domain";


 if (ot == "Credentials")
  name  = view.reponame_val;
 else if (ot == "Repository")
  name  = view.reponame_val;
 else if (ot == "ServerCredentials")
 { 
  //if (view.reponame_val == view.servername_val)
  // view.servername_val += "_server";
  name  = view.servername_val;
 }
 else if (ot == "Server") {
  name  = view.servername_val;
  ptype="Environment";
 } else if (ot == "Environment")
  name  = view.envname_val;
 else if (ot == "Application")
  name = view.appname_val;
 else if (ot == "Component")
  name = view.compname_val;
 else if (ot == "BuildEngine")
  name = (wt==1)?view.bename_val:view.bename2_val;
 else if (ot == "BuildJob") {
	 name = "Project "+((wt==1)?view.jenkinsproj_val:view.jenkinsproj2_val);
	 ptype="BUILDER";
 }
 
 var url = "GetNewID?objtype="+objtype+"&pid="+parentid + "&ptype="+ptype+"&at=&name="+name + "&treeid=";

 console.log(url);
 
 $.ajax({
  url: url,
  dataType: 'json',
  async: false,
  success: function(data) {
   console.log(data);
   $.each(data, function(key, val) {
    if (key == "name") name = val;
    if (key == "id") id = val;
    if (key == "rt") rt = val;
   });
   console.log("done creating in tree new id ="+id);
   if (ot == "Credentials") {
    SetupRepoCredId = id.substring(2);
    SetupRepoCredName = name;
   } else if (ot == "Repository") {
    SetupRepoId = id.substring(2);
    SetupRepoName = name;
   } else if (ot == "ServerCredentials") {
    SetupServerCredId = id.substring(2);
    SetupServerCredName = name;
   } else if (ot == "Server") {
    SetupServerId = id.substring(2);
    SetupServerName = name;
   } else if (ot == "Environment") {
    SetupEnvId = id.substring(2);
    SetupEnvName = name;
   } else if (ot == "Application") {
	SetupAppId = id.substring(2);
	SetupAppName = name;
   } else if (ot == "Component") {
	SetupCompId = id.substring(2);
	SetupCompName = name;
   } else if (ot == "BuildEngine") {
	SetupBuildEngineId = id.substring(2);
	SetupBuildEngineName = name;
   } else if (ot == "BuildJob") {
	SetupBuildJobId = id.substring(2);
	SetupBuildJobName = name;
   }
  }
 });
} 




