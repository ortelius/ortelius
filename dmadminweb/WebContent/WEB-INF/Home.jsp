 <%--
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
 --%>
<!doctype html>
<%@page contentType="text/html" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
<head>
<title>DeployHub</title>

<link rel="icon" href="favicon.ico">

<link rel='stylesheet' type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,700italic,800italic,600,400,700,800?v=${initParam.BUILD_VERSION}'>
<link rel="stylesheet" type="text/css" href="css/cupertino/jquery-ui-min.css?v=${initParam.BUILD_VERSION}" />
<link rel="stylesheet" type="text/css" href="css/jquery.dmcontextmenu.css?v=${initParam.BUILD_VERSION}" />
<link rel="stylesheet" type="text/css" href="css/jquery.jqplot.css?v=${initParam.BUILD_VERSION}" />
<link rel="stylesheet" type="text/css" href="css/calendar.css?v=${initParam.BUILD_VERSION}" />
<link rel="stylesheet" type="text/css" href="css/calendar-print.css?v=${initParam.BUILD_VERSION}" media="print" />
<link rel="stylesheet" type="text/css" href="css/vis.min.css?v=${initParam.BUILD_VERSION}" />
<link rel="stylesheet" type="text/css" href="css/devops_frydesign.css?v=${initParam.BUILD_VERSION}" />


<script type="text/javascript" src="js/jquery.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery-ui.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="jquery.jstree.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.cookie.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/cookies.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.blockUI.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.sessionTimeout.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-util.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-dom-adapter.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-drag.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-endpoint.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-connection.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-anchors.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-defaults.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-connectors-statemachine.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-connectors-flowchart.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-svg.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-canvas.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jsPlumb-renderers-vml.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.jsPlumb.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/selectenter.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.jqplot.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.barRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.pieRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasTextRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasAxisLabelRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasAxisTickRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.canvasOverlay.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.categoryAxisRenderer.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/plugins/jqplot.highlighter.min.js?v=${initParam.BUILD_VERSION}"></script> 
<script type="text/javascript" src="js/plugins/jqplot.cursor.min.js?v=${initParam.BUILD_VERSION}"></script> 
<script type="text/javascript" src="js/plugins/jqplot.json2.min.js?v=${initParam.BUILD_VERSION}"></script>

<script type="text/javascript" src="js/fullcalendar.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/date.format.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.json-2.4.min.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/globalize.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery-ui-timepicker-addon.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src='js/cultures/globalize.culture.de-DE.js?v=${initParam.BUILD_VERSION}'></script>
<script type="text/javascript" src='js/cultures/globalize.culture.en-GB.js?v=${initParam.BUILD_VERSION}'></script>
<script type="text/javascript" src='js/cultures/globalize.culture.en-US.js?v=${initParam.BUILD_VERSION}'></script>
<script type="text/javascript" src="js/vis.min.js?v=${initParam.BUILD_VERSION}"></script>

<script type="text/javascript" src="js/om/createtree.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/createtree_selection.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GlobalVars.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/Setup.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetSummary.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetAttributes.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetPrerequisities.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetComponents.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetComponentItems.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetVersionsOfApp.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetVersionsOfComp.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetApplicationVersions.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.form.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/newsfeed.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetReports.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetDeployReports.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetServers4Env.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetServers4Comp.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetComponents4Servers.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetApplications4Comp.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetApps4Env.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetEnv4Apps.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/About.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/Home.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetCalendar.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetGroupMembership.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetUserMembership.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetAdminRights.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetArgs.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetWorkflow.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetProperties.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetPatterns.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetNewsDetails.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetEmailTo.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/jquery.dmcontextmenu.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/edit_area/edit_area_full.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/TestServer.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/om/GetTemplateBody.js?v=${initParam.BUILD_VERSION}"></script>
<script type="text/javascript" src="js/jquery.fileDownload.js?v=${initParam.BUILD_VERSION}"></script>
<script>

// Set during login
var admin=GetCookie("admin");
if (admin == null) admin="N";
isAdmin=admin;

function Stack()
{
	this.stac=new Array();
	this.pop=function() {
 		return this.stac.pop();
 	}
 
 	this.push=function(item) {
  		this.stac.push(item);
 	}
 
	this.shift=function() {
		return this.stac.shift();
 	}
 
	this.prev=function() {
		if (this.stac.length-1 >= 0) return this.stac[this.stac.length-1];
  		return null;
 	} 
}

var breadcrumbs = new Stack();

function getCookie(Name,nodecode) {
 nodecode = nodecode || false;
 var search = Name + "="
 var returnvalue = "";
 if (document.cookie.length > 0) {
   offset = document.cookie.indexOf(search)
   // if cookie exists
   if (offset != -1) { 
     offset += search.length
     // set index of beginning of value
     end = document.cookie.indexOf(";", offset);
     // set index of end of cookie value
     if (end == -1) end = document.cookie.length;
     returnvalue=nodecode?document.cookie.substring(offset,end):unescape(document.cookie.substring(offset, end));
     }
  }
 return returnvalue;
}

String.prototype.replaceAll = function(search, replace)
{
    //if replace is null, return original string otherwise it will
    //replace search string with 'undefined'.
    if(!replace) 
        return this;

    return this.replace(new RegExp('[' + search + ']', 'g'), replace);
};

String.prototype.trimLeft = function(charlist) {
 if (charlist === undefined)
   charlist = "\s";

 return this.replace(new RegExp("^[" + charlist + "]+"), "");
};

String.prototype.trimRight = function(charlist) {
 if (charlist === undefined)
   charlist = "\s";

 return this.replace(new RegExp("[" + charlist + "]+$"), "");
};

