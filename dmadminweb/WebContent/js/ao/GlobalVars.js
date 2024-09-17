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

var rellist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true};
var applist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true,"Parent":true};
var complist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true,"Parent":true};
var envlist_filter_keywords = {"Environment":true,"Domain":true};
var endpointlist_filter_keywords = {"Endpoint":true,"Domain":true};
var buildenglist_filter_keywords =  {"Build Engine":true,"Domain":true};
var actionlist_filter_keywords = {"Action":true,"Domain":true};
var procedurelist_filter_keywords = {"Name":true,"Domain":true,"Type":true};
var notifierlist_filter_keywords = {"Notifier":true,"Domain":true};
var repositorylist_filter_keywords = {"Repository":true,"Domain":true};
var datasourcelist_filter_keywords = {"Data Source":true,"Domain":true};
var credentiallist_filter_keywords = {"Credential":true,"Domain":true};
var userlist_filter_keywords = {"User":true,"Domain":true};
var grouplist_filter_keywords = {"Group":true,"Domain":true};
var servercomptypelist_filter_keywords = {"Server Comp Type":true,"Domain":true};
var templatelist_filter_keywords = {"Template":true,"Notifier":true};
var buildjoblist_filter_keywords = {"Build Job":true,"Build Engine":true};

var compid2Name = {};

var notifiertype = "";
var credtype = "";
var dstype = "";
var repotype = "";
var buildenginetype = "";
var addcomptype = "";
var network;

var defect_rowcnt = 0;
var attr_rowcnt = 0;
var app4env_rowcnt = 0;
var env4app_rowcnt = 0;
var srv4env_rowcnt = 0;
var emailto_rowcnt = 0;
var groupmembership_rowcnt = 0;
var usermembership_rowcnt = 0;
var inputparms_rowcnt = 0;
var switchargs_rowcnt = 0;
var comp4srv_rowcnt = 0;
var task_rowcnt = 0;
var compdropdown = "";
var save_summ_data = "";
var msgbox_showing = false;
var isProfile = false;
var lastsel;
var lastsel_id = "";
var propslist = [];
var lastsel_rel = "";
var app_dblclick = true;
var comp_dblclick = true;
var rel_dblclick = true;
var env_dblclick = true;
var domain_dblclick = true;
var endpoint_dblclick = true;
var buildeng_dblclick = true;
var buildjob_dblclick = true;
var template_dblclick = true;
var action_dblclick = true;
var procedure_dblclick = true;
var notifier_dblclick = true;
var repository_dblclick = true;
var datasource_dblclick = true;
var credential_dblclick = true;
var user_dblclick = true;
var group_dblclick = true;
var servercomptype_dblclick = true;
var dropdownShowing = false;
var parentid = "";
var domlist = [];
var userdom = "";
var userdomid = "";
var parenttype = "";
var appcomplist_table = undefined;
var apps4complist_table = undefined;
var applist_table = undefined;
var rellist_table = undefined;
var complist_table = undefined;
var envlist_table = undefined;
var endpointlist_table = undefined;
var buildenglist_table = undefined;
var buildjoblist_table = undefined;
var templatelist_table = undefined;
var notifierlist_table = undefined;
var actionlist_table = undefined;
var procedurelist_table = undefined;
var repositorylist_table = undefined;
var datasourcelist_table = undefined;
var credentiallist_table = undefined;
var userlist_table = undefined;
var grouplist_table = undefined;
var servercomptypelist_table = undefined;
var provideslist_table = undefined;
var consumeslist_table = undefined;
var licenselist_table = undefined;
var scroecardlist_table = undefined;

var versiondd_list = [];
var scorecard_data = [];

var scorecard_desc = {};

