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

 function escapeRegExp(str) {
  return str.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, "\\$&");
}

$(document).click(function (e) {
   if ($(e.target).parents(".dropdown_menu").length === 0) {
       $(".dropdown_menu-content").hide();
   }
});

 $(document).ready(function()
 {
  if (window.location.href.indexOf("console.deployhub.com") != -1 || window.location.href.indexOf("localhost") != -1)
  {
	$(".omlogo").append("<img alt=\"Ortelius\" src=\"images/ortelius-hostedby.png\" style=\"margin-top:-4px !important\" width=\"260px\">");
  }
  else
  {
	$(".omlogo").append("<img alt=\"Ortelius\" src=\"images/logo.png\"  style=\"margin-top:4px\">");
  }

  cPlumb = jsPlumb.getInstance();
  aPlumb = jsPlumb.getInstance();
  avPlumb = jsPlumb.getInstance();
  cvPlumb = jsPlumb.getInstance();
  cisplumb = jsPlumb.getInstance();
  wfPlumb = jsPlumb.getInstance();
  window.addEventListener("resize", applist_table_resize);
  window.addEventListener("resize", complist_table_resize);
  window.addEventListener("resize", envlist_table_resize);
  window.addEventListener("resize", endpointlist_table_resize);
  window.addEventListener("resize", buildenglist_table_resize);
  window.addEventListener("resize", actionlist_table_resize);
  window.addEventListener("resize", procedurelist_table_resize);
  window.addEventListener("resize", notifierlist_table_resize);
  window.addEventListener("resize", repositorylist_table_resize);
  window.addEventListener("resize", datasourcelist_table_resize);
  window.addEventListener("resize", credentiallist_table_resize);
  window.addEventListener("resize", userlist_table_resize);
  window.addEventListener("resize", grouplist_table_resize);
  window.addEventListener("resize", servercomptypelist_table_resize);
  window.addEventListener("resize", templatelist_table_resize);
  window.addEventListener("resize", buildjoblist_table_resize);

  CreateReports();
  $(document).ajaxStart($.blockUI).ajaxStop($.unblockUI);
  $.blockUI.defaults.message = $('#throbber');
  $.blockUI.defaults.fadeOut = 200;
  $.blockUI.defaults.overlayCSS.opacity = 0;

  var blockcss = $.blockUI.defaults.css;
  blockcss['border'] = "none";
  blockcss["backgroundColor"] = "transparent";
  $.blockUI.defaults.css = blockcss;

  if (window.history && window.history.pushState)
  {
   $(window).on('popstate', function() {
     var hashLocation = location.hash;
     var hashSplit = hashLocation.split("#!/");
     var hashName = hashSplit[1];

     if (hashName !== '')
     {
       var hash = window.location.hash;
       if (hash === '#dhmain')
       {
        if (typeof currenttree != "undefined")
        {
         var listName = tree2menu[currenttree];
         openList(null,listName);
        }
       }
       else
       {
        window.history.back();
       }
     }
   });


    var href = window.location.href;
    href = href.replace("#dhmain","");
    href = href.replace("#dhnav", "");

    window.history.pushState('dhmain', null, href + '#dhmain');
    urlstack['dhmain'] = 1;
  }

  HideSetup();
 //$.blockUI.defaults.theme = false;
 //$.blockUI.defaults.themedCSS.width = "15%";

  loginformData = "";  // decodeURIComponent(GetCookie("logindata"));

  loggedin='N';
 // loginformData = loginformData.replaceAll("&initial=Y","")

   $.ajax({
    type : "POST",
    url : "Login",
    async : false,
    dataType: "json"
   }).done(function(data) {
    console.log("login data:");
    console.log(data);
     if (data == null) {
      loggedin = 'N';
     } else if (data.Msg.toLowerCase() == "Login OK".toLowerCase()) {
      console.log("normal login");
       loggedin = 'Y';
       admin = 'N';
       userdatefmt = data.datefmt;
       usertimefmt = data.timefmt;
       userdom = data.domain;
       userdomid = data.domainid;
       myuserid = data.userid;
       domlist = data.domlist.split(',');
       isSaas = data.issaas;
       licType = data.lictype;
       licCnt  = data.liccnt;
       if (isSaas == 'Y' && licType == "OSS")
        hideOSS = "Y";
       else
        hideOSS = "N";

       if (!hideOption())
        $("#verttab_setup").show();
       else
          $("#verttab_setup").hide();

     } else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
       loggedin = 'Y';
       admin = 'Y';
       userdatefmt = data.datefmt;
       usertimefmt = data.timefmt;
       userdom = data.domain;
       userdomid = data.domainid;
       myuserid = data.userid;
       domlist = data.domlist.split(',');
       isSaas = data.issaas;
       licType = data.lictype;
       licCnt  = data.liccnt;
       if (isSaas == 'Y' && licType == "OSS")
        hideOSS = "Y";
       else
        hideOSS = "N";

      if (!hideOption())
		    $("#verttab_setup").show();
	    else
	      $("#verttab_setup").hide();
    }
     SetCookie("admin",admin);
  });

  if ("${firstinstall}" == "Y" && isSaas != 'Y') {
   // First install - prompt the user to set admin password and show them
   // some basic information to get started
   var l = "<div id=\"firstinstall\">";
   l+="<table border=\"0\"><tr><td valign=\"top\"><img src=\"css/images/setupdog.png\"></td><td valign=\"top\">";
   l+="<h2>Set Admin Password</h2>Welcome. The first thing you need to do is to set the password for the \"admin\" user account. ";
   l+="Once this is done, you will be logged in as the \"admin\" user and you will be able to create more user accounts. ";
   l+="<br><br>";
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
     title : "Welcome to DeployHub!",
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
   var l = "<div style=\"background-color: #5a4475\"><img src=\"images/splash.png\" alt=\"\"  style=\"height:86px\" class=\"logindialog\" /></div>"
   l += "<div id=\"loginset\">";
   l += "  <form id=\"loginform\">";
   l += "<table id=\"logintab\" style=\"width: 100%;\" border=\"0\">";
   l += "  <tr style=\"display:none\">";
   l += "   <td class=\"login_label\" align=\"right\">Domain:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" name=\"domain\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"logindlg_user\" style=\"display:none\">";
   l += "   <td class=\"login_label\" align=\"right\">Username:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" name=\"username\" id=\"username\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"logindlg_pw\" style=\"display:none\">";
   l += "   <td class=\"login_label\" align=\"right\">Password:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"password\" name=\"password\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"logindlg_newpw\" class=\"newpw login_hide\">";
   l += "   <td class=\"login_label\" align=\"right\">New Password:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpassword\" name=\"newpassword\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"logindlg_newpwagain\" class=\"newpw login_hide\">";
   l += "   <td class=\"login_label\" align=\"right\">Verify New Password:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"password\" id=\"newpasswordagain\" name=\"newpasswordagain\" /></td>";
   l += "  </tr>";
   l += "  <tr>";
   l += "   <td colspan=\"2\" align=\"right\"></td>";
   l += "  </tr>";
   l += "</table>";
   l += " </form>";
   l += "</div>";
   l += "<div class=\"error\"><p class=\"error_txt\" id=\"login_err\"></p></div>";
   l += "<div class=\"positioncopyright\">";
   l += " <p class=\"copyright\">Copyright 2019, OpenMake Software. All rights reserved. OpenMake and OpenMake Meister are trademarks or registered trademarks of OpenMake Software.</p>";
   l += "</div>";
   $("#login_container_dialog").html(l);


   $("#login_container_dialog").dialog(
   {
    resizable : false,
    width : 501,
    height : 370,
    modal : true,
    dialogClass : "logindialog"
   });

   $("#login_container_dialog").css("padding", "0");

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
    id: "login",
    text : "Login",
    click : function()
    {
     DoLogin(this);
    }
   },
   {
    id: "signup",
    text : "Sign Up",
    click : function()
    {
	 window.location = 'https://www.deployhub.com/deployhub-team-signup/';
     // DoSignUp(this);
    }
   },
   {
    id: "oksignup",
    text : "OK",
    click : function()
    {
     DoOKSignUp(this);
    }
   },
    {
     id: "cancelsignup",
     text : "Cancel",
     click : function()
     {
      DoCancelSignUp(this);
     }
   },
/*   {
    id: "google",
    class: "stackedbutton",
    icons: {primary: 'google-logo', secondary: null},
    text : "Sign in with Google",
    click : function()
    {
     OAuth.initialize(htuAO);

     OAuth.popup('google').done(function(result) {
         console.log(result);
         result.me().done(function(data) {
          console.log("GOOGLE");
          console.log(data.email);
          DoOAuthLogin(data.email,result.access_token,result.provider);
         }).fail(function (err) {
          console.log(err);
         });;
     })
    }
   },
   {
    id: "github",
    class: "stackedbutton",
    icons: {primary: 'github-logo', secondary: null},
    text : "Sign in with GitHub",
    click : function()
    {
     OAuth.initialize(htuAO);

     OAuth.popup('github').done(function(result) {
         console.log(result);
         result.me().done(function(data) {
          DoOAuthLogin(data.alias,result.access_token,result.provider);
      }).fail(function (err) {
       console.log(err);
      });;
     })
    }
   }, */
   {
    id: "deployhub",
    class: "stackedbutton",
    icons: {primary: 'deployhub-logo', secondary: null},
    text : "Sign in with DeployHub",
    click : function()
    {
    $("#google").addClass("login_hide");
    $("#github").addClass("login_hide");
    $("#deployhub").addClass("login_hide");
    $("#login").removeClass("login_hide");
    $("#logindlg_user").show();
    $("#logindlg_pw").show();
    }
   }
   ]);

   $("#google").addClass("login_hide");
   $("#github").addClass("login_hide");
   $("#deployhub").addClass("login_hide");
   $("#login").removeClass("login_hide");
   $("#oksignup").addClass("login_hide");
   $("#cancelsignup").addClass("login_hide");
   $("#logindlg_user").show();
   $("#logindlg_pw").show();

   $("#login_container_dialog").dialog("open");
