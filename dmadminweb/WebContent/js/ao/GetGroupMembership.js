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