scorecard_desc["Maintained"] = {"risk": "High (possibly unpatched vulnerabilities)", "desc": "This check determines whether the project is actively maintained. If the project is archived, it receives the lowest score. If there is at least one commit per week during the previous 90 days, the project receives the highest score. If there is activity on issues from users who are collaborators, members, or owners of the project, the project receives a partial score."};
scorecard_desc["Code Review"] = {"risk": "High (unintentional vulnerabilities or possible injection of malicious code)", "desc": "This check determines whether the project requires human code review before pull requests (merge requests) are merged."};
scorecard_desc["CII Best Practices"] = {"risk": "Low (possibly not following security best practices)", "desc": "This check determines whether the project has earned an OpenSSF (formerly CII) Best Practices Badge at the passing, silver, or gold level. The OpenSSF Best Practices badge indicates whether or not that the project uses a set of security-focused best development practices for open source software. The check uses the URL for the Git repo and the OpenSSF Best Practices badge API."};
scorecard_desc["License"] = {"risk": "Low (possible impediment to security review)", "desc": "This check tries to determine if the project has published a license. It works by using either hosting APIs or by checking standard locations for a file named according to common conventions for licenses."};
scorecard_desc["Signed Releases"] = {"risk": "High (possibility of installing malicious releases)", "desc": "This check tries to determine if the project cryptographically signs release artifacts. It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["Dangerous Workflow"] = {"risk": "Critical (vulnerable to repository compromise)", "desc": "This check determines whether the project's GitHub Action workflows has dangerous code patterns. Some examples of these patterns are untrusted code checkouts, logging github context and secrets, or use of potentially untrusted inputs in scripts."};
scorecard_desc["Packaging"] = {"risk": "Medium (users possibly missing security updates)", "desc": "This check tries to determine if the project is published as a package. It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["Token Permissions"] = {"risk": "High (vulnerable to malicious code additions)", "desc": "This check determines whether the project's automated workflows tokens follow the principle of least privilege. This is important because attackers may use a compromised token with write access to, for example, push malicious code into the project."};
scorecard_desc["Branch Protection"] = {"risk": "High (vulnerable to intentional malicious code injection)", "desc": "This check determines whether a project's default and release branches are protected with GitHub's branch protection or repository rules settings. Branch protection allows maintainers to define rules that enforce certain workflows for branches, such as requiring review or passing certain status checks before acceptance into a main branch, or preventing rewriting of public history."};
scorecard_desc["Binary Artifacts"] = {"risk": "High (non-reviewable code)", "desc": "This check determines whether the project has generated executable (binary) artifacts in the source repository."};
scorecard_desc["Pinned Dependencies"] = {"risk": "Medium (possible compromised dependencies)", "desc": "This check tries to determine if the project pins dependencies used during its build and release process. A \"pinned dependency\" is a dependency that is explicitly set to a specific hash instead of allowing a mutable version or range of versions. It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["Security Policy"] = {"risk": "Medium (possible insecure reporting of vulnerabilities)", "desc": "This check tries to determine if the project has published a security policy. It works by looking for a file named SECURITY.md (case-insensitive) in a few well-known directories."};
scorecard_desc["Fuzzing"] = {"risk": "Medium (possible vulnerabilities in code)", "desc": "This check tries to determine if the project uses fuzzing."};
scorecard_desc["SAST"] = {"risk": "Medium (possible unknown bugs)", "desc": "This check tries to determine if the project uses Static Application Security Testing (SAST), also known as static code analysis. It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["CI Tests"] = {"risk": "Low (possible unknown vulnerabilities)", "desc": "This check tries to determine if the project runs tests before pull requests are merged. It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["Vulnerabilities"] = {"risk": "High (known vulnerabilities)", "desc": "This check determines whether the project has open, unfixed vulnerabilities in its own codebase or its dependencies using the OSV (Open Source Vulnerabilities) service. An open vulnerability is readily exploited by attackers and should be fixed as soon as possible."};
scorecard_desc["Contributors"] = {"risk": "Low (lower number of trusted code reviewers)", "desc": "This check tries to determine if the project has recent contributors from multiple organizations (e.g., companies). It is currently limited to repositories hosted on GitHub, and does not support other source hosting repositories (i.e., Forges)."};
scorecard_desc["Dependency Update Tool"] = {"risk": "High (possibly vulnerable to attacks on known flaws)", "desc": "This check tries to determine if the project uses a dependency update tool."};
scorecard_desc["SBOM"] = {"risk": "Medium (possible inaccurate reporting of dependencies/vulnerabilities)", "desc": "This check tries to determine if the project maintains a Software Bill of Materials (SBOM) either as a file in the source or a release artifact."};
scorecard_desc["Webhooks"] = {"risk": "Critical (service possibly accessible to third parties)", "desc": "This check determines whether the webhook defined in the repository has a token configured to authenticate the origins of requests."};
scorecard_desc["OpenSSF ScoreCard Score"] = {"risk": "Weight-based average of the individual checks weighted by risk", "desc": "Each individual check returns a score of 0 to 10, with 10 representing the best possible score. Scorecard also produces an aggregate score, which is a weight-based average of the individual checks weighted by risk."};
scorecard_desc["OpenSSF ScoreCard Pinned to Commit"] = {"risk": "Scoring pinned to commit", "desc": "Score values are pinned to the corresponding commit, otherwise the last Scorecard run is represented."};



var defectdropdown = "";
var envdropdown = "";
var appdropdown = "";
var srvdropdown = "";
var emailtodropdown = "";
var groupmembershipdropdown = "";
var usermembershipdropdown = "";
var addParams="";
var defectdict = {};

var displaySubTabs = "N";
var NodeType = "";
var path2root = [];
var isSaas = "N";
var licType = "OSS";
var licCnt = 9999999;
var hideOSS = "N";
var lastSelectedNode = "";
var lastSelectedNode_selection = "";
var current_tree_selection = null;
var oldsubtabmenu = ["","","",""];
var oldtabmenu = "";
var oldtreename = "";
var oldmenu = "";
var currentmenu = "application_menu";
var currenttree="";
var ad_currenttree = "";
var isAdmin = "";
var newUser = "N";
var arTimerID = 0;
var depComplete = false;
var explorerFlasher = false;
var viewFlasher = false;
var dvFlashInProgress = false;
var deployFlasher = false;
var dcFlasher = false;
var flowFlasher = false;
var connFlasher = false;
var domFlasher = false;
var uagFlasher = false;
var homeFlasher = false;
var ShowingInitialHelp = false;
var HelpPageNumber=1;
var myuid = "";
var envOwner = false;emailto_rowcnt
var readonly = false;
var hasAppVersionAccess = false;
var forcepwchange = false;
var saveRes = null;
var editdom = false;
var tdedit = "";
var tdedit2 = "";
var tdedit3 = "";
var attrs_tdedit = "";
var env4app_tdedit = "";
var defect_tdedit = "";
var srv4comp_tdedit = "";
var comp4srv_tdedit = "";
var switchargs_tdedit = "";
var emailto_tdedit = "";
var task_tdedit = "";
var inputparams_tdedit = "";
var buildid = "";
var domnav_text;
var domnav_back;
var groupmembership_tdedit = "";
var usermembership_tdedit = "";
var current_dom_data = null;
var current_dom_path = null;
var displayhome = false;
var save_fulldomain_val = "";
var save_preaction_val = "";
var save_postaction_val = "";
var save_customaction_val = "";
var save_taskaction_val = "";
var save_domain_val = "";
var save_comptype_id = "";
var save_comptype_val = "";
var save_repository_val = "";
var save_repository_id = "";
var save_rf_repository_val = "";
var save_rf_repository_id = "";
var save_rb_repository_val = "";
var save_rb_repository_id = "";
var current_repository_val = "";
var current_rf_repository_val = "";
var current_rb_repository_val = "";
var save_linkedtask_val = "";
var save_owner_val = "";
var save_owner_name = "";
var save_logdlg_height = 0;
var save_logdlg_width = 0;
var save_groupowner_val = "";
var save_kind_id = "";
var save_fragment_id = "";
var repolabeladded = false;
var cat2id = [];
var save_credential_val = "";
var save_type_val = "";
var save_kind_val = "";
var save_filter_val = "";
var special_user_select = "";
var avail_user_select = "";
var avail_group_select = "";
var save_category_val = "";
var save_template_val = "";
var save_fail_template_val = "";
var save_showoutput = "";
var errmsg = "";
var myuserid = "";
var DontSave = 0;
var isRelease = "";
var usedComponents = new Array();
var usedApplicationVersions = new Array();
var saveAdminRights = new Array();
var AppsInEnvCurrentRow=0;
var AppsInEnvDeletingRow=false;
var mode = "apps";
var sessionid = "";
var loginformData = null;
var loggedin = "";
var parentwindowid = "";
var propdefs = [];

var objtype = "";
var objkind = "";
var objecttype = "";
var objname = "";
var objDomainId = 0;
var objtypeAsInt = 0;
var objectid   = 0;
var objtypeName = "";
var objName = "";

var objtype_selection = "";
var obkind_selection = "";
var objecttype_selection = "";
var objname_selection = "";
var objtypeAsInt_selection = 0;
var objectid_selection   = 0;
var objtypeName_selection = "";
var objName_selection = "";

var SetupRepoCredId = "";
var SetupRepoId = "";
var SetupServerCredId = "";
var SetupServerId = "";
var SetupEnvId = "";
var SetupAppId = "";
var SetupCompId = "";
var SetupBuildEngineId = "";
var SetupBuildJobId = "";
var SetupRepoCredName = "";
var SetupRepoName = "";
var SetupServerCredName = "";
var SetupServerName = "";
var SetupEnvName = "";
var SetupAppName = "";
var SetupCompName = "";
var SetupBuildEngineName = "";
var SetupBuildJobName = "";

var ddwinid = "";
var namecbfn   = "";
var typecbfn   = "";
var eventcbfn  = "";
var cPlumb; // set to jsplumb instance
var aPlumb; // set to jsplumb instance
var avPlumb; // set to jsplumb instance
var cvPlumb; // set to jsplumb instance
var cviPlumb; // set to jsplumb instance
var cisplumb; // set to jsplumb instance
var wfPlumb; // set to jsplumb instance
var SelectedTask;
var CurrentTask;
var category_changed = false;

var MaxWinID=0;	// For action editor
var windowid=0; // For action editor

var saveLognum = "";
var saveRes = null;
var profSaveaddParams = "";
var profSaveobjtypeAsInt = 0;
var profSaveobjid = "";
var profSaveobjtype = "";
var profSavetablename = "";
var summSavetablename = "";
var summSaveobjtypeAsInt = 0;
var summSaveobjtype = "";
var summSaveobjid = "";
var summSavedomid = "";
var summSaveobjkind = "";
var summSaveItemobjid = "";
var summSaveaddParams = "";
var summSaveSubject = "";
var summSaveBody = "";
var summSaveisLifeCycle = false;
var summAutoPing = false;
var summSaveVDS = "";
var summSaveBTDS = "";
var summProtocol = "";
var deleteoldrows = false;
var olditemvalues = [];
var isLifeCycle = false;
var gotoDate = null;
var gotoDateEnvId = null;
var start_xpos = 0;
var start_ypos = 0;
var prevcb = null;
var task_tdedit = "";

var task_summSavetablename = "";
var task_summSaveobjtypeAsInt = "";
var task_summSaveobjtype = "";
var task_summSaveobjid = "";
var task_summSaveaddParams = "";

var mouseOverActiveElement = false;
var urlstack = {};

var lookupDom = "";
var domroot;
var deployedenvs = {};

var cdc = null;

var $calendar;
var $schedule;
var StartEndTimesChanged = true;
var OverEvent=false;
var LastEvent;
var LastStartEvent;
var LastEndEvent;
var LastAllDay;
var LastEventType;
var LastCalendar;
var StartEndTimesChanged;
var CalendarStartTime="00:00";
var CalendarEndTime="23:45";
var calendar;
var currentMousePos = { x: -1, y: -1 };

var userdatefmt = "MM/dd/yyyy";		// Default
var usertimefmt = "hh:mm";			// Default


//this is the paint style for the connecting lines..
var connectorPaintStyle = {
 lineWidth : 2, // was 4
 strokeStyle : "#2e2aF8",
 joinstyle : "round",
 outlineColor : "#EAEDEF",
 outlineWidth : 3
},
// .. and this is the hover style.
connectorHoverStyle = {
 lineWidth : 2, // was 4
 strokeStyle : "#deea18"
},
// the definition of source endpoints (the small blue ones)
sourceEndpoint = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#228822",
  radius : 7
 },
 isSource : true,
 connector : [ "Flowchart", {
  stub : [ 10, 10 ], // was 40,60
  gap : 5 ,    // was 10
  cornerRadius: 10
 } ],
 connectorStyle : connectorPaintStyle,
 hoverPaintStyle : connectorHoverStyle,
 connectorHoverStyle : connectorHoverStyle,
 maxConnections : 1,  // only one exit point from each window
 dragOptions : {},
 overlays : [ [ "Label", {
  location : [ 0.5, 1.5 ],
  // label : "out",
  cssClass : "endpointSourceLabel"
 } ] ]
},
compEndpoint = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#228822",
  radius : 7
 },
 isSource : true,
 isTarget : true,
 connector : [ "Flowchart", {
  stub : [ 10, 10 ], // was 40,60
  gap : 5 ,    // was 10
  cornerRadius: 10
 } ],
 connectorStyle : connectorPaintStyle,
 hoverPaintStyle : connectorHoverStyle,
 connectorHoverStyle : connectorHoverStyle,
 maxConnections : -1,  // multiple connections allowed between components
 dragOptions : {},
 overlays : [ [ "Label", {
  location : [ 0.5, 1.5 ],
  // label : "out",
  cssClass : "endpointSourceLabel"
 } ] ]
},
appverEndpoint = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#228822",
  radius : 7
 },
 isSource : true,
 connector : [ "Flowchart", {
  stub : [ 10, 10 ], // was 40,60
  gap : 5 ,    // was 10
  cornerRadius: 10
 } ],
 connectorStyle : connectorPaintStyle,
 hoverPaintStyle : connectorHoverStyle,
 connectorHoverStyle : connectorHoverStyle,
 maxConnections : -1,  // an app version can have multiple descendents
 dragOptions : {},
 overlays : [ [ "Label", {
  location : [ 0.5, 1.5 ],
  // label : "out",
  cssClass : "endpointSourceLabel"
 } ] ]
},
sourceEndpointLeft = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#882222",
  radius : 7
 },
 isSource : true,
 connector : [ "Flowchart", {
  stub : [ 10, 10 ],
  gap : 5 ,
  cornerRadius: 10
 } ],
 connectorStyle : connectorPaintStyle,
 hoverPaintStyle : connectorHoverStyle,
 connectorHoverStyle : connectorHoverStyle,
 maxConnections : 1,  // only one exit point from each window
 dragOptions : {},
 overlays : [ [ "Label", {
  location : [ -1.0, -0.5 ],
  label : "false",
  cssClass : "endpointSourceLabel"
 } ] ]
},
sourceEndpointRight = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#228822",
  radius : 7
 },
 isSource : true,
 connector : [ "Flowchart", {
  stub : [ 10, 10 ],
  gap : 5 ,
  cornerRadius: 10
 } ],
 connectorStyle : connectorPaintStyle,
 hoverPaintStyle : connectorHoverStyle,
 connectorHoverStyle : connectorHoverStyle,
 maxConnections : 1,  // only one exit point from each window
 dragOptions : {},
 overlays : [ [ "Label", {
  location : [ +1.5, -0.5 ],
  label : "true",
  cssClass : "endpointSourceLabel"
 } ] ]
},
// a source endpoint that sits at BottomCenter
// bottomSource = jsPlumb.extend( { anchor:"BottomCenter" }, sourceEndpoint),
// the definition of target endpoints (will appear when the user drags a connection)
targetEndpoint = {
 endpoint : "Dot",
 paintStyle : {
  fillStyle : "#222288",
  radius : 7
 },
 hoverPaintStyle : connectorHoverStyle,
 maxConnections : -1,  // any number of input connections
 dropOptions : {
  hoverClass : "hover",
  activeClass : "active"
 },
 isTarget : true,
 overlays : [ [ "Label", {
  location : [ 0.5, -0.5 ],
  // label : "in",
  cssClass : "endpointTargetLabel"
 } ] ]
};


 var Fragments;
 var OverLink;
 var LastLink;
 var EditMode="N";
 var Dragging=false;
 var scrollit=false;
 var oldid;
 var newid;
 var htuAO = "gBLM5pHlNyuYUYqV6IWBmWe3wcU";
 var cCurrentMousePos = { x: -1, y: -1 };
 var ComponentCurrentRow=0;
 var CurrentlySelectedID=0;

 var spinTimer1=null;
 var spinTimer2=null;

 var onpasteventmenu = [
                       {'View this Event': {icon:"images/edit.png",onclick:function(menuItem,menu){ModifyEvent(this);} } }
                     ];

 var ondeploymenu = [
                       {'Show Deployment Log': {icon:"images/deploy.gif",onclick:function(menuItem,menu){ShowDeploy(this);} } }
                     ];

 var ontempmenu = [
                    {'Create New Event':  {icon:"images/calendar-small.png",onclick:function(menuItem,menu){ModifyEvent(this);} } }
                  ];

