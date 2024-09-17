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
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<style>
#container
{
	margin: 0 auto;
	width: 100%;
}

#header
{
	padding: 10px;
}

#header h1 { margin: 0; }

#content-container
{
	float: left;
	width: 100%;
	height: 100%;
}

#content
{
	clear: left;
	float: left;
	width: 95%;
	height: 100%;
	padding: 5px 0;
	margin: 0 0 0 4%;
	display: inline;
}

#content h2 { margin: 0; }

h4 {
	line-height: 16px;
	margin: 0 0 0.4em;
}

</style>
</head>
<body>


<c:if test="${run == null}">
<h4 class="ui-widget-header">${task.name}</h4>
<div id="tasktabs" style="height: 100%;overflow:auto">
 <ul>
  <li><a href="#tasktabs-General">General</a></li>
 </ul>
 <div id="tasktabs-General">
  <div id="tasksummary"></div>
 </div>
</div>

<script type="text/javascript">
var taskSummaryCreated=false;

function ActivateTab(tabId) {
    switch(tabId) {
    case "General":
    	if(!taskSummaryCreated) {
			$("#tasksummary").html("Loading...")
				.load("GetSummary?suff=task&cbfunc=taskSummarySaved&objtype=${task.objectTypeAsInt}&id=${task.id}");
			taskSummaryCreated = true;
		}
		break;
    }
}

$(function()
{
	$( "#tasktabs" ).tabs({
		activate: function(event, ui) {
			var tid = ui.newPanel.selector;
			var sid = tid.split('#tasktabs-');
			ActivateTab(sid[1]);
		},
		create: function(event,ui) {
			// Tabs are created, fire the callback function (if supplied)
			<c:if test="${cbfunc != ''}">
				console.log("cbfunc is set to ${cbfunc}");
		  		var callbackFunction = eval("${cbfunc}");
		  		if (typeof callbackFunction === "function") {
		  			console.log("Calling it...");
		  			callbackFunction();
		  			console.log("${cbfunc} called");
		  		}
		  	</c:if>
		}
	}); // end tabs
	ActivateTab('General');
}) // end anonymous function
</script>
</body>
</c:if>

<c:if test="${run == 'Y'}">
<div id="container">
 <div id="header">
  <div style="float:left; padding-right:8px;"><img src="images/task-large.png"></div>
  <div>User-defined Task</div>
 </div>
 <div id="content-container">
  <div id="content" class="ui-widget-content ui-corner-all"></div>
 </div>
</div>
</c:if>
</html>
