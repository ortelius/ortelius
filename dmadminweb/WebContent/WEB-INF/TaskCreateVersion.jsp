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

<script src="jquery.jstree.js" type="text/javascript"></script>
<script src="js/createdtt.js" type="text/javascript"></script>

<c:if test="${run == null}">
<h4 class="ui-widget-header">${task.name}</h4>
<div id="tasktabs" style="height: 100%; font-size: 12px;overflow:auto">
 <ul>
  <li><a href="#tasktabs-General">General</a></li>
  <li><a href="#tasktabs-Target">Target Domain</a></li>
 </ul>
 <div id="tasktabs-General">
  <div id="tasksummary"></div>
 </div>
 <div id="tasktabs-Target" style="overflow-y:hidden;">
  <div style="overflow-y:auto;float:left;width:60%;height:75%;background-color:#ffffee;" class="ui-widget-content ui-corner-all">
   <div style="background-color:#ffffee;"><h4 class="ui-widget-header">Domain Selector</h4>
    <div id="domaintree" style="height:90%;background-color:#ffffee;"></div>
   </div>
  </div>
  <div style="overflow-y:hidden;float:right;width:39%;height:75%;" class="ui-widget-content ui-corner-all">
   <div><h4 class="ui-widget-header">Target Domain</h4>
    <div style="height:90%;">
     <input type=text id="tgtdomain" size="20" /><button id="savetgt">Save</button>
    </div>
   </div>
  </div>
 </div>
</div>

<script type="text/javascript">
var taskSummaryCreated=false;
var curtgtid=0;
var $tgtbutton;

function SaveTargetDomain() {
	$tgtbutton.button('disable');
	console.log("f=utcv&tid=${task.id}&tgt="+curtgtid.substr(2));
	$.getJSON("UpdateAttrs","f=utcv&tid=${task.id}&tgt="+curtgtid.substr(2),
			function(data){
				console.log("task updated successfully");
	});
}

function ActivateTab(tabId) {
    switch(tabId) {
    case "General":
    	if(!taskSummaryCreated) {
			$("#tasksummary").html("Loading...")
				.load("GetSummary?suff=task&cbfunc=taskSummarySaved&objtype=${task.objectTypeAsInt}&id=${task.id}");
			taskSummaryCreated = true;
		}
		break;
    case "Target":
		break;
    }
}

function dtSelectNode(data,loading)
{
	if (data) {
		var id = data.inst.get_json()[0].attr.id;
		$("#tgtdomain").val($('.jstree-clicked').text());
		if (loading==0 && curtgtid != id) {
			$tgtbutton.button('enable');
		}
		curtgtid=id;
	}
}

$(function()
{
	$tgtbutton = $("#savetgt");
	$tgtbutton.button().click(function() {
		SaveTargetDomain();
	});
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
	dtCreateTree("#domaintree","${domain.id}");
	$tgtbutton.button('disable');
}) // end anonymous function
</script>
</body>
</c:if>

<c:if test="${run == 'Y'}">
<div id="container">
 <div id="header">
  <div style="float:left; padding-right:8px;"><img src="images/newappver-med.png"></div>
  <div>Create new version of application ${app.name} in Domain "${domain.name}"</div>
 </div>
 <div id="content-container">
  <div id="content" class="ui-widget-content ui-corner-all">
   <h4 class="ui-widget-header">Predecessor Version:</h4>
    <select id="pred" style="width:95%">
     <option value="0">Based on parent application</option>
     <option value="-1">Latest version</option>
     <c:forEach items="${predlist}" var="av">
     <option value="${av.id}">${av.name}</option>
     </c:forEach>
   </select>
  </div>
 </div>
</div>
</c:if>
</html>
