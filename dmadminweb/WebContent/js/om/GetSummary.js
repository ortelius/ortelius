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
	var pwd = parent.$("#modal");
	var custid = $("#customaction_val").val();
	if (custid!="") {	
		pwd.find("#repository_row").hide();
		pwd.find("#rb_repository_row").hide();
		pwd.find("#rf_repository_row").hide();
		pwd.find("#summtab").find("tr.repoprops").remove();
		pwd.find("#summtab").find("tr.rf_repoprops").remove();
		pwd.find("#summtab").find("tr.rb_repoprops").remove();
	} else {
		CompTypeChanged();
	}
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

function LoadBuildData(tablename, objtypeAsInt, objtype, objid, addParams)
{
	var colours=["#dddddd","#eeeeee"];
	var c=0;

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
		    		 var success;
		    		 if (res.builds[i].result == "SUCCESS") {
		    			 icon = "<IMG src=\"images/blue_small.png\" style=\"vertical-align:middle;\">&nbsp;";
		    			 success=true;
		    		 } else {
		    			 icon = "<IMG src=\"images/red_small.png\" style=\"vertical-align:middle;\">&nbsp;";
		    			 success=false;
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
		    		 td+="<tr style=\"background-color:"+colours[c]+"\"><td>"+icon+"<a onClick=\"javascript:DisplayBuild(-"+objid+","+res.builds[i].id+","+success+");\">#"+res.builds[i].id+"</a></td><td>"+res.builds[i].timestamp+"</td><td>"+complink+"</td></tr>";
		    		 c=c?0:1;
		    	 }
		    	 $("#" + tablename + " > tbody").html(td);
		     }
		 }
	});
}


function LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 repolabeladded = false;

 console.log("LoadSummaryData, tablename="+tablename);
 console.trace();
 console.log("GetSummaryData?objtype=" + objtypeAsInt + "&id=" + objid + addParams);
 $
   .ajax(
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
     tdedit = "<form id=\"summform\"><table id=\"summtab\" class=\"summtable\">";
     var comptypedone = false;
     var comptypelabeldone = false;
     // summAutoPing = false;
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

      if (label == "Summary")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"summary_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"summary_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Name")
      {
       objName = val;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"name_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"name_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"name_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Availability")
      {
    	  tdedit += "<tr>";
          tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"availability_val\" id=\"availability_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"filename_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"usernameenc_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"usernameenc_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"usernameenc_oldval\" value=\"" + val + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
    	   // Default - CRED_VARUSERNAME
        tdedit += "<tr id=\"username_row\">";
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Username:</td>";
        tdedit += "<td><input name=\"username_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"groupname_val\" style='width:24em' "+ro+" type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Command Line Interpreter")
      {
       tdedit += "<tr id=\"interpreter_row\" style=\"display:none\">";       
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"interpreter_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"interpreter_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }  
      else if (label == "Real Name")
      {
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"realname_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"realname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Email")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"email_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"email_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Phone")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"phone_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + "</td>";
        tdedit += "<td><input name=\"passwordenc_val\" style='width:24em' type=\"password\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_oldval\" /></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<tr id=\"password_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
        tdedit += "<td><input name=\"password_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"password_oldval\" /></td>";
        tdedit += "</tr>";
       }
      }
      else if (label == "Hostname")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"hostname_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"hostname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "SSH Port Number")
      {
       tdedit += "<tr id=\"sshport_row\" style=\"display:none\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"sshport_val\" style='width:8em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"sshport_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Protocol")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select onChange=\"ProtocolChanged()\"; name=\"protocol_val\" id=\"protocol_val\">";
       summProtocol = val;
       if (val == "ftp")
       {
        val = "ftp";
        tdedit += "<option value=\"ftp\" selected>ftp</option>";
        tdedit += "<option value=\"ftps\">ftps</option>";
        tdedit += "<option value=\"sftp\">sftp</option>";
        tdedit += "<option value=\"win\">win</option>";
       }
       else if (val == "ftps")
       {
        val = "ftps";
        tdedit += "<option value=\"ftp\">ftp</option>";
        tdedit += "<option value=\"ftps\" selected>ftps</option>";
        tdedit += "<option value=\"sftp\">sftp</option>";
        tdedit += "<option value=\"win\">win</option>";
       }
       else if (val == "sftp")
       {
        val = "sftp";
        tdedit += "<option value=\"ftp\">ftp</option>";
        tdedit += "<option value=\"ftps\">ftps</option>";
        tdedit += "<option value=\"sftp\" selected>sftp</option>";
        tdedit += "<option value=\"win\">win</option>";
       }
       else if (val == "win")
       {
        val = "win";
        tdedit += "<option value=\"ftp\">ftp</option>";
        tdedit += "<option value=\"ftps\">ftps</option>";
        tdedit += "<option value=\"sftp\">sftp</option>";
        tdedit += "<option value=\"win\" selected>win</option>";
       }
       else
       {
        val = "";
        tdedit += "<option value=\"ftp\">ftp</option>";
        tdedit += "<option value=\"ftps\">ftps</option>";
        tdedit += "<option value=\"sftp\">sftp</option>";
        tdedit += "<option value=\"win\" selected>win</option>";
       }

       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"protocol_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Engine")
      {
       var engine = val;
       if (typeof engine.name == "undefined")
       {
        engine = new Object();
        engine.name = "";
        engine.type = "ei";
        engine.id = 0;
       }
       val = engine.name;
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"domain_val\">";
       save_domain_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_field\" value=\"" + tasktype + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"domain_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "End Point Component Types")
      {
       if (comptypedone == false)
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select multiple name=\"servercomptype_val\">";
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
          tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"taskaction_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"linkedtask_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"port_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"port_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Base Directory")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"basedirectory_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"basedirectory_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Display Name")
      {
       tdedit += "<tr id=\"fragmentname_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"fragmentname_val\" type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fragmentname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Repository")
      {
    	  console.log("Repository Label Seen");
       var repo = val;
       if (typeof repo.name == "undefined")
       {
        repo = new Object();
        repo.name = "";
        repo.type = "re";
        repo.id = 0;
       }
       val = repo.name;
       console.log(repo);
       var otid="re"+repo.id;
       linkval=repo.showlink?"<a href='javascript:SwitchDisplay(\""+otid+"\");'>"+val+"</a>":val;
       
       tdedit += "<tr id=\"repository_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><select name=\"repository_val\"/></td>";
       save_repository_val = repo.name;
       save_repository_id = repo.id;
       tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"" + repo.type + repo.id + "\"/></td>";
       tdedit += "</tr>";
       repolabeladded = true;
      }
      else if (label == "Filepath")
      {
       tdedit += "<tr id=\"filepath_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"filepath_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filepath_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
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

       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Owner Type:</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"ownertype_val\" name=\"ownertype_val\">";
       if (isuser)
       { 
        tdedit += "<option value=\"User\" selected>User</option>";
        tdedit += "<option value=\"Group\">Group</option>";
       }
       else
       { 
        tdedit += "<option value=\"User\">User</option>";
        tdedit += "<option value=\"Group\" selected>Group</option>";
       } 
       tdedit += "</select></td>";
       tdedit += "<td><input type=\"hidden\" name=\"ownertype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"ownertype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"ownertype_oldval\" value=\"" + owner.type + "\"/></td>";
       tdedit += "</tr>";
       
       tdedit += "<tr id=\"owner_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Users:</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"owner_val\">";
       save_owner_val = owner.name;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"owner_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"owner_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"owner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
       tdedit += "</tr>";
       
       tdedit += "<tr id=\"group_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Groups:</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"groupowner_val\">";
       save_groupowner_val = owner.name;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupowner_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupowner_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupowner_oldval\" value=\"" + owner.type + owner.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Parent Domain")
      {
       var domain = val;
       val = domain.value;
       if (val == "")
        val = "This is the top level domain";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"parentdomain_val\" style=\"padding-top:2px;padding-bottom:2px;\">" + val + "</td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val || objid == -1)
        tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input id=\"alwaysdeploy_val\" name=\"alwaysdeploy_val\" style='width:24em' type=\"checkbox\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val || objid == -1)
        tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input id=\"deployseq_val\" name=\"deployseq_val\" style='width:24em' type=\"checkbox\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input id=\"database_val\" name=\"database_val\" style='width:24em' type=\"checkbox\"/ checked></td>";
       else
        tdedit += "<td><input id=\"database_val\" name=\"database_val\" style='width:24em' type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"database_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
       
       val=val?"Yes":"No";
      }
      else if (label == "Delete Corresponding Directory")
      {
       var deletedir = val;
       val = deletedir.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input id=\"deletedir_val\" name=\"deletedir_val\" style='width:24em' type=\"checkbox\" checked/></td>";
       else
        tdedit += "<td><input id=\"deletedir_val\" name=\"deletedir_val\" style='width:24em' type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"deletedir_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"deletedir_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"deletedir_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
       
       val=val?"Yes":"No";
      }      
      else if (label == "LifeCycle Domain")
      {
       var domain = val;
       val = domain.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Contains Lifecycles:</td>";
       if (val)
       {
        summSaveisLifeCycle = true;
        isLifeCycle = true;
        tdedit += "<td><span id=\"lifecycledomain_txt\" style=\"display:none\">Yes</span><input name=\"lifecycledomain_val\" id=\"lifecycledomain_val\" style='width:24em' onClick=\"javascript:isLifeCycle=false;\" type=\"checkbox\" checked /></td>";
       }
       else
       {
        summSaveisLifeCycle = false;
        isLifeCycle = false;
        tdedit += "<td><span id=\"lifecycledomain_txt\" style=\"display:none\">Yes</span><input name=\"lifecycledomain_val\" id=\"lifecycledomain_val\" style='width:24em' onClick=\"javascript:isLifeCycle=true;\" type=\"checkbox\"/></td>";
       }
       tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"lifecycledomain_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Result Is Expr")
      {
       var resultisexpr = val;
       val = resultisexpr.value;
       tdedit += "<tr id=\"resultisexpr_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"resultisexpr_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"resultisexpr_val\" style='width:24em' type=\"checkbox\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"copytoremote_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"copytoremote_val\" style='width:24em' type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"copytoremote_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"copytoremote_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"copytoremote_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Show Output")
      {
       var showoutput = val;
       val = showoutput.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"showoutput_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"showoutput_val\" style='width:24em' type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"showoutput_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"showoutput_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"showoutput_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"showoutput_oldval\" value=\"false\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Available in SubDomains")
      {
       var subdomains = val;
       val = subdomains.value;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"subdomains_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"subdomains_val\" style='width:24em' type=\"checkbox\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"accountlocked_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"accountlocked_val\" style='width:24em' type=\"checkbox\"/></td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (val)
        tdedit += "<td><input name=\"forcechangepassword_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"forcechangepassword_val\" style='width:24em' type=\"checkbox\"/></td>";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"filterlevel_val\" id=\"filterlevel_val\" onChange=\"FilterLevelChanged()\">";
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
      else if (label == "Component Type" && summSaveobjtype != "se")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"comptype_val\" id=\"comptype_val\" onChange=\"CompTypeChanged()\">";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";

       save_comptype_val = val;
      } 
      else if (label == "Component Type"  && summSaveobjtype == "se")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select multiple name=\"comptype_val\" id=\"comptype_val\" onChange=\"CompTypeChanged()\">";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"comptype_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";

       save_comptype_val = val;
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
          tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"buildjob_val\">";
          tdedit += "<option value=\"\"></option></select>";
          save_buildjob_val = val;
          tdedit += "</td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_field\" value=\"" + field + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_callback\" value=\"" + callback + "\"/></td>";
          tdedit += "<td><input type=\"hidden\" name=\"buildjob_oldval\" value=\"" + oldval + "\"/></td>";
          tdedit += "</tr>";
      }
      else if (label == "Last Build Number") {
    	  if (val==0) val="";
    	  else val="<a onClick=\"javascript:DisplayBuild("+objid+","+val+");\">#"+val+"</a>";
      }
      else if (label == "Project Name" || label == "Plan Name") {
    	  var project = val;
    	  var oldval=val;
       
          tdedit += "<tr>";
          tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"projectname_val\">";
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
       
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"rollup_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"rollback_val\">";
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
      else if (label == "End Point Type")
      {
       var st = val;
       val = "st" + st.id;

       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"servertype_val\">";
       if (val == "st3")
       {
        oldval = val;
        val = "as400";
        tdedit += "<option value=\"st3\" selected>as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";
       }
       else if (val == "st4")
       {
        oldval = val;
        val = "mac";

        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\" selected>mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";        
       }
       else if (val == "st1")
       {
        oldval = val;
        val = "unix";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\" selected>unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";        
       }
       else if (val == "st5")
       {
        oldval = val;
        val = "Tandem";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\" selected>Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";        
       }
       else if (val == "st6")
       {
        oldval = val;
        val = "Stratus";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\" selected>Stratus</option>";
        tdedit += "<option value=\"st7\">OpenVMS</option>";        
       }
       else if (val == "st7")
       {
        oldval = val;
        val = "OpenVMS";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
        tdedit += "<option value=\"st5\">Tandem</option>";
        tdedit += "<option value=\"st6\">Stratus</option>";
        tdedit += "<option value=\"st7\" selected>OpenVMS</option>";        
       }
       else
       {
        oldval = val;
        val = "windows";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\" selected>windows</option>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"preaction_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"postaction_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (summSaveobjtype == "co") {
    	   tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"customaction_val\" id=\"customaction_val\" onChange=\"clearItemOptions();\">";
       } else {
    	   tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"customaction_val\" id=\"customaction_val\">";
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
       val = cred.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"credential_val\">";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><input name=\"category_val\" selectBoxOptions=\"\" />";
       save_category_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"category_oldval\" value=\"" + category.type + category.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Successful Deployment Template" || 
    		   label == "Approved Notify Template" ||
    		   label == "Success Notification Template" ||
    		   label == "Request Notification Template"
    		   )
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"template_val\">";
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
    		   label == "Failure Notification Template"
    		   )
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"fail_template_val\">";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"type_val\">";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"type_val\">";
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"kind_val\" id=\"kind_val\" onChange=\"KindChanged()\">";
       save_kind_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"kind_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"kind_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"kind_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
       }
       
       console.log("save_kind_id="+save_kind_id+" objid="+objid);
       
       save_kind_id = kind.id;
       
       console.log("kind.id="+kind.id);
       objkind = kind.id;
       
       
       if (currenttree == "#procedures_tree" || currenttree == "#functions_tree")
       { 
       switch (Number(kind.id))
       {
        case 0:
         $("#tabs-Args").hide();
         $("#tabs-ProcBody").hide();
         break;

        case 1: // DMScript Function in repository
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
      }
      else if (label == "Created")
      {
       var created = val;
       if (typeof created.when != "undefined")
       {
        var d = convertDate(created.when);
        val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
        val += " by " + created.tooltip;
       }
       else
        val = "";
      }
      else if (label == "Modified")
      {
       var modified = val;
       console.log("modified (Modified)");
       console.log(modified);
       if (typeof modified.when != "undefined")
       {
        var d = convertDate(modified.when);
        val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
        val += " by " + modified.tooltip;
       }
       else
        val = "";
      }
      else if (label == "Last Login")
      {
       var modified = val;
       if (typeof modified.name != "undefined")
       {
        var d = convertDate(modified.name);
        console.log("FullYear = "+d.getFullYear());
        if (d.getFullYear() < 2000) {
        	val = "";
        } else {
        	val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
        }
       }
       else
        val = "";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"lastlogin_val\" style=\"padding-top:2px;padding-bottom:2px;\">" + val + "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"lastlogin_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      
      if (label == "Owner")
      {
       td += "<tr><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       if (isuser)
        td += "<img src=\"css/images/user_16x.png\"\ style=\"vertical-align:middle;\">&nbsp;" + val;
       else
        td += "<img src=\"css/images/group_16x.png\"\ style=\"vertical-align:middle;\">&nbsp;" + val;
      }
      else if (	label == "LifeCycle Domain" || 
    		  	label == "Copy to Remote" || 
    		  	label == "Always Deploy" || 
    		  	label == "Deploy Sequentially" ||
    		  	label == "Result Is Expr" || 
    		  	label == "Show Output" || 
    		  	label == "Account Locked" || 
    		  	label == "Force Change Password" ||
    		  	label == "Available in SubDomains")
      {
    	  if (label == "LifeCycle Domain") label = "Contains Lifecycles";
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
       if (val.indexOf("Function") >= 0)
       {
        td += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\">";
        td += "Description:";
        td += "</td><td>";
        td += "This function does return a value";
        td += "</td></tr>";
       }
       if (val.indexOf("Procedure") >= 0)
       {
        td += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\">";
        td += "Description:";
        td += "</td><td>";
        td += "This procedure does not return a value";
        td += "</td></tr>";
       }    
       td += "<tr id='kind_sumrow'><td id=\"kind_summ\" class=\"summlabel\">";
       td += label;
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
      else if (label == "End Point Component Types" )
      {
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

     if (allocate_terminal_shown) tdedit += "<tr id=\"atfootnote\"><td colspan=\"2\">* Only for sftp targets</td></tr>";

     if (repolabeladded == false && summSaveobjtype == "co" && objid < 0)
     {
      label = "Repository";
      tdedit += "<tr id=\"repository_row\">";
      tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
      tdedit += "<td><select name=\"repository_val\" id=\"repository_val\" onChange=\"ToggleRepoProps(-1,'repository_row',false,true)\" /></td>";
      save_repository_val = "";
      current_repository_val = save_repository_val;
      tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"704\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"\"/></td>";
      tdedit += "</tr>";
      
      label = "Roll Foward Repository";
      tdedit += "<tr id=\"rf_repository_row\" style=\"display:none\">";
      tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
      tdedit += "<td><select name=\"rf_repository_val\" id=\"rf_repository_val\" onChange=\"ToggleRepoProps(-1,'rf_repository_row',false,true)\" /></td>";
      save_repository_val = "";
      current_repository_val = save_repository_val;
      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_field\" value=\"704\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_callback\" value=\"" + callback + "\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"rf_repository_oldval\" value=\"\"/></td>";
      tdedit += "</tr>";
      
      label = "Rollback Repository";
      tdedit += "<tr id=\"rb_repository_row\"  style=\"display:none\">";
      tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
      tdedit += "<td><select name=\"rb_repository_val\" id=\"rb_repository_val\" onChange=\"ToggleRepoProps(-1,'rb_repository_row',false,true)\" /></td>";
      save_repository_val = "";
      current_repository_val = save_repository_val;
      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_field\" value=\"704\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_callback\" value=\"" + callback + "\"/></td>";
      tdedit += "<td><input type=\"hidden\" name=\"rb_repository_oldval\" value=\"\"/></td>";
      tdedit += "</tr>";
     } 

     if (objtype == "do") {
    	 var sdl="";
    	 $.ajax({
    		 url : "SubDomains?domainid=" + objid,
    		 dataType : 'json',
    		 type : 'GET',
    		 success : function(res) {
    			 if (res.length != 0) {
    				 sdl += "<tr>";
    				 sdl += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">Sub Domains:</td>";
    				 if (isLifeCycle) {
    					 sdl += "<td style=\"padding-top:2px;padding-bottom:2px;background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
    				 } else {
    					 sdl += "<td style=\"padding-top:2px;padding-bottom:2px;background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
    				 }
    			 }
    			 var tmp = " class=\"islifecycle\"";
    			 if (!isLifeCycle) tmp = "";

    			 for (var k = 0; k < res.length; k++) {
    				 sdl += "<li id=\"dom_" + res[k].id + "\"" + tmp + "><span style=\"cursor: pointer;\" >" + res[k].name + "</span></li>";
    				 if (k == 0) {
    					 td += "<tr><td>Sub Domains:</td><td>" + res[k].name + "</td></tr>";
    				 } else {
    					 td += "<tr><td>&nbsp;</td><td>" + res[k].name + "</td></tr>";
    				 }
    			 }
    			 if (res.length != 0) {
    				 sdl += "</ul>";
    				 sdl += "</td>";
    				 sdl += "<td></td>";
    				 sdl += "<td></td>";
    				 sdl += "<td></td>";
    				 sdl += "</tr>";
    				 sdl += "<tr id=\"dndmsg\"><td colspan=\"5\" style=\"padding-top:8px;\"><i>Drag and drop the Sub Domains to order them for your lifecycle model.</i></td></tr>";
    				 sdl += "</table></form>";
    			 }
    			 $("#" + tablename + " > tbody").html(td);
    			 tdedit += sdl;
        
    			 HideFields();

    			 if (objid < 0) {
    				 EditSummaryButton(false);
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
      
    	 HideFields();

    	 if (objid < 0) {
    		 console.log("objid="+objid+" opening editing dialog (creation?)");
    		 EditSummaryButton(false);
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


function HideFields()
{
	console.log("HideFields - summSaveobjtype="+summSaveobjtype+" summProtocol="+summProtocol);
	if (summSaveobjtype == "se") {
		/*
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
		*/
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



function EditBodyButton()
{
	console.log("EditBodyButton");
	var pwd = parent.$("#modal");
	var buttons = [
	{
		text : "Ok",
		click : function() {
			var subject = $("#templatesubject").val();
			var body = $("#templatebody").val();
			console.log("OK Clicked: summSaveobjid="+summSaveobjid);
			console.log("saving subject ["+subject+"]");
			console.log("saving body ["+encodeURI(body)+"]");
			console.log("escaped is ["+convertForDisplay(body)+"]");
			$.ajax({
			url : "TemplateDetails?f=seb&id="+summSaveobjid+"&sub="+subject+"&b="+encodeURI(body),
			dataType : 'json',
			type : 'POST',
			success : function(res) {
				$("#bodytable > thead").html("<tr><td>Subject: "+convertForDisplay(subject)+"</td></tr>");
				$("#bodytable > tbody").html("<tr><td>&nbsp;</td></tr><tr><td>"+convertForDisplay(body)+"</td></tr>");
				parent.$("#modal").dialog("close");
				summSaveSubject = subject;
				summSaveBody = body;
			}
			});
		}
	},
	{
		text : "Cancel",
		click : function() {
			parent.$("#modal").dialog("close");
			if (summSaveobjid < 0)
	        $("#panel_container.right").hide();
		}
	}];
	pwd.empty();
	pwd.html(
	"<table style=\"width:100%; height:100%; padding:20px\"><tr style=\"vertical-align:top\"><td style=\"width:100px; height:30px\">Subject:</td><td><input type=\"text\" id=\"templatesubject\" name=\"subject\" style=\"width:100%\"></td></tr>"
	+"<tr style=\"vertical-align:top\"><td style=\"width:100px;\">Body:</td><td><textarea style=\"overflow: auto; width:100%; height:100%\" id=\"templatebody\"name=\"body\"></textarea></td></tr></table>"
	);
	pwd.dialog({modal : true, open: null});
	pwd.dialog("option", "title", "Edit Template Content");
	pwd.dialog("option", "height", "350");
	pwd.dialog("option", "minHeight", "350");
	pwd.dialog("option", "width", "600");
	pwd.dialog("option", "buttons", buttons);
	$("#templatesubject").val(summSaveSubject);
	$("#templatebody").val(summSaveBody);
	pwd.dialog('open');
}

function EditSummaryButton(isProfile)
{
	console.log("EditSummaryButton isProfile="+isProfile);
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
	  console.log("OK Clicked: summSaveobjtype="+summSaveobjtype+" summSaveobjid="+summSaveobjid);
   var name = "";
   var form = pwd.find("#summform");
   
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
    
   if (summSaveobjid < 0)
   {
    var namefld = pwd.find(":input[name=\"name_val\"]");

    if (typeof namefld !== undefined)
     name = namefld.val();

    var f = pwd.find("#summform");
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
     NewID("Template","TEMPLATE", currenttree, "", name);
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
     NewID("buildjob", "BUILDJOB", currenttree, "", name);
     break;

    default:
     break;
    }
   }
   if (isProfile) {
	   SaveSummaryData(parent.$("#modal"), profSavetablename, profSaveobjtypeAsInt, profSaveobjtype, profSaveobjid, profSaveaddParams,true);
   } else {
	   if (summSaveobjid != -1)
		   SaveSummaryData(parent.$("#modal"), summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams,false);
   }
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   parent.$("#modal").dialog("close");
   if (summSaveobjid < 0)
    $("#panel_container.right").hide();
  }
 } ];

 pwd.empty();
 pwd.html(isProfile?tdedit2:tdedit);
 pwd.dialog(
 {
  modal : true,
  open : function(event, ui)
  {
	  $("#summ tr").each(function( index ) {
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

    if (cnt > 0)
    {
     var lifecycle = pwd.find("#lifecycledomain_val");
     console.log(lifecycle);
     lifecycle.hide();
     pwd.find("#lifecycledomain_txt").show();
    }
   }
   else
   {
    pwd.find("#dndmsg").hide();

    var sdl = pwd.find("#subdomainlist");
    var cnt = sdl.children().length;

    if (cnt > 0)
    {
     var lifecycle = pwd.find("#lifecycledomain_val");
     console.log(lifecycle);
     lifecycle.hide();
     var lcd = pwd.find("#lifecycledomain_txt");
     lcd.html("No");
     lcd.show();
    }
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

   var myform = pwd.find("#summform");

   if (myform.find(":input[name=\"preaction_val\"]").length > 0)
   {
	   console.log("About to GetActions");
    $.ajax(
    {
     url : "GetActions?reason=actonly&objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      console.log(res);
      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var preaction = myform.find(":input[name=\"preaction_val\"]");
      var postaction = myform.find(":input[name=\"postaction_val\"]");
      var customaction = myform.find(":input[name=\"customaction_val\"]");
      var taskaction = myform.find(":input[name=\"taskaction_val\"]");

      for (n = 0; n < res.length; n++)
      {
    	var action = res[n].action;
        if (save_preaction_val == action.id)
        preaction.append('<option id="preact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
       else
        preaction.append('<option id="preact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');

       if (save_postaction_val == action.id)
        postaction.append('<option id="postact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
       else
        postaction.append('<option id="postact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');

       if (save_customaction_val == action.id)
        customaction.append('<option id="customact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
       else
        customaction.append('<option id="customact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');

       if (save_taskaction_val == action.id)
           taskaction.append('<option id="customact' + n + '" selected value=\"' + action.type + action.id + "\">" + action.name + '</option>');
          else
           taskaction.append('<option id="customact' + n + '" value=\"' + action.type + action.id + "\">" + action.name + '</option>');
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
    	 parent.$("#modal").dialog({"width":"auto"});
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
      var myform = pwd.find("#summform");
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
       comptype.prop('selectedIndex', 0);
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
      var myform = pwd.find("#summform");
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
   
   /*
   if (myform.find(":input[name=\"autoping_val\"]").length > 0)
   {
     $("#autoping_val").change(ToggleAutoPing);
     ToggleAutoPing();
   }
   */
   

   
   if (myform.find(":input[name=\"datasource_val\"]").length > 0)
   {
	   $.ajax({
		   url : "GetProviderDefs?f=ldap&id="+summSavedomid,
		   dataType : 'json',
		   type : 'GET',
		   success : function(res)
		   {
			   var myform = pwd.find("#summform");
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
   if (myform.find(":input[name=\"projectname_val\"]").length > 0)
   {
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
				   var datasource = myform.find(":input[name=\"projectname_val\"]");
				   var cnt = 0;
	
				   for (n = 0; n < res.jobs.length; n++)
				   {
					   if (save_projectname_val == res.jobs[n].name)
					   {
						   datasource.append('<option id="project' + n + '" selected value=\"' + res.jobs[n].name + "\">" + res.jobs[n].name + '</option>');
						   cnt++;
					   } 
					   else
						   datasource.append('<option id="project' + n + '" value=\"' + res.jobs[n].name + "\">" + res.jobs[n].name + '</option>');
				   }
				   if (cnt == 0) datasource.prop('selectedIndex', 0);
			   }
		   },
		   error : function(jqxhr, status, err)
		   {
			   console.log(status);
			   console.log(err);
		   }
		});
   }
   if (myform.find(":input[name=\"bt_datasource_val\"]").length > 0)
   {
	   $.ajax({
		   url : "GetProviderDefs?f=bt&id="+summSavedomid,
		   dataType : 'json',
		   type : 'GET',
		   success : function(res)
		   {
			   var myform = pwd.find("#summform");
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
			   var myform = pwd.find("#summform");
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
      var myform = pwd.find("#summform");
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
      var myform = pwd.find("#summform");
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
          var myform = pwd.find("#summform");
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
   
   

   if (myform.find(":input[name=\"repository_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetRepositories?objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var owner = myform.find(":input[name=\"repository_val\"]");
      
      console.log("save_repository_val=["+save_repository_val+"]");

      for (n = 0; n < res.length; n++)
      {
       // if (save_repository_val == res[n].name)
       if (save_repository_id == res[n].id)
        owner.append('<option id="repository' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="repository' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
      if (summSaveobjid < 0)
       ToggleRepoProps(-1,"repository_row",true);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }

   if (myform.find(":input[name=\"rf_repository_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetRepositories?objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var owner = myform.find(":input[name=\"rf_repository_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_repository_val == res[n].name)
        owner.append('<option id="rf_repository' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="rf_repository' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }
   
   if (myform.find(":input[name=\"rb_repository_val\"]").length > 0)
   {
    $.ajax(
    {
     url : "GetRepositories?objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var owner = myform.find(":input[name=\"rb_repository_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_repository_val == res[n].name)
        owner.append('<option id="rb_repository' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="rb_repository' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
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
      var myform = pwd.find("#summform");
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
      var myform = pwd.find("#summform");
      var typeobj = myform.find(":input[name=\"type_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_type_val == res[n].name)
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
      var myform = pwd.find("#summform");
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
		var myform = pwd.find("#summform");
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

   if (summSaveobjtype == "cr")
   {
    var myform = pwd.find("#summform");
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
      name : 'DMScript Function in database'
     },
     {
      type : 'ak',
      id : 3,
      name : 'Function provided by local external script or program'
     },
     {
      type : 'ak',
      id : 4,
      name : 'Function provided by remote external script or program'
     } ];
     
     
     desc += "<tr id='kind_sumrow_desc' ><td id=\"kind_summ_desc\" class=\"summlabel\" style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">";
     desc += "Description:";
     desc += "</td><td>";
     desc += "This function does return a value";
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
      name : 'DMScript Procedure in database'
     },
     {
      type : 'ak',
      id : 3,
      name : 'Procedure provided by local external script or program'
     },
     {
      type : 'ak',
      id : 4,
      name : 'Procedure provided by remote external script or program'
     } ];

     desc += "<tr id='kind_sumrow_desc'><td id=\"kind_summ_desc\" class=\"summlabel\" style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">";
     desc += "Description:";
     desc += "</td><td>";
     desc += "This procedure does not return a value";
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
   pwd.dialog("option", "width", "auto");	// this cuts off right edge of drop-downs
   pwd.dialog("option", "height","auto");
  }
 });
 
	
 var title = "General";

 if (isProfile)
  title = "Profile";
 
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
   objstr = "End Point";

   title = "New " + objstr;
 }

 pwd.dialog("option", "title", title);
 pwd.dialog("option", "width", pwd.outerWidth()+30);
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');
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

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view =
 {};

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
 
 /*
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
*/
 
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
  ret = true;
 }

 if (view.filename_val != view.filename_oldval)
 {
  console.log(prefix + 'change_' + view.filename_field + ' = ' + view.filename_val);
  data[prefix + 'change_' + view.filename_field] = view.filename_val;
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
  ret = true;
 }

 if (view.passwordenc_val != view.passwordenc_oldval)
 {
  console.log(prefix + 'change_' + view.passwordenc_field + ' = ' + view.passwordenc_val);
  data[prefix + 'change_' + view.passwordenc_field] = view.passwordenc_val;
  ret = true;
 }

 if (view.usernameenc_val != view.usernameenc_oldval)
 {
  console.log(prefix + 'change_' + view.usernameenc_field + ' = ' + view.usernameenc_val);
  data[prefix + 'change_' + view.usernameenc_field] = view.usernameenc_val;
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
  console.log(prefix + 'change_' + view.comptype_field + ' = ' + view.comptype_val.substring(2));
  data[prefix + 'change_' + view.comptype_field] = view.comptype_val.substring(2);
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

 if (view.kind_val != view.kind_oldval)
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
 
 /*
 if (view.automd5check_val != view.automd5check_oldval)
 {
  console.log(prefix + 'change_' + view.automd5check_field + "=" + view.automd5check_val);
  data[prefix + 'change_' + view.automd5check_field] = view.automd5check_val;
  ret = true;
 }
 */
 
 if (view.buildjob_val != view.buildjob_oldval)
 {
  console.log(prefix + 'change_' + view.buildjob_field + "=" + view.buildjob_val);
  data[prefix + 'change_' + view.buildjob_field] = view.buildjob_val;
  ret = true;
 }
 
 /*
 if (view.autoping_val != view.autoping_oldval)
 {
  console.log(prefix + 'change_' + view.autoping_field + "=" + view.autoping_val);
  data[prefix + 'change_' + view.autoping_field] = view.autoping_val;
  ret = true;
 }
 */
		 
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
 
 //console.log("pinginterval_val="+view.pinginterval_val);
 //console.log("pinginterval_oldval="+view.pinginterval_oldval);
 
 /*
 if (view.pinginterval_val != view.pinginterval_oldval)
 {
	 console.log(prefix + 'change_' + view.pinginterval_field + "=" + view.pinginterval_val);
	 data[prefix + 'change_' + view.pinginterval_field] = view.pinginterval_val;
	 ret = true; 
 }
 */
 var osv = "";
 var oev = "";
 /*
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
 */
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
 var s = $( "#comptype_val" ).val();

 if (s.substring(0,1) == "Y")
 {
  $("#repository_row").hide();
  $("#rf_repository_row").show();
  $("#rb_repository_row").show();
  $("#alwaysdeploy_val").prop( "checked", false );
  ToggleRepoProps(-1,"rf_repository_row",true);
  ToggleRepoProps(-1,"rb_repository_row",false);
 } 
 else
 {
  $("#repository_row").show();
  $("#rf_repository_row").hide();
  $("#rb_repository_row").hide();
  $("#alwaysdeploy_val").prop( "checked", true ); 
  ToggleRepoProps(-1,"repository_row",true);
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
 var heading = parent.$("#right_panel_title_area h1");
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

 var sumret = GetSaveSummaryData(instance, savedata, '');

 if (Int2obj[objtypeAsInt][0] == "ta")
  namecbfn = "taskSummarySaved";
 else
  namecbfn = "ChangeSummaryName";


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
     parent.$("#modal").dialog("close");
     console.log("Saved data");
     console.log("namecbfn is " + namecbfn + "; typecbfn is " + typecbfn);
     SaveTargetDomain(instance);
     SaveLinkedTask(instance,objid);
     SaveActionTask(instance,objid);
     SaveDomainOrder();
     //
     // If a callback function has been passed, call it with the new name
     //
     if (namecbfn != "")
     {
      var nameCallbackFn = eval(namecbfn);
      if ((typeof nameCallbackFn === "function") && savedata.nameCallbackReqd)
      {
       nameCallbackFn(savedata.newname, savedata.newsumm);
       console.log(namecbfn + " called");
      }
     }
  
     if (Int2obj[objtypeAsInt][0] == "co")
     {
      var myform = instance.find("#summform");

      if (myform.find(":input[name=\"comptype_val\"]").length > 0 && myform.find(":input[name=\"comptype_oldval\"]").length > 0)
      {
       if (myform.find(":input[name=\"comptype_oldval\"]").val() == "")
       {
    	   var custid = $("#customaction_val").val();
    	   if (custid == "") {
		        $.getJSON("UpdateAttrs", "f=inv&c=" + objid + "&xpos=440&ypos=100", function(data)
		          {
		            LoadComponentItemsData();
		            SaveRbRfValues(myform);
		          });
    	   }
	   }
      }
     } 
     if (!isProfile)
      LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
     
     if (category_changed)
     {
    	 console.log("category_changed (1) ");
      DoRefresh();
      category_changed = false;
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
  parent.$("#modal").dialog("close");
  SaveTargetDomain(instance);
  SaveLinkedTask(instance, objid);
  SaveActionTask(instance, objid);
  SaveDomainOrder();
  LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
  if (category_changed)
  {
	  console.log("category_changed (2)");
   DoRefresh();
   category_changed = false;
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
 var pwd = parent.$("#modal");
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
//  var pwd = parent.$("#modal");
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

/*
function ToggleAutoPing()
{
	// var checkedValue = $('.messageCheckbox:checked').val();
 var pwd = parent.$("#modal");
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
*/

function TogglePasswordFields()
{
	var pwd = parent.$("#modal");
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
 var pwd = parent.$("#modal");
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
   }
  });
 
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
     tdedit2 = "<form id=\"summform\"><table id=\"summtab\" class=\"summtable\">";

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
        tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td>" + val + "</td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"username_oldval\" value=\"" + val + "\"/></td>";
        tdedit2 += "</tr>";
      }
      else if (label == "Real Name")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"realname_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"realname_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Email")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"email_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"email_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Phone")
      {
       tdedit2 += "<tr>";
       tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit2 += "<td><input name=\"phone_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_field\" value=\"" + field + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_callback\" value=\"" + callback + "\"/></td>";
       tdedit2 += "<td><input type=\"hidden\" name=\"phone_oldval\" value=\"" + val + "\"/></td>";
       tdedit2 += "</tr>";
      }
      else if (label == "Password")
      {
        tdedit2 += "<tr id=\"password_row\">";
        tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\"> New " + label + ":</td>";
        tdedit2 += "<td><input name=\"password_val\" style='width:24em' type=\"password\" /></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_oldval\" /></td>";
        tdedit2 += "</tr>";
        tdedit2 += "<tr id=\"password_again_row\">";
        tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\"> New " + label + " Again:</td>";
        tdedit2 += "<td><input name=\"password_again_val\" style='width:24em' type=\"password\" /></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"password_again_oldval\" /></td>";
        tdedit2 += "</tr>";
      }
      else if (label == "Date Format")
      {
        tdedit2 += "<tr id=\"datefmt_row\">";
        tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td><select name=\"datefmt_val\" style='width:24em'>";
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
        tdedit2 += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit2 += "<td><select name=\"timefmt_val\" style='width:24em'>";
        tdedit2 += GetTimeFormats(val);
        tdedit2 += "</select></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_field\" value=\"" + field + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_callback\" value=\"" + callback + "\"/></td>";
        tdedit2 += "<td><input type=\"hidden\" name=\"timefmt_oldval\" /></td>";
        tdedit2 += "</tr>";
      }
     }
     tdedit2 += "</table></form>";
     var pwd = parent.$("#modal");
     console.log(tdedit2);
     pwd.html(td);
     EditSummaryButton(true);
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
}


