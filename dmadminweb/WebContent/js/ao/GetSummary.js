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

function GetDateFormats(currval)
{
	var formats = [
	{fmt:"dd/MM/yyyy", disp:"dd/mm/yyyy"},
	{fmt:"MM/dd/yyyy", disp:"mm/dd/yyyy"}
	];
	var rv="";
	for (i=0;i<formats.length;i++) {
		var s=(currval==formats[i].fmt)?"selected":"";
		rv += "<option value=\""+formats[i].fmt+"\" "+s+">"+formats[i].disp+"</option>";
	}
	return rv;
}

function GetTimeFormats(currval)
{
	var formats = [
   	{fmt:"h:mm aa", disp: "12 Hour"},
   	{fmt:"HH:mm", disp: "24 Hour"}
   	];
   	var rv="";
   	for (i=0;i<formats.length;i++) {
   		var s=(currval==formats[i].fmt)?"selected":"";
   		rv += "<option value=\""+formats[i].fmt+"\" "+s+ ">"+formats[i].disp+"</option>";
   	}
   	console.log("rv="+rv);
   	return rv;
}

function clearItemOptions()
{
//	var pwd = parent.$("#summ_data_edit");
//	var custid = $("#customaction_val").val();
//	if (custid!="") {	$("#repository_sumrow")
//		pwd.find("#repository_row").hide();
//		pwd.find("#rb_repository_row").hide();
//		pwd.find("#rf_repository_row").hide();
//		pwd.find("#summtab").find("tr.repoprops").remove();
//		pwd.find("#summtab").find("tr.rf_repoprops").remove();
//		pwd.find("#summtab").find("tr.rb_repoprops").remove();
//	} else {
//		CompTypeChanged();
//	}
}

function LoadServerStatusData(tablename,objtypeAsInt,objtype)
{
$.ajax({
	url : "GetSummaryData?s=1&objtype=" + objtypeAsInt + "&id=" + objid,
	dataType : 'json',
	type : 'GET',
	success : function(res) {
		console.log(res);
		var td = "";
		for (var r = 0; r < res.data.length; r++) {
			var row = res.data[r];
			var label = row[3];
			var val = row[4];
			td=td+"<tr><td>"+label+"</td><td>"+val+"</td></tr>";
		}
		$("#" + tablename + " > tbody").html(td);
	}
});
}

function LoadAppLogList(tablename,objtypeAsInt,objtype, objid)
{
 deployedenvs = {};

$.ajax({
 url : "NewsFeed?otid=" + objtype + objid + "&reason=applogs",
 dataType : 'json',
 async: false,
 type : 'GET',
 success : function(res) {
  console.log(res);
  var td = "";
  for (var r = 0; r < res.data.length; r++) {
   var row = res.data[r];

   var label = "<a id=\"deployment" + row.id + "\" onclick=\"DisplayDeploy(" + row.id + ");\">#" + row.id + "</a>";
   var sf = row.rc;
   var val = row.finished;
   var env = row.env;

   if (env in deployedenvs)
   {
    var deployid = deployedenvs[env];
    deployid = Math.max(deployid, row.id);
    deployedenvs[env] = deployid;
   }
   else
    deployedenvs[env] = row.id;

   td=td+"<tr><td>"+label+"</td><td>"+sf+"</td><td>"+val+"</td><td>" + env + "</tr>";
  }
  $("#" + tablename + " > tbody").html(td);

  if (res.data.length >= 1)
  {
   CreateDeployReports4();
   ReplotDeployReports4(res.data[0].id);
  }
 }
});
}

function LoadBuildData(tablename, objtypeAsInt, objtype, objid, addParams)
{
	console.log("LoadBuildData, tablename="+tablename+" objtypeAsInt="+objtypeAsInt+" objtype="+objtype+" objid="+objid);
	$.ajax({
		 url 		: "GetBuildHistory?objtype=" + objtype + "&id=" + objid,
		 dataType	: 'json',
		 type 		: 'GET',
		 success	: function(res) {
		     console.log(res);
		     var td="";
		     if (typeof res.error != "undefined") {
		    	 alert(res.error);
		     } else {
		    	 for (i=0;i<res.builds.length;i++) {
		    		 var icon;
		    		 var success = false;
		    		 var successmsg = "Failed";
		    		 if (res.builds[i].result == "SUCCESS")
		    		 {
		    		  successmsg = "Success";
		    			 success=true;
		    		 }
		    		 var complink="";
		    		 var comparr = res.builds[i].components;
		    		 var spc="";
		    		 if (comparr.length>=1) {
		    			 for (var x=0;x<comparr.length;x++) {
		    				 complink = complink+spc+"<a onClick=\"SwitchDisplay('"+comparr[x].type+comparr[x].id+"');\">"+comparr[x].name+"</a>";
		    				 spc="&nbsp;&nbsp;&nbsp;&nbsp;";
		    			 }
		    		 }
		    		 td+="<tr><td><a onClick=\"javascript:DisplayBuild(-"+objid+","+res.builds[i].id+","+success+");\">#"+res.builds[i].id+"</a></td><td>" + successmsg + "<td>"+res.builds[i].timestamp+"</td><td>"+complink+"</td></tr>";
		    	 }
		    	 $("#" + tablename + " > tbody").html(td);
		     }
		 }
	});
}

function ToggleLifeCycle()
{
	var pwd = $("#summ_data_edit");
	var cb = pwd.find("#lifecycledomain_val");
	isLifeCycle = cb.is(":checked");
	var sdl = pwd.find("#subdomainlist");
	if (isLifeCycle) {
		sdl.find('li').addClass("islifecycle");
		sdl.sortable({
			axis : "y",
			cursor : "move",
			stop : function(event, ui) {
			}
		});
		sdl.sortable("enable");
	} else {
		sdl.find('li').removeClass("islifecycle");
		sdl.sortable("disable");
	}
}

function AppMap()
{
 $("#tabs-General-map").html("");

 $.ajax({
  url: "GetApps2Comps?appids=" + objid,
  type: 'POST',
  dataType: "json",
  success: function(x) {

  // create a network
  var nodes = new vis.DataSet(x.nodes);
  var edges = new vis.DataSet(x.edges);

   var data = {
     nodes: nodes,
     edges: edges
   };

   var options = {autoResize: false,
     height: '100%',
     width: '100%',
                  physics:{
                   enabled: true,
                   minVelocity: 5,
                   stabilization: {
                    enabled: true,
                    fit: true
                   },
                   barnesHut: {
                     gravitationalConstant: -2000,
                     centralGravity: .1,
                     springLength: 250,
                     springConstant: 0.01,
                     damping: 0.09,
                     avoidOverlap: .5
                   }},
                   edges:{arrows: 'to',smooth: true},
                   nodes:{shape:'box'},
                   interaction:{navigationButtons: true}
  };

   var container = document.getElementById('tabs-General-map');
//   waitForElement("#tabs-General-map",function(){
   network = new vis.Network(container, data, options);
   network.once('afterDrawing', function() {
 //  var scaleOption = { scale : 0.45 };
 //   network.moveTo(scaleOption);
   setTimeout(clickFit(), 500);
    });
//   });
  }
 });
}

function CompMap()
{
 $("#tabs-General-map").html("");

 $.ajax({
  url: "GetComps2Apps?compids=" + objid,
  type: 'POST',
  dataType: "json",
  success: function(x) {

  // create a network
  var nodes = new vis.DataSet(x.nodes);
  var edges = new vis.DataSet(x.edges);

   var data = {
     nodes: nodes,
     edges: edges
   };

   var options = {autoResize: false,
     height: '100%',
     width: '100%',

                  physics:{
                   enabled: true,
                   minVelocity: 5,
                   stabilization: {
                    enabled: true,
                    fit: false
                   },
                   barnesHut: {
                     gravitationalConstant: -2000,
                     centralGravity: .1,
                     springLength: 250,
                     springConstant: 0.01,
                     damping: 0.09,
                     avoidOverlap: .5
                   }},
                   edges:{arrows: 'to',smooth: true},
                   nodes:{shape:'box'},
                   interaction:{navigationButtons: true}
  };

   var container = document.getElementById('tabs-General-map');
   waitForElement("#tabs-General-map",function(){
   network = new vis.Network(container, data, options);
//   network.once('afterDrawing', function() {
   // var scaleOption = { scale : 0.45 };
   // network.moveTo(scaleOption);
//    setTimeout(clickFit, 500);
//   })
   });
  }
 });
}

function clickFit()
{
 network.fit();
}

