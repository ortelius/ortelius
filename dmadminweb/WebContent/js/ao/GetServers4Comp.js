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

function LoadSrv4CompData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#srv4comp_header_buttons > button.delete_button').css("color", "grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 var url = "";
 var servlet = "";
 srv4comp_tdedit = "";

 srv4comp_rowcnt = 0;

 if (srvdropdown == "")
 {
  $.ajax(
  {
   url : "ReportsData?type=EndPointList",
   dataType : 'json',
   type : 'GET',
   async : false,
   success : function(res)
   {
    console.log(res);

    data = res.data.sort(compareConcatName);

    for (n = 0; n < data.length; n++)
    {
     srvdropdown += '<option value=\"' + data[n].id + "\">" + data[n].domain + "." + data[n].name + '</option>';
    }
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
   }
  });
 }

 servlet = "GetComponentData";
 url = "compid=" + objid;

 $.getJSON(servlet, url, function(data)
 {
  var td = "";
  var rowcnt = 0;

  for (a = 0; a < data.length; a++)
  {
   var row = data[a];

 
   linkval1 = row.name;
   if (row.deployid == 0)
    linkval2 = "Manually Deployed";
   else
    linkval2 = "<a href='javascript:DisplayDeploy(\"" + row.deployid + "\");'>#" + row.deployid + "</a>";

   rowcnt++;

   var key = row.name;
   var val = "se" + row.id;

   srv4comp_tdedit += '<tr id="srv4comp_row_' + rowcnt + '">';
   srv4comp_tdedit += '<td><input type="checkbox" id="srv4comp_edit_cb_' + rowcnt + '" onchange="toggleSrv4CompDelete(this);" />&nbsp;</td>';
   srv4comp_tdedit += '<td><select id="srv4comp_key_' + rowcnt + '" value="' + val + '">' + srvdropdown + '</select></td>';
   srv4comp_tdedit += "<td><input type=\"hidden\" id=\"srv4comp_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
   srv4comp_tdedit += "<td><input type=\"hidden\" id=\"srv4comp_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
   td += '<tr><td><input type="checkbox" id="srv4comp_cb_' + rowcnt + '" onchange="toggleSrv4CompDelete(this);" />&nbsp;</td><td>' + linkval1 + '</td><td>' + linkval2 + '</td></tr>';
   srv4comp_rowcnt = rowcnt;
  }
  $("#" + tablename + " > tbody").empty().append(td);
  $("#" + tablename + "-edit > tbody").html(srv4comp_tdedit);

  rowcnt = 0;
  for (var r = 0; r < data.length; r++)
  {
   var row = data[r];
   id = "se" + row.id;
   rowcnt++;

   id = $("#srv4comp_oldval_" + rowcnt).val();
   $("#srv4comp_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
  }
 });
}

function SaveSrv4CompData(tablename)
{
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  var id = $("#srv4comp_key_" + rowid).val();
  var oldid = $("#srv4comp_oldval_" + rowid).val();

  if (id == null || id == oldid)
   return;

  $("#srv4comp_key_" + rowid).val(id);
  $("#srv4comp_oldval_" + rowid).val(id);

  srvid = id.substring(2);
  oldid = oldid.substring(2);

  if (objtype == "cv")
   isVersion = "true";
  else
   isVersion = "false";

  if (isVersion)
   url = "UpdateAttrs?f=scam&c=" + objid + "&s=" + srvid + "&xpos=20&ypos=20&ol";
  else
   url = "UpdateAttrs?f=scvm&c=" + objid + "&s=" + srvid + "&xpos=20&ypos=20";

  console.log(url);

  $.ajax(
  {
   url : url,
   type : 'GET',
   dataType : "text",
   success : function(data)
   {
    url = "UpdateAttrs?f=scd&c=" + objid + "&s=" + oldid;
    $.ajax(
    {
     url : url,
     type : 'GET',
     dataType : "text",
     success : function(data)
     {
      LoadSrv4CompData(tablename, objtypeAsInt, objtype, objid, "");
     }
    });
   }
  });
 });
}