String.prototype.trimChars = function(charlist) {
 return this.trimLeft(charlist).trimRight(charlist);
};


 function getDialogButton(dialog_selector, button_name)
 {
  var buttons = $(dialog_selector + ' .ui-dialog-buttonpane button');

  for (var i = 0; i < buttons.length; ++i)
  {
   var jButton = $(buttons[i]);

   if (jButton.text() == button_name)
   {
    return jButton;
   }
  }
  return null;
 }

 $(document).ready(function()
 {
  cPlumb = jsPlumb.getInstance();
  aPlumb = jsPlumb.getInstance();
  avPlumb = jsPlumb.getInstance();
  cvPlumb = jsPlumb.getInstance();
  cisplumb = jsPlumb.getInstance();
  wfPlumb = jsPlumb.getInstance();
  CreateReports();
  $(document).ajaxStart($.blockUI).ajaxStop($.unblockUI);
  $.blockUI.defaults.message = "<p style=\"margin-left:40px\">Loading........</p>";
  $.blockUI.defaults.fadeOut = 200;
  $.blockUI.defaults.theme = true;
  $.blockUI.defaults.themedCSS.width = "15%";

  loginformData = getCookie("logindata",true);
  
  loggedin='N';
  
  if (loginformData != null && loginformData != "")
  {
	  $.ajax({
		  type : "POST",
		  url : "Login?" + loginformData,
		  async : false,
		  dataType: "json"
 		}).done(function(data) {
 			console.log("login data:"+data);
 			console.log(data);
  			if (data == null) {
  				loggedin = 'N';
  			} else if (data.Msg.toLowerCase() == "Login OK".toLowerCase()) {
  				console.log("normal login");
   				loggedin = 'Y';
   				admin = 'N';
  			} else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
  				console.log("1) admin login");
   				loggedin = 'Y';
   				admin = 'Y';
 			}
  			SetCookie("admin",admin);
  			userdatefmt = data.datefmt;
			usertimefmt = data.timefmt;
 	});
  } else {
 	loggedin = 'N';
 	admin = 'N';
 	SetCookie("admin","N");
  }
  
  if ("${firstinstall}" == "Y") {
	  // First install - prompt the user to set admin password and show them
	  // some basic information to get started
	  var l = "<div id=\"firstinstall\">";
	  l+="<table border=\"0\"><tr><td valign=\"top\"><img src=\"css/images/setupdog.png\"></td><td valign=\"top\">";
	  l+="<h2>Set Admin Password</h2>Welcome to DeployHub Open Source Edition. The first thing you need to do is to set the password for the \"admin\" user account. ";
	  l+="Once this is done, you will be logged in as the \"admin\" user and you will be able to create more user accounts ";
	  l+="and set up the objects which will allow you to automate your deployments with ease.<br><br>";
	  l+="<form id=\"firstinstallform\">";
	  l+="<table style=\"width:100%;\" border=\"0\">";
	  l+="<tr>";
	  l+="<td align=\"right\">\Admin Password:</td>";
	  l+="<td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpassword\" name=\"newpassword\" /></td>";
	  l+="</tr>";
	  l+="<tr>";
	  l+="<td align=\"right\">Confirm Admin Password:</td>";
	  l+="<td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpasswordagain\" name=\"newpasswordagain\" /></td>";
	  l+="</tr>";
	  l+="</table>";
	  l+="</form><BR><BR><HR>";
	  l+="</div>";
	  l+="<div class=\"error\"><p class=\"error_txt\" id=\"login_err\"></p></div>";
	  l+="</td></tr></table></div>";
	  $("#modal").html(l);

	  $("#modal").dialog({
	    resizable : false,
	    draggable: false,
	    width : 600,
	    height : 410,
	    modal : true,
	    title : "Welcome to DeployHub Open Source Edition!",
	    open: function(event,ui){
	    	$(".ui-dialog-buttonpane button:contains('Login')").button('disable');
	    }
	   });
	   $("#modal").keyup(function(event) {
		   if(event.keyCode === 13) {
		        DoAdminPasswordSet(this);
		   }
	   });
	   $("#modal").dialog("option", "buttons", [
	   {
	    text : "Login",
	    disabled: true,
	    click : function()
	    {
	     DoAdminPasswordSet(this);
	    }
	   } ]);
	   $("#modal").dialog("open");
	   
	   $("#newpassword").on("keyup", VerifyPW);
	   $("#newpasswordagain").on("keyup", VerifyPW);
  }
  else
  if (loggedin != "Y")
  {
   var l = "<img src=\"images/devops_splash.png\" alt=\"\"  class=\"logindialog\" />"
   l += "<div id=\"loginset\">";
   l += "  <form id=\"loginform\">";
   l += "<table id=\"logintab\" style=\"width: 100%;\" border=\"0\">";
   l += "		<tr style=\"display:none\">";
   l += "			<td class=\"login_label\" align=\"right\">Domain:</td>";
   l += "			<td align=\"left\"><input class=\"inputbox\" type=\"text\" name=\"domain\" /></td>";
   l += "		</tr>";
   l += "		<tr>";
   l += "			<td class=\"login_label\" align=\"right\">Username:</td>";
   l += "			<td align=\"left\"><input class=\"inputbox\" type=\"text\" name=\"username\" /></td>";
   l += "		</tr>";
   l += "		<tr>";
   l += "			<td class=\"login_label\" align=\"right\">Password:</td>";
   l += "			<td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"password\" name=\"password\" /></td>";
   l += "		</tr>";
   l += "		<tr class=\"newpw\" style=\"display:none\">";
   l += "			<td class=\"login_label\" align=\"right\">New Password:</td>";
   l += "			<td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpassword\" name=\"newpassword\" /></td>";
   l += "		</tr>";
   l += "		<tr class=\"newpw\" style=\"display:none\">";
   l += "			<td class=\"login_label\" align=\"right\">Verify New Password:</td>";
   l += "			<td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpasswordagain\" name=\"newpasswordagain\" /></td>";
   l += "		</tr>";
   l += "		<tr>";
   l += "			<td colspan=\"2\" align=\"right\"></td>";
   l += "		</tr>";
   l += "</table>";
   l += "	</form>";
   l += "</div>";
   l += "<div class=\"error\"><p class=\"error_txt\" id=\"login_err\"></p></div>";
   l += "<div class=\"positioncopyright\">";
   l += "	<p class=\"copyright\">Copyright 2014, OpenMake Software. All rights reserved. OpenMake and OpenMake Meister are trademarks or registered trademarks of OpenMake Software.</p>";
   l += "</div>";

   $("#login_container_dialog").html(l);

   $("#login_container_dialog").dialog(
   {
    resizable : false,
    width : 501,
    height : 580,
    modal : true,
    dialogClass : "logindialog"
   });
   $("#login_container_dialog").dialog(
   {
    draggable : false
   });
   $("#login_container_dialog").keyup(function(event) {
	   if(event.keyCode === 13) {
	        DoLogin(this);
	   }
   });
   $("#login_container_dialog").dialog("option", "buttons", [
   {
    text : "Login",
    click : function()
    {
     DoLogin(this);
    }
   } ]);
   $("#login_container_dialog").dialog("open");
   
   $("#newpassword").on("keyup", VerifyPW);
   $("#newpasswordagain").on("keyup", VerifyPW);
  }
  else
  {
   var parts = loginformData.split("&");

   for (var x = 0; x < parts.length; x++)
   {
    if (parts[x].indexOf("username=") >= 0)
     myuserid = parts[x].substring(9);
   }

   breadcrumbs.push("#applications_tree");

   EnableTabs("release_menu");
   if (newUser=="Y") {
	   ShowHelpForNewUsers();
   }
  }
 });

 function VerifyPW()
 {
  var pw1 = $("#newpassword").val();
  var pw2 = $("#newpasswordagain").val();

  if (pw1 != pw2 || pw1=="")
  {
   $(".ui-dialog-buttonpane button:contains('Login')").button('disable');
   if (pw1=="") {
		$("#login_err").html("");
   } else {
		$("#login_err").html("Passwords do not match");
   }
  }
  else
  {
   $(".ui-dialog-buttonpane button:contains('Login')").button('enable');
   $("#login_err").html("");
  }

 }
 
 function DoAdminPasswordSet(dlg)
 {
	 console.log("DoAdminPasswordSet");
	 var newpw = $("#newpassword").val();
	 var loginformData = "domain=&username=admin&initial=Y&password="+encodeURIComponent(newpw);
	 $.post("Login", loginformData, null, "json").done(function(data) {
		console.log("data=");
		console.log(data);
		if (data.Msg.toLowerCase() == "Login OK".toLowerCase()) {
			console.log("Normal login");
			loggedin="Y";
			admin="N";
			isAdmin="N";
			newUser=data.newuser;
			console.log("datefmt="+data.datefmt);
			userdatefmt = data.datefmt;
			usertimefmt = data.timefmt;
		} else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
			console.log("2) admin login");
			loggedin="Y";
			admin="Y";
			isAdmin="Y";
			newUser=data.newuser;
			console.log("datefmt="+data.datefmt);
			userdatefmt = data.datefmt;
			usertimefmt = data.timefmt;
		}
 		if (loggedin=="Y") {
 			SetCookie("admin",admin);
 			console.log("newUser="+newUser);
  			$("#modal").dialog("close");
  			$.sessionTimeout({
   				logoutUrl : "Logout",
   				redirUrl : "Logout",
   				keepAliveUrl : "KeepAlive"
  			});
  			breadcrumbs.push("#releases_tree");
  			document.cookie = "logindata=" + loginformData;
  			EnableTabs("release_menu");
  			if (newUser=="Y") {
  			   ShowHelpForNewUsers();
  		    }
 		} else {
  			$("#login_err").html(data.Msg);
 		}

	 });
 }

 function DoLogin(dlg)
 {
  	var logindlg = parent.$("#login_container_dialog");
  	var form = logindlg.find("#loginform");
  	loginformData = form.serialize();

  	var parts = loginformData.split("&");

  	for (var x = 0; x < parts.length; x++) {
   		if (parts[x].indexOf("username=") >= 0)
    	myuserid = parts[x].substring(9);
  	}

  	if (forcepwchange) {
   		var oldpw = $("#password").val();
   		var newpw = $("#newpassword").val();

   		$.ajax({
   			type : "POST",
    		dataType : 'json',
    		url : "ChangePassword?oldpw=" + encodeURIComponent(oldpw) + "&newpw=" + encodeURIComponent(newpw),
    		async : false
   		}).done(function(data) { 
    		console.log(data.errtext);
    
    		if (data.errtext != "") {
      			$("#login_err").html(data.errtext);
      			loggedin = 'N';
      			admin = 'N';
    		} else {
     			for (var x = 0; x < parts.length; x++) {
      				if (parts[x].indexOf("password=") >= 0)
       				parts[x] = "password=" + newpw;
     			}
     			loginformData = parts.join("&");
     			$.post("Login", loginformData).done(function(data) {
    				if (data.toLowerCase() == "Login OK".toLowerCase()) {
       					loggedin = 'Y';
       					admin = 'N';
    				} else if (data.toLowerCase() == "Login Admin".toLowerCase()) {
    		 			loggedin = 'Y';
    		 			admin = 'N';
    				}
    				if (loggedin = "Y") {
       					parent.$("#login_container_dialog").dialog("close");
    					$.sessionTimeout({
        					logoutUrl : "Logout",
        					redirUrl : "Logout",
        					keepAliveUrl : "KeepAlive"
       					});
       					breadcrumbs.push("#applications_tree");
       					EnableTabs("release_menu");
       					document.cookie = "logindata=" + loginformData;
     				} else { 
       					loggedin = "N";
       					admin = "N";
     				}
     			});
     		} 
     	}); 
    } else {
   		$.post("Login", loginformData, null, "json").done(function(data) {
   			console.log("data="+data);
   			if (data.Msg.toLowerCase() == "Login OK".toLowerCase()) {
   				console.log("Normal login");
   				loggedin="Y";
   				admin="N";
   				isAdmin="N";
   				newUser=data.newuser;
   				console.log("datefmt="+data.datefmt);
   				userdatefmt = data.datefmt;
   				usertimefmt = data.timefmt;
   			} else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
   				console.log("2) admin login");
   				loggedin="Y";
   				admin="Y";
   				isAdmin="Y";
   				newUser=data.newuser;
   				console.log("datefmt="+data.datefmt);
   				userdatefmt = data.datefmt;
   				usertimefmt = data.timefmt;
   			}
    		if (loggedin=="Y") {
    			SetCookie("admin",admin);
     			parent.$("#login_container_dialog").dialog("close");
     			$.sessionTimeout({
      				logoutUrl : "Logout",
      				redirUrl : "Logout",
      				keepAliveUrl : "KeepAlive"
     			});
     			breadcrumbs.push("#applications_tree");
     			EnableTabs("release_menu");
     			document.cookie = "logindata=" + loginformData;
    		} else {
     			if (data.Msg == "Password must be changed") {
     				newUser=data.newuser;
      				forcepwchange = true;
     				$(".ui-dialog-buttonpane button:contains('Login')").button('disable');
      				parent.$("#login_container_dialog").dialog("option", "height", 675);
      				$("#logintab").find("tr.newpw").show();
     			}
     			$("#login_err").html(data.Msg);
    		}
    		if (newUser=="Y") {
    			ShowHelpForNewUsers();
    		}
   		});
  	}

 }

 function ActivateSubTabs()
 {
  console.log("ActivateSubTabs()");
  ShowHome(false,ShowingInitialHelp);

  if (typeof objid == "undefined")
   objid = 1;

  if (currenttree == "#domains_tree" && lastSelectedNode == "")
  {
	  /*
   $("#domains_tree").jstree('select_node', '#do1');
   lastSelectedNode = 'do1';
  	*/
    objid = 1;	//OTID, first two chars are Object Type
  	objtype = "do";
  	objkind="";
  	objtypeAsInt = obj2Int[objtype][0];
   objtypeName = obj2Int[objtype][1];	
   objName = "GLOBAL";
   console.log("id="+objid+" objtype="+objtype + " objtypeAsInt=" + objtypeAsInt + " objtypeName=" + objtypeName + " objName=" + objName + " objkind=" + objkind+ " oldmenu=" + oldmenu + " currenttree=" + currenttree);
   tabId  = "tabs-General";
   $("#tabs-General").addClass("current active");
  }  
  
  var classname = objtypeName.toLowerCase();
  
  console.log("classname="+classname+" currenttree="+currenttree);

  for ( var menuKey in appTabs)
  {
   var tabs = appTabs[menuKey];

   for (var i = 0; i < tabs.length; i++)
   {
    var key = tabs[i];
    var tmp = key + "-data";
    $(key).removeClass('current');
    $(key).removeClass('active');
    $(tmp).hide();
   }
  }

  if (objtypeName == "Notify")
   objtypeName = "Notifier";
  
  if (objtypeName == "Server")
   objtypeName = "End Point";

  if (classname == "application version")
   classname = "appversion";

  if (classname == "release version")
   classname = "relversion";

  if (classname == "component version")
   classname = "compversion";

  if (classname == "component type")
   classname = "comptype";
  
  if (classname == "build engine")
   classname = "builder";
  
  if (classname == "build job")
   classname = "buildjob";
  
  if (classname == "data source")
   classname = "datasource";
  
  if (classname == "end point")
   classname = "server";
  
  if (classname == "user group")
   classname = "usergroup";
  
  console.log(breadcrumbs);
  console.log(currentSubTabsSelection);

  var tabIdNode = currentSubTabsSelection[currenttree];
  var tabId = "";

  if (tabIdNode != null)
   tabId = tabIdNode[0];

  console.log("tabId=#"+tabId);

  if ($("#" + tabId).length > 0)
  {
   $("#" + tabId).addClass('current');
   $("#" + tabId).addClass('active');
  }
  
  console.log("currenttree="+currenttree+" lastSelectedNode.substring(0, 2)="+lastSelectedNode.substring(0, 2));
  console.log("currentSubTab="+currentSubTabsSelection[currenttree]);

  if (currenttree == "#servers_tree" && lastSelectedNode.substring(0,2) == "se")
  {
	$("#tabs-ServerStatus").show();  
  }
  if (currenttree == "#environments_tree")
  {
   if (lastSelectedNode.substring(0, 2) == "se")
   { 
    $("#tabs-Servers").hide();
    $("#tabs-Calendar").hide();
    $("#tabs-Applications").hide();
    $("#tabs-Reports").show();
    $("#tabs-ServerStatus").show();  
    $("#tabs-SrvComponents").show();  
    $("#tabs-EnvDefects").hide();
    if (currentSubTabsSelection["#environments_tree"][0] == "tabs-EnvDefects") {
    	currentSubTabsSelection["#environments_tree"] = ["tabs-General", ""];
        tabId  = "tabs-General";
        $("#tabs-General").addClass("current active");
    }
   }
   else
   {
    $("#tabs-ServerStatus").hide();  
    $("#tabs-SrvComponents").hide();  
    $("#tabs-EnvDefects").show();
   } 
  }
  if (currenttree == "#notifiers_tree" && lastSelectedNode.substring(0, 2) == "te")
  {
   $("#tabs-Access").hide();
   $("#tabs-Properties").hide();
   $("#tabs-Body").show();
   $("#tabs-Recipients").show();
   if (currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Access" || currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Properties" )
   {
    currentSubTabsSelection["#notifiers_tree"] = ["tabs-General", ""];
    tabId  = "tabs-General";
    $("#tabs-General").addClass("current active");
   }
  }
  if (currenttree == "#notifiers_tree" && lastSelectedNode.substring(0, 2) == "no")
  {
   $("#tabs-Access").show();
   $("#tabs-Properties").show();
   $("#tabs-Body").hide();
   $("#tabs-Recipients").hide();
   if (currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Body" || currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Recipients" )
   {
    currentSubTabsSelection["#notifiers_tree"] = ["tabs-General", ""];
    tabId  = "tabs-General";
    $("#tabs-General").addClass("current active");
   }
  }  
  else if (currenttree == "#environments_tree" && lastSelectedNode.substring(0, 2) == "en")
  {
   $("#tabs-Servers").show();
   $("#tabs-Calendar").show();
   $("#tabs-Applications").show();
   $("#tabs-Reports").show();
  }
  else if (currenttree == "#applications_tree" && lastSelectedNode.substring(0, 2) == "av")
  {
   $("#tabs-Versions").hide();
   $("#tabs-Environments").show();	// was hide but need environments tab for app versions
  }
  else if (currenttree == "#applications_tree" && lastSelectedNode.substring(0, 2) == "ap")
  {
   $("#tabs-Versions").show();
   $("#tabs-Environments").show();
  }
  else if (currenttree == "#components_tree" && lastSelectedNode.substring(0, 2) == "cv")
  {
   $("#tabs-ComponentVersions").hide();
  }
  else if (currenttree == "#components_tree" && lastSelectedNode.substring(0, 2) == "co")
  {
   $("#tabs-ComponentVersions").show();
  }
  else if (currenttree == "#procedures_tree" && (lastSelectedNode.substring(0, 2) == "pr" || lastSelectedNode.substring(0, 2) == "fn"))
  {
	  console.log("SWITCHING on "+objkind+" lastSelectedNode="+lastSelectedNode);
	  var lsn = lastSelectedNode.substring(2);
	  // remove the object type (if present)
	  var i=lsn.indexOf("-");
	  console.log("i="+i);
	  if (i>=0)
	  {
	   var parts = lsn.split("-");
	   lsn=parts[0];
	  }
	  console.log("lsn="+lsn);
	  objid=Number(lsn);
	  console.log("objid="+objid+" objkind="+objkind);
	  switch (Number(objkind))
      {
	   case 0:  // undefined (newly created)
	   console.log("Hiding args and procbody");
	    $("#tab-Args").hide();
	    $("#tabs-ProcBody").hide();
	    break;
	    
       case 1: // DMScript Function in repository
       console.log("Showing arguments tab");
        $("#tabs-Args").show();
        $("#tabs-ProcBody").hide();
        break;

       case 2: // DMScript Function in database
        $("#tabs-Args").show();
        $("#tabs-ProcBody").show();
        break;

      case 3: // Function provided by local external script or program
       $("#tabs-Args").show();
       $("#tabs-ProcBody").hide();
       break;

      case 4: // Function provided by remote external script or program
       $("#tabs-Args").show();
       $("#tabs-ProcBody").hide();
       break;

      case 5: // Function provided by plugin
       $("#tabs-Args").hide();
       $("#tabs-ProcBody").hide();
       break;

      case 6: // Graphical deployment flow
       $("#tabs-Args").hide();
       $("#tabs-ProcBody").hide();
      default:
       break;
      }  
  }
  else if (currenttree == "#builders_tree" && lastSelectedNode.substring(0, 2) == "be")
  {
	  $("#tabs-General").show();
	  $("#tabs-Builds").hide();
	  $("#tabs-Properties").show();
	  $("#tabs-Access").show();
	  if (tabId == "tabs-Builds") {
		  // Switching from a Build Job "Builds" tab back to the build engine
		  tabId="tabs-General";
		  currentSubTabsSelection["#builders_tree"] = ["tabs-General", ""];
	      $("#tabs-General").addClass("current active");
	  }
  }
  else if (currenttree == "#builders_tree" && lastSelectedNode.substring(0, 2) == "bj")
  {
	  console.log("switching to build job tabId="+tabId);
	  $("#tabs-General").show();
	  $("#tabs-Builds").show();
	  $("#tabs-Access").hide();
	  $("#tabs-Properties").hide();
	  if (tabId != "tabs-Builds") {
		  tabId="tabs-General";
		  currentSubTabsSelection["#builders_tree"] = ["tabs-General", ""];
	      $("#tabs-General").addClass("current active");
	  }
  }
  
  console.log("tabId="+tabId+" currenttree="+currenttree+" lastSelectedNode="+lastSelectedNode+" objtype="+objtype);
  
	  
  var isAdmin = "N";
  if (canView[currenttree])
   isAdmin = "Y";

  switch (tabId)
  {
  case "tabs-General":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree") || (objtype == "do" && currenttree == "#domains_tree") || (objtype == "pr" && currenttree == "#procedures_tree")
     || (objtype == "fn" && currenttree == "#procedures_tree") || (objtype == "us" && currenttree == "#users_tree") || (objtype == "gr" && currenttree == "#groups_tree")
     || ((objtype == "no"  || objtype == "te") && currenttree == "#notifiers_tree") || (objtype == "ac" && currenttree == "#actions_tree") || (objtype == "cr" && currenttree == "#credentials_tree")
     || (objtype == "ds" && currenttree == "#datasources_tree") || (objtype == "re" && currenttree == "#repositories_tree") || (objtype == "se" && currenttree == "#servers_tree")
     || ((objtype == "en" || objtype == "se") && currenttree == "#environments_tree") || (objtype == "ct" && currenttree == "#types_tree") || (objtype == "be" && currenttree == "#builders_tree")
     || (objtype == "bj" && currenttree == "#builders_tree"))
   {
	   console.log("doing general - objtype="+objtype+" currenttree="+currenttree);
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");

    $("#title_icon").html("<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false)\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>");
    $("#tabs-General-data").html("<table id=\"summ\" class=\"dev_table\"><tbody></tbody></table>");
    console.log("LoadSummaryData, objtypeAsInt="+objtypeAsInt+" objtype="+objtype);
    LoadSummaryData("summ", objtypeAsInt, objtype, objid, objtype=="bj"?"&be="+lastSelectedNode:"");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#tabs-General-data").html("");
    $("#panel_container_right").hide();
   }
   break;
  case "tabs-Builds":
   if (objtype=='bj' && currenttree == "#builders_tree") {
	   console.log("Loading builds");
	   LoadBuildData("BuildHistory", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#tabs-General-data").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Calendar":
   if (objtype == "en" && currenttree == "#environments_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html("");
    CreateCalendar(objid);
   }
   else
   {
    if (objtype == "se")
    {
     currentSubTabsSelection[currenttree][0] = "tabs-General";
     ActivateSubTabs()
     return;
    }

    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Servers":
   if ((objtype == "en" && currenttree == "#environments_tree") || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"add_button\" onClick=\"javascript:AddSrv4EnvButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteSrv4EnvButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadSrv4EnvData("srv4env", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    if (objtype == "se")
    {
     currentSubTabsSelection[currenttree][0] = "tabs-General";
     ActivateSubTabs();
     return;
    }

    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;
   
  case "tabs-ServerStatus":
	  LoadServerStatusData("servstat", objtypeAsInt,objid);
	  parent.$("#title_icon").html("<button class=\"test_button\" onClick=\"javascript:TestServer('servstat',"+objtypeAsInt+","+objid+")\">Test Now</button>");
	  break;

  case "tabs-CompServers":
   if ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
 //   $("#title_icon").html(
 //     "<button class=\"add_button\" onClick=\"javascript:AddSrv4CompButton(objtype=='cv')\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
 //       + "<button class=\"delete_button\" onClick=\"javascript:DeleteSrv4CompButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadSrv4CompData("srv4comp", objtypeAsInt, objtype, objid, "");
    LoadBuildDataForComponent("compbuilds", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-SrvComponents":
   if ((objtype == "se" && currenttree == "#servers_tree") || (objtype == "se" && currenttree == "#environments_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
        "<button class=\"add_button\" onClick=\"javascript:AddCompVersion4SrvButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>" +
        "<button class=\"delete_button\" onClick=\"javascript:DeleteComp4SrvButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>" );

    LoadComp4SrvData("comp4srv", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-CompDefects":
	if ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree") {
		$("#panel_container_right").show();
		if (objtype == "co")
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
		else
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
		parent.$("#title_icon").html("");

	    $("#title_icon").html(
	        "<button class=\"add_button\" onClick=\"javascript:AddDefectButton(false)\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
	        + "<button class=\"delete_button\" onClick=\"javascript:DeleteDefectButton(false)\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");
	    
	    LoadDefect4CompData("defects4comp", objtypeAsInt, objtype, objid, "");
	    LoadDefectHistory("defecthistory4comp", objtypeAsInt, objtype, objid);
	} else {
		parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
		parent.$("#title_icon").html("");
		$("#panel_container_right").hide();
	}
	break;
  case "tabs-AppDefects":
	if ((objtype == "ap" || objtype == "av") && currenttree == "#applications_tree") {
		$("#panel_container_right").show();
		if (objtype == "ap")
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
		else
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
		parent.$("#title_icon").html("");

	    $("#title_icon").html(
	        "<button class=\"add_button\" onClick=\"javascript:AddDefectButton(true)\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
	        + "<button class=\"delete_button\" onClick=\"javascript:DeleteDefectButton(true)\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");
	    
	    LoadDefect4AppData("defects4app", objtypeAsInt, objtype, objid, "");
	    LoadDefectHistory("defecthistory4app", objtypeAsInt, objtype, objid);
	} else {
		parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
		parent.$("#title_icon").html("");
		$("#panel_container_right").hide();
	}
	break;
  case "tabs-EnvDefects":
	  // console.log("tabs-EnvDefects, objtype=["+objtype+"] currenttree=["+currenttree+"]");
		if ((objtype == "en") && currenttree == "#environments_tree") {
			$("#panel_container_right").show();	
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
			parent.$("#title_icon").html("");
		    LoadDefect4EnvData("defects4env", objtypeAsInt, objtype, objid, "");
		} else {
			parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
			parent.$("#title_icon").html("");
			$("#panel_container_right").hide();
		}
		break;
  case "tabs-CompApplications":
   if ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    
    $("#title_icon").html("");


    LoadApp4CompData("app4comp", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-GroupMembership":
   if (objtype == "us" && currenttree == "#users_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"add_button\" onClick=\"javascript:AddGroup2UserButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteGroupFromUserButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadGroupMembershipData("groupmembership", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-UserMembership":
   if (objtype == "gr" && currenttree == "#groups_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"add_button\" onClick=\"javascript:AddUser2GroupButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteUserFromGroupButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadUserMembershipData("usermembership", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Args":
   
 	   console.log("tabs-Args objtype="+objtype+" objkind="+objkind);
	   if ((objtype == "fn" || objtype == "pr") && currenttree == "#procedures_tree")
	   {
	    $("#panel_container_right").show();
	    if (objtype == "ap" || objtype == "co")
	     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
	    else
	     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
	    $("#inputparamsh1").html("<h1 style=\"display:inline;margin-right:35em;\">Inputs to this "+((objtype == "pr")?"procedure":"function")+"</h1><div id=\"rightbuttonsarg\" style=\"float:right\">"
	        + "<button class=\"edit_button\" onClick=\"javascript:EditArgsButton('switch')\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>"
	        + "<button class=\"add_button\" onClick=\"javascript:AddArgsButton('switch')\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
	        + "<button class=\"delete_button\" onClick=\"javascript:DeleteArgsButton('switch')\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>"
	        + "<button class=\"up_button\" onClick=\"javascript:UpArgsButton()\"><img alt=\"Edit\" src=\"css/images/up.png\" height=\"22\" width=\"18\"></button>"
	        + "<button class=\"down_button\" onClick=\"javascript:DownArgsButton()\"><img alt=\"Edit\" src=\"css/images/down.png\" height=\"22\" width=\"18\"></button></div>");

	    if (objkind != 1 && objkind != 2) {
	   		 $("#argsh1").html("<h1 style=\"display:inline;margin-right:25em;\">Additional command line switches for program below</h1><div id=\"rightbuttonsswitch\"style=\"float:right\">" 
       		+ "<button class=\"edit_button\" onClick=\"javascript:EditSwitchButton()\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>"
       		+ "<button class=\"add_button\" onClick=\"javascript:AddSwitchButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
       		+ "<button class=\"delete_button\" onClick=\"javascript:DeleteSwitchButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button></div>");
	    	parent.$("#title_icon").html("");
	    	console.log("HOME objid="+objid);
	    	$("#dragtextdiv").html("Drag the arguments from here onto the command line below to assemble the command. Use the table above to add and configure the switches/arguments.");
			$("#argstablediv").html("<table id=\"argstable\" cellspacing=\"0\" width=\"100%\" ><thead class=\"ui-widget-header\"><tr><th>Additional Command Line Switch</th></tr></thead><tbody></tbody></table>");
			$("#inputparamsdiv").html(
							"<table id=\"inputparamstable\" cellspacing=\"0\" width=\"100%\">"
						+ 	"<thead class=\"ui-widget-header\"><tr><th>Name</th><th>Type</th><th style=\"display:none\">Mode</th><th>Present</th><th>Missing</th><th>Pad</th><th>Required</th></tr>"
	           			+	"</thead><tbody></tbody></table>");
	    } else {
	    	$("#argsh1").html("");
	    	$("#dragtextdiv").html("");
	    	$("#argstablediv").html("");
	    	$("#inputparamsdiv").html(
					"<table id=\"inputparamstable\" cellspacing=\"0\" width=\"100%\">"
				+ 	"<thead class=\"ui-widget-header\"><tr><th>Name</th><th>Type</th><th>Required</th></tr>"
       			+	"</thead><tbody></tbody></table>");
	    }
	    LoadArgsData("argstable", objtypeAsInt, objtype, objid, objkind, "");

	   }
	   else
	   {
	    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
	    parent.$("#title_icon").html("");
	    $("#panel_container_right").hide();
	   }
   break;
   
  case "tabs-ProcBody":
	  if (objkind == 1 || objkind == 2) {
		  console.log("tabs-ProcBody objtype="+objtype+" objkind="+objkind);
		   if ((objtype == "fn" || objtype == "pr") && currenttree == "#procedures_tree")
		   {
		    $("#panel_container_right").show();
		    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
		    $("#title_icon").html("<button class=\"edit_button\" onClick=\"javascript:EditProcButton("+objid+",false)\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>");
		   }
		   else
		   {
		    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
		    parent.$("#title_icon").html("");
		    $("#panel_container_right").hide();
		   }
		   LoadProcBody("ProcBodyTextArea",objid,false,false);
	  } else {
		  currentSubTabsSelection[currenttree][0] = "tabs-General";
		  ActivateSubTabs();
		  return;
	  }
	   break;

  case "tabs-Properties":
   if ((objtype == "ds" && currenttree == "#datasources_tree") || (objtype == "re" && currenttree == "#repositories_tree") 
		|| (objtype == "no" && currenttree == "#notifiers_tree") || (objtype == "be" && currenttree == "#builders_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"edit_button\" onClick=\"javascript:EditPropertiesButton()\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>"
        + "<button class=\"add_button\" onClick=\"javascript:AddPropertiesButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeletePropertiesButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadPropertiesData("propstable", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Recipients":
   if ((objtype == "te") && currenttree == "#notifiers_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html("<button class=\"add_button\" onClick=\"javascript:AddEmailToButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteEmailToButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadEmailToData("emailtotable", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;
   
  case "tabs-Body":
   if ((objtype == "te") && currenttree == "#notifiers_tree")
   {
	$("#helppanel").show();
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"edit_button\" onClick=\"javascript:EditBodyButton()\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>");
    LoadBodyData("bodytable", objtypeAsInt, objtype, objid, "");
   }
   else
   {
	$("#helppanel").hide();
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;
   
  case "tabs-Patterns":
   if ((objtype == "re" && currenttree == "#repositories_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"edit_button\" onClick=\"javascript:EditPatternsButton()\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>"
        + "<button class=\"add_button\" onClick=\"javascript:AddPatternsButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeletePatternsButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadPatternsData("patternstable", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Applications":
   if ((objtype == "en" && currenttree == "#environments_tree") || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"add_button\" onClick=\"javascript:AddApp4EnvButton()\"><img alt=\"Add\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteApp4EnvButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");
      //  + "<button class=\"add_button\" onClick=\"javascript:AddAppVer4EnvButton()\"><img alt=\"Add\" src=\"css/images/add_22x22.png\"></button>");

    LoadApp4EnvData("App4Env", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    if (objtype == "se")
    {
     currentSubTabsSelection[currenttree][0] = "tabs-General";
     ActivateSubTabs()
     return;
    }

    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#Env4App > tbody").html("");
    // parent.$("#PendingApp4Env > tbody").html("");
    parent.$("#PendingApp4Env").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Components":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree") || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");
    console.log("GetComponents?appid=" + objid);
    LoadComponentsData();
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-Components-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container compversions\">" + "<div id=\"innercomp\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
        + "<div class=\"accordionpanel\" id=\"ag\">" + "<h4 class=\"ui-widget-header\" style=\"margin:0\" >Component Versions</h4><div id=componentlist align=\"left\"</div>" + "</div>" + "</div>");

    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-ApplicationVersions":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");

    LoadApplicationVersionsData();
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-ApplicationVersions-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container appversions\">" + "<div id=\"innerappver\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
      + "<div class=\"accordionpanel\" id=\"ag\">" + "<h4 class=\"ui-widget-header\" style=\"margin:0\" >Application Versions</h4><div id=applicationversionlist align=\"left\"</div>" + "</div>" + "</div>");
  
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Workflow":
   if (objtype == "ac" && currenttree == "#actions_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");

    LoadWorkflowsData("");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-Workflow-data").html(
    "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container fragments\">" + "<div id=\"innerworkflow\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
    + "<div class=\"accordionpanel\" id=\"wg\">" + "<h4 class=\"ui-widget-header\" style=\"margin:0\" >Activities</h4><div id=fragmentlist align=\"left\"</div>" + "</div>" + "</div>");

    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Versions":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "rl")
    {
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
     parent.$("#title_icon").html("");

     isRelease = 'N';

     if (objtype == "rl")
      isRelease = 'Y';

     LoadVersionsData();
    }
    else
    {
     if (objtype == "av" || objtype == "rv")
     {
      currentSubTabsSelection[currenttree][0] = "tabs-General";
      ActivateSubTabs()
      return;
     }

     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
     parent.$("#title_icon").html("");
     $("#innerversions").html("No Versions available");
    }
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-Versions-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container appversions\">" + "<div id=\"innerversions\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
        + "</div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-ComponentVersions":
   if ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "co")
    {
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
     parent.$("#title_icon").html("");

     LoadComponentVersionsData();
    }
    else
    {
     if (objtype == "cv")
     {
      currentSubTabsSelection[currenttree][0] = "tabs-General";
      ActivateSubTabs()
      return;
     }
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
     parent.$("#title_icon").html("");
     $("#innercompversions").html("No Versions available");
    }
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-ComponentsVersions-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container compversions\">" + "<div id=\"innercompversions\" style=\"height:600px;width:100%;\">" + "</div>"
        + "</div>" + "</div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-ComponentItems":
   if ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree")
   {
    $("#panel_container_right").show();

    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    parent.$("#title_icon").html("");

    LoadComponentItemsData();
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-ComponentsItems-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container compitems\">" + "<div id=\"inneritem\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
        + "</div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Environments":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    $("#title_icon").html(
      "<button class=\"add_button\" onClick=\"javascript:AddEnv4AppButton((objtype=='av' || objtype=='rv')?true:false)\"><img alt=\"Add\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteEnv4AppButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");

    LoadEnv4AppData("Env4App", objtypeAsInt, objtype, objid, "");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#Env4App > tbody").html("");
    // parent.$("#PendingEnv4App > tbody").html("");
    parent.$("#PendingEnv4App").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Attributes":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || (objtype == "no" && currenttree == "#notifiers_tree") || (objtype == "re" && currenttree == "#repositories_tree")
     || ((objtype == "en" || objtype == "se") && currenttree == "#environments_tree") || (objtype == "se" && currenttree == "#servers_tree") || (objtype == "ds" && currenttree == "#datasources_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");
    $("#title_icon").html(
      "<button class=\"edit_button\" onClick=\"javascript:EditAttributeButton()\"><img alt=\"Edit\" src=\"images/pencil_24x22.png\"></button>"
        + "<button class=\"add_button\" onClick=\"javascript:AddAttributeButton()\"><img alt=\"Edit\" src=\"css/images/add_22x22.png\"></button>"
        + "<button class=\"delete_button\" onClick=\"javascript:DeleteAttributeButton()\"><img alt=\"Edit\" src=\"css/images/delete_22x22.png\"></button>");
    LoadAttributesData();
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Tasks":
   $("#tabs-Tasks-data").html("");
   if (objtype == "ap" || objtype == "co")
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
   else
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
   parent.$("#title_icon").html("");
   console.log("DomainDetails?a=" + isAdmin + "&domainid=" + objid);
   $("#tabs-Tasks-data").load("DomainDetails?a=" + isAdmin + "&domainid=" + objid);
   $("#panel_container_right").show();
   break;

  case "tabs-AdminRights":
   if (objtype == "gr" && currenttree == "#groups_tree")
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");
    LoadAdminRightsData(objtypeAsInt, objtype, objid);
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-Access":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree") || (objtype == "do" && currenttree == "#domains_tree") || (objtype == "pr" && currenttree == "#procedures_tree")
     || (objtype == "no" && currenttree == "#notifiers_tree") || (objtype == "fn" && currenttree == "#procedures_tree") || (objtype == "ac" && currenttree == "#actions_tree")
     || (objtype == "re" && currenttree == "#repositories_tree") || (objtype == "cr" && currenttree == "#credentials_tree") || (objtype == "se" && currenttree == "#servers_tree")
     || (objtype == "ds" && currenttree == "#datasources_tree") || ((objtype == "en" || objtype == "se") && currenttree == "#environments_tree") || (objtype == "ct" && currenttree == "#types_tree") || (objtype =="be" && currenttree == "#builders_tree"))
   {
    $("#panel_container_right").show();
    $("#tabs-Access-data").html("");
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");

    console.log("GetAccess?objtype=" + objtypeAsInt + "&id=" + objid);
    if (objtype == "do")
     $("#tabs-Access-data").load("GetAccess?objtype=" + objtypeAsInt + "&id=" + objid + "&din=Y");
    else
     $("#tabs-Access-data").load("GetAccess?objtype=" + objtypeAsInt + "&id=" + objid + "&din=");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-Access-data").html("");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;
   
  case "tabs-Reports":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || (objtype == "se" && currenttree == "#servers_tree") || ((objtype == "se" || objtype == "en") && currenttree == "#environments_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");
    ReplotReports(objtype);
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  case "tabs-History":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
     || (objtype == "us" && currenttree == "#users_tree") || (objtype == "se" && currenttree == "#servers_tree") || (objtype == "re" && currenttree == "#repositories_tree")
     || (objtype == "en" && currenttree == "#environments_tree") || (objtype == "se" && currenttree == "#environments_tree")
     || (objtype == "ac" && currenttree == "#actions_tree") || ((objtype == "fn" || objtype == "pr") && currenttree == "#procedures_tree")
     || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree")
     || (objtype == "gr" && currenttree == "#groups_tree"))
   {
    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + "  Base Version:  " + objName + "</h1>");
    else
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div><h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    parent.$("#title_icon").html("");
    switch (objtype) {
    case 'av':
    case 'ap':
    	objname='Application';
    	break;
    case 'rl':
    case 'rv':
    	objname='Release';
    	break;
    case 'us':
    	objname='User';
    	break;
    case 'se':
    	objname='End Point';
    	break;
    case 're':
    	objname='Repository';
    	break;
    case 'en':
    	objname='Environment';
    	break;
    case 'co':
    case 'cv':
    	objname='Component';
    	break;
    case 'ac':
    	objname="Action";
    	break;
    case 'pr':
    	objname="Procedure";
    	break;
    case 'fn':
    	objname="Function";
    	break;
    case 'gr':
    	objname="Group";
    	break;
    default:
    	objname="";
    	break;
    }
    $("#tabs-History-data").html("<div id='historyadd' style='width:96%;height:100px;display:inline'></div><table id=\"histfeed\" class=\"dev_table\"><tbody></tbody></table>");
    $('#historyadd').createNewsMessageBox('#histfeed', 'Say something about this '+objname+'?', objtype+objid);
    createNewsFeed("histfeed", objtype, objid, false);
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#panel_container_right").hide();
   }
   break;

  default:
   break;
  }

  $("#" + tabId + "-data").show();

  if (tabId == "tabs-Reports")
  {
   plot1.replot();
   plot2.replot();
  }
  if (tabId == "tabs-Calendar") {
	  console.log(">>>>>> TABS RENDER");
	  console.trace();
   		$('#calendar').fullCalendar('render');
   }
  
  /*
  if (currenttree != "#domains_tree" && lastSelectedNode.substring(0, 2) == "do")
  {
   // $("#panel_container_right").hide();
      $("#panel_container_right").show();
	  displayhome=true;
	  ShowHome(false);
	  displayhome=false;
  }
  */
  if (typeof lastSelectedNode == "undefined" || lastSelectedNode=="") {
	  $("#panel_container_right").show();
	  displayhome=true;
	  ShowHome(false,ShowingInitialHelp);
	  displayhome=false;
  }
  console.log($("#panel_container_right"));
 }

 function SetActiveMenu(menuname)
 {
	 console.log("SetActiveMenu("+menuname+")");
  if ($("#" + oldmenu).length > 0)
  {
   $("#" + oldmenu).removeClass('current');
   $("#" + oldmenu).removeClass('active');
  }

  $("#" + menuname).addClass('current');
  $("#" + menuname).addClass('active');
  var tabname = menu2tree[menuname];

  SetActive(tabname);
  oldmenu = menuname;
 }

 function SetTabActive(menuid,tabname)
 {
	console.log("SetTabActive("+tabname+")");
	if (oldsubtabmenu[menuid].length > 0) {
		$("#" + oldsubtabmenu[menuid]).removeClass('current');
		$("#" + oldsubtabmenu[menuid]).removeClass('active');
		console.log("Hiding #" + oldsubtabmenu+"-data");
		$("#" + oldsubtabmenu[menuid]+"-data").hide();
	}
	$("#"+tabname).addClass('current');
	$("#"+tabname).addClass('active');
	oldsubtabmenu[menuid] = tabname;
	if (tabname=='displaylogtabs-log' || tabname=='displaylogtabs-files'  || tabname=='displaylogtabs-general' || tabname=='displaylogtabs-scripts') {
		if (!depComplete) $("#deprefresh").show();
		RefreshLogDetail(saveLognum);
	} else {
		$("#deprefresh").hide();
	}
	$("#"+tabname+"-data").show();
	console.log("Showing #" + tabname+"-data");
	
	if (tabname=="displaylogtabs-reports") {
		// May need to call replot here
		CreateDeployReports();
		ReplotDeployReports(saveLognum); 
	}
	if (tabname=="displaylogtabs-deploydeps") {
		// May need to call replot here
		CreateDeployDeps(saveLognum); 
	}
 }
 
 function SetActive(tabname)
 {
	 explorerFlasher = false;	// switch off highlighting if in help mode
	 lastSelectedNode="";
	 console.log("SetActive("+tabname+") oldtabmenu="+oldtabmenu);
  if (oldtabmenu.length > 0)
  {
   $("#" + oldtabmenu).removeClass('current');
   $("#" + oldtabmenu).removeClass('active');
  }

  $("#" + tabname).addClass('current');
  $("#" + tabname).addClass('active');
  var itemname = tabname.substring(0, tabname.length - 4);
  var treename = itemname + "_tree";

  ad_currenttree = "ad_" + treename;
  currenttree = "#" + treename;

  oldtabmenu = tabname;

  if (oldtreename.length > 0)
  {
   $(oldtreename).hide();
   $(oldtreename).jstree("destroy");
   $(currenttree).show();
  }

  menu2tree[currentmenu] = tabname;

  HideSubTabs(oldtreename, currenttree);

  // CreateTree(currenttree, itemname, canView[currenttree], "mainframe3");
  console.log("Calling CreateTree(\""+currenttree+"\",\""+itemname+"\",\""+admin+"\", \"mainframe3\"");
  CreateTree(currenttree, itemname, admin, "mainframe3");
  
  // $("#footer_container").html("");

  ActivateSubTabs();

  breadcrumbs.push(currenttree);
  oldtreename = currenttree;
 }

 function HideSubTabs(oldtreename, currenttree)
 {
  var tabs = appTabs[oldtreename];
  for ( var i in tabs)
  {
	  console.log("Hiding tab "+tabs[i]);
   $(tabs[i]).hide();
   $("#right-panel-main").hide();
  }
  if (oldtreename == "#environments_tree") {
	  // Might have server selected...
	  tabs = appTabs["#servers_tree"];
	  for (var s in tabs) {
		  console.log("Hiding server tab "+tabs[s]);
		  $(tabs[s]).hide();
	  }
  }

  tabs = appTabs[currenttree];
  for ( var k in tabs)
  {
   $(tabs[k]).show();
  }
 }

 function FlashViews()
 {
	 // If user clicks BACK to go from flashing JUST Deploy View to flashing ALL views you
	 // can get into an odd effect whereby the Deploy View flashes out of sequence with the
	 // other shortcuts. So wait for dvFlashInProgress to drop to false; 
	 if (viewFlasher) {
		if (dvFlashInProgress) {
			// Still in the process of flashing DeployView
			setTimeout(FlashViews,500);
		} else {
			$("#release_menu").fadeIn(500).fadeOut(500).fadeIn(500);
			$("#endpoints_menu").fadeIn(500).fadeOut(500).fadeIn(500);
			$("#deploy_menu").fadeIn(500).fadeOut(500).fadeIn(500);
			$("#containers_menu").fadeIn(500).fadeOut(500).fadeIn(500);
			$("#domains_menu").fadeIn(500).fadeOut(500).fadeIn(500);
			$("#usersgroups_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
				FlashViews();
			});
		}
	 }
 }
 
 function FlashDeployView()
 {
	 if (deployFlasher) {
		 console.log("setting dvFlashInProgress=true");
		 dvFlashInProgress=true;
		 $("#release_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 console.log("setting dvFlashInProgress=false");
			 dvFlashInProgress=false;
			 FlashDeployView();
		 });
	 }
 }
 
 function FlashDataCenterView()
 {
	 if (dcFlasher) {
		 $("#endpoints_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashDataCenterView();
		 });
	 }
 }
 
 function FlashConnectionsView()
 {
	 if (connFlasher) {
		 $("#containers_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashConnectionsView();
		 });
	 }
 }
 
 function FlashFlowView()
 {
	 if (flowFlasher) {
		 $("#deploy_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashFlowView();
		 });
	 }
 }
 
 function FlashDomainsView()
 {
	 if (domFlasher) {
		 $("#domains_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashDomainsView();
		 });
	 }
 }
 
 function FlashUsersView()
 {
	 if (uagFlasher) {
		 $("#usersgroups_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashUsersView();
		 });
	 }
 }
 
 function FlashExplorerPanel()
 {
	 if (explorerFlasher) {
		 $("#left_panel_tabs").delay(500).fadeOut(500).fadeIn(200);
		 $("#left_panel_tree").delay(500).fadeOut(500).fadeIn(200,function() {
			 FlashExplorerPanel();
		 });
	 }
 }
 
 function FlashHomeIcon()
 {
	 if (homeFlasher) {
		 $("#home_menu").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashHomeIcon();
		 });
	 }
 }
 
 function FlashBreadcrumbPanel()
 {
	 if (breadcrumbFlasher) {
		 $("#footer_container").fadeIn(500).fadeOut(500).fadeIn(500,function() {
			 FlashBreadcrumbPanel();
		 });
	 }
 }
 
 function clearAllFlashFlags()
 {
	explorerFlasher = false;
	viewFlasher = false;
	deployFlasher = false;
	dcFlasher = false;
	flowFlasher = false;
	connFlasher = false;
	domFlasher = false;
	uagFlasher = false;
	homeFlasher = false;
	breadcrumbFlasher = false;
 }
 
 
 function ShowHelpPage(pageno)
 {
	 console.log("ShowHelpPage("+pageno+")");
	 HelpPageNumber=pageno;
 	$('div[id^="helppage"]').hide();
 	$("#helppage"+pageno.toString()).show();
 	if (pageno==1) {
 		$("#modal").dialog("option", "buttons", [
      	   {
      	    text : "Yes, Please!",
      	    click : function()
      	    {
      	    	HelpPageNumber++;
      	    	ShowHelpPage(HelpPageNumber);
      	    }
      	   },{
      		    text : "No, thanks!",
      		    click : function()
      		    {
      		    	$("#modal").dialog("close");
      		    	ShowingInitialHelp=false;
      		    	clearAllFlashFlags();
      		    }
      		 }
      	   ]);
 	}
 	else if (pageno>1 && pageno<12) {
		$("#modal").dialog("option", "buttons",
		[{
       	    text : "Back",
       	    click : function() {
       	    	HelpPageNumber--;
       	    	ShowHelpPage(HelpPageNumber);
            }
         },{
            text : "Next",
			click : function() {
				HelpPageNumber++;
				ShowHelpPage(HelpPageNumber);
			}
         },{
        	text : "Finish",
 			click : function() {
 				$("#modal").dialog("close");
 		    	ShowingInitialHelp=false;
 		    	clearAllFlashFlags();
 			}
         }
		]);
 	} else {
 		$("#modal").dialog("option", "buttons", [
       	   {
       	    text : "Back",
       	    click : function()
       	    {
       	    	HelpPageNumber--;
       	    	ShowHelpPage(HelpPageNumber);
       	    }
       	   },{
       		    text : "Finish",
       		    click : function()
       		    {
       		    	$("#modal").dialog("close");
       		    	ShowingInitialHelp=false;
       		    	clearAllFlashFlags();
       		    }
       		 }
       	   ]);	
 	}
 	clearAllFlashFlags();
 	switch(pageno) {
 	case 2:
 		explorerFlasher = true;
 		FlashExplorerPanel();
 		break;
 	case 3:
 		viewFlasher = true;
 		FlashViews();
 		break;
 	case 4:
 		deployFlasher = true;
 		FlashDeployView();
 		EnableTabs("release_menu",true);
 		break;
 	case 5:
 		dcFlasher = true;
 		FlashDataCenterView();
 		EnableTabs("endpoints_menu",true);
 		break;
 	case 6:
 		flowFlasher = true;
 		FlashFlowView();
 		EnableTabs("deploy_menu",true);
 		break;
 	case 7:
 		connFlasher = true;
 		FlashConnectionsView();
 		EnableTabs("containers_menu",true);
 		break;
 	case 8:
 		domFlasher = true;
 		FlashDomainsView();
 		EnableTabs("domains_menu",true);
 		break;
 	case 9:
 		uagFlasher = true;
 		FlashUsersView();
 		EnableTabs("usersgroups_menu",true);
 		break;
 	case 10:
 		homeFlasher = true;
 		FlashHomeIcon();
 		displayhome=true;
 		ShowHome(true,true);
 		break;
 	case 11:
 		breadcrumbFlasher = true;
 		FlashBreadcrumbPanel();
 		break;
 	}
 }

 function ShowHelpForNewUsers()
 {
	 // Open up the help dialog for new users
	 ShowingInitialHelp=true;
	 newUser=false;
	 var w1="<table border=\"0\"><tr><td valign=\"top\"><img src=\"css/images/setupdog.png\"></td><td valign=\"top\">";
	 var w2="</td></tr></table></div>";
	 var t="<div id=\"helppage1\">"+w1;
	  t+="<h2>Hello!</h2>It looks like you've not logged into DeployHub before. Do you want to have an overview of how DeployHub works ";
	  t+="and how to navigate the User Interface? If you click \"Yes Please!\" you will be shown each major UI component. You can drag ";
	  t+="this dialog around the screen and interact with the DeployHub UI and this dialog will show you information about what you ";
	  t+="have clicked on.";
	  t+=w2;
	  t+="<div id=\"helppage2\" style=\"display:none\">"+w1;
	  t+="<h2>Explorer Panel</h2>The flashing panel on the left of the screen is the <I>Explorer</I>. Nearly all navigation is done from here. The explorer panel shows the ";
	  t+="domain hierarchy starting from your <I>Home Domain</I>. The objects shown in the explorer panel are controlled by the ";
	  t+="tabs at the top of the explorer panel as well as the currently selected <I>View</I>. We will discuss views on the next ";
	  t+="page.<BR><BR>You can generally bring up a <I>Context Menu</I> by selecting an object in the Explorer Panel and right-clicking. ";
	  t+="To create new objects of a specified type you need to switch the Explorer Panel to show objects of that type, right-click on ";
	  t+="the Domain in which you want to create the object and select \"Create New...\"";
	  t+=w2;
	  t+="<div id=\"helppage3\" style=\"display:none\">"+w1;
	  t+="<h2>Views</h2>The flashing links across the top of the screen switch between different <I>views</I>. Each view shows information about ";
	  t+="different DeployHub objects. You can click on one of these views for a description of what the view contains, or just click ";
	  t+="\"Next\" or \"Back\" to walk through the views one by one."
	  t+=w2;
	  t+="<div id=\"helppage4\" style=\"display:none\">"+w1;
	  t+="<h2>Deploy View</h2>The <I>Deploy</I> view contains all of the Applications and Components. Applications are ";
	  t+="made up of one or more Components. You assemble an Application by dragging Components onto a panel that represents the ";
	  t+="deployment sequence.<br><br>";
	  t+="Applications are deployed to Environments. Environments are configured via the <I>Data Center</I> view.";
	  t+="Applications and Components can be versioned. This allows DeployHub to know which version of the application has been ";
	  t+="deployed to which Environment and to handle the roll-forward and roll-back logic.";
	  t+=w2;
	  t+="<div id=\"helppage5\" style=\"display:none\">"+w1;
	  t+="<h2>Data Center View</h2>The <I>Data Center</I> view contains all of the End Points, Environments and Build Engines that ";
	  t+="DeployHub connects to. Applications are deployed to Environments. An Environment is made up of one or more End Points. An ";
	  t+="End Point can be a physical or virtual server, a container or any other device that receives changes (such as a router or ";
	  t+="load balancer). Each End Point has <I>Component Types</I> associated with it. When an Application is deployed, its components ";
	  t+="are delivered to the appropriate end-points within the targeted Environment";
	  t+=w2;
	  t+="<div id=\"helppage6\" style=\"display:none\">"+w1;
	  t+="<h2>Flows View</h2>The <I>Flows</I> view contains all the Actions, Procedures and Functions that control the deployment logic. ";
	  t+="Actions are created with a drag-and-drop graphical editor to build workflow logic. Procedures and Functions can be written ";
	  t+="to run on the target end-point or locally on the DeployHub server. Local scripts can also be written in <I>DMScript</I> ";
	  t+="(DeployHub's own scripting language) which can then be used as stored procedures. Actions can be built from Procedures and ";
	  t+="Functions. Actions are attached to Components and Applications to control the activities that DeployHub will perform ";
	  t+="before and after each Component (or Application) is deployed.";
	  t+=w2;
	  t+="<div id=\"helppage7\" style=\"display:none\">"+w1;
	  t+="<h2>Connections View</h2>The <I>Connections</I> view contains all the Repositories, Data Sources and Credentials that DeployHub ";
	  t+="can access. A Repository is a source of deployment artifacts and can be an SCM tool, an FTP(S) server, a web URL (for binary ";
	  t+="repositories) or even just a file system. Credentials are objects (typically username/password combinations) that allow DeployHub ";
	  t+="to connect to external systems, such as repositories, end-points or Data Sources. A <I>Data Source</I> is a source of external ";
	  t+="data such as an ODBC connection to a database.";
	  t+=w2;
	  t+="<div id=\"helppage8\" style=\"display:none\">"+w1;
	  t+="<h2>Domains View</h2>The <I>Domains</I> view allows you to modify the Domain Hierarchy. Every object in DeployHub is associated ";
	  t+="with a Domain. A user's <I>Home Domain</I> is always shown at the top of the explorer panel. ";
	  t+="The \"My Pipeline\" domain contains sub-domains which represent a pipeline that an application would ";
	  t+="typically go through on its way to production. You can change this pipeline if required.<BR><BR>Domains contain <I>Tasks</I> which users ";
	  t+="can invoke. Tasks perform activities such as deployments, approvals or move application versions between domains in a pipeline.";
	  t+=w2;
	  t+="<div id=\"helppage9\" style=\"display:none\">"+w1;
	  t+="<h2>Users & Groups View</h2>The <I>Users & Groups</I> view allows you to view/create users in DeployHub and assign those ";
	  t+="users to different user groups. The permission model in DeployHub is based around membership of different user groups. <I>Tasks</I> ";
	  t+="are made available to members of specific User Groups and every object in DeployHub has access control via User Groups.<BR><BR>";
	  t+="User Groups are granted permissions to create new objects of specific types.";
	  t+=w2;
	  t+="<div id=\"helppage10\" style=\"display:none\">"+w1;
	  t+="<h2>Home Page</h2>This is your Home Page. You click here to view your \"To Do\" list. You are assigned things to do based on your membership of ";
	  t+="different <I>User Groups</I> or if you are the \"owner\" of certain objects. For example, if a user requests a time slot on ";
	  t+="an environment calendar and you are the owner of that environment (either as an individual or as a member of the owning user ";
	  t+="group) then the request to authorise the time will appear on your \"To Do\" list.<BR><BR>";
	  t+="The Home Page also shows the combined timeline of all the objects to which you have subscribed. You can subscribe to any ";
	  t+="object in DeployHub to which you have access. The timeline for all subscribed objects is brought together into the Timeline ";
	  t+="Tab";
	  t+=w2;
	  t+="<div id=\"helppage11\" style=\"display:none\">"+w1;
	  t+="<h2>Breadcrumb Tray</h2>The flashing panel at the bottom of the screen is the <I>Breadcrumb Tray</I>. It contains shortcuts to ";
	  t+="previously selected objects. Every time you select an object in the <I>Explorer</I> it is placed into the Breadcrumb Tray. To ";
	  t+="return to a previously selected object, just click on the link in the Breadcrumb Tray. DeployHub will automatically switch ";
	  t+="the <I>View</I> and the <I>Explorer Tab</I> to select the desired object.<BR><BR>";
	  t+="Why not try it now? Click on different views and select objects from the different Explorer Panels. See how they appear in the ";
	  t+="Breadcrumb Tray. Click on the object in the Breadcrumb Tray to reset the views and tabs to redisplay the selected obejct.";
	  t+=w2;
	  t+="<div id=\"helppage12\" style=\"display:none\">"+w1;
	  t+="<h2>Go Deploy!</h2>That's it! You now know all you need to navigate the DeployHub User Interface. Click \"Finish\" to close ";
	  t+="this dialog and to start using DeployHub. Click \"Back\" to revisit the earlier pages."
	  t+=w2;
	  $("#modal").html(t);

	  $("#modal").dialog({
		title: "Welcome to DeployHub Open Source Edition!",
	    resizable : false,
	    width : 600,
	    height : 400,
	    modal : false,
	    draggable: true
	   });
	   $("#modal").dialog("option", "buttons", [
	   {
	    text : "Yes, Please!",
	    click : function()
	    {
	    	HelpPageNumber++;
	    	ShowHelpPage(HelpPageNumber);
	    }
	   },{
		    text : "No, thanks!",
		    click : function()
		    {
		    	$("#modal").dialog("close");
		    	ShowingInitialHelp=false;
		    	clearAllFlashFlags();
		    }
		 }
	   ]);

	   $("#modal").dialog("open");
 }
 
 function EnableTabs(menuname,nocallback)
 {
  viewFlasher=false;
  currentmenu = menuname;
  SetActiveMenu(menuname);
  
  nocallback = nocallback || false;
  console.log("EnableTabs("+menuname+") ShowingInitialHelp="+ShowingInitialHelp);

  if (menuname == "domains_menu")
  {
   $("#applications_tab").hide();
   $("#components_tab").hide();
   $("#environments_tab").hide();
   $("#builders_tab").hide();
   $("#credentials_tab").hide();
   $("#servers_tab").hide();
   $("#datasources_tab").hide();
   $("#actions_tab").hide();
   $("#functions_tab").hide();
   $("#procedures_tab").hide();
   $("#notifiers_tab").hide();
   $("#users_tab").hide();
   $("#groups_tab").hide();
   $("#repositories_tab").hide();
   $("#domains_tab").show();
   $("#types_tab").show();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(8);
  }
  else if (menuname == "release_menu")
  {
   $("#applications_tab").show();
   $("#components_tab").show();
   $("#environments_tab").hide();
   $("#builders_tab").hide();
   $("#credentials_tab").hide();
   $("#servers_tab").hide();
   $("#datasources_tab").hide();
   $("#actions_tab").hide();
   $("#functions_tab").hide();
   $("#procedures_tab").hide();
   $("#notifiers_tab").hide();
   $("#users_tab").hide();
   $("#groups_tab").hide();
   $("#repositories_tab").hide();
   $("#domains_tab").hide();
   $("#types_tab").hide();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(4);
  }
  else if (menuname == "containers_menu")
  {
   $("#applications_tab").hide();
   $("#components_tab").hide();
   $("#environments_tab").hide();
   $("#builders_tab").hide();
   $("#credentials_tab").show();
   $("#servers_tab").hide();
   $("#datasources_tab").show();
   $("#actions_tab").hide();
   $("#functions_tab").hide();
   $("#procedures_tab").hide();
   $("#notifiers_tab").hide();
   $("#users_tab").hide();
   $("#groups_tab").hide();
   $("#repositories_tab").show();
   $("#domains_tab").hide();
   $("#types_tab").hide();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(7);
  }
  else if (menuname == "endpoints_menu")
  {
   $("#applications_tab").hide();
   $("#components_tab").hide();
   $("#environments_tab").show();
   $("#builders_tab").show();
   $("#credentials_tab").hide();
   $("#servers_tab").show();
   $("#datasources_tab").hide();
   $("#actions_tab").hide();
   $("#functions_tab").hide();
   $("#procedures_tab").hide();
   $("#notifiers_tab").hide();
   $("#users_tab").hide();
   $("#groups_tab").hide();
   $("#repositories_tab").hide();
   $("#domains_tab").hide();
   $("#types_tab").hide();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(5);
  }
  else if (menuname == "deploy_menu")
  {
   $("#applications_tab").hide();
   $("#components_tab").hide();
   $("#environments_tab").hide();
   $("#credentials_tab").hide();
   $("#servers_tab").hide();
   $("#datasources_tab").hide();
   $("#builders_tab").hide();
   $("#actions_tab").show();
   $("#functions_tab").show();
   $("#procedures_tab").show();
   $("#notifiers_tab").show();
   $("#users_tab").hide();
   $("#groups_tab").hide();
   $("#repositories_tab").hide();
   $("#domains_tab").hide();
   $("#types_tab").hide();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(6);
  }
  else if (menuname == "usersgroups_menu")
  {
   $("#applications_tab").hide();
   $("#components_tab").hide();
   $("#environments_tab").hide();
   $("#builders_tab").hide();
   $("#builders_tab").hide();
   $("#credentials_tab").hide();
   $("#servers_tab").hide();
   $("#datasources_tab").hide();
   $("#actions_tab").hide();
   $("#functions_tab").hide();
   $("#procedures_tab").hide();
   $("#notifiers_tab").hide();
   $("#users_tab").show();
   $("#groups_tab").show();
   $("#repositories_tab").hide();
   $("#domains_tab").hide();
   $("#types_tab").hide();
   if (ShowingInitialHelp && !nocallback) ShowHelpPage(9);
  }
 }