function FetchSummary(tablename, objtypeAsInt, objtype, objid, addParams)
{
 var isDocker = false;

 if (typeof addParams == "undefined")
  addParams = "";

 console.log("GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid + addParams);

 $.ajax(
   {
    url : "GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid + addParams,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     console.log(res);
     readonly = res.readOnly;
     domain = res.domain;
     if (typeof domain != "undefined") {
      summSavedomid = domain.id;
     }

     if (readonly)
      $("#title_icon").hide();
     else
      $("#title_icon").show();

     saveRes = res;
     var td = "";
     tdedit = "<form id=\"summform\"><table id=\"summtab\" class=\"dev_table\">";
     var comptypedone = false;
     var comptypelabeldone = false;
     summAutoPing = false;
     summSaveVDS = "";
     summSaveBTDS = "";
     var allocate_terminal_shown=false;

     var isGlobalDomain = false;

     if (summSaveobjtype == "do")
     {
      for (var r = 0; r < res.data.length; r++)
      {
	   var row = res.data[r];
	   var label = row[3];
       var val = row[4];

       if (label == "Name" && val == "GLOBAL")
       {
         isGlobalDomain = true;
         break;
       }
      }
     }


     for (var r = 0; r < res.data.length; r++)
     {
      var row = res.data[r];
      var field = row[0];
      var callback = row[1];
      var rtype = row[2];
      var label = row[3];
      var val = row[4];
      var isuser = true;
      var oldval = "";
      var linkval = null;

      if (label == "Notifier")
      {
	   td += "<tr id=\"contact_details_row\">";
       td += "<td id=\"contact_details_title\">";
       td += "<h2>Details</h2>";
       td += "</td>";
       td += "<td><div id=\"summ_header_buttons\">";
       td += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       td += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       td += "</button>";
       td += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       td += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       td += "</button>";
       td += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       td += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       td += "</button>";
       td += "</div></td></tr>";

       tdedit += "<tr id=\"contact_details_row\">";
       tdedit += "<td id=\"contact_details_title\">";
       tdedit += "<h2>Details</h2>";
       tdedit += "</td>";
       tdedit += "<td><div id=\"summ_header_buttons\">";
       tdedit += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       tdedit += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       tdedit += "</button>";
       tdedit += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       tdedit += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       tdedit += "</button>";
       tdedit += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       tdedit += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       tdedit += "</button>";
       tdedit += "</div></td></tr>";
 	  }

      // Add in domain to summary page
      if (label == "Full Domain")
      {
	   if (objtype == "co" || objtype == "cv")
	   {
	   td += "<tr id=\"contact_details_row\">";
       td += "<td id=\"contact_details_title\">";
       td += "<h2>Component Details</h2>";
       td += "</td>";
       td += "<td><div id=\"summ_header_buttons\">";
       td += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       td += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       td += "</button>";
       td += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       td += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       td += "</button>";
       td += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       td += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       td += "</button>";
       td += "<button class=\"test_button\" onClick=\"javascript:TestServer('servstat',objtypeAsInt,objid)\">";
       td += "<i class=\"fad fa-alien\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Test Connection";
       td += "</button>";
       td += "<button class=\"add_button\" onClick=\"javascript:AddDomain(objtypeAsInt,objid)\">";
       td += "<i class=\"fal fa-plus\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Add Subdomain";
       td += "</button>";
       td += "<button class=\"delete_button\" onClick=\"javascript:DeleteDomain(objtypeAsInt,objid)\">";
       td += "<i class=\"fal fa-trash\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Delete Domain";
       td += "</button>";
       td += "</div></td></tr>";

       tdedit += "<tr id=\"contact_details_row\">";
       tdedit += "<td id=\"contact_details_title\">";
       tdedit += "<h2>Component Details</h2>";
       tdedit += "</td>";
       tdedit += "<td><div id=\"summ_header_buttons\">";
       tdedit += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       tdedit += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       tdedit += "</button>";
       tdedit += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       tdedit += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       tdedit += "</button>";
       tdedit += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       tdedit += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       tdedit += "</button>";
       tdedit += "<button class=\"test_button\" onClick=\"javascript:TestServer('servstat',objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fad fa-alien\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Test Connection";
       tdedit += "</button>";
       tdedit += "<button class=\"add_button\" onClick=\"javascript:AddDomain(objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fal fa-plus\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Add Subdomain";
       tdedit += "</button>";
       tdedit += "<button class=\"delete_button\" onClick=\"javascript:DeleteDomain(objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fal fa-trash\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Delete Domain";
       tdedit += "</button>";
       tdedit += "</div></td></tr>";
       }
       else
       {
	   td += "<tr id=\"contact_details_row\">";
       td += "<td id=\"contact_details_title\">";
       td += "Details";
       td += "</td>";
       td += "<td><div id=\"summ_header_buttons\">";
       td += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       td += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       td += "</button>";
       td += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       td += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       td += "</button>";
       td += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       td += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       td += "</button>";
       if (objtype == "sv")
       {
        td += "<button class=\"test_button\" onClick=\"javascript:TestServer('servstat',objtypeAsInt,objid)\">";
        td += "<i class=\"fad fa-alien\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Test Connection";
        td += "</button>";
       } else if (objtype == "do")
       {
        td += "<button class=\"add_button\" onClick=\"javascript:AddDomain(objtypeAsInt,objid)\">";
        td += "<i class=\"fal fa-plus\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Add Subdomain";
        td += "</button>";
        td += "<button class=\"delete_button\" onClick=\"javascript:DeleteDomain(objtypeAsInt,objid)\">";
        td += "<i class=\"fal fa-trash\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Delete Domain";
        td += "</button>";
       }
       td += "</div></td></tr>";

       tdedit += "<tr id=\"contact_details_row\">";
       tdedit += "<td id=\"contact_details_title\">";
       tdedit += "Details";
       tdedit += "</td>";
       tdedit += "<td><div id=\"summ_header_buttons\">";
       tdedit += "<button class=\"edit_button\" onClick=\"javascript:EditSummaryButton(false,'')\">";
       tdedit += "<i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit";
       tdedit += "</button>";
       tdedit += "<button class=\"save_button\" onClick=\"javascript:summOK(false,'')\">";
       tdedit += "<i class=\"fal fa-save\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Save";
       tdedit += "</button>";
       tdedit += "<button class=\"cancel_button\" onClick=\"javascript:summCancel('')\">";
       tdedit += "<i class=\"fal fa-times-circle\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Cancel";
       tdedit += "</button>";
       if (objtype == "sv")
       {
       tdedit += "<button class=\"test_button\" onClick=\"javascript:TestServer('servstat',objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fad fa-alien\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Test Connection";
       tdedit += "</button>";
       }
       else if (objtype == "do")
       {
       tdedit += "<button class=\"add_button\" onClick=\"javascript:AddDomain(objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fal fa-plus\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Add Subdomain";
       tdedit += "</button>";
       tdedit += "<button class=\"delete_button\" onClick=\"javascript:DeleteDomain(objtypeAsInt,objid)\">";
       tdedit += "<i class=\"fal fa-trash\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Delete Domain";
       tdedit += "</button>";
       }
       tdedit += "</div></td></tr>";
	   }

       // label = "Domain";
       if (summSaveobjtype == "do")
         label = "Parent Domain";

       if (isGlobalDomain)
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Full Domain:</td>";
        tdedit += "<td >" + val;
        save_fulldomain_val = val;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Full Domain:</td>";
        tdedit += "<td ><select name=\"fulldomain_val\">";
        save_fulldomain_val = val;
        tdedit += "</select>";
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"fulldomain_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
	   }

       // td += "<tr id='fulldomain_sumrow_desc'><td id=\"fulldomain_summ_desc\" class=\"summlabel\">";
       // td += "Domain:";
       // td += "</td><td>";
       // td += val;
       // td += "</td></tr>";
      }
      else if (label == "Notifier")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Notifier:</td>";
       tdedit += "<td ><select name=\"notifier_val\">";
       save_notifier_val = val;
       tdedit += "</select>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"notifier_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"notifier_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"notifier_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "CD Engine")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">CD Engine:</td>";
       tdedit += "<td ><select name=\"builder_val\" onChange=\"BuilderChanged()\">";
       save_builder_val = val;
       tdedit += "</select>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"builder_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"builder_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"builder_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Summary")
      {
       label = "Description";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"summary_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Name")
      {
       if (summSaveobjtype == "do")
         label = "Domain Name";
       objName = val;

       if (isGlobalDomain)
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td>" + val + "<input type=\"hidden\" name=\"name_val\" autocomplete=\"off\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"name_val\" autocomplete=\"off\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
	   }
      }
      else if (label == "Availability")
      {
       tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select name=\"availability_val\" id=\"availability_val\">";
          summAvailability = val;
          if (val == "E") {
           val = "Always Available Unless Denied by Calendar";
           tdedit += "<option value=\"E\" selected>Always Available Unless Denied by Calendar</option>";
           tdedit += "<option value=\"O\">Always Unavailable Unless Allowed by Calendar</option>";
          } else {
           val = "Always Unavailable Unless Allowed by Calendar";
           tdedit += "<option value=\"E\">Always Available Unless Denied by Calendar</option>";
           tdedit += "<option value=\"O\" selected>Always Unavailable Unless Allowed by Calendar</option>";
          }
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"availability_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"availability_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"availability_oldval\" value=\"" + val + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Filename")
      {
       tdedit += "<tr id=\"filename_row\" style=\"display:none\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"filename_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filename_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filename_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filename_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Username" || label == "User Name")
      {
       console.log("username seen, field="+field+" val="+val);
       if (field == 802)
       {
        // CRED_ENCUSERNAME - encrypted username
        tdedit += "<tr id=\"usernameenc_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"usernameenc_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       if (field == 807)
       {
        // CRED_USERNAME - decrypted username
        tdedit += "<tr id=\"usernameenc_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"usernameenc_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
        // Default - CRED_VARUSERNAME
        tdedit += "<tr id=\"username_row\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Username:</td>";
        tdedit += "<td><input name=\"username_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"username_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"username_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"username_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
      }
      else if (label == "Group Name")
      {
       tdedit += "<tr>";
       var ro = (objid==1 || objid==2)?"readonly":"";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"groupname_val\" style='width:100%' "+ro+" type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Command Line Interpreter")
      {
       tdedit += "<tr id=\"interpreter_row\" style=\"display:none\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"interpreter_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Real Name")
      {
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"realname_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Email")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"email_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Phone")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"phone_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"phone_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"phone_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"phone_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Password")
      {
       if (field == 803)
       {
        tdedit += "<tr id=\"passwordenc_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + "</td>";
        tdedit += "<td><input name=\"passwordenc_val\" style='width:100%' type=\"password\"  autocomplete=\"off\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_oldval\" /></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<tr id=\"password_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Password:</td>";
        tdedit += "<td><input name=\"password_val\" style='width:100%' type=\"password\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_oldval\" /></td>";
        tdedit += "</tr>";
       }
      }
      else if (label == "Hostname")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"hostname_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "SSH Port Number")
      {
       tdedit += "<tr id=\"sshport_row\" style=\"display:none\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"sshport_val\" style='width:8em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Protocol")
      {
	   if (hideOption())
		  continue;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select onChange=\"ProtocolChanged()\"; name=\"protocol_val\" id=\"protocol_val\">";
       summProtocol = val;

       if (val == "sftp")
       {
        val = "sftp";
        tdedit += "<option value=\"sftp\" selected>ssh</option>";
        tdedit += "<option value=\"win\">WinRM</option>";
       }
       else if (val == "win")
       {
        val = "win";
        tdedit += "<option value=\"sftp\">ssh</option>";
        tdedit += "<option value=\"win\" selected>WinRM</option>";
       }
       else
       {
        val = "sftp";
        tdedit += "<option value=\"sftp\" selected>ssh</option>";
        tdedit += "<option value=\"win\">WinRM</option>";
       }

       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Engine")
      {
       label = "Engine Hostname";
       var engine = val;
       if (typeof engine.name == "undefined")
       {
        engine = new Object();
        engine.name = "";
        engine.type = "ei";
        engine.id = 0;
       }
       val = engine.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + " Hostname:</td>";
       tdedit += "<td><input name=\"engine_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Approval Domain" || label == "Move to Domain" || label == "Create Version in Domain")
      {
       var tasktype = "utap";

       if (label == "Approval Domain")
        tasktype = "utap";
       else if (label == "Move to Domain")
        tasktype = "utmp";
       else if (label == "Create Version in Domain")
        tasktype = "utcv";
       else
        tasktype = "";

       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"domain_val\">";
       save_domain_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_field\" value=\"" + tasktype + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Endpoint Types")
      {
 	   if (hideOption())
 		  continue;

       if (comptypedone == false)
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left;vertical-align:top;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td ><select multiple name=\"servercomptype_val\">";
        var parts = val.split(";");

        save_servercomptype_val = parts[0];
        val = parts[1];
        comptypedone = true;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"servercomptype_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"servercomptype_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"servercomptype_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
        var parts = val.split(";");
        save_servercomptype_val += ";" + parts[0];
        val = parts[1];
       }
      }
      else if (label == "Action to Run")
      {
       var action = val;
       if (typeof action.name == "undefined") {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
          }
          val = action.name;
          linkval="<a href='javascript:SwitchDisplay(\"ac"+action.id+"\");'>"+action.name+"</a>";
       var tasktype = "utat";
       tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select name=\"taskaction_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_taskaction_val = action.id;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"taskaction_field\" value=\"" + tasktype + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"taskaction_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"taskaction_oldval\" value=\"" + val + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Linked Task")
      {
       var tasktype = "utrp";

       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"linkedtask_val\">";
       save_linkedtask_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"linkedtask_field\" value=\"" + tasktype + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"linkedtask_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"linkedtask_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Port")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"port_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Base Directory")
      {
	   if (hideOption())
		  continue;
 //      if (isDocker)
 //       continue;

       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"basedirectory_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Display Name")
      {
       tdedit += "<tr id=\"fragmentname_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"fragmentname_val\" type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
//      else if (label == "Repository")
//      {
//       console.log("Repository Label Seen");
//       var repo = val;
//       if (typeof repo.name == "undefined")
//       {
//        repo = new Object();
//        repo.name = "";
//        repo.type = "re";
//        repo.id = 0;
//       }
//       val = repo.name;
//       console.log(repo);
//       var otid="re"+repo.id;
//       linkval=repo.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+val+"</a>":val;
//
//       tdedit += "<tr id=\"repository_row\">";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       tdedit += "<td><select name=\"repository_val\"/></td>";
//       save_repository_val = repo.name;
//       save_repository_id = repo.id;
//       tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"" + repo.type + repo.id + "\"/></td>";
//       tdedit += "</tr>";
//       repolabeladded = true;
//      }
      else if (label == "Filepath")
      {
       tdedit += "<tr id=\"filepath_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"filepath_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Owner")
      {
// Skip displaying Owner - 10/28/21 SBT

	   continue;

//
//       var owner = val;
//       if (typeof owner.name != "undefined")
//       {
//        val = owner.name;
//        if (owner.type != "us")
//         isuser = false;
//       }
//       else
//       {
//        owner = new Object();
//        owner.type = "us";
//        owner.name = "";
//        owner.id = 0;
//        val = "";
//       }
//
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Owner Type:</td>";
//       tdedit += "<td ><select id=\"ownertype_val\" name=\"ownertype_val\">";
//       if (isuser)
//       {
//        tdedit += "<option value=\"User\" selected>User</option>";
//        tdedit += "<option value=\"Group\">Group</option>";
//       }
//       else
//       {
//        tdedit += "<option value=\"User\">User</option>";
//        tdedit += "<option value=\"Group\" selected>Group</option>";
//       }
//       tdedit += "</select></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"ownertype_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"ownertype_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"ownertype_oldval\" value=\"" + owner.type + "\"/></td>";
//       tdedit += "</tr>";
//
//       tdedit += "<tr id=\"owner_row\">";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Owner:</td>";
//       tdedit += "<td ><select name=\"owner_val\">";
//       save_owner_val = owner.name;
//       tdedit += "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"owner_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"owner_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"owner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
//       tdedit += "</tr>";
//
//       tdedit += "<tr id=\"group_row\">";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Groups:</td>";
//       tdedit += "<td ><select name=\"groupowner_val\">";
//       save_groupowner_val = owner.name;
//       tdedit += "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"groupowner_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"groupowner_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"groupowner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
//       tdedit += "</tr>";
//
//       td += "<tr id=\"ownertype\" ><td class=\"summlabel\">";
//       td += "Owner Type";
//       td += ":</td><td>";
//       if (owner.type == "us")
//         td += "User";
//       else
//         td += "Group";
      }
      else if (label == "Parent Domain")
      {
       var domain = val;
       val = domain.value;
       if (val == "")
        val = "This is the top level domain";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"parentdomain_val\" >" + val + "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"parentdomain_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"parentdomain_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"parentdomain_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Always Deploy")
      {
       var always = val;
       val = always.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val || objid == -1)
        tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"off\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Deploy Sequentially")
      {
       var seq = val;
       val = seq.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val || objid == -1)
        tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"deployseq_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"deployseq_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"deployseq_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"deployseq_oldval\" value=\"off\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Database Roll-forward/Rollback")
      {
       var database = val;
       val = database.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input id=\"database_val\" name=\"database_val\" type=\"checkbox\"/ checked></td>";
       else
        tdedit += "<td><input id=\"database_val\" name=\"database_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";

       val=val?"Yes":"No";
      }
      else if (label == "Delete Corresponding Directory")
      {
      }
      else if (label == "Auto Ping")
      {
//       var always = val;
//       val = always.value;
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       if (val || objid == -1)
//           tdedit += "<td><input id=\"autoping_val\" name=\"autoping_val\" type=\"checkbox\" checked /></td>";
//          else
//           tdedit += "<td><input id=\"autoping_val\" name=\"autoping_val\" type=\"checkbox\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"autoping_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"autoping_callback\" value=\"" + callback + "\"/></td>";
//       if (val)
//        tdedit += "<td><input type=\"hidden\" name=\"autoping_oldval\" value=\"on\"/></td>";
//       else
//        tdedit += "<td><input type=\"hidden\" name=\"autoping_oldval\" value=\"off\"/></td>";
//       tdedit += "</tr>";
//       summAutoPing=val;
//       val=val?"Yes":"No";
      }
      else if (label == "Auto MD5 Check")
      {
//       var always = val;
//       val = always.value;
//       tdedit += "<tr id=\"automd5_row\">";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       if (val || objid == -1)
//           tdedit += "<td><input id=\"automd5check_val\" name=\"automd5check_val\" type=\"checkbox\" checked /></td>";
//          else
//           tdedit += "<td><input id=\"automd5check_val\" name=\"automd5check_val\" type=\"checkbox\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"automd5check_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"automd5check_callback\" value=\"" + callback + "\"/></td>";
//       if (val)
//        tdedit += "<td><input type=\"hidden\" name=\"automd5check_oldval\" value=\"on\"/></td>";
//       else
//        tdedit += "<td><input type=\"hidden\" name=\"automd5check_oldval\" value=\"off\"/></td>";
//       tdedit += "</tr>";
//       val=val?"Yes":"No";
      }
      else if (label == "Ping Interval")
      {
//       tdedit += "<tr id=\"pinginterval_row\">";
//          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//          tdedit += "<td >";
//          tdedit += "<select name=\"pinginterval_val\">";
//          var pif=false;
//          for (i=15;i<=240;i=i+15) {
//           if (i == val) pif=true;
//           sel=(i == val)?"selected":"";
//           tdedit += "<option value=\""+i+"\" "+sel+">"+i+"</option>";
//          }
//          if (!pif) val="15"; // default value
//          tdedit += "</select>"
//          tdedit += " (minutes)</td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pinginterval_field\" value=\"" + field + "\"/></td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pinginterval_oldval\" value=\""+val+"\"/></td>";
      }
      else if (label == "Ping Start")
      {
//       tdedit += "<tr id=\"pingstart_row\">";
//          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//          tdedit += "<td >";
//          tdedit += "<select name=\"pingstart_val\">";
//          valhours = val.substr(0,2);
//          valmins  = val.substr(3,2);
//          valsecs = (valhours * 3600) + (valmins * 60);
//          for (h=0;h<=23;h++)
//          for (m=0;m<=45;m=m+15) {
//           secs = (h*3600) + (m*60);
//           sel=(secs == valsecs)?"selected":"";
//           tdedit += "<option value=\""+secs+"\" "+sel+">"+(h<10?"0"+h:h)+":"+(m<10?"0"+m:m)+"</option>";
//          }
//          tdedit += "</select>"
//          tdedit += "</td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pingstart_field\" value=\"" + field + "\"/></td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pingstart_oldval\" value=\""+val+"\"/></td>";
      }
      else if (label == "Ping End")
      {
//       tdedit += "<tr id=\"pingend_row\">";
//          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//          tdedit += "<td >";
//          tdedit += "<select name=\"pingend_val\">";
//          valhours = val.substr(0,2);
//          valmins  = val.substr(3,2);
//          valsecs = (valhours * 3600) + (valmins * 60);
//          for (h=0;h<=23;h++)
//          for (m=(h==0)?15:0;m<=45;m=m+15) {
//           secs = (h*3600) + (m*60);
//           sel=(secs == valsecs)?"selected":"";
//           tdedit += "<option value=\""+secs+"\" "+sel+">"+(h<10?"0"+h:h)+":"+(m<10?"0"+m:m)+"</option>";
//          }
//          tdedit += "</select>"
//          tdedit += "</td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pingend_field\" value=\"" + field + "\"/></td>";
//          tdedit += "<td><input type=\"hidden\" name=\"pingend_oldval\" value=\""+val+"\"/></td>";
      }
      else if (label == "LifeCycle Domain")
      {
	   continue;

      }
      else if (label == "Result Is Expr")
      {
       var resultisexpr = val;
       val = resultisexpr.value;
       tdedit += "<tr id=\"resultisexpr_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"resultisexpr_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"resultisexpr_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Allocate Terminal")
      {
       var allocateterminal = val;
       val = allocateterminal.value;
       console.log("Allocate Terminal="+val);
       tdedit += "<tr id=\"allocateterminal_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"allocateterminal_val\" style='width:1.5em' type=\"checkbox\" checked />*</td>";
       else
        tdedit += "<td><input name=\"allocateterminal_val\" style='width:1.5em' type=\"checkbox\"/>*</td>";
       tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
       allocate_terminal_shown=true;
       val=val?"Yes":"No";
      }
      else if (label == "Copy to Remote")
      {
       var copytoremote = val;
       val = copytoremote.value;
       tdedit += "<tr id=\"copytoremote_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"copytoremote_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"copytoremote_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"copytoremote_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"copytoremote_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Available in SubDomains")
      {
       var subdomains = val;
       val = subdomains.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"subdomains_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"subdomains_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"subdomains_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"subdomains_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"subdomains_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"subdomains_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Account Locked")
      {
       var showoutput = val;
       val = showoutput.value;
       tdedit += "<tr id=\"al_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"accountlocked_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"accountlocked_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"accountlocked_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"accountlocked_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"accountlocked_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"accountlocked_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Force Change Password")
      {
       var showoutput = val;
       val = showoutput.value;
       tdedit += "<tr id=\"fcp_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"forcechangepassword_val\" type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"forcechangepassword_val\" type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Filter Level")
      {
       if (objid > 0 && val == "Components")
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td ><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
        if (val == "Components")
        {
         tdedit += "<option value=\"Components\" selected>Components</option>";
         tdedit += "<option value=\"Items\">Items</option>";
        }
        else
        {
         tdedit += "<option value=\"Components\">Components</option>";
         tdedit += "<option value=\"Items\" selected>Items</option>";
        }
       }
       else
       {
        tdedit += "<tr style=\"display:none\">";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td ><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
        tdedit += "<option value=\"Components\">Components</option>";
        tdedit += "<option value=\"Items\" selected>Items</option>";
       }

       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";

       save_filter_val = val;
      }
      else if (label == "Endpoint Type" && summSaveobjtype != "se")
      {
	   if (hideOption())
		  continue;

	   if (objid === -1)
        val = "";
       tdedit += "<tr id=\"endpointtype_row_edit\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"comptype_val\" id=\"comptype_val\" onChange=\"CompTypeChanged()\">";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";

       save_comptype_val = val;
      }
//      else if (label == "Endpoint Type"  && summSaveobjtype == "se")
//      {
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       tdedit += "<td ><select multiple name=\"comptype_val\" id=\"comptype_val\" onChange=\"CompTypeChanged()\">";
//       tdedit += "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"comptype_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"comptype_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"comptype_oldval\" value=\"" + val + "\"/></td>";
//       tdedit += "</tr>";
//
//       save_comptype_val = val;
//      }
      else if (label == "Change Request Data Source") {
       console.log("Change Request Data Source seen");
       console.log(val);
       var datasource = val;
       var oldval;
       if (typeof datasource.name == "undefined") {
        datasource = new Object();
        datasource.name = "";
        datasource.type = "ds";
        datasource.id = 0;
        oldval="";
       } else {
        oldval=datasource.type+datasource.id;
       }
       val = datasource.name;
       summSaveBTDS = val;
          linkval=datasource.showlink?"<a href='javascript:SwitchDisplay(\"ds"+datasource.id+"\");'>"+datasource.name+"</a>":datasource.name;
          tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select name=\"bt_datasource_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_datasource_val = val;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_oldval\" value=\"" + oldval + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Build Job") {
       console.log("Build Job seen");
       console.log(val);
       var buildjob = val;
       var oldval;
       if (typeof buildjob.name == "undefined") {
        buildjob = new Object();
        buildjob.name = "";
        buildjob.type = "bj";
        buildjob.id = 0;
        oldval="";
       } else {
        oldval=buildjob.type+buildjob.id;
       }
       val = buildjob.name;
       summSaveBTDS = val;
          linkval=buildjob.showlink?"<a href='javascript:SwitchDisplay(\"bj"+buildjob.id+"\");'>"+buildjob.name+"</a>":val;
          tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select name=\"buildjob_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_buildjob_val = val;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_oldval\" value=\"" + oldval + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Last Build Number") {
//       if (val==0) val="";
//       else val="<a onClick=\"javascript:DisplayBuild("+objid+","+val+");\">#"+val+"</a>";
//
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       tdedit += "<td name=\"lastlogin_val\" >" + val + "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_oldval\" value=\"" + val + "\"/></td>";
//       tdedit += "</tr>";
      }
      else if (label == "Project Name" || label == "Plan Name") {
       var project = val;
       var oldval=val;

          tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select name=\"projectname_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_projectname_val = val;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"projectname_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"projectname_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"projectname_oldval\" value=\"" + oldval + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Roll Forward")
      {
       if (save_filter_val == "Items")
        tdedit += "<tr id=\"rollforward_row\" style=\"display:none\" >";
       else
        tdedit += "<tr id=\"rollforward_row\">";

       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"rollup_val\">";
       if (val == "OFF")
       {
        val = "Off";
        tdedit += "<option value=\"OFF\" selected>Off</option>";
        tdedit += "<option value=\"ON\">On</option>";
        tdedit += "<option value=\"ALL\">All</option>";
       }
       else if (val == "ON")
       {
        val = "On";
        tdedit += "<option value=\"OFF\">Off</option>";
        tdedit += "<option value=\"ON\" selected>On</option>";
        tdedit += "<option value=\"ALL\">All</option>";
       }
       else
       {
        val = "All";
        tdedit += "<option value=\"OFF\">Off</option>";
        tdedit += "<option value=\"ON\">On</option>";
        tdedit += "<option value=\"ALL\" selected>All</option>";
       }
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollup_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollup_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollup_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Rollback")
      {
       if (save_filter_val == "Items")
        tdedit += "<tr id=\"rollback_row\" style=\"display:none\" >";
       else
        tdedit += "<tr id=\"rollback_row\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"rollback_val\">";
       if (val == "OFF")
       {
        val = "Off";
        tdedit += "<option value=\"OFF\" selected>Off</option>";
        tdedit += "<option value=\"ON\">On</option>";
        tdedit += "<option value=\"ALL\">All</option>";
       }
       else if (val == "ON")
       {
        val = "On";
        tdedit += "<option value=\"OFF\">Off</option>";
        tdedit += "<option value=\"ON\" selected>On</option>";
        tdedit += "<option value=\"ALL\">All</option>";
       }
       else
       {
        val = "All";
        tdedit += "<option value=\"OFF\">Off</option>";
        tdedit += "<option value=\"ON\">On</option>";
        tdedit += "<option value=\"ALL\" selected>All</option>";
       }
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollback_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollback_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"rollback_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Endpoint Type")
      {
       var st = val;
       val = "st" + st.id;
       label = "Endpoint Operating System";

       tdedit += "<tr id=\"servertype_row_edit\">";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"servertype_val\">";
       if (val == "st3")
       {
        oldval = val;
        val = "Unix";
        tdedit += "<option value=\"st1\" selected>Unix</option>";
        tdedit += "<option value=\"st2\">Windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st4")
       {
        oldval = val;
        val = "Unix";
        tdedit += "<option value=\"st1\" selected>Unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st1")
       {
        oldval = val;
        val = "Unix";
        tdedit += "<option value=\"st1\" selected>Unix</option>";
        tdedit += "<option value=\"st2\">Windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st5")
       {
        oldval = val;
        val = "Tandem";
        tdedit += "<option value=\"st1\">Unix</option>";
        tdedit += "<option value=\"st2\">Windows</option>";
        tdedit += "<option value=\"st5\" selected>Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st6")
       {
        oldval = val;
        val = "Stratus";
        tdedit += "<option value=\"st1\">Unix</option>";
        tdedit += "<option value=\"st2\">Windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\" selected>Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st7")
       {
        oldval = val;
        val = "OpenVMS";
        tdedit += "<option value=\"st1\">Unix</option>";
        tdedit += "<option value=\"st2\">Windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\" selected>OpenVMS</option>";
       }
       else
       {
        oldval = val;
        val = "Windows";
        tdedit += "<option value=\"st1\">Unix</option>";
        tdedit += "<option value=\"st2\" selected>Windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }

       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label.toLowerCase() == "Pre-Action".toLowerCase() && summSaveobjtype != "rl")
      {
	   if (hideOption())
	     continue;

       if (isDocker)
        continue;

       var action = val;
       console.log("Pre-Action seen");
       console.log(val);
       var oldval;
       if (typeof action.name == "undefined")
       {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
        oldval="";
       } else {
        oldval=action.type+action.id;
       }
       val = action.name;
       var otid=(action.type=="ac")?action.type+action.id:action.type+action.id+"-"+action.kind;
       linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>":action.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"preaction_val\">";
       tdedit += "<option value=\"\"></option></select>";
       save_preaction_val = action.id;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label.toLowerCase() == "Post-Action".toLowerCase() && summSaveobjtype != "rl")
      {
	   if (hideOption())
	     continue;

       if (isDocker)
        continue;

       var action = val;
       var oldval;
       if (typeof action.name == "undefined")
       {
   action = new Object();
   action.name = "";
   action.type = "ac";
   action.id = 0;
   oldval="";
       } else {
        oldval=action.type+action.id;
       }
       val = action.name;
       var otid=(action.type=="ac")?action.type+action.id:action.type+action.id+"-"+action.kind;
       linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>":action.name;
       // linkval="<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"postaction_val\">";
       save_postaction_val = action.id;
       tdedit += "<option value=\"\"></option></select>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Custom Action" && summSaveobjtype != "rl")
      {
	   if (hideOption())
	     continue;
       var action = val;
       var oldval;
       if (typeof action.name == "undefined")
       {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
        oldval="";
       } else {
        oldval=action.type+action.id;
       }
       val = action.name;
       //linkval="<a href='javascript:SwitchDisplay(\""+action.type+action.id+"\");'>"+action.name+"</a>";
       linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+action.type+action.id+"\");'>"+action.name+"</a>":action.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (summSaveobjtype == "co") {
        tdedit += "<td ><select name=\"customaction_val\" id=\"customaction_val\" onChange=\"clearItemOptions();\">";
       } else {
        tdedit += "<td ><select name=\"customaction_val\" id=\"customaction_val\">";
       }
       save_customaction_val = action.id;
       tdedit += "<option value=\"\"></option></select>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Credential")
      {
	   if (hideOption())
		  continue;

       var cred = val;
       var oldval;
       if (typeof cred.name == "undefined")
       {
        cred = new Object();
        cred.name = "";
        cred.type = "cr";
        cred.id = 0;
        oldval="";
       } else {
        oldval = cred.type + cred.id;
       }
       linkval=cred.showlink?"<a href='javascript:SwitchDisplay(\"cr"+cred.id+"\");'>"+cred.name+"</a>":cred.name;
       val = cred.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"credential_val\">";
       save_credential_val = val;
       tdedit += "<option value=\"\">None Required</option></select>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Category")
      {
       var category = val;
       if (typeof category.name == "undefined")
       {
        category = new Object();
        category.name = "";
        category.type = "cy";
        category.id = 0;
       }
       val = category.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><input name=\"category_val\" selectBoxOptions=\"\" />";
       save_category_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_oldval\" value=\"" + category.type + category.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Validation Data Source") {
       console.log("VDS seen");
       console.log(val);
       var datasource = val;
       var oldval;
       if (typeof datasource.name == "undefined") {
        datasource = new Object();
        datasource.name = "";
        datasource.type = "ds";
        datasource.id = 0;
        oldval="";
       } else {
        oldval=datasource.type+datasource.id;
       }
       val = datasource.name;
       summSaveVDS = val;
          linkval="<a href='javascript:SwitchDisplay(\"ds"+datasource.id+"\");'>"+datasource.name+"</a>";
          tdedit += "<tr>";
          tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td ><select onChange=\"TogglePasswordFields();\" name=\"datasource_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_datasource_val = val;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"datasource_oldval\" value=\"" + oldval + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Successful Deployment Template" ||
         label == "Approved Notify Template" ||
         label == "Success Notification Template" ||
         label == "Request Notification Template" ||
         label == "Ping Failure Template")
      {
       var template = val;
       var oldval;
       if (typeof template.name == "undefined")
       {
        template = new Object();
        template.name = "";
        template.type = "te";
        template.id = 0;
        oldval="";
       } else {
        oldval=template.type+template.id;
       }
       val = template.name;
       linkval=template.showlink?"<a href='javascript:SwitchDisplay(\"te"+template.id+"\");'>"+template.name+"</a>":template.name;
       console.log("label=["+label+"]");
       if (label == "Ping Failure Template") {
        tdedit += "<tr id=\"pingtemplate_row\">";
       } else {
        tdedit += "<tr>";
       }
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"template_val\">";
       tdedit += "<option value=\"\"></option></select>";
       save_template_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Failed Deployment Template" ||
         label == "Rejected Notify Template" ||
         label == "Failure Notification Template")
      {
       var template = val;
       var oldval;
       if (typeof template.name == "undefined")
       {
        template = new Object();
        template.name = "";
        template.type = "te";
        template.id = 0;
        oldval="";
       } else {
        oldval=template.type+template.id;
       }
       val = template.name;
       linkval=template.showlink?"<a href='javascript:SwitchDisplay(\"te"+template.id+"\");'>"+template.name+"</a>":template.name;
//       if (label == "MD5 Mismatch Template"){
//        tdedit += "<tr id=\"md5template_row\">";
//       } else {
        tdedit += "<tr>";
//       }
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td ><select name=\"fail_template_val\">";
       tdedit += "<option value=\"\"></option></select>";
       save_fail_template_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Type")
      {
       var typeobj = val;
       var oldval;
       if (typeof typeobj.name == "undefined")
       {
        typeobj = new Object();
        typeobj.name = "";
        typeobj.type = "pd";
        typeobj.id = 0;
        oldval="";
       } else {
        oldval=typeobj.type+typeobj.id;
       }
       val = typeobj.name;
       console.log("Type: val="+val);
       tdedit += "<tr>";
       /*
       if (summSaveobjtype == "ds")
       {
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td ><select name=\"type_val\">";
        save_type_val = val;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"" + oldval + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
       */
        if (objid < 0)
        {
         var name = notifiertype;
         val = notifiertype;

         if (objtype == "ds")
         {
          val = dstype;
          if (dstype == "ldap")
           name = "LDAP";
          else if (dstype == "bugzilla")
           name = "Bugzilla"
          else if (dstype == "github")
           name = "GitHub"
          else if (dstype == "jira")
           name = "Jira"
          else if (dstype == "odbc")
           name = "ODBC"
         }
         else if (objtype == "re")
         {
          val = repotype;

          if (repotype == "filesystem")
            name = "File System";
          else if (repotype == "http")
            name = "Http";
          else if (repotype == "ftprep")
           name = "Ftp";
          else if (repotype == "meister")
           name = "Meister";
          else if (repotype == "perforce")
           name = "Perforce";
          else if (repotype == "svn")
           name = "SVN";
         }
         else if (objtype == "co")
         {
          val = addcomptype;

          if (addcomptype == "docker")
            name = "Docker";
          else if (addcomptypepotype == "file")
            name = "File";
          else if (addcomptype == "database")
           name = "Database";
         }
         else if (objtype == "no")
         {
          val = notifiertype;

          if (notifiertype == "smtpemail")
            name = "Email";
          else if (notifiertype == "txtlocal")
            name = "SMS";
          else if (notifiertype == "hipchat")
           name = "HipChat";
          else if (notifiertype == "slack")
           name = "Slack";
         }
         else if (objtype == "be")
         {
          val = buildenginetype;

          if (buildenginetype == "jenkins")
            name = "Jenkins";
         }
        }
        else
        {
         var name = "";

         if (objtype == "ds")
         {
          if (val == "ldap")
           name = "LDAP";
          else if (val == "bugzilla")
           name = "Bugzilla"
          else if (val == "github")
           name = "GitHub"
          else if (val == "jira")
           name = "Jira"
          else if (val == "odbc")
           name = "ODBC"
         }
         else if (objtype == "no")
         {
          if (val == "smtpemail")
            name = "Email";
          else if (val == "txtlocal")
            name = "SMS";
          else if (val == "hipchat")
           name = "HipChat";
          else if (val == "slack")
           name = "Slack";
         }
         else if (objtype == "re")
         {
          if (val == "filesystem")
            name = "File System";
          else if (val == "http")
            name = "Http";
          else if (val == "ftprep")
           name = "Ftp";
          else if (val == "meister")
           name = "Meister";
          else if (val == "perforce")
           name = "Perforce";
          else if (val == "svn")
           name = "SVN";
         }
         else if (objtype == "co")
         {
          if (val== "docker")
            name = "Docker";
          else if (val == "file")
            name = "File";
          else if (val == "database")
           name = "Database";
         }
         else if (objtype == "be")
         {
          if (val == "jenkins")
            name = "Jenkins";
         }
        }

        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td>" + name + "</td>";
        tdedit += "<td ><select name=\"type_val\" style=\"display:none\">";
        save_type_val = val;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"" + oldval + "\"/></td>";
        tdedit += "</tr>";
        /*
       }
       */
      }
      else if (label == "Kind")
      {
       var kind = val;
       var oldval;

	   label = "Component Type";

       if (typeof kind.name != "undefined" && kind.name.includes("Container"))
        isDocker = true;

       if (objid < 0 && kind.type == "ck")
       {
        kind.id = credtype.substring(2);

        if (kind.id == 10)
         kind.name = "Harvest DFO in Filesystem";
        else if (kind.id == 4)
         kind.name = "From Variables";
        else if (kind.id == 2)
         kind.name = "Encrypted in Database";
        else if (kind.id == 5)
         kind.name = "Private Key";
       }

       if (typeof kind.name == "undefined")
       {
        kind = new Object();
        kind.name = "";
        kind.type = "ak";
        kind.id = 0;
        oldval="";
       } else {
        oldval=kind.type+kind.id;
       }
       val = kind.name;
       if (val == "From Vars")
        val = "From Variables";

       if (kind.id != "6")
       {
 // No longer distinguishing between different types of components
 //
 //        tdedit += "<tr id=\"componenttype_row_edit\">";
 //        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
 //        tdedit += "<td>" + val + "</td>"
 //        tdedit += "<td ><input type=\"hidden\" name=\"kind_val\" value=\"" + oldval + "\"/>";
       save_kind_val = val;
 //        tdedit += "</td>";
 //        tdedit += "<td><input type=\"hidden\" name=\"kind_field\" value=\"" + field + "\"/></td>";
 //        tdedit += "<td><input type=\"hidden\" name=\"kind_callback\" value=\"" + callback + "\"/></td>";
 //        tdedit += "<td><input type=\"hidden\" id=\"kind_oldval\" name=\"kind_oldval\" value=\"\"/></td>";
 //        tdedit += "</tr>";
       }

       console.log("save_kind_id="+save_kind_id+" objid="+objid);

       save_kind_id = kind.id;

       console.log("kind.id="+kind.id);
       objkind = kind.id;
       continue;  // skip dsiplaying it
      }
      else if (label == "Created")
      {
// Skip Display - 10/28/21 SBT

	   continue;

//       var created = val;
//       if (typeof created.when != "undefined")
//       {
//        //var d = convertDate(created.when);
//        //val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
//        val = created.when + " by " + created.tooltip;
//       }
//       else
//        val = "";
//
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       tdedit += "<td>" + val + "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_oldval\" value=\"" + val + "\"/></td>";
//       tdedit += "</tr>";
      }
      else if (label == "Modified")
      {
// Skip Display - 10/28/21 SBT

	   continue;

//       var modified = val;
//       console.log("modified (Modified)");
//       console.log(modified);
//       if (typeof modified.when != "undefined")
//       {
//        //var d = convertDate(modified.when);
//        //val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
//        val = modified.when + " by " + modified.tooltip;
//       }
//       else
//        val = "";
//
//       tdedit += "<tr>";
//       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       tdedit += "<td>" + val + "</td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_field\" value=\"" + field + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_callback\" value=\"" + callback + "\"/></td>";
//       tdedit += "<td><input type=\"hidden\" name=\"created_oldval\" value=\"" + val + "\"/></td>";
//       tdedit += "</tr>";
      }
      else if (label == "Last Login")
      {
       var modified = val;
       console.log("Last Login");
       console.log(val);
       if (modified.value == 0) {
        val="";
       } else if (typeof modified.name != "undefined") {
        val = modified.name;
       } else {
        val = "";
       }
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"lastlogin_val\" >" + val + "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }

      if (label == "Protocol")
      {
       if (val == "win")
        val = "WinRM";
       else
        val = "ssh";

       td += "<tr><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += val;
      }
      else if (label == "Owner")
      {
       td += "<tr><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       if (isuser)
        td += val;
       else
        td += val;
      }
      else if ( label == "LifeCycle Domain" ||
         label == "Copy to Remote" ||
         label == "Always Deploy" ||
         label == "Deploy Sequentially" ||
         label == "Result Is Expr" ||
         label == "Account Locked" ||
         label == "Force Change Password" ||
         label == "Available in SubDomains")
      {
       if (label == "LifeCycle Domain") label = "All Subdomains are Lifecycles";
       var myid = label.toLocaleLowerCase().replace(/ /g, '') + "_sumrow";

       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       if (val == true)
        td += "Yes";
       else
        td += "No";
      }
      else if (label == "Password")
      {
       // re-added to display password
       if (((save_kind_id == 0 || save_kind_id == 4) && field == 805) || ((save_kind_id == 2) && field == 803))
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (field == 805)
         td += val;
        else
         td += "*********";
       }
       else if (save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 5 && save_kind_id != 10)
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (val.length == 0)
        td += "";
       else
        td += "*********";
       }

      }
      else if (label == "Username")
      {
       console.log("Username seen, field="+field+" save_kind_id="+save_kind_id);
       if (((save_kind_id == 0 || save_kind_id == 4) && field == 804) || ((save_kind_id == 2) && (field == 802 || field == 807)))
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (field == 802)
         td += "*********";
        else
         td += val;
       }
       else if ((save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 10 && save_kind_id != 5) || (save_kind_id == 5 && field==804))
       {
        console.log("added row for username");
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        td += val;
       }
      }
      else if (label == "Kind")
      {
// Skip displaying Object - 10/28/21 SBT

//	   continue;

//        td += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\">";
//        td += "Object:";
//        td += "</td><td>";
//        td += objtypeName;
//        td += "</td></tr>";
//
       td += "<tr id='kind_sumrow'><td id=\"kind_summ\" class=\"summlabel\">";
       td += "Kind";
       td += ":</td><td>";
       td += val;
      }
      else if (label.toLowerCase() == "Filter Level".toLowerCase() && save_filter_val == "Items")
      {

      }
      else if (label.toLowerCase() == "Pre-Action".toLowerCase() && summSaveobjtype == "rl")
      {

      }
      else if (label.toLowerCase() == "Post-Action".toLowerCase() && summSaveobjtype == "rl")
      {

      }
      else if (label.toLowerCase() == "Custom Action".toLowerCase() && summSaveobjtype == "rl")
      {

      }
      else if ((label == "Roll Forward" || label == "Rollback") && save_filter_val == "Items")
      {

      }
      else if (label.toLowerCase() == "Show Output".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "XPos".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "YPos".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "recursive".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Auto Ping".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Auto MD5 Check".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Ping Start".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Ping End".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Ping Interval".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "MD5 Mismatch Template".toLowerCase())
      {

      }
      else if (label.toLowerCase() == "Last Build Number".toLowerCase())
      {

      }
      else if (label == "Endpoint Types" )
      {
 	   if (hideOption())
 		  continue;

        var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";

        if (comptypelabeldone == false)
        {
         td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
         td += label;
         td += ":</td><td>";
         td += (linkval!=null)?linkval:val;
        }
        else
        {
         td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
         td += "";
         td += "</td><td>";
         td += (linkval!=null)?linkval:val;
        }
        comptypelabeldone = true;
      }
      else if (label == "Type")
      {
       if (objtype == "ds")
       {
        if (val == "ldap")
         name = "LDAP";
        else if (val == "bugzilla")
         name = "Bugzilla"
        else if (val == "github")
         name = "GitHub"
        else if (val == "jira")
         name = "Jira"
        else if (val == "odbc")
         name = "ODBC"
       }
       else if (objtype == "no")
       {
        if (val == "smtpemail")
          name = "Email";
        else if (val == "txtlocal")
          name = "SMS";
        else if (val == "hipchat")
         name = "HipChat";
        else if (val == "slack")
         name = "Slack";
       }
       else if (objtype == "re")
       {
        if (val == "filesystem")
          name = "File System";
        else if (val == "http")
          name = "Http";
        else if (val == "ftprep")
         name = "Ftp";
        else if (val == "meister")
         name = "Meister";
        else if (val == "perforce")
         name = "Perforce";
        else if (val == "svn")
         name = "SVN";
       }
       else if (objtype == "co")
       {
        if (val== "docker")
          name = "Docker";
        else if (val == "file")
          name = "File";
        else if (val == "database")
         name = "Database";
       }
       else if (objtype == "be")
       {
        if (val == "jenkins")
          name = "Jenkins";
       }
       var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
       console.log("3) <tr id=\"" + myid + "\" ><td class=\"summlabel\">");
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += name;
      }
      else
      {
       // Everything else
       var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
       console.log("3) <tr id=\"" + myid + "\" ><td class=\"summlabel\">");
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += (linkval!=null)?linkval:val;
      }
      td += "</td></tr>";
     }

     if (allocate_terminal_shown) tdedit += "<tr id=\"atfootnote\"><td colspan=\"2\">* Only for ssh targets</td></tr>";

//     if (repolabeladded == false && summSaveobjtype == "co" && objid < 0)
//     {
//      label = "Repository";
//      tdedit += "<tr id=\"repository_row\">";
//      tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//      tdedit += "<td><select name=\"repository_val\" id=\"repository_val\" onChange=\"ToggleRepoProps(-1,'repository_row',false,true)\" /></td>";
//      save_repository_val = "";
//      current_repository_val = save_repository_val;
//      tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"704\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"\"/></td>";
//      tdedit += "</tr>";
//
//      label = "Roll Foward Repository";
//      tdedit += "<tr id=\"rf_repository_row\" style=\"display:none\">";
//      tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//      tdedit += "<td><select name=\"rf_repository_val\" id=\"rf_repository_val\" onChange=\"ToggleRepoProps(-1,'rf_repository_row',false,true)\" /></td>";
//      save_repository_val = "";
//      current_repository_val = save_repository_val;
//      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_field\" value=\"704\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_callback\" value=\"" + callback + "\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_oldval\" value=\"\"/></td>";
//      tdedit += "</tr>";
//
//      label = "Rollback Repository";
//      tdedit += "<tr id=\"rb_repository_row\"  style=\"display:none\">";
//      tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//      tdedit += "<td><select name=\"rb_repository_val\" id=\"rb_repository_val\" onChange=\"ToggleRepoProps(-1,'rb_repository_row',false,true)\" /></td>";
//      save_repository_val = "";
//      current_repository_val = save_repository_val;
//      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_field\" value=\"704\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_callback\" value=\"" + callback + "\"/></td>";
//      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_oldval\" value=\"\"/></td>";
//      tdedit += "</tr>";
//     }

     if (objtype == "do") {
      var sdl="";
      $.ajax({
       url : "SubDomains?domainid=" + objid,
       dataType : 'json',
       type : 'GET',
       success : function(res) {
        if (res.length != 0) {
         sdl += "<tr>";
         sdl += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">Subdomains:</td>";
         if (isLifeCycle) {
          sdl += "<td style=\"background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
         } else {
          sdl += "<td style=\"background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
         }
        }
        var tmp = " class=\"islifecycle\"";
        if (!isLifeCycle) tmp = "";
        for (var k = 0; k < res.length; k++) {
         sdl += "<li id=\"dom_" + res[k].id + "\"" + tmp + "><span style=\"cursor: pointer;\" >" + res[k].name + "</span></li>";
         if (k == 0) {
          td += "<tr><td style=\"vertical-align: top;\">Subdomains:</td><td><div class=\"scrolltd\">" + res[k].name;
         } else {
          td += "<br>" + res[k].name;
         }
        }

        if (res.length != 0) {
         td += "</div></td></tr>"
         sdl += "</ul>";
         sdl += "</td>";
         sdl += "<td></td>";
         sdl += "<td></td>";
         sdl += "<td></td>";
         sdl += "</tr>";
         sdl += "<tr id=\"dndmsg\"><td colspan=\"5\" style=\"padding-top:8px;\"><i>Drag and drop the Subdomains to order them for your lifecycle model.</i></td></tr>";
         sdl += "</table></form>";
        }
        $("#" + tablename + " > tbody").html(td);
        tdedit += sdl;

        HideFields();

        if (objid < 0) {
         EditSummaryButton(false,"");
        }
       },
       error : function(jqxhr, status, err) {
    console.log(status);
    console.log(err);
       }
      });
     } else {
      tdedit += "</table></form>";
      $("#" + tablename + " > tbody").html(td);
      $("#summtab > tbody").html(tdedit);


      HideFields();

      if (objid < 0) {
       console.log("objid="+objid+" opening editing dialog (creation?)");
       EditSummaryButton(false,"");
      }
     }
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
}

function LoadDomNav()
{
 tablinks = document.getElementsByClassName("tablinks");
 for (i = 0; i < tablinks.length; i++) {
   tablinks[i].className = tablinks[i].className.replace(" active", "");
 }

 // Show the current tab, and add an "active" class to the link that opened the tab
 var vb = $("#verttab_domain");
 vb.addClass("active");

 $("#header_icon").html("<h1 class=\"domain\">Domains</h1>");

 if ($('#domlist').length > 0)
 	$('#domlist').empty();

 objid = userdomid;
 objtypeAsInt = 3;
 objtype = "do";
 currenttree = "#domains_tree";
 displaySubTabs = "Y";
 ActivateSubTabs();
}

function LoadTaskSummaryData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 task_summSavetablename = tablename;
 task_summSaveobjtypeAsInt = objtypeAsInt;
 task_summSaveobjtype = objtype;
 task_summSaveobjid = objid;
 task_summSaveaddParams = addParams;

 $("#task_summ_data_edit").hide();
 $("#task_summ_data").show();

  $.ajax(
    {
     url : "GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid + addParams,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      console.log(res);
      readonly = res.readOnly;
      domain = res.domain;
      if (typeof domain != "undefined") {
       summSavedomid = domain.id;
      }

      if (readonly)
       $("#title_icon").hide();
      else
       $("#title_icon").show();

      saveRes = res;
      var td = "";
      task_tdedit = "<form id=\"task_summform\"><table id=\"task_summtab\" class=\"dev_table\">";
      var comptypedone = false;
      var comptypelabeldone = false;
      summAutoPing = false;
      summSaveVDS = "";
      summSaveBTDS = "";
      var allocate_terminal_shown=false;


      for (var r = 0; r < res.data.length; r++)
      {
       var row = res.data[r];
       var field = row[0];
       var callback = row[1];
       var rtype = row[2];
       var label = row[3];
       var val = row[4];
       var isuser = true;
       var oldval = "";
       var linkval = null;

       // Add in domain to summary page
       if (label == "Full Domain")
       {
       }
       else if (label == "Notifier")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Notifier:</td>";
        task_tdedit += "<td ><select name=\"notifier_val\">";
        save_notifier_val = val;
        task_tdedit += "</select>";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"notifier_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"notifier_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"notifier_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "CD Engine")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">CD Engine:</td>";
        task_tdedit += "<td ><select name=\"builder_val\" onChange=\"BuilderChanged()\">";
        save_builder_val = val;
        task_tdedit += "</select>";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"builder_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"builder_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"builder_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Summary")
       {
        label = "Description";
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"summary_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"summary_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"summary_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"summary_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Name")
       {
        objName = val;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"name_val\" autocomplete=\"off\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"name_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"name_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Availability")
       {
        task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select name=\"availability_val\" id=\"availability_val\">";
           summAvailability = val;
           if (val == "E") {
            val = "Always Available Unless Denied by Calendar";
            task_tdedit += "<option value=\"E\" selected>Always Available Unless Denied by Calendar</option>";
            task_tdedit += "<option value=\"O\">Always Unavailable Unless Allowed by Calendar</option>";
           } else {
            val = "Always Unavailable Unless Allowed by Calendar";
            task_tdedit += "<option value=\"E\">Always Available Unless Denied by Calendar</option>";
            task_tdedit += "<option value=\"O\" selected>Always Unavailable Unless Allowed by Calendar</option>";
           }
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"availability_field\" value=\"" + field + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"availability_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"availability_oldval\" value=\"" + val + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Filename")
       {
        task_tdedit += "<tr id=\"filename_row\" style=\"display:none\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"filename_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filename_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filename_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filename_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Username" || label == "User Name")
       {
        console.log("username seen, field="+field+" val="+val);
        if (field == 802)
        {
         // CRED_ENCUSERNAME - encrypted username
         task_tdedit += "<tr id=\"usernameenc_row\" style=\"display:none\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td><input name=\"usernameenc_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_oldval\" value=\"" + val + "\"/></td>";
         task_tdedit += "</tr>";
        }
        else
        if (field == 807)
        {
         // CRED_USERNAME - decrypted username
         task_tdedit += "<tr id=\"usernameenc_row\" style=\"display:none\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td><input name=\"usernameenc_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"usernameenc_oldval\" value=\"" + val + "\"/></td>";
         task_tdedit += "</tr>";
        }
        else
        {
         // Default - CRED_VARUSERNAME
         task_tdedit += "<tr id=\"username_row\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Username:</td>";
         task_tdedit += "<td><input name=\"username_val\" style='width:100%' type=\"text\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"username_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"username_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"username_oldval\" value=\"" + val + "\"/></td>";
         task_tdedit += "</tr>";
        }
       }
       else if (label == "Group Name")
       {
        task_tdedit += "<tr>";
        var ro = (objid==1 || objid==2)?"readonly":"";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"groupname_val\" style='width:100%' "+ro+" type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupname_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupname_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupname_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Command Line Interpreter")
       {
        task_tdedit += "<tr id=\"interpreter_row\" style=\"display:none\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"interpreter_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"interpreter_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"interpreter_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"interpreter_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Real Name")
       {
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"realname_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"realname_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"realname_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"realname_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Email")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"email_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"email_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"email_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"email_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Phone")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"phone_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"phone_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"phone_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"phone_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Password")
       {
        if (field == 803)
        {
         task_tdedit += "<tr id=\"passwordenc_row\" style=\"display:none\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + "</td>";
         task_tdedit += "<td><input name=\"passwordenc_val\" style='width:100%' type=\"password\"  autocomplete=\"off\" /></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"passwordenc_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"passwordenc_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"passwordenc_oldval\" /></td>";
         task_tdedit += "</tr>";
        }
        else
        {
         task_tdedit += "<tr id=\"password_row\" style=\"display:none\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Password:</td>";
         task_tdedit += "<td><input name=\"password_val\" style='width:100%' type=\"password\" value=\"" + val + "\" autocomplete=\"off\" /></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"password_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"password_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"password_oldval\" /></td>";
         task_tdedit += "</tr>";
        }
       }
       else if (label == "Hostname")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"hostname_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"hostname_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"hostname_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"hostname_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "SSH Port Number")
       {
        task_tdedit += "<tr id=\"sshport_row\" style=\"display:none\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"sshport_val\" style='width:8em' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"sshport_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"sshport_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"sshport_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Protocol")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select onChange=\"ProtocolChanged()\"; name=\"protocol_val\" id=\"protocol_val\">";
        summProtocol = val;

        if (val == "sftp")
        {
         val = "sftp";
         task_tdedit += "<option value=\"sftp\" selected>ssh</option>";
         task_tdedit += "<option value=\"win\">WinRM</option>";
        }
        else if (val == "win")
        {
         val = "win";
         task_tdedit += "<option value=\"sftp\">ssh</option>";
         task_tdedit += "<option value=\"win\" selected>WinRM</option>";
        }
        else
        {
         val = "ssh";
         task_tdedit += "<option value=\"sftp\" selected>ssh</option>";
         task_tdedit += "<option value=\"win\" selected>WinRM</option>";
        }

        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"protocol_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"protocol_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"protocol_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Engine")
       {
        label = "Engine Hostname";
        var engine = val;
        if (typeof engine.name == "undefined")
        {
         engine = new Object();
         engine.name = "";
         engine.type = "ei";
         engine.id = 0;
        }
        val = engine.name;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"engine_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"engine_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"engine_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"engine_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Approval Domain" || label == "Move to Domain" || label == "Create Version in Domain")
       {
        var tasktype = "utap";

        if (label == "Approval Domain")
         tasktype = "utap";
        else if (label == "Move to Domain")
         tasktype = "utmp";
        else if (label == "Create Version in Domain")
         tasktype = "utcv";
        else
         tasktype = "";

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"domain_val\">";
        save_domain_val = val;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"domain_field\" value=\"" + tasktype + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"domain_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"domain_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Endpoint Types")
       {
 	   if (hideOption())
 		  continue;

        if (comptypedone == false)
        {
         task_tdedit += "<tr id=\"servercomptype_row_edit\">";
         task_tdedit += "<td style=\"text-align:left;vertical-align:top;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td ><select multiple name=\"servercomptype_val\">";
         var parts = val.split(";");

         save_servercomptype_val = parts[0];
         val = parts[1];
         comptypedone = true;
         task_tdedit += "</td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"servercomptype_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"servercomptype_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"servercomptype_oldval\" value=\"" + val + "\"/></td>";
         task_tdedit += "</tr>";
        }
        else
        {
         var parts = val.split(";");
         save_servercomptype_val += ";" + parts[0];
         val = parts[1];
        }
       }
       else if (label == "Action to Run")
       {
        var action = val;
        if (typeof action.name == "undefined") {
         action = new Object();
         action.name = "";
         action.type = "ac";
         action.id = 0;
           }
           val = action.name;
           linkval="<a href='javascript:SwitchDisplay(\"ac"+action.id+"\");'>"+action.name+"</a>";
        var tasktype = "utat";
        task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select name=\"taskaction_val\">";
           task_tdedit += "<option value=\"\"></option></select>";
           save_taskaction_val = action.id;
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"taskaction_field\" value=\"" + tasktype + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"taskaction_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"taskaction_oldval\" value=\"" + val + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Linked Task")
       {
        var tasktype = "utrp";

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"linkedtask_val\">";
        save_linkedtask_val = val;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"linkedtask_field\" value=\"" + tasktype + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"linkedtask_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"linkedtask_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Port")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"port_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"port_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"port_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"port_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Base Directory")
       {
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"basedirectory_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"basedirectory_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"basedirectory_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Display Name")
       {
        task_tdedit += "<tr id=\"fragmentname_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"fragmentname_val\" type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fragmentname_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fragmentname_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fragmentname_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Filepath")
       {
        task_tdedit += "<tr id=\"filepath_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td><input name=\"filepath_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filepath_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filepath_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filepath_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Owner")
       {
        var owner = val;
        if (typeof owner.name != "undefined")
        {
         val = owner.name;
         if (owner.type != "us")
          isuser = false;
        }
        else
        {
         owner = new Object();
         owner.type = "us";
         owner.name = "";
         owner.id = 0;
         val = "";
        }

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Owner Type:</td>";
        task_tdedit += "<td ><select id=\"ownertype_val\" name=\"ownertype_val\">";
        if (isuser)
        {
         task_tdedit += "<option value=\"User\" selected>User</option>";
         task_tdedit += "<option value=\"Group\">Group</option>";
        }
        else
        {
         task_tdedit += "<option value=\"User\">User</option>";
         task_tdedit += "<option value=\"Group\" selected>Group</option>";
        }
        task_tdedit += "</select></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"ownertype_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"ownertype_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"ownertype_oldval\" value=\"" + owner.type + "\"/></td>";
        task_tdedit += "</tr>";

        task_tdedit += "<tr id=\"owner_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Owner:</td>";
        task_tdedit += "<td ><select name=\"owner_val\">";
        save_owner_val = owner.name;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"owner_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"owner_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"owner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
        task_tdedit += "</tr>";

        task_tdedit += "<tr id=\"group_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">Groups:</td>";
        task_tdedit += "<td ><select name=\"groupowner_val\">";
        save_groupowner_val = owner.name;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupowner_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupowner_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"groupowner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
        task_tdedit += "</tr>";

        td += "<tr id=\"ownertype\" ><td class=\"summlabel\">";
        td += "Owner Type";
        td += ":</td><td>";
        if (owner.type == "us")
          td += "User";
        else
          td += "Group";
       }
       else if (label == "Parent Domain")
       {
        var domain = val;
        val = domain.value;
        if (val == "")
         val = "This is the top level domain";
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td name=\"parentdomain_val\" >" + val + "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"parentdomain_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"parentdomain_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"parentdomain_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Always Deploy")
       {
        var always = val;
        val = always.value;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val || objid == -1)
         task_tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"off\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Deploy Sequentially")
       {
        var seq = val;
        val = seq.value;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val || objid == -1)
         task_tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"deployseq_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"deployseq_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"deployseq_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"deployseq_oldval\" value=\"off\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Database Roll-forward/Rollback")
       {
        var database = val;
        val = database.value;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input id=\"database_val\" name=\"database_val\" type=\"checkbox\"/ checked></td>";
        else
         task_tdedit += "<td><input id=\"database_val\" name=\"database_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"database_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"database_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"database_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";

        val=val?"Yes":"No";
       }
       else if (label == "Delete Corresponding Directory")
       {
       }
       else if (label == "Auto Ping")
       {
//        var always = val;
//        val = always.value;
//        task_tdedit += "<tr>";
//        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//        if (val || objid == -1)
//            task_tdedit += "<td><input id=\"autoping_val\" name=\"autoping_val\" type=\"checkbox\" checked /></td>";
//           else
//            task_tdedit += "<td><input id=\"autoping_val\" name=\"autoping_val\" type=\"checkbox\"/></td>";
//        task_tdedit += "<td><input type=\"hidden\" name=\"autoping_field\" value=\"" + field + "\"/></td>";
//        task_tdedit += "<td><input type=\"hidden\" name=\"autoping_callback\" value=\"" + callback + "\"/></td>";
//        if (val)
//         task_tdedit += "<td><input type=\"hidden\" name=\"autoping_oldval\" value=\"on\"/></td>";
//        else
//         task_tdedit += "<td><input type=\"hidden\" name=\"autoping_oldval\" value=\"off\"/></td>";
//        task_tdedit += "</tr>";
//        summAutoPing=val;
//        val=val?"Yes":"No";
       }
       else if (label == "Auto MD5 Check")
       {
//        var always = val;
//        val = always.value;
//        task_tdedit += "<tr id=\"automd5_row\">";
//        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//        if (val || objid == -1)
//            task_tdedit += "<td><input id=\"automd5check_val\" name=\"automd5check_val\" type=\"checkbox\" checked /></td>";
//           else
//            task_tdedit += "<td><input id=\"automd5check_val\" name=\"automd5check_val\" type=\"checkbox\"/></td>";
//        task_tdedit += "<td><input type=\"hidden\" name=\"automd5check_field\" value=\"" + field + "\"/></td>";
//        task_tdedit += "<td><input type=\"hidden\" name=\"automd5check_callback\" value=\"" + callback + "\"/></td>";
//        if (val)
//         task_tdedit += "<td><input type=\"hidden\" name=\"automd5check_oldval\" value=\"on\"/></td>";
//        else
//         task_tdedit += "<td><input type=\"hidden\" name=\"automd5check_oldval\" value=\"off\"/></td>";
//        task_tdedit += "</tr>";
//        val=val?"Yes":"No";
       }
       else if (label == "Ping Interval")
       {
//        task_tdedit += "<tr id=\"pinginterval_row\">";
//           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//           task_tdedit += "<td >";
//           task_tdedit += "<select name=\"pinginterval_val\">";
//           var pif=false;
//           for (i=15;i<=240;i=i+15) {
//            if (i == val) pif=true;
//            sel=(i == val)?"selected":"";
//            task_tdedit += "<option value=\""+i+"\" "+sel+">"+i+"</option>";
//           }
//           if (!pif) val="15"; // default value
//           task_tdedit += "</select>"
//           task_tdedit += " (minutes)</td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pinginterval_field\" value=\"" + field + "\"/></td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pinginterval_oldval\" value=\""+val+"\"/></td>";
       }
       else if (label == "Ping Start")
       {
//        task_tdedit += "<tr id=\"pingstart_row\">";
//           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//           task_tdedit += "<td >";
//           task_tdedit += "<select name=\"pingstart_val\">";
//           valhours = val.substr(0,2);
//           valmins  = val.substr(3,2);
//           valsecs = (valhours * 3600) + (valmins * 60);
//           for (h=0;h<=23;h++)
//           for (m=0;m<=45;m=m+15) {
//            secs = (h*3600) + (m*60);
//            sel=(secs == valsecs)?"selected":"";
//            task_tdedit += "<option value=\""+secs+"\" "+sel+">"+(h<10?"0"+h:h)+":"+(m<10?"0"+m:m)+"</option>";
//           }
//           task_tdedit += "</select>"
//           task_tdedit += "</td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pingstart_field\" value=\"" + field + "\"/></td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pingstart_oldval\" value=\""+val+"\"/></td>";
       }
       else if (label == "Ping End")
       {
//        task_tdedit += "<tr id=\"pingend_row\">";
//           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//           task_tdedit += "<td >";
//           task_tdedit += "<select name=\"pingend_val\">";
//           valhours = val.substr(0,2);
//           valmins  = val.substr(3,2);
//           valsecs = (valhours * 3600) + (valmins * 60);
//           for (h=0;h<=23;h++)
//           for (m=(h==0)?15:0;m<=45;m=m+15) {
//            secs = (h*3600) + (m*60);
//            sel=(secs == valsecs)?"selected":"";
//            task_tdedit += "<option value=\""+secs+"\" "+sel+">"+(h<10?"0"+h:h)+":"+(m<10?"0"+m:m)+"</option>";
//           }
//           task_tdedit += "</select>"
//           task_tdedit += "</td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pingend_field\" value=\"" + field + "\"/></td>";
//           task_tdedit += "<td><input type=\"hidden\" name=\"pingend_oldval\" value=\""+val+"\"/></td>";
       }
       else if (label == "LifeCycle Domain")
       {
        var domain = val;
        val = domain.value;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">All Subdomains are Lifecycles:</td>";
        if (val)
        {
         summSaveisLifeCycle = true;
         isLifeCycle = true;
         task_tdedit += "<td><span id=\"lifecycledomain_txt\" style=\"display:none\">Yes</span><input name=\"lifecycledomain_val\" id=\"lifecycledomain_val\" onClick=\"javascript:ToggleLifeCycle();\" type=\"checkbox\" checked /></td>";
        }
        else
        {
         summSaveisLifeCycle = false;
         isLifeCycle = false;
         task_tdedit += "<td><span id=\"lifecycledomain_txt\" style=\"display:none\">Yes</span><input name=\"lifecycledomain_val\" id=\"lifecycledomain_val\" onClick=\"javascript:ToggleLifeCycle();\" type=\"checkbox\"/></td>";
        }
        task_tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Result Is Expr")
       {
        var resultisexpr = val;
        val = resultisexpr.value;
        task_tdedit += "<tr id=\"resultisexpr_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"resultisexpr_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input name=\"resultisexpr_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"resultisexpr_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Allocate Terminal")
       {
        var allocateterminal = val;
        val = allocateterminal.value;
        console.log("Allocate Terminal="+val);
        task_tdedit += "<tr id=\"allocateterminal_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"allocateterminal_val\" style='width:1.5em' type=\"checkbox\" checked />*</td>";
        else
         task_tdedit += "<td><input name=\"allocateterminal_val\" style='width:1.5em' type=\"checkbox\"/>*</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"allocateterminal_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
        allocate_terminal_shown=true;
        val=val?"Yes":"No";
       }
       else if (label == "Copy to Remote")
       {
        var copytoremote = val;
        val = copytoremote.value;
        task_tdedit += "<tr id=\"copytoremote_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"copytoremote_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input name=\"copytoremote_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"copytoremote_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"copytoremote_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Available in SubDomains")
       {
        var subdomains = val;
        val = subdomains.value;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"subdomains_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input name=\"subdomains_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"subdomains_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"subdomains_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"subdomains_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"subdomains_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Account Locked")
       {
        var showoutput = val;
        val = showoutput.value;
        task_tdedit += "<tr id=\"al_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"accountlocked_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input name=\"accountlocked_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"accountlocked_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"accountlocked_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"accountlocked_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"accountlocked_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Force Change Password")
       {
        var showoutput = val;
        val = showoutput.value;
        task_tdedit += "<tr id=\"fcp_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (val)
         task_tdedit += "<td><input name=\"forcechangepassword_val\" type=\"checkbox\" checked /></td>";
        else
         task_tdedit += "<td><input name=\"forcechangepassword_val\" type=\"checkbox\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_callback\" value=\"" + callback + "\"/></td>";
        if (val)
         task_tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_oldval\" value=\"on\"/></td>";
        else
         task_tdedit += "<td><input type=\"hidden\" name=\"forcechangepassword_oldval\" value=\"false\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Filter Level")
       {
        if (objid > 0 && val == "Components")
        {
         task_tdedit += "<tr>";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td ><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
         if (val == "Components")
         {
          task_tdedit += "<option value=\"Components\" selected>Components</option>";
          task_tdedit += "<option value=\"Items\">Items</option>";
         }
         else
         {
          task_tdedit += "<option value=\"Components\">Components</option>";
          task_tdedit += "<option value=\"Items\" selected>Items</option>";
         }
        }
        else
        {
         task_tdedit += "<tr style=\"display:none\">";
         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td ><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
         task_tdedit += "<option value=\"Components\">Components</option>";
         task_tdedit += "<option value=\"Items\" selected>Items</option>";
        }

        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filterlevel_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filterlevel_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"filterlevel_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";

        save_filter_val = val;
       }
       else if (label == "Endpoint Type" && summSaveobjtype != "se")
       {
        if (objid === -1)
        	val = "";
        task_tdedit += "<tr id=\"endpointtype_row_edit\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"comptype_val\" id=\"comptype_val\" onChange=\"CompTypeChanged()\">";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"comptype_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"comptype_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"comptype_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";

        save_comptype_val = val;
       }
       else if (label == "Change Request Data Source") {
        console.log("Change Request Data Source seen");
        console.log(val);
        var datasource = val;
        var oldval;
        if (typeof datasource.name == "undefined") {
         datasource = new Object();
         datasource.name = "";
         datasource.type = "ds";
         datasource.id = 0;
         oldval="";
        } else {
         oldval=datasource.type+datasource.id;
        }
        val = datasource.name;
        summSaveBTDS = val;
           linkval=datasource.showlink?"<a href='javascript:SwitchDisplay(\"ds"+datasource.id+"\");'>"+datasource.name+"</a>":datasource.name;
           task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select name=\"bt_datasource_val\">";
           task_tdedit += "<option value=\"\"></option></select>";
           save_datasource_val = val;
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_field\" value=\"" + field + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_oldval\" value=\"" + oldval + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Build Job") {
        console.log("Build Job seen");
        console.log(val);
        var buildjob = val;
        var oldval;
        if (typeof buildjob.name == "undefined") {
         buildjob = new Object();
         buildjob.name = "";
         buildjob.type = "bj";
         buildjob.id = 0;
         oldval="";
        } else {
         oldval=buildjob.type+buildjob.id;
        }
        val = buildjob.name;
        summSaveBTDS = val;
           linkval=buildjob.showlink?"<a href='javascript:SwitchDisplay(\"bj"+buildjob.id+"\");'>"+buildjob.name+"</a>":val;
           task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select name=\"buildjob_val\">";
           task_tdedit += "<option value=\"\"></option></select>";
           save_buildjob_val = val;
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"buildjob_field\" value=\"" + field + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"buildjob_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"buildjob_oldval\" value=\"" + oldval + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Last Build Number") {
        if (val==0) val="";
        else val="<a onClick=\"javascript:DisplayBuild("+objid+","+val+");\">#"+val+"</a>";

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td name=\"lastlogin_val\" >" + val + "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Project Name" || label == "Plan Name") {
        var project = val;
        var oldval=val;

           task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select name=\"projectname_val\">";
           task_tdedit += "<option value=\"\"></option></select>";
           save_projectname_val = val;
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"projectname_field\" value=\"" + field + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"projectname_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"projectname_oldval\" value=\"" + oldval + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Roll Forward")
       {
        if (save_filter_val == "Items")
         task_tdedit += "<tr id=\"rollforward_row\" style=\"display:none\" >";
        else
         task_tdedit += "<tr id=\"rollforward_row\">";

        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"rollup_val\">";
        if (val == "OFF")
        {
         val = "Off";
         task_tdedit += "<option value=\"OFF\" selected>Off</option>";
         task_tdedit += "<option value=\"ON\">On</option>";
         task_tdedit += "<option value=\"ALL\">All</option>";
        }
        else if (val == "ON")
        {
         val = "On";
         task_tdedit += "<option value=\"OFF\">Off</option>";
         task_tdedit += "<option value=\"ON\" selected>On</option>";
         task_tdedit += "<option value=\"ALL\">All</option>";
        }
        else
        {
         val = "All";
         task_tdedit += "<option value=\"OFF\">Off</option>";
         task_tdedit += "<option value=\"ON\">On</option>";
         task_tdedit += "<option value=\"ALL\" selected>All</option>";
        }
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollup_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollup_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollup_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Rollback")
       {
        if (save_filter_val == "Items")
         task_tdedit += "<tr id=\"rollback_row\" style=\"display:none\" >";
        else
         task_tdedit += "<tr id=\"rollback_row\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"rollback_val\">";
        if (val == "OFF")
        {
         val = "Off";
         task_tdedit += "<option value=\"OFF\" selected>Off</option>";
         task_tdedit += "<option value=\"ON\">On</option>";
         task_tdedit += "<option value=\"ALL\">All</option>";
        }
        else if (val == "ON")
        {
         val = "On";
         task_tdedit += "<option value=\"OFF\">Off</option>";
         task_tdedit += "<option value=\"ON\" selected>On</option>";
         task_tdedit += "<option value=\"ALL\">All</option>";
        }
        else
        {
         val = "All";
         task_tdedit += "<option value=\"OFF\">Off</option>";
         task_tdedit += "<option value=\"ON\">On</option>";
         task_tdedit += "<option value=\"ALL\" selected>All</option>";
        }
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollback_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollback_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"rollback_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Endpoint Type")
       {
        var st = val;
        val = "st" + st.id;
        label = "Endpoint Operating System";

        task_tdedit += "<tr id=\"servertype_row_edit\">";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"servertype_val\">";
        if (val == "st3")
        {
         oldval = val;
         val = "Unix";
         task_tdedit += "<option value=\"st1\" selected>Unix</option>";
         task_tdedit += "<option value=\"st2\">Windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }
        else if (val == "st4")
        {
         oldval = val;
         val = "Unix";

         task_tdedit += "<option value=\"st1\" selected>Unix</option>";
         task_tdedit += "<option value=\"st2\">windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }
        else if (val == "st1")
        {
         oldval = val;
         val = "Unix";

         task_tdedit += "<option value=\"st1\" selected>Unix</option>";
         task_tdedit += "<option value=\"st2\">Windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }
        else if (val == "st5")
        {
         oldval = val;
         val = "Tandem";

         task_tdedit += "<option value=\"st1\">Unix</option>";
         task_tdedit += "<option value=\"st2\">Windows</option>";
         task_tdedit += "<option value=\"st5\" selected>Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }
        else if (val == "st6")
        {
         oldval = val;
         val = "Stratus";

         task_tdedit += "<option value=\"st1\">Unix</option>";
         task_tdedit += "<option value=\"st2\">Windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\" selected>Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }
        else if (val == "st7")
        {
         oldval = val;
         val = "OpenVMS";

         task_tdedit += "<option value=\"st1\">Unix</option>";
         task_tdedit += "<option value=\"st2\">Windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\" selected>OpenVMS</option>";
        }
        else
        {
         oldval = val;
         val = "Windows";

         task_tdedit += "<option value=\"st1\">Unix</option>";
         task_tdedit += "<option value=\"st2\" selected>Windows</option>";
         task_tdedit += "<option value=\"st5\">Tandem</option>";
         task_tdedit += "<option value=\"st6\">Stratus</option>";
         task_tdedit += "<option value=\"st7\">OpenVMS</option>";
        }

        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"servertype_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"servertype_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"servertype_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label.toLowerCase() == "Pre-Action".toLowerCase() && summSaveobjtype != "rl")
       {
	    if (hideOption())
	     continue;

        var action = val;
        console.log("Pre-Action seen");
        console.log(val);
        var oldval;
        if (typeof action.name == "undefined")
        {
         action = new Object();
         action.name = "";
         action.type = "ac";
         action.id = 0;
         oldval="";
        } else {
         oldval=action.type+action.id;
        }
        val = action.name;
        var otid=(action.type=="ac")?action.type+action.id:action.type+action.id+"-"+action.kind;
        linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>":action.name;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"preaction_val\">";
        task_tdedit += "<option value=\"\"></option></select>";
        save_preaction_val = action.id;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"preaction_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"preaction_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"preaction_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label.toLowerCase() == "Post-Action".toLowerCase() && summSaveobjtype != "rl")
       {
	   if (hideOption())
	     continue;

        var action = val;
        var oldval;
        if (typeof action.name == "undefined")
        {
    action = new Object();
    action.name = "";
    action.type = "ac";
    action.id = 0;
    oldval="";
        } else {
         oldval=action.type+action.id;
        }
        val = action.name;
        var otid=(action.type=="ac")?action.type+action.id:action.type+action.id+"-"+action.kind;
        linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>":action.name;
        // linkval="<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a>";
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"postaction_val\">";
        save_postaction_val = action.id;
        task_tdedit += "<option value=\"\"></option></select>";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"postaction_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"postaction_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"postaction_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Custom Action" && summSaveobjtype != "rl")
       {
	   if (hideOption())
	     continue;

        var action = val;
        var oldval;
        if (typeof action.name == "undefined")
        {
         action = new Object();
         action.name = "";
         action.type = "ac";
         action.id = 0;
         oldval="";
        } else {
         oldval=action.type+action.id;
        }
        val = action.name;
        //linkval="<a href='javascript:SwitchDisplay(\""+action.type+action.id+"\");'>"+action.name+"</a>";
        linkval=action.showlink?"<a href='javascript:SwitchDisplay(\""+action.type+action.id+"\");'>"+action.name+"</a>":action.name;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        if (summSaveobjtype == "co") {
         task_tdedit += "<td ><select name=\"customaction_val\" id=\"customaction_val\" onChange=\"clearItemOptions();\">";
        } else {
         task_tdedit += "<td ><select name=\"customaction_val\" id=\"customaction_val\">";
        }
        save_customaction_val = action.id;
        task_tdedit += "<option value=\"\"></option></select>";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"customaction_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"customaction_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"customaction_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Credential")
       {
        var cred = val;
        var oldval;
        if (typeof cred.name == "undefined")
        {
         cred = new Object();
         cred.name = "";
         cred.type = "cr";
         cred.id = 0;
         oldval="";
        } else {
         oldval = cred.type + cred.id;
        }
        linkval=cred.showlink?"<a href='javascript:SwitchDisplay(\"cr"+cred.id+"\");'>"+cred.name+"</a>":cred.name;
        val = cred.name;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"credential_val\">";
        save_credential_val = val;
        task_tdedit += "<option value=\"\">None Required</option></select>";
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"credential_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"credential_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"credential_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Category")
       {
        var category = val;
        if (typeof category.name == "undefined")
        {
         category = new Object();
         category.name = "";
         category.type = "cy";
         category.id = 0;
        }
        val = category.name;
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><input name=\"category_val\" selectBoxOptions=\"\" />";
        save_category_val = val;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"category_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"category_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"category_oldval\" value=\"" + category.type + category.id + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Validation Data Source") {
        console.log("VDS seen");
        console.log(val);
        var datasource = val;
        var oldval;
        if (typeof datasource.name == "undefined") {
         datasource = new Object();
         datasource.name = "";
         datasource.type = "ds";
         datasource.id = 0;
         oldval="";
        } else {
         oldval=datasource.type+datasource.id;
        }
        val = datasource.name;
        summSaveVDS = val;
           linkval="<a href='javascript:SwitchDisplay(\"ds"+datasource.id+"\");'>"+datasource.name+"</a>";
           task_tdedit += "<tr>";
           task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
           task_tdedit += "<td ><select onChange=\"TogglePasswordFields();\" name=\"datasource_val\">";
           task_tdedit += "<option value=\"\"></option></select>";
           save_datasource_val = val;
           task_tdedit += "</td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_field\" value=\"" + field + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_callback\" value=\"" + callback + "\"/></td>";
           task_tdedit += "<td><input type=\"hidden\" name=\"datasource_oldval\" value=\"" + oldval + "\"/></td>";
           task_tdedit += "</tr>";
       }
       else if (label == "Successful Deployment Template" ||
          label == "Approved Notify Template" ||
          label == "Success Notification Template" ||
          label == "Request Notification Template" ||
          label == "Ping Failure Template")
       {
        var template = val;
        var oldval;
        if (typeof template.name == "undefined")
        {
         template = new Object();
         template.name = "";
         template.type = "te";
         template.id = 0;
         oldval="";
        } else {
         oldval=template.type+template.id;
        }
        val = template.name;
        linkval=template.showlink?"<a href='javascript:SwitchDisplay(\"te"+template.id+"\");'>"+template.name+"</a>":template.name;
        console.log("label=["+label+"]");
        if (label == "Ping Failure Template") {
         task_tdedit += "<tr id=\"pingtemplate_row\">";
        } else {
         task_tdedit += "<tr>";
        }
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"template_val\">";
        task_tdedit += "<option value=\"\"></option></select>";
        save_template_val = val;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"template_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"template_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"template_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Failed Deployment Template" ||
          label == "Rejected Notify Template" ||
          label == "Failure Notification Template")
       {
        var template = val;
        var oldval;
        if (typeof template.name == "undefined")
        {
         template = new Object();
         template.name = "";
         template.type = "te";
         template.id = 0;
         oldval="";
        } else {
         oldval=template.type+template.id;
        }
        val = template.name;
        linkval=template.showlink?"<a href='javascript:SwitchDisplay(\"te"+template.id+"\");'>"+template.name+"</a>":template.name;
//        if (label == "MD5 Mismatch Template"){
//         task_tdedit += "<tr id=\"md5template_row\">";
//        } else {
         task_tdedit += "<tr>";
//        }
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td ><select name=\"fail_template_val\">";
        task_tdedit += "<option value=\"\"></option></select>";
        save_fail_template_val = val;
        task_tdedit += "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fail_template_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fail_template_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"fail_template_oldval\" value=\"" + oldval + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Type")
       {
        var typeobj = val;
        var oldval;
        if (typeof typeobj.name == "undefined")
        {
         typeobj = new Object();
         typeobj.name = "";
         typeobj.type = "pd";
         typeobj.id = 0;
         oldval="";
        } else {
         oldval=typeobj.type+typeobj.id;
        }
        val = typeobj.name;
        console.log("Type: val="+val);
        task_tdedit += "<tr>";
         if (objid < 0)
         {
          var name = notifiertype;
          val = notifiertype;

          if (objtype == "ds")
          {
           val = dstype;
           if (dstype == "ldap")
            name = "LDAP";
           else if (dstype == "bugzilla")
            name = "Bugzilla"
           else if (dstype == "github")
            name = "GitHub"
           else if (dstype == "jira")
            name = "Jira"
           else if (dstype == "odbc")
            name = "ODBC"
          }
          else if (objtype == "re")
          {
           val = repotype;

           if (repotype == "filesystem")
             name = "File System";
           else if (repotype == "http")
             name = "Http";
           else if (repotype == "ftprep")
            name = "Ftp";
           else if (repotype == "meister")
            name = "Meister";
           else if (repotype == "perforce")
            name = "Perforce";
           else if (repotype == "svn")
            name = "SVN";
          }
          else if (objtype == "co")
          {
           val = addcomptype;

           if (addcomptype == "docker")
             name = "Docker";
           else if (addcomptypepotype == "file")
             name = "File";
           else if (addcomptype == "database")
            name = "Database";
          }
          else if (objtype == "no")
          {
           val = notifiertype;

           if (notifiertype == "smtpemail")
             name = "Email";
           else if (notifiertype == "txtlocal")
             name = "SMS";
           else if (notifiertype == "hipchat")
            name = "HipChat";
           else if (notifiertype == "slack")
            name = "Slack";
          }
          else if (objtype == "be")
          {
           val = buildenginetype;

           if (buildenginetype == "jenkins")
             name = "Jenkins";
          }
         }
         else
         {
          var name = "";

          if (objtype == "ds")
          {
           if (val == "ldap")
            name = "LDAP";
           else if (val == "bugzilla")
            name = "Bugzilla"
           else if (val == "github")
            name = "GitHub"
           else if (val == "jira")
            name = "Jira"
           else if (val == "odbc")
            name = "ODBC"
          }
          else if (objtype == "no")
          {
           if (val == "smtpemail")
             name = "Email";
           else if (val == "txtlocal")
             name = "SMS";
           else if (val == "hipchat")
            name = "HipChat";
           else if (val == "slack")
            name = "Slack";
          }
          else if (objtype == "re")
          {
           if (val == "filesystem")
             name = "File System";
           else if (val == "http")
             name = "Http";
           else if (val == "ftprep")
            name = "Ftp";
           else if (val == "meister")
            name = "Meister";
           else if (val == "perforce")
            name = "Perforce";
           else if (val == "svn")
            name = "SVN";
          }
          else if (objtype == "co")
          {
           if (val== "docker")
             name = "Docker";
           else if (val == "file")
             name = "File";
           else if (val == "database")
            name = "Database";
          }
          else if (objtype == "be")
          {
           if (val == "jenkins")
             name = "Jenkins";
          }
         }

         task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
         task_tdedit += "<td>" + name + "</td>";
         task_tdedit += "<td ><select name=\"type_val\" style=\"display:none\">";
         save_type_val = val;
         task_tdedit += "</td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"type_field\" value=\"" + field + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"type_callback\" value=\"" + callback + "\"/></td>";
         task_tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"" + oldval + "\"/></td>";
         task_tdedit += "</tr>";
         /*
        }
        */
       }
       else if (label == "Created")
       {
        var created = val;
        if (typeof created.when != "undefined")
        {
         //var d = convertDate(created.when);
         //val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
         val = created.when + " by " + created.tooltip;
        }
        else
         val = "";

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td>" + val + "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Modified")
       {
        var modified = val;
        console.log("modified (Modified)");
        console.log(modified);
        if (typeof modified.when != "undefined")
        {
         //var d = convertDate(modified.when);
         //val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
         val = modified.when + " by " + modified.tooltip;
        }
        else
         val = "";

        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td>" + val + "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"created_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }
       else if (label == "Last Login")
       {
        var modified = val;
        console.log("Last Login");
        console.log(val);
        if (modified.value == 0) {
         val="";
        } else if (typeof modified.name != "undefined") {
         val = modified.name;
        } else {
         val = "";
        }
        task_tdedit += "<tr>";
        task_tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        task_tdedit += "<td name=\"lastlogin_val\" >" + val + "</td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_field\" value=\"" + field + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_callback\" value=\"" + callback + "\"/></td>";
        task_tdedit += "<td><input type=\"hidden\" name=\"lastlogin_oldval\" value=\"" + val + "\"/></td>";
        task_tdedit += "</tr>";
       }

       if (label == "Owner")
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (isuser)
         td += val;
        else
         td += val;
       }
       else if ( label == "LifeCycle Domain" ||
          label == "Copy to Remote" ||
          label == "Always Deploy" ||
          label == "Deploy Sequentially" ||
          label == "Result Is Expr" ||
          label == "Account Locked" ||
          label == "Force Change Password" ||
          label == "Available in SubDomains")
       {
        if (label == "LifeCycle Domain") label = "All Subdomains are Lifecycles";
        var myid = label.toLocaleLowerCase().replace(/ /g, '') + "_sumrow";

        td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (val == true)
         td += "Yes";
        else
         td += "No";
       }
       else if (label == "Password")
       {
        // re-added to display password
        if (((save_kind_id == 0 || save_kind_id == 4) && field == 805) || ((save_kind_id == 2) && field == 803))
        {
         td += "<tr><td class=\"summlabel\">";
         td += label;
         td += ":</td><td>";
         if (field == 805)
          td += val;
         else
          td += "*********";
        }
        else if (save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 5 && save_kind_id != 10)
        {
         td += "<tr><td class=\"summlabel\">";
         td += label;
         td += ":</td><td>";
         if (val.length == 0)
         td += "";
        else
         td += "*********";
        }

       }
       else if (label == "Username")
       {
        console.log("Username seen, field="+field+" save_kind_id="+save_kind_id);
        if (((save_kind_id == 0 || save_kind_id == 4) && field == 804) || ((save_kind_id == 2) && (field == 802 || field == 807)))
        {
         td += "<tr><td class=\"summlabel\">";
         td += label;
         td += ":</td><td>";
         if (field == 802)
          td += "*********";
         else
          td += val;
        }
        else if ((save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 10 && save_kind_id != 5) || (save_kind_id == 5 && field==804))
        {
         console.log("added row for username");
         td += "<tr><td class=\"summlabel\">";
         td += label;
         td += ":</td><td>";
         td += val;
        }
       }
       else if (label == "Kind")
       {
         td += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\">";
         td += "Object:";
         td += "</td><td>";
         td += objtypeName;
         td += "</td></tr>";
        td += "<tr id='kind_sumrow'><td id=\"kind_summ\" class=\"summlabel\">";
        td += "Type";
        td += ":</td><td>";
        td += val;
       }
       else if (label.toLowerCase() == "Filter Level".toLowerCase() && save_filter_val == "Items")
       {

       }
       else if (label.toLowerCase() == "Pre-Action".toLowerCase() && summSaveobjtype == "rl")
       {

       }
       else if (label.toLowerCase() == "Post-Action".toLowerCase() && summSaveobjtype == "rl")
       {

       }
       else if (label.toLowerCase() == "Custom Action".toLowerCase() && summSaveobjtype == "rl")
       {

       }
       else if ((label == "Roll Forward" || label == "Rollback") && save_filter_val == "Items")
       {

       }
       else if (label.toLowerCase() == "Full Domain".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "XPos".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Show Output".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "YPos".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "recursive".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Auto Ping".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Auto MD5 Check".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Ping Start".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Ping End".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "Ping Interval".toLowerCase())
       {

       }
       else if (label.toLowerCase() == "MD5 Mismatch Template".toLowerCase())
       {

       }
       else if (label == "Endpoint Types" )
       {
		if (hideOption())
 		  continue;

         var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";

         if (comptypelabeldone == false)
         {
          td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
          td += label;
          td += ":</td><td>";
          td += (linkval!=null)?linkval:val;
         }
         else
         {
          td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
          td += "";
          td += "</td><td>";
          td += (linkval!=null)?linkval:val;
         }
         comptypelabeldone = true;
       }
       else if (label == "Type")
       {
        if (objtype == "ds")
        {
         if (val == "ldap")
          name = "LDAP";
         else if (val == "bugzilla")
          name = "Bugzilla"
         else if (val == "github")
          name = "GitHub"
         else if (val == "jira")
          name = "Jira"
         else if (val == "odbc")
          name = "ODBC"
        }
        else if (objtype == "no")
        {
         if (val == "smtpemail")
           name = "Email";
         else if (val == "txtlocal")
           name = "SMS";
         else if (val == "hipchat")
          name = "HipChat";
         else if (val == "slack")
          name = "Slack";
        }
        else if (objtype == "re")
        {
         if (val == "filesystem")
           name = "File System";
         else if (val == "http")
           name = "Http";
         else if (val == "ftprep")
          name = "Ftp";
         else if (val == "meister")
          name = "Meister";
         else if (val == "perforce")
          name = "Perforce";
         else if (val == "svn")
          name = "SVN";
        }
        else if (objtype == "co")
        {
         if (val== "docker")
           name = "Docker";
         else if (val == "file")
           name = "File";
         else if (val == "database")
          name = "Database";
        }
        else if (objtype == "be")
        {
         if (val == "jenkins")
           name = "Jenkins";
        }
        var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
        console.log("3) <tr id=\"" + myid + "\" ><td class=\"summlabel\">");
        td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        td += name;
       }
       else
       {
        // Everything else
        var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
        console.log("3) <tr id=\"" + myid + "\" ><td class=\"summlabel\">");
        td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        td += (linkval!=null)?linkval:val;
       }
       td += "</td></tr>";
      }

       task_tdedit += "</table></form>";
       $("#" + tablename + " > tbody").html(td);

       if (objid < 0) {
        console.log("objid="+objid+" opening editing dialog (creation?)");
        EditTaskSummaryButton(false);
      }
      else
        $("#" + prevcb).prop("checked", true);

	 },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
}

function LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 envdropdown = "";
 appdropdown = "";
 srvdropdown = "";
 emailtodropdown = "";
 groupmembershipdropdown = "";
 usermembershipdropdown = "";

 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 repolabeladded = false;

  var href = window.location.href;
  href = href.replace("#dhmain","");
  href = href.replace("#dhnav", "");

  window.history.pushState('dhnav', null, href + '#dhmain#dhnav');

 $("#summ_data_edit").hide();
 $("#summ_data").show();
 $('.save_button').css("color","grey");
 $('.cancel_button').css("color","grey");

 $("#engine_details_row_edit").show();

 if (objid < 0 && (objtype == "co" || objtype == "cv"))
  addParams += "&comptype=" + addcomptype

 console.log("LoadSummaryData, tablename="+tablename);

 if (objtype == "co" || objtype == "cv")
 {
  $("#summ_header_buttons").hide();
  $("#row-10-right-panel").show();
 }
 else
 {
  $("#compowner_summ_title").hide();
  $("#summ_header_buttons").hide();
 }

  $("#summ_header_buttons > button.add_button").hide();
  $("#summ_header_buttons > button.delete_button").hide();

 $("#domnav-panel").hide();

 if (objtype != "do" && objtype != "us" && objtype != "gr")
 {
  $("#summ_header_buttons > button.add_button").show();
  $("#summ_header_buttons > button.delete_button").show();
 }

 if (!$("#domnav-panel").is(":visible"))
 {
  $("#summ_data").show();
  $("#summ_data_edit").hide();
  $("button.test_button").hide();
  $("#inputparams-panel").hide();
  $("#trends-panel").hide();
  $("#env4app-panel").hide();
  $("#app4comp-panel").hide();
  $("#planningtimeline").hide();
  $("#attrs-panel").hide();
  $("#attrs10-panel").hide();
  $("#logs-panel").hide();
  $("#steps-panel").hide();
  $("#srv4comp-panel").hide();
  $("#compowner_summ_data_edit").hide();
  $("#compowner_summ_data").hide();
  $("#compitem_data").hide();
  $("#compitem_data_edit").hide();
  $("#emailbody_data").hide();
  $("#emailbody_data_edit").hide();
  $("#props_data").hide();
  $("#props_data_edit").hide();
  $("#emailto-panel").hide();
  $("#emailhelp-panel").hide();
  $("#adminrights_data").hide();
  $("#adminrights_data_edit").hide();
  $("#groupmembership-panel").hide();
  $("#usermembership-panel").hide();
  $("#outerworkflow").hide();
  $("#pickproc").hide();
  $("#switchargs-panel").hide();
  $("#feedback-panel").hide();
  $("#procbody-panel").hide();
  $("#comp4srv-panel").hide();
  $("#cmdline-panel").hide();
  $("#servstat_data").hide();
  $("#app4env-panel").hide();
  $("#srv4env-panel").hide();
  $("#defect-panel").hide();
  $("#defect-panel-20").hide();
  $("#deployedenv4app-panel").hide();
  $("#planning-panel").hide();
  $("#task-panel").hide();
  $("#tabs-General-right").hide();
  $("#row-10-right-panel").hide();
  $("#row-10b-access").hide();
  $("#tabs-General-row-9").hide();
  $("#tabs-General-row-9a").hide();
  $("#tabs-General-row-9b").hide();
  $("#tabs-General-row-9c").hide();
  $("#tabs-General-right-15").hide();
  $("#tabs-General-right-20").hide();
  $("#tabs-General-row-lic").hide();
  $("#tabs-General-row-12").hide();
  $("#tabs-General-row-12-0").hide();
  $("#tabs-General-row-12a").hide();
  $("#tabs-General-row-12b").hide();
  $("#licensemd-panel").hide();
  $("#tabs-Calendar-data").hide();
  $("#cr-panel-20").hide();
  $("#tabs-General-row-14").hide();
  $("#tabs-General-row-15").hide();
  $("#tabs-General-row-15a").hide();
  $("#appcomp-data").hide();
  $("#tabs-General-row-20").hide();
  $("#tabs-General-row-25").hide();
  $("#tabs-General-row-28").hide();
  $("#tabs-General-row-30").hide();
  $("#tabs-General-row-32").hide();
  $("#tabs-General-row-35").hide();
  $("#tabs-General-row-35a").hide();
  $("#tabs-General-row-40").hide();
  $("#tabs-General-row-50").hide();
  $("#tabs-General-row-51").hide();
  $("#tabs-General-row-scorecard").hide();
  $("#tabs-General-row-lic").hide();
 }

  FetchSummary(tablename, objtypeAsInt, objtype, objid, addParams);

  if (objtype == "ap" || objtype == "av")
  {
   $("#devops-details").html("Application with DevOps Details");
   $("#bookmark-DevOps-Details > u").html("Application with DevOps Details");
   $("#trends-panel").show();
   $("#logs-panel").show();
   $("#attrs-panel").show();
   $("#tabs-General-right").show();
   $("#tabs-General-row-9").show();
   $("#tabs-General-row-12a").show();
   $("#tabs-General-row-12b").show();
   $("#tabs-General-row-14").show();
   $("#tabs-General-row-15").show();
   $("#tabs-General-row-15a").show();
   $("#tabs-General-right-15").show();
   $("#tabs-General-row-20").show();
   $("#tabs-General-row-25").show();
   $("#feedback-panel").show();
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#tabs-General-row-40").show();
 //  $("#defect-panel").show();
   AppMap();
   LoadAttributesData("attrib",objtypeAsInt, objtype, objid);

   $("#appcomp-data").show();
   $("#row-10-right-panel > #summ_title > h2").html("Dependencies");
   getAppCompList("list");

   LoadAppLogList("loghist",objtypeAsInt, objtype, objid);
   LoadDeployedEnv4AppData("deployedenv4app", objtypeAsInt, objtype, objid, addParams);
   LoadEnv4AppData("env4app", objtypeAsInt, objtype, objid, "");
   getLicenseList("list");
   getCVEList("list");

   if (objtype == "av")
    $("#deployedenv4app-panel").show();
   else
   {
    $("#deployedenv4app-panel").show();
    $("#env4app-panel").show();
   }

   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");

   $("#audit35-panel").show();
   createNewsFeed("audit35", objtype, objid, false);

   hookSummary(tablename, objtypeAsInt, objtype, objid, addParams);
  }
  else if (objtype == "co" || objtype == "cv")
  {
   $("#devops-details").html("Component with DevOps Details");
   $("#bookmark-DevOps-Details > u").html("Component with DevOps Details");

   $("#app4comp-panel").show();
   $("#srv4comp-panel").show();
   $("#attrs-panel").show();
   $("#tabs-General-right").show();
   $("#tabs-CompItems-left").show();
   $("#compowner_summ_data").show();
   $("#compitem_data").show();
   $("#row-10-right-panel").show();
   $("#tabs-General-row-lic").show();
   $("#tabs-General-row-12").show();
   $("#tabs-General-row-12-0").show();
   $("#tabs-General-row-12a").show();
   $("#tabs-General-row-12b").show();
   $("#tabs-General-row-scorecard").show();
   $("#licensemd-panel").show();
   $("#cr-panel-20").show();
   $("#tabs-General-row-14").show();
   $("#tabs-General-row-15").show();
   $("#tabs-General-row-15a").show();
   $("#tabs-General-right-15").show();
 //  $("#feedback-panel").show();      // SBT remove for components 11/4/21
 //  $("#tabs-General-row-28").show(); // SBT remove for components 11/4/21
   $("#tabs-General-row-30").show();
   $("#tabs-General-row-32").show();
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#tabs-General-row-40").show();
 //  $("#defect-panel").show();

   LoadReadme(objid);
   LoadSwagger(objid);
   LoadLicense(objid);
   LoadComponentItems(objid);
   CompMap();
   LoadAttributesData("attrib",objtypeAsInt, objtype, objid);
   getApps4Comp("list");
 //  LoadSrv4CompData("srv4comp", objtypeAsInt, objtype, objid, "");
   getLicenseList("list");
   getCVEList("list");

   CreateCurrentEnv2Comps(objid);

   getProvidesList("list");
   getConsumesList("list");
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");

   $("#audit35-panel").show();
   createNewsFeed("audit35", objtype, objid, false);

   hookSummary(tablename, objtypeAsInt, objtype, objid, addParams);

  }
  else if (objtype == "te")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#row-10-right-panel").show();
   $("#emailto-panel").show();
   $("#emailhelp-panel").show();
   LoadBodyData("emailbody", objtypeAsInt, objtype, objid, addParams);
   LoadEmailToData("emailto", objtypeAsInt, objtype, objid, "");
  }
  else if (objtype == "no")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");

   $("#props_data").show();
   $("#props_data_edit").hide();
   LoadPropertiesData("propstab", objtypeAsInt, objtype, objid, "");
  }
  else if (objtype == "ct")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
  }
  else if (objtype == "cr")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);
  }
  else if (objtype == "ac")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);

   $("#tabs-General-row-14").show();
   $("#tabs-General-row-15").show();
   $("#tabs-General-row-15a").show();
   $("#outerworkflow").show();
   $("#pickproc").show();
   $("#cmdline-panel").show();
   LoadWorkflowsData("");
  }
  else if (objtype == "us")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#groupmembership-panel").show();
   LoadGroupMembershipData("groupmembership", objtypeAsInt, objtype, objid, "");

   $("#row-10-right-panel").show();
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);
  }
  else if (objtype == "gr")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#usermembership-panel").show();
   LoadUserMembershipData("usermembership", objtypeAsInt, objtype, objid, "");
   LoadAdminRightsData(objtypeAsInt, objtype, objid);

   $("#row-10-right-panel").show();
   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);
  }
  else if (objtype == "ds")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#props_data").show();
   $("#props_data_edit").hide();
   LoadPropertiesData("propstab", objtypeAsInt, objtype, objid, "");


   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);
  }
  else if (objtype == "re")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#props_data").show();
   $("#props_data_edit").hide();
   LoadPropertiesData("propstab", objtypeAsInt, objtype, objid, "");

   $("#tabs-General-right-15").show();


   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);
  }
  else if (objtype == "rl" || objtype == "rv")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#trends-panel").show();
   $("#logs-panel").show();
   $("#row-10-right-panel").show();
   $("#tabs-General-row-20").show();

   $("#attrs10-panel").show();
   LoadAttributesData("attrib10",objtypeAsInt, objtype, objid);

   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);

   hookSummary(tablename, objtypeAsInt, objtype, objid, addParams);

   $("#planning-panel").show();
   $("#planningtimeline").show();
   LoadTimeline4App("planningtimeline", objtypeAsInt, objtype, objid);
  }
  else if (objtype == "en")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");

   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   $("#tabs-Calendar-data").show();
   CreateCalendar(objid);
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);

   $("#app4env-panel").show();
   LoadApp4EnvData("app4env", objtypeAsInt, objtype, objid, addParams);
   $("#row-10-right-panel").show();

   $("#tabs-General-row-15").show();
   $("#tabs-General-row-15a").show();
   $("#attrs-panel").show();
   LoadAttributesData("attrib",objtypeAsInt, objtype, objid);

   $("#srv4env-panel").show();
   LoadSrv4EnvData("srv4env",objtypeAsInt, objtype, objid);

   $("#tabs-General-row-20").show();

   $("#tabs-General-row-28").show();
   $("#feedback-panel").show();
   CreateCurrentComp2Envs(objid);

   hookSummary(tablename, objtypeAsInt, objtype, objid, addParams);

  // $("#tabs-General-row-30").show();
  // $("#comp4srv-panel").show();
  // LoadComp4SrvData("comp4srv", objtypeAsInt, objtype, objid, "");
  }
  else if (objtype == "pr" || objtype == "fn")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   $("#tabs-General-row-14").show();
   $("#tabs-General-row-15").show();
   $("#tabs-General-row-15a").show();
   $("#inputparams-panel").show();
   $("#tabs-General-row-20").show();

   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);

   $("#switchargs-panel").show();
   $("#inputparams-panel").show();
   $("#cmdline-panel").show();
   $("#tabs-General-row-9a").show();
   $("#tabs-General-row-9b").show();
   $("#tabs-General-row-9c").show();
   LoadArgsData("inputparams", objtypeAsInt, objtype, objid, objkind, addParams);
  }
  else if (objtype == "se")
  {
   $("#devops-details").html("Details");
   $("#bookmark-DevOps-Details > u").html("Details");
   if (!hideOption())
   {
    $("button.test_button").show();
    $("#servstat_data").show();
    LoadServerStatusData("servstat", objtypeAsInt,objid);
   }

   $("#attrs10-panel").show();
   LoadAttributesData("attrib10",objtypeAsInt, objtype, objid);

   $("#tabs-General-row-35").show();
   $("#tabs-General-row-35a").show();
   $("#row-35-access").show();
   LoadAccess("row-35-access-data", objtypeAsInt, objtype, objid, "");
   createNewsFeed("audit35", objtype, objid, false);

   $("#row-10-right-panel").show();

   $("#tabs-General-row-20").show();

   $("#comp4srv-panel").show();
   LoadComp4SrvData("comp4srv", objtypeAsInt, objtype, objid, "");
  }
  else if (objtype == "do" )
  {
   $("#tabs-General-row-10").show();
   $("#domnav-panel").show();
   DomNav();

   $("#row-10b-access").show();
   LoadAccess("row-10b-access-data", objtypeAsInt, objtype, objid, "");

   if (!hideOption())
   {
   	$("#tabs-General-row-50").show();
    $("#tabs-General-row-51").show();
    $("#task-panel").show();
    LoadTasks("taskstab", objtypeAsInt, objtype, objid, "");
   }
  }
}

function LoadAccess(parentdiv, objtypeAsInt, objtype, objid, addParams)
{
 var url = "GetAccess?objtype=" + objtypeAsInt + "&id=" + objid + "&din=";

 if (objtype == "do")
  url = "GetAccess?objtype=" + objtypeAsInt + "&id=" + objid + "&din=Y";


 console.log(url);

  $.ajax(
    {
     url : url,
     type : 'GET',
     success : function(res)
     {
	  $("#" + parentdiv).html(res);
     }
   });
}

function LoadComponentItems(objid)
{
 if (objid < 0)
 {
  if (addcomptype == "database")
  {
   ciClickElement(objid,"rf_database");
   ciClickElement(objid,"rb_database");
  }
  else
   ciClickElement(objid,addcomptype);
  return;
 }

 $.getJSON('GetComponentItemLayout', "compid=" + objid, function(data)
   {
    for (var i=0;i<data.Nodes.length;i++)
      ciClickElement(data.Nodes[i].nodeid, "");
   });
}


function LoadLicense(objid)
{
    $.ajax(
    {
     url : "/msapi/textfile?compid=" + objid + "&filetype=license",
     type : 'GET',
     success : function(res)
     {
	  var converter = new showdown.Converter({openLinksInNewWindow: true});
	  converter.setFlavor('github');
      html      = converter.makeHtml(res);
      $("#licensemd-data").html(html);
     }
   });
}

function LoadReadme(objid)
{

    $.ajax(
    {
     url : "/msapi/textfile?compid=" + objid + "&filetype=readme",
     type : 'GET',
     success : function(res)
     {
	  var converter = new showdown.Converter({openLinksInNewWindow: true});
	  converter.setFlavor('github');
      html      = converter.makeHtml(res);
      $("#readme-data").html(html);
     }
   });
}


function LoadSwagger(objid)
{
  const ui = SwaggerUIBundle({
    url: "/msapi/textfile?compid=" + objid + "&filetype=swagger",
    dom_id: '#swagger-data',
    deepLinking: true,
    presets: [
      SwaggerUIBundle.presets.apis,
      SwaggerUIStandalonePreset
    ],
    plugins: [
      SwaggerUIBundle.plugins.DownloadUrl
    ],
    layout: "StandaloneLayout"
  });
}

function HideFields()
{
	console.log("HideFields - summSaveobjtype="+summSaveobjtype+" summProtocol="+summProtocol);
	if (summSaveobjtype == "se") {
		if (summAutoPing) {
			// Show auto fields if auto ping is on
			$("#automd5check_sumrow").show();
			$("#pinginterval_sumrow").show();
			$("#pingstart_sumrow").show();
			$("#pingend_sumrow").show();
			$("#pingfailuretemplate_sumrow").show();
			$("#md5mismatchtemplate_sumrow").show();
		} else {
			// Hide auto fields if auto ping is off
			$("#automd5check_sumrow").hide();
			$("#pinginterval_sumrow").hide();
			$("#pingstart_sumrow").hide();
			$("#pingend_sumrow").hide();
			$("#pingfailuretemplate_sumrow").hide();
			$("#md5mismatchtemplate_sumrow").hide();
		}
		if (summProtocol == "sftp") {
			$("#sshportnumber_sumrow").show();
		} else {
			$("#sshportnumber_sumrow").hide();
		}
	}
	if (summSaveobjtype == "us") {
		if (summSaveVDS != "") {
			$("#accountlocked_sumrow").hide();
			$("#forcechangepassword_sumrow").hide();
		} else {
			$("#accountlocked_sumrow").show();
			$("#forcechangepassword_sumrow").show();
		}
	}
	if (summSaveobjtype != "ac" && summSaveobjtype != "pr" && summSaveobjtype != "fn") {
		return;
	}

	console.log("save_kind_id="+save_kind_id);

 switch (save_kind_id)
 {
 case 1: // DMScript Function in repository
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").show();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").hide();
  $("#allocateterminal_sumrow").hide();
  $("#atfootnote").hide();
  $("#commandlineinterpreter_sumrow").hide();
  break;

 case 2: // DMScript Function in database
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  $("#allocateterminal_sumrow").hide();
  $("#atfootnote").hide();
  $("#commandlineinterpreter_sumrow").hide();
  break;

 case 3: // Function provided by local external script or program
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").hide();
  $("#allocateterminal_sumrow").hide();
  $("#atfootnote").hide();
  $("#commandlineinterpreter_sumrow").hide();
  break;

 case 4: // Function provided by remote external script or program
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").show();
  $("#allocateterminal_sumrow").show();
  $("#atfootnote").show();
  $("#commandlineinterpreter_sumrow").show();
  break;

 case 5: // Function provided by plugin
  $("#fragmentname_sumrow").show();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  $("#allocateterminal_sumrow").hide();
  $("#atfootnote").hide();
  $("#commandlineinterpreter_sumrow").hide();
  break;

 case 6: // Graphical deployment flow
  $('#displayname_sumrow').hide();	// For now - cannot use Graphical Action Flows in other actions (yet)
  $("#category_sumrow").show();		// For now - cannot use Graphical Action Flows in other actions (yet)
  $('#kind_sumrow').hide();			// Always Graphical Action Flow
  $("#fragmentname_sumrow").show();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  $("#allocateterminal_sumrow").hide();
  $("#atfootnote").hide();
  $("#commandlineinterpreter_sumrow").hide();
  break;

 default:
  break;
 }

}

function endsWith(str, suffix) {
	// if (typeof str != "undefined")
    return str?str.indexOf(suffix, str.length - suffix.length) !== -1:false;
}

function summOK(isProfile, prefix)
{
 event.preventDefault();

 $('.edit_button').css("color","#3367d6");

 var dom_summSavetablename = summSavetablename;
 var dom_summSaveobjtypeAsInt = summSaveobjtypeAsInt;
 var dom_summSaveobjtype = summSaveobjtype;
 var dom_summSaveobjid = summSaveobjid;
 var dom_summSaveaddParams = summSaveaddParams;

 if (prefix != "")
 {
  summSavetablename = task_summSavetablename;
  summSaveobjtypeAsInt = task_summSaveobjtypeAsInt;
  summSaveobjtype = task_summSaveobjtype;
  summSaveobjid = task_summSaveobjid;
  summSaveaddParams = task_summSaveaddParams;
 }

 var pwd = parent.$("#" + prefix + "summ_data_edit");
 var form = pwd.find("#" + prefix + "summform");

 if (typeof summSavedomid == "undefined")
  summSavedomid = domlist[0];

 if (typeof parentid == "undefined" || parentid == "")
  parentid = domlist[0];

 if (typeof parenttype == "undefined" || parenttype == "")
  parenttype = "Domain";

 console.log("EditSummaryButton isProfile="+isProfile);

  console.log("OK Clicked: summSaveobjtype="+summSaveobjtype+" summSaveobjid="+summSaveobjid);
  var name = "";

  if (form.find(":input[name=\"password_again_val\"]").length > 0)
  {
   var pw1 = form.find(":input[name=\"password_val\"]").val();
   var pw2 = form.find(":input[name=\"password_again_val\"]").val();

   if (pw1 != pw2)
   {
    alert("Passwords don't match.");
    return;
   }
  }

  if (form.find(":input[name=\"fulldomain_val\"]").length > 0)
    parentid = form.find(":input[name=\"fulldomain_val\"]").val();

  if (summSaveobjid < 0)
  {
   var namefld;

   if (summSaveobjtype == "us")
    namefld = pwd.find(":input[name=\"username_val\"]");
   else if (summSaveobjtype == "gr")
    namefld = pwd.find(":input[name=\"groupname_val\"]");
   else
    namefld = pwd.find(":input[name=\"name_val\"]");

    if (typeof namefld !== undefined)
     name = namefld.val();

    if (name == null)
     return;

    if (name.trim() == "")
     return;

   var f = pwd.find("#" + prefix + "summform");
   var catid;
   var kindid=null;
   switch (summSaveobjtype) {
   case "pr":
   case "fn":
    var kind = f.find(":input[name=\"kind_val\"]");
    kindid=kind.val();
    // Deliberate drop-through
   case "ac":
   case "co":
       var cat = f.find(":input[name=\"category_val\"]");
       catid = cat2id[cat.val()];
       console.log("catid="+catid);
       break;
   default:
    break;
   }
   switch (summSaveobjtype)
   {
   case "pr":
    NewID("Action", "PROCEDURE", currenttree, "P", name, catid, kindid);
    break;

   case "fn":
    NewID("Action", "FUNCTION", currenttree, "F", name, catid, kindid);
    break;

   case "ac":
    NewID("Action", "ACTION", currenttree, "G", name, catid);
    break;

   case "rl":
    NewID("Release", "RELEASE", currenttree, "", name);
    break;

   case "no":
    NewID("Notify", "NOTIFY", currenttree, "", name);
    break;

   case "te":
    {
     notifierfld = pwd.find(":input[name=\"notifier_val\"]");

     if (typeof notifierfld  !== undefined)
     {
      if (notifierfld.val() != null)
      {
       parentid = notifierfld.val().substring(2);
       parenttype = "NOTIFY";
      }
     }
     if (notifierfld.val() != null)
        NewID("Template","TEMPLATE", currenttree, "", name);
    }
    break;

   case "ds":
    NewID("Datasource", "DATASOURCE", currenttree, "", name);
    break;

   case "ap":
    NewID("Application", "APPLICATION", currenttree, "", name);
    break;

   case "re":
    NewID("Repository", "REPOSITORY", currenttree, "", name);
    break;

   case "cr":
    NewID("Credentials", "CREDENTIALS", currenttree, "", name);
    break;

   case "se":
    NewID("Server", "Server", currenttree, "", name);
    break;

   case "co":
    NewID("Component", "COMPONENT", currenttree, "", name,catid);
    break;

   case "en":
    NewID("Environment", "ENVIRONMENT", currenttree, "", name);
    break;

   case "ct":
    NewID("Type", "SERVERCOMPTYPE", currenttree, "", name);
    break;

   case "us":
   {
    namefld = pwd.find(":input[name=\"username_val\"]");

    if (typeof namefld !== undefined)
     name = namefld.val();

    NewID("User", "USER", currenttree, "", name);
   }
    break;

   case "gr":
   {
    namefld = pwd.find(":input[name=\"groupname_val\"]");

    if (typeof namefld !== undefined)
     name = namefld.val();
    NewID("Usergroup", "USERGROUP", currenttree, "", name);
   }
   break;

   case "be":
    NewID("buildengine", "BUILDENGINE", currenttree, "", name);
    break;

   case "bj":
    {
     builderfld = pwd.find(":input[name=\"builder_val\"]");

     if (typeof builderfld !== undefined)
     {
      parentid = builderfld.val().substring(2);
      parenttype = "BUILDER";
     }
     NewID("buildjob", "BUILDJOB", currenttree, "", name);
    }
    break;

   default:
    break;
   }
  }
  if (isProfile) {
   SaveSummaryData(parent.$("#" + prefix + "summ_data_edit"), profSavetablename, profSaveobjtypeAsInt, profSaveobjtype, profSaveobjid, profSaveaddParams,true);
  } else {
   if (summSaveobjid != -1)
    SaveSummaryData(parent.$("#" + prefix + "summ_data_edit"), prefix + "summ", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams,false);
  }

  summSavetablename = dom_summSavetablename;
  summSaveobjtypeAsInt = dom_summSaveobjtypeAsInt;
  summSaveobjtype = dom_summSaveobjtype;
  summSaveobjid = dom_summSaveobjid;
  summSaveaddParams = dom_summSaveaddParams;
}

function summCancel(prefix)
{
 // event.preventDefault();
 var pwd = parent.$("#" + prefix + "summ_data_edit");

 if (summSaveobjid < 0)
   $("#panel_container.right").hide();

 $('.edit_button').css("color","#3367d6");
 $('.save_button').css("color","grey");
 $('.cancel_button').css("color","grey");
 pwd.hide();
 parent.$("#" + prefix + "summ_data").show();

 if (objtype == "cv" || objtype == "co")
 {
  parent.$("#compowner_summ_data_edit").hide();
  parent.$("#compowner_summ_data").show();

  parent.$("#compitem_data_edit").hide();
  parent.$("#compitem_data").show();
 }
 else if (objtype == "te")
 {
  parent.$("#emailbody_data_edit").hide();
  parent.$("#emailbody_data").show();
 }
 else if (objtype == "no")
 {
  $("#props_data_edit").hide();
  $("#props_data").show();
 }
 else if (objtype == "gr")
 {
  $("#adminrights_data_edit").hide();
  $("#adminrights_data").show();
 }
 else if (objtype == "re")
 {
  $("#props_data_edit").hide();
  $("#props_data").show();
 }
 else if (objtype == "be")
 {
  $("#props_data_edit").hide();
  $("#props_data").show();
 }

 if (objid < 0)
 {
  var listName = tree2menu[currenttree];
  openList(null,listName);
 }
}

function EditSummaryButton(userDialog, prefix)
{

 console.log($('.edit_button').css("color"));

 $("#engine_details_row_edit").hide();

 if ($('.edit_button').css("color") == "rgb(128, 128, 128)")
   return;

 $('.edit_button').css("color","grey");
 $('.save_button').css("color","#3367d6");
 $('.cancel_button').css("color","#3367d6");


 isProfile = userDialog;

 if (typeof summSavedomid == "undefined")
  summSavedomid = domlist[0];

 if (typeof parentid == "undefined" || parentid == "")
  parentid = domlist[0];

 if (typeof parenttype == "undefined" || parenttype == "")
  parenttype = "Domain";

	console.log("EditSummaryButton isProfile="+isProfile);
	var pwd = parent.$("#" + prefix + "summ_data_edit");

	if (prefix != "")
	 pwd.html(task_tdedit);
	else
  pwd.html(isProfile?tdedit2:tdedit);

 $("#" + prefix + "summ tr").each(function( index ) {
	   console.log($(this).attr("id"));
	   if (typeof $(this).attr("id") != "undefined" && $(this).attr("id").indexOf("/") < 0)
		   if (endsWith($(this).attr("id"),"_sumrow")) {
			   if($(this).is(":visible")) {
			   } else {
				   var ef = $(this).attr("id").replace("_sumrow","_row");
				   console.log("Hiding "+ef);
				   $("#"+ef).hide();
				   if (ef=="allocateterminal_row") {
					   console.log("Hiding atfootnote");
					   $("#atfootnote").hide();
				   }
				   if (ef=="displayname_row") {
					   console.log("Hiding fragmentname_row");
					   $("#fragmentname_row").hide();
				   }
			   }
		  }
	  });
	  ProtocolChanged();
   if (isLifeCycle)
   {
    var sdl = pwd.find("#subdomainlist");
    var cnt = sdl.children().length;
   }
   else
   {
    pwd.find("#dndmsg").hide();

    var sdl = pwd.find("#subdomainlist");
    var cnt = sdl.children().length;
   }

   var sdl = pwd.find("#subdomainlist");
   if (sdl.length > 0 && isLifeCycle)
   {
    sdl.sortable(
    {
     axis : "y",
     cursor : "move",
     stop : function(event, ui)
     {
     }
    });
   }

   var myform = pwd.find("#" + prefix + "summform");

   if (myform.find(":input[name=\"customaction_val\"]").length > 0)
   {
	   console.log("About to GetActions");
    $.ajax(
    {
     url : "GetActions?reason=dropdown&objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      console.log(res);
      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#" + prefix + "summform");
      var preaction = myform.find(":input[name=\"preaction_val\"]");
      var postaction = myform.find(":input[name=\"postaction_val\"]");
      var customaction = myform.find(":input[name=\"customaction_val\"]");
      var taskaction = myform.find(":input[name=\"taskaction_val\"]");

	  var action_list = [];
      var actionids = {};

      for (n = 0; n < res.length; n++)
      {
       var action = res[n];

       action.type = action.id.substring(0, 2);
       action.id = action.id.substring(2);

       if (action.type == "ac" && !(action.id in actionids))
       {
        action_list.push(action);
        actionids[action.id] = action;
       }
      }

	  action_list = action_list.sort(function(a, b) {
 		 var nameA = a.name.toUpperCase(); // ignore upper and lowercase
  		 var nameB = b.name.toUpperCase(); // ignore upper and lowercase
         if (nameA < nameB) {return -1;}
         if (nameA > nameB) {return 1;}
         return 0;
      });

      for (action of action_list)
      {
    	  if (typeof preaction != "undefined")
    	  {
        if (save_preaction_val == action.id)
        preaction.append('<option id="preact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
       else
        preaction.append('<option id="preact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');
    	  }

    	  if (typeof postaction != "undefined")
    	  {
        if (save_postaction_val == action.id)
         postaction.append('<option id="postact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
        else
         postaction.append('<option id="postact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');
    	  }

    	  if (typeof customaction != "undefined")
    	  {
        if (save_customaction_val == action.id)
         customaction.append('<option id="customact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
        else
         customaction.append('<option id="customact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');
    	  }

    	  if (typeof taskaction != "undefined")
    	  {
        if (save_taskaction_val == action.id)
          taskaction.append('<option id="customact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
        else
          taskaction.append('<option id="customact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');
    	  }
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     },
     complete : function(jqxhr, status, err)
     {
    	 console.log("complete");
   // 	 parent.$("#summ_data_edit").dialog({"width":"auto"});
     }
    });
   }

   if (myform.find(":input[name=\"fulldomain_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetFullDomains",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var domain = myform.find(":input[name=\"fulldomain_val\"]");
      var cnt = 0;

      for (n = 0; n < res.length; n++)
      {
       if (!res[n].name.startsWith(userdom))
        continue;

       if (save_fulldomain_val == res[n].name)
       {
        save_fulldomain_id = res[n].id;
        domain.append('<option id="domain' + n + '" selected value=\"' + save_fulldomain_id + "\">" + res[n].name + '</option>');

        cnt++;
       }
       else
        domain.append('<option id="domain' + n + '" value=\"' + res[n].id + "\">" + res[n].name + '</option>');
      }

      if (cnt == 0)
       domain.prop('selectedIndex', 0);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"comptype_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetComponentData?comptype=y",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var comptype = myform.find(":input[name=\"comptype_val\"]");
      var cnt = 0;

      for (n = 0; n < res.data.length; n++)
      {
       if (save_comptype_val == res.data[n].name)
       {
    	save_comptype_id = res.data[n].database + res.data[n].deletedir + res.data[n].id;
        comptype.append('<option id="comptype' + n + '" selected value=\"' + save_comptype_id + "\">" + res.data[n].name + '</option>');

        cnt++;
       }
       else
        comptype.append('<option id="comptype' + n + '" value=\"' + res.data[n].database + res.data[n].deletedir  + res.data[n].id + "\">" + res.data[n].name + '</option>');
      }

      if (cnt == 0)
      {
       if (save_kind_val == "Database")
        $("#comptype_val option:contains(Postgres)").attr('selected', true);
       else if (save_kind_val == "Container")
        $("#comptype_val option:contains(Kubernetes)").attr('selected', true);
       else
        $("#comptype_val option:contains(Application Server)").attr('selected', true);
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"servercomptype_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetComponentData?comptype=y",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var comptype = myform.find(":input[name=\"servercomptype_val\"]");
      var cnt = 0;
      var parts = save_servercomptype_val.split(";");
      console.log("PARTS");
      console.log(parts);
      save_comptype_id = "";
      for (n = 0; n < res.data.length; n++)
      {
       if (parts.indexOf(res.data[n].id.toString()) >= 0)
       {
    	save_comptype_id = save_comptype_id + res.data[n].id + ";";
        comptype.append('<option id="servercomptype' + n + '" selected value=\"' + res.data[n].id + "\">" + res.data[n].name + '</option>');
        cnt++;
       }
       else
        comptype.append('<option id="servercomptype' + n + '" value=\"' + res.data[n].id + "\">" + res.data[n].name + '</option>');
      }

      if (cnt == 0)
       comptype.prop('selectedIndex', 0);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"autoping_val\"]").length > 0)
   {
     $("#autoping_val").change(ToggleAutoPing);
     ToggleAutoPing();
   }

   if (myform.find(":input[name=\"datasource_val\"]").length > 0)
   {
	   $.ajax({
		   url : "GetProviderDefs?f=ldap&id="+summSavedomid,
		   dataType : 'json',
		   type : 'GET',
		   success : function(res)
		   {
			   var myform = pwd.find("#" + prefix + "summform");
			   var datasource = myform.find(":input[name=\"datasource_val\"]");
			   var cnt = 0;

			   for (n = 0; n < res.length; n++)
			   {
				   if (save_datasource_val == res[n].name)
				   {
					   datasource.append('<option id="datasource' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
					   cnt++;
				   }
				   else
					   datasource.append('<option id="datasource' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
			   }
			   if (cnt == 0) datasource.prop('selectedIndex', 0);
			   TogglePasswordFields();
		   },
		   error : function(jqxhr, status, err)
		   {
			   console.log(status);
			   console.log(err);
		   }
		});
   }

   if (myform.find(":input[name=\"notifier_val\"]").length > 0)
   {
    $.ajax({
     url : "GetNotifiers",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      if (typeof res.error != "undefined") {
       alert("Cannot retrieve Notifier List: "+res.error);
      } else {
       var myform = pwd.find("#" + prefix + "summform");
       var notifier = myform.find(":input[name=\"notifier_val\"]");
       var cnt = 0;

       for (n = 0; n < res.length; n++)
       {
        fullname = res[n].domain + "." + res[n].name;

        if (save_notifier_val == fullname)
        {
         notifier.append('<option id="notifier' + n + '" selected value="' + res[n].id + "\">" + fullname + '</option>');
         cnt++;
        }
        else
         notifier.append('<option id="notifier' + n + '" value="' + res[n].id + "\">" + fullname + '</option>');
       }
       if (cnt == 0) notifier.prop('selectedIndex', 0);
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
  });
   }

   if (myform.find(":input[name=\"builder_val\"]").length > 0)
   {
    $.ajax({
     url : "GetBuildEngines",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      if (typeof res.error != "undefined") {
       alert("Cannot retrieve builder List: "+res.error);
      } else {
       var myform = pwd.find("#" + prefix + "summform");
       var builder = myform.find(":input[name=\"builder_val\"]");
       var cnt = 0;

       for (n = 0; n < res.length; n++)
       {
        fullname = res[n].domain + "." + res[n].name;

        if (save_builder_val == fullname)
        {
         builder.append('<option id="builder' + n + '" selected value="' + res[n].id + "\">" + fullname + '</option>');
         cnt++;
        }
        else
         builder.append('<option id="builder' + n + '" value="' + res[n].id + "\">" + fullname + '</option>');
       }
       if (cnt == 0) builder.prop('selectedIndex', 0);
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
  });
   }

   BuilderChanged();

   if (myform.find(":input[name=\"bt_datasource_val\"]").length > 0)
   {
	   $.ajax({
		   url : "GetProviderDefs?f=bt&id="+summSavedomid,
		   dataType : 'json',
		   type : 'GET',
		   success : function(res)
		   {
			   var myform = pwd.find("#" + prefix + "summform");
			   var datasource = myform.find(":input[name=\"bt_datasource_val\"]");
			   var cnt = 0;

			   for (n = 0; n < res.length; n++)
			   {
				   if (save_datasource_val == res[n].name)
				   {
					   datasource.append('<option id="datasource' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
					   cnt++;
				   }
				   else
					   datasource.append('<option id="datasource' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
			   }
			   if (cnt == 0) datasource.prop('selectedIndex', 0);
			   TogglePasswordFields();
		   },
		   error : function(jqxhr, status, err)
		   {
			   console.log(status);
			   console.log(err);
		   }
		});
   }
   if (myform.find(":input[name=\"buildjob_val\"]").length > 0)
   {
	   console.log("GetBuildJobs?compid="+summSaveobjid+"&domid="+summSavedomid);
	   console.log("")
	   $.ajax({
		   url : "GetBuildJobs?compid="+summSaveobjid+"&domid="+summSavedomid,
		   dataType : 'json',
		   type : 'GET',
		   success : function(res)
		   {
			   console.log(res);
			   var myform = pwd.find("#" + prefix + "summform");
			   var datasource = myform.find(":input[name=\"buildjob_val\"]");
			   var cnt = 0;

			   for (n = 0; n < res.length; n++)
			   {
				   if (save_buildjob_val == res[n].name)
				   {
					   datasource.append('<option id="buildjob' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
					   cnt++;
				   }
				   else
					   datasource.append('<option id="buildjob' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
			   }
			   if (cnt == 0) datasource.prop('selectedIndex', 0);
		   },
		   error : function(jqxhr, status, err)
		   {
			   console.log(status);
			   console.log(err);
		   }
		});
   }
   if (myform.find(":input[name=\"template_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetTemplates?objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var template = myform.find(":input[name=\"template_val\"]");
      var fail_template = myform.find(":input[name=\"fail_template_val\"]");
      var cnt = 0;

      for (n = 0; n < res.length; n++)
      {
       if (save_template_val == res[n].name)
       {
        template.append('<option id="template' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
        cnt++;
       }
       else
        template.append('<option id="template' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

       if (save_fail_template_val == res[n].name)
        fail_template.append('<option id="fail_template' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        fail_template.append('<option id="fail_template' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

      }

      //if (cnt == 0)
      // template.prop('selectedIndex', 1);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"owner_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetUsers",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#" + prefix + "summform");
      var owner = myform.find(":input[name=\"owner_val\"]");

      var userid = save_owner_val;

      if (userid == "")
       userid = myuserid;

      for (n = 0; n < res.length; n++)
      {
       if (userid == res[n].name)
        owner.append('<option id="owner' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="owner' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }

      $.ajax(
        {
         url : "GetGroups",
         dataType : 'json',
         type : 'GET',
         success : function(res)
         {

          // res =
          // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
          var myform = pwd.find("#" + prefix + "summform");
          var owner = myform.find(":input[name=\"groupowner_val\"]");

          var group = save_groupowner_val;

          for (n = 0; n < res.length; n++)
          {
           if (group == res[n].name)
            owner.append('<option id="groupowner' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
           else
            owner.append('<option id="groupowner' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
          }
          $("#ownertype_val").change(ToggleOwner);

          ToggleOwner();
         },
         error : function(jqxhr, status, err)
         {
          console.log(status);
          console.log(err);
         }
        });
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"credential_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetCredentials",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var owner = myform.find(":input[name=\"credential_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_credential_val == res[n].name)
        owner.append('<option id="credential' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="credential' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"category_val\"]").length > 0)
   {
    var cat = myform.find(":input[name=\"category_val\"]");

    $.ajax(
    {
     url : "GetCategories?reason=action",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var selections = "";

      console.log(res);
      cat2id = [];
      for (n = 1; n < res.length; n++)
      {
       selections += res[n].name + ";";
       cat2id[res[n].name] = res[n].id;
      }
      cat.attr('selectBoxOptions', selections);
      createEditableSelect(cat[0]);
      window.addEventListener("selectBox", ToggleDisplayName, false);
      cat.val(save_category_val);
//      if (save_category_val == "No category" || save_category_val == "Do not display")
//       $("#fragmentname_row").hide();

     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });

   }

   if (myform.find(":input[name=\"type_val\"]").length > 0)
   {
    var t = obj2Int[summSaveobjtype][0];

    $.ajax(
    {
     url : 'GetProviderDefs',
     data :
     {
      objtype : t
     },
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var typeobj = myform.find(":input[name=\"type_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_type_val.toLowerCase() == res[n].name.toLowerCase())
        typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        typeobj.append('<option id="type' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"domain_val\"]").length > 0)
   {

    $.ajax(
    {
     url : 'GetAllDomains',
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var myform = pwd.find("#" + prefix + "summform");
      var typeobj = myform.find(":input[name=\"domain_val\"]");

      for (n = 0; n < res.length; n++)
      {
 console.log("res["+n+"].name="+res[n].name);
 console.log("save_domain_val="+save_domain_val);
       if (save_domain_val == res[n].name)
       {
        var oldval = myform.find(":input[name=\"domain_oldval\"]");
        oldval.val(res[n].id);
        typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].id + "\">" + res[n].name + '</option>');
       }
       else
        typeobj.append('<option id="type' + n + '" value=\"' + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"linkedtask_val\"]").length > 0)
   {
		var myform = pwd.find("#" + prefix + "summform");
		var typeobj = myform.find(":input[name=\"linkedtask_val\"]");
		var myTask = myform.find(":input[name=\"name_val\"]");

		for (n = 0; n < TaskList.length; n++) {
			if (TaskList[n].name == myTask.val()) continue;
			if (save_linkedtask_val == TaskList[n].name) {
				var oldval = myform.find(":input[name=\"linkedtask_oldval\"]");
				oldval.val(TaskList[n].id);
				typeobj.append('<option id="type' + n + '" selected value=\"' + TaskList[n].id + "\">" + TaskList[n].name + '</option>');
			} else {
				typeobj.append('<option id="type' + n + '" value=\"' + TaskList[n].id + "\">" + TaskList[n].name + '</option>');
			}
		}

	/*
    $.ajax(
    {
     url : 'GetTasks',
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      console.log("EHERE");
      console.log(res);
      var myform = pwd.find("#summform");
      var typeobj = myform.find(":input[name=\"linkedtask_val\"]");
      var myTask = myform.find(":input[name=\"name_val\"]");

      for (n = 0; n < TaskList.length; n++)
      {
       if (TaskList[n].name == myTask.val())
        continue;

       if (save_linkedtask_val == TaskList[n].name)
       {
        var oldval = myform.find(":input[name=\"linkedtask_oldval\"]");
        oldval.val(TaskList[n].id);
        typeobj.append('<option id="type' + n + '" selected value=\"' + TaskList[n].id + "\">" + TaskList[n].name + '</option>');
       }
       else
        typeobj.append('<option id="type' + n + '" value=\"' + TaskList[n].id + "\">" + TaskList[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
    */
  }

   myform=$("#compownerform");
   if (myform.find(":input[name=\"owner_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetUsers",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");

      var owner = myform.find(":input[name=\"owner_val\"]");

      var userid = save_owner_val;

      if (userid == "")
       userid = myuserid;

      for (n = 0; n < res.length; n++)
      {
       if (userid == res[n].name)
        owner.append('<option id="owner' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="owner' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }


   if (summSaveobjtype == "cr")
   {
    var myform = pwd.find("#" + prefix + "summform");
    var typeobj = myform.find(":input[name=\"kind_val\"]");

    var res = [
    {
        type : 'ck',
        id : 10,
        name : 'Harvest DFO in Filesystem'
    },
    {
     type : 'ck',
     id : 4,
     name : 'From Variables'
    },
    {
        type : 'ck',
        id : 2,
        name : 'Encrypted in Database'
    },
    {
    	type : 'ck',
    	id :  5,
    	name : 'Private Key'
    }];

    for (n = 0; n < res.length; n++)
    {
     if (save_kind_val == res[n].name || (save_kind_val == "unconfigured" && n == 2))
     {
      typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      if (res[n].id == 2)
      {
       $("#usernameenc_row").show();
       $("#passwordenc_row").show();
       $("#username_row").hide();
       $("#password_row").hide();
       $("#filename_row").hide();
      }
      else if (res[n].id == 4)
      {
       $("#usernameenc_row").hide();
       $("#passwordenc_row").hide();
       $("#username_row").show();
       $("#password_row").show();
       $("#filename_row").hide();
      }
      else if (res[n].id == 5)
      {
       $("#usernameenc_row").hide();
       $("#passwordenc_row").hide();
       $("#username_row").show();
       $("#password_row").hide();
       $("#filename_row").show();
      }
      else
      {
       $("#usernameenc_row").hide();
       $("#passwordenc_row").hide();
       $("#username_row").hide();
       $("#password_row").hide();
       $("#filename_row").show();
      }
     }
     else
      typeobj.append('<option id="type' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
    }
   }
   else if (summSaveobjtype == "ac" || summSaveobjtype == "fn" || summSaveobjtype == "pr")
   {
    var myform = pwd.find("#summform");
    var typeobj = myform.find(":input[name=\"kind_val\"]");
    var res = [];
    var desc = "";
    if (summSaveobjtype == "ac")
    {
     res = [
     {
      type : 'ak',
      id : 6,
      name : 'Graphical deployment flow'
     } ];
    }
    else if (summSaveobjtype == "fn")
    {
     res = [
     {
      type : 'ak',
      id : 1,
      name : 'DMScript Function in repository'
     },
     {
      type : 'ak',
      id : 2,
      name : 'DMScript in Database'
     },
     {
      type : 'ak',
      id : 3,
      name : 'Local Script'
     },
     {
      type : 'ak',
      id : 4,
      name : 'Endpoint Script'
     } ];


     desc += "<tr id='kind_sumrow_desc' ><td id=\"kind_summ_desc\" class=\"summlabel\" style=\"text-align:left; white-space: nowrap;\">";
     desc += "Object:";
     desc += "</td><td>";
     desc += "Function";
     desc += "</td></tr>";
    }
    else
    {
     res = [
     {
      type : 'ak',
      id : 1,
      name : 'DMScript Procedure in repository'
     },
     {
      type : 'ak',
      id : 2,
      name : 'DMScript in Database'
     },
     {
      type : 'ak',
      id : 3,
      name : 'Local Script'
     },
     {
      type : 'ak',
      id : 4,
      name : 'Endpoint Script'
     } ];

     desc += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\" style=\"text-align:left; white-space: nowrap;\">";
     desc += "Object:";
     desc += "</td><td>";
     desc += "Procedure";
     desc += "</td></tr>";
    }

    if (desc != "") // insert in desc
    {
     $("#group_row").after(desc);
    }

    for (n = 0; n < res.length; n++)
    {
     if (save_kind_val == res[n].name || (save_kind_val == "unconfigured" && n == 2))
     {
      typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

      console.log("res["+n+"].id="+res[n].id);
      switch (res[n].id)
      {
      case 1: // DMScript Function in repository
       $("#fragmentname_row").hide();
       $("#repository_row").show();
       $("#filepath_row").show();
       $("#copytoremote_row").hide();
       $("#allocateterminal_row").hide();
       $("#atfootnote").hide();
       $("#interpreter_row").hide();
       break;

      case 2: // DMScript Function in database
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       $("#allocateterminal_row").hide();
       $("#atfootnote").hide();
       $("#interpreter_row").hide();
       break;

      case 3: // Function provided by local external script or program
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").show();
       $("#copytoremote_row").hide();
       $("#allocateterminal_row").hide();
       $("#atfootnote").hide();
       $("#interpreter_row").hide();
       break;

      case 4: // Function provided by remote external script or program
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").show();
       $("#copytoremote_row").show();
       $("#allocateterminal_row").show();
       $("#atfootnote").show();
       $("#interpreter_row").show();
       break;

      case 5: // Function provided by plugin
       $("#fragmentname_row").show();
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       $("#allocateterminal_row").hide();
       $("#atfootnote").hide();
       $("#interpreter_row").hide();
       break;

      case 6: // Graphical deployment flow
       $("#fragmentname_row").hide(); // for now - graphical actions can not be called from a graphical action (yet)
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       $("#allocateterminal_row").hide();
       $("#atfootnote").hide();
       $("#interpreter_row").hide();
       break;

      default:
       break;
      }
      ToggleDisplayName();
     }
     else
      typeobj.append('<option id="type' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
    }
   }
   else if (summSaveobjtype == "us")
   {
    $("#usernameenc_row").hide();
    $("#passwordenc_row").hide();
    $("#username_row").show();
    $("#password_row").show();
   }

 if (summSaveobjid < 0)
 {
  var objstr = obj2Int[summSaveobjtype][1];
  if (objstr == "Notify")
   objstr = "Notifier";

  if (objstr == "Application")
   objstr = "Application Base Version";

  if (objstr == "Component")
   objstr = "Component Base Version";

  if (objstr == "Server")
   objstr = "Endpoint";

   title = "New " + objstr;
 }

 parent.$("#compowner_summ_data_edit").hide();
 parent.$("#compowner_summ_data").hide();
 parent.$("#compitem_data").hide();
 parent.$("#compitem_data_edit").hide();
 parent.$("#emailbody_data").hide();
 parent.$("#emailbody_data_edit").hide();
 parent.$("#props_data").hide();
 parent.$("#props_data_edit").hide();
 parent.$("#adminrights_data").hide();
 parent.$("#adminrights_data_edit").hide();
 parent.$("#" + prefix + "summ_data").hide();
 parent.$("#" + prefix + "summ_data_edit").show();

 if (objtype == "co" || objtype == "cv")
 {
   parent.$("#compitem_data_edit").show();
   parent.$("#compowner_summ_data_edit").show();
 }
 else if (objtype == "te")
 {
  parent.$("#emailbody_data_edit").show();
 }
 else if (objtype == "no")
 {
  parent.$("#props_data_edit").show();
 }
 else if (objtype == "gr")
 {
  parent.$("#adminrights_data_edit").show();
 }
 else if (objtype == "ds")
 {
  parent.$("#props_data_edit").show();
 }
 else if (objtype == "re")
 {
  parent.$("#props_data_edit").show();
 }
 else if (objtype == "be")
 {
  parent.$("#props_data_edit").show();
 }
}

function convertDate(in_date)
{
	// Takes a string in format MM/dd/YYYY HH:mi[:ss] and converts to a date
	console.log("convertDate("+in_date+")");
	console.log(userdatefmt);
 var match = in_date.match(/^(\d+)\/(\d+)\/(\d+) (\d+)\:(\d+)\:(\d+)/);
 if (match == null) {
	 // try alternate (no seconds)
	 match = in_date.match(/^(\d+)\/(\d+)\/(\d+) (\d+)\:(\d+)/);
 }
 var t = in_date.split(" ")[1];
 var newdate;
 if (userdatefmt == "dd/MM/yyyy") {
	 console.log("UK format (dd/MM/yyyy)");
	 console.log("match[1]="+match[1]);
	 console.log("match[2]="+match[2]);
	 newdate = match[3] + "/" + match[2] + "/" + match[1] + " " + t;
 } else {
	 console.log("US format (MM/dd/yyyy");
	 console.log("match[1]="+match[1]);
	 console.log("match[2]="+match[2]);
	 newdate = match[3] + "/" + match[1] + "/" + match[2] + " " + t;
 }
 console.log("newdate="+newdate);

 var date = new Date(newdate);
 return date;
}

function GetSaveSummaryData(instance, data, prefix)
{
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.oldname = null;
 data.newsumm = null;

 var form = instance.find("#" + prefix + "summform");
 var viewArr = form.serializeArray();

 var viewArr2 = $("#compownerform").serializeArray();

 viewArr = viewArr.concat(viewArr2);

 var view = {};

 for ( var i in viewArr)
 {
  if (prefix.length > 0 && viewArr[i].name.startsWith(prefix))
   view[viewArr[i].name.substring(3)] = viewArr[i].value;
  else if (prefix.length == 0 && !viewArr[i].name.startsWith("rf_") && !viewArr[i].name.startsWith("rb_"))
    view[viewArr[i].name] = viewArr[i].value;
 }

 var view = {};

 prefix = "";

 view["servercomptype_val"] = "";

 for ( var i in viewArr)
 {
  if (viewArr[i].name == "servercomptype_val")
   view[viewArr[i].name] += viewArr[i].value + ";";
  else
   view[viewArr[i].name] = viewArr[i].value;
 }

 if (typeof view.lifecycledomain_val == "undefined")
  view.lifecycledomain_val = false;

 if (view.lifecycledomain_val == "on" || view.lifecycledomain_val == "true")
  view.lifecycledomain_val = true;
 else
  view.lifecycledomain_val = false;

 if (view.lifecycledomain_oldval == "on" || view.lifecycledomain_oldval == "true")
  view.lifecycledomain_oldval = true;
 else
  view.lifecycledomain_oldval = false;

 if (typeof view.alwaysdeploy_val == "undefined")
  view.alwaysdeploy_val = false;

 if (view.alwaysdeploy_val == "on" || view.alwaysdeploy_val == "true")
  view.alwaysdeploy_val = true;
 else
  view.alwaysdeploy_val = false;

 if (view.alwaysdeploy_oldval == "on" || view.alwaysdeploy_oldval == "true")
  view.alwaysdeploy_oldval = true;
 else
  view.alwaysdeploy_oldval = false;

 if (typeof view.deployseq_val == "undefined")
  view.deployseq_val = false;

 if (view.deployseq_val == "on" || view.deployseq_val == "true")
  view.deployseq_val = true;
 else
  view.deployseq_val = false;

 if (view.deployseq_oldval == "on" || view.deployseq_oldval == "true")
  view.deployseq_oldval = true;
 else
  view.deployseq_oldval = false;

 if (typeof view.resultisexpr_val == "undefined")
  view.resultisexpr_val = false;

 if (view.resultisexpr_val == "on" || view.resultisexpr_val == "true")
  view.resultisexpr_val = true;
 else
  view.resultisexpr_val = false;

 if (view.resultisexpr_oldval == "on" || view.resultisexpr_oldval == "true")
  view.resultisexpr_oldval = true;
 else
  view.resultisexpr_oldval = false;

 if (typeof view.allocateterminal_val == "undefined")
  view.allocateterminal_val = false;

 if (view.allocateterminal_val == "on" || view.allocateterminal_val == "true")
  view.allocateterminal_val = true;
 else
  view.allocateterminal_val = false;

 if (view.allocateterminal_oldval == "on" || view.allocateterminal_oldval == "true")
  view.allocateterminal_oldval = true;
 else
  view.allocateterminal_oldval = false;

 if (typeof view.copytoremote_val == "undefined")
  view.copytoremote_val = false;

 if (view.copytoremote_val == "on" || view.copytoremote_val == "true")
  view.copytoremote_val = true;
 else
  view.copytoremote_val = false;

 if (view.copytoremote_oldval == "on" || view.copytoremote_oldval == "true")
  view.copytoremote_oldval = true;
 else
  view.copytoremote_oldval = false;

 if (typeof view.showoutput_val == "undefined")
  view.showoutput_val = false;

 if (view.showoutput_val == "on" || view.showoutput_val == "true")
  view.showoutput_val = true;
 else
  view.showoutput_val = false;

 if (view.showoutput_oldval == "on" || view.showoutput_oldval == "true")
  view.showoutput_oldval = true;
 else
  view.showoutput_oldval = false;

 if (typeof view.subdomains_val == "undefined")
  view.subdomains_val = false;

 if (view.subdomains_val == "on" || view.subdomains_val == "true")
  view.subdomains_val = true;
 else
  view.subdomains_val = false;

 if (view.subdomains_oldval == "on" || view.subdomains_oldval == "true")
  view.subdomains_oldval = true;
 else
  view.subdomains_oldval = false;

 if (view.accountlocked_val == "on" || view.accountlocked_val == "true")
  view.accountlocked_val = true;
 else
  view.accountlocked_val = false;

 if (view.accountlocked_oldval == "on" || view.accountlocked_oldval == "true")
  view.accountlocked_oldval = true;
 else
  view.accountlocked_oldval = false;

 if (view.forcechangepassword_val == "on" || view.forcechangepassword_val == "true")
  view.forcechangepassword_val = true;
 else
  view.forcechangepassword_val = false;

 if (view.forcechangepassword_oldval == "on" || view.forcechangepasswordd_oldval == "true")
  view.forcechangepassword_oldval = true;
 else
  view.forcechangepassword_oldval = false;

 if (view.automd5check_val == "on" || view.automd5check_val == "true")
	 view.automd5check_val = true;
 else
	 view.automd5check_val = false;

 if (view.automd5check_oldval == "on" || view.automd5check_oldval == "true")
	 view.automd5check_oldval = true;
 else
	 view.automd5check_oldval = false;


 if (view.autoping_val == "on" || view.autoping_val == "true")
	 view.autoping_val = true;
 else
	 view.autoping_val = false;

 if (view.autoping_oldval == "on" || view.autoping_oldval == "true")
	 view.autoping_oldval = true;
 else
	 view.autoping_oldval = false;


 if (view.deletedir_val == "on" || view.deletedir_val == "true")
  view.deletedir_val = true;
 else
  view.deletedir_val = false;

 if (view.database_val == "on" || view.database_val == "true")
  view.database_val = true;
 else
  view.database_val = false;

 console.log(view);

 if (view.name_val != view.name_oldval)
 {
  data.newname = view.name_val;
  data.oldname = view.name_oldval;
  data.nameCallbackReqd = true;
  console.log(prefix + 'change_' + view.name_field + ' = ' + view.name_val);
  data[prefix + 'change_' + view.name_field] = view.name_val;
  ret = true;
 }

 if (view.password_val != view.password_oldval)
 {
  console.log(prefix + 'change_' + view.password_field + ' = ' + view.password_val);
  data[prefix + 'change_' + view.password_field] = view.password_val;
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }

 if (view.filename_val != view.filename_oldval)
 {
  console.log(prefix + 'change_' + view.filename_field + ' = ' + view.filename_val);
  data[prefix + 'change_' + view.filename_field] = view.filename_val;
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }


 if (view.interpreter_val != view.interpreter_oldval)
 {
  console.log(prefix + 'change_' + view.interpreter_field + ' = ' + view.interpreter_val);
  data[prefix + 'change_' + view.interpreter_field] = view.interpreter_val;
  ret = true;
 }

 if (view.username_val != view.username_oldval && typeof view.username_val != "undefined")
 {
  if (summSaveobjtype != "cr")
  {
   data.newname = view.username_val;
   data.oldname = view.username_oldval;
   data.nameCallbackReqd = true;
  }
  console.log(prefix + 'change_' + view.username_field + ' = ' + view.username_val);
  data[prefix + 'change_' + view.username_field] = view.username_val;
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }

 if (view.passwordenc_val != view.passwordenc_oldval)
 {
  console.log(prefix + 'change_' + view.passwordenc_field + ' = ' + view.passwordenc_val);
  data[prefix + 'change_' + view.passwordenc_field] = view.passwordenc_val;
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }

 if (view.usernameenc_val != view.usernameenc_oldval)
 {
  console.log(prefix + 'change_' + view.usernameenc_field + ' = ' + view.usernameenc_val);
  data[prefix + 'change_' + view.usernameenc_field] = view.usernameenc_val;
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }

 if (view.groupname_val != view.groupname_oldval)
 {
  data.newname = view.groupname_val;
  data.oldname = view.groupname_oldval;
  data.nameCallbackReqd = true;
  console.log(prefix + 'change_' + view.groupname_field + ' = ' + view.groupname_val);
  data[prefix + 'change_' + view.groupname_field] = view.groupname_val;
  ret = true;
 }

 if (view.realname_val != view.realname_oldval)
 {
  console.log(prefix + 'change_' + view.realname_field + ' = ' + view.realname_val);
  data[prefix + 'change_' + view.realname_field] = view.realname_val;
  ret = true;
 }

 if (view.email_val != view.email_oldval)
 {
  console.log(prefix + 'change_' + view.email_field + ' = ' + view.email_val);
  data[prefix + 'change_' + view.email_field] = view.email_val;
  ret = true;
 }

 if (view.category_val != "" && typeof view.category_val != "undefined")
 {
  var id = cat2id[view.category_val];

  console.log("id="+id);

  if (typeof id == 'undefined')
  {
   $.ajax(
   {
    url : "GetCategories?reason=newcat&name=" + view.category_val,
    dataType : 'json',
    async : false,
    type : 'GET',
    success : function(res)
    {
     cat2id[view.category_val] = res.newid;
     id = "cy" + id;
     console.log(prefix + 'change_' + view.category_field + ' = ' + "cy" + res.newid);
     data[prefix + 'change_' + view.category_field] = "cy" + res.newid;
     category_changed = true;
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
  }
  else if (view.category_oldval != "cy"+cat2id[view.category_val])
  {
   id = "cy" + id;
   console.log(prefix + 'change_' + view.category_field + ' = ' + id);
   data[prefix + 'change_' + view.category_field] = id;
   category_changed = true;
  }
  ret = true;
 }

 if (view.phone_val != view.phone_oldval)
 {
  console.log(prefix + 'change_' + view.phone_field + ' = ' + view.phone_val);
  data[prefix + 'change_' + view.phone_field] = view.phone_val;
  ret = true;
 }

 if (view.protocol_val != view.protocol_oldval)
 {
  console.log(prefix + 'change_' + view.protocol_field + ' = ' + view.protocol_val);
  data[prefix + 'change_' + view.protocol_field] = view.protocol_val;
  ret = true;
 }

 if (view.availability_val != view.availability_oldval)
 {
  console.log(prefix + 'change_' + view.availability_field + ' = ' + view.availability_val);
  data[prefix + 'change_' + view.availability_field] = view.availability_val;
  ret = true;
 }

 if (view.servertype_val != view.servertype_oldval)
 {
  console.log(prefix + 'change_' + view.servertype_field + ' = ' + view.servertype_val);
  data[prefix + 'change_' + view.servertype_field] = view.servertype_val;
  ret = true;
 }

 console.log("view.comptype_val=["+view.comptype_val+"]");
 console.log("view.comptype_oldval=["+view.comptype_oldval+"]");
 console.log("save_comptype_val=["+save_comptype_val+"]");
 console.log("save_comptype_id=["+save_comptype_id+"]");
 if (typeof view.comptype_val == "undefined") view.comptype_val="";
 if (save_comptype_id != view.comptype_val && view.comptype_field != "undefined")
 {
  var val = view.comptype_val;
  val = val.replace(/N/g,'');
  val = val.replace(/Y/g,'');
  console.log(prefix + 'change_' + view.comptype_field + ' = ' + val);
  data[prefix + 'change_' + view.comptype_field] = val;
  ret = true;
 }

 if (view.basedirectory_val != view.basedirectory_oldval)
 {
  console.log(prefix + 'change_' + view.basedirectory_field + ' = ' + view.basedirectory_val);
  data[prefix + 'change_' + view.basedirectory_field] = view.basedirectory_val;
  ret = true;
 }

 if (view.engine_val != view.engine_oldval)
 {
  console.log(prefix + 'change_' + view.engine_field + ' = ' + view.engine_val);
  data[prefix + 'change_' + view.engine_field] = view.engine_val;
  ret = true;
 }

 if (view.port_val != view.port_oldval)
 {
  console.log(prefix + 'change_' + view.port_field + ' = ' + view.port_val);
  data[prefix + 'change_' + view.port_field] = view.port_val;
  ret = true;
 }

 if (view.hostname_val != view.hostname_oldval)
 {
  console.log(prefix + 'change_' + view.hostname_field + ' = ' + view.hostname_val);
  data[prefix + 'change_' + view.hostname_field] = view.hostname_val;
  ret = true;
 }

 if (view.sshport_val != view.sshport_oldval)
 {
  console.log(prefix + 'change_' + view.sshport_field + ' = ' + view.sshport_val);
  data[prefix + 'change_' + view.sshport_field] = view.sshport_val;
  ret = true;
 }

 if (view.alwaysdeploy_val != view.alwaysdeploy_oldval)
 {
  console.log(prefix + 'change_' + view.alwaysdeploy_field + ' = ' + view.alwaysdeploy_val);
  data[prefix + 'change_' + view.alwaysdeploy_field] = view.alwaysdeploy_val;
  ret = true;
 }

 if (view.deployseq_val != view.deployseq_oldval)
 {
  console.log(prefix + 'change_' + view.deployseq_field + ' = ' + view.deployseq_val);
  data[prefix + 'change_' + view.deployseq_field] = view.deployseq_val;
  ret = true;
 }

 if (view.showoutput_val != view.showoutput_oldval)
 {
  console.log(prefix + 'change_' + view.showoutput_field + ' = ' + view.showoutput_val);
  data[prefix + 'change_' + view.showoutput_field] = view.showoutput_val;
  ret = true;
 }

 if (view.subdomains_val != view.subdomains_oldval)
 {
  console.log(prefix + 'change_' + view.subdomains_field + ' = ' + view.subdomains_val);
  data[prefix + 'change_' + view.subdomains_field] = view.subdomains_val;
  ret = true;
 }

 if (view.accountlocked_val != view.accountlocked_oldval)
 {
  console.log(prefix + 'change_' + view.accountlocked_field + ' = ' + view.accountlocked_val);
  data[prefix + 'change_' + view.accountlocked_field] = view.accountlocked_val;
  ret = true;
 }

 if (view.forcechangepassword_val != view.forcechangepassword_oldval)
 {
  console.log(prefix + 'change_' + view.forcechangepassword_field + ' = ' + view.forcechangepassword_val);
  data[prefix + 'change_' + view.forcechangepassword_field] = view.forcechangepassword_val;
  ret = true;
 }

 if (view.summary_val != view.summary_oldval)
 {
  console.log(prefix + 'change_' + view.summary_field + ' = ' + view.summary_val);
  data[prefix + 'change_' + view.summary_field] = view.summary_val;
  ret = true;
 }

 if (view.filterlevel_val != view.filterlevel_oldval)
 {
  console.log(prefix + 'change_' + view.filterlevel_field + ' = ' + view.filterlevel_val);
  data[prefix + 'change_' + view.filterlevel_field] = view.filterlevel_val;
  ret = true;
 }

;
 if (typeof view.rollup_val == "undefined") view.rollup_val="";
 if (typeof view.rollup_oldval == "undefined") view.rollup_oldval="";
 if (view.rollup_val.toLowerCase() != view.rollup_oldval.toLowerCase())
 {
  console.log(prefix + 'change_' + view.rollup_field + ' = ' + view.rollup_val);
  data[prefix + 'change_' + view.rollup_field] = view.rollup_val;
  ret = true;
 }

 if (typeof view.rollback_val == "undefined") view.rollback_val="";
 if (typeof view.rollback_oldval == "undefined") view.rollback_oldval="";

 if (view.rollback_val.toLowerCase() != view.rollback_oldval.toLowerCase())
 {
  console.log(prefix + 'change_' + view.rollback_field + ' = ' + view.rollback_val);
  data[prefix + 'change_' + view.rollback_field] = view.rollback_val;
  ret = true;
 }

 console.log("view.preaction_val=["+view.preaction_val+"] view.preaction_oldval=["+view.preaction_oldval+"]");
 if (view.preaction_val != view.preaction_oldval)
 {
  if (view.preaction_val == "NULL")
   view.preaction_val = null;

  if (view.preaction_val != null) {
	  console.log(prefix + 'change_' + view.preaction_field + ' = ' + view.preaction_val);
	  data[prefix + 'change_' + view.preaction_field] = view.preaction_val;
	  ret = true;
  }
 }

 if (view.postaction_val != view.postaction_oldval)
 {
  if (view.postaction_val == "NULL")
   view.postaction_val = null;

  console.log(prefix + 'change_' + view.postaction_field + ' = ' + view.postaction_val);
  data[prefix + 'change_' + view.postaction_field] = view.postaction_val;
  ret = true;
 }

 console.log("view.fulldomain_val=["+view.fulldomain_val+"]");
 console.log("view.fulldomain_oldval=["+view.fulldomain_oldval+"]");
 if (view.fulldomain_val != view.fulldomain_oldval)
 {
  if (view.fulldomain_val == "NULL")
   view.fulldomain_val = null;

  console.log(prefix + 'change_' + view.fulldomain_field + ' = ' + view.fulldomain_val);
  data[prefix + 'change_' + view.fulldomain_field] = view.fulldomain_val;
  ret = true;
 }

 console.log("view.notifier_val=["+view.notifier_val+"]");
 console.log("view.notifier_oldval=["+view.notifier_oldval+"]");
 if (view.notifier_val != view.notifier_oldval)
 {
  if (view.notifier_val == "NULL")
   view.notifier_val = null;

  console.log(prefix + 'change_' + view.notifier_field + ' = ' + view.notifier_val);
  data[prefix + 'change_' + view.notifier_field] = view.notifier_val;
  ret = true;
 }

 console.log("view.builder_val=["+view.builder_val+"]");
 console.log("view.builder_oldval=["+view.builder_oldval+"]");
 if (view.builder_val != view.builder_oldval)
 {
  if (view.builder_val == "NULL")
   view.builder_val = null;

  console.log(prefix + 'change_' + view.builder_field + ' = ' + view.builder_val);
  data[prefix + 'change_' + view.builder_field] = view.builder_val;
  ret = true;
 }

 console.log("view.customaction_val=["+view.customaction_val+"]");
 console.log("view.customaction_oldval=["+view.customaction_oldval+"]");
 if (view.customaction_val != view.customaction_oldval)
 {
  if (view.customaction_val == "NULL")
   view.customaction_val = null;

  console.log(prefix + 'change_' + view.customaction_field + ' = ' + view.customaction_val);
  data[prefix + 'change_' + view.customaction_field] = view.customaction_val;
  ret = true;
 }

 if (view.template_val != view.template_oldval)
 {
  if (view.template_val == "NULL")
   view.template_val = null;

  console.log(prefix + 'change_' + view.template_field + ' = ' + view.template_val);
  data[prefix + 'change_' + view.template_field] = view.template_val;
  ret = true;
 }

 if (view.fail_template_val != view.fail_template_oldval)
 {
  if (view.fail_template_val == "NULL")
   view.fail_template_val = null;

  console.log(prefix + 'change_' + view.fail_template_field + ' = ' + view.fail_template_val);
  data[prefix + 'change_' + view.fail_template_field] = view.fail_template_val;
  ret = true;
 }

 if (view.type_val != view.type_oldval)
 {
  if (view.type_val == "NULL")
   view.type_val = null;

  console.log(prefix + 'change_' + view.type_field + ' = ' + view.type_val);
  data[prefix + 'change_' + view.type_field] = view.type_val;
  ret = true;
 }

 if (typeof view.kind_val != "undefined")
 {
  if (view.kind_val == "NULL")
   view.kind_val = null;

  console.log(prefix + 'change_' + view.kind_field + ' = ' + view.kind_val);
  data[prefix + 'change_' + view.kind_field] = view.kind_val;
  ret = true;
 }

 if (view.datefmt_val != view.datefmt_oldval)
 {
	 console.log(prefix + 'change_' + view.datefmt_field + ' = ' + view.datefmt_val);
	 userdatefmt = view.datefmt_val;
	 data[prefix + 'change_' + view.datefmt_field] = view.datefmt_val;
	 ret = true;
 }

 if (view.timefmt_val != view.timefmt_oldval)
 {
	 console.log(prefix + 'change_' + view.timefmt_field + ' = ' + view.timefmt_val);
	 usertimefmt = view.timefmt_val;
	 data[prefix + 'change_' + view.timefmt_field] = view.timefmt_val;
	 ret = true;
 }

 var ownertype = $("#ownertype_val");
 var owt = "User";

 if (ownertype.length > 0)
   owt = ownertype.val();

 if (owt == "User")
 {
  if (view.owner_val != view.owner_oldval)
  {
   console.log(prefix + 'change_' + view.owner_field + ' = ' + view.owner_val);
   data[prefix + 'change_' + view.owner_field] = view.owner_val;
   ret = true;
  }
 }
 else
 {
  if (view.groupowner_val != view.groupowner_oldval)
  {
   console.log(prefix + 'change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data[prefix + 'change_' + view.groupowner_field] = view.groupowner_val;
   ret = true;
  }
 }

 if (view.fragmentname_val != view.fragmentname_oldval)
 {
  console.log(prefix + 'change_' + view.fragmentname_field + ' = ' + view.fragmentname_val);
  data[prefix + 'change_' + view.fragmentname_field] = view.fragmentname_val;
  ret = true;
 }

 if (form.find(":input[name=\"comptype_val\"]").length == 0)
 {
	 console.log("view.repository_val="+view.repository_val+" view.repository_oldval="+view.repository_oldval);
  if (view.repository_val != view.repository_oldval)
  {
   console.log(prefix + 'change_' + view.repository_field + ' = ' + view.repository_val);
   data[prefix + 'change_' + view.repository_field] = view.repository_val;
   ret = true;
  }
 }

 if (view.filepath_val != view.filepath_oldval)
 {
  console.log(prefix + 'change_' + view.filepath_field + ' = ' + view.filepath_val);
  data[prefix + 'change_' + view.filepath_field] = view.filepath_val;
  ret = true;
 }

 if (view.lifecycledomain_val != view.lifecycledomain_oldval)
 {
  console.log(prefix + 'change_' + view.lifecycledomain_field + ' = ' + view.lifecycledomain_val);
  data[prefix + 'change_' + view.lifecycledomain_field] = view.lifecycledomain_val;
  ret = true;
 }

 if (view.copytoremote_val != view.copytoremote_oldval)
 {
  console.log(prefix + 'change_' + view.copytoremote_field + ' = ' + view.copytoremote_val);
  data[prefix + 'change_' + view.copytoremote_field] = view.copytoremote_val;
  ret = true;
 }

 if (view.allocateterminal_val != view.allocateterminal_oldval)
 {
  console.log(prefix + 'change_' + view.allocateterminal_field + ' = ' + view.allocateterminal_val);
  data[prefix + 'change_' + view.allocateterminal_field] = view.allocateterminal_val;
  ret = true;
 }

 console.log("view.credential_val=["+view.credential_val+"]");
 console.log("view.credential_oldval=["+view.credential_oldval+"]");
 if (view.credential_val != view.credential_oldval)
 {
  console.log(prefix + 'change_' + view.credential_field + ' = ' + view.credential_val);
  data[prefix + 'change_' + view.credential_field] = view.credential_val;
  ret = true;
 }

 if (view.resultisexpr_val != view.resultisexpr_oldval)
 {
  console.log(prefix + 'change_' + view.resultisexpr_field + ' = ' + view.resultisexpr_val);
  data[prefix + 'change_' + view.resultisexpr_field] = view.resultisexpr_val;
  ret = true;
 }

 console.log("view.datasource_val="+view.datasource_val);
 console.log("view.bt_datasource_val="+view.bt_datasource_val);
 console.log("view.datasource_oldval="+view.datasource_oldval);

 if (view.datasource_val != view.datasource_oldval && typeof view.datasource_val != "undefined")
 {
	  console.log(prefix + 'change_' + view.datasource_field + ' = ' + view.datasource_val);
	  data[prefix + 'change_' + view.datasource_field] = view.datasource_val;
	  ret = true;
 }

 if (view.bt_datasource_val != view.datasource_oldval && typeof view.bt_datasource_val != "undefined")
 {
	  console.log(prefix + 'change_' + view.datasource_field + ' = ' + view.bt_datasource_val);
	  data[prefix + 'change_' + view.datasource_field] = view.bt_datasource_val;
	  ret = true;
 }

 console.log("view.projectname_val="+view.projectname_val);
 console.log("view.projectname_oldval="+view.projectname_oldval);

 if (view.projectname_val != view.projectname_oldval && typeof view.projectname_val != "undefined")
 {
	 console.log(prefix + 'change_' + view.projectname_field + ' = ' + view.projectname_val);
	 data[prefix + 'change_' + view.projectname_field] = view.projectname_val;
	 ret = true;
 }


 if (view.automd5check_val != view.automd5check_oldval)
 {
  console.log(prefix + 'change_' + view.automd5check_field + "=" + view.automd5check_val);
  data[prefix + 'change_' + view.automd5check_field] = view.automd5check_val;
  ret = true;
 }

 if (view.buildjob_val != view.buildjob_oldval)
 {
  console.log(prefix + 'change_' + view.buildjob_field + "=" + view.buildjob_val);
  data[prefix + 'change_' + view.buildjob_field] = view.buildjob_val;
  ret = true;
 }

 if (view.autoping_val != view.autoping_oldval)
 {
  console.log(prefix + 'change_' + view.autoping_field + "=" + view.autoping_val);
  data[prefix + 'change_' + view.autoping_field] = view.autoping_val;
  ret = true;
 }

 if (view.database_val != view.database_oldval && typeof view.database_oldval != "undefined")
 {
  console.log(prefix + 'change_' + view.database_field + "=" + view.database_val);
  data[prefix + 'change_' + view.database_field] = view.database_val;
  ret = true;
 }

 if (view.deletedir_val != view.deletedir_oldval && typeof view.deletedir_oldval != "undefined")
 {
  console.log(prefix + 'change_' + view.deletedir_field + "=" + view.deletedir_val);
  data[prefix + 'change_' + view.deletedir_field] = view.deletedir_val;
  ret = true;
 }

 console.log("view.servercomptype_val=["+view.servercomptype_val+"]");
 console.log("view.servercomptype_oldval=["+view.servercomptype_oldval+"]");
 console.log("save_comptype_id=["+save_comptype_id+"]");
 //if (view.servercomptype_val != view.servercomptype_oldval && typeof view.servercomptype_oldval != "undefined")
 if (view.servercomptype_val != save_comptype_id)
 {
	 if (typeof view.servercomptype_field != "undefined") {
	  console.log(prefix + 'change_' + view.servercomptype_field + "=" + view.servercomptype_val);
	  data[prefix + 'change_' + view.servercomptype_field] = view.servercomptype_val;
	  ret = true;
	 }
 }

 console.log("pinginterval_val="+view.pinginterval_val);
 console.log("pinginterval_oldval="+view.pinginterval_oldval);

 if (view.pinginterval_val != view.pinginterval_oldval)
 {
	 console.log(prefix + 'change_' + view.pinginterval_field + "=" + view.pinginterval_val);
	 data[prefix + 'change_' + view.pinginterval_field] = view.pinginterval_val;
	 ret = true;
 }

 var osv = "";
 var oev = "";

 if (typeof view.pingstart_oldval != "undefined")
  osv = (parseInt(view.pingstart_oldval.substr(0,2)) * 3600) + (parseInt(view.pingstart_oldval.substr(3,2)) * 60);

 if (typeof view.pingend_oldval != "undefined")
  oev = (parseInt(view.pingend_oldval.substr(0,2)) * 3600) + (parseInt(view.pingend_oldval.substr(3,2)) * 60);

 if (view.pingstart_val != osv && typeof view.pingstart_val != "undefined")
 {
	 console.log(prefix + 'change_' + view.pingstart_field + "=" + view.pingstart_val);
	 data[prefix + 'change_' + view.pingstart_field] = view.pingstart_val;
	 ret = true;
 }

 if (view.pingend_val != oev  && typeof view.pingend_val != "undefined")
 {
	 console.log(prefix + 'change_' + view.pingend_field + "=" + view.pingend_val);
	 data[prefix + 'change_' + view.pingend_field] = view.pingend_val;
	 ret = true;
 }

 return ret;
}

function FilterLevelChanged()
{
 var id = $("#filterlevel_val").val();

 if (id == "Items")
 {
  $("#rollback_row").hide();
  $("#rollforward_row").hide();
 }
 else
 {
  $("#rollback_row").show();
  $("#rollforward_row").show();
 }
}

function ProtocolChanged()
{
 var id = $("#protocol_val").val();

 if (id == "sftp")
 {
  $("#sshport_row").show();
 }
 else
 {
  $("#sshport_row").hide();
 }
}

function CompTypeChanged()
{
// var s = $( "#comptype_val" ).val();
//
// if (s.substring(0,1) == "Y")
// {
//  $("#repository_row").hide();
//  $("#rf_repository_row").show();
//  $("#rb_repository_row").show();
//  $("#alwaysdeploy_val").prop( "checked", false );
//  ToggleRepoProps(-1,"rf_repository_row",true);
//  ToggleRepoProps(-1,"rb_repository_row",false);
// }
// else
// {
//  $("#repository_row").show();
//  $("#rf_repository_row").hide();
//  $("#rb_repository_row").hide();
//  $("#alwaysdeploy_val").prop( "checked", true );
//  ToggleRepoProps(-1,"repository_row",true);
// }
}

function BuilderChanged()
{
 var pwd = parent.$("#summ_data_edit");
 var myform = pwd.find("#summform");

 if (myform.find(":input[name=\"projectname_val\"]").length > 0)
 {

  myform.find(":input[name=\"projectname_val\"]").empty();

  $.ajax({
   url : "GetProjectsFromBuilder?id="+summSaveobjid+"&be="+lastSelectedNode.substr(2),
   dataType : 'json',
   type : 'GET',
   success : function(res)
   {
    if (typeof res.error != "undefined") {
     alert("Cannot retrieve Project List: "+res.error);
    } else {
     var myform = pwd.find("#summform");
     var builder = myform.find(":input[name=\"projectname_val\"]");
     var cnt = 0;

     if ("jobs" in res)
     {
      for (n = 0; n < res.jobs.length; n++)
      {
       if (save_projectname_val == res.jobs[n].name)
       {
        builder.append('<option id="project' + n + '" selected value=\"' + res.jobs[n].name + "\">" + res.jobs[n].name + '</option>');
        cnt++;
       }
       else
        builder.append('<option id="project' + n + '" value=\"' + res.jobs[n].name + "\">" + res.jobs[n].name + '</option>');
      }
     }
     if (cnt == 0) builder.prop('selectedIndex', 0);
    }
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
});
 }
}

function KindChanged()
{
 var id = $("#kind_val").val().substring(2);

 console.log(id);

 if (summSaveobjtype == "cr")
 {
  if (id == 2)
  {
   $("#usernameenc_row").show();
   $("#passwordenc_row").show();
   $("#username_row").hide();
   $("#password_row").hide();
   $("#filename_row").hide();
  }
  else if (id == 4)
  {
   $("#usernameenc_row").hide();
   $("#passwordenc_row").hide();
   $("#username_row").show();
   $("#password_row").show();
   $("#filename_row").hide();
  }
  else if (id == 5)
  {
   $("#usernameenc_row").hide();
   $("#passwordenc_row").hide();
   $("#username_row").show();
   $("#password_row").hide();
   $("#filename_row").show();
  }
  else
  {
   $("#usernameenc_row").hide();
   $("#passwordenc_row").hide();
   $("#username_row").hide();
   $("#password_row").hide();
   $("#filename_row").show();
  }
 }
 else if (summSaveobjtype == "ac" || summSaveobjtype == "fn" || summSaveobjtype == "pr")
 {
  id = Number(id);

  console.log("summSaveobjtype="+summSaveobjtype+" id="+id);

  switch (id)
  {
  case 1: // DMScript Function in repository
   $("#fragmentname_row").hide();
   $("#repository_row").show();
   $("#filepath_row").show();
   $("#copytoremote_row").hide();
   $("#allocateterminal_row").hide();
   $("#atfootnote").hide();
   break;

  case 2: // DMScript Function in database
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
   $("#allocateterminal_row").hide();
   $("#atfootnote").hide();
   break;

  case 3: // Function provided by local external script or program
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").show();
   $("#copytoremote_row").hide();
   $("#allocateterminal_row").hide();
   $("#atfootnote").hide();
   break;

  case 4: // Function provided by remote external script or program
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").show();
   $("#copytoremote_row").show();
   $("#allocateterminal_row").show();
   $("#atfootnote").show();
   break;

  case 5: // Function provided by plugin
   $("#fragmentname_row").show();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
   $("#allocateterminal_row").hide();
   $("#atfootnote").hide();
   break;

  case 6: // Graphical deployment flow
   $("#fragmentname_row").show();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
   $("#allocateterminal_row").hide();
   $("#atfootnote").hide();
   break;

  default:
   break;
  }

 }
}

function GetSaveTargetData(instance)
{
 ret = false;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view =
 {};

 for ( var i in viewArr)
 {
  view[viewArr[i].name] = viewArr[i].value;
 }

 if (view.domain_val != view.domain_oldval)
 {
  var tasktype = view.domain_field;
  var id = view.domain_val;
  return new Array(tasktype, id);
 }

 return null;
}

function ChangeSummaryName(newname)
{
 var heading = parent.$("#header_icon");
 var html = heading.html();
 var parts = html.split(':');
 html = parts[0] + ": " + newname;

 heading.html(html);
 RefreshName(currenttree, newname);
}

function GetSaveLinkedData(instance)
{
	ret = false;

	var form = instance.find("#summform");
	var viewArr = form.serializeArray();
	var view = {};

	for (var i in viewArr) {
		view[viewArr[i].name] = viewArr[i].value;
	}

	if (view.linkedtask_val != view.linkedtask_oldval) {
		var tasktype = view.linkedtask_field;
		var id = view.linkedtask_val;
		return new Array(tasktype, id);
	}

	return null;
}

function GetSaveActionData(instance)
{
	ret = false;

	var form = instance.find("#summform");
	var viewArr = form.serializeArray();
	var view = {};

	for (var i in viewArr) {
		view[viewArr[i].name] = viewArr[i].value;
	}

	if (view.taskaction_val != view.taskaction_oldval) {
		var tasktype = view.taskaction_field;
		var id = view.taskaction_val;
		return new Array(tasktype, id);
	}

	return null;
}

function SaveRepo4Item(objid,repo)
{
 var savedata =
 {
  objtype : 14,
  id : objid
 };

 savedata['change_704'] = repo;

 console.log("id=" + objid + " change_704 = " + repo);

 $.ajax(
   {
    url : "UpdateSummaryData",
    dataType : "json",
    type : "POST",
    data : savedata,
    success : function(data)
    {
     console.log(data);
    }
   });
}

function SaveSummaryData(instance, tablename, objtypeAsInt, objtype, objid, addParams,isProfile)
{
 var savedata =
 {
  objtype : objtypeAsInt,
  id : objid
 };

 var sumret = "";

 if (tablename == "task_summ")
  sumret = GetSaveSummaryData(instance, savedata, "task_");
 else
  sumret = GetSaveSummaryData(instance, savedata, '');

 console.log(savedata);

 if (sumret)
 {
  $.ajax(
  {
   url : "UpdateSummaryData",
   dataType : "json",
   type : "POST",
   data : savedata,
   success : function(data)
   {
    console.log(data);
    if (data && data.saved)
    {
     if (tablename == "task_summ")
     {
      var cb = prevcb;
      LoadTasks("taskstab", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, "");
      $("#" + cb).prop("checked",true);
      LoadTaskSummary(cb);
     }
     else
     {
      objid = data.id;
      console.log("Saved data");
      SaveTargetDomain(instance);
      SaveLinkedTask(instance,objid);
      SaveActionTask(instance,objid);
      SaveDomainOrder();
      SavePropertiesData();
      SaveBodyData();
      SaveAdminRightsData();
      SaveSummaryItemData(instance, tablename, objtypeAsInt, objtype, objid, addParams);
      LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
      if (summSaveobjtype == "do" && ('change_1' in savedata || 'change_1002' in savedata))
      {
       lookupDom = savedata.newname;
       LoadDomNav();
      }
     }
    }
    else
    {
     if (data && data.error)
     {
      alert(data.error);
      return;
     }
     else
     {
      alert("Save failed!");
     }
    }
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    alert(err);
    return;
   }
  });
 }
 else
 {
  if (tablename == "task_summ")
  {
   var cb = prevcb;
   prevcb = null;
   LoadTasks("taskstab", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, "");
   $("#" + cb).prop("checked",true);
   LoadTaskSummary(cb);
  }
  else
  {
   SaveTargetDomain(instance);
   SaveLinkedTask(instance,objid);
   SaveActionTask(instance,objid);
   SaveDomainOrder();
   SavePropertiesData();
   SaveBodyData();
   SaveAdminRightsData();
   SaveSummaryItemData(instance, tablename, objtypeAsInt, objtype, objid, addParams);
   LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
  }
 }
 return;
}

function SaveRbRfValues(myform)
{
 ret = false;
 var data =
 {
  objtype : 14,
  id : objid
 };

 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 $.ajax(
   {
    url : "GetComponentItemLayout?rbrf=y&compid=" + objid,
    type : 'POST',
    dataType : "json",
    success : function(res)
    {
     console.log("######");
     console.log(res);

     var comptype = myform.find(":input[name=\"comptype_val\"]").val();

     var viewArr = myform.serializeArray();

     for (var k=0;k < res.data.length;k++)
     {
      var ci = res.data[k];
      if (ci.rollback == "ON")
      {
       var view = {};

       for ( var i in viewArr)
       {
        if (viewArr[i].name.indexOf("rb_prop_") >= 0)
         view[viewArr[i].name.substring(3)] = viewArr[i].value;

       }

       SaveRepo4Item(ci.id,myform.find(":input[name=\"rb_repository_val\"]").val());
       SavePropertyData(ci.id, view)
      }

      if (ci.rollforward == "ON")
      {
       var view = {};

       for ( var i in viewArr)
       {
        if (viewArr[i].name.indexOf("rf_prop_") >= 0)
         view[viewArr[i].name.substring(3)] = viewArr[i].value;
       }
       SaveRepo4Item(ci.id,myform.find(":input[name=\"rf_repository_val\"]").val());
       SavePropertyData(ci.id, view)
      }

      if (ci.rollback == "OFF" && ci.rollforward == "OFF")
      {
       var view = {};

       for ( var i in viewArr)
       {
        if (viewArr[i].name.indexOf("prop_") >= 0)
         view[viewArr[i].name] = viewArr[i].value;
       }
       SaveRepo4Item(ci.id,myform.find(":input[name=\"repository_val\"]").val());
       SavePropertyData(ci.id, view)
      }
     }
    }
   ,
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    alert(err);
    return;
   }
   });

}

function SaveDomainOrder()
{
 var pwd = parent.$("#summ_data_edit");
 var sdl = pwd.find("#subdomainlist");
 if (sdl.length > 0 && isLifeCycle)
 {
  $.ajax(
  {
   async : true,
   url : "DomainDetails?f=md&domainid=" + objid + "&" + sdl.sortable("serialize",
   {
    key : "dom"
   }),
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
	   console.log("refreshing domain order?");
    DoRefresh();
   }
  });
 }
 else if (summSaveisLifeCycle != isLifeCycle)
 {
	 console.log("lifecycles have changed");
  DoRefresh();
 }
}

function SaveActionTask(instance,objid)
{
	console.log("**** SaveActionTask");
	var linkedret = GetSaveActionData(instance);
	console.log(linkedret);
	if (linkedret != null) {
		var tasktype = linkedret[0];
		var actionOTID = linkedret[1];
		var url = "UpdateAttrs?f=" + tasktype + "&tid=" + objid + "&tgt=" + actionOTID.substr(2);
		console.log(url);

		$.ajax({
			url : url,
			type : "GET",
			async : false,
			dataType : "text",
			success : function(data) {},
			error : function(jqxhr, status, err) {
				console.log(status);
				console.log(err);
				alert(err);
				return;
			}
		});
	}
}

function SaveLinkedTask(instance,objid)
{
	var linkedret = GetSaveLinkedData(instance);
	if (linkedret != null) {
		var tasktype = linkedret[0];
		var domainid = linkedret[1];
		var url = "UpdateAttrs?f=" + tasktype + "&tid=" + objid + "&tgt=" + domainid;
		console.log(url);

		$.ajax({
			url : url,
			type : "GET",
			async : false,
			dataType : "text",
			success : function(data) {},
			error : function(jqxhr, status, err) {
				console.log(status);
				console.log(err);
				alert(err);
				return;
			}
		});
	}
}

function SaveTargetDomain(instance)
{
 var tarret = GetSaveTargetData(instance);

 if (tarret != null)
 {
  var tasktype = tarret[0];
  var domainid = tarret[1];
  var url = "UpdateAttrs?f=" + tasktype + "&tid=" + summSaveobjid + "&tgt=" +  domainid;
  console.log(url);

  $.ajax(
  {
   async : false,
   url : url,
   type : "GET",
   dataType : "text",
   success : function(data)
   {
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    alert(errmsg);
    return;
   }
  });
 }
}

function FireSummaryEvent(reason, data)
{
 if (eventcbfn != "")
 {
  try
  {
   var eventCallbackFn = eval(eventcbfn);
   if (typeof eventCallbackFn === "function")
   {
    console.log(eventcbfn + " called");
    return eventCallbackFn(reason, data);
   }
  }
  catch (e)
  {
   console.log('EXCEPTION: ' + e);
  }
 }
 return false;
}



function ToggleDisplayName(ev) {
// if (event.param2 == "No category" || event.param2 == "Do not display")
// {
//  var pwd = parent.$("#summ_data_edit");
//  var myform = pwd.find("#summform");
//  var f = myform.find(":input[name=\"fragmentname_val\"]");
//
//  if (f.length > 0)
//   f.val("");
//
//  $("#fragmentname_row").hide();
// }
// else
  $("#fragmentname_row").show();
}

function ToggleAutoPing()
{
	// var checkedValue = $('.messageCheckbox:checked').val();
 var pwd = parent.$("#summ_data_edit");
 var myform = pwd.find("#summform");
 var f = myform.find(":checked[name=\"autoping_val\"]");
 var val = f.val();
 if (val) {
	 $("#automd5_row").show();
	 $("#pinginterval_row").show();
	 $("#pingstart_row").show();
	 $("#pingend_row").show();
	 $("#pingtemplate_row").show();
	 $("#md5template_row").show();
 } else {
	 $("#automd5_row").hide();
	 $("#pinginterval_row").hide();
	 $("#pingstart_row").hide();
	 $("#pingend_row").hide();
	 $("#pingtemplate_row").hide();
	 $("#md5template_row").hide();
 }
}

function TogglePasswordFields()
{
	var pwd = parent.$("#summ_data_edit");
	var myform = pwd.find("#summform");
	var f = myform.find("[name=\"datasource_val\"]");
	if (f.val() != "") {
		$("#fcp_row").hide();
		$("#password_row").hide();
		$("#al_row").hide();
	} else {
		$("#fcp_row").show();
		$("#password_row").show();
		$("#al_row").show();
	}
	//alert("TogglePasswordFields "+val);
}

function ToggleOwner()
{
 var pwd = parent.$("#summ_data_edit");
 var myform = pwd.find("#summform");
 var f = myform.find(":input[name=\"ownertype_val\"]");
 var val = f.val();

 if (val == "User")
 {
  $("#owner_row").show();
  $("#group_row").hide();
 }
 else
 {
  $("#owner_row").hide();
  $("#group_row").show();
 }
}


function LoadUserProfile()
{
 $.ajax(
   {
    url : "Profile",
    dataType : 'json',
    async: false,
    type : 'GET',
    success : function(res)
   {
    myuid = res.uid;
    var data = {};
    data['id'] = "us" + myuid;
    eventOpenRow("users",admin,data);
   }
  });

 return;

 console.log("GetSummaryData?objtype=6&id=" + myuid);

 profSaveobjtypeAsInt = 6;
 profSaveobjid = myuid;
 profSaveobjtype = "us";
 profSavetablename = "summtab";

 $.ajax(
   {
    url : "GetSummaryData?objtype=6&id=" + myuid,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     console.log(res);
     readonly = res.readOnly;

     if (readonly)
      $("#title_icon").hide();
     else
      $("#title_icon").show();

     saveRes = res;
     var td = "";
     tdedit2 = "<form id=\"summform\"><table id=\"summtab\" class=\"dev_table\">";

     for (var r = 0; r < res.data.length; r++)
     {
      var row = res.data[r];
      var field = row[0];
      var callback = row[1];
      var rtype = row[2];
      var label = row[3];
      var val = row[4];
      var isuser = true;
      var oldval = "";

      if (label == "Username" || label == "User Name")
      {
        tdedit2 += "<tr id=\"username_row\">";
        tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td>" + val + "</td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_oldval\" value=\"" + val + "\"/></td>";
        tdedit2 += "</tr>";
      }
      else if (label == "Real Name")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"realname_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Email")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"email_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Phone")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"phone_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Password")
      {
        tdedit2 += "<tr id=\"password_row\">";
        tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\"> New " + label + ":</td>";
        tdedit2 += "<td><input name=\"password_val\" style='width:100%' type=\"password\" /></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_oldval\" /></td>";
        tdedit2 += "</tr>";
        tdedit2 += "<tr id=\"password_again_row\">";
        tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\"> New " + label + " Again:</td>";
        tdedit2 += "<td><input name=\"password_again_val\" style='width:100%' type=\"password\" /></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_oldval\" /></td>";
        tdedit2 += "</tr>";
      }
      else if (label == "Date Format")
      {
        tdedit2 += "<tr id=\"datefmt_row\">";
        tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td><select name=\"datefmt_val\" style='width:100%'>";
        tdedit2 += GetDateFormats(val);
        tdedit2 += "</select></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"datefmt_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"datefmt_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"datefmt_oldval\" /></td>";
        tdedit2 += "</tr>";
      }
      else if (label == "Time Format")
      {
        tdedit2 += "<tr id=\"timefmt_row\">";
        tdedit2 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td><select name=\"timefmt_val\" style='width:100%'>";
        tdedit2 += GetTimeFormats(val);
        tdedit2 += "</select></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_oldval\" /></td>";
        tdedit2 += "</tr>";
      }
     }
     tdedit2 += "</table></form>";
     var pwd = parent.$("#summ_data_edit");
     console.log(tdedit2);
     pwd.html(td);
     EditSummaryButton(true,"");
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
}
