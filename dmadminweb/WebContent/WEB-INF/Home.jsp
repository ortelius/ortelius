 <%--
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
 --%>
<!doctype html>
<%@page contentType="text/html" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
<head>
<title>Ortelius</title>
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />

<link rel="icon" href="favicon.ico">
<link rel='stylesheet' type="text/css" href='https://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,700italic,800italic,600,400,700,800'>
<link rel="stylesheet" type="text/css" href="css/cupertino/jquery-ui-min.css" />
<link rel="stylesheet" type="text/css" href="css/jquery.dmcontextmenu.css" />
<link rel="stylesheet" type="text/css" href="css/jquery.jqplot.css" />
<link rel="stylesheet" type="text/css" href="css/calendar.css" />
<link rel="stylesheet" type="text/css" href="css/calendar-print.css" media="print" />
<link rel="stylesheet" type="text/css" href="css/vis.min.css" />
<link rel="stylesheet" type="text/css" href="css/visualsearch-datauri.css" />
<link rel="stylesheet" href="https://cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css" type="text/css" />
<link rel="stylesheet" href="https://cdn.datatables.net/buttons/1.5.6/css/buttons.dataTables.min.css" />
<link rel="stylesheet" href="https://cdn.datatables.net/responsive/2.2.3/css/responsive.dataTables.min.css" />
<link rel="stylesheet" href="https://cdn.datatables.net/select/1.3.0/css/select.dataTables.min.css" />
<link rel="stylesheet" href="https://cdn.datatables.net/rowgroup/1.1.3/css/rowGroup.dataTables.min.css" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/jquery-confirm/3.3.2/jquery-confirm.min.css">
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.6.0/swagger-ui.css">
<link rel="stylesheet" type="text/css" href="css/devops_frydesign.css" />

<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery-ui.js"></script>
<script type="text/javascript" src="jquery.jstree.js"></script>
<script type="text/javascript" src="js/jquery.cookie.js"></script>
<script type="text/javascript" src="js/jquery.ui.widget.js"></script>
<script type="text/javascript" src="js/underscore-1.5.2.js"></script>
<script type="text/javascript" src="js/backbone-1.1.0.js"></script>
<script type="text/javascript" src="js/visualsearch.js"></script>
<script type="text/javascript" src="js/cookies.js"></script>
<script type="text/javascript" src="js/jquery.blockUI.js"></script>
<script type="text/javascript" src="js/jquery.sessionTimeout.js"></script>
<script type="text/javascript" src="js/jsPlumb-util.js"></script>
<script type="text/javascript" src="js/jsPlumb-dom-adapter.js"></script>
<script type="text/javascript" src="js/jsPlumb-drag.js"></script>
<script type="text/javascript" src="js/jsPlumb.js"></script>
<script type="text/javascript" src="js/jsPlumb-endpoint.js"></script>
<script type="text/javascript" src="js/jsPlumb-connection.js"></script>
<script type="text/javascript" src="js/jsPlumb-anchors.js"></script>
<script type="text/javascript" src="js/jsPlumb-defaults.js"></script>
<script type="text/javascript" src="js/jsPlumb-connectors-statemachine.js"></script>
<script type="text/javascript" src="js/jsPlumb-connectors-flowchart.js"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-svg.js"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-canvas.js"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-vml.js"></script>
<script type="text/javascript" src="js/jquery.jsPlumb.js"></script>
<script type="text/javascript" src="js/selectenter.js"></script>
<script type="text/javascript" src="js/jquery.jqplot.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.barRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.pieRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasTextRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasAxisLabelRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasAxisTickRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasOverlay.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.categoryAxisRenderer.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.highlighter.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.cursor.min.js"></script>
<script type="text/javascript" src="js/plugins/jqplot.json2.min.js"></script>
<script src="https://kit.fontawesome.com/ba98665da8.js" crossorigin="anonymous"></script>

<script type="text/javascript" src="js/fullcalendar.min.js"></script>
<script type="text/javascript" src="js/date.format.js"></script>
<script type="text/javascript" src="js/jquery.json-2.4.min.js"></script>
<script type="text/javascript" src="js/globalize.js"></script>
<script type="text/javascript" src="js/jquery-ui-timepicker-addon.js"></script>
<script type="text/javascript" src='js/cultures/globalize.culture.de-DE.js'></script>
<script type="text/javascript" src='js/cultures/globalize.culture.en-GB.js'></script>
<script type="text/javascript" src='js/cultures/globalize.culture.en-US.js'></script>
<script type="text/javascript" src="js/vis.min.js"></script>
<script type="text/javascript" src="js/oauth.min.js"></script>
<script type="text/javascript" src="js/jquery.form.js"></script>
<script type="text/javascript" src="js/jquery.fileDownload.js"></script>
<script type="text/javascript" src="js/edit_area/edit_area_full.js"></script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.6.0/swagger-ui-bundle.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/3.6.0/swagger-ui-standalone-preset.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/showdown/1.9.1/showdown.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/dataTables.buttons.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.flash.min.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.1.3/jszip.min.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/pdfmake.min.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/vfs_fonts.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.html5.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.print.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/responsive/2.2.3/js/dataTables.responsive.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/plug-ins/1.10.19/features/pageResize/dataTables.pageResize.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/select/1.3.0/js/dataTables.select.min.js"></script>
<script type="text/javascript" src="https://cdn.datatables.net/rowgroup/1.1.3/js/dataTables.rowGroup.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-confirm/3.3.2/jquery-confirm.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/underscore.js/1.9.1/underscore-min.js"></script>
<script src="https://d3js.org/d3.v5.min.js"></script>

<script type="text/javascript" src="js/ao/prototypes.js"></script>
<script type="text/javascript" src="js/ao/ao_list.js"></script>
<script type="text/javascript" src="js/ao/createtree.js"></script>
<script type="text/javascript" src="js/ao/createtree_selection.js"></script>
<script type="text/javascript" src="js/ao/GlobalVars.js"></script>
<script type="text/javascript" src="js/ao/Setup.js"></script>
<script type="text/javascript" src="js/ao/GetSummary.js"></script>
<script type="text/javascript" src="js/ao/GetAttributes.js"></script>
<script type="text/javascript" src="js/ao/GetPrerequisities.js"></script>
<script type="text/javascript" src="js/ao/GetComponents.js"></script>
<script type="text/javascript" src="js/ao/GetComponentItems.js"></script>
<script type="text/javascript" src="js/ao/GetVersionsOfApp.js"></script>
<script type="text/javascript" src="js/ao/GetVersionsOfComp.js"></script>
<script type="text/javascript" src="js/ao/GetApplicationVersions.js"></script>
<script type="text/javascript" src="js/ao/newsfeed.js"></script>
<script type="text/javascript" src="js/ao/GetReports.js"></script>
<script type="text/javascript" src="js/ao/GetDeployReports.js"></script>
<script type="text/javascript" src="js/ao/GetServers4Env.js"></script>
<script type="text/javascript" src="js/ao/GetServers4Comp.js"></script>
<script type="text/javascript" src="js/ao/GetComponents4Servers.js"></script>
<script type="text/javascript" src="js/ao/GetApplications4Comp.js"></script>
<script type="text/javascript" src="js/ao/GetApps4Env.js"></script>
<script type="text/javascript" src="js/ao/GetEnv4Apps.js"></script>
<script type="text/javascript" src="js/ao/About.js"></script>
<script type="text/javascript" src="js/ao/Home.js"></script>
<script type="text/javascript" src="js/ao/GetGroupMembership.js"></script>
<script type="text/javascript" src="js/ao/GetUserMembership.js"></script>
<script type="text/javascript" src="js/ao/GetAdminRights.js"></script>
<script type="text/javascript" src="js/ao/GetArgs.js"></script>
<script type="text/javascript" src="js/ao/GetWorkflow.js"></script>
<script type="text/javascript" src="js/ao/GetProperties.js"></script>
<script type="text/javascript" src="js/ao/GetNewsDetails.js"></script>
<script type="text/javascript" src="js/ao/GetEmailTo.js"></script>
<script type="text/javascript" src="js/ao/jquery.dmcontextmenu.js"></script>
<script type="text/javascript" src="js/ao/TestServer.js"></script>
<script type="text/javascript" src="js/ao/GetTemplateBody.js"></script>
<script type="text/javascript" src="js/ao/DomNav.js"></script>
<script type="text/javascript" src="js/ao/ao_hook.js"></script>
<script type="text/javascript" src="js/ao/ao_main.js"></script>