function LoadBuildDataForComponent(divname, objtypeAsInt, objtype, objid, addParams)
{
 var url = "";
 var servlet = "";
 var duplist = new Array();

 servlet = "GetComponentLayout";
 url = "compid=" + objid + "&t=b";

 $.getJSON(servlet, url, function(res)
 {
  console.log("Build Info:");
  console.log(res);
  data = res.data;
  console.log("data.length is " + data.length);
  var headerPrinted = false;
  var td = "";

  if (data.length == 0)
   td = "<br><h3>No Builds Found</h3>";

  for (a = 0; a < data.length; a++)
  {
   var row = data[a];
   console.log(row);
   if (typeof row[0].id == "undefined")
    continue;
   console.log("row[0].id=" + row[0].id);
   console.log("row[0].name=" + row[0].name);
   console.log("row[1]=" + row[1]);
   console.log("row[2]=" + row[2]);
   linkval1 = "<a href='javascript:SwitchDisplay(\"" + row[1].type + row[1].id + "\");'>" + row[1].name + "</a>";
   linkval2 = "<a href='javascript:SwitchDisplay(\"" + row[0].type + row[0].id + "\");'>" + row[0].name + "</a>";
   if (!headerPrinted)
   {
    td += "<h3>Builds:</h3>";
    td += "<table cellspacing=\"0\" width=\"100%\">";
    td += "<thead class=\"ui-widget-header\">";
    td += "<tr><th align=\"left\" style=\"width:100px;\">Build Number</th><th align=\"left\" style=\"width:150px\">Date/Time</th><th align=\"left\">Component</th><th>Build Job</th></tr>";
    td += "</thead>";
    td += "<tbody>";
    headerPrinted = true;
   }
   console.log("row[4]=" + row[4]);
   var icon = "";
   var success;
   if (row[4] == "Y")
   {
    icon = "<img src=\"images/blue_small.png\" style=\"vertical-align:middle;\">&nbsp;";
    success = true;
   }
   else
   {
    icon = "<img src=\"images/red_small.png\" style=\"vertical-align:middle;\">&nbsp;";
    success = false;
   }

   td += "<tr><td style=\"border: 1px solid #ddd;\">" + icon + "<a onClick=\"javascript:DisplayBuild(" + objid + "," + row[2] + "," + success + ");\">#" + row[2] + "</a></td>";
   td += "<td style=\"border: 1px solid #ddd;\">" + row[3] + "</td>";
   td += "<td style=\"border: 1px solid #ddd;\">" + linkval1 + "</td>";
   td += "<td style=\"border: 1px solid #ddd;\">" + linkval2 + "</td></tr>";
  }
  if (headerPrinted)
  {
   td + "</tbody></table>";
  }
  $("#" + divname).html(td);
 });
}

function AddSrv4CompRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditSrv4CompRow(tablename);

 key = "";
 val = "";
 srv4comp_rowcnt++;

 var td = '<tr id="srv4comp_row_' + srv4comp_rowcnt + '">';
 td += '<td><input type="checkbox" id="srv4comp_edit_cb_' + srv4comp_rowcnt + '" onchange="toggleSrv4CompDelete(this);" />&nbsp;</td>';
 td += '<td><select id="srv4comp_key_' + srv4comp_rowcnt + '" value="' + val + '">' + srvdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"srv4comp_oldkey_" + srv4comp_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"srv4comp_oldval_" + srv4comp_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function DeleteSrv4CompData(key, val, tablename)
{
 val = val.substring(2);

 $.ajax(
 {
  url : "UpdateAttrs?f=scd&c=" + summSaveobjid + "&s=" + val,
  async : false,
  type : 'GET',
  dataType : "text",
  async : false,
  success : function(data)
  {
  }
 });

 LoadSrv4CompData("srv4comp", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}

function CancelSrv4CompRow(tablename)
{
 if ($('#srv4comp_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#srv4comp_header_buttons > button.save_button').css("color", "grey");
 $('#srv4comp_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditSrv4CompRow(tablename)
{
 $('#srv4comp_header_buttons > button.save_button').css("color", "#3367d6");
 $('#srv4comp_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(srv4comp_tdedit);

 for (var r = 0; r < srv4comp_rowcnt; r++)
 {
  var rowcnt = r + 1;
  id = $("#srv4comp_oldval_" + rowcnt).val();
  $("#srv4comp_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteSrv4CompRow(tablename)
{
 if ($('#srv4comp_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#srv4comp_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#srv4comp_oldkey_" + rowid).val();
   val = $("#srv4comp_oldval_" + rowid).val();

   if (key != "")
    DeleteSrv4CompData(key, val, tablename);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#srv4comp_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   key = $("#srv4comp_oldkey_" + rowid).val();
   val = $("#srv4comp_oldval_" + rowid).val();

   if (key != "")
    DeleteSrv4CompData(key, val, tablename);
  }
 });
}

function SaveSrv4CompRow(tablename)
{
 if ($('#srv4comp_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#srv4comp_header_buttons > button.save_button').css("color", "grey");
 $('#srv4comp_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveSrv4CompData(tablename);
}

function toggleSrv4CompDelete(e)
{
 if (e.checked)
  $('#srv4comp_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#srv4comp_header_buttons > button.delete_button').css("color", "grey");
}
