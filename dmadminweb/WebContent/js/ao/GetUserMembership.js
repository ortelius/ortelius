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

function LoadUserMembershipData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#usermembership_header_buttons > button.delete_button').css("color", "grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 usermembership_tdedit = "";
 usermembership_rowcnt = 0;

 $.ajax(
 {
  url : "GroupDetails?gid=" + summSaveobjid + "&type=AvailUsers4Group",
  async : false,
  dataType : 'json',
  type : 'GET',
  success : function(data)
  {
   console.log(data);
   usermembershipdropdown = "";

   for (a = 0; a < data.length; a++)
   {
    if (!data[a].name.startsWith(userdom))
     continue;

    usermembershipdropdown += "<option value=\"us" + data[a].id + "\">" + data[a].name + "</option>";
   }
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });

 $.getJSON('GroupDetails', "gid=" + objid + "&type=Users4Group", function(data)
 {
  var td = "";
  var rowcnt = 0;

  for (a = 0; a < data.users.length; a++)
  {
   key = "us" + data.users[a].id;
   val = data.users[a].name;
   rowcnt++;

   usermembershipdropdown += "<option value=\"" + key + "\">" + val + "</option>";
   usermembership_tdedit += '<tr id="usermembership_row_' + rowcnt + '">';
   usermembership_tdedit += '<td><input type="checkbox" id="usermembership_edit_cb_' + rowcnt + '" onchange="toggleUserMembershipDelete(this);" />&nbsp;</td>';
   usermembership_tdedit += '<td><select id="usermembership_key_' + rowcnt + '" value="' + key + '">' + usermembershipdropdown + '</select></td>';
   usermembership_tdedit += "<td><input type=\"hidden\" id=\"usermembership_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
   usermembership_tdedit += "<td><input type=\"hidden\" id=\"usermembership_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
   td += '<tr><td><input type="checkbox" id="usermembership_cb_' + rowcnt + '" onchange="toggleUserMembershipDelete(this);" />&nbsp;</td><td>' + val + '</td></tr>';
  }
  usermembership_rowcnt = rowcnt;

  $("#" + tablename + " > tbody").empty().append(td);
  $("#" + tablename + "-edit > tbody").html(usermembership_tdedit);

  rowcnt = 0;
  for (var r = 0; r < data.length; r++)
  {
   row = data[r];
   id = row.type + row.id;
   rowcnt++;

   id = $("#usermembership_oldkey_" + rowcnt).val();
   $("#usermembership_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
  }
 });
}

function AddUserMembershipRow()
{
 key = "";
 val = "";
 var td = "";
 usermembership_rowcnt++;
 button_col = "<button class=\"delete_button\" style=\"display:none;padding:0px;padding-left:16px;\" onClick=\"javascript:DeleteUserMembershipRow('" + usermembership_rowcnt
   + "')\"><i class=\"fal fa-trash\" aria-hidden=\"true\"></i></button>";

 td += '<tr id="usermembership_row_' + usermembership_rowcnt + '"><td><input type="text" id="usermembership_keytext_' + usermembership_rowcnt + '" value=""><select id="usermembership_key_'
   + usermembership_rowcnt + '" value="" style="display:none">' + usermembershipdropdown + '</select></td><td>' + button_col + '</td>';
 td += '<td style="display:none"><input type="hidden" id="usermembership_oldkey_' + usermembership_rowcnt + '" value=""></td></tr>';

 $("#usermembership > tbody").append(td);

 $("#usermembership_row_" + usermembership_rowcnt).mouseenter(function()
 {
  usermembership_rowid = $(this).attr("id");
  parts = usermembership_rowid.split('_');
  rowid = parts.pop();
  $("#usermembership_keytext_" + rowid).hide();
  $("#usermembership_key_" + rowid).show();
  $("#" + usermembership_rowid + " > td:nth-child(2) > button").show();
 }).mouseleave(function()
 {
  usermembership_rowid = $(this).attr("id");
  $("#" + usermembership_rowid + " > td:nth-child(2) > button").hide();
  parts = usermembership_rowid.split('_');
  rowid = parts.pop();

  $("#usermembership_keytext_" + rowid).show();
  $("#usermembership_key_" + rowid).hide();
  SaveUserMembershipData(rowid);
 });
}

function DeleteUserMembershipData(key, val, tablename)
{
 oldid = key;

 if (oldid != "")
 {
  rid = oldid.substring(2);

  $.ajax(
  {
   url : "GroupDetails?gid=" + summSaveobjid + "&id=" + rid + "&f=r",
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
    LoadUserMembershipData("usermembership", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   }
  });
 }
}

function SaveUserMembershipData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  id = $("#usermembership_key_" + rowid).val();
  oldid = $("#usermembership_oldkey_" + rowid).val();

  if (id == null || oldid == null || id == oldid)
   return;

  $("#usermembership_key_" + rowid).val(id);
  $("#usermembership_oldkey_" + rowid).val(id);

  var rid = oldid.substring(2);
  var srvid = id.substring(2);

  $.ajax(
  {
   url : "GroupDetails?gid=" + summSaveobjid + "&id=" + rid + "&f=r",
   type : 'POST',
   async : false,
   dataType : "json",
   success : function(data)
   {
   }
  });

  $.ajax(
  {
   url : "GroupDetails?id=" + srvid + "&gid=" + summSaveobjid + "&f=a",
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
    LoadUserMembershipData("usermembership", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   }
  });
 });
}

function AddUserMembershipRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditUserMembershipRow(tablename);

 key = "";
 val = "";
 usermembership_rowcnt++;

 var td = '<tr id="usermembership_row_' + usermembership_rowcnt + '">';
 td += '<td><input type="checkbox" id="usermembership_edit_cb_' + usermembership_rowcnt + '" onchange="toggleUserMembershipDelete(this);" />&nbsp;</td>';
 td += '<td><select id="usermembership_key_' + usermembership_rowcnt + '" value="' + val + '">' + usermembershipdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"usermembership_oldkey_" + usermembership_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"usermembership_oldval_" + usermembership_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelUserMembershipRow(tablename)
{
 if ($('#usermembership_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#usermembership_header_buttons > button.save_button').css("color", "grey");
 $('#usermembership_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditUserMembershipRow(tablename)
{
 $('#usermembership_header_buttons > button.save_button').css("color", "#3367d6");
 $('#usermembership_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(usermembership_tdedit);

 for (var r = 0; r < usermembership_rowcnt; r++)
 {
  var rowcnt = r + 1;
  id = $("#usermembership_oldkey_" + rowcnt).val();
  $("#usermembership_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteUserMembershipRow(tablename)
{
 if ($('#usermembership_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#usermembership_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#usermembership_oldkey_" + rowid).val();
   val = $("#usermembership_oldval_" + rowid).val();

   if (key != "")
    DeleteUserMembershipData(key, val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#usermembership_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#usermembership_oldkey_" + rowid).val();
   val = $("#usermembership_oldval_" + rowid).val();

   if (key != "")
    DeleteUserMembershipData(key, val, tablename);
  }
 });
}

function SaveUserMembershipRow(tablename)
{
 if ($('#usermembership_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#usermembership_header_buttons > button.save_button').css("color", "grey");
 $('#usermembership_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveUserMembershipData(tablename);
}

function toggleUserMembershipDelete(e)
{
 if (e.checked)
  $('#usermembership_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#usermembership_header_buttons > button.delete_button').css("color", "grey");
}