var blankmenu = [
                 {'Create New Event':function(menuItem,menu) { ModifyEvent(this); }} ];

var ontempReqmenu = [
        {'Request Time Slot': {icon:"images/calendar-small.png",onclick:function(menuItem,menu){ModifyEvent(this);} } }
                  ];

 var menu1=[
            {"Go to this Component":  {icon:"css/images/components_sm.png",onclick:function(menuItem,menu){cGotoComponent(this);} }},
            {"Delete this Component": {icon:"images/delete.png",onclick:function(menuItem,menu){cDeleteNode(this);} }}
         ];

 var mainmenu = [
               {'Create New Component': {icon:"css/images/components_sm.png",onclick:function(menuItem,menu){cNewComponent(this);} } }
         ];

 var menuVersions=[
            {"Go to this Application Version":  {icon:"css/images/applications_sm.png",onclick:function(menuItem,menu){aGotoApplication(this);} }},
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){aDeleteNode(this);} }}
         ];

 var menu1Workflow=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}
            ];

 var menu1WorkflowProc=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Procedure":	   {icon:"css/images/functions_procedures_sm.png",onclick:function(menuItem,menu){wfViewAction(this);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}
            ];

 var menu1WorkflowFunction=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Function":	   {icon:"css/images/functions_procedures_sm.png",onclick:function(menuItem,menu){wfViewAction(this);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}
            ];

 var menu2Workflow=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }}
            ];

 var menu2WorkflowProc=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Procedure":	   {icon:"css/images/functions_procedures_sm.png",onclick:function(menuItem,menu){wfViewAction(this);} }}
            ];

 var menu2WorkflowFunction=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Function":	   {icon:"css/images/functions_procedures_sm.png",onclick:function(menuItem,menu){wfViewAction(this);} }}
            ];

 var menuRelVersions=[
                   {"Delete this Release Version": {icon:"images/delete.png",onclick:function(menuItem,menu){aDeleteNode(this);} }}
                ];

 var mainmenuVersions = [
               {'Create New Application Version': {icon:"css/images/appversions_sm.png",onclick:function(menuItem,menu){aNewVersions(this);} } }
         ];

 var mainmenuAction = [
                         {'Create New Action': {icon:"css/images/appversions_sm.png",onclick:function(menuItem,menu){wfNewVersions(this);} } }
                   ];

 var mainmenuRelVersions = [
                         {'Create New Release Version': {icon:"css/images/appversions_sm.png",onclick:function(menuItem,menu){aNewVersions(this);} } }
                   ];

 var menuApplicationVersion=[
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){avDeleteNode(this);} }}
         ];

 var menuContextApplicationVersion=[
            {"Go to this Application Version":  {icon:"css/images/applications_sm.png",onclick:function(menuItem,menu){aGotoApplication(this);} }},
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){avDeleteNode(this);} }}
];

 var mainmenuApplicationVersion = [
               {'Create New Application Version': {icon:"css/images/appversions_sm.png",onclick:function(menuItem,menu){avNewApplicationVersion(this);} } }
         ];

 var menuComponentVersions=[
                             {"Delete this Component Version": {icon:"images/delete.png",onclick:function(menuItem,menu){cvDeleteNode(this);} }}
                          ];

