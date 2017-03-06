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

function SetupWizard()
{
 console.log("In setup");
 var td = "";

 tdedit = "<form id=\"summform\"><table id=\"summtab\" class=\"summtable\">";

 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:left;margin-top:5px;\" colspan=\"3\" class=\"ui-widget-header\"><B>Select Domain and Owner</B></td>";
 tdedit += "<td>&nbsp;</td>";
 tdedit += "<td>&nbsp;</td>";
 tdedit += "</tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Domain:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"domain_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"domain_field\" value=\"\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Owner Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"ownertype_val\" name=\"ownertype_val\">";
 tdedit += "<option value=\"User\">User</option>";
 tdedit += "<option value=\"Group\" selected>Group</option>";
 tdedit += "</select></td>";
 tdedit += "<td><input type=\"hidden\" name=\"ownertype_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr id=\"owner_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Users:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"owner_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"owner_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 tdedit += "<tr id=\"group_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Groups:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"groupowner_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"groupowner_field\" value=\"3\"/></td>";
 tdedit += "</tr>";
 
 // Repo
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:left;margin-top:5px;\" class=\"ui-widget-header\" colspan=\"3\"><B>Create Initial Repository for Deploy Sources</B></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Repository Name:</td>";
 tdedit += "<td><input name=\"reponame_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"reponame_field\" value=\"1\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"repousernameenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">User Name:</td>";
 tdedit += "<td><input name=\"repousernameenc_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"repousernameenc_field\" value=\"802\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"repopasswordenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
 tdedit += "<td><input name=\"repopasswordenc_val\" style='width:24em' type=\"password\" /></td>";
 tdedit += "<td><input type=\"hidden\" name=\"repopasswordenc_field\"  value=\"803\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select id=\"setuptype_val\" name=\"setuptype_val\"></td>";
 tdedit += "<td><input type=\"hidden\" name=\"setuptype_field\" value=\"301\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr><td colspan=\"3\"><table id=\"propdata\" style=\"float:right\">";
 tdedit += "</table></tr>";
// Server
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:left;margin-top:5px;\" colspan=\"3\" class=\"ui-widget-header\"><B>Create Initial End Point Server for Deploy Target</B></td>";
 tdedit += "<td>&nbsp;</td>";
 tdedit += "<td>&nbsp;</td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Environment Name:</td>";
 tdedit += "<td><input name=\"envname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"envname_field\" value=\"1\"/></td>";
 tdedit += "</tr>"; 
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*End Point Name:</td>";
 tdedit += "<td><input name=\"servername_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"servername_field\" value=\"1\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Host Name:</td>";
 tdedit += "<td><input name=\"hostname_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"hostname_field\" value=\"202\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"serverusernameenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">User Name:</td>";
 tdedit += "<td><input name=\"serverusernameenc_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"serverusernameenc_field\" value=\"802\"/></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"serverpasswordenc_row\">";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Password:</td>";
 tdedit += "<td><input name=\"serverpasswordenc_val\" style='width:24em' type=\"password\" /></td>";
 tdedit += "<td><input type=\"hidden\" name=\"serverpasswordenc_field\" value=\"803\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">End Point Type:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"servertype_val\">";
 tdedit += "<option value=\"st3\">as400</option>";
 tdedit += "<option value=\"st4\">mac</option>";
 tdedit += "<option value=\"st1\">unix</option>";
 tdedit += "<option value=\"st2\" selected>windows</option>";
 tdedit += "<option value=\"st5\">Tandem</option>";
 tdedit += "<option value=\"st6\">Stratus</option>";
 tdedit += "<option value=\"st7\">OpenVMS</option>";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"servertype_field\" value=\"201\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Protocol:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"protocol_val\">";
 tdedit += "<option value=\"ftp\">ftp</option>";
 tdedit += "<option value=\"ftps\">ftps</option>";
 tdedit += "<option value=\"sftp\">sftp</option>";
 tdedit += "<option value=\"win\" selected>win</option>";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"protocol_field\" value=\"203\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">*Base Directory:</td>";
 tdedit += "<td><input name=\"basedirectory_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"basedirectory_field\" value=\"204\"/></td>";
 tdedit += "</tr>";
 
 tdedit += "<tr>";
 tdedit += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">End Point Component Types:</td>";
 tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select multiple name=\"servercomptype_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"servercomptype_field\" value=\"208\"/></td>";
 tdedit += "<td><input type=\"hidden\" name=\"servercomptype_oldval\" value=\"\"/></td>";
 tdedit += "</tr>";
   
 tdedit += "</table></form>";
      
  EditSetupButton();
}


function EditSetupButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveSetupData(parent.$("#modal"), summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
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
 pwd.html(tdedit);
 

 pwd.dialog(
 {
  modal : true,
  open : function(event, ui)
  {
   var myform = pwd.find("#summform");
   
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

   if (myform.find(":input[name=\"setuptype_val\"]").length > 0)
   {
    var t = obj2Int["re"][0];

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
      var typeobj = myform.find(":input[name=\"setuptype_val\"]");

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

      for (n = 0; n < res.data.length; n++)
      {
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
 
   $("#setuptype_val").change(GetSetupProps);

   GetSetupProps();
  }
 });
 

 
 pwd.dialog("option", "title", "Quick Setup");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "auto");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');
}

function GetSetupProps()
{ 
 var pwd = parent.$("#modal");
 var myform = pwd.find("#summform");
 var setuptype = myform.find(":input[name=\"setuptype_val\"]");
 var id = setuptype.val();
 
 if (id == null || id == "")
  id = "pd10";
 
 id = id.substring(2);
 
 $.ajax(
   {
    url : "GetPropertiesData?objtype=8&id=" + id + "&defonly=Y",
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     var propdata = $("#propdata");
     console.log(res);
     var td = "<tr><th>&nbsp;</th><th>Value</th><th>Encrypted</th><th>Override</th><th>Append</th></tr>";
     var rowcnt = 0;

     for (a = 0; a < res.defs.length; a++)
     {
      var row = res.defs[a];
      if (row.name == "dsn")
       row.name = "data source name";
      
      propdefs[row.name] =
      {
       "append" : row.append,
       "reqd" : row.reqd,
       "used" : false
      };
     
       var req = "";
       if (row.reqd)
        req = "*";
        
       if (row.append)
        td += "<tr><td  align=\"center\" id=\"propname_" + rowcnt + "\">" + req+ row.name + "</td><td align=\"center\"><input name=\"propval_" + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propenc_"  + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propoverride_" + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propappend_" + row.name + "\" /></td></tr>";
       else
        td += "<tr><td  align=\"center\" id=\"propname_" + rowcnt + "\">" + req+ row.name + "</td><td align=\"center\"><input name=\"propval_" + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propenc_"  + row.name + "\" /></td><td align=\"center\"><input type=\"checkbox\" name=\"propoverride_" + row.name + "\" /></td><td align=\"center\">&nbsp;</td></tr>";

      rowcnt++;
     }

    propdata.html(td);

    },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
   }); 
}