//   $("#login").addClass("login_hide");
   $("#newpassword").on("keyup", VerifyPW);
   $("#newpasswordagain").on("keyup", VerifyPW);

   if (window.location.hostname != "localhost" && window.location.hostname != "console.deployhub.com")
      $("#signup").attr("style", "display: none !important");
  }
  else
  {
   var parts = loginformData.split("&");

   for (var x = 0; x < parts.length; x++)
   {
    if (parts[x].indexOf("username=") >= 0)
     myuserid = parts[x].substring(9);
   }

   EnableTabs("application_menu");
  }


  console.log("Myuserid="+myuserid);
  EnableTabs('application_menu');

  $.ajax(
    {
     url:"Engine?userid="+myuserid,
     dataType: "json",
     success: function (res)
     {
      $("#rproxy_menu").text(res.runningcnt + " of " + res.totalcnt + " Reverse Proxy running");
     }
   });
//set timeout
  var tid = setTimeout(rproxy_online, 240000);

  function rproxy_online() {

   $.ajax(
     {
      url:"Engine?userid="+myuserid,
      dataType: "json",
      success: function (res)
      {
 //      $("#rproxy_menu").text(res.runningcnt + " of " + res.totalcnt + " Reverse Proxy running");
      }
    });

    tid = setTimeout(rproxy_online, 240000); // repeat myself
  }
  function abortTimer() { // to be called when you want to stop the timer
    clearTimeout(tid);
  }

  document.getElementById('upload_readme').addEventListener('click', getReadme);
  document.getElementById('upload_swagger').addEventListener('click', getSwagger);
  if (!hideOption())
	$("#verttab_setup").show();
  else
    $("#verttab_setup").hide();

 });


function ActivateMenuItems()
{
 if (hideOption())
 {
  $("#verttab_setup").hide();
  $("#verttab_release").hide();
  $("#verttab_action").hide();
  $("#verttab_procedure").hide();
  $("#verttab_servercomptype").hide();
  $("#verttab_credential").hide();
  $("#verttab_repository").hide();
  $("#verttab_datasource").hide();
  $("#verttab_notifier").hide();
  $("#verttab_template").hide();
 }
 else
 {
  $("#verttab_setup").show();
  $("#verttab_release").show();
  $("#verttab_action").show();
  $("#verttab_procedure").show();
  $("#verttab_servercomptype").show();
  $("#verttab_credential").show();
  $("#verttab_repository").show();
  $("#verttab_datasource").show();
  $("#verttab_notifier").show();
  $("#verttab_template").show();
 }
}

async function getReadme()
{
	 let fileHandle;
     [fileHandle] = await window.showOpenFilePicker();
     const file = await fileHandle.getFile();
     const contents = await file.text();
     AddCompFile(contents, 'readme');
}

async function getSwagger()
{
	let fileHandle;
     [fileHandle] = await window.showOpenFilePicker();
     const file = await fileHandle.getFile();
     const contents = await file.text();
     AddCompFile(contents, 'swagger');
}

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

 function VerifyEmail(field)
 {
  var val = $("#" + field).val();
  var label = $("#" + field).closest('td').prev('td').text().replace(":","");
  var filter = /^[a-z0-9][a-z0-9-_\.]+@([a-z]|[a-z0-9]?[a-z0-9-]+[a-z0-9])\.[a-z0-9]{2,10}(?:\.[a-z]{2,10})?$/
  if (!filter.test(val))
   $("#login_err").html(label + " must be a valid address format");
 }

 function VerifyRequired(field)
 {
  var val = $("#" + field).val();
  var label = $("#" + field).closest('td').prev('td').text().replace(":","");

  if (val.replace(/ /g,"") == "")
   $("#login_err").html(label + " is required");
 }

 function VerifyPassword(field)
 {
  var val = $("#" + field).val();
  var label = $("#" + field).closest('td').prev('td').text().replace(":","");

  if (val.replace(/ /g,"") == "")
   $("#login_err").html(label + " is required");

  if (val.replace(/ /g, "") != val)
   $("#login_err").html(label + " must cannot contain spaces");
 }

 function VerifyField(field)
 {
  var val = $("#" + field).val();
  var label = $("#" + field).closest('td').prev('td').text().replace(":","");

  if (val.replace(/ /g,"") == "")
  {
   $("#login_err").html(label + " is required");
   return;
  }

  if (val.replace(/([a-zA-Z0-9 ])/g, "") != "")
   $("#login_err").html(label + " must contain only spaces, letters and numbers");

 }

 function VerifyUserName(field)
 {
  var val = $("#" + field).val();
  var label = $("#" + field).closest('td').prev('td').text().replace(":","");

  if (val.replace(/ /g,"") == "")
  {
   $("#login_err").html(label + " is required");
   return;
  }

  if (val.replace(/([a-z0-9])/g, "") != "")
   $("#login_err").html(label + " must be lower case and contain only letters and numbers");
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
   userdom = data.domain;
   userdomid = data.domainid;
   domlist = data.domlist.split(',');
   isSaas = data.issaas;
   licType = data.lictype;
   licCnt  = data.liccnt;
   if (isSaas == 'Y' && licType == "OSS")
    hideOSS = "Y";
   else
    hideOSS = "N";

   if (!hideOption())
    $("#verttab_setup").show();
   else
    $("#verttab_setup").hide();

//   SetCookie("p1",newUser);
//   SetCookie("p2",newpw);
   SetCookie("admin",isAdmin);
   SetCookie("loggedin",loggedin);
  } else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
   console.log("2) admin login");
   loggedin="Y";
   admin="Y";
   isAdmin="Y";
   newUser=data.newuser;
   console.log("datefmt="+data.datefmt);
   userdatefmt = data.datefmt;
   usertimefmt = data.timefmt;
   userdom = data.domain;
   userdomid = data.domainid;
   domlist = data.domlist.split(',');
   isSaas = data.issaas;
   licType = data.lictype;
   licCnt  = data.liccnt;
   if (isSaas == 'Y' && licType == "OSS")
    hideOSS = "Y";
   else
    hideOSS = "N";

   if (!hideOption())
    $("#verttab_setup").show();
   else
    $("#verttab_setup").hide();

