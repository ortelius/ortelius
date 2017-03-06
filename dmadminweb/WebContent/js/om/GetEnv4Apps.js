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
function createBugChart(dn,open,closed)
{
	console.log("Adding bug chart to div "+dn+" open="+open+" closed="+closed);
	try {
	  plot2 = $.jqplot(dn, [[['open', open],['closed',closed]]], {
	   seriesDefaults: {
	    renderer: $.jqplot.PieRenderer,
	    trendline: { show: false },
	    rendererOptions: { padding: 1, showDataLabels: false },
	    shadow: false
	   },
	   grid: {borderColor: 'transparent', shadow: false, drawBorder: false, shadowColor: 'transparent'},
	   seriesColors: [ '#daa520', '#666666' ],
	      cursor: {
	    	  show: false,
	    	  showTooltip: false
	      }
	  });
	 } catch(e) {
	  alert('Exception: ' + e);
	 }
	 $("#"+dn ).tooltip({
		  content: open+" Open<BR>"+closed+" Closed<br>"+(open+closed)+" Total",
		  position: { my: "left+15 center", at: "right center" }
		});
}

function LoadTimeline4App(divname,objtypeAsInt,objtype,objid)
{
$.getJSON('GetPendingEnvData', "appid=" + objid, function(res) {
	  var td = "";
	  var rowcnt = 0;
	  console.log("***");
	  console.log(res);
	  console.log("***");
	  
	 var groups;
	 var isRelease = (objtype=="rl" || objtype=="rv");
	 if (isRelease) {
		 var domains=[];
		 for (var x=0;x<res.domains.length;x++) {
			 var dom = res.domains[x];
			 dom.subgroupOrder = function (a,b) {return b.subgroupOrder - a.subgroupOrder;};
			 if (x==0) {
				 dom.className = "vis-release";
			 } else {
				 var bclosed = dom.bclosed;
				 var btotal = dom.bopen + dom.bclosed;
				 dom.content =	"<table style=\"width:100%;height:65px;\" border=\"0\"><tr>"+
				 				"<td><a onClick=\"javascript:SwitchDisplay('"+dom.type+dom.id+"');\">"+dom.content+"</a></td>"+
				 				"<td style=\"width:65px;border-left:1px;border-left-style:solid;\"><div style=\"height:65px;width:65px;\" id=\"btdiv"+dom.id+"\"></div><div align=\"center\" style=\"font-size:10px;\">"+bclosed+"/"+btotal+"</div></td></tr></table>";
			 }
			 domains.push(dom);
		 }
		 console.log("domains");
		 console.log(domains);
		 groups = new vis.DataSet(domains);
	 } else {
		 groups = new vis.DataSet(res.domains);
	 }
	 
	  var items = [];
	  var now=new Date();
	  
	  if (typeof res['data'] != "undefined")
	  for (a = 0; a < res['data'].length; a++)
	  {
		  console.log("data");
		  console.log(res['data'][a]);
	   var starttime = res['data'][a][0];
	   var endtime   = res['data'][a][1];
	   var app       = res['data'][a][2];
	   var domainid  = res['data'][a][3];
	   var domain    = res['data'][a][4];
	   var env       = res['data'][a][5];
	   var detailStr = res['data'][a][6];
	   var calid     = res['data'][a][7];
	   var appid     = res['data'][a][8];
	   var appname   = res['data'][a][9]
	   var domorder  = res['data'][a][10];
	   var classname   = res['data'][a][11];
	   
	   console.log("isRelease="+isRelease+" starttime="+starttime+" endtime="+endtime+" domainid="+domainid+" domain="+domain+" calid="+calid);

	   if (starttime === "")
	    continue;
	   
	   if (starttime==endtime) {
		   // Full Day Event
		   endtime = starttime+86400;
	   }
	   
	   var title=app.name+" on "+env.name;
	   if (classname == "absent") title+=" (overdue)";
	   if (isRelease) {
		   console.log("group="+appname+" domorder="+domorder+" content="+domain+" calid="+calid);
		   var item = {start: new Date(starttime*1000), end: new Date(endtime*1000), group: appid, subgroup: domain, subgroupOrder: domorder, content: domain, id: calid,title: title};
		   if (classname != "") item.className = classname;
		   items.push(item);
	   } else {
		   var item = {start: new Date(starttime*1000), end: new Date(endtime*1000), group: domainid, content: app.name, id: calid,title: title};
		   if (classname != "") item.className = classname;
		   items.push(item);
	   }
	  }
	  
	  var rangeStart=new Date(now.getTime()-864000000);		// 10 days in past
	  var rangeEnd=new Date(now.getTime()+4320000000);		// 50 days in future
	  
	  
	  console.log("rangeStart="+rangeStart.getTime()+" rangeEnd="+rangeEnd.getTime()+" now="+now.getTime());
	 
	  
	  var container = document.getElementById(divname);
	  container.innerHTML = "";
	  var options = {
	    groupOrder: function (a, b) {
	      return a.value - b.value;
	    },
	    groupOrderSwap: function (a, b, groups) {
	    	var v = a.value;
	    	a.value = b.value;
	    	b.value = v;
	    },
	    orientation: 'both',
	    editable: false,
	    selectable: true,
	    groupEditable: false,
	    stack: false,
	    start: rangeStart,
	    end: rangeEnd
	  };
	  
	  var timeline = new vis.Timeline(container);
	  timeline.setOptions(options);
	  timeline.setGroups(groups);
	  timeline.setItems(items);
	  timeline.on('select', function (properties) {
		  console.log("calid="+properties.items[0]);
		  SwitchToCalendar(properties.items[0]);
	  });
	  if (isRelease) {
			for (var x=1;x<res.domains.length;x++) {
				createBugChart("btdiv"+res.domains[x].id,res.domains[x].bopen,res.domains[x].bclosed);
			}
	  }
	 });
	
}