var mainmenuComponentVersions = [
                                {'Create New Component Version': {icon:"css/images/components_sm.png",onclick:function(menuItem,menu){cvNewComponentVersions(this);} } }
                          ];

var menuComponentItems=[
                           {"Edit Details":    {icon:"images/view.png",onclick:function(menuItem,menu){ciOpenDetails(this);} }},
                           {"Delete this Component Item": {icon:"images/delete.png",onclick:function(menuItem,menu){ciDeleteNode(this);} }}
                        ];

var mainmenuComponentItems = [
                              {'Create New Component Item': {icon:"css/images/component_items_sm.png",onclick:function(menuItem,menu){ciNewComponentItems(this,'file');} } },
                              {'Create New Container Component Item': {icon:"css/images/component_items_sm.png",onclick:function(menuItem,menu){ciNewComponentItems(this,'docker');} } },
                              {'Create New Database Component Item': {icon:"css/images/component_items_sm.png",onclick:function(menuItem,menu){ciNewComponentItems(this,'database');} } }
                        ];

var menuReleaseVersion=[
                      {"Delete this Release Version": {icon:"images/delete.png",onclick:function(menuItem,menu){avDeleteNode(this);} }}
                   ];

var startmenu=[
               {"View Start Parameters":  {icon:"images/view.png",onclick:function(menuItem,menu){OpenStartParameters(this);} }}
            ];

var TaskList = new Array();

var obj2Int = { "en":[2,"Environment"],
  "do":[3,"Domain"],
  "se":[4,"Server"],
  "ap":[5,"Application"],
  "us":[6,"User"],
  "gr":[7,"User Group"],
  "re":[8,"Repository"],
  "no":[9,"Notify"],
  "ds":[10,"Data Source"],
  "ac":[11,"Action"],
  "co":[13,"Component"],
  "ci":[14,"Component Item"],
  "xf":[16,"Transfer"],
  "av":[17,"Application Version"],
  "ta":[18,"Task"],
  "cr":[19,"Credentials"],
  "te":[20,"Template"],
  "rl":[21,"Release"],
  "rv":[22,"Release Version"],
  "cv":[23,"Component Version"],
  "be":[24,"Build Engine"],
  "bj":[25,"Build Job"],
  "rp":[27,"Reverse Proxy"],
  "cp":[66,"Category"],
  "ct":[67,"Endpoint Type"],
  "cy":[68,"Category"],
  "cf":[69,"Category"],
  "cc":[70,"Category"],
  "pr":[90,"Procedure"],
  "fn":[91,"Function"],
  "fg":[92,"Fragment"],
  "sr":[100,"Source"],
  "cm":[101,"Commit"],
  "df":[102,"Defect"]};


var Int2obj = { 2:["en","Environment"],
  3:["do","Domain"],
  4:["se","Server"],
  5:["ap","Application"],
  6:["us","User"],
  7:["gr","User Group"],
  8:["re","Repository"],
  9:["no","Notify"],
  10:["ds","Data Source"],
  11:["ac","Action"],
  13:["co","Component"],
  14:["ci","Component Item"],
  16:["xf","Transfer"],
  17:["av","Application Version"],
  18:["ta","Task"],
  19:["cr","Credentials"],
  20:["te","Template"],
  21:["rl","Release"],
  22:["rv","Release Version"],
  23:["cv","Component Version"],
  24:["be","Build Engine"],
  25:["bj","Build Job"],
  27:["rp","Reverse Proxy"],
  66:["cp","Category"],
  67:["ct","Endpoint Type"],
  68:["cy","Category"],
  69:["cf","Category"],
  70:["cc","Category"],
  90:["pr","Procedure"],
  91:["fn","Function"],
  92:["fg","Fragment"],
  100:["sr","Source"],
  101:["cm","Commit"],
  102:["df","Defect"]};

var tree2menu = {  "#applications_tree": "application",
  "#releases_tree": "release",
"#components_tree": "component",
"#environments_tree": "environment",
"#builders_tree": "buildeng",
"#credentials_tree": "credential",
"#servers_tree": "endpoint",
"#domains_tree": "domain",
"#datasources_tree": "datasource",
"#repositories_tree": "repository",
"#notifiers_tree": "notifier",
"#actions_tree": "action",
"#notifiers_tree": "notifier",
"#users_tree": "user",
"#types_tree": "servercomptype",
"#groups_tree": "group"};

var menu2tree = {
  "setup_menu":"setup_tab",
  "domains_menu":"domains_tab",
  "application_menu":"releases_tab",
  "containers_menu":"repositories_tab",
  "endpoints_menu":"environments_tab",
  "deploy_menu":"actions_tab",
  "usersgroups_menu":"users_tab"};

var treeloaded = {  "applications_tree": false,
                    "releases_tree": false,
        "components_tree": false,
        "environments_tree": false,
        "builders_tree": false,
        "credentials_tree": false,
        "servers_tree": false,
        "domains_tree": false,
        "datasources_tree": false,
        "repositories_tree": false,
        "notifiers_tree": false,
        "actions_tree": false,
        "notifiers_tree": false,
        "users_tree": false,
        "types_tree": false,
        "groups_tree": false};

var canView = {  "#applications_tree": true,
  "#releases_tree": true,
  "#components_tree": true,
  "#environments_tree": true,
  "#builders_tree": true,
  "#credentials_tree": true,
  "#servers_tree": true,
  "#repositories_tree": true,
  "#datasources_tree": true,
  "#actions_tree": true,
  "#domains_tree": true,
  "#notifiers_tree": true,
  "#users_tree": true,
  "#types_tree": true,
  "#groups_tree": true};

var appTabs = { "#applications_tree":["#tabs-General"],
    "#components_tree":["#tabs-General"],
    "#releases_tree":["#tabs-General",
                      "#tabs-PackageApplications"],
     "#domains_tree":["#tabs-General",
          "#tabs-Tasks",
          "#tabs-Access"],
    "#environments_tree":["#tabs-General",
                          "#tabs-Calendar"],
    "#servers_tree":["#tabs-General"],
             "#credentials_tree":["#tabs-General"],
                "#actions_tree":["#tabs-General"],
                "#functions_tree":["#tabs-General"],
                "#procedures_tree":["#tabs-General"],
                "#repositories_tree":["#tabs-General"],
                "#datasources_tree":["#tabs-General"],
                "#notifiers_tree":["#tabs-General"],
                "#users_tree":["#tabs-General"],
                 "#groups_tree":["#tabs-General"],
             "#builders_tree":["#tabs-General"],
             "#types_tree":["#tabs-General"]
            };

