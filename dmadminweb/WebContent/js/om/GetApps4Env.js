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
function LoadApp4EnvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 $.getJSON('GetAppVersInEnvData', "envid=" + objid, function(res)
 {
  var td = "";
  var rowcnt = 0;

  console.log(res['data']);

  for (a = 0; a < res['data'].length; a++)
  {
	  console.log("res[data]");
	  console.log(res['data'][a]);
   var data = res['data'][a][1];
   var lastapp = res['data'][a][2];
   var details = res['data'][a][3];
   var lastappStr = "";
   var detailsStr = "";
   var linkval = "";
   var linkval2 = "";
   
   if (typeof data.id == "undefined")
    continue;

   if (lastapp != "") {
	   lastappStr = lastapp.name;
	   linkval2="<a href='javascript:SwitchDisplay(\""+lastapp.type+lastapp.id+"\");'>"+lastapp.name+"</a>";
   }
   if (details != "")
   {
    var type = details.type;

    if (type == "cm")
    {
     var d = convertDate(details.when);
     detailsStr = "Created at " + d.toLocaleDateString() + " " + d.toLocaleTimeString();
     detailsStr += " by " + details.tooltip;
    }
    else if (type == "de")
    { 
     if (typeof details.tooltip == "undefined")
     {
      detailsStr = "Deployment " + details.name;
     } 
     else
     {
    	 if (details.tooltip.charAt(0)=='#') {
    		 detailsStr = "<a onClick=\"DisplayDeploy(" +details.name.substr(1)+");\">" + details.name + "</a>";
    	 } else {
    		 var d = convertDate(details.tooltip);
    		 detailsStr = "<a onClick=\"DisplayDeploy(" +details.name.substr(1)+");\">" + details.name + "</a> deployed at " + d.toLocaleDateString() + " " + d.toLocaleTimeString();
    	 }
     } 
    } 
   }
   
   linkval="<a href='javascript:SwitchDisplay(\""+data.type+data.id+"\");'>"+data.name+"</a>";

   td += "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\srv" + data.id + ">" + linkval + "</td><td style=\"border: 1px solid #ddd;\">" + linkval2
     + "</td><td style=\"border: 1px solid #ddd;\">" + detailsStr + "</td></tr>";
   rowcnt++;
  }

  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
  }

  $("#" + tablename + " > tbody").html(td);

  var $table = $("#" + tablename);
  $table.delegate("tr", "click", function()
  {
   $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
   $(this).addClass('ui-state-highlight');

   $("edit_button").button("option", "disabled", false);
   $("delete_button").button("option", "disabled", false);
  });

  $("add_button").button();
  $("delete_button").button(
  {
   disabled : true
  });

  $("edit_button").button(
  {
   disabled : true
  });
 });

 $.getJSON('GetPendingEnvData', "envid=" + objid, function(res)
 {
  var td = "";
  var rowcnt = 0;
  var lastApp = "";
  var groups = [];
  var items = [];
  var now=new Date();
  
  for (a = 0; a < res['data'].length; a++)
  {
   var starttime = res['data'][a][0];
   var endtime   = res['data'][a][1];
   var app       = res['data'][a][2];
   var detailStr = res['data'][a][6];
   var calid     = res['data'][a][7];

   if (starttime == "")
    continue;
   
   if (starttime==endtime) {
	   // Full Day Event
	   endtime = starttime+86400;
   }

   console.log("starttime="+starttime+" endtime="+endtime+" app=");
   console.log(app);
   console.log("detailstr="+detailStr);
   if (app.name != lastApp) {
	   groups.push({"content": "<a href='javascript:SwitchDisplay(\""+app.type+app.id+"\");'><b>"+app.name+"</b></a>", "id": app.id, "value": app.id});
	   lastApp = app.name;
   }
   var title=detailStr;
   items.push({start: new Date(starttime*1000), end: new Date(endtime*1000), group: app.id, content: detailStr, id: calid,title: title});
  }
  var rangeStart=new Date(now.getTime()-864000000);		// 10 days in past
  var rangeEnd=new Date(now.getTime()+4320000000);		// 50 days in future
  
  // debug
  console.log("groups:");
  for (g=0;g<groups.length;g++) {
	  console.log(groups[g]);
  }
  console.log("items:");
  for (i=0;i<items.length;i++) {
	  console.log(items[i]);
  }
  console.log("end");
  // end debug
  
  console.log("rangeStart="+rangeStart.getTime()+" rangeEnd="+rangeEnd.getTime()+" now="+now.getTime());
 
 });
}

function AddApp4EnvButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveApp4EnvData(parent.$("#modal"), false);
   parent.$("#modal").dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   parent.$("#modal").dialog("close");
  }
 } ];

 var tdedit = "<form id=\"App4Envform\"><table id=\"App4Envtab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\"><I>Associate Application(s) with this environment. This tells Release Engineer which applications are allowed to be deployed to this environment.</I></td></tr>";
 tdedit += "<tr><td style=\"text-align:left;\"><B>Application:</B></td></tr>";
 tdedit += "<tr><td><div id=\"addapp\" name=\"addapp_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
 {
  open : function(event, ui)
  {
   CreateTree_Selection("#addapp","applications",false,"mainframe3");
  }
 });

 pwd.dialog("option", "title", "Add Application Association");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "500");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}

function AddAppVer4EnvButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveApp4EnvData(parent.$("#modal"), true);
   parent.$("#modal").dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   parent.$("#modal").dialog("close");
  }
 } ];

 var tdedit = "<form id=\"App4Envform\"><table id=\"App4Envtab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\"><I>Manually add Application Version(s) to this environment. Use this if the envirnonment has had the application version installed outside of Release Engineer.</td></tr>";
 tdedit += "<tr><td style=\"text-align:left;\"><B>Application Versions:<B></td></tr>";
 tdedit += "<tr><td><div id=\"addapp\" name=\"addapp_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
 {
  open : function(event, ui)
  {
   CreateTree_Selection("#addapp","applications",false,"mainframe3");
  }
 });

 pwd.dialog("option", "title", "Manually Add Application Version(s)");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "500");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}

function DeleteApp4EnvButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var key = $(this).find("td").eq(0).attr("id");
  key = key.substring(3);

  $.ajax(
  {
   url : "GetAppVersInEnvData",
   data :
   {
    reason : 'rem',
    envid : summSaveobjid,
    appid : key,
    note : 'Manually updated'
   },
   dataType : 'json',
   type : 'POST',
   success : function(res)
   {
    LoadApp4EnvData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
 });
}

function SaveApp4EnvData(instance, isVersion)
{
	console.log("In SaveApp4EnvData isVersion="+isVersion);
 var form = instance.find("#App4Envform");
 var srvlist = [];
 
 $('#addapp').jstree("get_checked",null,true).each(function(){
   if (this.id.indexOf("ap") == 0)
     srvlist.push(this.id);
 });
 
 var len = srvlist.length;

 for (var i = 0; i < len; i++)
 {
  if (srvlist[i].length == 0)
   continue;

  srvid = srvlist[i].substring(2);

   $.ajax(
   {
    url : "GetAppVersInEnvData",
    async : false,
    data :
    {
     reason : 'set',
     isversion : isVersion,
     envid : summSaveobjid,
     appid : srvid,
     note : 'Manually updated'
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
 }
 LoadApp4EnvData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);

}