function GetSetupSummaryData(instance, data, SaveType)
{
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view = {};

 view["servercomptype_val"] = "";
 
 for ( var i in viewArr)
 {
  if (viewArr[i].name == "servercomptype_val")
   view[viewArr[i].name] += viewArr[i].value + ";";
  else
   view[viewArr[i].name] = viewArr[i].value;
 }

 console.log(view);

 if (SaveType == "Credentials")
 {
  data.newname = view.reponame_val;
  console.log('change_' + view.reponame_field + ' = ' + view.reponame_val);
  data['change_' + view.reponame_field] = view.reponame_val;

  console.log('change_801 = ck2');
  data['change_801' ] = 'ck2';

  console.log('change_' + view.repousernameenc_field + ' = ' + view.repousernameenc_val);
  data['change_' + view.repousernameenc_field] = view.repousernameenc_val;

  console.log('change_' + view.repopasswordenc_field + ' = ' + view.repopasswordenc_val);
  data['change_' + view.repopasswordenc_field] = view.repopasswordenc_val;

  ret = true;
 }
 else if (SaveType == "Repository")
 {
  data.newname = view.reponame_val;
  console.log('change_' + view.reponame_field + ' = ' + view.reponame_val);
  data['change_' + view.reponame_field] = view.reponame_val;

  console.log('change_' + view.setuptype_field + ' = ' + view.setuptype_val);
  data['change_' + view.setuptype_field] = view.setuptype_val;

  console.log('change_302 = ' + SetupRepoCredId);
  data['change_302'] = "cr" + SetupRepoCredId;

  var ownertype = $("#ownertype_val");
  var owt = "User";

  if (ownertype.length > 0)
   owt = ownertype.val();

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 

  ret = true;
 }
 else if (SaveType == "ServerCredentials")
 {
  data.newname = view.servername_val;
  console.log('change_' + view.servername_field + ' = ' + view.servername_val);
  data['change_' + view.servername_field] = view.servername_val;

  console.log('change_801 = ck2');
  data['change_801' ] = 'ck2';
  
  console.log('change_' + view.serverusernameenc_field + ' = ' + view.serverusernameenc_val);
  data['change_' + view.serverusernameenc_field] = view.serverusernameenc_val;

  console.log('change_' + view.serverpasswordenc_field + ' = ' + view.serverpasswordenc_val);
  data['change_' + view.serverpasswordenc_field] = view.serverpasswordenc_val;

  ret = true;
 }
 else if (SaveType == "Server")
 {
  data.newname = view.servername_val;
  console.log('change_' + view.servername_field + ' = ' + view.servername_val);
  data['change_' + view.servername_field] = view.servername_val;

  console.log('change_' + view.servertype_field + ' = ' + view.servertype_val);
  data['change_' + view.servertype_field] = view.servertype_val;

  console.log('change_' + view.hostname_field + ' = ' + view.hostname_val);
  data['change_' + view.hostname_field] = view.hostname_val;

  console.log('change_' + view.protocol_field + ' = ' + view.protocol_val);
  data['change_' + view.protocol_field] = view.protocol_val;

  console.log('change_' + view.basedirectory_field + ' = ' + view.basedirectory_val);
  data['change_' + view.basedirectory_field] = view.basedirectory_val;

  if (view.servercomptype_val != view.servercomptype_oldval)
  {
   console.log('change_' + view.servercomptype_field + "=" + view.servercomptype_val);
   data['change_' + view.servercomptype_field] = view.servercomptype_val;
   ret = true;
  }
  
  console.log('change_205 = ' + SetupServerCredId);
  data['change_205'] = 'cr' + SetupServerCredId;

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 

  ret = true;
 }
 else if (SaveType == "Environment")
 {
  data.newname = view.envname_val;
  console.log('change_' + view.envname_field + ' = ' + view.envname_val);
  data['change_' + view.envname_field] = view.envname_val;

  if (owt == "User")
  { 
   console.log('change_' + view.owner_field + ' = ' + view.owner_val);
   data['change_' + view.owner_field] = view.owner_val;
  }
  else
  {
   console.log('change_' + view.groupowner_field + ' = ' + view.groupowner_val);
   data['change_' + view.groupowner_field] = view.groupowner_val;
  } 
 }

 return ret;
}

