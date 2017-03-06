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
function LoadEmailToData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 $.getJSON('GetNotifyRecipientList', "id=" + objid, function(data)
 {

  var td = "";
  var tdedit = "";
  var rowcnt = 0;

  console.log(data);

  for (a = 0; a < data.RecipientCount; a++)
  {
   var type = "ru";

   if (data.Recipients[a].type == "group")
    type = "rg";
   else if (data.Recipients[a].type == "user" && Number(data.Recipients[a].id) < 0)
   {
    type = "rs";
    data.Recipients[a].id = Number(data.Recipients[a].id) * -1;
   } 

   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\" class='tmain " + data.Recipients[a].type + "' id=\"" + type + data.Recipients[a].id + "\">" + data.Recipients[a].name
     + "</td></tr>";
   rowcnt++;
  }

  special_user_select = "";
  avail_user_select = "";
  avail_group_select = "";

  for (a = 0; a < data.SpecialCount; a++)
  {
   special_user_select += "<option value=\"as" + data.SpecialUsers[a].id + "\">" + data.SpecialUsers[a].name + "</option>";
  }
  for (a = 0; a < data.UserCount; a++)
  {
   avail_user_select += "<option value=\"au" + data.AvailableUsers[a].id + "\">" + data.AvailableUsers[a].name + "</option>";
  }
  for (a = 0; a < data.UserGroupCount; a++)
  {
   avail_group_select += "<option value=\"ag" + data.AvailableGroups[a].id + "\">" + data.AvailableGroups[a].name + "</option>";
  }

  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
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
}

function AddEmailToButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveEmailToData(parent.$("#modal"), false);
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

 var tdedit = "<form id=\"EmailToform\"><table id=\"EmailTotab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\">Type:</td></tr>";
 tdedit += "<tr><td style=\"text-align:left;\"><select id=\"totype\" name=\"totype\" onChange=\"ToggleToType()\">";
 tdedit += "<option val=\"Calculated\" selected >Calculated</option>";
 tdedit += "<option val=\"User\">User</option>";
 tdedit += "<option val=\"Group\">Group</option>";
 tdedit += "</td></tr>";
 tdedit += "<tr class=\"calc_row\"><td style=\"text-align:left;\">Specical Users:</td></tr>";
 tdedit += "<tr class=\"calc_row\"><td><select multiple id=\"calc_user_list\" style=\"width:250px\" name=\"calc_users_val\" size=10 >";
 tdedit += special_user_select;
 tdedit += "</select></td></tr>";
 tdedit += "<tr class=\"user_row\"><td style=\"text-align:left;\">Users:</td></tr>";
 tdedit += "<tr class=\"user_row\"><td><select multiple id=\"avail_user_list\" style=\"width:250px\" name=\"avail_users_val\" size=10 >";
 tdedit += avail_user_select;
 tdedit += "</select></td></tr>";
 tdedit += "<tr class=\"group_row\"><td style=\"text-align:left;\">Groups:</td></tr>";
 tdedit += "<tr class=\"group_row\"><td><select multiple id=\"avail_group_list\" style=\"width:250px\" name=\"avail_groups_val\" size=10 >";
 tdedit += avail_group_select;
 tdedit += "</select></td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
 {
  open : function(event, ui)
  {
   ToggleToType();
  }
 });

 pwd.dialog("option", "title", "Add Recipient");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "280px");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}

function DeleteEmailToButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var key = $(this).find("td").eq(0).attr("id");
  var t = key.substring(0, 2);
  key = key.substring(2);

  saveRes = $.ajax(
  {
   async : true,
   url : "TemplateDetails?f=" + t + "&rid=" + key + "&id=" + summSaveobjid,
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
   }
  });
 });

 saveRes.success(function(data)
 {
  LoadEmailToData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
 });

 saveRes.fail(function(jqXHR, textStatus)
 {
  alert("Request failed: " + textStatus);
 });
}

function SaveEmailToData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
{
 var form = instance.find("#EmailToform");
 var viewArr = form.serializeArray();
 var view =
 {};

 console.log(viewArr);

 for ( var i in viewArr)
 {
  if (view[viewArr[i].name] === undefined)
   view[viewArr[i].name] = viewArr[i].value + ";";
  else
   view[viewArr[i].name] = view[viewArr[i].name] + viewArr[i].value + ";";
 }

 var srvid = "";
 var f = "";

 if (view['totype'] == "Calculated;")
 {
  srvlist = $("#calc_user_list").val();
  f = "as";
 }
 else if (view['totype'] == "User;")
 {
  srvlist = $("#avail_user_list").val();
  f = "au";
 }
 else
 {
  srvlist = $("#avail_group_list").val();
  f = "ag";
 }
 var len = srvlist.length;
 for (var i = 0; i < len; i++)
 {
  if (srvlist[i].length == 0)
   continue;

  srvid = srvlist[i].substring(2);
  console.log("TemplateDetails?f=" + f + "&rid=" + srvid + "&id=" + summSaveobjid);

  $.ajax(
  {
   async : false,
   url : "TemplateDetails?f=" + f + "&rid=" + srvid + "&id=" + summSaveobjid,
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
   }
  });
 }
 LoadEmailToData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);

}

function ToggleToType()
{
 var val = $("#totype").val();

 if (val == "Calculated")
 {
  $(".calc_row").show();
  $(".user_row").hide();
  $(".group_row").hide();
 }
 else if (val == "User")
 {
  $(".calc_row").hide();
  $(".user_row").show();
  $(".group_row").hide();
 }
 else
 {
  $(".calc_row").hide();
  $(".user_row").hide();
  $(".group_row").show();
 }
}
