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
var lastSelectedNode = "";
var lastSelectedNode_selection = "";
var current_tree_selection = null;
var oldsubtabmenu = ["","","",""];
var oldtabmenu = "";
var oldtreename = "";
var oldmenu = "";
var currentmenu = "release_menu";
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
var breadcrumbFlasher = false;
var ShowingInitialHelp = false;
var HelpPageNumber=1;
var myuid = "";
var envOwner = false;
var readonly = false;
var hasAppVersionAccess = false;
var forcepwchange = false;
var saveRes = null;
var tdedit = "";
var tdedit2 = "";
var displayhome = false;
var save_preaction_val = "";
var save_postaction_val = "";
var save_customaction_val = "";
var save_taskaction_val = "";
var save_domain_val = "";
var save_comptype_id = "";
var save_comptype_val = "";
var save_repository_val = "";
var save_repository_id = "";
var current_repository_val = "";
var save_linkedtask_val = "";
var save_owner_val = "";
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
            {"Go to this Component":  {icon:"css/images/component_16x.png",onclick:function(menuItem,menu){cGotoComponent(this);} }},
            {"Delete this Component": {icon:"images/delete.png",onclick:function(menuItem,menu){cDeleteNode(this);} }}
         ];
 
 var mainmenu = [
               {'Create New Component': {icon:"css/images/component_16x.png",onclick:function(menuItem,menu){cNewComponent(this);} } }
         ];       
 
 var menuVersions=[
            {"Go to this Application Version":  {icon:"css/images/application_16x.png",onclick:function(menuItem,menu){aGotoApplication(this);} }},
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){aDeleteNode(this);} }}
         ];

 var menu1Workflow=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}
            ];
 
 var menu1WorkflowProc=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Procedure":	   {icon:"css/images/proc.png",onclick:function(menuItem,menu){wfViewAction(this);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}        
            ];
 
 var menu1WorkflowFunction=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Function":	   {icon:"css/images/func.png",onclick:function(menuItem,menu){wfViewAction(this);} }},
            {"Delete this Activity":   {icon:"images/delete.png",onclick:function(menuItem,menu){wfDeleteNode(this);} }}              
            ];
 
 var menu2Workflow=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }}
            ];
         
 var menu2WorkflowProc=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Procedure":	   {icon:"css/images/proc.png",onclick:function(menuItem,menu){wfViewAction(this);} }}   
            ];
         
 var menu2WorkflowFunction=[
            {"View Details":           {icon:"images/view.png",onclick:function(menuItem,menu){wfOpenDetails(this,false);} }},
            {"Goto this Function":	   {icon:"css/images/func.png",onclick:function(menuItem,menu){wfViewAction(this);} }}        
            ];

 var menuRelVersions=[
                   {"Delete this Release Version": {icon:"images/delete.png",onclick:function(menuItem,menu){aDeleteNode(this);} }}
                ];
 
 var mainmenuVersions = [
               {'Create New Application Version': {icon:"css/images/appversion_16x.png",onclick:function(menuItem,menu){aNewVersions(this);} } }
         ];
 
 var mainmenuAction = [
                         {'Create New Action': {icon:"css/images/appversion_16x.png",onclick:function(menuItem,menu){wfNewVersions(this);} } }
                   ];
           
 var mainmenuRelVersions = [
                         {'Create New Release Version': {icon:"css/images/appversion_16x.png",onclick:function(menuItem,menu){aNewVersions(this);} } }
                   ]; 
 
 var menuApplicationVersion=[
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){avDeleteNode(this);} }}
         ];
 
 var menuContextApplicationVersion=[
            {"Go to this Application Version":  {icon:"css/images/application_16x.png",onclick:function(menuItem,menu){aGotoApplication(this);} }},
            {"Delete this Application Version": {icon:"images/delete.png",onclick:function(menuItem,menu){avDeleteNode(this);} }}
];
 
 var mainmenuApplicationVersion = [
               {'Create New Application Version': {icon:"css/images/appversion_16x.png",onclick:function(menuItem,menu){avNewApplicationVersion(this);} } }
         ]; 

 var menuComponentVersions=[
                             {"Delete this Component Version": {icon:"images/delete.png",onclick:function(menuItem,menu){cvDeleteNode(this);} }}
                          ];
                  
