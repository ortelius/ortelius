/*
 *
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

function LoadGroupMembershipData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#groupmembership_header_buttons > button.delete_button').css("color", "grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 groupmembership_tdedit = "";
 groupmembership_rowcnt = 0;
 
 $.ajax(
   {
    url : "UserDetails?uid=" + summSaveobjid + "&type=AvailGroups4User",
    async : false,
    dataType : 'json',
    type : 'GET',
    success : function(data)
    {
     console.log(data);
     groupmembershipdropdown = "";

     for (a = 0; a < data.groups.length; a++)
     {
      if (!data.groups[a].name.startsWith(userdom))
       continue;
      
      groupmembershipdropdown += "<option value=\"gr" + data.groups[a].id + "\">" + data.groups[a].name + "</option>";
     }
    },
    error : function(jqxhr, status, err) {
     console.log(status);
     console.log(err);
    }
   }); 
 

 $.getJSON('UserDetails', "uid=" + objid + "&type=Groups4User", function(data)
 {
  var td = "";
  var rowcnt = 0;
  
  for (a = 0; a < data.groups.length; a++)
  {
    key = "gr" + data.groups[a].id;
    val = data.groups[a].name;
    rowcnt++;

    groupmembershipdropdown += "<option value=\"gr" + data.groups[a].id + "\">" + data.groups[a].name + "</option>";
    groupmembership_tdedit += '<tr id="groupmembership_row_' + rowcnt + '">';
    groupmembership_tdedit += '<td><input type="checkbox" id="groupmembership_edit_cb_' + rowcnt + '" onchange="toggleGroupMembershipDelete(this);" />&nbsp;</td>';
    groupmembership_tdedit += '<td><select id="groupmembership_key_' + rowcnt + '" value="' + key + '">' + groupmembershipdropdown + '</select></td>';
    groupmembership_tdedit += "<td><input type=\"hidden\" id=\"groupmembership_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
    groupmembership_tdedit += "<td><input type=\"hidden\" id=\"groupmembership_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
    td += '<tr><td><input type="checkbox" id="groupmembership_cb_' + rowcnt + '" onchange="toggleGroupMembershipDelete(this);" />&nbsp;</td><td>' + val + '</td></tr>';
   }
   groupmembership_rowcnt = rowcnt;

   $("#" + tablename + " > tbody").empty().append(td);
   $("#" + tablename + "-edit > tbody").html(groupmembership_tdedit);

   rowcnt = 0;
   for (var r = 0; r < data.length; r++)
   {
    row = data[r];
    id = row.type + row.id;
    rowcnt++;

    id = $("#groupmembership_oldkey_" + rowcnt).val();
    $("#groupmembership_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
   }
 });
}


function DeleteGroupMembershipData(key, val, tablename)
{
 oldid = key;

 if (oldid != "")
 {
  rid = oldid.substring(2);

   $.ajax(
    {
      url: "UserDetails?id=" + summSaveobjid + "&gid="+ rid + "&f=r",
      type: 'POST',
      dataType: "json",
      success: function (data) {
       LoadGroupMembershipData("groupmembership", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
      }
    });
  }
}

function SaveGroupMembershipData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  id = $("#groupmembership_key_" + rowid).val();
  oldid = $("#groupmembership_oldkey_" + rowid).val();

  if (id == null || oldid == null || id == oldid)
   return;

  $("#groupmembership_key_" + rowid).val(id);
  $("#groupmembership_oldkey_" + rowid).val(id);

  var rid = oldid.substring(2);
  var srvid = id.substring(2);

  $.ajax(
    {
      url: "UserDetails?id=" + summSaveobjid + "&gid="+ rid + "&f=r",
      type: 'POST',
      async: false,
      dataType: "json",
      success: function (data) {
      }
    });
  
  $.ajax(
    {
     url: "UserDetails?gid="+ srvid + "&id=" + summSaveobjid + "&f=a",
     type: 'POST',
     dataType: "json",
     success: function(data)
     {
      LoadGroupMembershipData("groupmembership", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
     }
    });
 });
}



function AddGroupMembershipRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditGroupMembershipRow(tablename);

 key = "";
 val = "";
 groupmembership_rowcnt++;

 var td = '<tr id="groupmembership_row_' + groupmembership_rowcnt + '">';
 td += '<td><input type="checkbox" id="groupmembership_edit_cb_' + groupmembership_rowcnt + '" onchange="toggleGroupMembershipDelete(this);" />&nbsp;</td>';
 td += '<td><select id="groupmembership_key_' + groupmembership_rowcnt + '" value="' + val + '">' + groupmembershipdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"groupmembership_oldkey_" + groupmembership_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"groupmembership_oldval_" + groupmembership_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelGroupMembershipRow(tablename)
{
 if ($('#groupmembership_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#groupmembership_header_buttons > button.save_button').css("color", "grey");
 $('#groupmembership_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditGroupMembershipRow(tablename)
{
 $('#groupmembership_header_buttons > button.save_button').css("color", "#3367d6");
 $('#groupmembership_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(groupmembership_tdedit);

 for (var r = 0; r < groupmembership_rowcnt; r++)
 {
  var rowcnt = r + 1;
  id = $("#groupmembership_oldkey_" + rowcnt).val();
  $("#groupmembership_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteGroupMembershipRow(tablename)
{
 if ($('#groupmembership_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#groupmembership_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#groupmembership_oldkey_" + rowid).val();
   val = $("#groupmembership_oldval_" + rowid).val();

   if (key != "")
    DeleteGroupMembershipData(key, val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#groupmembership_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#groupmembership_oldkey_" + rowid).val();
   val = $("#groupmembership_oldval_" + rowid).val();

   if (key != "")
    DeleteGroupMembershipData(key, val, tablename);
  }
 });
}

function SaveGroupMembershipRow(tablename)
{
 if ($('#groupmembership_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#groupmembership_header_buttons > button.save_button').css("color", "grey");
 $('#groupmembership_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveGroupMembershipData(tablename);
}

function toggleGroupMembershipDelete(e)
{
 if (e.checked)
  $('#groupmembership_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#groupmembership_header_buttons > button.delete_button').css("color", "grey");
}