var currentSubTabsSelection = { "#applications_tree":["tabs-General",""],
                            "#releases_tree":["tabs-General", ""],
                               "#domains_tree":["tabs-General", ""],
                               "#components_tree":["tabs-General", ""],
                            "#environments_tree":["tabs-General", ""],
                            "#endpoints_tree":["tabs-General", ""],
                            "#types_tree":["tabs-General", ""],
                                "#credentials_tree":["tabs-General", ""],
                                "#actions_tree":["tabs-General", ""],
                                "#functions_tree":["tabs-General", ""],
                                "#procedures_tree":["tabs-General", ""],
                                "#repositories_tree":["tabs-General", ""],
                                "#datasources_tree":["tabs-General", ""],
                                "#groups_tree":["tabs-General", ""],
                                "#users_tree":["tabs-General", ""],
                                "#servers_tree":["tabs-General", ""],
                                "#notifiers_tree":["tabs-General", ""],
                                "#builders_tree":["tabs-General", ""]
          };

var plot1 = null, plot2 = null, plot3 = null, plot4 = null;

var menuForType = {
  "co": {tm: "application_menu",    t: "components_tab",   image : "css/images/components_sm.png"},  // Component Base
  "cv": {tm: "application_menu",    t: "components_tab",   image : "css/images/components_sm.png"},  // Component Version
  "ap": {tm: "application_menu",    t: "applications_tab", image : "css/images/applications_sm.png"},  // Aoplication Base
  "av": {tm: "application_menu",    t: "applications_tab", image : "css/images/appversions_sm.png"},  // Application Version
  "rl": {tm: "application_menu",    t: "releases_tab",     image : "css/images/applications_sm.png"},   // Releases
  "ac": {tm: "deploy_menu",     t: "actions_tab",      image : "css/images/actions_sm.png"},   // Actions
  "no": {tm: "deploy_menu",     t: "notifiers_tab",    image : "css/images/notifiers_sm.png"},  // Notifiers
  "te": {tm: "deploy_menu",     t: "notifiers_tab",    image : "css/images/templates_sm.png"},  // Notifier Templates
  "se": {tm: "endpoints_menu",  t: "servers_tab",      image : "css/images/endpoints_sm.png"},   // Servers
  "en": {tm: "endpoints_menu",  t: "environments_tab", image : "css/images/environments_sm.png"},  // Environments
  "pr": {tm: "deploy_menu",     t: "procedures_tab",   image : "css/images/functions_procedures_sm.png"},  // Procedures
  "fn": {tm: "deploy_menu",     t: "procedures_tab",   image : "css/images/functions_procedures_sm.png"},  // Functions
  "do": {tm: "domains_menu",    t: "domains_tab",      image : "css/images/domains_sm.png"},   // Domains
  "rp": {tm: "domains_menu",    t: "domains_tab",      image : "css/images/domains_sm.png"},   // Domains
  "ds": {tm: "containers_menu", t: "datasources_tab",  image : "css/images/datasources_sm.png"},  // Datasources
  "be": {tm: "endpoints_menu",  t: "builders_tab",     image : "css/images/buildengines_sm.png"},   // Build Engines
  "bj": {tm: "endpoints_menu",  t: "builders_tab",     image : "css/images/buildjobs_sm.png"},   // Build Jobs
  "re": {tm: "containers_menu", t: "repositories_tab", image : "css/images/repositories_sm.png"},  // Repositories
  "us": {tm: "usersgroups_menu",t: "users_tab",        image : "css/images/users_sm.png"},   // Users
  "gr": {tm: "usersgroups_menu",t: "groups_tab",       image : "css/images/groups_sm.png"},   // User Group
  "cr": {tm: "containers_menu", t: "credentials_tab",  image : "css/images/credentials_sm.png"}  // Credentials
  };

var compitem_colmap =  new Map();

compitem_colmap.set('serviceownerid',["1627", false, "", "Service Owner Id"]);
compitem_colmap.set('serviceowner',["3", false, "", "Service Owner"]);
compitem_colmap.set('serviceowneremail', ["1619", false, "", "Service Owner Email"]);
compitem_colmap.set('serviceownerphone', ["1620", false, "", "Service Owner Phone"]);
compitem_colmap.set('pagerdutybusinessurl', ["1625", false, "", "PagerDuty Business Service Url"]);
compitem_colmap.set('pagerdutyurl', ["1624", false, "", "PagerDuty Service Url"]);
compitem_colmap.set('slackchannel', ["1621", false, "", "Slack Channel"]);
compitem_colmap.set('discordchannel', ["1622", false, "", "Discord Channel"]);
compitem_colmap.set('hipchatchannel', ["1623", false, "", "HipChat Channel"]);
compitem_colmap.set('builddate', ["1605", false, "", "Build Date"]);
compitem_colmap.set('buildid', ["1601", false, "", "Build Id"]);
compitem_colmap.set('buildurl', ["1602", false, "", "Build URL"]);
compitem_colmap.set('dockerrepo', ["1606", false, "", "Container Registry"]);
compitem_colmap.set('dockersha', ["1607", false, "", "Container Digest"]);
compitem_colmap.set('dockertag', ["1614", false, "", "Container Tag"]);
compitem_colmap.set('chart', ["1603", false, "", "Helm Chart"]);
compitem_colmap.set('chartnamespace', ["1613", false, "", "Helm Chart Namespace"]);
compitem_colmap.set('chartrepo', ["1615", false, "", "Helm Chart Repo"]);
compitem_colmap.set('chartrepourl', ["1616", false, "", "Helm Chart Repo Url"]);
compitem_colmap.set('chartversion', ["1612", false, "", "Helm Chart Version"]);
compitem_colmap.set('gitcommit', ["1608", false, "", "Git Commit"]);
compitem_colmap.set('gitrepo', ["1609", false, "", "Git Repo"]);
compitem_colmap.set('gittag', ["1610", false, "", "Git Tag"]);
compitem_colmap.set('giturl', ["1611", false, "", "Git URL"]);
compitem_colmap.set('kind', ["718", false,  "ik", "Kind"]);
compitem_colmap.set('operator', ["1604", false, "", "Operator"]);
compitem_colmap.set('repository', ["704", false, "re", "Repository"]);
compitem_colmap.set('rollback', ["703", false, "rr", "Roll Back"]);
compitem_colmap.set('rollup', ["702", false, "rr", "Roll Forward"]);
compitem_colmap.set('targetdirectory', ["705", false, "", "Target Directory"]);
compitem_colmap.set('xpos', ["714", false, "", "xpos"]);
compitem_colmap.set('ypos', ["715", false, "", "ypos"]);

