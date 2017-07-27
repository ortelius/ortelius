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

function LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

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
     
     if (readonly)
      $("#title_icon").hide();
     else
      $("#title_icon").show();    
      
     saveRes = res;
     var td = "";
     tdedit = "<form id=\"summform\"><table id=\"summtab\" class=\"summtable\">";

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
       if (field == 802)
       {
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
        tdedit += "<tr id=\"username_row\">";
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
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
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><input name=\"groupname_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"groupname_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Real Name")
      {
       tdedit += "<tr>";
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
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"passwordenc_val\" style='width:24em' type=\"password\" /></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"passwordenc_oldval\" /></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<tr id=\"password_row\" style=\"display:none\">";
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td><input name=\"password_val\" style='width:24em' type=\"password\" /></td>";
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
      else if (label == "Protocol")
      {
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"protocol_val\">";
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + " Hostname:</td>";
       tdedit += "<td><input name=\"engine_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"engine_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Approval Domain" || label == "Move to Domain" || label == "Copy to Domain" || label == "Create Version in Domain")
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
       var repo = val;
       if (typeof repo.name == "undefined")
       {
        repo = new Object();
        repo.name = "";
        repo.type = "re";
        repo.id = 0;
       }
       val = repo.name;
       tdedit += "<tr id=\"repository_row\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td><select name=\"repository_val\"/></td>";
       save_repository_val = repo.name;
       tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"" + repo.type + repo.id + "\"/></td>";
       tdedit += "</tr>";
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
       if (val)
        tdedit += "<td><input name=\"alwaysdeploy_val\" style='width:24em' type=\"checkbox\" checked /></td>";
       else
        tdedit += "<td><input name=\"alwaysdeploy_val\" style='width:24em' type=\"checkbox\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_callback\" value=\"" + callback + "\"/></td>";
       if (val)
        tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"on\"/></td>";
       else
        tdedit += "<td><input type=\"hidden\" name=\"alwaysdeploy_oldval\" value=\"off\"/></td>";
       tdedit += "</tr>";
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
      else if (label == "Account Locked")
      {
       var showoutput = val;
       val = showoutput.value;
       tdedit += "<tr>";
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
       tdedit += "<tr>";
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"filterlevel_val\">";
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
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"filterlevel_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Roll Forward")
      {
       tdedit += "<tr>";
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
       tdedit += "<tr>";
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
       }
       else if (val == "st4")
       {
        oldval = val;
        val = "mac";

        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\" selected>mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
       }
       else if (val == "st1")
       {
        oldval = val;
        val = "unix";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\" selected>unix</option>";
        tdedit += "<option value=\"st2\">windows</option>";
       }
       else
       {
        oldval = val;
        val = "windows";
        tdedit += "<option value=\"st3\">as400</option>";
        tdedit += "<option value=\"st4\">mac</option>";
        tdedit += "<option value=\"st1\">unix</option>";
        tdedit += "<option value=\"st2\" selected>windows</option>";
       }

       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"servertype_oldval\" value=\"" + oldval + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label.toLowerCase() == "Pre-Action".toLowerCase())
      {
       var action = val;
       if (typeof action.name == "undefined")
       {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
       }
       val = action.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"preaction_val\">";
       tdedit += "<option value=\"\"></option>";
       save_preaction_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"preaction_oldval\" value=\"" + action.type + action.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label.toLowerCase() == "Post-Action".toLowerCase())
      {
       var action = val;
       if (typeof action.name == "undefined")
       {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
       }
       val = action.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"postaction_val\">";
       save_postaction_val = val;
       tdedit += "<option value=\"\"></option>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"postaction_oldval\" value=\"" + action.type + action.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Custom Action" && (summSaveobjtype == "ap" || summSaveobjtype == "av"))
      {
       var action = val;
       if (typeof action.name == "undefined")
       {
        action = new Object();
        action.name = "";
        action.type = "ac";
        action.id = 0;
       }
       val = action.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"customaction_val\">";
       save_customaction_val = val;
       tdedit += "<option value=\"\"></option>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"customaction_oldval\" value=\"" + action.type + action.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Credential")
      {
       var cred = val;
       if (typeof cred.name == "undefined")
       {
        cred = new Object();
        cred.name = "";
        cred.type = "cr";
        cred.id = 0;
       }
       val = cred.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"credential_val\">";
       save_credential_val = val;
       tdedit += "<option value=\"\"></option>";
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"credential_oldval\" value=\"" + cred.type + cred.id + "\"/></td>";
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
      else if (label == "Successful Deployment Template")
      {
       var template = val;
       if (typeof template.name == "undefined")
       {
        template = new Object();
        template.name = "";
        template.type = "te";
        template.id = 0;
       }
       val = template.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"template_val\" />";
       save_template_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"template_oldval\" value=\"" + template.type + template.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Failed Deployment Template")
      {
       var template = val;
       if (typeof template.name == "undefined")
       {
        template = new Object();
        template.name = "";
        template.type = "te";
        template.id = 0;
       }
       val = template.name;
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"fail_template_val\" />";
       save_fail_template_val = val;
       tdedit += "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"fail_template_oldval\" value=\"" + template.type + template.id + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Type")
      {
       var typeobj = val;
       if (typeof typeobj.name == "undefined")
       {
        typeobj = new Object();
        typeobj.name = "";
        typeobj.type = "pd";
        typeobj.id = 0;
       }
       val = typeobj.name;
       tdedit += "<tr>";
       if (summSaveobjtype == "ds")
       {
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\">odbc<select name=\"type_val\" style=\"display:none\">";
        save_type_val = val;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"" + typeobj.type + typeobj.id + "\"/></td>";
        tdedit += "</tr>";
       }
       else
       {
        tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
        tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"type_val\">";
        save_type_val = val;
        tdedit += "</td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_field\" value=\"" + field + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_callback\" value=\"" + callback + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"" + typeobj.type + typeobj.id + "\"/></td>";
        tdedit += "</tr>";
       }
      }
      else if (label == "Kind")
      {
       var kind = val;
       if (typeof kind.name == "undefined")
       {
        kind = new Object();
        kind.name = "";
        kind.type = "ak";
        kind.id = 0;
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
       tdedit += "<td><input type=\"hidden\" name=\"kind_oldval\" value=\"" + kind.type + kind.id + "\"/></td>";
       tdedit += "</tr>";
       save_kind_id = kind.id;
       }
       
       switch (Number(kind.id))
       {
        case 1: // DMScript Function in repository
         $("#tabs-Args").hide();
         break;

        case 2: // DMScript Function in database
         $("#tabs-Args").hide();
         break;

       case 3: // Function provided by local external script or program
        $("#tabs-Args").show();
        break;

       case 4: // Function provided by remote external script or program
        $("#tabs-Args").show();
        break;

       case 5: // Function provided by plugin
        $("#tabs-Args").hide();
        break;

       case 6: // Graphical deployment flow
        $("#tabs-Args").hide();

       default:
        break;
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
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"created_val\" style=\"padding-top:2px;padding-bottom:2px;\">" + val + "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"created_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"created_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"created_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Modified")
      {
       var modified = val;
       if (typeof modified.when != "undefined")
       {
        var d = convertDate(modified.when);
        val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
        val += " by " + modified.tooltip;
       }
       else
        val = "";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       tdedit += "<td name=\"modified_val\" style=\"padding-top:2px;padding-bottom:2px;\">" + val + "</td>";
       tdedit += "<td><input type=\"hidden\" name=\"modified_field\" value=\"" + field + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"modified_callback\" value=\"" + callback + "\"/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"modified_oldval\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      else if (label == "Last Login")
      {
       var modified = val;
       if (typeof modified.when != "undefined")
       {
        var d = convertDate(modified.when);
        val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
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
      else if (label == "LifeCycle Domain" || label == "Copy to Remote" || label == "Always Deploy" || label == "Result Is Expr" || label == "Show Output" || label == "Account Locked"
        || label == "Force Change Password")
      {
       if (label == "LifeCycle Domain")
        label = "Contains Lifecycles";

       td += "<tr><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       if (val == true)
        td += "Yes";
       else
        td += "No";
      }
      else if (label == "Password")
      {
       /*
       if (((save_kind_id == 0 || save_kind_id == 4) && field == 805) || ((save_kind_id == 2) && field == 803))
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (field == 805 && val.length == 0)
         td += "";
        else
         td += "*********";
       }
       else if (save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 10)
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (val.length == 0)
        td += "";
       else
        td += "*********";
       }
       */
      }
      else if (label == "Username")
      {
       if (((save_kind_id == 0 || save_kind_id == 4) && field == 804) || ((save_kind_id == 2) && field == 802))
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        if (field == 802)
         td += "*********";
        else
         td += val;
       }
       else if (save_kind_id != 2 && save_kind_id != 3 && save_kind_id != 4 && save_kind_id != 10)
       {
        td += "<tr><td class=\"summlabel\">";
        td += label;
        td += ":</td><td>";
        td += val;
       }
      }
      else if (label == "Kind")
      {
       td += "<tr><td id=\"kind_summ\" class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += val;
      }
      else  if (label == "Custom Action" &&  !(summSaveobjtype == "ap" || summSaveobjtype == "av"))
      {
       // skip these guys for components 
      }
      else
      {
       var myid = label.toLocaleLowerCase().replace(" ", "") + "_sumrow";

       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += val;
      }
      td += "</td></tr>";
     }

     if (objtype == "do")
     {
      $.ajax(
      {
       url : "SubDomains?domainid=" + objid,
       dataType : 'json',
       type : 'GET',
       success : function(res)
       {

        if (res.length != 0)
        {
         tdedit += "<tr>";
         tdedit += "<td style=\"text-align:right;vertical-align:top;padding-right: 5px;white-space: nowrap;\">Sub Domains:</td>";

         if (isLifeCycle)
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
         else
          tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;background-color:white;border:1px solid #aaa;\"><ul id=\"subdomainlist\" style=\"list-style-type: none;\" >";
        }
        var tmp = " class=\"islifecycle\"";

        if (!isLifeCycle)
         tmp = "";

        for (var k = 0; k < res.length; k++)
        {

         tdedit += "<li id=\"dom_" + res[k].id + "\"" + tmp + "><span style=\"cursor: pointer;\" >" + res[k].name + "</span></li>";

         if (k == 0)
          td += "<tr><td>Sub Domains:</td><td>" + res[k].name + "</td></tr>";
         else
          td += "<tr><td>&nbsp;</td><td>" + res[k].name + "</td></tr>";
        }
        if (res.length != 0)
        {
         tdedit += "</ul>";
         tdedit += "</td>";
         tdedit += "<td></td>";
         tdedit += "<td></td>";
         tdedit += "<td></td>";
         tdedit += "</tr>";
         tdedit += "<tr id=\"dndmsg\"><td colspan=\"5\" style=\"padding-top:8px;\"><i>Drag and drop the Sub Domains to order them for your lifecycle model.</i></td></tr>";

         tdedit += "</table></form>";
        }
        $("#" + tablename + " > tbody").html(td);

        HideFields();

        if (objid < 0)
         EditSummaryButton();
       },
       error : function(jqxhr, status, err)
       {
        console.log(status);
        console.log(err);
       }
      });
     }
     else
     {
      tdedit += "</table></form>";
      $("#" + tablename + " > tbody").html(td);
      HideFields();

      if (objid < 0)
       EditSummaryButton();
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
 if (summSaveobjtype != "ac" && summSaveobjtype != "pr" && summSaveobjtype != "fn")
  return;

 switch (save_kind_id)
 {
 case 1: // DMScript Function in repository
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").show();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").hide();
  break;

 case 2: // DMScript Function in database
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  break;

 case 3: // Function provided by local external script or program
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").hide();
  break;

 case 4: // Function provided by remote external script or program
  $("#fragmentname_sumrow").hide();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").show();
  $("#copytoremote_sumrow").show();
  break;

 case 5: // Function provided by plugin
  $("#fragmentname_sumrow").show();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  break;

 case 6: // Graphical deployment flow
  $("#fragmentname_sumrow").show();
  $("#repository_sumrow").hide();
  $("#filepath_sumrow").hide();
  $("#copytoremote_sumrow").hide();
  break;

 default:
  break;
 }
 
}

function EditSummaryButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   var name = "";

   if (summSaveobjid < 0)
   {
    var namefld = pwd.find(":input[name=\"name_val\"]");

    if (typeof namefld !== undefined)
     name = namefld.val();

    switch (summSaveobjtype)
    {
    case "pr":
     NewID("Action", "PROCEDURE", currenttree, "P", name);
     break;

    case "fn":
     NewID("Action", "FUNCTION", currenttree, "F", name);
     break;

    case "ac":
     NewID("Action", "ACTION", currenttree, "G", name);
     break;

    case "rl":
     NewID("Release", "RELEASE", currenttree, "", name);
     break;

    case "no":
     NewID("Notify", "NOTIFY", currenttree, "", name);
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
     NewID("Component", "COMPONENT", currenttree, "", name);
     break;

    case "en":
     NewID("Environment", "ENVIRONMENT", currenttree, "", name);
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

    default:
     break;
    }
   }
   SaveSummaryData(parent.$("#modal"), summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
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
 pwd.html(tdedit);
 pwd.dialog(
 {
  modal : true,
  open : function(event, ui)
  {   
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
    $.ajax(
    {
     url : "GetActions?reason=actonly",
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

      for (n = 0; n < res.length; n++)
      {
        if (save_preaction_val == res[n].name)
        preaction.append('<option id="preact' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        preaction.append('<option id="preact' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

       if (save_postaction_val == res[n].name)
        postaction.append('<option id="postact' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        postaction.append('<option id="postact' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

       if (save_customaction_val == res[n].name)
        customaction.append('<option id="customact' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        customaction.append('<option id="customact' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
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
     url : "GetTemplates",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#summform");
      var template = myform.find(":input[name=\"template_val\"]");
      var fail_template = myform.find(":input[name=\"fail_template_val\"]");

      for (n = 0; n < res.length; n++)
      {
       if (save_template_val == res[n].name)
        template.append('<option id="template' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        template.append('<option id="template' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

       if (save_fail_template_val == res[n].name)
        fail_template.append('<option id="fail_template' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        fail_template.append('<option id="fail_template' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

      }
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

      for (n = 0; n < res.length; n++)
      {
       if (save_repository_val == res[n].name)
        owner.append('<option id="repository' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
       else
        owner.append('<option id="repository' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
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
   }

   if (summSaveobjtype == "cr")
   {
    var myform = pwd.find("#summform");
    var typeobj = myform.find(":input[name=\"kind_val\"]");

    var res = [
    {
     type : 'ck',
     id : 2,
     name : 'Encrypted in Database'
    },
    {
     type : 'ck',
     id : 4,
     name : 'From Variables'
    },
    {
     type : 'ck',
     id : 10,
     name : 'Harvest DFO in Filesystem'
    } ];

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
    }

    for (n = 0; n < res.length; n++)
    {
     if (save_kind_val == res[n].name || (save_kind_val == "unconfigured" && n == 2))
     {
      typeobj.append('<option id="type' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');

      switch (res[n].id)
      {
      case 1: // DMScript Function in repository
       $("#fragmentname_row").hide();
       $("#repository_row").show();
       $("#filepath_row").show();
       $("#copytoremote_row").hide();
       break;

      case 2: // DMScript Function in database
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       break;

      case 3: // Function provided by local external script or program
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").show();
       $("#copytoremote_row").hide();
       break;

      case 4: // Function provided by remote external script or program
       $("#fragmentname_row").hide();
       $("#repository_row").hide();
       $("#filepath_row").show();
       $("#copytoremote_row").show();
       break;

      case 5: // Function provided by plugin
       $("#fragmentname_row").show();
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       break;

      case 6: // Graphical deployment flow
       $("#fragmentname_row").show();
       $("#repository_row").hide();
       $("#filepath_row").hide();
       $("#copytoremote_row").hide();
       break;

      default:
       break;
      }
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
  }
 });

 var title = "General";

 if (summSaveobjid < 0)
 {
  var objstr = obj2Int[summSaveobjtype][1];
  if (objstr == "Notify")
   objstr = "Notifier";
  
  if (objstr == "Application")
   objstr = "Application Base Version";
  
  if (objstr == "Component")
   objstr = "Component Base Version";

   title = "New " + objstr;
 }

 pwd.dialog("option", "title", title);
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "auto");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');
}

function convertDate(in_date)
{
 var match = in_date.match(/^(\d+)\/(\d+)\/(\d+) (\d+)\:(\d+)\:(\d+)$/);
 if (match == null) {
	 // try alternate (no seconds)
	 match = in_date.match(/^(\d+)\/(\d+)\/(\d+) (\d+)\:(\d+)$/);
 }
 var t = in_date.split(" ")[1];
 var newdate = match[3] + "/" + match[2] + "/" + match[1] + " " + t;

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
 data.newsumm = null;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view =
 {};

 for ( var i in viewArr)
 {
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

 console.log(view);

 if (view.name_val != view.name_oldval)
 {
  data.newname = view.name_val;
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

 if (view.username_val != view.username_oldval)
 {
  if (summSaveobjtype != "cr")
  {
   data.newname = view.username_val;
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
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
  }
  else
  {
   id = "cy" + id;
   console.log(prefix + 'change_' + view.category_field + ' = ' + id);
   data[prefix + 'change_' + view.category_field] = id;
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

 if (view.servertype_val != view.servertype_oldval)
 {
  console.log(prefix + 'change_' + view.servertype_field + ' = ' + view.servertype_val);
  data[prefix + 'change_' + view.servertype_field] = view.servertype_val;
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

 if (view.alwaysdeploy_val != view.alwaysdeploy_oldval)
 {
  console.log(prefix + 'change_' + view.alwaysdeploy_field + ' = ' + view.alwaysdeploy_val);
  data[prefix + 'change_' + view.alwaysdeploy_field] = view.alwaysdeploy_val;
  ret = true;
 }

 if (view.showoutput_val != view.showoutput_oldval)
 {
  console.log(prefix + 'change_' + view.showoutput_field + ' = ' + view.showoutput_val);
  data[prefix + 'change_' + view.showoutput_field] = view.showoutput_val;
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

 if (view.rollup_val != view.rollup_oldval)
 {
  console.log(prefix + 'change_' + view.rollup_field + ' = ' + view.rollup_val);
  data[prefix + 'change_' + view.rollup_field] = view.rollup_val;
  ret = true;
 }

 if (view.rollback_val != view.rollback_oldval)
 {
  console.log(prefix + 'change_' + view.rollback_field + ' = ' + view.rollback_val);
  data[prefix + 'change_' + view.rollback_field] = view.rollback_val;
  ret = true;
 }

 if (view.preaction_val != view.preaction_oldval)
 {
  if (view.preaction_val == "NULL")
   view.preaction_val = null;

  console.log(prefix + 'change_' + view.preaction_field + ' = ' + view.preaction_val);
  data[prefix + 'change_' + view.preaction_field] = view.preaction_val;
  ret = true;
 }

 if (view.postaction_val != view.postaction_oldval)
 {
  if (view.postaction_val == "NULL")
   view.postaction_val = null;

  console.log(prefix + 'change_' + view.postaction_field + ' = ' + view.postaction_val);
  data[prefix + 'change_' + view.postaction_field] = view.postaction_val;
  ret = true;
 }

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

 if (view.repository_val != view.repository_oldval)
 {
  console.log(prefix + 'change_' + view.repository_field + ' = ' + view.repository_val);
  data[prefix + 'change_' + view.repository_field] = view.repository_val;
  ret = true;
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

 return ret;
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

  switch (id)
  {
  case 1: // DMScript Function in repository
   $("#fragmentname_row").hide();
   $("#repository_row").show();
   $("#filepath_row").show();
   $("#copytoremote_row").hide();
   break;

  case 2: // DMScript Function in database
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
   break;

  case 3: // Function provided by local external script or program
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").show();
   $("#copytoremote_row").hide();
   break;

  case 4: // Function provided by remote external script or program
   $("#fragmentname_row").hide();
   $("#repository_row").hide();
   $("#filepath_row").show();
   $("#copytoremote_row").show();
   break;

  case 5: // Function provided by plugin
   $("#fragmentname_row").show();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
   break;

  case 6: // Graphical deployment flow
   $("#fragmentname_row").show();
   $("#repository_row").hide();
   $("#filepath_row").hide();
   $("#copytoremote_row").hide();
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
 var view =
 {};

 for ( var i in viewArr)
 {
  view[viewArr[i].name] = viewArr[i].value;
 }

 if (view.linkedtask_val != view.linkedtask_oldval)
 {
  var tasktype = view.linkedtask_field;
  var id = view.linkedtask_val;
  return new Array(tasktype, id);
 }

 return null;
}

function SaveSummaryData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
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

     /*
      * if (typecbfn != "" && typecbfn.indexOf("{") < 0) { var typeCallbackFn =
      * eval(typecbfn); if ((typeof typeCallbackFn === "function") &&
      * savedata.typeCallbackReqd) { typeCallbackFn(); console.log(typecbfn + "
      * called"); } }
      * 
      * if (savedata.nameCallbackReqd) { FireSummaryEvent('namechg', { newname :
      * savedata.newname, newsumm : savedata.newsumm }); } if
      * (savedata.typeCallbackReqd) { FireSummaryEvent('typechg'); }
      */
     LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
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
  SaveDomainOrder();
  LoadSummaryData(tablename, objtypeAsInt, objtype, objid, addParams);
 }
 return;
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
    DoRefresh();
   }
  });
 }
 else if (summSaveisLifeCycle != isLifeCycle)
 {
  DoRefresh();
 }
}

function SaveLinkedTask(instance,objid)
{
 var linkedret = GetSaveLinkedData(instance);

 if (linkedret != null)
 {
  var tasktype = linkedret[0];
  var domainid = linkedret[1];
  var url = "UpdateAttrs?f=" + tasktype + "&tid=" + objid + "&tgt=" + domainid;
  console.log(url);

  $.ajax(
  {
   url : url,
   type : "GET",
   async : false,
   dataType : "text",
   success : function(data)
   {
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

