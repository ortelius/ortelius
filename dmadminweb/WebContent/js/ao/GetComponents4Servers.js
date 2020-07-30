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

function compareCompRows(a, b)
{
 var aname = a.name;
 var bname = b.name;
 return aname.localeCompare(bname);
}

function compareConcatName(a, b)
{
 var aname = a.domain + "." + a.name;
 var bname = b.domain + "." + b.name;
 return aname.localeCompare(bname);
}

function compareName(a, b)
{
 return a.name.localeCompare(b.name);
}

function LoadComp4SrvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#comp4srv_header_buttons > button.delete_button').css("color","grey"); 
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 comp4srv_tdedit = "";
 comp4srv_rowcnt = 0;

 if (compdropdown == "")
 {
  $.ajax(
  {
   url : "ReportsData?type=CompList",
   dataType : 'json',
   type : 'GET',
   async : false,
   success : function(res)
   {
    data = res.data.sort(compareConcatName);

    for (n = 0; n < data.length; n++)
    {
     compdropdown += '<option value=\"' + data[n].id + "\">" + data[n].domain + "." + data[n].name + '</option>';
    }
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
 }

 var url = "";
 var servlet = "";

 servlet = "GetCompsOnEndPoint";
 url = "servid=" + objid;

 $.getJSON(servlet, url, function(res)
 {
  var td = "";
  var rowcnt = 0;

  data = res.sort(compareCompRows);
  console.log(data);

  for (a = 0; a < data.length; a++)
  {
   row = data[a];
   rowcnt++;

   var key = row.type + row.id;
   var val = row.name;

   buildnumber = row.buildnumber;
   if (buildnumber == 0)
    buildnumber = '-'
   else
    buildnumber = "<a id=\"deployment" + buildnumber + "\" onclick=\"DisplayBuild(" + objid + "," + buildnumber + ");\">#" + buildnumber + "</a>";

   deploynum = row.deployid
   if (deploynum == 0)
    deploynum = 'Manually Assigned';
   else
    deploynum = "<a id=\"deployment" + deploynum + "\" onclick=\"DisplayDeploy(" + deploynum + ");\">#" + deploynum + "</a>";

   comp4srv_tdedit += '<tr id="comp4srv_row_' + rowcnt + '">';
   comp4srv_tdedit += '<td><input type="checkbox" id="comp4srv_edit_cb_' + rowcnt + '" onchange="toggleComp4SrvDelete(this);" />&nbsp;</td>';
   comp4srv_tdedit += '<td><select id="comp4srv_key_' + rowcnt + '" value="' + key + '">' + compdropdown + '</select></td>';
   comp4srv_tdedit += "<td><input type=\"hidden\" id=\"comp4srv_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
   comp4srv_tdedit += "<td><input type=\"hidden\" id=\"comp4srv_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
   td += '<tr><td><input type="checkbox" id="comp4srv_cb_' + rowcnt + '" onchange="toggleComp4SrvDelete(this);" />&nbsp;</td><td>' + val + '</td><td>' + deploynum
     + '</td></tr>';
  }
  comp4srv_rowcnt = rowcnt;

  $("#" + tablename + " > tbody").empty().append(td);
  $("#" + tablename + "-edit > tbody").html(comp4srv_tdedit);

  rowcnt = 0;
  for (var r = 0; r < data.length; r++)
  {
   row = data[r];
   id = row.type + row.id;
   rowcnt++;

   id = $("#comp4srv_oldkey_" + rowcnt).val();
   $("#comp4srv_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
  }
 });
}

function SaveComp4SrvData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {

  rowid++;
  id = $("#comp4srv_key_" + rowid).val();
  oldid = $("#comp4srv_oldkey_" + rowid).val();

  if (id == null || oldid == null || id == oldid)
   return;

  $("#comp4srv_key_" + rowid).val(id);
  $("#comp4srv_oldkey_" + rowid).val(id);

  srvid = id.substring(2);
  oldkey = oldid.substring(2);

  $.ajax(
  {
   url : "UpdateAttrs?f=scd&s=" + objid + "&c=" + oldkey,
   type : 'GET',
   async : false,
   dataType : "text",
   success : function(res)
   {
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    s
   }
  });

  $.ajax(
  {
   url : "UpdateAttrs?f=scam&s=" + summSaveobjid + "&c=" + srvid + "&xpos=20&ypos=20",
   type : 'GET',
   dataType : "text",
   async : false,
   success : function(data)
   {
   }
  });
 });
 LoadComp4SrvData("comp4srv", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function DeleteComp4SrvData(key, val,tablename)
{
  key = key.substring(2);

  $.ajax(
  {
   url : "UpdateAttrs?f=scd&s=" + objid + "&c=" + key,
   type : 'GET',
   dataType : "text",
   success : function(res)
   {
    LoadComp4SrvData("comp4srv", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    s
   }
  });
}

function AddComp4SrvRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
   EditComp4SrvRow(tablename);
 
 key = "";
 val = "";
 comp4srv_rowcnt++;
 
 var td = '<tr id="comp4srv_row_' + comp4srv_rowcnt + '">';
 td += '<td><input type="checkbox" id="comp4srv_edit_cb_' + comp4srv_rowcnt + '" onchange="toggleComp4SrvDelete(this);" />&nbsp;</td>';
 td += '<td><select id="comp4srv_key_' + comp4srv_rowcnt + '" value="' + val + '">' + compdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"comp4srv_oldkey_" + comp4srv_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"comp4srv_oldval_" + comp4srv_rowcnt + "\" value=\"" + val + "\"\></td></tr>";
 
 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelComp4SrvRow(tablename)
{
 if ($('#comp4srv_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#comp4srv_header_buttons > button.save_button').css("color", "grey");
 $('#comp4srv_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditComp4SrvRow(tablename)
{
 $('#comp4srv_header_buttons > button.save_button').css("color", "#3367d6");
 $('#comp4srv_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(comp4srv_tdedit);

 for (var r = 0; r < comp4srv_rowcnt; r++)
 {
  var rowcnt = r + 1;
  id = $("#comp4srv_oldkey_" + rowcnt).val();
  $("#comp4srv_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteComp4SrvRow(tablename)
{
 if ($('#comp4srv_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#comp4srv_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#comp4srv_oldkey_" + rowid).val();
   val = $("#comp4srv_oldval_" + rowid).val();

   if (key != "")
    DeleteComp4SrvData(key, val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#comp4srv_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#comp4srv_oldkey_" + rowid).val();
   val = $("#comp4srv_oldval_" + rowid).val();

   if (key != "")
    DeleteComp4SrvData(key, val, tablename);
  }
 });
}

function SaveComp4SrvRow(tablename)
{
 if ($('#comp4srv_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#comp4srv_header_buttons > button.save_button').css("color", "grey");
 $('#comp4srv_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveComp4SrvData(tablename);
}

function toggleComp4SrvDelete(e)
{
 if (e.checked)
  $('#comp4srv_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#comp4srv_header_buttons > button.delete_button').css("color", "grey");
}