compitem_colmap.set('purl', ["1650", false, "", "Purl"]);
compitem_colmap.set('scorecardpinned', ["1700",false, "", "OpenSSF ScoreCard Pinned to Commit"]);
compitem_colmap.set('scorecardscore', ["1701", false, "", "OpenSSF ScoreCard Score"]);
compitem_colmap.set('maintained', ["1702", false, "", "Maintained"]);
compitem_colmap.set('codereview', ["1703", false, "", "Code Review"]);
compitem_colmap.set('ciibestpractices', ["1704", false, "", "CII Best Practices"]);
compitem_colmap.set('license', ["1705", false, "", "License"]);
compitem_colmap.set('signedreleases', ["1706", false, "", "Signed Releases"]);
compitem_colmap.set('dangerousworkflow', ["1707", false, "", "Dangerous Workflow"]);
compitem_colmap.set('packaging', ["1708", false, "", "Packaging"]);
compitem_colmap.set('tokenpermissions' ["1709", false, "", "Token Permissions"]);
compitem_colmap.set('brancprotection' ["1710", false, "", "Branch Protection"]);
compitem_colmap.set('binaryartifacts', ["1711", false, "", "Binary Artifacts"]);
compitem_colmap.set('pinneddepndencies', ["1712", false, "", "Pinned Dependencies"]);
compitem_colmap.set('securitypolicy', ["1713", false, "", "Security Policy"]);
compitem_colmap.set('fuzzing', ["1714", false, "", "Fuzzing"]);
compitem_colmap.set('sast', ["1715", false, "", "SAST"]);
compitem_colmap.set('valunerabilities', ["1716", false, "", "Vulnerabilities"]);
compitem_colmap.set('citests', ["1717", false, "", "CI Tests"]);
compitem_colmap.set('contributors', ["1718", false, "", "Contributors"]);
compitem_colmap.set('dependencyupdatetooll', ["1719", false, "", "Dependency Update Tool"]);
compitem_colmap.set('sbom', ["1720", false, "", "SBOM"]);
compitem_colmap.set('webhooks', ["1721,", false, "", "Webhooks"]);

function getLocaleDateString(){

 var formats = {
    "ar-SA" : "dd/mm/yy",
    "bg-BG" : "dd.m.yyyy",
    "ca-ES" : "dd/mm/yyyy",
    "zh-TW" : "yyyy/m/d",
    "cs-CZ" : "d.m.yyyy",
    "da-DK" : "dd-mm-yyyy",
    "de-DE" : "dd.mm.yyyy",
    "el-GR" : "d/m/yyyy",
    "en-US" : "m/d/yyyy",
    "fi-FI" : "d.m.yyyy",
    "fr-FR" : "dd/mm/yyyy",
    "he-IL" : "dd/mm/yyyy",
    "hu-HU" : "yyyy. mm. dd.",
    "is-IS" : "d.m.yyyy",
    "it-IT" : "dd/mm/yyyy",
    "ja-JP" : "yyyy/mm/dd",
    "ko-KR" : "yyyy-mm-dd",
    "nl-NL" : "d-m-yyyy",
    "nb-NO" : "dd.mm.yyyy",
    "pl-PL" : "yyyy-mm-dd",
    "pt-BR" : "d/m/yyyy",
    "ro-RO" : "dd.mm.yyyy",
    "ru-RU" : "dd.mm.yyyy",
    "hr-HR" : "d.m.yyyy",
    "sk-SK" : "d. m. yyyy",
    "sq-AL" : "yyyy-mm-dd",
    "sv-SE" : "yyyy-mm-dd",
    "th-TH" : "d/m/yyyy",
    "tr-TR" : "dd.mm.yyyy",
    "ur-PK" : "dd/mm/yyyy",
    "id-ID" : "dd/mm/yyyy",
    "uk-UA" : "dd.mm.yyyy",
    "be-BY" : "dd.mm.yyyy",
    "sl-SI" : "d.m.yyyy",
    "et-EE" : "d.mm.yyyy",
    "lv-LV" : "yyyy.mm.dd.",
    "lt-LT" : "yyyy.mm.dd",
    "fa-IR" : "mm/dd/yyyy",
    "vi-VN" : "dd/mm/yyyy",
    "hy-AM" : "dd.mm.yyyy",
    "az-Latn-AZ" : "dd.mm.yyyy",
    "eu-ES" : "yyyy/mm/dd",
    "mk-MK" : "dd.mm.yyyy",
    "af-ZA" : "yyyy/mm/dd",
    "ka-GE" : "dd.mm.yyyy",
    "fo-FO" : "dd-mm-yyyy",
    "hi-IN" : "dd-mm-yyyy",
    "ms-MY" : "dd/mm/yyyy",
    "kk-KZ" : "dd.mm.yyyy",
    "ky-KG" : "dd.mm.yy",
    "sw-KE" : "m/d/yyyy",
    "uz-Latn-UZ" : "dd/mm yyyy",
    "tt-RU" : "dd.mm.yyyy",
    "pa-IN" : "dd-mm-yy",
    "gu-IN" : "dd-mm-yy",
    "ta-IN" : "dd-mm-yyyy",
    "te-IN" : "dd-mm-yy",
    "kn-IN" : "dd-mm-yy",
    "mr-IN" : "dd-mm-yyyy",
    "sa-IN" : "dd-mm-yyyy",
    "mn-MN" : "yy.mm.dd",
    "gl-ES" : "dd/mm/yy",
    "kok-IN" : "dd-mm-yyyy",
    "syr-SY" : "dd/mm/yyyy",
    "dv-MV" : "dd/mm/yy",
    "ar-IQ" : "dd/mm/yyyy",
    "zh-CN" : "yyyy/m/d",
    "de-CH" : "dd.mm.yyyy",
    "en-GB" : "dd/mm/yyyy",
    "es-MX" : "dd/mm/yyyy",
    "fr-BE" : "d/mm/yyyy",
    "it-CH" : "dd.mm.yyyy",
    "nl-BE" : "d/mm/yyyy",
    "nn-NO" : "dd.mm.yyyy",
    "pt-PT" : "dd-mm-yyyy",
    "sr-Latn-CS" : "d.m.yyyy",
    "sv-FI" : "d.m.yyyy",
    "az-Cyrl-AZ" : "dd.mm.yyyy",
    "ms-BN" : "dd/mm/yyyy",
    "uz-Cyrl-UZ" : "dd.mm.yyyy",
    "ar-EG" : "dd/mm/yyyy",
    "zh-HK" : "d/m/yyyy",
    "de-AT" : "dd.mm.yyyy",
    "en-AU" : "d/mm/yyyy",
    "es-ES" : "dd/mm/yyyy",
    "fr-CA" : "yyyy-mm-dd",
    "sr-Cyrl-CS" : "d.m.yyyy",
    "ar-LY" : "dd/mm/yyyy",
    "zh-SG" : "d/m/yyyy",
    "de-LU" : "dd.mm.yyyy",
    "en-CA" : "dd/mm/yyyy",
    "es-GT" : "dd/mm/yyyy",
    "fr-CH" : "dd.mm.yyyy",
    "ar-DZ" : "dd-mm-yyyy",
    "zh-MO" : "d/m/yyyy",
    "de-LI" : "dd.mm.yyyy",
    "en-NZ" : "d/mm/yyyy",
    "es-CR" : "dd/mm/yyyy",
    "fr-LU" : "dd/mm/yyyy",
    "ar-MA" : "dd-mm-yyyy",
    "en-IE" : "dd/mm/yyyy",
    "es-PA" : "mm/dd/yyyy",
    "fr-MC" : "dd/mm/yyyy",
    "ar-TN" : "dd-mm-yyyy",
    "en-ZA" : "yyyy/mm/dd",
    "es-DO" : "dd/mm/yyyy",
    "ar-OM" : "dd/mm/yyyy",
    "en-JM" : "dd/mm/yyyy",
    "es-VE" : "dd/mm/yyyy",
    "ar-YE" : "dd/mm/yyyy",
    "en-029" : "mm/dd/yyyy",
    "es-CO" : "dd/mm/yyyy",
    "ar-SY" : "dd/mm/yyyy",
    "en-BZ" : "dd/mm/yyyy",
    "es-PE" : "dd/mm/yyyy",
    "ar-JO" : "dd/mm/yyyy",
    "en-TT" : "dd/mm/yyyy",
    "es-AR" : "dd/mm/yyyy",
    "ar-LB" : "dd/mm/yyyy",
    "en-ZW" : "m/d/yyyy",
    "es-EC" : "dd/mm/yyyy",
    "ar-KW" : "dd/mm/yyyy",
    "en-PH" : "m/d/yyyy",
    "es-CL" : "dd-mm-yyyy",
    "ar-AE" : "dd/mm/yyyy",
    "es-UY" : "dd/mm/yyyy",
    "ar-BH" : "dd/mm/yyyy",
    "es-PY" : "dd/mm/yyyy",
    "ar-QA" : "dd/mm/yyyy",
    "es-BO" : "dd/mm/yyyy",
    "es-SV" : "dd/mm/yyyy",
    "es-HN" : "dd/mm/yyyy",
    "es-NI" : "dd/mm/yyyy",
    "es-PR" : "dd/mm/yyyy",
    "am-ET" : "d/m/yyyy",
    "tzm-Latn-DZ" : "dd-mm-yyyy",
    "iu-Latn-CA" : "d/mm/yyyy",
    "sma-NO" : "dd.mm.yyyy",
    "mn-mong-CN" : "yyyy/m/d",
    "gd-GB" : "dd/mm/yyyy",
    "en-MY" : "d/m/yyyy",
    "prs-AF" : "dd/mm/yy",
    "bn-BD" : "dd-mm-yy",
    "wo-SN" : "dd/mm/yyyy",
    "rw-RW" : "m/d/yyyy",
    "qut-GT" : "dd/mm/yyyy",
    "sah-RU" : "mm.dd.yyyy",
    "gsw-FR" : "dd/mm/yyyy",
    "co-FR" : "dd/mm/yyyy",
    "oc-FR" : "dd/mm/yyyy",
    "mi-NZ" : "dd/mm/yyyy",
    "ga-IE" : "dd/mm/yyyy",
    "se-SE" : "yyyy-mm-dd",
    "br-FR" : "dd/mm/yyyy",
    "smn-FI" : "d.m.yyyy",
    "moh-CA" : "m/d/yyyy",
    "arn-CL" : "dd-mm-yyyy",
    "ii-CN" : "yyyy/m/d",
    "dsb-DE" : "d. m. yyyy",
    "ig-NG" : "d/m/yyyy",
    "kl-GL" : "dd-mm-yyyy",
    "lb-LU" : "dd/mm/yyyy",
    "ba-RU" : "dd.mm.yy",
    "nso-ZA" : "yyyy/mm/dd",
    "quz-BO" : "dd/mm/yyyy",
    "yo-NG" : "d/m/yyyy",
    "ha-Latn-NG" : "d/m/yyyy",
    "fil-PH" : "m/d/yyyy",
    "ps-AF" : "dd/mm/yy",
    "fy-NL" : "d-m-yyyy",
    "ne-NP" : "m/d/yyyy",
    "se-NO" : "dd.mm.yyyy",
    "iu-Cans-CA" : "d/m/yyyy",
    "sr-Latn-RS" : "d.m.yyyy",
    "si-LK" : "yyyy-mm-dd",
    "sr-Cyrl-RS" : "d.m.yyyy",
    "lo-LA" : "dd/mm/yyyy",
    "km-KH" : "yyyy-mm-dd",
    "cy-GB" : "dd/mm/yyyy",
    "bo-CN" : "yyyy/m/d",
    "sms-FI" : "d.m.yyyy",
    "as-IN" : "dd-mm-yyyy",
    "ml-IN" : "dd-mm-yy",
    "en-IN" : "dd-mm-yyyy",
    "or-IN" : "dd-mm-yy",
    "bn-IN" : "dd-mm-yy",
    "tk-TM" : "dd.mm.yy",
    "bs-Latn-BA" : "d.m.yyyy",
    "mt-MT" : "dd/mm/yyyy",
    "sr-Cyrl-ME" : "d.m.yyyy",
    "se-FI" : "d.m.yyyy",
    "zu-ZA" : "yyyy/mm/dd",
    "xh-ZA" : "yyyy/mm/dd",
    "tn-ZA" : "yyyy/mm/dd",
    "hsb-DE" : "d. m. yyyy",
    "bs-Cyrl-BA" : "d.m.yyyy",
    "tg-Cyrl-TJ" : "dd.mm.yy",
    "sr-Latn-BA" : "d.m.yyyy",
    "smj-NO" : "dd.mm.yyyy",
    "rm-CH" : "dd/mm/yyyy",
    "smj-SE" : "yyyy-mm-dd",
    "quz-EC" : "dd/mm/yyyy",
    "quz-PE" : "dd/mm/yyyy",
    "hr-BA" : "d.m.yyyy.",
    "sr-Latn-ME" : "d.m.yyyy",
    "sma-SE" : "yyyy-mm-dd",
    "en-SG" : "d/m/yyyy",
    "ug-CN" : "yyyy-m-d",
    "sr-Cyrl-BA" : "d.m.yyyy",
    "es-US" : "m/d/yyyy"
 };

    // return formats[navigator.language] || 'dd/mm/yyyy';
    return userdatefmt;

 }