function SaveSetupData(instance)
{
 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view = {};

 for ( var i in viewArr)
 {
  if (viewArr[i].value == "on")
   viewArr[i].value = "Y";

  if (viewArr[i].value == "off")
   viewArr[i].value = "N";
  
  view[viewArr[i].name] = viewArr[i].value;
 }

// Edit Checks 
 if (view.reponame_val.trim() == "")
 {
  alert("Repository Name is required");
  return;
 }
 
 if (view.envname_val.trim() == "")
 {
  alert("Environment Name is required");
  return;
 }
 
 if (view.servername_val.trim() == "")
 {
  alert("End Point Name is required");
  return;
 }
 
 if (view.hostname_val.trim() == "")
 {
  alert("Host Name is required");
  return;
 }
 
 if (view.basedirectory_val.trim() == "")
 {
  alert("Base Directory is required");
  return;
 }
 
 $.blockUI();
 var domainid = view.domain_val;

 var SaveList = [ "Credentials", "Repository", "ServerCredentials", "Server", "Environment" ];

 for (var k=0; k < SaveList.length; k++)
 {
  var savedata = { objtype : -1, id : "" };
  
  SetupNewID(SaveList[k], domainid, view);

  if (SaveList[k] == "Credentials")
   {
    savedata = { objtype : 19, id : SetupRepoCredId };
   }
  else if (SaveList[k] == "Repository")
  {
   savedata = { objtype : 8, id : SetupRepoId };
  }
  else if (SaveList[k] == "ServerCredentials")
  {
   savedata = { objtype : 19, id : SetupServerCredId };
  }
  else if (SaveList[k] == "Server")
  {
   savedata = { objtype : 4, id : SetupServerId };
  }
  else if (SaveList[k] == "Environment")
  {
   savedata = { objtype : 2, id : SetupEnvId };
  }
  
  if (savedata.id == "")
   continue;

  var sumret = GetSetupSummaryData(instance, savedata, SaveList[k]);

  console.log(savedata);

  if (sumret)
  {
   $.ajax(
   {
    url : "UpdateSummaryData",
    dataType : "json",
    type : "POST",
    async : false,
    data : savedata,
    success : function(data)
    {
     console.log(data);
    }
   });
  }
 }
 

 var data =
 {
  objtype : 8,
  id : SetupRepoId 
 };
 
 for (var x in view)
 {
  if (x.indexOf("propval_") == 0)
  {
   var name = x.substring(8);

   if (view['propappend_' + name] != "Y" && view['propappend_' + name] != "N")
    view['propappend_' + name] = 'N';

   if (view['propenc_' + name] != "Y" && view['propenc_' + name] != "N")
    view['propenc_' + name] = 'N';

   if (view['propoverride_' + name] != "Y" && view['propoverride_' + name] != "N")
    view['propoverride_' + name] = 'N';
   
   var val = view['propenc_' + name] + view['propoverride_' + name] + view['propappend_' + name] + view['propval_' + name];
   var key = "add_" + name;

   data[key] = val;
  } 
 }
 
 console.log(data);
 
 $.ajax(
   {
    url : "UpdatePropertiesData",
    type : 'POST',
    dataType : "json",
    data : data,
    success : function(data)
    {
    }
   });
 
 console.log("UpdateAttrs?f=sm&e="+ SetupEnvId + "&s=" + SetupServerId + "&xpos=20&ypos=20");
 
 $.ajax(
  {
   url: "UpdateAttrs?f=sm&e="+ SetupEnvId + "&s=" + SetupServerId + "&xpos=20&ypos=20",
   type: 'GET',
   dataType: "text",
   async: false,
   success: function(data)
   {
   }
  });

 
 parent.$("#modal").dialog("close");
 $.unblockUI();
 return;
}

function SetupNewID(ot, parentid, view)
{
 var objtype = ot;
 
 if (ot == "ServerCredentials")
  objtype = "Credentials";
 
 var name = "";


 if (ot == "Credentials")
  name  = view.reponame_val;
 else if (ot == "Repository")
  name  = view.reponame_val;
 else if (ot == "ServerCredentials")
 { 
  if (view.reponame_val == view.servername_val)
   view.servername_val += "_server";
  name  = view.servername_val;
 }
 else if (ot == "Server")
  name  = view.servername_val;
 else if (ot == "Environment")
  name  = view.envname_val;
 
 if (name == "")
 {
  if (ot == "Credentials")
   SetupRepoCredId = "";
  else if (ot == "Repository")
   SetupRepoId = "";
  else if (ot == "ServerCredentials")
   SetupServerCredId = "";
  else if (ot == "Server")
   SetupServerId = "";
  else if (ot == "Environment")
   SetupEnvId = "";
  
  return;
 } 
 
 var url = "GetNewID?objtype="+objtype+"&pid="+parentid + "&ptype=Domain&at=&name="+name + "&treeid=";

 console.log(url);
 
 $.ajax({
  url: url,
  dataType: 'json',
  async: false,
  success: function(data) {
   console.log(data);
   $.each(data, function(key, val) {
    if (key == "name") name = val;
    if (key == "id") id = val;
    if (key == "rt") rt = val;
   });
   console.log("done creating in tree new id ="+id);
   if (ot == "Credentials")
    SetupRepoCredId = id.substring(2);
   else if (ot == "Repository")
    SetupRepoId = id.substring(2);
   else if (ot == "ServerCredentials")
    SetupServerCredId = id.substring(2);
   else if (ot == "Server")
    SetupServerId = id.substring(2);
   else if (ot == "Environment")
    SetupEnvId = id.substring(2);
  }
 });
} 




