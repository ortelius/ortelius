/*
 *
 *  Ortelius for Microservice Configuration Mapping
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

function LoadDeploymentSummaryData(objid)
{
	console.log("GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid);
	$.ajax({
		url : "GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid,
		dataType : 'json',
		type : 'GET',
		success : function(res) {
			console.log(res);
			var td = "";

			for (var r = 0; r < res.data.length; r++) {
				var row = res.data[r];
				var field = row[0];
				var callback = row[1];
				var rtype = row[2];
				var label = row[3];
				var val = row[4];
				var isuser = true;
				var oldval = "";

				if (label == "Name") {
					objName = val;
				} else if (label == "Deployed By") {
					var owner = val;
					if (typeof owner.name != "undefined") {
						val = owner.name;
						if (owner.type != "us") isuser = false;
					} else {
						owner = new Object();
						owner.type = "us";
						owner.name = "";
						owner.id = 0;
						val = "";
					}
				} else if (label == "Application Name") {
					var owner = val;
					if (typeof owner.name != "undefined") {
						val = owner.name;
						if (owner.type != "us") isuser = false;
					} else {
						owner = new Object();
						owner.type = "us";
						owner.name = "";
						owner.id = 0;
						val = "";
					}
				} else if (label == "Environment Name") {
					var owner = val;
					if (typeof owner.name != "undefined") {
						val = owner.name;
						if (owner.type != "us") isuser = false;
					} else {
						owner = new Object();
						owner.type = "us";
						owner.name = "";
						owner.id = 0;
						val = "";
					}
				} else if (label == "Started") {
					var created = val;
					if (typeof created.value != "undefined") {
						var d = new Date(created.value * 1000);
						val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
					} else if (typeof created.name != "undefined") {
						var d = convertDate(created.name);
						val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
					} else {
						val = "";
					}
				} else if (label == "Finished") {
					$("#deprefresh").hide(); // Deployment Complete
					depComplete = true;
					var modified = val;
					if (typeof modified.value != "undefined") {
						var d = new Date(modified.value * 1000);
						val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
					} else if (typeof modified.name != "undefined") {
						var d = convertDate(modified.name);
						val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
					} else {
						val = "";
					}
				}
				
				if (label == "Deployed By") {
					td += "<tr><td class=\"summlabel\" style=\"width:30%\">";
					td += label;
					td += ":</td><td>";
					if (isuser) {
						td += "<img src=\"css/images/users_sm.png\"\ style=\"vertical-align:middle;\">&nbsp;" + val;
					} else {
						td += "<img src=\"css/images/groups_sm.png\"\ style=\"vertical-align:middle;\">&nbsp;" + val;
					}
				} else {
					var myid = label.toLocaleLowerCase().replace(" ", "") + "_sumrow";
					td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\" style=\"width:30%\">";
					td += label;
					td += ":</td><td>";
					td += val;
				}  
				td += "</td></tr>";
			}

			$("#" + tablename + " > tbody").html(td);
		},
		error : function(jqxhr, status, err) {
			console.log(status);
			console.log(err);
		}
	});
}




function GetNewsDetails(objid)
{
 objtypeAsInt = 99;
 tablename = "newssum";
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = "";
 summSaveobjid = objid;
 depComplete = false;
 
 LoadDeploymentSummaryData(objid);

 
  
  $.getJSON("GetDeploymentFilesData?f=d&id=" + objid, function(res) {
	   console.log(res);
	   var td = "<table id=\"deploymentdefects\" cellspacing=\"0\" width=\"100%\" >";
	   td += "<thead class=\"ui-widget-header\"><tr><th>CR ID</th><th>Description</th></tr></thead><tbody>";
	   var rowcnt = 0;
	   var length = 0;
	   if (typeof res.data != "undefined" && typeof res.data.length != "undefined") length = res.data.length;
	   for (i=0;i<length;i++) {
		   var row = res.data[i];
		   console.log("row");
		   console.log(row);
		   linkval="<a href=\""+row.htmlurl+"\" target=\"_blank\">"+row.id+"</a>";
		   td += "<tr><td style=\"border: 1px solid #ddd;\">" + linkval + "</td><td style=\"border: 1px solid #ddd;\">" + row.title + "</td></tr>";
	   }
	   td += "</tbody></table></div>";
	   $("#displaylogtabs-defects-data").html(td);
  });
  
  oldsubtabmenu[1]="displaylogtabs-files";
  RefreshLogDetail(objid);
 
  
  // ReplotDeployReports(objid);  
}



function getBuildLogDetails(compid,buildid)
{
	 objtypeAsInt = 26;
	 tablename = "newssum";
	 summSavetablename = tablename;
	 summSaveobjtypeAsInt = objtypeAsInt;
	 summSaveobjtype = "";
	 summSaveobjid = buildid;
	 
	 var url=(compid < 0)?"&jobid="+Math.abs(compid):"&compid="+compid;
	 
	 console.log("GetBuildLog?buildid=" + buildid +url);

	  $.getJSON("GetBuildLog?buildid=" + buildid +url, function(data) {

	     console.log(data);

	     $("#dbt-buildlog-data").html(data.buildlog.replaceAll("\n","<BR>"));
	     for (var a=0;a<data.comps.length;a++) {
	    	 var c = data.comps[a];
	    	 var dlink=(c.deploymentid>0)?"<a onClick=\"DisplayDeploy("+c.deploymentid+");\"><u>#"+c.deploymentid+"</u></a>":"";
	    	 td+="<tr><td style=\"border: 1px solid #ddd\">"+c.component.name+"</td><td style=\"border: 1px solid #ddd\"><a onClick=\"javascript:SwitchDisplay('se"+c.server.id+"');\"><u>"+c.server.name+"</u></a></td><td style=\"border: 1px solid #ddd\">"+dlink+"</td><td style=\"border: 1px solid #ddd\">"+c.dt+"</td></tr>";
	     }
	     $("#dbt-comp-tab > tbody").html(td);
	     var builder = data.details.builder;
	     var buildjob = data.details.buildjob;
	     var files = data.files;
	     var defects = data.defects;
	     if (typeof defects == "undefined") defects=[];
	     var dt = data.details.timestamp;
	     var dur = (data.details.duration / 1000);
	     if (files.length>0) {
	    	 var td="";
	    	 for (var b=0;b<files.length;b++) {
	    		 td+="<tr><td>"+files[b]+"</td></tr>";
	    	 }
	    	 $("#buildfiles > tbody").html(td);
	    	 $("#dbt-files").show();
	    	 $("#dbt-defects").show();
	     }
	     if (defects.length>0) {
	    	 var td="";
	    	 for (var c=0;c<defects.length;c++) {
	    		 var style=(c>0)?"vertical-align:top; border-top: 1px solid #ddd":"vertical-align:top;";
	    		 td+="<tr><td style=\""+style+"\"><a href=\""+defects[c].htmlurl+"\" target=\"_blank\">"+defects[c].bugid+"</a></td><td style=\""+style+"\">"+defects[c].title+"</td></tr>";
	    	 }
	    	 $("#builddefects > tbody").html(td);
	     }
	     var td="";
	     td="<tr><td class=\"summlabel\" style=\"width:30%\">Build Number:</td><td>"+buildid+"</td></tr>";
	     if (data.details.commit !="") {
	    	 td+="<tr><td class=\"summlabel\" style=\"width:30%\">Commit ID:</td><td>"+data.details.commit+"</td></tr>";
	     }
	     td+="<tr><td class=\"summlabel\" style=\"width:30%\">Date/Time:</td><td>"+dt+"</td></tr>";
	     td+="<tr><td class=\"summlabel\" style=\"width:30%\">Duration:</td><td>"+dur+" seconds</td></tr>";
	     td+="<tr><td class=\"summlabel\" style=\"width:30%\">Build Engine:</td>";
	     td+="<td><img src=\"css/images/buildengines_sm.png\"\ style=\"vertical-align:middle;\">&nbsp";
	     td+="<a onClick=\"javascript:SwitchDisplay('be"+builder.id+"');\">"+builder.name+"</a></td>";
	     td+="</tr>"
	     td+="<tr><td class=\"summlabel\" style=\"width:30%\">Build Job:</td>";
	     td+="<td><img src=\"images/buildjobs_sm.png\"\ style=\"vertical-align:middle;\">&nbsp";
	     td+="<a onClick=\"javascript:SwitchDisplay('bj"+buildjob.id+"');\">"+buildjob.name+"</a></td>";
	     td+="</tr>" 
	     $("#buildsum > tbody").html(td);
	  });  
}

function MaxLogDetail(objid)
{
 save_logdlg_height = $('.ui-dialog').height();
 save_logdlg_width  = $('.ui-dialog').width();
 
 if ($('#container').height()-90 > 100)
  $('.ui-dialog').height($('#container').height()-90);
 
 if ($('#container').width()-90 > 100)
  $('.ui-dialog').width($('#container').width()-90);
 
 $('.ui-dialog').top(50);
}  

function MinLogDetail(objid)
{
  $('.ui-dialog').height(save_logdlg_height);
  $('.ui-dialog').width(save_logdlg_width);
} 
 
function TogglePlayPause(objid)
{
	if ($("#autorefresh").is(':checked')) RefreshLogDetail(objid);
}

function RefreshLogDetail(objid)
{
	var logRefresh=$("#autorefresh").is(':checked');
	console.log("RefreshLogDetail("+objid+") logRefresh="+logRefresh);
	console.log("Tab="+oldsubtabmenu[1]);
	// Refresh data in current tab
	if (oldsubtabmenu[1]=="displaylogtabs-log") {
		$.getJSON("GetDeploymentLog?deployid=" + objid, function(data) {
			console.log("Refreshing deployment log");
			console.log(data);
			var td = "";
			for (i=0;i<data.log.length;i++) {
				var row = data.log[i];
				row.line = row.line.replace("<BR>","");
				if (row.stream == 2)
					td += "<tr><td style=\"color:#e00\" class=\"tid" + row.color + "\">" + row.line + "</td></tr>";
				else
					td += "<tr><td class=\"tid" + row.color + "\">" + row.line + "</td></tr>";
			}
			if (!data.complete) {
				td+="<tr><td><img src=\"images/spinner.gif\" /></td></tr>";
			}
			$("#deploymentlog").html(td);
			if (data.complete) {
				logRefresh = false;
				$("#deprefresh").hide();
			}
			if (logRefresh && !data.complete) {
				// Scroll to bottom of log
				var wtf = $('#displaylogtabs-log-data');
				if (typeof wtf[0] != "undefined") {
					var height = wtf[0].scrollHeight;
					wtf.scrollTop(height);
					clearTimeout(arTimerID);
					arTimerID = setTimeout(function() {
						RefreshLogDetail(objid);
					},8000);
				}
			}
		});
	}
	if (oldsubtabmenu[1]=="displaylogtabs-files") {
		$.getJSON("GetDeploymentFilesData?f=f&id=" + objid, function(res) {
			console.log("Refreshing deployment files");
			console.log(res);  
			var td = "<table id=\"deploymentfiles\" cellspacing=\"0\" width=\"100%\" >";
			td += "<thead class=\"ui-widget-header\"><tr><th>Step</th><th>Repository</th><th>Repository Path</th><th>Version</th><th>Component</th><th>Build</th><th>Target</th><th>Target Path</th><th>MD5</th></tr></thead><tbody>";
			var rowcnt = 0;
			for (i=0;i<res.data.length;i++) {
				var row = res.data[i];
				var step = row[0].name;
				var repo = row[1].name;
				var repopath = row[2];
				var version = row[3];
				var component = row[4].name;
				var target = row[5].name;
				var targetpath = row[6];
				for (var z=0;z<targetpath.length;z++) {
					if (targetpath.charAt(z)=='/') {
						targetpath=targetpath.replaceAll("\\\\","/");
						break;
					}
					else
					if (targetpath.charAt(z)=='\\') {
						targetpath=targetpath.replaceAll("/","\\");
						break;
					}
				}
				var md5 = row[7];
				var buildno = (row[8]==0)?"":"#"+row[8];
				td += "<tr><td style=\"border: 1px solid #ddd;\">" + step + "</td><td style=\"border: 1px solid #ddd;\">" + repo + "</td><td style=\"border: 1px solid #ddd;\">" + repopath + "</td><td style=\"border: 1px solid #ddd;\">" + version + "</td><td style=\"border: 1px solid #ddd;\">" + component + "</td><td style=\"border: 1px solid #ddd;\">" + buildno + "</td><td style=\"border: 1px solid #ddd;\">" + target + "</td><td style=\"border: 1px solid #ddd;\">" + targetpath + "</td><td style=\"border: 1px solid #ddd;\">"+md5+"</td></tr>";
			} 
			for (rowcnt; rowcnt < 5; rowcnt++) {
				td += "<tr><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";   
			}
			td += "</tbody></table></div>";
			$("#displaylogtabs-files-data").html(td);
			if (res.complete) {
				logRefresh = false;
				$("#deprefresh").hide();
			}
			if (logRefresh && !res.complete) {
				clearTimeout(arTimerID);
				arTimerID = setTimeout(function() {
					RefreshLogDetail(objid);
				},8000);
			}
		});
	}
	
	if (oldsubtabmenu[1]=="displaylogtabs-general") {
		LoadDeploymentSummaryData(objid);
		if (logRefresh && !depComplete) {
			clearTimeout(arTimerID);
			arTimerID = setTimeout(function() {
				RefreshLogDetail(objid);
			},8000);
		} 
	}
	
	if (oldsubtabmenu[1]=="displaylogtabs-scripts") {
		$.getJSON("GetDeploymentFilesData?f=s&id=" + objid, function(res) {
			console.log(res);
			var td = "<table id=\"deploymentscripts\" cellspacing=\"0\" width=\"100%\" >";	   
			td += "<thead class=\"ui-widget-header\"><tr><th>Step</th><th>Action/Procedure/Function</th></tr></thead><tbody>";
			var rowcnt = 0;   
			for (i=0;i<res.data.length;i++) {
				var row = res.data[i];
				var step = row.step;
				var action = row.action;
				var otid=(action.type=="ac")?action.type+action.id:action.type+action.id+"-"+action.kind;
				var img="action.png";
				if (action.type=="pr") img="functions_procedures.png";
				if (action.type=="fn") img="functions_procedures.png";
				linkval="<span style=\"position:relative; top:-3px;\"><img src=\"css/images/"+img+"\" style=\"position:relative; top: 3px; padding-right:5px\"><a href='javascript:SwitchDisplay(\""+otid+"\");'>"+action.name+"</a></span>";
				td += "<tr><td style=\"border: 1px solid #ddd;\">" + step + "</td><td style=\"border: 1px solid #ddd;\">" + linkval + "</td></tr>";
			}
			td += "</tbody></table></div>";
			$("#displaylogtabs-scripts-data").html(td);
			if (logRefresh && !res.complete) {
				clearTimeout(arTimerID);
				arTimerID = setTimeout(function() {
					RefreshLogDetail(objid);
				},8000);
			}
		});	
	}
}