function getLocaleDatePickerString(){

 var formats = {
    "ar-SA" : "dd/mm/y",
    "bg-BG" : "dd.m.yy",
    "ca-ES" : "dd/mm/yy",
    "zh-TW" : "yy/m/d",
    "cs-CZ" : "d.m.yy",
    "da-DK" : "dd-mm-yy",
    "de-DE" : "dd.mm.yy",
    "el-GR" : "d/m/yy",
    "en-US" : "m/d/yy",
    "fi-FI" : "d.m.yy",
    "fr-FR" : "dd/mm/yy",
    "he-IL" : "dd/mm/yy",
    "hu-HU" : "yy. mm. dd.",
    "is-IS" : "d.m.yy",
    "it-IT" : "dd/mm/yy",
    "ja-JP" : "yy/mm/dd",
    "ko-KR" : "yy-mm-dd",
    "nl-NL" : "d-m-yy",
    "nb-NO" : "dd.mm.yy",
    "pl-PL" : "yy-mm-dd",
    "pt-BR" : "d/m/yy",
    "ro-RO" : "dd.mm.yy",
    "ru-RU" : "dd.mm.yy",
    "hr-HR" : "d.m.yy",
    "sk-SK" : "d. m. yy",
    "sq-AL" : "yy-mm-dd",
    "sv-SE" : "yy-mm-dd",
    "th-TH" : "d/m/yy",
    "tr-TR" : "dd.mm.yy",
    "ur-PK" : "dd/mm/yy",
    "id-ID" : "dd/mm/yy",
    "uk-UA" : "dd.mm.yy",
    "be-BY" : "dd.mm.yy",
    "sl-SI" : "d.m.yy",
    "et-EE" : "d.mm.yy",
    "lv-LV" : "yy.mm.dd.",
    "lt-LT" : "yy.mm.dd",
    "fa-IR" : "mm/dd/yy",
    "vi-VN" : "dd/mm/yy",
    "hy-AM" : "dd.mm.yy",
    "az-Latn-AZ" : "dd.mm.yy",
    "eu-ES" : "yy/mm/dd",
    "mk-MK" : "dd.mm.yy",
    "af-ZA" : "yy/mm/dd",
    "ka-GE" : "dd.mm.yy",
    "fo-FO" : "dd-mm-yy",
    "hi-IN" : "dd-mm-yy",
    "ms-MY" : "dd/mm/yy",
    "kk-KZ" : "dd.mm.yy",
    "ky-KG" : "dd.mm.y",
    "sw-KE" : "m/d/yy",
    "uz-Latn-UZ" : "dd/mm yy",
    "tt-RU" : "dd.mm.yy",
    "pa-IN" : "dd-mm-y",
    "gu-IN" : "dd-mm-y",
    "ta-IN" : "dd-mm-yy",
    "te-IN" : "dd-mm-y",
    "kn-IN" : "dd-mm-y",
    "mr-IN" : "dd-mm-yy",
    "sa-IN" : "dd-mm-yy",
    "mn-MN" : "y.mm.dd",
    "gl-ES" : "dd/mm/y",
    "kok-IN" : "dd-mm-yy",
    "syr-SY" : "dd/mm/yy",
    "dv-MV" : "dd/mm/y",
    "ar-IQ" : "dd/mm/yy",
    "zh-CN" : "yy/m/d",
    "de-CH" : "dd.mm.yy",
    "en-GB" : "dd/mm/yy",
    "es-MX" : "dd/mm/yy",
    "fr-BE" : "d/mm/yy",
    "it-CH" : "dd.mm.yy",
    "nl-BE" : "d/mm/yy",
    "nn-NO" : "dd.mm.yy",
    "pt-PT" : "dd-mm-yy",
    "sr-Latn-CS" : "d.m.yy",
    "sv-FI" : "d.m.yy",
    "az-Cyrl-AZ" : "dd.mm.yy",
    "ms-BN" : "dd/mm/yy",
    "uz-Cyrl-UZ" : "dd.mm.yy",
    "ar-EG" : "dd/mm/yy",
    "zh-HK" : "d/m/yy",
    "de-AT" : "dd.mm.yy",
    "en-AU" : "d/mm/yy",
    "es-ES" : "dd/mm/yy",
    "fr-CA" : "yy-mm-dd",
    "sr-Cyrl-CS" : "d.m.yy",
    "ar-LY" : "dd/mm/yy",
    "zh-SG" : "d/m/yy",
    "de-LU" : "dd.mm.yy",
    "en-CA" : "dd/mm/yy",
    "es-GT" : "dd/mm/yy",
    "fr-CH" : "dd.mm.yy",
    "ar-DZ" : "dd-mm-yy",
    "zh-MO" : "d/m/yy",
    "de-LI" : "dd.mm.yy",
    "en-NZ" : "d/mm/yy",
    "es-CR" : "dd/mm/yy",
    "fr-LU" : "dd/mm/yy",
    "ar-MA" : "dd-mm-yy",
    "en-IE" : "dd/mm/yy",
    "es-PA" : "mm/dd/yy",
    "fr-MC" : "dd/mm/yy",
    "ar-TN" : "dd-mm-yy",
    "en-ZA" : "yy/mm/dd",
    "es-DO" : "dd/mm/yy",
    "ar-OM" : "dd/mm/yy",
    "en-JM" : "dd/mm/yy",
    "es-VE" : "dd/mm/yy",
    "ar-YE" : "dd/mm/yy",
    "en-029" : "mm/dd/yy",
    "es-CO" : "dd/mm/yy",
    "ar-SY" : "dd/mm/yy",
    "en-BZ" : "dd/mm/yy",
    "es-PE" : "dd/mm/yy",
    "ar-JO" : "dd/mm/yy",
    "en-TT" : "dd/mm/yy",
    "es-AR" : "dd/mm/yy",
    "ar-LB" : "dd/mm/yy",
    "en-ZW" : "m/d/yy",
    "es-EC" : "dd/mm/yy",
    "ar-KW" : "dd/mm/yy",
    "en-PH" : "m/d/yy",
    "es-CL" : "dd-mm-yy",
    "ar-AE" : "dd/mm/yy",
    "es-UY" : "dd/mm/yy",
    "ar-BH" : "dd/mm/yy",
    "es-PY" : "dd/mm/yy",
    "ar-QA" : "dd/mm/yy",
    "es-BO" : "dd/mm/yy",
    "es-SV" : "dd/mm/yy",
    "es-HN" : "dd/mm/yy",
    "es-NI" : "dd/mm/yy",
    "es-PR" : "dd/mm/yy",
    "am-ET" : "d/m/yy",
    "tzm-Latn-DZ" : "dd-mm-yy",
    "iu-Latn-CA" : "d/mm/yy",
    "sma-NO" : "dd.mm.yy",
    "mn-Mong-CN" : "yy/m/d",
    "gd-GB" : "dd/mm/yy",
    "en-MY" : "d/m/yy",
    "prs-AF" : "dd/mm/y",
    "bn-BD" : "dd-mm-y",
    "wo-SN" : "dd/mm/yy",
    "rw-RW" : "m/d/yy",
    "qut-GT" : "dd/mm/yy",
    "sah-RU" : "mm.dd.yy",
    "gsw-FR" : "dd/mm/yy",
    "co-FR" : "dd/mm/yy",
    "oc-FR" : "dd/mm/yy",
    "mi-NZ" : "dd/mm/yy",
    "ga-IE" : "dd/mm/yy",
    "se-SE" : "yy-mm-dd",
    "br-FR" : "dd/mm/yy",
    "smn-FI" : "d.m.yy",
    "moh-CA" : "m/d/yy",
    "arn-CL" : "dd-mm-yy",
    "ii-CN" : "yy/m/d",
    "dsb-DE" : "d. m. yy",
    "ig-NG" : "d/m/yy",
    "kl-GL" : "dd-mm-yy",
    "lb-LU" : "dd/mm/yy",
    "ba-RU" : "dd.mm.y",
    "nso-ZA" : "yy/mm/dd",
    "quz-BO" : "dd/mm/yy",
    "yo-NG" : "d/m/yy",
    "ha-Latn-NG" : "d/m/yy",
    "fil-PH" : "m/d/yy",
    "ps-AF" : "dd/mm/y",
    "fy-NL" : "d-m-yy",
    "ne-NP" : "m/d/yy",
    "se-NO" : "dd.mm.yy",
    "iu-Cans-CA" : "d/m/yy",
    "sr-Latn-RS" : "d.m.yy",
    "si-LK" : "yy-mm-dd",
    "sr-Cyrl-RS" : "d.m.yy",
    "lo-LA" : "dd/mm/yy",
    "km-KH" : "yy-mm-dd",
    "cy-GB" : "dd/mm/yy",
    "bo-CN" : "yy/m/d",
    "sms-FI" : "d.m.yy",
    "as-IN" : "dd-mm-yy",
    "ml-IN" : "dd-mm-y",
    "en-IN" : "dd-mm-yy",
    "or-IN" : "dd-mm-y",
    "bn-IN" : "dd-mm-y",
    "tk-TM" : "dd.mm.y",
    "bs-Latn-BA" : "d.m.yy",
    "mt-MT" : "dd/mm/yy",
    "sr-Cyrl-ME" : "d.m.yy",
    "se-FI" : "d.m.yy",
    "zu-ZA" : "yy/mm/dd",
    "xh-ZA" : "yy/mm/dd",
    "tn-ZA" : "yy/mm/dd",
    "hsb-DE" : "d. m. yy",
    "bs-Cyrl-BA" : "d.m.yy",
    "tg-Cyrl-TJ" : "dd.mm.y",
    "sr-Latn-BA" : "d.m.yy",
    "smj-NO" : "dd.mm.yy",
    "rm-CH" : "dd/mm/yy",
    "smj-SE" : "yy-mm-dd",
    "quz-EC" : "dd/mm/yy",
    "quz-PE" : "dd/mm/yy",
    "hr-BA" : "d.m.yy.",
    "sr-Latn-ME" : "d.m.yy",
    "sma-SE" : "yy-mm-dd",
    "en-SG" : "d/m/yy",
    "ug-CN" : "yy-m-d",
    "sr-Cyrl-BA" : "d.m.yy",
    "es-US" : "m/d/yy"
 };

    return formats[navigator.language] || 'dd/MM/yy';

 	// return userdatefmt;
 }
