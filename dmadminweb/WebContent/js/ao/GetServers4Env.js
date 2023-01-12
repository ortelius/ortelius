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

function EditSrv4EnvRow(tablename)
{
 $('#srv4env_header_buttons > button.save_button').css("color","#3367d6");
 $('#srv4env_header_buttons > button.cancel_button').css("color","#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(srv4env_tdedit);

 for (i = 0; i < srv4env_rowcnt; i++)
 {
  var row = i + 1;
  var sel = $("#srv4env_val_" + row).val();
  $("#srv4env_key_" + row).val(sel).change();
 }

 $("#" + tablename + "-edit").show();
}

function AddSrv4EnvRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
   EditSrv4EnvRow(tablename);

 var key = "";
 var val = "";
 var arrid = "";
 var td = "";
 srv4env_rowcnt++;

td += '<tr id="srv4env_row_' + srv4env_rowcnt + '">';
td += '<td><input type="checkbox" id="srv4env_edit_cb_' + srv4env_rowcnt + '" onchange="toggleSrv4EnvDelete(this);" />&nbsp;</td>';
td += '<td><select id="srv4env_key_' + srv4env_rowcnt + '" value="">' + srvdropdown + '</select></td><td></td>';
td += '<td style="display:none"><input type="hidden" id="srv4env_val_' + srv4env_rowcnt + '" value=""></td>';
td += '<td style="display:none"><input type="hidden" id="srv4env_oldkey_' + srv4env_rowcnt + '" value=""></td></tr>';

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelSrv4EnvRow(tablename)
{
 if ($('#srv4env_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#srv4env_header_buttons > button.save_button').css("color","grey");
 $('#srv4env_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function SaveSrv4EnvRow(tablename)
{
 if ($('#srv4env_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#srv4env_header_buttons > button.save_button').css("color","grey");
 $('#srv4env_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveSrv4EnvData(tablename);
}

function DeleteSrv4EnvRow(tablename)
{
 if ($('#srv4env_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb  = $("#srv4env_edit_cb_" + rowid);

  if (cb.is( ":checked" ) == true)
  {
   DeleteSrv4Env(rowid);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;

    cb  = $("#srv4env_cb_" + rowid);

    if (cb.is( ":checked" ) == true)
    {
     DeleteSrv4Env(rowid);
    }
   });

 LoadSrv4EnvData("srv4env", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function toggleSrv4EnvDelete(e)
{
  if (e.checked)
   $('#srv4env_header_buttons > button.delete_button').css("color","#3367d6");
  else
   $('#srv4env_header_buttons > button.delete_button').css("color","grey");
}


function LoadSrv4EnvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 srv4env_tdedit = "";

 $('#srv4env_header_buttons > button.delete_button').css("color","grey");

 if (srvdropdown == "")
 {
  $.ajax(
  {
   url: "ReportsData?type=EndPointList",
   dataType: 'json',
   type: 'GET',
   async: false,
   success: function(res)
   {
    console.log(res);

    data = res.data.sort(compareConcatName);

    for (n = 0; n < data.length; n++)
    {
     srvdropdown += '<option value=\"' + data[n].id + "\">" + data[n].domain + "." + data[n].name + '</option>';
    }
   },
   error: function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
 }

 $.getJSON('GetServerLayout', "envid=" + objid, function(data)
 {
  var td = "";
  var rowcnt = 0;

  Nodes = data.Nodes.sort(compareName);

  for (a = 0; a < Nodes.length; a++)
  {
   rowcnt++;

   srv4env_tdedit += '<tr id="srv4env_row_' + rowcnt + '">';
   srv4env_tdedit += '<td><input type="checkbox" id="srv4env_edit_cb_' + rowcnt + '" onchange="toggleSrv4EnvDelete(this);" />&nbsp;</td>';
   srv4env_tdedit += '<td><select id="srv4env_key_' + rowcnt + '" value="se' + Nodes[a].nodeid + '">' + srvdropdown + '</select></td><td>' + Nodes[a].hostname + '</td>';
   srv4env_tdedit += '<td style="display:none"><input type="hidden" id="srv4env_val_' + rowcnt + '" value="se' + Nodes[a].nodeid + '"></td>';
   srv4env_tdedit += '<td style="display:none"><input type="hidden" id="srv4env_oldkey_' + rowcnt + '" value="se' + Nodes[a].nodeid + '"></td></tr>';
   td += '<tr><td><input type="checkbox" id="srv4env_cb_' + rowcnt + '" onchange="toggleSrv4EnvDelete(this);" />&nbsp;</td><td>' + Nodes[a].name  + '</td><td>' + Nodes[a].hostname + '</td>';
  }

  srv4env_rowcnt = rowcnt;

  $("#" + tablename + " > tbody").html(td);
  $("#" + tablename + "-edit > tbody").html(srv4env_tdedit);
 });
}

function SaveSrv4EnvData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

 id = $("#srv4env_key_" + rowid).val();
 oldid = $("#srv4env_oldkey_" + rowid).val();

 if (id == null || oldid == null || id == oldid)
  return;

 $("#srv4env_key_" + rowid).val(id);
 $("#srv4env_oldkey_" + rowid).val(id);

 srvid = id.substring(2);
 key = oldid.substring(2);

 $.ajax(
 {
  url: "UpdateAttrs?f=ds&e=" + summSaveobjid + "&w=" + key,
  type: 'GET',
  async: false,
  dataType: "text",
  success: function(res)
  {
   $.ajax(
     {
      url: "UpdateAttrs?f=sm&e=" + summSaveobjid + "&s=" + srvid + "&xpos=20&ypos=20",
      type: 'GET',
      dataType: "text",
      async: false,
      success: function(data) {
      }
     })
  },
  error: function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
 });

 LoadSrv4EnvData("srv4env", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function DeleteSrv4Env(rowid)
{
 id = $("#srv4env_oldkey_" + rowid).val();
 key = id.substring(2);

 $.ajax(
 {
  url: "UpdateAttrs?f=ds&e=" + summSaveobjid + "&w=" + key,
  type: 'GET',
  dataType: "text",
  async: false,
  success: function(res)
  {
  },
  error: function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
}