</head>
<body>
<div id="container">

 <div id="header">
  <div class="omlogo"><img alt="Ortelius" src="images/logo.png" style="margin-top:4px"></div>
   <div id="positiondevopsmenu">
	<div class="moduletable_menu">
      <ul class="menu_menu">
      </ul>
	</div>
   </div>

   <div id="positiondevopsmenu_right">
    <div class="moduletable_menu">
      <ul class="menu_menu">
        <li id="rproxy_menu" onclick="ShowHome(true,false);">Warning: 0 of 1 Reverse Proxy Online !</li>
        <li id="about_menu" onclick="ShowAbout()"><i class="fal fa-info-circle fa-2x" aria-hidden="true"  style="padding-right:5px"></i></li>
        <li id="user_menu" onclick="LoadUserProfile()" ><i class="fal fa-user fa-2x" aria-hidden="true"  style="padding-right:5px"></i></li>
        <li id="help_menu"><a class="menu_icon" href="https://docs.deployhub.com" target="_blank" ><i class="fal fa-question-circle fa-2x" aria-hidden="true"  style="padding-right:5px"></i></a></li>
        <li id="logout_menu"><a class="menu_icon" href="Logout" ><i class="fal fa-sign-out fa-2x" aria-hidden="true"  style="padding-right:5px"></i></a></li>
      </ul>
	 </div>
    </div>

	<div class="clear"></div>
	</div> <!-- end header -->
	<div id="panel_container_menu" class="left" style="display:none;">
	 <div id="verttabs" class="verttab">
	   <button id="verttab_application" class="tablinks verttab_app" onclick="openList(event, 'application')">Applications</button>
       <button id="verttab_component" class="tablinks verttab_comp" onclick="openList(event, 'component')">Components</button>
       <button id="verttab_domain" class="tablinks verttab_domain" onclick="LoadDomNav()">Domains</button>
       <button id="verttab_environment" class="tablinks verttab_env" onclick="openList(event, 'environment')">Environments</button>
       <button id="verttab_endpoint" class="tablinks verttab_endpoint" onclick="openList(event, 'endpoint')">Endpoints</button>
       <button id="verttab_action" class="tablinks verttab_action" onclick="openList(event, 'action')">Actions</button>
       <button id="verttab_procedure" class="tablinks verttab_procedure" onclick="openList(event, 'procedure')">Func/Procs</button>
       <button id="verttab_servercomptype" class="tablinks verttab_servercomptype" onclick="openList(event, 'servercomptype')">Customize Types</button>
       <button id="verttab_setup" class="tablinks verttab_setup" onclick="toggleSetup(event, 'setup')">Setup<i class="fad fa-chevron-double-down fa-lg" aria-hidden="true" style="padding-left:50%;"></i></button>
       <button id="verttab_credential" class="tablinks verttab_credential" onclick="openList(event, 'credential')">Credentials</button>
       <button id="verttab_repository" class="tablinks verttab_repository" onclick="openList(event, 'repository')">Repositories</button>
       <button id="verttab_datasource" class="tablinks verttab_datasource" onclick="openList(event, 'datasource')">Data Sources</button>
       <button id="verttab_notifier" class="tablinks verttab_notifier" onclick="openList(event, 'notifier')">Notifiers</button>
       <button id="verttab_template" class="tablinks verttab_template" onclick="openList(event, 'template')">Notifier Templates</button>
       <button id="verttab_user" class="tablinks verttab_user" onclick="openList(event, 'user')">Users</button>
       <button id="verttab_group" class="tablinks verttab_group" onclick="openList(event, 'group')">Groups</button>
     </div>
	</div>
	<div id="panel_container_right_list" class="right" style="display:none;">
     <div id="applist_pane">
        <div id="applist_pane_title" style="display:inline-block;"><H2>Applications</H2></div>
          <div id="applist_buttons" style="display:inline-block;">
         	 <button class="title_buttons" onclick="openList(event, 'application')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'application')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Base</button>
             <button class="title_buttons" onclick="addRow(event, 'appversion')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Version</button>
             <button class="title_buttons" onclick="delRow(event, 'application')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <div class="dropdown_menu">
               <button class="title_buttons taskMenuButton" onclick="taskMenu()"><i class="fal fa-tasks" aria-hidden="true" style="padding-right:5px;"></i>Tasks<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content">
               </div>
             </div>
             <button class="title_buttons" onclick="openList(event, 'application')"><i class="fal fa-list" aria-hidden="true" style="padding-right:5px;"></i>List</button>
          	 <button class="title_buttons" onclick="openMap(event, 'application')"><i class="fal fa-map-signs" aria-hidden="true" style="padding-right:5px;"></i>Map</button>
          	 <button class="title_buttons" onclick="openDiff(event, 'application')"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>
          	 <button class="title_buttons" onclick="openPkgSearch()"><i class="fa-light fa-magnifying-glass" aria-hidden="true" style="padding-right:5px;"></i>Package Search</button>

          </div>
          <div id="applist_filter_area" style="height:25px;">
                  <div id="app_search_box_container"></div>
          </div>
        <div id="applist_list">
		  <table id="applist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="applist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Version</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Parent</th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Last Deployment to Environment</th>
				<th style="text-align: left;">Completed</th>
				<th style="text-align: left;">Result</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">appid</th>
            </tr>
           </thead>
          </table>
        </div>
        <div id="applist_map" height="100%" width="100%"></div>
      </div>
     <div id="complist_pane">
        <div id="complist_pane_title" style="display:inline-block;"><H2>Components</H2></div>
          <div id="complist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'component')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="compMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Base<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'docker');">Container</a>
                   <a onclick="addRow(event,'file');">Application File</a>
                   <a onclick="addRow(event,'database');">Database</a>
               </div>
             </div>
             <button class="title_buttons" onclick="addRow(event, 'compversion')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Version</button>
             <button class="title_buttons" onclick="delRow(event, 'component')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <button class="title_buttons" onclick="openList(event, 'component')"><i class="fal fa-list" aria-hidden="true" style="padding-right:5px;"></i>List</button>
          	 <button class="title_buttons" onclick="openMap(event, 'component')"><i class="fal fa-map-signs" aria-hidden="true" style="padding-right:5px;"></i>Map</button>
          	 <button class="title_buttons" onclick="openDiff(event, 'component')"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>
          </div>
          <div id="complist_filter_area" style="height:25px;">
             <div id="comp_search_box_container"></div>
          </div>
         <div id="complist_list">
		  <table id="complist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="complist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Version</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Parent</th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Last Deployment</th>
				<th style="text-align: left;">Completed</th>
				<th style="text-align: left;">Result</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">compid</th>
            </tr>
           </thead>
          </table>
         </div>
        <div id="complist_map" height="100%" width="100%"></div>
      </div>
      <div id="envlist_pane">
        <div id="envlist_pane_title" style="display:inline-block;"><H2>Environments</H2></div>
          <div id="envlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'environmet')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'environment')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'environment')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <div class="dropdown_menu">
               <button class="title_buttons envRptMenuButton" onclick="envRptMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-file-contract" aria-hidden="true" style="padding-right:5px;"></i>Reports<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a href="/dmadminweb/reports/EnvSuccessFail.html" target="_blank">Success/Failed Deployments per Environment Report</a>
               </div>
             </div>
             <button class="title_buttons" onclick="openDiff(event, 'environment')"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>
           </div>
           <div id="envlist_filter_area" style="height:25px;">
                  <div id="env_search_box_container"></div>
          </div>
         <div id="envlist_list">
		  <table id="envlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="envlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Environment</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">envid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="endpointlist_pane">
        <div id="endpointlist_pane_title" style="display:inline-block;"><H2>Endpoints</H2></div>
          <div id="endpointlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'endpoint')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'endpoint')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'endpoint')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="endpointlist_filter_area" style="height:25px;">
                  <div id="endpoint_search_box_container"></div>
          </div>
         <div id="endpointlist_list">
		  <table id="endpointlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="endpointlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Endpoint</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">endpointid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="actionlist_pane">
        <div id="actionlist_pane_title" style="display:inline-block;"><H2>Actions</H2></div>
          <div id="actionlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'action')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'action')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'action')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="actionlist_filter_area" style="height:25px;">
                  <div id="action_search_box_container"></div>
          </div>
         <div id="actionlist_list">
		  <table id="actionlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="actionlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Action</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">actionid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="procedurelist_pane">
        <div id="procedurelist_pane_title" style="display:inline-block;"><H2>Func/Procs</H2></div>
          <div id="procedurelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'procedure')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <div class="dropdown_menu">
               <button class="title_buttons addProcButton" onclick="procMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Procedure<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'procedure-ak2');">DMScript in database</a>
                   <a onclick="addRow(event,'procedure-ak3');">Local Script</a>
                   <a onclick="addRow(event,'procedure-ak4');">Endpoint Script</a>
               </div>
              </div>
             <div class="dropdown_menu">
               <button class="title_buttons addFuncButton" onclick="funcMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add Function<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'function-ak2');">DMScript in database</a>
                   <a onclick="addRow(event,'function-ak3');">Local Script</a>
                   <a onclick="addRow(event,'function-ak4');">Endpoint Script</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'procedure')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
             <button class="title_buttons" onclick="exportFuncProc()"><i class="fal fa-download" aria-hidden="true" style="padding-right:5px;"></i>Export</button>
             <button class="title_buttons" onclick="importProc()"><i class="fal fa-upload" aria-hidden="true" style="padding-right:5px;"></i>Import</button>
           </div>
           <div id="procedurelist_filter_area" style="height:25px;">
                  <div id="procedure_search_box_container"></div>
          </div>
         <div id="procedurelist_list">
		  <table id="procedurelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="procedurelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Name</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">Type</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">procedureid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="notifierlist_pane">
        <div id="notifierlist_pane_title" style="display:inline-block;"><H2>Notifiers</H2></div>
          <div id="notifierlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'notifier')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="notifierMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'hipchat');">HipChat</a>
                   <a onclick="addRow(event,'slack');">Slack</a>
                   <a onclick="addRow(event,'smtpemail');">Email</a>
                   <a onclick="addRow(event,'txtlocal');">SMS</a>
               </div>
             </div>
             <button class="title_buttons" onclick="delRow(event, 'notifier')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="notifierlist_filter_area" style="height:25px;">
                  <div id="notifier_search_box_container"></div>
          </div>
         <div id="notifierlist_list">
		  <table id="notifierlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="notifierlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Notifier</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">notifierid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="repositorylist_pane">
        <div id="repositorylist_pane_title" style="display:inline-block;"><H2>Repositories</H2></div>
          <div id="repositorylist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'repository')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="repoMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'filesystem');">File System</a>
                   <a onclick="addRow(event,'http');">Http</a>
                   <a onclick="addRow(event,'meister');">Meister</a>
                   <a onclick="addRow(event,'svn');">SVN</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'repository')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="repositorylist_filter_area" style="height:25px;">
                  <div id="repository_search_box_container"></div>
          </div>
         <div id="repositorylist_list">
		  <table id="repositorylist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="repositorylist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Repository</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">repositoryid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="datasourcelist_pane">
        <div id="datasourcelist_pane_title" style="display:inline-block;"><H2>Data Sources</H2></div>
          <div id="datasourcelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'datasource')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="datasourceMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'github');">GitHub</a>
                   <a onclick="addRow(event,'jira');">Jira</a>
                   <a onclick="addRow(event,'bugzilla');">Bugzilla</a>
                   <a onclick="addRow(event,'ldap');">LDAP</a>
                   <a onclick="addRow(event,'odbc');">ODBC</a>
               </div>
              </div>
             <button class="title_buttons" onclick="delRow(event, 'datasource')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="datasourcelist_filter_area" style="height:25px;">
                  <div id="datasource_search_box_container"></div>
          </div>
         <div id="datasourcelist_list">
		  <table id="datasourcelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="datasourcelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Data Source</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">datasourceid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="credentiallist_pane">
        <div id="credentiallist_pane_title" style="display:inline-block;"><H2>Credentials</H2></div>
          <div id="credentiallist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'credential')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
          	 <div class="dropdown_menu">
               <button class="title_buttons addMenuButton" onclick="credMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
               <div class="dropdown_menu-content" style="display: none;">
                   <a onclick="addRow(event,'ck2');">Encrypted in Database</a>
                   <a onclick="addRow(event,'ck5');">Private Key</a>
               </div>
             </div>
             <button class="title_buttons" onclick="delRow(event, 'credential')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="credentiallist_filter_area" style="height:25px;">
                  <div id="credential_search_box_container"></div>
          </div>
         <div id="credentiallist_list">
		  <table id="credentiallist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="credentiallist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Credential</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">credentialid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="userlist_pane">
        <div id="userlist_pane_title" style="display:inline-block;"><H2>Users</H2></div>
          <div id="userlist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'user')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'user')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'user')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="userlist_filter_area" style="height:25px;">
                  <div id="user_search_box_container"></div>
          </div>
         <div id="userlist_list">
		  <table id="userlist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="userlist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">User</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">userid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="grouplist_pane">
        <div id="grouplist_pane_title" style="display:inline-block;"><H2>Groups</H2></div>
          <div id="grouplist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'group')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
           </div>
           <div id="grouplist_filter_area" style="height:25px;">
                  <div id="group_search_box_container"></div>
          </div>
         <div id="grouplist_list">
		  <table id="grouplist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="grouplist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Group</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">groupid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="servercomptypelist_pane">
        <div id="servercomptypelist_pane_title" style="display:inline-block;"><H2>Types</H2></div>
          <div id="servercomptypelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'servercomptype')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'servercomptype')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'servercomptype')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="servercomptypelist_filter_area" style="height:25px;">
                  <div id="servercomptype_search_box_container"></div>
          </div>
         <div id="servercomptypelist_list">
		  <table id="servercomptypelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="servercomptypelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Type</th>
				<th style="text-align: left;">Domain</th>
				<th style="text-align: left;">domainid</th>
				<th style="text-align: left;">servercomptypeid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
      <div id="templatelist_pane">
        <div id="templatelist_pane_title" style="display:inline-block;"><H2>Notifiers Templates</H2></div>
          <div id="templatelist_buttons" style="display:inline-block;">
          	 <button class="title_buttons" onclick="openList(event, 'template')"><i class="fal fa-refresh" aria-hidden="true" style="padding-right:5px;"></i>Refresh</button>
             <button class="title_buttons" onclick="addRow(event, 'template')"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add</button>
             <button class="title_buttons" onclick="delRow(event, 'template')"><i class="fal fa-trash" aria-hidden="true" style="padding-right:5px;"></i>Delete</button>
           </div>
           <div id="templatelist_filter_area" style="height:25px;">
                  <div id="template_search_box_container"></div>
          </div>
         <div id="templatelist_list">
		  <table id="templatelist" class="hover row-border nowrap" height="100%" width="100%">
           <thead>
            <tr>
                <th>
                  <button style="border: none; background: transparent;" id="templatelist_checkall">
                    <i class="far fa-square"></i>
                  </button>
                </th>
				<th style="text-align: left;">Template</th>
				<th style="text-align: left;">Notifier</th>
				<th style="text-align: left;">templateid</th>
            </tr>
          </thead>
         </table>
        </div>
      </div>
     </div>
	<div id="dashboard_panel">
	<div id="panel_container_right" class="right" style="display:none" >
	 <div id="right_panel_header"></div>
	 <div id="right_panel_tabs">
	  <div class="tabrow_tabmenu">
	   <div class="moduletable_tabmenu">
	    <ul>
	      <li id="tabs-General" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">General</li>
          <li id="tabs-PackageComponents" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Package Components</li>
          <li id="tabs-apps2s" onclick="AppS2S(objid)" style="display:none">Application Service Hierarchy Bundle</li>
          <li id="tabs-DeliveryPipeline" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Delivery Pipeline</li>
        </ul>
	   </div>
	  </div>
	 </div>
	 <div id="right_panel_main">
	  <div id="right_panel_title_area" class="right_panel_title_area"></div>
	  <div id="title_icon" class="title_icon"></div>
        <div id="tabs-General-data" class="div_scroll" style="display:none">
            <div id="tabs-General-row-10">
            	 <div id="domnav-panel">
     			</div>
                <div id="tabs-General-left">
                    <div id="tabs-General-left-header">
                        <div id="compowner_summ_title">
                            <h2>Component Owner</h2>
                        </div>
                        <div id="compowner_summ_data">
                          <table id="compowner_summ" class="dev_table">
                            <tbody></tbody>
                          </table>
                        </div>
                        <div id="compowner_summ_data_edit" style="display:none";>
                          <table id="compowner_summtab" class="dev_table">
                            <tbody></tbody>
                          </table>
                        </div>
                        <div id="summ_title">
                            <h2>Details</h2>
                        </div>
                        <div id="summ_header_buttons">
                            <button class="edit_button" onClick="javascript:EditSummaryButton(false,'')">
                                <i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit
                            </button>
                            <button class="save_button" onClick="javascript:summOK(false,'')">
                                <i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save
                            </button>
                            <button class="cancel_button" onClick="javascript:summCancel('')">
                                <i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel
                            </button>
                            <button class="test_button" onClick="javascript:TestServer('servstat',objtypeAsInt,objid)">
                                <i class="fad fa-alien" aria-hidden="true"  style="padding-right:5px"></i>Test Connection
                            </button>
                            <button class="add_button" onClick="javascript:AddDomain(objtypeAsInt,objid)">
                                <i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add Subdomain
                            </button>
                            <button class="delete_button" onClick="javascript:DeleteDomain(objtypeAsInt,objid)">
                                <i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete Domain
                            </button>
                        </div>
                    </div>
                    <div id="summ_data">
                        <table id="summ" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="summ_data_edit" style="display:none";>
                        <table id="summtab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="emailbody_data">
                        <table id="emailbody" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="emailbody_data_edit" style="display:none";>
                        <table id="emailbodytab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="props_data">
                        <table id="props" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="props_data_edit" style="display:none";>
                        <table id="propstab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="adminrights_data">
                        <table id="adminrights" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="adminrights_data_edit" style="display:none";>
                        <table id="adminrightstab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="servstat_data">
                        <table id="servstat" class="dev_table">
                            <thead><tr><th>Test Connection Results</th></tr></thead>
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="row-10b-access">
                      <div id="row-10-baccess_title" style="text-align:left"">
                            <h2>Access Control</h2>
                      </div>
                      <div id="row-10b-access-data">
                      </div>
                   </div>
                </div>
              <div id="row-10-right-panel">
                <div id="summ_title">
                    <h2>Component Details</h2>
                </div>
                  <div id="appcomp-data">
                     <div id="appcomplist_list" style="width:96%;height:100px;display:inline"></div>
                        <table id="appcomplist" width="100%">
                            <thead>
                                <tr>
                                    <th style="text-align:left">Component</th>
                                    <th style="text-align:left">Domain</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                  </div>
                <div id="compowner_summ_header_buttons">
                            <button class="edit_button" onClick="javascript:EditSummaryButton(false,'')">
                                <i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit
                            </button>
                            <button class="save_button" onClick="javascript:summOK(false,'')">
                                <i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save
                            </button>
                            <button class="cancel_button" onClick="javascript:summCancel('')">
                                <i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel
                            </button>
                            <button class="test_button" onClick="javascript:TestServer('servstat',objtypeAsInt,objid)">
                                <i class="fad fa-alien" aria-hidden="true"  style="padding-right:5px"></i>Test Connection
                            </button>
                            <button class="add_button" onClick="javascript:AddDomain(objtypeAsInt,objid)">
                                <i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add Subdomain
                            </button>
                            <button class="delete_button" onClick="javascript:DeleteDomain(objtypeAsInt,objid)">
                                <i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete Domain
                            </button>
                </div>
                <div id="compitem_data">
                    <table id="compitem" class="dev_table">
                       <tbody></tbody>
                    </table>
                </div>
                <div id="compitem_data_edit" style="display:none";>
                   <table id="compitemtab" class="dev_table">
                      <tbody></tbody>
                   </table>
               	</div>
                <div id="emailto-panel">
                  <div id="emailto-panel-header">
                    <div id="emailto_title" style="text-align:left"">
                            <h2>Notifier Recipients</h2>
                    </div>
                    <div id="emailto_header_buttons">
                        	<button class="add_button" onClick="javascript:AddEmailToRow('emailto')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditEmailToRow('emailto')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteEmailToRow('emailto')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveEmailToRow('emailto')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelEmailToRow('emailto')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="emailto" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Recipients</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="emailto-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Recipients</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="groupmembership-panel">
                  <div id="groupmembership-panel-header">
                    <div id="groupmembership_title" style="text-align:left"">
                            <h2>Group Membership</h2>
                    </div>
                    <div id="groupmembership_header_buttons">
                        	<button class="add_button" onClick="javascript:AddGroupMembershipRow('groupmembership')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditGroupMembershipRow('groupmembership')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteGroupMembershipRow('groupmembership')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveGroupMembershipRow('groupmembership')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelGroupMembershipRow('groupmembership')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="groupmembership" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Groups for User</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="groupmembership-edit" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Groups for User</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="usermembership-panel">
                  <div id="usermembership-panel-header">
                    <div id="usermembership_title" style="text-align:left">
                            <h2>User Membership</h2>
                    </div>
                    <div id="usermembership_header_buttons">
                        	<button class="add_button" onClick="javascript:AddUserMembershipRow('usermembership')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditUserMembershipRow('usermembership')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteUserMembershipRow('usermembership')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveUserMembershipRow('usermembership')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelUserMembershipRow('usermembership')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="usermembership" class="dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Users for Group</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="usermembership-edit" class="dev_table">
                            <thead>
                                <tr>
                                <th>&nbsp;</th>
                                    <th>Users for Group</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                 <div id="attrs10-panel">
                 	<div id="attrs10-header">
                     	<div id="attrs10_title">
                      	  <h2>Key Value Configuration</h2>
                    	</div>
                    	<div id="attrs10_header_buttons">
                        	<button class="add_button" onClick="javascript:AddAttrRow('attrib10')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditAttrRow('attrib10')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteAttrRow('attrib10')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveAttrRow('attrib10')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelAttrRow('attrib10')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="attrib10-data">
                        <table id="attrib10" class="dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="row-10-access">
                    <div id="row-10-access_title" style="text-align:left"">
                            <h2>Access Control</h2>
                    </div>
                    <div id="row-10-access-data">
                    </div>
                </div>
                <div id="row-10-audit">
                        <div id="audit10_title" style="text-align:left"">
                            <h2>Audit Trail</h2>
                        </div>
                        <div id="audit10-data">
                          <div id="audit10-historyadd" style="width:96%;height:100px;display:inline""></div>
                          <table id="audit10-histfeed" class="dev_table" style="display:none;">
                               <tbody></tbody>
                          </table>
                        <table id="audit10" class="dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>Event</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        </div>
                </div>
            </div>
           </div>
           <div id="tabs-General-row-12">
            	<div id="readme-panel">
            	  <div id="readme_header">
	            	  <div id="readme_title" style="text-align:left"">
	                            <h2>Readme</h2>
	                  </div>
	                  <div id="readme_header_buttons">
                       	<button class="add_button" id="upload_readme"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
	                  </div>
                  </div>
                  <div id="readme-data">
                  </div>
            	</div>
            	<div id="swagger-panel">
            	  <div id="swagger_header">
	            	  <div id="swagger_title" style="text-align:left"">
	                            <h2>Swagger</h2>
	                  </div>
	                  <div id="swagger_header_buttons">
                       	<button class="add_button" id="upload_swagger"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
	                  </div>
                  </div>
                  <div id="swagger-data">
                  </div>
            	</div>
            </div>
            <div id="tabs-General-row-12a">
            	<div id="cve-panel">
            	  <div id="cve_title" style="text-align:left"">
                      <h2>Vulnerabilities</h2>
                  </div>
                  <div id="cve-data">
                     <div id="cvelist_list" style="width:96%;height:100px;display:inline"></div>
                        <table id="cvelist" width="100%">
                            <thead>
                                <tr>
                                    <th style="padding:0;">&nbsp;</th>
                                    <th>Package</th>
                                    <th>Version</th>
                                    <th>ID</th>
                                    <th>Url</th>
                                    <th>Summary</th>
                                    <th>Component</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                  </div>
                 </div>
            	<div id="license-panel">
            	  <div id="license_title" style="text-align:left"">
                      <h2>Software Bill of Materials (SBOM)</h2>
                  </div>
                  <div id="license-data">
                     <div id="licenselist_list" style="width:96%;height:100px;display:inline"></div>
                        <table id="licenselist" width="100%">
                            <thead>
                                <tr>
                                    <th>Package</th>
                                    <th>Version</th>
                                    <th>License</th>
                                    <th>Url</th>
                                    <th>Component</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                  </div>
                  </div>
            	</div>
            </div>
            <div id="tabs-General-row-15"  >
                <div id="logs-panel">
                        <div id="logs_title" style="text-align:left"">
                            <h2>Log History</h2>
                        </div>
                        <div id="logs-data">
                        <table id="loghist" class="dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>Deployment</th>
                                    <th>Result</th>
                                    <th>Date</th>
                                    <th>Environment</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        </div>
                </div>
                                <div id="attrs-panel">
                 	<div id="attrs-header">
                     	<div id="attrs_title">
                      	  <h2>Key Value Configuration</h2>
                    	</div>
                    	<div id="attrs_header_buttons">
                        	<button class="add_button" onClick="javascript:AddAttrRow('attrib')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditAttrRow('attrib')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteAttrRow('attrib')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveAttrRow('attrib')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelAttrRow('attrib')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="attrib-data">
                        <table id="attrib" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="attrib-edit" class="cb_dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Name</th>
                                    <th>Value</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
          		<div id="licensemd-panel">
          		   <div id="licensemd_header">
	            	  <div id="licensemd_title" style="text-align:left"">
	                     <h2>License</h2>
	                  </div>
	                  <div id="licensemd_header_buttons">
                       	<button class="add_button" id="upload_licensemd"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Upload</button>
	                  </div>
                  </div>
                  <div id="licensemd-data">
                  </div>
            	</div>
                <div id="outerworkflow" class="versionsandinformation">
     	 			<div class="versions">
	     				<div id="innerworkflow" style="height:600px;width:100%;">
		 				</div>
	    			</div>
       			</div>
    			<div id="pickproc" class="accordionpanel">
	     			<h4>Function/Procedure</h4>
   	     			 <div id="fragmentlist" align="left"></div>
        		</div>
                <div id="inputparams-panel">
                  <div id="inputparams-panel-header">
                    <div id="inputparams_title">
                    	<h2>Input Parameters</h2>
                    </div>
                    <div id="inputparams_header_buttons">
                        	<button class="add_button" onClick="javascript:AddInputParamsRow('inputparams')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditInputParamsRow('inputparams')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteInputParamsRow('inputparams')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveInputParamsRow('inputparams')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelInputParamsRow('inputparams')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                     </div>
                   </div>
                  	<table id="inputparams" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;<th>Name</th><th>Type</th><th>Format</th><th>Missing</th><th>Padding</th><th>Required</th><th>Pos</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          			<table id="inputparams-edit" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;<th>Name</th><th>Type</th><th>Format</th><th>Missing</th><th>Padding</th><th>Required</th><th>Pos</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
                </div>
                <div id="switchargs-panel">
                  <div id="switchargs-panel-header">
                    <div id="switchargs_title">
                    	<h2>Command Line Switches</h2>
                    </div>
                    <div id="switchargs_header_buttons">
                        	<button class="add_button" onClick="javascript:AddSwitchArgsRow('switchargs')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditSwitchArgsRow('switchargs')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteSwitchArgsRow('switchargs')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveSwitchArgsRow('switchargs')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelSwitchArgsRow('switchargs')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                     </div>
                   </div>
                   <table id="switchargs" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Name</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          			<table id="switchargs-edit" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Name</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
                </div>
               <div id="srv4env-panel">
                 	<div id="srv4env-header">
                     	<div id="srv4env_title">
                      	  <h2>Assigned Endpoints</h2>
                    	</div>
                    	<div id="srv4env_header_buttons">
                        	<button class="add_button" onClick="javascript:AddSrv4EnvRow('srv4env')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditSrv4EnvRow('srv4env')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteSrv4EnvRow('srv4env')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveSrv4EnvRow('srv4env')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelSrv4EnvRow('srv4env')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    	</div>
                    </div>
                    <div id="srv4env-data">
                        <table id="srv4env" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th></th>
                                    <th>Name</th>
                                    <th>Hostname</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="srv4env-edit" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                    <th></th>
                                    <th>Name</th>
                                    <th>Hostname</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
              <div id="emailhelp-panel" style="display:none">
                <div id="emailhelp_title">
                      	  <h2>Using Variables</h2>
                </div>
                <div id="emailhelp_data">
          When constructing the subject line and message body for a template, you can use
          <I>Variables</I>. These variables are automatically expanded when the notification
          is issued.<br><br>
          Variables can be either <I>Object Methods</I> (such as \${application.name}) or
          <I>Scalar Variables</I> that are set by DeployHub during specific operations
          (such as $SERVER_STATUS during a periodic server scan).<br><br>Object Methods can be used for
          any object that is available to DeployHub during the operation. For example, you could
          use \${application.id} to get the internal application id or \${application.domain.name} to get
          the current domain for the application.<br><br>
          The table below shows which variables are set during specific operations.<br><br>
          <table id="emailhelptab" class="dev_table" style="white-space:nowrap">
          <thead>
          <tr><th>Variable Name</th><th>Description</th><th>Operation</th></tr>
          </thead>
          <tbody>
          <tr ><td>\${application.name}</td><td>The name of the current application</td><td>Any DeployHub task operating on an Application (such as <i>Deploy, Move, Approve</i> etc)</td></tr>
 	 	  <tr><td>\${environment.name}</td><td>The name of the target environment</td><td>Any DeployHub task operating on an Environment (such as <i>Deploy</i> etc)</td></tr>
		  <tr ><td>\${user.name}</td><td>The name of the invoking user</td><td>Any DeployHub task.</td></tr>
		  <tr><td>\$DEPLOY_ID</td><td>The Deployment ID Number</td><td>Set during a Deploy task.</td></tr>
          <tr ><td>\$SERVER_NAME</td><td>Endpoint Name</td><td>The name of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr><td>\$SERVER_HOSTNAME</td><td>Endpoint hostname</td><td>The hostname of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr ><td>\$SERVER_STATUS</td><td>OK or FAILED</td><td>The status of the server during an automatic periodic scan (AutoPing). Use this variable in a Ping template.
          Note this variable is always OK in an MD5 template since MD5 scans are only performed if the ping check succeeds.</td></tr>
          <tr><td>\$NAME_RESOLUTION</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of resolution of hostname. Use this variable in a Ping template.</td></tr>
          <tr ><td>\$PING</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the ping to the server. Use this variable in a Ping template.</td></tr>
          <tr><td>\$CONNECTION</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the connection attempt to the server using the associated credentials. Use this variable in a Ping template.</td></tr>
          <tr ><td>\$BASEDIR</td><td>OK or FAILED</td><td>Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the attempt to switch to the server's base directory. Use this variable in a Ping template.</td></tr>
          <tr><td>\$MD5_FILELIST</td><td>A list of files (one per line)</td><td>Set during an automatic periodic scan (AutoMD5). Lists all the files whose MD5 checksum differs from that calculated during the last deployment to the server. Use this variable in a MD5 template.</td></tr>
          <tr ><td>\$SRCDOMAIN</td><td>The name of the Source Domain</td><td>Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>from</I> (for a move) or its current domain (for an approve).</td></tr>
          <tr><td>\$TGTDOMAIN</td><td>The name of the Target Domain</td><td>Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>to</I> or approved <I>for</I>.</td></tr>
          <tr ><td>\$NOTES</td><td>Free Text</td><td>Set during a <I>Move, Approve</I> or <I>Request</I> task. The notes that were entered (if any) by the user when the task was run.</td></tr>
          <tr><td>\$TASKNAME</td><td>The name of the Requested Task</td><td>Set during a <I>Request</I> task. The name of the task being requested to be run.</td></tr>
          <tr ><td>\$ERROR_TEXT</td><td>Last Endpoint Error</td><td>Set during an automatic periodic scan (AutoPing). The last error detected during the connection attempt.</td></tr>
          </tbody>
          </table>
               </div>
              </div>
            </div>
            <div id="tabs-General-row-20">
                <div id="tabs-General-right-20">
                    <div id="trends-panel">
                        <div id="trends_title" style="text-align:left"">
                            <h2>Trends</h2>
                        </div>
                        <div id="reports-data" style="display:inline-block">
                            <div id="plot1resizable" style="float:left;height:200px;width:400px">
                                <div id="timetodeploylinechart" style="height:96%;width:96%;"></div>
                            </div>
                            <div id="plot2resizable" style="float:left;height:200px;width:200px">
                                <div id="applicationsuccesspiechart" style="height:96%;width:96%;"></div>
                            </div>
                        </div>
                    </div>
                </div>
                <div id="steps-panel">
                        <div id="steps_title" style="text-align:left"">
                            <h2>Current Step Duration</h2>
                        </div>
                        <div id="steps-data" style="display:inline-block">
     						<div id="plot4resizable" style="float:left;height:200px;width:200px">
     						<div id="deploymenttimebystep4" style="height:96%;width:96%;"></div>
     					</div>
                        </div>
                </div>
                <div id="comp4srv-panel">
                  <div id="comp4srv-panel-header">
                    <div id="comp4srv_title">
                        <h2>Deployed Components</h2>
                    </div>
                  </div>
                    <div id="comp4srv-data">
                        <table id="comp4srv" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Current Version</th>
                                    <th>Deployment</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="comp4srv-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Current Version</th>
                                    <th>Deployment</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
                <div id="env4app-panel">
                  <div id="env4app-panel-header">
                    <div id="envs_title">
                        <h2>Assigned Environments</h2>
                    </div>
                    <div id="env4app_header_buttons">
                        	<button class="add_button" onClick="javascript:AddEnv4AppRow('env4app')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditEnv4AppRow('env4app')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteEnv4AppRow('env4app')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveEnv4AppRow('env4app')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelEnv4AppRow('env4app')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                    </div>
                  </div>
                    <div>
                        <table id="env4app" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Environment</th>
                                    <th>Details</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="env4app-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>Environment</th>
                                    <th>Details</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>

                 <div id="deployedenv4app-panel">
                  <div id="deployedenv4app-panel-header">
                    <div id="deployed_envs_title">
                        <h2>Deployed Environments</h2>
                    </div>
                  </div>
                    <div id="deployedenv4app-data">
                        <table id="deployedenv4app" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>Environment</th>
                                    <th>Details</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>

                 <div id="app4env-panel">
                  <div id="app4env-panel-header">
                    <div id="app4env_title">
                        <h2>Assigned Applications</h2>
                    </div>
                  </div>
                    <div>
                        <table id="app4env" class="dev_table">
                            <thead>
                                <tr>
                                    <th>Application</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>
                <div id="cmdline-panel">
                   <div id="dragtextdiv" style="margin:10px">Drag the arguments from here onto the command line below to assemble the command. Use the table above to add and configure the switches/arguments.</div>
                   <div id="argsoutputpane"></div>
                </div>
                <div id="defect-panel-20">
                        <div id="defects_title">
                            <h2>Change Requests</h2>
                        </div>
                         <div id="defect_header_buttons_20">
                        </div>
                        <div id="defect-20-data">
                        <table id="defect-20" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>CR</th>
                                    <th>Summary</th>
                                    <th>Component</th>
                                    <th>Status</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                 </div>
            </div>
            <div id="tabs-General-row-25">
            </div>
            <div id="tabs-General-row-30">
                <div id="app4comp-panel">
                        <div id="app4comp_title" style="text-align:left"">
                            <h2>Consuming Applications</h2>
                        </div>
                      	<table id="app4comp" class="dev_table" width="100%">
           					<thead>
           					</thead>
           					<tbody>
           					</tbody>
          			  	</table>
          		</div>
               <div id="srv4comp-panel">
                  <div id="srv4comp-panel-header">
                    <div id="srv4comp_title">
                        <h2>Endpoints</h2>
                    </div>
                    <div id="srv4comp_header_buttons">
                        	<button class="add_button" onClick="javascript:AddSrv4CompRow('srv4comp')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditSrv4CompRow('srv4comp')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteSrv4CompRow('srv4comp')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveSrv4CompRow('srv4comp')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelSrv4CompRow('srv4comp')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                     </div>
                  </div>
                  <div>
                   	<table id="srv4comp" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Endpoint</th><th>Deployment</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          			<table id="srv4comp-edit" class="cb_dev_table" width="100%" >
           				<thead>
            				<tr><th>&nbsp;</th><th>Endpoint</th><th>Deployment</th></tr>
           				</thead>
           				<tbody>
           				</tbody>
          			</table>
          		 </div>
                </div>
          		<div id="planning-panel">
                        <div id="planning_title" style="text-align:left"">
                            <h2>Applications scheduled for this Release</h2>
                        </div>
          		        <div id="planningtimeline">
          		        </div>
               </div>
            </div>
            <div id="tabs-General-row-28">
                 <div id="feedback-panel">
                    <div id="feedback-panel-header">
                        <div id="feedback_title" style="text-align:left"">
                            <h2>Last Deployment Difference based on Environment:</h2>
                        </div>
                        <div id="feedback-env"><select id="feedback-env-sel" onchange="updateDeployDiff(this);"></select></div>
                        <div id="feedback-label"></div>
                    </div>
                    <div id="feedback-data">
                    </div>
                </div>
                <div id="procbody-panel">
                 <div id="ProcBodyMainFrame" style="height:100%;width:100%;">
                    <textarea id='editproc' cols='400' wrap='off' spellcheck='false' style='width:100%;height:600px;font-size:14px;font-family:courier;white-space: pre; overflow: auto;'></textarea>
                 </div>
                </div>
            </div>
            <div id="tabs-General-row-32">
                <div id="provides-panel">
                   <div id="provides_title" style="text-align:left"">
                        <h2>Providing API End Points</h2>
                    </div>
                     <div id="provides_list">
					  <table id="provideslist" class="hover row-border nowrap" height="100%" width="100%">
			           <thead>
			            <tr>
							<th style="text-align: left;">Verb</th>
							<th style="text-align: left;">API End Point</th>
			            </tr>
			          </thead>
			         </table>
			        </div>
          		</div>
          		<div id="consumes-panel">
                        <div id="consumes_title" style="text-align:left"">
                            <h2>Consuming API End Points</h2>
                        </div>
                     <div id="consumes_list">
					  <table id="consumeslist" class="hover row-border nowrap" height="100%" width="100%">
			           <thead>
			            <tr>
							<th style="text-align: left;">Verb</th>
							<th style="text-align: left;">API End Point</th>
							<th style="text-align: left;">Component</th>
			            </tr>
			          </thead>
			         </table>
			        </div>
               </div>
             </div>
               <div id="tabs-General-row-35">
                <div id="row-35-audit">
                        <div id="audit35_title" style="text-align:left"">
                            <h2>Audit Trail</h2>
                        </div>
                        <div id="audit35-data">
                          <div id="audit35-historyadd" style="width:96%;height:100px;display:inline""></div>
                          <table id="audit35-histfeed" class="dev_table" style="display:none"">
                               <tbody></tbody>
                          </table>
                        <table id="audit35" class="dev_table" width="100%">
                            <thead>
                                <tr>
                                    <th>Event</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        </div>
                </div>
                <div id="row-35-access">
                    <div id="row-35-access_title" style="text-align:left"">
                            <h2>Access Control</h2>
                    </div>
                    <div id="row-35-access-data">
                    </div>
                </div>
               </div>
                <div id="tabs-General-row-40">
                    <div id="defect-panel">
                        <div id="defects_title">
                            <h2>Change Requests</h2>
                        </div>
                         <div id="defect_header_buttons">
                        	<button class="add_button" onClick="javascript:AddDefectRow('defect')"><i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add</button>
                        	<button class="edit_button" onClick="javascript:EditDefectRow('defect')"><i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit</button>
                        	<button class="delete_button" onClick="javascript:DeleteDefectRow('defect')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                        	<button class="save_button" onClick="javascript:SaveDefectRow('defect')"><i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save</button>
                        	<button class="cancel_button" onClick="javascript:CancelDefectRow('defect')"><i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel</button>
                        </div>
                        <div id="defect-data">
                        <table id="defect" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>CR</th>
                                    <th>Summary</th>
                                    <th>Component</th>
                                    <th>Status</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                        <table id="defect-edit" class="cb_dev_table">
                            <thead>
                                <tr>
                                    <th>&nbsp;</th>
                                    <th>CR</th>
                                    <th>Summary</th>
                                    <th>Component</th>
                                    <th>Status</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                    </div>
                 <div id="tabs-General-right">
                    <div id="tabs-General-right-header">
                        <h2>Dependency Map</h2>
                    </div>
                    <div id="tabs-General-map"></div>
                </div>
                </div>
             <div id="tabs-General-row-50">
		       <div id="panel_container_taskspaneltop">
				<div id="tasks-panel">
				  <div id="tasks-header">
                     	<div id="tasks_title">
                      	  <h2>Tasks</h2>
                    	</div>
                    	<div id="tasks_header_buttons">
                        	<div class="dropdown_menu">
                              <button class="title_buttons addMenuButton" onclick="addTaskMenu()" style="color: rgb(51, 103, 214);"><i class="fal fa-plus" aria-hidden="true" style="padding-right:5px;"></i>Add<i class="fal fa-caret-down fa-lg" aria-hidden="true" style="padding-left:5px;"></i></button>
                                <div class="dropdown_menu-content" style="display: none;">
                                  <a onclick="AddTaskRow('taskstab','Approve');">Approve Verision for move to next pipeline stage</a>
                                  <a onclick="AddTaskRow('taskstab','Move');">Move Version to the next or previous stage in the pipeline</a>
                                  <a onclick="AddTaskRow('taskstab','Deploy');">Deploy Version to an Environment</a>
                                  <a onclick="AddTaskRow('taskstab','Request');">Request Calendar entry for deployment to an Environment</a>
                                  <a onclick="AddTaskRow('taskstab','RunAction');">Mannually trigger Action to be executed</a>
                               </div>
                            </div>
                        	<button class="delete_button" onClick="javascript:DeleteTaskRow('taskstab')"><i class="fal fa-trash" aria-hidden="true"  style="padding-right:5px"></i>Delete</button>
                    	</div>
                    </div>
                    <div id="taskstab-data">
                        <table id="taskstab" class="cb_dev_table" width="100%" >
                            <thead>
                                <tr>
                                <th>&nbsp;</th>
                                    <th>Name</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
				</div>
				<div id="panel_deploy">
				     <div id="task-right-header">
                        <div id="task_summ_title">
                            <h2>Task Details</h2>
                        </div>
                        <div id="task_summ_header_buttons">
                            <button class="edit_button" onClick="javascript:EditSummaryButton(false,'task_')">
                                <i class="fal fa-pencil" aria-hidden="true"  style="padding-right:5px"></i>Edit
                            </button>
                            <button class="save_button" onClick="javascript:summOK(false,'task_')">
                                <i class="fal fa-save" aria-hidden="true"  style="padding-right:5px"></i>Save
                            </button>
                            <button class="cancel_button" onClick="javascript:summCancel('task_')">
                                <i class="fal fa-times-circle" aria-hidden="true"  style="padding-right:5px"></i>Cancel
                            </button>
                        </div>
                    </div>
                    <div id="task_summ_data">
                        <table id="task_summ" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
                    <div id="task_summ_data_edit" style="display:none";>
                        <table id="task_summtab" class="dev_table">
                            <tbody></tbody>
                        </table>
                    </div>
				</div>
			</div>
		<div id="panel_container_taskspanelbottom">
			<div id="sub_panel_bottomleft">
			  <div class="panel_container_title_area tasks"><h1 class="taskpanels">Group Access for Task</h1></div>
			   <div class="deploy_sub_panel_container">
			    <ul class="subpanel" id="tasks_assigned_groups">
 			    </ul>
 			   </div>
			</div>
			<div id="sub_panel_bottommiddle">
			 	<div class="panel_container_title_area"><h1 class="taskpanels">Available Groups for Task</h1></div>
			 	<div class="deploy_sub_panel_container">
			 		<ul class="subpanel" id="tasks_available_groups">
 					</ul>
 				</div>
 			</div>
 			<div id="sub_panel_bottomright">
 			    <div class="panel_container_title_area"><h1 class="taskpanels">Task Parameters</h1></div>
			 	<div id="task_params"></div>
			 	<div id="task_param_content"></div>
 			</div>
		</div>
		</div>
                    </div>
                </div>

       <div id="tabs-PackageComponents-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="compversions">
	     <div id="innercomp">
		 </div>
	    </div>
	    <div class="accordionpanel">
	     <h4 class="ui-widget-header">Component Selector</h4>
   	     <div id=componentlist align="left" style="margin:0; overflow-x: auto; height: calc(100% - 52px); background-color: #f8f8f8;"></div>
        </div>
       </div>
      </div>
		 </div>
		</div>
       </div>
     </div>
	</div> <!-- end panel container -->
	<div id="panel_container_historyandpending" style="display:none">
	 <div id="home_right_panel_tabs">
	  <div class="tabrow_tabmenu">
	   <div class="moduletable_tabmenu">
	    <ul>
	      <li id="tabs-HomeDashboard" onclick="ActivateHomeSubTabs('tabs-HomeDashboard')" class="active current">Global Reports</li>
          <li id="tabs-HomePending" onclick="ActivateHomeSubTabs('tabs-HomePending')">To Do List</li>
          <li id="tabs-HomeHistory" onclick="ActivateHomeSubTabs('tabs-HomeHistory')">Audit Trail</li>
        </ul>
	   </div>
	  </div>
	 </div>
   </div>
   </div>  <!-- end container -->
	<div class="clear">
	</div>
	<div id="login_container_dialog"></div>
	<div id="firstinstall_container_dialog"></div>
	<div id="modal" style="display:none"></div>
	<div id="rel-upgrade" title="Upgrade to Pro" class="dmdialog" style="display:none">
 		 <h2 style="padding-right:30px">Please upgrade to the Pro version in order to create Releases.</h2>
	     <form>
	     </form>
	</div>
	<div id="dom-upgrade" title="Upgrade to Pro" class="dmdialog" style="display:none">
 		 <h2 style="padding-right:30px">Please upgrade to the Pro version in order to create project subfolders.</h2>
	     <form>
	     </form>
	</div>
	<div id="displaylog"></div>
	<div id="buildlog"></div>
	<div id="throbber" style="display:none;">
    	<img src="css/images/dog.gif" style="height:80px;border-radius: 25px;" />
    	<p style="position:relative;left:-7px;">Sit tight....</p>
	</div>
 <footer class="footer">
	<div id="footer_container">
	</div>
 </footer>
</body>
</html>
