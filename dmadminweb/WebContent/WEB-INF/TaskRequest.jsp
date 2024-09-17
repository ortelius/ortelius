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
<html>
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

.amain   {white-space:nowrap;cursor: pointer;padding: 0 5px 5px 16px;}


</style>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
</head>
<body>
<c:if test="${run == null}">
<h4 class="ui-widget-header">${task.name}</h4>
<div id="tasktabs" style="height: 100%;overflow:auto">
	<ul>
		<li><a href="#tasktabs-General">General</a></li>
		<li><a href="#tasktabs-Target">Requested Task</a></li>
	</ul>
	<div id="tasktabs-General">
		<div id="tasksummary"></div>
	</div>
	<div id="tasktabs-Target" style="overflow-y:hidden;">
		<div style="overflow-y:auto;float:left;width:60%;height:75%;background-color:#ffffee;" class="ui-widget-content ui-corner-all">
			<div style="background-color:#ffffee;"><h4 class="ui-widget-header">Select a Task to Request</h4>
				<c:forEach items="${tasklist}" var="tl">
				<div id="a${tl.id}" class="amain ${tl.taskType}">${tl.name}</div>
				</c:forEach>
			</div>
		</div>
		<div style="overflow-y:hidden;float:right;width:39%;height:75%;" class="ui-widget-content ui-corner-all">
			<div><h4 class="ui-widget-header">Linked Approval Task</h4>
				<div style="height:90%;">
				<input type=text id="tgttask" size="20" /><button id="savetgt">Save</button>
				</div>
			</div>
		</div>
   </div>
</div>
</body>
<script type="text/javascript">
var taskSummaryCreated=false;
var targetSelected=false;

console.log("Setting linkSelectedTask to null");
var linkSelectedTask="";
var CurrentTask;

function appover()
{
	CurrentTask = $(this).attr("id");
	if (CurrentTask != linkSelectedTask)
	{
		$(this).css("background-color","#e7f4f9");
	}
}

function appout()
{
	CurrentTask="";
	if ($(this).attr("id") != linkSelectedTask)
	{
		$(this).css("background-color","white");
	}
}

function appseltask()
{
	console.log("in appseltask 1");
	//if (!targetSelected) {
		console.log("in appseltask, targetSelected="+targetSelected+" linkSelectedTask="+linkSelectedTask);
		$("#"+linkSelectedTask).css("background-color","white");
		$(this).css("background-color","#96ebff");
		var id=$(this).attr("id");
		console.log("id="+id);
		$("#tgttask").val($("#"+id).text());
		if (linkSelectedTask != id) {
			console.log("enabling save button");
			$tgtbutton.button('enable');
		}
		linkSelectedTask=id;
	//}
}

var $tgtbutton;

function SaveLinkedTask() {
	console.log("linkSelectedTask="+linkSelectedTask);
	$tgtbutton.button('disable');
	console.log("f=utrp&tid=${task.id}&tgt="+linkSelectedTask.substr(1));
	$.getJSON("UpdateAttrs","f=utrp&tid=${task.id}&tgt="+linkSelectedTask.substr(1),
			function(data){
				console.log("task updated successfully");
	});
}

function ActivateTab(tabId) {
    switch(tabId) {
    case "General":
    	if(!taskSummaryCreated) {
    		console.log("GetSummary?suff=task&objtype=${task.objectTypeAsInt}&id=${task.id}");
			$("#tasksummary").html("Loading...")
				.load("GetSummary?suff=task&cbfunc=taskSummarySaved&objtype=${task.objectTypeAsInt}&id=${task.id}");
			taskSummaryCreated = true;
		}
		break;
    case "Target":
    	if(!targetSelected) {
    		console.log("linktask id=${linktask.id}");
    		linkSelectedTask="a${linktask.id}";
    		$("#a${linktask.id}").click();
    		targetSelected=true;
    	}
	}
}

$(function()
{
	linkSelectedTask="";
	$tgtbutton = $("#savetgt");
	$tgtbutton.button().click(function() {
		SaveLinkedTask();
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
	console.log("disabling save button");
	$tgtbutton.button('disable');
	$(".amain").hover(appover,appout);
	$(".amain").click(appseltask);
}) // end anonymous function


</script>
</c:if>
<c:if test="${run == 'Y'}">
<div id="container">
	<div id="header">
		<div style="float:left;"><img id="icon" src="images/approved-med.png"></div>
		<div>${titletext}</div>
	</div>
	<div id="content-container">
		<div id="content" class="ui-widget-content ui-corner-all">
			<h4 class="ui-widget-header">Notes:</h4>
			<textarea id="notes" rows=9 style="width:95%;height:100%"></textarea>
		</div>
	</div>
</div>
</c:if>
</html>