var mainmenuComponentVersions = [
                                {'Create New Component Version': {icon:"css/images/component_ver_16x.png",onclick:function(menuItem,menu){cvNewComponentVersions(this);} } }
                          ]; 

var menuComponentItems=[
                           {"Edit Details":    {icon:"images/view.png",onclick:function(menuItem,menu){ciOpenDetails(this);} }},
                           {"Delete this Component Item": {icon:"images/delete.png",onclick:function(menuItem,menu){ciDeleteNode(this);} }}
                        ];
                
var mainmenuComponentItems = [
                              {'Create New Component Item': {icon:"css/images/component_item_16x.png",onclick:function(menuItem,menu){ciNewComponentItems(this);} } }
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
  "cp":[66,"Category"],
  "ct":[67,"Component Type"],
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
  66:["cp","Category"],  
  67:["ct","Component Type"],
  68:["cy","Category"],  
  69:["cf","Category"], 
  70:["cc","Category"],  
  90:["pr","Procedure"],
  91:["fn","Function"],
  92:["fg","Fragment"],
  100:["sr","Source"],
  101:["cm","Commit"],
  102:["df","Defect"]};
  
var menu2tree = {
  "setup_menu":"setup_tab",
  "domains_menu":"domains_tab",
  "release_menu":"applications_tab",
  "containers_menu":"repositories_tab",  
  "endpoints_menu":"environments_tab", 
  "deploy_menu":"actions_tab",
  "usersgroups_menu":"users_tab"};