//   SetCookie("p1",newUser);
//   SetCookie("p2",newpw);
   SetCookie("admin",isAdmin);
   SetCookie("loggedin",loggedin);
  }
   if (loggedin=="Y") {
//    SetCookie("p1",newUser);
//    SetCookie("p2",newpw);
    SetCookie("admin",isAdmin);
//    SetCookie("loggedin",loggedin);
    console.log("newUser="+newUser);
     $("#modal").dialog("close");
     $.sessionTimeout({
       logoutUrl : "Logout",
       redirUrl : "Logout",
       keepAliveUrl : "KeepAlive"
     });
//     SetCookie("logindata",loginformData);
     EnableTabs("application_menu");
     openList(event, 'application');
   } else {
     $("#login_err").html(data.Msg);
   }

  });
 }


 function DoSignUp(dlg)
 {
  console.log("DoLogin");
   var logindlg = parent.$("#login_container_dialog");
   var form = logindlg.find("#loginform");
   var mypw = "";
   var l = "";

   l += "  <tr id=\"companyname_row\">";
   l += "   <td class=\"login_label\" align=\"right\">Company Name:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"companyname\" name=\"companyname\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"projectname_row\">";
   l += "   <td class=\"login_label\" align=\"right\">Project Name:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"projectname\" name=\"projectname\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"firstname_row\">";
   l += "   <td class=\"login_label\" align=\"right\">First Name:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"firstname\" name=\"firstname\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"lastname_row\">";
   l += "   <td class=\"login_label\" align=\"right\">Last Name:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"lastname\" name=\"lastname\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"email_row\">";
   l += "   <td class=\"login_label\" align=\"right\">Email:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"email\" name=\"email\" /></td>";
   l += "  </tr>";
   l += "  <tr id=\"phone_row\">";
   l += "   <td class=\"login_label\" align=\"right\">Phone:</td>";
   l += "   <td align=\"left\"><input class=\"inputbox\" type=\"text\" id=\"phone\" name=\"phone\" /></td>";
   l += "  </tr>";

   $("#logindlg_user").before(l);
   $("#login_container_dialog").height(660);
   $("#oksignup").removeClass("login_hide");
   $("#cancelsignup").removeClass("login_hide");
   $("#logindlg_newpw").removeClass("login_hide");
   $("#logindlg_newpwagain").removeClass("login_hide");
   $("#logindlg_pw").addClass("login_hide");
   $("#login").addClass("login_hide");
   $("#signup").addClass("login_hide");

 }

 function DoCancelSignUp()
 {
  location.href = location.protocol + '//' + location.host + location.pathname.replace('Home', 'Login');
 }

 function DoOKSignUp()
 {
  $("#login_err").html("");
  VerifyField("companyname");
  VerifyField("projectname");
  VerifyRequired("firstname");
  VerifyRequired("lastname");
  VerifyEmail("email");
  VerifyUserName("username");
  VerifyPassword("newpassword");
  VerifyPassword("newpasswordagain");

  var err = $("#login_err").text();

  if (err == "")
  {
   data = {};
   data['companyname'] = $("#companyname").val();
   data['projectname'] = $("#projectname").val();
   data['domname'] = "GLOBAL." + $("#companyname").val() + "." + $("#projectname").val();
   data['firstname'] = $("#firstname").val();
   data['lastname'] = $("#lastname").val();
   data['realname'] = $("#firstname").val() + " " + $("#lastname").val();
   data['email'] = $("#email").val();
   data['tel'] = $("#phone").val();
   data['userid'] = $("#username").val();
   data['pw'] = $("#newpassword").val();

   $.ajax({
      type : "POST",
      dataType : 'json',
      data: data,
      url : "API/signup"
     }).done(function(res) {
      if (res['err'] != "")
      {
       $("#login_err").html(res.err);
       return;
      }
      parent.$("#login_container_dialog").dialog("close");
      $.unblockUI();
      $.confirm({
       boxWidth: '30%',
       useBootstrap: false,
       title: 'Sign-up Complete',
       content: 'Please login with your new userid and password.<br><br>The Hipster Store Sample application will take a few minutes to provision into your domain.',
       buttons: {
           ok: function () {
            location.href = location.protocol + '//' + location.host + location.pathname.replace('Home', 'Login');
           }
         }
      });
      console.log(res);
      $.ajax({
       type : "POST",
       dataType : 'json',
       data: data,
       url : "API/adddemo"
      }).done(function(data) {
       console.log(data);
      });
     });
  }
 }

 function DoLogin(dlg)
 {
  console.log("DoLogin");
   var logindlg = parent.$("#login_container_dialog");
   var form = logindlg.find("#loginform");
   var mypw = "";
   var myuserid  = "";
   loginformData = form.serialize();

   var parts = loginformData.split("&");

   for (var x = 0; x < parts.length; x++) {
     if (parts[x].indexOf("username=") >= 0)
     myuserid = parts[x].substring(9);
     if (parts[x].startsWith("password="))
     mypw = parts[x].substring(9);
   }

   if (forcepwchange) {
     var oldpw = $("#password").val();
     var newpw = $("#newpassword").val();

     $.ajax({
      type : "POST",
      dataType : 'json',
      url : "ChangePassword?oldpw=" + encodeURIComponent(oldpw) + "&newpw=" + encodeURIComponent(newpw) + "&username=" + myuserid,
      async : false
     }).done(function(data) {
      console.log(data.errtext);

      if (data.errtext != "") {
         $("#login_err").html(data.errtext);
         loggedin = 'N';
         admin = 'N';
         SetCookie("admin",admin);
         SetCookie("loggedin",loggedin);
         console.log("newUser="+newUser);
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
//                SetCookie("p1",myuserid);
//                SetCookie("p2",newpw);
                SetCookie("admin",isAdmin);
                SetCookie("loggedin",loggedin);
        } else if (data.toLowerCase() == "Login Admin".toLowerCase()) {
          loggedin = 'Y';
          admin = 'N';
//              SetCookie("p1",myuserid);
//              SetCookie("p2",newpw);
              SetCookie("admin",isAdmin);
              SetCookie("loggedin",loggedin);
        }
        if (loggedin = "Y") {
            parent.$("#login_container_dialog").dialog("close");
         $.sessionTimeout({
             logoutUrl : "Logout",
             redirUrl : "Logout",
             keepAliveUrl : "KeepAlive"
            });

            EnableTabs("application_menu");
  //          SetCookie("logindata",loginformData);
            openList(event, 'application');
             } else {
            loggedin = "N";
            admin = "N";
            SetCookie("admin",admin);
                SetCookie("loggedin",loggedin);
         }
        });
       }
      });
    } else {
	    $.ajax({
	     url : "API/doms4user/" + myuserid,
	     dataType : 'json',
	     type : 'GET',
	     success : function(res)
	     {
		  if (res.length > 1)
		  {
		  html = "<div id=\"select-domain\" style=\"list-style:none\">";
		  for (i=0;i<res.length;i++)
		  {
		   html += "<li class=\"ui-widget-content\">" + res[i] + "</li>";
		  }

		  html += "</div>";

		  $("#modal").html(html);

		  $("#select-domain").selectable();

		  $("#modal").dialog({
		      autoOpen: false,
		      modal: true,
      		  height: 200,
      		  title: "Select Domain",
		      buttons: {
		        "OK": function() {
		          var selectedItems = $("#select-domain .ui-selected").map(function() {
		            return $(this).text();
		          }).get().join(", ");

		          loginformData = loginformData.replace("username=", "username=" + encodeURIComponent(selectedItems) + ".");
		          $(this).dialog("close");
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
				       userdom = data.domain;
				       userdomid = data.domainid;
				       domlist = data.domlist.split(',');
				       isSaas = data.issaas;
				       licType = data.lictype;
				       licCnt  = data.liccnt;
				       if (isSaas == 'Y' && licType == "OSS")
				        hideOSS = "Y";
				       else
				        hideOSS = "N";

                if (!hideOption())
                  $("#verttab_setup").show();
                else
                  $("#verttab_setup").hide();

				//       SetCookie("p1",myuserid);
				//       SetCookie("p2",mypw);
				       SetCookie("admin",isAdmin);
				       SetCookie("loggedin",loggedin);
				      } else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
				       console.log("2) admin login");
				       loggedin="Y";
				       admin="Y";
				       isAdmin="Y";
				       newUser=data.newuser;
				       console.log("datefmt="+data.datefmt);
				       userdatefmt = data.datefmt;
				       usertimefmt = data.timefmt;
				       userdom = data.domain;
				       userdomid = data.domainid;
				       domlist = data.domlist.split(',');
				       isSaas = data.issaas;
				       licType = data.lictype;
				       licCnt  = data.liccnt;
				       if (isSaas == 'Y' && licType == "OSS")
				        hideOSS = "Y";
				       else
				        hideOSS = "N";

                if (!hideOption())
                  $("#verttab_setup").show();
                else
                  $("#verttab_setup").hide();

				//       SetCookie("p1",myuserid);
				//       SetCookie("p2",mypw);
				       SetCookie("admin",isAdmin);
				       SetCookie("loggedin",loggedin);
				      }
				      if (loggedin=="Y") {
				//       SetCookie("p1",myuserid);
				//       SetCookie("p2",mypw);
				       SetCookie("admin",isAdmin);
				       SetCookie("loggedin",loggedin);
				        parent.$("#login_container_dialog").dialog("close");
				        $.sessionTimeout({
				          logoutUrl : "Logout",
				          redirUrl : "Logout",
				          keepAliveUrl : "KeepAlive"
				        });

				        EnableTabs("application_menu");
				 //       SetCookie("logindata",loginformData);
				        openList(event, 'application');
				      } else {
				        if (data.Msg == "Password must be changed") {
				         newUser=data.newuser;
				          forcepwchange = true;
				         $(".ui-dialog-buttonpane button:contains('Login')").button('disable');
				          parent.$("#login_container_dialog").dialog("option", "height", 450);
				          $("#logintab").find("tr.newpw").show();
				        }
				        $("#login_err").html(data.Msg);
				      }
				     });
		        },
		        "Cancel": function() {
		          $(this).dialog("close");
		        }
		      }
		    });

		   $("#modal").dialog("open");
	      }
	     else
	     {
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
		       userdom = data.domain;
		       userdomid = data.domainid;
		       domlist = data.domlist.split(',');
		       isSaas = data.issaas;
		       licType = data.lictype;
		       licCnt  = data.liccnt;
		       if (isSaas == 'Y' && licType == "OSS")
		        hideOSS = "Y";
		       else
		        hideOSS = "N";

            if (!hideOption())
              $("#verttab_setup").show();
            else
              $("#verttab_setup").hide();

		//       SetCookie("p1",myuserid);
		//       SetCookie("p2",mypw);
		       SetCookie("admin",isAdmin);
		       SetCookie("loggedin",loggedin);
		      } else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
		       console.log("2) admin login");
		       loggedin="Y";
		       admin="Y";
		       isAdmin="Y";
		       newUser=data.newuser;
		       console.log("datefmt="+data.datefmt);
		       userdatefmt = data.datefmt;
		       usertimefmt = data.timefmt;
		       userdom = data.domain;
		       userdomid = data.domainid;
		       domlist = data.domlist.split(',');
		       isSaas = data.issaas;
		       licType = data.lictype;
		       licCnt  = data.liccnt;
		       if (isSaas == 'Y' && licType == "OSS")
		        hideOSS = "Y";
		       else
		        hideOSS = "N";

            if (!hideOption())
              $("#verttab_setup").show();
            else
              $("#verttab_setup").hide();

		//       SetCookie("p1",myuserid);
		//       SetCookie("p2",mypw);
		       SetCookie("admin",isAdmin);
		       SetCookie("loggedin",loggedin);
		      }
		      if (loggedin=="Y") {
		//       SetCookie("p1",myuserid);
		//       SetCookie("p2",mypw);
		       SetCookie("admin",isAdmin);
		       SetCookie("loggedin",loggedin);
		        parent.$("#login_container_dialog").dialog("close");
		        $.sessionTimeout({
		          logoutUrl : "Logout",
		          redirUrl : "Logout",
		          keepAliveUrl : "KeepAlive"
		        });

		        EnableTabs("application_menu");
		 //       SetCookie("logindata",loginformData);
		        openList(event, 'application');
		      } else {
		        if (data.Msg == "Password must be changed") {
		         newUser=data.newuser;
		          forcepwchange = true;
		         $(".ui-dialog-buttonpane button:contains('Login')").button('disable');
		          parent.$("#login_container_dialog").dialog("option", "height", 450);
		          $("#logintab").find("tr.newpw").show();
		        }
		        $("#login_err").html(data.Msg);
		      }
		     });
		   }
		 }
	    });

	// Display List dialog
	// on OK - combine domain with userid and perform login


   }

 }

 function DoOAuthLogin(username,access_token,provider)
 {
  var url = 'Login?username=' + username + "&password=" + access_token + "&provider=" + provider;
  console.log(url);
  $.ajax(
    {
     url : url,
     type : 'POST',
     dataType : 'json'}).success(function(data)
     {
  console.log("data=");
  console.log(data);
        if (data.Msg.toLowerCase() == "Login OK".toLowerCase()) {
            parent.$("#login_container_dialog").dialog("close");
   console.log("Normal login");
   loggedin="Y";
   admin="N";
   isAdmin="N";
   newUser=data.newuser;
   console.log("datefmt="+data.datefmt);
   userdatefmt = data.datefmt;
   usertimefmt = data.timefmt;
   userdom = data.domain;
   userdomid = data.domainid;
   domlist = data.domlist.split(',');
   isSaas = data.issaas;
   licType = data.lictype;
   licCnt  = data.liccnt;
   if (isSaas == 'Y' && licType == "OSS")
    hideOSS = "Y";
   else
    hideOSS = "N";

    if (!hideOption())
      $("#verttab_setup").show();
    else
      $("#verttab_setup").hide();

//   SetCookie("p1",username);
//   SetCookie("p2",provider + ":" + access_token);
   SetCookie("admin",isAdmin);
   SetCookie("loggedin",loggedin);
  } else if (data.Msg.toLowerCase() == "Login Admin".toLowerCase()) {
            parent.$("#login_container_dialog").dialog("close");
   console.log("2) admin login");
   loggedin="Y";
   admin="Y";
   isAdmin="Y";
   newUser=data.newuser;
   console.log("datefmt="+data.datefmt);
   userdatefmt = data.datefmt;
   usertimefmt = data.timefmt;
   userdom = data.domain;
   userdomid = data.domainid;
   domlist = data.domlist.split(',');
   isSaas = data.issaas;
   licType = data.lictype;
   licCnt  = data.liccnt;
   if (isSaas == 'Y' && licType == "OSS")
    hideOSS = "Y";
   else
    hideOSS = "N";

    if (!hideOption())
      $("#verttab_setup").show();
    else
      $("#verttab_setup").hide();

//   SetCookie("p1",username);
//   SetCookie("p2",provider + ":" + access_token);
   SetCookie("admin",isAdmin);
   SetCookie("loggedin",loggedin);
  }
  else
  {
        $("#login_err").html(data.Msg);
        loggedin = 'N';
        admin = 'N';
        SetCookie("admin",admin);
        SetCookie("loggedin",loggedin);
        console.log("newUser="+newUser);
        }

  if (loggedin=="Y") {
//   SetCookie("p1",username);
//   SetCookie("p2",provider + ":" + access_token);
   SetCookie("admin",isAdmin);
   SetCookie("loggedin",loggedin);
    parent.$("#login_container_dialog").dialog("close");
    $.sessionTimeout({
      logoutUrl : "Logout",
      redirUrl : "Logout",
      keepAliveUrl : "KeepAlive"
    });

    EnableTabs("application_menu");
//    SetCookie("logindata",loginformData);
    openList(event, 'application');
  }
     });
 }

 function ActivateSubTabs()
 {

//  $("#bookmark-Security-Posture").hide();
//  $("#bookmark-Impact-Assessment").hide();
//  $("#bookmark-DevOps-Details").hide();
//  $("#bookmark-Calendar").hide();
//  $("#bookmark-Audit-Access").hide();

  if (displaySubTabs == "N")
  {
   $("#panel_container_right").hide();
   return;
  }

  $("#scorecard_dd").hide();

  if (parent.$('#summ_data_edit').is(':visible') && !msgbox_showing)
  {
   msgbox_showing = true;
   $.confirm({
    boxWidth: '30%',
    useBootstrap: false,
    title: 'Save',
    content: 'Would you like to save your changes?',
    buttons: {
        ok: function () {
            msgbox_showing = false;
            summOK(isProfile, "");
            ActivateSubTabs();
        },
        close: function () {
          msgbox_showing = false;
          parent.$('#summ_data_edit').hide();
          ActivateSubTabs();
        }
      }
   });
   return;
  }

  $("#panel_container_right_list").hide();
  $("#panel_container_right").css('left', '278px');


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
    objid = 1; //OTID, first two chars are Object Type
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
    $(key).hide();
    $(tmp).hide();
   }
  }

  HideSubTabs(oldtreename, currenttree);

  if (objtypeName == "Notify")
   objtypeName = "Notifier";

  if (objtypeName == "Server")
   objtypeName = "Endpoint";

  if (classname == "application version")
   classname = "appversion";

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

  classname = hookActivateSubTabs();

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

  $("#panel_container_right").hide();
  $("#panel_container_historyandpending").hide();

  if (currenttree == "#servers_tree" && lastSelectedNode.substring(0,2) == "se")
  {
   $("#panel_container_right").show();
   $("#tabs-ServerStatus").hide();
  }
  if (currenttree == "#environments_tree")
  {
   if (lastSelectedNode.substring(0, 2) == "se")
   {
    $("#panel_container_right").show();
    $("#tabs-Servers").hide();
    $("#tabs-Applications").hide();
    $("#tabs-Reports").hide();
    $("#tabs-ServerStatus").hide();
    $("#tabs-SrvComponents").hide();
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
    $("#tabs-EnvDefects").hide();
   }
  }
  if (currenttree == "#notifiers_tree" && lastSelectedNode.substring(0, 2) == "te")
  {
   $("#panel_container_right").show();
   $("#tabs-Access").hide();
   $("#tabs-Properties").hide();
   if (currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Access" || currentSubTabsSelection["#notifiers_tree"][0] == "tabs-Properties" )
   {
    currentSubTabsSelection["#notifiers_tree"] = ["tabs-General", ""];
    tabId  = "tabs-General";
    $("#tabs-General").addClass("current active");
   }
  }
  if (currenttree == "#notifiers_tree" && lastSelectedNode.substring(0, 2) == "no")
  {
   $("#panel_container_right").show();
   $("#tabs-Access").hide();
   $("#tabs-Properties").hide();
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
   $("#panel_container_right").show();
   $("#tabs-Servers").hide();
   $("#tabs-Applications").hide();
   $("#tabs-Reports").hide();
  }
  else if (currenttree == "#applications_tree" && lastSelectedNode.substring(0, 2) == "av")
  {
   $("#panel_container_right").hide();
   $("#tabs-Versions").hide();
  // $("#tabs-Environments").show(); // was hide but need environments tab for app versions
  }
  else if (currenttree == "#applications_tree" && lastSelectedNode.substring(0, 2) == "ap")
  {
   $("#panel_container_right").hide();
  // $("#tabs-Versions").show();
  // $("#tabs-Environments").show();
  }
  else if (currenttree == "#components_tree" && lastSelectedNode.substring(0, 2) == "cv")
  {
   $("#panel_container_right").hide();
   $("#tabs-ComponentVersions").hide();
  }
  else if (currenttree == "#components_tree" && lastSelectedNode.substring(0, 2) == "co")
  {
   $("#panel_container_right").hide();
   $("#tabs-ComponentVersions").hide();
  }
  else if (currenttree == "#procedures_tree" && (lastSelectedNode.substring(0, 2) == "pr" || lastSelectedNode.substring(0, 2) == "fn"))
  {
   $("#panel_container_right").show();
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
        $("#tabs-Args").hide();
        $("#tabs-ProcBody").hide();
        break;

       case 2: // DMScript Function in database
        $("#tabs-Args").hide();
        $("#tabs-ProcBody").hide();
        break;

      case 3: // Function provided by local external script or program
       $("#tabs-Args").hide();
       $("#tabs-ProcBody").hide();
       break;

      case 4: // Function provided by remote external script or program
       $("#tabs-Args").hide();
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
  else if (currenttree == "#domains_tree")
  {
   $("#panel_container_right_list").hide();
   $("#panel_container_right").show();
   console.log("switching to build job tabId="+tabId);
   $("#tabs-General").hide();
   $("#tabs-Tasks").hide();
   $("#tabs-Access").hide();
   if (tabId != "tabs-General") {
    tabId="tabs-General";
    currentSubTabsSelection["#domains_tree"] = ["tabs-General", ""];
       $("#tabs-General").addClass("current active");
   }
  }
  else
   hookTabs(currenttree);

  console.log("tabId="+tabId+" currenttree="+currenttree+" lastSelectedNode="+lastSelectedNode+" objtype="+objtype);


  var isAdmin = "N";
  if (canView[currenttree])
   isAdmin = "Y";

  switch (tabId)
  {
  case "tabs-General":
   {
	var vercnt = 0;
	versiondd_list = [];

	if (objtype == "co" || objtype == "cv" || objtype == "ap" || objtype == "av")
	{
		$.ajax({
	     url : "/dmadminweb/API/versionlist/" + objtype + objid,
	     dataType : 'json',
	     type : 'GET',
	     async: false,
	     success : function(res)
	     {
		  versiondd_list = res.data;
		  vercnt = versiondd_list.length;
	     }
	   	});


	    console.log("doing general - objtype="+objtype+" currenttree="+currenttree);
	    $("#panel_container_right").show();
	    if (objtype == "ap" || objtype == "co")
	    {
		 var button_html = '<button id="versions" class="versions ui-button ui-corner-all ui-widget" onclick="openVersionDlg(event, objtype, objid)"><svg aria-hidden="true" focusable="false" role="img" class="octicon octicon-history" viewBox="0 0 16 16" width="16" height="16" fill="currentColor" style="display: inline-block; user-select: none; vertical-align: text-bottom; overflow: visible;"><path d="m.427 1.927 1.215 1.215a8.002 8.002 0 1 1-1.6 5.685.75.75 0 1 1 1.493-.154 6.5 6.5 0 1 0 1.18-4.458l1.358 1.358A.25.25 0 0 1 3.896 6H.25A.25.25 0 0 1 0 5.75V2.104a.25.25 0 0 1 .427-.177ZM7.75 4a.75.75 0 0 1 .75.75v2.992l2.028.812a.75.75 0 0 1-.557 1.392l-2.5-1A.751.751 0 0 1 7 8.25v-3.5A.75.75 0 0 1 7.75 4Z"></path></svg>&nbsp;' + vercnt + ' Versions</button>';
	     button_html += '<button class="compare ui-button ui-corner-all ui-widget" onclick="openDiffDlg(event, objtype, objid)"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>';

	     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
	     parent.$("#header_icon").html("<h1 style=\"display:inline;\">" + objtypeName + ":  " + objName + "</h1>" + button_html);
	    }
	    else
	    {
		 var button_html = '<button id="versions" class="versions ui-button ui-corner-all ui-widget" onclick="openVersionDlg(event, objtype, objid)"><svg aria-hidden="true" focusable="false" role="img" class="octicon octicon-history" viewBox="0 0 16 16" width="16" height="16" fill="currentColor" style="display: inline-block; user-select: none; vertical-align: text-bottom; overflow: visible;"><path d="m.427 1.927 1.215 1.215a8.002 8.002 0 1 1-1.6 5.685.75.75 0 1 1 1.493-.154 6.5 6.5 0 1 0 1.18-4.458l1.358 1.358A.25.25 0 0 1 3.896 6H.25A.25.25 0 0 1 0 5.75V2.104a.25.25 0 0 1 .427-.177ZM7.75 4a.75.75 0 0 1 .75.75v2.992l2.028.812a.75.75 0 0 1-.557 1.392l-2.5-1A.751.751 0 0 1 7 8.25v-3.5A.75.75 0 0 1 7.75 4Z"></path></svg>&nbsp;' + vercnt + ' Versions</button>';
	     button_html += '<button class="compare ui-button ui-corner-all ui-widget" onclick="openDiffDlg(event, objtype, objid)"><i class="fal fa-equals" aria-hidden="true" style="padding-right:5px;"></i>Compare</button>';

	     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
	     parent.$("#header_icon").html("<h1 style=\"display:inline;\">" + objtypeName + ": " + objName + "</h1>"  + button_html);
	    }
    }
    else
    {
	 $("#versions").hide();
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
     parent.$("#header_icon").html("<h1 style=\"display:inline;\">" + objtypeName + ": " + objName + "</h1>");
	}
    parent.$("#title_icon").html("");

    reset_html = '<tbody></tbody>';

    parent.$("#summ").html(reset_html);

    console.log("LoadSummaryData, objtypeAsInt="+objtypeAsInt+" objtype="+objtype);

    if (objtype == "bj")
     addParams = "&be="+lastSelectedNode;
    else if (objtype != "pr" && objtype != "fn")
     addParams = "";

    LoadSummaryData("summ", objtypeAsInt, objtype, objid, addParams);
    addParams = "";
   }
   break;

  case "tabs-Calendar":
   if (objtype == "en" && currenttree == "#environments_tree")
   {
    $("#panel_container_right").show();
    $("#title_icon").html("");
    parent.$("#right_panel_title_area").html("");
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
   }
   break;

  case "tabs-PackageComponents":
   if (((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree") || ((objtype == "co" || objtype == "cv") && currenttree == "#components_tree"))
   {
    if ((objtype == "av" || objtype == "ap") && currenttree == "#applications_tree")
    {
     $("#tabs-General-row-15").hide();
     $("#tabs-General-row-15b").hide();
     $("#tabs-General-row-20").hide();
     $("#tabs-General-row-30").hide();
     $("#tabs-General-row-32").hide();
     $("#tabs-General-row-28").hide();
     $("#audit-access").hide();
     $("#tabs-General-row-35").hide();
     $("#tabs-General-row-35a").hide();
     $("#tabs-General-row-40").hide();
     $("#tabs-General-row-lic").hide();
     $("#row-10-right-panel").hide();
    }
    else
    {
  //   $("#tabs-General-row-15").show();
    //   $("#tabs-General-row-15a").show();
     $("#tabs-General-row-20").show();
     $("#tabs-General-row-30").show();
     $("#tabs-General-row-32").show();
     $("#audit-access").show();
     $("#tabs-General-row-35").show();
     $("#tabs-General-row-35a").show();
     $("#tabs-General-row-40").show();
     $("#tabs-General-row-lic").show();
    }

    $("#panel_container_right").show();
    if (objtype == "ap" || objtype == "co")
    {
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
     parent.$("#header_icon").html("<h1 class=\"" + classname + "\">" + objtypeName + ":  " + objName + "</h1>");
    }
    else
    {
     parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
     parent.$("#header_icon").html("<h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
    }
    parent.$("#title_icon").html("");
    console.log("GetComponents?appid=" + objid);
    LoadComponentsData();
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#title_icon").html("");
    $("#tabs-PackageComponents-data").html(
      "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container compversions\">" + "<div id=\"innercomp\" style=\"height:600px;width:100%;height:100%;\">" + "</div>" + "</div>"
        + "<div class=\"accordionpanel\" id=\"ag\">" + "<h4 class=\"ui-widget-header\" style=\"margin:0\" >Component Hub</h4><div id=componentlist align=\"left\" style=\"width:256px;overflow-x:auto;height:97%;\"></div>" + "</div>" + "</div>");
    parent.$("#title_icon").html("");
   }
   break;

  default:
   hookTabId(tabId, classname);
   break;
  }

  $("#" + tabId + "-data").show();

  if (tabId == "tabs-General")
  {
   plot1.replot();
   plot2.replot();
  }
  if (tabId == "tabs-Calendar") {
   console.log(">>>>>> TABS RENDER");
   console.trace();
     $('#calendar').fullCalendar('render');
   }

  if (tabId == "tabs-DomainSummary")
  {
   if (lastSelectedNode.substring(0, 2) == "do") {
    var colours=["#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0"];
    $.ajax({
     url : "SubDomains?domainid=" + objid + "&ds=Y",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      $("#domsumm").text("");
      // Calculate max height for each subdomain
      var maxdepth=$(window).height()-140;
      console.log("initial maxdepth="+maxdepth+" res.length="+res.length);
      if (res.length==0) {
       // No sub-domains
       $("#tabs-DomainSummary").hide();
       return;

      }
      for (i=0;i<res.length;i++) {
       if (!res[i].lifecycle) {
        // not a lifecycle domain
        $("#tabs-DomainSummary").hide();
        return;
       }
       var uac = res[i].appcount;
       var udh = (uac<3)?220:80+(uac*50);
       var dac = 0;
       for (i2=0;i2<res[i].envcount;i2++) {
        console.log("res["+i+"].envs["+i2+"].appcount="+res[i].envs[i2].appcount);
        var ac=res[i].envs[i2].appcount;
        var dh=(ac<3)?220:80+(ac*50);
        dac += dh
       }
       console.log(i+") uac="+uac+" dac="+dac);
       if (udh+dac > maxdepth) maxdepth = udh+dac;
      }
      console.log("maxdepth="+maxdepth);

      for(i=0;i<res.length;i++) {
       var x="<div style='border-radius: 5px; border:2px solid #d4dde7;background-color: "
      +colours[i]
        +";width:233px;height:"+maxdepth+"px;float:left'>"
        +"<table border=0><tr>"
        +"<td><img src='css/images/lifecycle_24x.png'></td>"
        +"<td valign='center'>"+res[i].name+"</td>"
        +"</tr></table><hr>";
       // undeployed apps
       x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ffddaa;width:210px;min-height:190px;'>"
        +"<table border=0><tr>"
        +"<td valign='center'>"
        +"Undeployed Applications"
        +"</td>"
        +"</tr></table><hr>";
       for (z=0;z<res[i].appcount;z++) {
        switch(res[i].apps[z].status) {
        case "Y":
         icon="approved_24x.png";
         break;
        case "N":
         icon="rejected_24x.png";
         break;
        default:
         icon=(res[i].apps[z].pid>0)?"appversion_24x.png":"application_24x.png";
           break;
        }
        console.log("icon for "+res[i].apps[z].name+" is "+icon);
        var pf=(res[i].apps[z].pid>0)?"av":"ap";
        if (z==0) {

        }
        x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ccbbaa;width:190px;height:35px;'>"
         +"<table border=0><tr>"
         +"<td><img src='css/images/"+icon+"'></td>"
         +"<td valign='center'>"
         +"<a href='javascript:SwitchDisplay(\""+pf+res[i].apps[z].id+"\");'><b>"+res[i].apps[z].name+"</b></a>"
         +"</td>"
         +"</tr></table>"
         +"</div>"
         +"<div style='height:10px;width:210px;margin 0 auto'></div>";
       }
       x=x+"</div><div style='height:10px;width:160px;margin 0 auto'></div>";
       x=x+"<div style='height:10px;width:160px;margin 0 auto'></div>";
       for (e=0;e<res[i].envcount;e++) {
         console.log("env="+res[i].envs[e].name);
         x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ffeedd;width:210px;min-height:190px;'>"
         +"<table border=0><tr>"
         +"<td><img src='css/images/environment_24x.png'></td>"
         +"<td valign='center'>"
         +"<a href='javascript:SwitchDisplay(\"en"+res[i].envs[e].id+"\");'><b>"+res[i].envs[e].name+"</b></a>"
         +"</td>"
         +"</tr></table><hr>";
         // Apps in env
         console.log("res["+i+"].envs["+e+"]");
         console.log(res[i].envs[e]);
         for (a=0;a<res[i].envs[e].appcount;a++) {
          console.log("app="+res[i].envs[e].apps[a].name+" pid="+res[i].envs[e].apps[a].pid+" status="+res[i].envs[e].apps[a].status);
          switch(res[i].envs[e].apps[a].status) {
          case "Y":
           icon="approved_24x.png";
           break;
          case "N":
           icon="rejected_24x.png";
           break;
          default:
           icon=(res[i].envs[e].apps[a].pid>0)?"appversion_24x.png":"application_24x.png";
             break;
          }
          console.log("icon="+icon);
          var pf=(res[i].envs[e].apps[a].pid>0)?"av":"ap";
          x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ccbbaa;width:190px;height:35px;'>"
          +"<table border=0><tr>"
          +"<td><img src='css/images/"+icon+"'></td>"
          +"<td valign='center'>"
          +"<a href='javascript:SwitchDisplay(\""+pf+res[i].envs[e].apps[a].id+"\");'><b>"+res[i].envs[e].apps[a].name+"</b></a>"
          +"</td>"
          +"</tr></table>"
          +"</div>"
          +"<div style='height:10px;width:190px;margin 0 auto'></div>";
         }
         x=x+"</div><div style='height:20px;width:210px;margin 0 auto'></div>";
       }
        x=x+"</div>"
        +"<div style='width:7px;height:20px;float:left'></div>";

       $("#domsumm").append(x);
      }
      $("#tabs-DomainSummary").hide();
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
  }
  }

  if ($("#panel_container_right").is(":hidden")) {
   $("#panel_container_right").show();
   displayhome=true;
   ShowHome(false,ShowingInitialHelp);
   displayhome=false;
   console.log($("#panel_container_right show HOME"));
  }

  $("#tabs-General").hide();
  $("#bookmark-Security-Posture").hide();
  $("#bookmark-Impact-Assessment").hide();
  $("#bookmark-Proc-Definition").hide();
  $("#bookmark-Calendar").hide();
  $("#security-posture").hide();
  $("#impact-assessment").hide();
  $("#calendar-section").hide();
  $("#proc-definition").hide();


  $("#bookmark-DevOps-Details").show();
  $("#devops-details").show();
  $("#bookmark-Audit-Access").show();
  $("#audit-access").show();
  $("#right_panel_tabs").css("top", "-20px");

  if (objtype == "co" || objtype == "cv" || objtype == "ap" || objtype == "av")
  {
	$("#bookmark-Security-Posture").show();
    $("#bookmark-Impact-Assessment").show();
    $("#security-posture").show();
    $("#impact-assessment").show();
  }
  else if (objtype == "en")
  {
	$("#bookmark-Impact-Assessment").show();
    $("#bookmark-Calendar").show();
    $("#calendar-section").show();
    $("#impact-assessment").show();
  }
  else if (objtype == "se")
  {
	$("#bookmark-Impact-Assessment").show();
    $("#impact-assessment").show();
  }
  else if (objtype == "te")
  {
    $("#bookmark-Audit-Access").hide();
    $("#audit-access").hide();
  }
  else if (objtype == "fn" || objtype == "pr")
  {
   $("#bookmark-Proc-Definition").show();
   $("#proc-definition").show();
  }
  else if (objtype == "do")
  {
   $("#bookmark-DevOps-Details").hide();
   $("#devops-details").hide();
   $("#bookmark-Audit-Access").hide();
   $("#audit-access").hide();
  }
 }

 function Scroll2Bookmark(bookmarkId)
 {
  const container = document.getElementById("right_panel_main");
  const bookmark = document.getElementById(bookmarkId);

bookmark.scrollIntoView({behavior: 'smooth'});
 }

 function SetActiveMenu(menuname)
 {

  console.log("SetActiveMenu("+menuname+")");
  if (oldmenu.length > 0 && $("#" + oldmenu).length > 0)
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
  explorerFlasher = false; // switch off highlighting if in help mode
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

  ActivateSubTabs();

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

 function EnableTabs(menuname,nocallback)
 {
  viewFlasher=false;
  currentmenu = menuname;
  SetActiveMenu(menuname);

  nocallback = nocallback || false;
  console.log("EnableTabs("+menuname+") ShowingInitialHelp="+ShowingInitialHelp);

   displaySubTabs = "N";
   $("#panel_container_menu").show();
   $("#panel_container").show();
   $("#panel_container_historyandpending").hide();

   var i, tabcontent, tablinks;

   // Get all elements with class="tablinks" and remove the class "active"
   var act = 0;
   tablinks = document.getElementsByClassName("tablinks");
   for (i = 0; i < tablinks.length; i++) {
    var className = tablinks[i].className;
    if (className.includes("active"))
     act++;
   }

   if (act == 0)
   {
    tablinks[0].className = tablinks[0].className + " active";
    getAppList("list");
   }

   hookHide();

   $("#panel_container_right_list").show();
   $("#panel_container").hide();
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
   $("#domains_tab").hide();
   $("#types_tab").hide();

  console.log("EnableTabs("+menuname+") DONE");
  ActivateSubTabs();
 }

 function openList(evt, listName) {

  if (parent.$('#summ_data_edit').is(':visible') && !msgbox_showing)
  {
   msgbox_showing = true;
   $.confirm({
    boxWidth: '30%',
    useBootstrap: false,
    title: 'Save',
    content: 'Would you like to save your changes?',
    buttons: {
        ok: {
         text: "Yes",
         action: function () {
            msgbox_showing = false;
            summOK(isProfile, "");
            openList(evt,listName);
           }
        },
        close: {
          text: "No",
          action: function () {
            msgbox_showing = false;
            parent.$('#summ_data_edit').hide();
            openList(evt,listName);
           }
        }
    }
   });
   return;
  }

  // Declare all variables
  var i, tabcontent, tablinks;

  $("#panel_container_historyandpending").hide();
  $("#panel_container").hide();
  $("#tabs-PackageComponents-data").hide();
  $("#tabs-PackageApplications-data").hide();
  // Get all elements with class="tablinks" and remove the class "active"
  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }

  // Show the current tab, and add an "active" class to the link that opened the tab
  var vb = $("#verttab_" + listName);
  vb.addClass("active");

  $("#panel_container_right_list").show();
  $("#panel_container_right").hide();
  if (currenttree == "#applications_tree" || currenttree == "#components_tree")
   $("#panel_container_right").css('left', '274px');
  else
   $("#panel_container_right").css('left', '396px');

  if (listName == "application")
  {
   getAppList("list");
  }
  else if (listName == "component")
  {
   getCompList("list");
  }
  else if (listName == "environment")
  {
   getEnvList("list");
  }
  else if (listName == "endpoint")
  {
   getEndpointList("list");
  }
  else if (listName == "action")
  {
   getActionList("list");
  }
  else if (listName == "procedure")
  {
   getProcedureList("list");
  }
  else if (listName == "notifier")
  {
   getNotifierList("list");
  }
  else if (listName == "repository")
  {
   getRepositoryList("list");
  }
  else if (listName == "datasource")
  {
   getDatasourceList("list");
  }
  else if (listName == "credential")
  {
   getCredentialList("list");
  }
  else if (listName == "user")
  {
   getUserList("list");
  }
  else if (listName == "group")
  {
   getGroupList("list");
  }
  else if (listName == "servercomptype")
  {
   getServerCompTypeList("list");
  }
  else if (listName == "template")
  {
   getTemplateList("list");
  }
  else
  {
   hookList(listName);
  }

}

 function openMap(evt, listName) {
  if (listName == "application")
  {
   getAppList("map");
  }
  else if (listName == "component")
  {
   getCompList("map");
  }
 }

 function delRow(event,type)
 {
  var data = null;

  if (type == "application")
   data = applist_table.rows({selected:  true}).data();
  else if (type == "component")
   data = complist_table.rows({selected:  true}).data();
  else if (type == "environment")
   data = envlist_table.rows({selected:  true}).data();
  else if (type == "endpoint")
   data =endpointlist_table.rows({selected:  true}).data();
  else if (type == "action")
   data = actionlist_table.rows({selected:  true}).data();
  else if (type == "procedure")
   data = procedurelist_table.rows({selected:  true}).data();
  else if (type == "notifier")
   data = notifierlist_table.rows({selected:  true}).data();
  else if (type == "repository")
   data = repositorylist_table.rows({selected:  true}).data();
  else if (type == "datasource")
   data = datasourcelist_table.rows({selected:  true}).data();
  else if (type == "credential")
   data = credentiallist_table.rows({selected:  true}).data();
  else if (type == "user")
   data = userlist_table.rows({selected:  true}).data();
  else if (type == "group")
   data = grouplist_table.rows({selected:  true}).data();
  else if (type == "servercomptype")
   data = servercomptypelist_table.rows({selected:  true}).data();
  else if (type == "template")
   data = templatelist_table.rows({selected:  true}).data();
  else
   data = hookDelRow(type);

  if (data == null || data.length == 0)
   return;

  var newarray=[];
  var msg = "";

  for (var i=0; i < data.length ;i++)
  {
   newarray.push(data[i]['id'].substr(2));

   if (i==0)
     msg += data[i]['name'];
   else
     msg += ", " + data[i]['name'];
  }

  var pwd = parent.$("#modal");
  var buttons = [
  {
   text : "Ok",
   click : function()
   {
    for (var i=0;i<newarray.length;i++)
    {
     var id = newarray[i];

     $.ajax(
      {
       url:"/dmadminweb/API/del/" + type + "/" + id,
       async: false,
       dataType: "json",
       success: function (res)
       {
       }
      });
    }

    parent.$("#modal").dialog("close");
    openList(event, type);
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
  pwd.html("<table border=0><tr><td valign='center'><img src='css/images/question_36px.png'></td><td><b>Are you sure?</b><br><hr>Please confirm you want to delete " + msg + "\"?</td></tr></table>");
  pwd.dialog({open : null});
  pwd.dialog("option", "title", "Delete Confirmation");
  pwd.dialog("option", "height", "auto");
  pwd.dialog("option", "width", "400px");
  pwd.dialog("option", "buttons", buttons);
  pwd.dialog('open');
 }

 function addRow(event,type)
 {
  displaySubTabs = "Y";
  if (type == "application")
  {
   currenttree = "#applications_tree";
   objtypeName = "Application";
   NewApp(null);
  }
  else if (type == "appversion")
  {
   objid = lastSelectedNode.substr(2); //OTID, first two chars are Object Type
   objtype = lastSelectedNode.substr(0,2);
   objkind="";
   currenttree = "#applications_tree";
   objtypeName = "Application";
   NewAppVer(objid);
  }
  else if (type == "compversion")
  {
   objid = lastSelectedNode.substr(2); //OTID, first two chars are Object Type
   objtype = lastSelectedNode.substr(0,2);
   objkind="";
   currenttree = "#components_tree";
   objtypeName = "Component";
   NewCompVer(objid);
  }
  else if (type == "docker" || type == "database" || type == "file")
  {
   currenttree = "#components_tree";
   objtypeName = "Component";
   NewComponent(type);
  }
  else if (type == "environment")
  {
   currenttree = "#environments_tree";
   objtypeName = "Environment";
   NewEnvironment(null);
  }
  else if (type == "endpoint")
  {
   currenttree = "#servers_tree";
   objtypeName = "Endpoint";
   NewServer(null);
  }
  else if (type == "action")
  {
   currenttree = "#actions_tree";
   objtypeName = "Action";
   NewAction(null,"G");
  }
  else if (type.includes("procedure"))
  {
   currenttree = "#procedures_tree";
   objtypeName = "Procedure";
   var parts = type.split("-ak");
   objkind=parts[1];
   addParams="&actkind=" + objkind;
   NewAction(null,"P");
  }
  else if (type.includes("function"))
  {
   currenttree = "#procedures_tree";
   objtypeName = "Function";
   var parts = type.split("-ak");
   objkind=parts[1];
   addParams="&actkind=" + objkind;
   NewAction(null,"F");
  }
  else if (type == "slack" || type == "hipchat" || type == "smtpemail" || type == "txtlocal")
  {
   currenttree = "#notifiers_tree";
   objtypeName = "Notifiers";
   NewNotifier(type);
  }
  else if (type == "filesystem" || type == "ftprep" || type == "harvest" || type == "http" || type == "meister" || type == "perforce" || type == "svn")
  {
   currenttree = "#repositories_tree";
   objtypeName = "Repositories";
   NewRepository(type);
  }
  else if (type == "ldap" || type == "odbc" || type == "jira" || type == "github" || type == "bugzilla")
  {
   currenttree = "#datasources_tree";
   objtypeName = "Data Sources";
   NewDatasource(type);
  }
  else if (type == "ck10" || type == "ck4" || type == "ck2" || type == "ck5")
  {
   currenttree = "#credentials_tree";
   objtypeName = "Credential";
   NewCredential(type);
  }
  else if (type == "user")
  {
   currenttree = "#users_tree";
   objtypeName = "User";
   NewUser(null);
  }
  else if (type == "group")
  {
   currenttree = "#groups_tree";
   objtypeName = "Groups";
   NewGroup(null);
  }
  else if (type == "servercomptype")
  {
   currenttree = "#types_tree";
   objtypeName = "Type";
   NewCompType(null);
  }
  else if (type == "template")
  {
   currenttree = "#notifiers_tree";
   objtypeName = "Notifier Template";
   NewTemplate(null);
  }
  else
  {
   hookAddRow(type);
  }
 }

 function eventOpenRow(frameid,admin,data)
 {
  $.blockUI();
  var msgid = $("body > div.blockUI.blockMsg.blockPage");
  waitForElement(msgid,function(){

  if (frameid == "applications")
   currenttree = "#applications_tree";
  else if (frameid == "components")
   currenttree = "#components_tree";
  else if (frameid == "environments")
   currenttree = "#environments_tree";
  else if (frameid == "endpoints")
   currenttree = "#servers_tree";
  else if (frameid == "actions")
   currenttree = "#actions_tree";
  else if (frameid == "procedures")
   currenttree = "#procedures_tree";
  else if (frameid == "notifiers")
   currenttree = "#notifiers_tree";
  else if (frameid == "repositories")
   currenttree = "#repositories_tree";
  else if (frameid == "datasources")
   currenttree = "#datasources_tree";
  else if (frameid == "credentials")
   currenttree = "#credentials_tree";
  else if (frameid == "users")
   currenttree = "#users_tree";
  else if (frameid == "servercomptypes")
   currenttree = "#types_tree";
  else if (frameid == "templates")
   currenttree = "#notifiers_tree";
  else
   currenttree = hookEventOpenRow(frameid);

  lastSelectedNode = data["id"];

  console.log("selected node = "+lastSelectedNode);
  objid = lastSelectedNode.substr(2); //OTID, first two chars are Object Type
  objtype = lastSelectedNode.substr(0,2);
  objkind="";

  console.log("*** SelectNode objtype="+objtype+" objid="+objid);
    if (objtype == "pr" || objtype == "fn") {
     objkind=objid.substr(objid.indexOf("-")+1);
     objid = objid.substr(0,objid.indexOf("-"));
    }
  objtypeAsInt = obj2Int[objtype][0];
  objtypeName = obj2Int[objtype][1];

  if ("type" in data && currenttree == "#procedures_tree")
  {
   if (data["type"] == "Func")
     objtypeName = "Function";
   else
    objtypeName = "Procedure";
  }

  objName = data['name'];

  if (typeof objName == "undefined")
   objName = "";

  summSavedomid = domlist[0];
  lastdomain = domlist[0];
  parentid = domlist[0];
  parenttype = "Domain";

  console.log("id="+objid+" objtype="+objtype + " objtypeAsInt=" + objtypeAsInt + " objtypeName=" + objtypeName + " objName=" + objName + " objkind=" + objkind+ " oldmenu=" + oldmenu + " currenttree=" + currenttree);

  displaySubTabs = "Y";
  ActivateSubTabs();

  });

 }

 function notifierMenu()
 {
  var pwd = parent.$("#notifierlist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }


 function addTaskMenu()
 {
  var pwd = parent.$("#tasks_header_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function addRptMenu()
 {
  var pwd = parent.$("#rpts_header_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function credMenu()
 {
  var pwd = parent.$("#credentiallist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function datasourceMenu()
 {
  var pwd = parent.$("#datasourcelist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function buildengineMenu()
 {
  var pwd = parent.$("#buildenglist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function compMenu()
 {
  var pwd = parent.$("#complist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function scorecardMenu()
 {
  var pwd = parent.$("#scorecard_dd > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }


 function envRptMenu()
 {
  var pwd = parent.$("#envlist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();

   var data = envlist_table.rows({selected:  true}).data();

   if (data != null && data.length > 0)
   {
	var envname = data[0].parent;

	if (envname == "-")
	 envname = data[0].name;

    var newhtml = "<a href=\"/dmadminweb/reports/EnvSuccessFail.html\" target=\"_blank\">Success/Failed Deployments per Environment Report</a>";


    url = "/msapi/sbom?envid=" + encodeURIComponent(data[0].id);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Export SBOM</a>";
	pwd.html(newhtml);
    dropdownShowing = true;
   }
   else
   {
    var newhtml = "<a href=\"/dmadminweb/reports/EnvSuccessFail.html\" target=\"_blank\">Success/Failed Deployments per Environment Report</a>";
	pwd.html(newhtml);
    dropdownShowing = false;
   }
 }

 function endpointRptMenu()
 {
  var pwd = parent.$("#endpointlist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function repoMenu()
 {
  var pwd = parent.$("#repositorylist_buttons > div > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function procMenu()
 {
  $("#procedurelist_buttons > div:nth-child(3) > div").hide();
  var pwd = parent.$("#procedurelist_buttons > div:nth-child(2) > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function funcMenu()
 {
  $("#procedurelist_buttons > div:nth-child(2) > div").hide();
  var pwd = parent.$("#procedurelist_buttons > div:nth-child(3) > div");

  if (pwd.is(":visible"))
    pwd.hide();
  else
    pwd.show();
 }

 function taskMenu()
 {
  if (currenttree == "#applications_tree")
  {
   var data = applist_table.rows({selected:  true}).data();

   if (!dropdownShowing && data != null && data.length > 0)
   {
    var id = lastsel_id.substr(2);
    $.ajax({
     url: "GetMenu?t=APPLICATION&id="+id+"&d=0&a=N&ct=applications",
     async: false,
     cache: false,
     context: document.body
    }).done(function(data) {
    newhtml = "";
     $.each(data,function(key,val){
      $.each(val,function(key2,val2){
       var tmo = {};
       console.log("key2="+key2+" val2="+val2);
       newhtml += "<a onclick='callback(\"" + currenttree + "\",\"" + key2 + "\",\"" + val2 + "\",this);'>" + key2 + '</a>';
       $("#apptask_buttons").html(newhtml);
       $("#apptask_buttons").css({display: "block"});
       dropdownShowing = true;
      });
     });
   });
  }
  else
  {
   $("#apptask_buttons").css({display: "none"});
   dropdownShowing = false;
  }
 }
 else
  hookTaskMenu(currenttree);
}

function rptMenu()
 {
  if (currenttree == "#applications_tree")
  {
   var data = applist_table.rows({selected:  true}).data();

   if (!dropdownShowing && data != null && data.length > 0)
   {
	var appname = data[0].parent;

	if (appname == "-")
	 appname = data[0].name;

    var newhtml = "";

    url = "/reports/scorecard.html?appid=" + data[0].id.substring(2);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Compliance Summary</a>";

    url = "/reports/frequency.html?appname=" + encodeURIComponent(appname);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Deployment Frequency</a>";

    url = "/reports/lag.html?appname=" + encodeURIComponent(appname);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Lead Time for Changes</a>";

    url = "/msapi/sbom?appid=" + encodeURIComponent(data[0].id);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Export SBOM</a>";

    $("#rptlist_buttons").html(newhtml);
    $("#rptlist_buttons").css({display: "block"});
    dropdownShowing = true;
   }
   else
   {
    $("#rptlist_buttons").css({display: "none"});
    dropdownShowing = false;
   }
  }
  else if (currenttree == "#environments_tree")
  {
   var data = envlist_table.rows({selected:  true}).data();

   if (!dropdownShowing && data != null && data.length > 0)
   {
	var envname = data[0].parent;

	if (envname == "-")
	 envname = data[0].name;

    var newhtml = "";

    url = "/msapi/sbom?envid=" + encodeURIComponent(data[0].id);
    newhtml += "<a onclick='window.open(\"" + url + "\", \"_blank\");'>Export SBOM</a>";
    $("#comp_rptlist_buttons").html(newhtml);
    $("#comp_rptlist_buttons").css({display: "block"});
    dropdownShowing = true;
   }
   else
   {
    $("#comp_rptlist_buttons").css({display: "none"});
    dropdownShowing = false;
   }
  }
  else
  {
   $("#rptlist_buttons").css({display: "none"});
   dropdownShowing = false;
  }
}

function hideOption()
{
	if (licType != "" && licType != "OSS")
	 return false;
	else
	 return true;
}

function wrapWithAnchor(val) {
    const urlPattern = /(http:\/\/|https:\/\/)/i; // case-insensitive regex to check for http or https
    if (urlPattern.test(val)) {
        return `<a href="${val}" target="_blank" style="color:#4990e2">${val}</a>`;
    }
    return val;
}

function exportCompSBOM(event, objtype, objid)
{
	url = "/msapi/sbom?compid=" + encodeURIComponent(objtype + objid);
	window.open(url, "_blank");
}

function jump2Comp(id, name)
{
  var data = {id: id, name: name}
  eventOpenRow("components",isAdmin,data);
}

function jump2App(id, name)
{
  var data = {id: id, name: name}
  eventOpenRow("applications",isAdmin,data);
}

function AddCompFile(file_data, filetype)
{
 encoded_bytes = btoa(file_data)

 var file = [];

 for (var i = 0, charsLength = encoded_bytes.length; i < charsLength; i += 76)
 {
  file.push(encoded_bytes.substring(i, i + 76));
 }

 payload = JSON.stringify({'compid': parseInt(objid), 'filetype': filetype, 'file': file})

 $.ajax({
      type : "POST",
      dataType : 'json',
      url : "/msapi/textfile",
	  contentType: "application/json",
      data : payload
     }).done(function(data) {
      if (filetype == 'readme')
        LoadReadme(objid);
      else if (filetype == 'swagger')
        LoadSwagger(objid);
      else if (filetype == 'license')
        LoadLicense(objid);
      console.log(data.errtext);
     });
}

function openPkgSearch()
{
  var pwd = parent.$("#modal");

  var buttons =
    [
          { text: "Ok",     click: function() { LaunchPkgSearch($(this)); } },
          { text: "Cancel", click: function() { $( this ).dialog("close"); } }
       ];

   var tdedit = "<form><table border=0>" +
	"<tr><td style=\"width:150px\">Package Name</td><td><input text id=\"pkgname\" name=\"pkgname\" /></td></tr>" +
	"<tr><td style=\"width:150px\">Package Version</td><td><input text id=\"pkgversion\" name=\"pkgversion\" /></td></tr>" +
	"</table></form>";

   pwd.dialog({ resizable: false, modal: true, dialogClass: "aboutsDialog", open: null });
   pwd.empty();
   pwd.html(tdedit);
   pwd.dialog("option", "title", "Package Search");
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "400px");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog('open');
}

function LaunchPkgSearch(dlg)
{
 dlg.dialog("close");
 var pkgname = $("#pkgname").val();
 var pkgversion = $("#pkgversion").val();

 window.open("/dmadminweb/reports/PkgSearch.html?pkgname=" + pkgname + "&pkgversion=" + pkgversion + "&userid=" + myuserid, '_blank');

}

function openVersionDlg(event, objname, objid) {
	  var pwd = parent.$("#modal");

  var buttons =
    [
          { text: "Ok",     click: function() { openComponent($(this)); } },
          { text: "Cancel", click: function() { $( this ).dialog("close"); } }
       ];

   var tdedit = "<form><table border=0 width=\"100%\" ><tr><td ><select name=\"version_val\" id=\"version_val\" size=\"" + versiondd_list.length + "\" style=\"width:100%;padding-right:40px;max-height:200px\">";

   for (i=0;i<versiondd_list.length;i++)
   {
	var val = versiondd_list[i];
	tdedit += "<option value=\"" + val.id + "\">" + val.name + "</option>";
   }

   tdedit += "</select></td></tr></table></form>";

   pwd.dialog({ resizable: false, modal: true, dialogClass: "aboutsDialog", open: null });
   pwd.empty();
   pwd.html(tdedit);
   pwd.dialog("option", "title", "Choose Version");
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "auto");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog('open');
}

function openComponent(dlg) {
  dlg.dialog("close");

  lastSelectedNode = $('#version_val option:selected').val();

  console.log("selected node = "+lastSelectedNode);
  objid = lastSelectedNode.substr(2); //OTID, first two chars are Object Type
  objtype = lastSelectedNode.substr(0,2);
  objkind="";

  console.log("*** SelectNode objtype="+objtype+" objid="+objid);
    if (objtype == "pr" || objtype == "fn") {
     objkind=objid.substr(objid.indexOf("-")+1);
     objid = objid.substr(0,objid.indexOf("-"));
    }
   objtypeAsInt = obj2Int[objtype][0];
   objtypeName = obj2Int[objtype][1];
   objName = $('#version_val option:selected').text();

   console.log("LoadSummaryData, objtypeAsInt="+objtypeAsInt+" objtype="+objtype);

   data["id"] = lastSelectedNode;
   data["name"] = objName;

   eventOpenRow("components",isAdmin,data);
}
