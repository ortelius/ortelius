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

function ShowAbout()
{
 $.ajax(
 {
  url : "About",
  dataType : 'json',
  type : 'GET',
  success : function(res)
  {
   var pwd = parent.$("#modal");

   var buttons =
    [
          { text: "Ok",     click: function() { UpdateLicense($(this)); } },
          { text: "Cancel", click: function() { $( this ).dialog("close"); } }
       ];

   // var tdedit = "<p style=\"padding-left:25px\">Version 7.7.0<p style=\"padding-left:25px\">Build: " + res.build + "<p style=\"padding-left:25px\">Schema: " + res.schema;
   var tdedit = "<table border=0><tr><td style=\"width:150px\">Version</td><td>9.0.0</td></tr>"
	   +"<tr><td>Build</td><td>"+res.build+"</td></tr>"
	   +"<tr><td>Database Schema:</td><td>"+res.schema+"</td></tr>"
	   +"<tr><td>Current License Key:</td><td>"+res.license+"</td></tr>"
    +"<tr><td>Instance Id:</td><td>"+res.dbuid+"</td></tr>"
	   +"</table>";
   if (isAdmin != "")
    tdedit += "<p><form>Update License: <input text id=\"lickey\" name=\"lickey\" /></form>";

   pwd.dialog({ resizable: false, modal: true, dialogClass: "aboutsDialog", open: null });
   pwd.empty();
   pwd.html(tdedit);
   pwd.dialog("option", "title", "About Ortelius");
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "300px");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog('open');
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });

}

function UpdateLicense(dlg)
{
 if ($("#lickey").val() != "")
 {
  $.ajax(
    {
     url : "About?k=" + $("#lickey").val(),
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      dlg.dialog("close");
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
      dlg.dialog("close");
     }
    });
 }

 return;
}