var treeloaded = {  "applications_tree": false,
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
    
var appTabs = { "#applications_tree":["#tabs-General",
            "#tabs-Components",
           "#tabs-Attributes",
            "#tabs-Access",
           "#tabs-History",
           "#tabs-Versions",
           "#tabs-Environments",
           "#tabs-AppDefects",
           "#tabs-Reports"],
     "#domains_tree":["#tabs-General",
          "#tabs-Tasks",
          "#tabs-Access"],
     "#types_tree":["#tabs-General",
                    "#tabs-Access"],          
    "#components_tree":["#tabs-General",
         "#tabs-ComponentItems",
            "#tabs-ComponentVersions",
            "#tabs-History",
            "#tabs-Access", 
         "#tabs-Attributes",
            "#tabs-CompApplications",
            "#tabs-CompDefects",
         "#tabs-CompServers"],
    "#environments_tree":["#tabs-General",
                          "#tabs-Calendar", 
                          "#tabs-Access", 
                          "#tabs-Attributes",
           "#tabs-Applications",
           "#tabs-EnvDefects",
              "#tabs-Servers",
           "#tabs-History",
           "#tabs-Reports"],
    "#endpoints_tree":["#tabs-General",
                     "#tabs-Properties",
                  "#tabs-Attributes",
                  "#tabs-History",
                  "#tabs-Access"],
             "#credentials_tree":["#tabs-General",
                         "#tabs-Access"],
                "#actions_tree":["#tabs-General",
                             "#tabs-Workflow", 
                             "#tabs-History",
                              "#tabs-Access"],
                "#functions_tree":["#tabs-General",
                                 "#tabs-Args", 
                                 "#tabs-ProcBody",
                                 "#tabs-History",
                                 "#tabs-Access"],
                "#procedures_tree":["#tabs-General",
                                    "#tabs-Args",  
                                    "#tabs-ProcBody",
                                    "#tabs-History",
                                     "#tabs-Access"],                                                
                "#repositories_tree":["#tabs-General",
                             "#tabs-Properties",
                             "#tabs-Patterns",
                             "#tabs-Attributes",
                             "#tabs-Access"], 
                "#datasources_tree":["#tabs-General",
                                    "#tabs-Properties",
                                    "#tabs-Access"],                              
                "#notifiers_tree":["#tabs-General",
                                 "#tabs-Properties",
                                 "#tabs-Recipients",
                                 "#tabs-Body",
                                 "#tabs-Access"],
                "#users_tree":["#tabs-General",
                               "#tabs-GroupMembership",
                               "#tabs-History"],
                 "#groups_tree":["#tabs-General",
                                 "#tabs-History",
                                 "#tabs-UserMembership",
                                 "#tabs-AdminRights"],
             "#servers_tree":["#tabs-General",
                              "#tabs-History",
                              "#tabs-Reports",
                              "#tabs-ServerStatus",
                              "#tabs-Attributes",
                              "#tabs-SrvComponents",
                              "#tabs-Access"],
             "#builders_tree":["#tabs-General",
                               "#tabs-Builds",
                               "#tabs-Properties",
                               "#tabs-Access"]
            };  
 
var currentSubTabsSelection = { "#applications_tree":["tabs-Components",""],      
                               "#domains_tree":["tabs-Tasks", ""],
                               "#components_tree":["tabs-ComponentItems", ""],
                            "#environments_tree":["tabs-Calendar", ""],
                            "#endpoints_tree":["tabs-General", ""],
                            "#types_tree":["tabs-General", ""],
                                "#credentials_tree":["tabs-Access", ""],
                                "#actions_tree":["tabs-Workflow", ""],
                                "#functions_tree":["tabs-Args", ""],
                                "#procedures_tree":["tabs-Args", ""],
                                "#repositories_tree":["tabs-Properties", ""], 
                                "#datasources_tree":["tabs-Properties", ""],
                                "#groups_tree":["tabs-UserMembership", ""], 
                                "#users_tree":["tabs-GroupMembership", ""], 
                                "#servers_tree":["tabs-SrvComponents", ""],
                                "#notifiers_tree":["tabs-Properties", ""],
                                "#builders_tree":["tabs-General", ""]
          };  
          
var plot1 = null, plot2 = null, plot3 = null, plot4 = null;  

var menuForType = {
	"co":	{tm: "release_menu",    t: "components_tab"},		// Component Base
	"cv":	{tm: "release_menu",    t: "components_tab"},		// Component Version
	"ap":	{tm: "release_menu",    t: "applications_tab"},		// Aoplication Base
	"av":	{tm: "release_menu",    t: "applications_tab"},		// Application Version
	"ac":	{tm: "deploy_menu",     t: "actions_tab"},			// Actions
	"no":	{tm: "deploy_menu",     t: "notifiers_tab"},		// Notifiers
	"te":	{tm: "deploy_menu",     t: "notifiers_tab"},		// Notifier Templates
	"se":	{tm: "endpoints_menu",  t: "servers_tab"},			// Servers
	"en":	{tm: "endpoints_menu",  t: "environments_tab"},		// Environments
	"pr":	{tm: "deploy_menu",	    t: "procedures_tab"},		// Procedures
	"fn":	{tm: "deploy_menu",	    t: "procedures_tab"},		// Functions
	"do":	{tm: "domains_menu",    t: "domains_tab"},			// Domains 
	"ds":	{tm: "containers_menu", t: "datasources_tab"},		// Datasources
	"be":	{tm: "endpoints_menu",  t: "builders_tab"},			// Build Engines
	"bj":	{tm: "endpoints_menu",  t: "builders_tab"},			// Build Jobs
	"re":	{tm: "containers_menu", t: "repositories_tab"},		// Repositories
	"us":	{tm: "usersgroups_menu",t: "users_tab"},			// Users
	"gr":	{tm: "usersgroups_menu",t: "groups_tab"},			// User Group
	"cr":	{tm: "containers_menu",	t: "credentials_tab"}		// Credentials
};

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
