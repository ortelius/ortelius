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

function LoadEmailToData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#emailto_header_buttons > button.delete_button').css("color", "grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 emailto_tdedit = "";
 emailto_rowcnt = 0;

 $.getJSON('GetNotifyRecipientList', "id=" + objid, function(data)
 {
  var td = "";
  var tdedit = "";
  var rowcnt = 0;

  console.log(data);
  emailtodropdown = "";

  for (a = 0; a < data.Available.length; a++)
  {
//   found = false;
//   for (i = 0; i < data.Recipients.length; i++)
//   {
//    if (data.Recipients[i].id == data.Available[a].id)
//    {
//     found = true;
//     break;
//    }
//   }
//   if (!found)
    emailtodropdown += "<option value=\"" + data.Available[a].id + "\">" + data.Available[a].name + "</option>";
  }

  for (a = 0; a < data.Recipients.length; a++)
  {
   var key = data.Recipients[a].id;
   var val = data.Recipients[a].name;

   rowcnt++;

   emailto_tdedit += '<tr id="emailto_row_' + rowcnt + '">';
   emailto_tdedit += '<td><input type="checkbox" id="emailto_edit_cb_' + rowcnt + '" onchange="toggleEmailToDelete(this);" />&nbsp;</td>';
   emailto_tdedit += '<td><select id="emailto_key_' + rowcnt + '" value="' + key + '">' + emailtodropdown + '</select></td>';
   emailto_tdedit += "<td><input type=\"hidden\" id=\"emailto_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
   emailto_tdedit += "<td><input type=\"hidden\" id=\"emailto_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
   td += '<tr><td><input type="checkbox" id="emailto_cb_' + rowcnt + '" onchange="toggleEmailToDelete(this);" />&nbsp;</td><td>' + val + '</td></tr>';
  }
  emailto_rowcnt = rowcnt;

  $("#" + tablename + " > tbody").empty().append(td);
  $("#" + tablename + "-edit > tbody").html(emailto_tdedit);

  rowcnt = 0;
  for (var r = 0; r < data.length; r++)
  {
   row = data[r];
   id = row.type + row.id;
   rowcnt++;

   id = $("#emailto_oldkey_" + rowcnt).val();
   $("#emailto_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
  }
 });
}

function DeleteEmailToData(key, val, tablename)
{
 oldid = key;

 if (oldid != "")
 {
  var rid = oldid.substring(2);
  var rf = oldid.substring(0, 2);
  rf = rf.replace("a", "r");

  console.log("TemplateDetails?f=" + rf + "&rid=" + rid + "&id=" + summSaveobjid);

  $.ajax(
  {
   async : false,
   url : "TemplateDetails?f=" + rf + "&rid=" + rid + "&id=" + summSaveobjid,
   type : 'POST',
   dataType : "json",
   success : function(data)
   {
   }
  });
 }

 LoadEmailToData("emailto", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function SaveEmailToData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  id = $("#emailto_key_" + rowid).val();
  oldid = $("#emailto_oldkey_" + rowid).val();

  if (id == null || oldid == null || id == oldid)
   return;

  $("#emailto_key_" + rowid).val(id);
  $("#emailto_oldkey_" + rowid).val(id);

  var srvid = id.substring(2);

  var f = id.substring(0, 2);

  if (oldid != "")
  {
   var rid = oldid.substring(2);
   var rf = oldid.substring(0, 2);
   rf = rf.replace("a", "r");

   console.log("TemplateDetails?f=" + rf + "&rid=" + rid + "&id=" + summSaveobjid);

   $.ajax(
   {
    async : false,
    url : "TemplateDetails?f=" + rf + "&rid=" + rid + "&id=" + summSaveobjid,
    type : 'POST',
    dataType : "json",
    success : function(data)
    {
    }
   });
  }
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
 });
 LoadEmailToData("emailto", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function AddEmailToRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditEmailToRow(tablename);

 key = "";
 val = "";
 emailto_rowcnt++;

 var td = '<tr id="emailto_row_' + emailto_rowcnt + '">';
 td += '<td><input type="checkbox" id="emailto_edit_cb_' + emailto_rowcnt + '" onchange="toggleEmailToDelete(this);" />&nbsp;</td>';
 td += '<td><select id="emailto_key_' + emailto_rowcnt + '" value="' + val + '">' + emailtodropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"emailto_oldkey_" + emailto_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"emailto_oldval_" + emailto_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelEmailToRow(tablename)
{
 if ($('#emailto_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#emailto_header_buttons > button.save_button').css("color", "grey");
 $('#emailto_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditEmailToRow(tablename)
{
 $('#emailto_header_buttons > button.save_button').css("color", "#3367d6");
 $('#emailto_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(emailto_tdedit);

 for (var r = 0; r < emailto_rowcnt; r++)
 {
  var rowcnt = r + 1;
  id = $("#emailto_oldkey_" + rowcnt).val();
  $("#emailto_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteEmailToRow(tablename)
{
 if ($('#emailto_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#emailto_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#emailto_oldkey_" + rowid).val();
   val = $("#emailto_oldval_" + rowid).val();

   if (key != "")
    DeleteEmailToData(key, val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#emailto_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#emailto_oldkey_" + rowid).val();
   val = $("#emailto_oldval_" + rowid).val();

   if (key != "")
    DeleteEmailToData(key, val, tablename);
  }
 });
}

function SaveEmailToRow(tablename)
{
 if ($('#emailto_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#emailto_header_buttons > button.save_button').css("color", "grey");
 $('#emailto_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveEmailToData(tablename);
}

function toggleEmailToDelete(e)
{
 if (e.checked)
  $('#emailto_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#emailto_header_buttons > button.delete_button').css("color", "grey");
}