</script>  
</head>
<body>
<div id="container">

 <div id="header">
  <div class="omlogo"><img alt="DeployHub" src="images/deployhubdogwhite.png"></div>
   <div id="positiondevopsmenu">		
	<div class="moduletable_menu">			
      <ul class="menu_menu">
        <li id="setup_menu" onclick="SetupWizard()">Setup</li>
        <li id="release_menu"  onclick="EnableTabs(this.id)">Deploy</li>     
        <li id="endpoints_menu" onclick="EnableTabs(this.id)">Data Center</li>
        <li id="deploy_menu" onclick="EnableTabs(this.id)">Flows</li>
        <li id="containers_menu" onclick="EnableTabs(this.id)">Connections</li>   
        <li id="domains_menu" onclick="EnableTabs(this.id)">Domains</li>
        <li id="usersgroups_menu" onclick="EnableTabs(this.id)">Users &amp; Groups</li>
      </ul>
	</div>
   </div>	
   
   <div id="positiondevopsmenu_right">		
    <div class="moduletable_menu">					
      <ul class="menu_menu">
        <li id="home_menu" onclick="displayhome=true;ShowHome(true);"><img src="images/home.png" alt="Home" /></li>
        <li id="about_menu" onclick="ShowAbout()"><img src="images/about.png" alt="About" /></li>
        <li id="user_menu" onclick="LoadUserProfile()" ><img src="css/images/user_16x.png" alt="User:" /></li>
        <li id="help_menu"><a class="menu_icon" href="http://www.deployhub.org/help/NetHelp/index.html" target="_blank" ><img src="images/help.png" alt="Help" /></a></li>
        <li id="logout_menu"><a class="menu_icon" href="Logout" ><img src="images/logout.png" alt="Logout" /></a></li>
      </ul>
	 </div>
    </div>
	
	<div class="clear"></div>	
	</div> <!-- end header -->
	<div id="panel_container" class="left">
	 <div id="left_panel_tabs">
	  <div class="tabrow_tabmenu">
	   <div class="moduletable_tabmenu">
		<ul class="menu_tabmenu">
	      <li id="domains_tab" onclick="SetActive(this.id)" style="display:none">Domains</li>
	      <li id="components_tab" onclick="SetActive(this.id)" style="display:none">Components</li>
          <li id="applications_tab" onclick="SetActive(this.id)" style="display:none">Applications</li>
          <li id="environments_tab" onclick="SetActive(this.id)" style="display:none">Environments</li>
          <li id="repositories_tab" onclick="SetActive(this.id)" style="display:none">Repositories</li> 
          <li id="servers_tab" onclick="SetActive(this.id)" style="display:none">End Points</li>  
          <li id="builders_tab" onclick="SetActive(this.id)" style="display:none">Build Engines</li>  
          <li id="datasources_tab" onclick="SetActive(this.id)" style="display:none">Data Sources</li> 
          <li id="credentials_tab" onclick="SetActive(this.id)" style="display:none">Credentials</li>
          <li id="actions_tab" onclick="SetActive(this.id)" style="display:none">Actions</li>  
          <li id="procedures_tab" onclick="SetActive(this.id)" style="display:none">Functions & Procedures</li>   
          <li id="notifiers_tab" onclick="SetActive(this.id)" style="display:none">Notifiers</li> 
          <li id="users_tab" onclick="SetActive(this.id)" style="display:none">Users</li>
          <li id="groups_tab" onclick="SetActive(this.id)" style="display:none">Groups</li>
          <li id="types_tab" onclick="SetActive(this.id)" style="display:none">Component & End Point Types</li>                                                       
        </ul>
	   </div>
	  </div>
	 </div>
	 <div id="left_panel_tree" class="div_scroll">
	 
	  <div id="applications_tree" align="left"></div>
	  <input type=hidden name="ad_applications_tree" value="Y">
 	  
	  <div id="components_tree" align="left""></div>
	  <input type=hidden name="ad_components_tree" value="Y">	
	      
	  <div id="environments_tree" align="left"></div>
	  <input type=hidden name="ad_environments_tree" value="Y">	
	  
	  <div id="builders_tree" align="left"></div>
	  <input type=hidden name="ad_builders_tree" value="Y">	
	   
	  <div id="credentials_tree" align="left"></div>
	  <input type=hidden name="ad_credentials_tree" value="Y">	
	   
	  <div id="datasources_tree" align="left"></div>
	  <input type=hidden name="ad_datasources_tree" value="Y">	
	   
	  <div id="actions_tree" align="left"></div>
	  <input type=hidden name="ad_actions_tree" value="Y">	
	   
	  <div id="functions_tree" align="left"></div>
	  <input type=hidden name="ad_functions_tree" value="Y">	
	
	  <div id="procedures_tree" align="left"></div>
	  <input type=hidden name="ad_procedures_tree" value="Y">	
	
	  <div id="users_tree" align="left"></div>
	  <input type=hidden name="ad_users_tree" value="Y">	
	  	   
	  <div id="groups_tree" align="left"></div>
	  <input type=hidden name="ad_groups_tree" value="Y">	
	  
	  <div id="servers_tree" align="left"></div>
	  <input type=hidden name="ad_servers_tree" value="Y">	
	  
	  <div id="notifiers_tree" align="left"></div>
	  <input type=hidden name="ad_notifiers_tree" value="Y">	
	  
	  <div id="repositories_tree" align="left"></div>
	  <input type=hidden name="ad_repositories_tree" value="Y">	
	  	  
	  <div id="domains_tree" align="left"></div>
	  <input type=hidden name="ad_domains_tree" value="Y">	
	  	
	  <div id="types_tree" align="left"></div>
	  <input type=hidden name="ad_types_tree" value="Y">	
	 </div>
    </div> <!-- end panel container -->
	
	<div id="panel_container_right" class="right">
	 <div id="right_panel_tabs">
	  <div class="tabrow_tabmenu">		
	   <div class="moduletable_tabmenu">	
	    <ul>			
          <li id="tabs-ApplicationVersions" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned Applications</li>
          <li id="tabs-Components" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned Components</li>
          <li id="tabs-ComponentItems" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Component Items</li>
          <li id="tabs-Versions" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Versions</li>
          <li id="tabs-ComponentVersions" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Versions</li>       
          <li id="tabs-ProcBody" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Body</li>        
          <li id="tabs-Workflow" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Workflow</li>
          <li id="tabs-Args" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Args</li> 
          <li id="tabs-History" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Timeline</li>
          <li id="tabs-Tasks" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Tasks</li>                   
          <li id="tabs-Calendar" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Calendar</li>          
          <li id="tabs-GroupMembership" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Group Membership</li>
          <li id="tabs-UserMembership" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">User Membership</li>                    
          <li id="tabs-Applications" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned Applications</li>
          <li id="tabs-Servƒers" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned End Points</li>
          <li id="tabs-SrvComponents" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Deployed Components</li>
          <li id="tabs-CompServers" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">End Points & Builds</li>
          <li id="tabs-CompApplications" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned Applications</li>
          <li id="tabs-Properties" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Properties</li>
          <li id="tabs-Patterns" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Patterns</li>                      
          <li id="tabs-AdminRights" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Admin Rights</li>
          <li id="tabs-Environments" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Assigned Environments</li>
          <li id="tabs-Reports" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Trends</li>
          <li id="tabs-RelPlanning" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Planner</li>
          <li id="tabs-ServerStatus" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Status</li>
          <li id="tabs-Servers" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">End Points</li>
          <li id="tabs-Recipients" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Recipients</li> 
          <li id="tabs-Body" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Body</li>
          <li id="tabs-Attributes" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Attributes</li>
          <li id="tabs-Builds" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Builds</li> 
          <li id="tabs-Access" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">Access</li>
          <li id="tabs-General" onclick="SubTabBreadCrumb(this.id);ActivateSubTabs()" style="display:none">General</li>
        </ul>
	   </div>
	  </div>
	 </div>
	 <div id="right_panel_main">
	  <div id="right_panel_title_area" class="right_panel_title_area" style="display:inline-block;"></div>
	  <div id="title_icon" class="title_icon"></div>
	   <div id="tabs-General-data" class="div_scroll" style="display:none">
          <table id="summ" class="dev_table">
           <tbody>
           </tbody>
          </table>
       </div>

	   <div id="tabs-Servers-data" class="div_scroll" style="display:none">
          <table id="srv4env" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Name</th><th>Host Name</th><th>Summary</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>         
       </div>

	   <div id="tabs-SrvComponents-data" class="div_scroll" style="display:none">
          <table id="comp4srv" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Component</th><th>Current Version</th><th>Build Number</th><th>Details</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>         
       </div>
       
	   <div id="tabs-CompServers-data" class="div_scroll" style="display:none">
          <table id="srv4comp" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Environment</th><th>End Point</th><th>Component Version</th><th>Details</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>
          <div id="compbuilds"></div>     
       </div>

	   <div id="tabs-CompApplications-data" class="div_scroll" style="display:none">
          <table id="app4comp" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Application Version</th><th>Component Version</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>         
       </div>
       
	   <div id="tabs-GroupMembership-data" class="div_scroll" style="display:none">
          <table id="groupmembership" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Groups</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>         
       </div>

	   <div id="tabs-UserMembership-data" class="div_scroll" style="display:none">
          <table id="usermembership" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Users</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>         
       </div>


	   <div id="tabs-Args-data" class="div_scroll" style="display:none">
          <div id="ArgsMainFrame"></div>
          <div id="inputparamsh1"></div>
          <div id="inputparamsdiv"></div>
          <div id="argsh1"></div>
          <div id="argstablediv"></div>
          <div id="dragtextdiv" style="margin:10px"></div> 
          <div id="argsoutputpane"></div>         
       </div>
       
       <div id="tabs-ProcBody-data" class="div_scroll" style="display:none;height:87%;">
          <div id="ProcBodyMainFrame" style="height:100%;width:100%;">
          <textarea id="ProcBodyTextArea" readonly style='width:100%;height:100%;font-family:courier;font-size:14px;'></textarea>   
          </div>
       </div>

	   <div id="tabs-Properties-data" class="div_scroll" style="display:none">
          <div id="PropertiesMainFrame"></div>
          <table id="propstable" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Name</th><th>Value</th><th>Encrypted</th><th>Overridable</th><th>Appendable</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>        
       </div>
   
   	   <div id="tabs-Patterns-data" class="div_scroll" style="display:none">
          <div id="PatternsMainFrame"></div>
          <table id="patternstable" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Path</th><th>Pattern</th><th>Text?</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>        
       </div>
       
       <div id="tabs-Recipients-data" class="div_scroll" style="display:none">
          <div id="RecipientsMainFrame"></div>
          <table id="emailtotable" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Recipients</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>        
       </div>
       
       <div id="tabs-Builds-data" class="div_scroll" style="display:none">
          <table id="BuildHistory" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th align="left" style="width:100px">Build Number</th><th align="left" style="width:150px">Date/Time</th><th align="left">Component(s)</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>   
       </div>

       
	   <div id="tabs-Body-data"  class="tbd" style="display:none">
          <div id="BodyMainFrame"></div>
          <div style="float: left;width:45%;overflow:auto;" style="display:none">
          <table id="bodytable" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Subject</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table> 
          </div>
          <!-- <div style="float:right; overflow:hidden; width:54%; padding: 5px; border-left: 1px solid gray;"> -->
          <div id="helppanel" class="helppanel" style="display:none">
          <h2>Using Variables</h2>
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
          <table border="0">
          <tr><td><B>Variable Name</B></td><td width="120"><B>Description</B></td><td><B>Operation</B></td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\${application.name}</td><td valign="top">The name of the current application</td><td valign="top">Any DeployHub task operating on an Application (such as <i>Deploy, Move, Approve</i> etc)</td></tr>
 	 	  <tr><td valign="top">\${environment.name}</td><td valign="top">The name of the target environment</td><td valign="top">Any DeployHub task operating on an Environment (such as <i>Deploy</i> etc)</td></tr>
		  <tr bgcolor="#E0E0E0"><td valign="top">\${user.name}</td><td valign="top">The name of the invoking user</td><td valign="top">Any DeployHub task.</td></tr>
		  <tr><td valign="top">\$DEPLOY_ID</td><td valign="top">The Deployment ID Number</td><td valign="top">Set during a Deploy task.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$SERVER_NAME</td><td valign="top">End Point Name</td><td valign="top">The name of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr><td valign="top">\$SERVER_HOSTNAME</td><td valign="top">End Point hostname</td><td valign="top">The hostname of the end point during an automatic periodic scan (AutoPing or AutoMD5). Use this variable in a Ping or MD5 template.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$SERVER_STATUS</td><td valign="top">OK or FAILED</td><td valign="top">The status of the server during an automatic periodic scan (AutoPing). Use this variable in a Ping template.
          Note this variable is always OK in an MD5 template since MD5 scans are only performed if the ping check succeeds.</td></tr>
          <tr><td valign="top">\$NAME_RESOLUTION</td><td valign="top">OK or FAILED</td><td valign="top">Set during an automatic periodic scan (AutoPing). Reflects the success or failure of resolution of hostname. Use this variable in a Ping template.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$PING</td><td valign="top">OK or FAILED</td><td valign="top">Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the ping to the server. Use this variable in a Ping template.</td></tr>
          <tr><td valign="top">\$CONNECTION</td><td valign="top">OK or FAILED</td><td valign="top">Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the connection attempt to the server using the associated credentials. Use this variable in a Ping template.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$BASEDIR</td><td valign="top">OK or FAILED</td><td valign="top">Set during an automatic periodic scan (AutoPing). Reflects the success or failure of the attempt to switch to the server's base directory. Use this variable in a Ping template.</td></tr>
          <tr><td valign="top">\$MD5_FILELIST</td><td valign="top">A list of files (one per line)</td><td valign="top">Set during an automatic periodic scan (AutoMD5). Lists all the files whose MD5 checksum differs from that calculated during the last deployment to the server. Use this variable in a MD5 template.</td></tr>      
          <tr bgcolor="#E0E0E0"><td valign="top">\$SRCDOMAIN</td><td valign="top">The name of the Source Domain</td><td valign="top">Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>from</I> (for a move) or its current domain (for an approve).</td></tr>
          <tr><td valign="top">\$TGTDOMAIN</td><td valign="top">The name of the Target Domain</td><td valign="top">Set during a <I>Move</I> or <I>Approve</I> task. The name of the Domain the application is being moved <I>to</I> or approved <I>for</I>.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$NOTES</td><td valign="top">Free Text</td><td valign="top">Set during a <I>Move, Approve</I> or <I>Request</I> task. The notes that were entered (if any) by the user when the task was run.</td></tr>
          <tr><td valign="top">\$TASKNAME</td><td valign="top">The name of the Requested Task</td><td valign="top">Set during a <I>Request</I> task. The name of the task being requested to be run.</td></tr>
          <tr bgcolor="#E0E0E0"><td valign="top">\$ERROR_TEXT</td><td valign="top">Last End Point Error</td><td valign="top">Set during an automatic periodic scan (AutoPing). The last error detected during the connection attempt.</td></tr>
          
          </table>
          </div>    
       </div>
       
	   <div id="tabs-Calendar-data" class="div_scroll" style="display:none">
         <div id="calendar"></div>
       </div>
       
	   <div id="tabs-Applications-data" class="div_scroll" style="display:none">
	     <div>
          <table id="App4Env" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Application</th><th>Current Version</th><th>Details</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>
         </div>
         <br>
       </div>
    
       <div id="tabs-PreReqs-data" style="display:none">
          <table id="prereqs_table" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Name</th><th>Action</th></tr>
           </thead>
           <tbody>
           </tbody>
          </table>
          <div id="prereqs_btnAddApp" class="tablebutton">Add Application</div><div id="prereqs_btnAddComp" class="tablebutton">Add Component</div><div id="prereqs_btnDelete" class="tablebutton">Delete</div>
        </div>

       <div id="tabs-Tasks-data" style="display:none">
       </div>
       
       <div id="tabs-Components-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="panel_container compversions">
	     <div id="innercomp" style="height:600px;width:100%;">
		 </div>
	    </div> 	
	    <div class="accordionpanel">
	     <h4 class="ui-widget-header">Component Versions</h4>
   	     <div id=componentlist align="left" style="margin:0"></div>
        </div>
       </div>
      </div>

       <div id="tabs-ComponentVersions-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="panel_container compversions">
	      <div id="innercompversions" style="height:600px;width:100%;">
		  </div>
	     </div> 	
        </div>
       </div>
       
        <div id="tabs-ComponentItems-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="panel_container compitems">
	      <div id="inneritem" style="height:600px;width:100%;">
		  </div>
	     </div> 	
        </div>
       </div>      

       <div id="tabs-ApplicationVersions-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="panel_container appversions">
	     <div id="innerappver" style="height:600px;width:100%;">
		 </div>
	    </div> 	
	    <div class="accordionpanel">
	     <h4 class="ui-widget-header">Component Versions</h4>
   	     <div id="applicationversionlist" align="left" style="margin:0"></div>
        </div>
       </div>
      </div>                	

       <div id="tabs-Workflow-data" style="display:none">
        <div id="outerworkflow" class="panel_container versionsandinformation">
     	 <div class="panel_container versions">
	     <div id="innerworkflow" style="height:600px;width:100%;">
		 </div>
	    </div> 	
	    <div class="accordionpanel">
	     <h4 class="ui-widget-header">Function/Procedure</h4>
   	     <div id="fragmentlist" align="left" style="margin:0"></div>
        </div>
       </div>
      </div>
      
       <div id="tabs-Attributes-data" style="display:none">
         <div id="attrib-data">
          <table id="attrib" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Name</th><th>Value</th></tr>
           </thead>
           <tbody>
           </tbody>
          </table>
         </div>
        </div>
	
       <div id="tabs-Access-data" style="display:none">
       </div>
		
       <div id="tabs-History-data" style="display:none">
       	  
          <table id="histfeed" class="dev_table">
           <tbody>
           </tbody>
          </table> 
       </div>
       
       <div id="tabs-Versions-data" style="display:none">
        <div class="panel_container versionsandinformation">
     	 <div class="panel_container appversions">
	      <div id="innerversions" style="height:600px;width:100%;">
		  </div>
	     </div> 	
        </div>
       </div>

	   <div id="tabs-ServerStatus-data" style="display:none">
	   <table id="servstat" class="dev_table">
           <tbody>
           </tbody>
       </table>
	   </div>
	   
	   <div id="tabs-Environments-data" class="div_scroll" style="display:none">
	     <div>
          <table id="Env4App" cellspacing="0" width="100%" >
           <thead class="ui-widget-header">
            <tr><th>Environment</th><th>Current Version</th><th>Details</th></tr>
           </thead>	   
           <tbody>
           </tbody> 
          </table>
         </div>
         <br>
         <div class="ui-widget-header">Pending Events for this Application</div>
         <div id="PendingEnv4App"> 
        </div>  
       </div>
	
	   <div id="tabs-RelPlanning-data" class="div_scroll" style="display:none">
           <div id="planningtimeline"></div>          
       </div>
       
       <div id="tabs-Reports-data" style="display:none">
        <div id="changehistoryheader" class="ui-widget-header" style="display":none">Change History</div>
        <div id="endpointtimeline" style="display:none"></div>  
        <div class="ui-widget-header">Trends</div>
        <div id="reports-data">
         <div id="plot1resizable" style="float:left;height:200px;width:400px">
          <div id="timetodeploylinechart" style="height:96%;width:96%;"></div>
         </div>
         <div id="plot2resizable" style="float:left;height:200px;width:200px">
          <div id="applicationsuccesspiechart" style="height:96%;width:96%;"></div>
         </div>
        </div> 
      </div>
      		
       <div id="tabs-AdminRights-data" style="display:none">
       	<div id="panel_container_taskspaneltop_left">
       	  <div id="sub_panel_topleft">
				<div class="panel_container_title_area tasks ">
				  <h1 class="taskpanels">Access Control</h1>
				</div>
				<div class="deploy_sub_panel_container" style="overflow:hidden">
				 <table>
				  <tr><td align="left">
				 	<input type="checkbox" id="ar_override"><label for="ar_override">Override Access Control</label>
  				  </td></tr>
  				 </table>
  				</div>
  		 </div>		
  		</div>						
			<div id="panel_container_taskspanelbottom_left" style="display:none">
			  <div id="sub_panel_topleft">
  				<div class="panel_container_title_area tasks" >
				  <h1 class="taskpanels">Tab Access</h1>
				</div>
				<div class="deploy_sub_panel_container" style="overflow:hidden">
				 <table>
				  <tr><td align="left"><input type="checkbox" id="ar_endpoints"><label for="ar_endpoints">Endpoints and Credentials</label></td></tr>
				  <tr><td align="left"><input type="checkbox" id="ar_applications"><label for="ar_applications">Applications and Components</td></tr>
				  <tr><td align="left"><input type="checkbox" id="ar_actions"><label for="ar_actions">Actions and Procedures</label></td></tr>
				  <tr><td align="left"><input type="checkbox" id="ar_providers"><label for="ar_providers">Providers</label></td></tr>
				  <tr><td align="left"><input type="checkbox" id="ar_users"><label for="ar_users">Users and Groups</label></td></tr>
  				 </table>
  				</div>					
  			 </div>	
			</div>			
		  <div id="panel_container_taskspaneltop" style="height:380px;left:30px;">
			<div id="panel_deploy" >
			   	<div class="panel_container_title_area tasks ">
					<h1 class="taskpanels">Creation Rights</h1>
				</div>	
				 <table>
					<tr><td align="left"><input type="checkbox" id="ar_cusers"><label for="ar_cusers">Users</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cgroups"><label for="ar_cgroups">User Groups</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cdomain"><label for="ar_cdomain">Domains</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cenv"><label for="ar_cenv">Environments</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cserver"><label for="ar_cserver">End Points</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_crepo"><label for="ar_crepo">Repositories</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_ccomp"><label for="ar_ccomp">Components</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_ccred"><label for="ar_ccred">Credentials</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_capp"><label for="ar_capp">Applications</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cappver"><label for="ar_capp">Application Versions</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_caction"><label for="ar_caction">Actions</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cproc"><label for="ar_cproc">Procedures</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cdsrc"><label for="ar_cdsrc">DataSources</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cnotify"><label for="ar_cnotify">Notifiers</label></td></tr>
					<tr><td align="left"><input type="checkbox" id="ar_cengine"><label for="ar_cengine">Build Engines</label></td></tr>			
			 </table>
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
          <li id="tabs-HomePending" onclick="ActivateHomeSubTabs('tabs-HomePending')" class="active current">To Do List</li>
          <li id="tabs-HomeHistory" onclick="ActivateHomeSubTabs('tabs-HomeHistory')">Timeline</li>  
          <li id="tabs-HomeDashboard" onclick="ActivateHomeSubTabs('tabs-HomeDashboard')">Global Reports</li>                      
          <li id="tabs-DomainSummary" onclick="ActivateHomeSubTabs('tabs-DomainSummary')">Delivery Pipeline</li>
        </ul>
	   </div>
	  </div>
	 </div>
	 <div id="home_right_panel_main">
	  <div id="tabs-HomePending-data">
	    <table id="homependingtab"><tbody></tbody></table>
	  </div> 
	  <div id="tabs-HomeHistory-data" style="display:none">
	    <table id="homehisttab"><tbody></tbody></table>
	  </div>
	  <div id="tabs-DomainSummary-data" style="display:none">
	    <div id="domsumm" style="height:100%"></div>
	  </div>
	  <div id="tabs-HomeDashboard-data" style="display:none">
	  <ul>
	   <li class="omdashboard">
	    <a href="/dmadminweb/reports/ServerInventory.html" target="_blank">End Point Inventory Report</a><br>List of all assets deployed on to each server.
	    </li>
	    <li class="omdashboard">
	      <a href="/dmadminweb/reports/FileAudit.html" target="_blank">File Audit Trace</a><br>Deployment history for a deployed file.
	    </li>
	    <li class="omdashboard">
	     <a href="/dmadminweb/reports/ServerAudit.html" target="_blank">Audit of the files deployed to End Points</a><br>Differences between the files on the End Point compared to what was deployed.
	    </li>
	    <li class="omdashboard">
	      <a href="/dmadminweb/reports/EnvSuccessFail.html" target="_blank">Success/Failed Deployments per Environment Report</a><br>List the number of successful and failed deployments per Environment.
	    </li>
	   </ul> 
	  </div>
	 </div> 
   </div>
   </div>  <!-- end container -->
	<div id="footer_container">	 
	</div>
	<div class="clear">
	</div>
	<div id="login_container_dialog"></div>
	<div id="firstinstall_container_dialog"></div>
	<div id="modal"></div>
	<div id="displaylog"></div>
	<div id="buildlog"></div>
</body>
</html>

