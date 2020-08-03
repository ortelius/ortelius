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

function LoadApp4EnvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 if (appdropdown == "")
 {
  $.ajax(
   {
    url : "ReportsData?type=AppList",
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     for (n = 0; n < res.data.length; n++)
     {
      if (res.data[n].id.startsWith("ap"))
        appdropdown += '<option value=\"' + res.data[n].id + "\">" + res.data[n].domain + "." + res.data[n].name + '</option>';
     }
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
 } 
 
 console.log("LoadApp4EnvData appid="+objid);
 
 $.getJSON('GetAppVersInEnvData', "envid=" + objid, function(res)
 {
  var td = "";
  var rowcnt = 0;
  console.log("res");
  console.log(res);
  
  if (!("data" in res))
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

   td += '<tr id="app4env_row_' + rowcnt + '"><td>' + data.name + '</td></tr>';
  }
  app4env_rowcnt = rowcnt;

  $("#" + tablename + " > tbody").empty().append(td);
 });
}

function AddApp2EnvRow()
{
 key = "";
 val = "";
 var td  = "";
 app4env_rowcnt++;
 button_col = "<button class=\"delete_button\" style=\"display:none;padding:0px;padding-left:16px;\" onClick=\"javascript:DeleteApp4Env('" + app4env_rowcnt + "')\"><i class=\"fal fa-trash\" aria-hidden=\"true\"></i></button>";

 td += '<tr id="app4env_row_' + app4env_rowcnt + '"><td><input type="text" id="app4env_keytext_' + app4env_rowcnt + '" value=""><select id="app4env_key_' + app4env_rowcnt + '" value="">' + appdropdown + '</select></td><td>' + button_col + '</td>';
 td += '<td style="display:none"><input type="hidden" id="app4env_oldkey_' + app4env_rowcnt + '" value="en0"></td></tr>';
 
 $("#app4env > tbody").append(td);
 $("#app4env_keytext_" + app4env_rowcnt).focus();
 
 $("#app4env_row_" + app4env_rowcnt).mouseenter(function() {
  app4env_rowid = $(this).attr("id");
  parts = app4env_rowid.split('_');
  rowid = parts.pop();
  $("#app4env_keytext_" + rowid).hide();
  $("#app4env_key_" + rowid).show();
  $("#" + app4env_rowid + " > td:nth-child(2) > button").show();
}).mouseleave(function() {
     app4env_rowid = $(this).attr("id");
    $("#" + app4env_rowid + " > td:nth-child(2) > button").hide();
    parts = app4env_rowid.split('_');
    rowid = parts.pop();
    
    $("#app4env_keytext_" + rowid).show();
    $("#app4env_key_" + rowid).hide();
    SaveApp4EnvData(rowid);
});
}


function SaveApp4EnvData(rowid)
{
 id = $("#app4env_key_" + rowid).val();
 oldid = $("#app4env_oldkey_" + rowid).val();
 
 if (id == null || oldid == null || id == oldid)
  return;
 
 $("#app4env_key_" + rowid).val(id);
 $("#app4env_oldkey_" + rowid).val(id);
 
 srvid = id.substring(2);
 key = oldid.substring(2);
 
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
     LoadApp4EnvData("app4env", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
 
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
  LoadApp4EnvData("app4env", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function DeleteApp4Env(rowid)
{
 id = $("#app4env_oldkey_" + rowid).val();
 key = id.substring(2);

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
    LoadApp4EnvData("app4env", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
}