function LoadEnv4AppData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 console.log("LoadEnv4AppData appid="+objid);
 $.getJSON('GetAppVersInEnvData', "appid=" + objid, function(res)
 {
  var td = "";
  var rowcnt = 0;
 console.log("res="+res);
  for (a = 0; a < res['data'].length; a++)
  {
   var data = res['data'][a][1];
   var lastapp = res['data'][a][2];
   var details = res['data'][a][3];
   var lastappStr = "";
   var detailsStr = "";

   if (typeof data.id == "undefined")
    continue;
   
   var linkval2="";
   if (lastapp != "") {
    lastappStr = lastapp.name;
    linkval2="<a href='javascript:SwitchDisplay(\""+lastapp.type+lastapp.id+"\");'>"+lastapp.name+"</a>";
   }
   
   console.log("details="+details);

   if (details != "")
   {
    var type = details.type;
    
    console.log("type="+type);

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
      var d = convertDate(details.tooltip);
      detailsStr = "<a onClick=\"DisplayDeploy(" + details.name.substr(1) + ");\">"+details.name+"</a>  deployed at " + d.toLocaleDateString() + " " + d.toLocaleTimeString();
     } 
   }
   } 

   linkval="<a href='javascript:SwitchDisplay(\""+data.type+data.id+"\");'>"+data.name+"</a>";
   td += "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"env" + data.id + "\">" + linkval + "</td><td style=\"border: 1px solid #ddd;\">" + linkval2
     + "</td><td style=\"border: 1px solid #ddd;\">" + detailsStr + "</td></tr>";
   rowcnt++;
   console.log("td="+td);
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
 LoadTimeline4App("PendingEnv4App",objtypeAsInt,objtype,objid);
}

function AddEnv4AppButton(isVersion)
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveEnv4AppData(parent.$("#modal"), isVersion);
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

 var tdedit = "<form id=\"Env4Appform\"><table id=\"Env4Apptab\" class=\"summtable\">";
 if (isVersion) {
	 tdedit += "<tr><td style=\"text-align:left;\"><I>Manually add this Application Version to an environment. Use this if the envirnonment has had this application version installed outside of Release Engineer.</td></tr>";
 } else {
	 tdedit += "<tr><td style=\"text-align:left;\"><I>Associate Environments with this Application. This tells Release Engineer to which environments this application is allowed to be deployed.</I></td></tr>";
 }
 tdedit += "<tr><td style=\"text-align:left;\"><B>Environment:</B></td></tr>";
 tdedit += "<tr><td><div id=\"addenv2app\" name=\"addenv2app_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
 {
  open : function(event, ui)
  {
   CreateTree_Selection("#addenv2app","environments",false,"mainframe3");
  }
 });

 if (isVersion) {
	 pwd.dialog("option", "title", "Manually Add Application Version to Environment");
 } else {
	 pwd.dialog("option", "title", "Add Environment Association");
 }
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "500");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}


function DeleteEnv4AppButton()
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
    envid : key,
    appid : summSaveobjid,
    note : 'Manually updated'
   },
   dataType : 'json',
   type : 'POST',
   success : function(res)
   {
    LoadEnv4AppData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
 });
}

function SaveEnv4AppData(instance, isVersion)
{
 var form = instance.find("#Env4Appform");
 var srvid = "";
 
 var srvlist = [];
 $('#addenv2app').jstree("get_checked",null,true).each(function(){
  if (this.id.indexOf("en") == 0)
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
     reason : 'add',
     isversion : isVersion,
     envid : srvid,
     appid : summSaveobjid,
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
 LoadEnv4AppData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);

}
