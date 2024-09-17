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
	   grid: {borderColor: 'transparent', shadow: false, drawBorder: false, shadowColor: 'transparent', backgroundColor: 'white'},
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
		 if ('domiains' in res)
		 {
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
					 				"<td style=\"width:65px;border-left:1px;border-left-style:solid;\"><div style=\"height:65px;width:65px;\" id=\"btdiv"+dom.id+"\"></div><div align=\"center\">"+bclosed+"/"+btotal+"</div></td></tr></table>";
				 }
				 domains.push(dom);
		     }
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
		if ('domiains' in res)
		{
			for (var x=1;x<res.domains.length;x++) {
				createBugChart("btdiv"+res.domains[x].id,res.domains[x].bopen,res.domains[x].bclosed);
			}
		}
	  }
	 });

}

/*** Load Deployment for Env ****/



function LoadDeployedEnv4AppData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 td = "";

 for (env in deployedenvs)
  {
   td += '<tr><td></td><td>' + env + '</td><td><a href="javascript:void(0);" onclick="DisplayDeploy(' + deployedenvs[env] + ')">#' + deployedenvs[env] + '</a></td></tr>';
  }

  $("#" + tablename + " > tbody").empty().append(td);

}


/****** End Load  ****/



function LoadEnv4AppData(tablename, objtypeAsInt, objtype, objid, addParams)
{

 $('#env4app_header_buttons > button.delete_button').css("color","grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 env4app_tdedit = "";
 env4app_rowcnt = 0;
 feedbackenv = {};

 if (envdropdown == "")
 {
  $.ajax(
   {
    url : "ReportsData?type=EnvList",
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     for (n = 0; n < res.data.length; n++)
     {
       envdropdown += '<option value=\"' + res.data[n].id + "\">" + res.data[n].domain + "." + res.data[n].name + '</option>';
     }
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
 }

 console.log("LoadEnv4AppData appid="+objid);

 $.getJSON('GetAppVersInEnvData', "appid=" + objid, function(res)
 {
  var td = "";
  var rowcnt = 0;
  console.log("res");
  console.log(res);
  if (!('data' in res))
   return;


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
    if (type == "hn")
    {
     detailsStr = details.name;
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
   rowcnt++;

   var key = data.name;
   var val = data.type+data.id;

   env4app_tdedit += '<tr id="env4app_row_' + rowcnt + '">';
   env4app_tdedit += '<td><input type="checkbox" id="env4app_edit_cb_' + rowcnt + '" onchange="toggleEnv4AppDelete(this);" />&nbsp;</td>';
   env4app_tdedit += '<td><select id="env4app_key_' + rowcnt + '" value="' + val + '">' + envdropdown + '</select></td>';
   env4app_tdedit += "<td><input type=\"hidden\" id=\"env4app_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
   env4app_tdedit += "<td><input type=\"hidden\" id=\"env4app_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
   td += '<tr><td><input type="checkbox" id="env4app_cb_' + rowcnt + '" onchange="toggleEnv4AppDelete(this);" />&nbsp;</td><td>' + key + '</td><td>' + detailsStr + '</td></tr>';
   env4app_rowcnt = rowcnt;

  }
  $("#" + tablename + " > tbody").empty().append(td);
  $("#" + tablename + "-edit > tbody").html(env4app_tdedit);

  if (objtype == "av")
  {
   $("#env4app > thead > tr > th:nth-child(1)").hide();
   $("#env4app > tbody > tr > td:nth-child(1)").hide();
   $("#env4app_header_buttons").hide();
  }
  else
  {
   $("#env4app > thead > tr > th:nth-child(1)").show();
   $("#env4app > tbody > tr > td:nth-child(1)").show();
   $("#env4app_header_buttons").show();
  }


  rowcnt = 0;
  for (var r = 0; r < res.data.length; r++)
  {
   data = res['data'][r][1];
   id = data.type+data.id;
   rowcnt++;

   id = $("#env4app_oldval_" + rowcnt).val();
   $("#env4app_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
  }

  if (objtype == "av")
  {
   $("#env4app_add").show();
   $("#env4app_add").empty().append('<i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add this Application Version to an Environment to log a mnaual Deployment');
  }
  else
  {
   $("#env4app_add").show();
   $("#env4app_add").empty().append('<i class="fal fa-plus" aria-hidden="true"  style="padding-right:5px"></i>Add this Application to an Environment to enable Deployments');
  }
 });
 // LoadTimeline4App("PendingEnv4App",objtypeAsInt,objtype,objid);
}

function SaveEnv4AppData(tablename)
{

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;

 id = $("#env4app_key_" + rowid).val();
 oldid = $("#env4app_oldval_" + rowid).val();

 if (id == null || id == oldid)
  return;

 $("#env4app_key_" + rowid).val(id);
 $("#env4app_oldval_" + rowid).val(id);

 srvid = id.substring(2);
 oldid = oldid.substring(2);

 if (objtype == "av")
  isVersion = "true";
 else
  isVersion = "false";

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
     oldenv : oldid,
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
   });
  LoadEnv4AppData("env4app", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function DeleteEnv4AppData(key, val,tablename)
{
  val = val.substring(2);

  $.ajax(
  {
   url : "GetAppVersInEnvData",
   data :
   {
    reason : 'rem',
    envid : val,
    appid : summSaveobjid,
    note : 'Manually updated'
   },
   dataType : 'json',
   type : 'POST',
   success : function(res)
   {
    LoadEnv4AppData("env4app", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
}


function AddEnv4AppRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
   EditEnv4AppRow(tablename);

 key = "";
 val = "";
 env4app_rowcnt++;

 var td = '<tr id="env4app_row_' + env4app_rowcnt + '">';
 td += '<td><input type="checkbox" id="env4app_edit_cb_' + env4app_rowcnt + '" onchange="toggleEnv4AppDelete(this);" />&nbsp;</td>';
 td += '<td><select id="env4app_key_' + env4app_rowcnt + '" value="' + val + '">' + envdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"env4app_oldkey_" + env4app_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"env4app_oldval_" + env4app_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelEnv4AppRow(tablename)
{
 if ($('#env4app_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#env4app_header_buttons > button.save_button').css("color","grey");
 $('#env4app_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditEnv4AppRow(tablename)
{
 $('#env4app_header_buttons > button.save_button').css("color","#3367d6");
 $('#env4app_header_buttons > button.cancel_button').css("color","#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(env4app_tdedit);

 for (var r = 0; r < env4app_rowcnt; r++)
 {
  var rowcnt = r+1;
  id = $("#env4app_oldval_" + rowcnt).val();
  $("#env4app_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteEnv4AppRow(tablename)
{
 if ($('#env4app_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb  = $("#env4app_edit_cb_" + rowid);

  if (cb.is( ":checked" ) == true)
  {
   key = $("#env4app_oldkey_" + rowid).val();
   val = $("#env4app_oldval_" + rowid).val();

   if (key != "")
     DeleteEnv4AppData(key,val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;

    cb  = $("#env4app_cb_" + rowid);

    if (cb.is( ":checked" ) == true)
    {
     key = $("#env4app_oldkey_" + rowid).val();
     val = $("#env4app_oldval_" + rowid).val();

     if (key != "")
       DeleteEnv4AppData(key,val, tablename);
    }
   });
}

function SaveEnv4AppRow(tablename)
{
 if ($('#env4app_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#env4app_header_buttons > button.save_button').css("color","grey");
 $('#env4app_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveEnv4AppData(tablename);
}

function toggleEnv4AppDelete(e)
{
  if (e.checked)
   $('#env4app_header_buttons > button.delete_button').css("color","#3367d6");
  else
   $('#env4app_header_buttons > button.delete_button').css("color","grey");
}
