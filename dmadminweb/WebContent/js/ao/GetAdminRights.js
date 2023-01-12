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

function LoadAdminRightsData(objtypeAsInt, objtype, objid)
{
	summSaveobjtypeAsInt = objtypeAsInt;
	summSaveobjtype = objtype;
	summSaveobjid = objid;

	var ar = {};
	ar["ar_override"] = "Override Access Control";
 ar["ar_cusers"] = "Users";
 ar["ar_cgroups"] = "User Groups";
 ar["ar_cdomain"] = "Domains";
 ar["ar_cenv"] = "Environments";
 ar["ar_cserver"] = "Endpoints";
 ar["ar_crepo"] = "Repositories";
 ar["ar_ccomp"] = "Components";
 ar["ar_ccred"] = "Credentials";
 ar["ar_capp"] = "Applications";
 ar["ar_cappver"] = "Application Versions";
 ar["ar_caction"] = "Actions";
 ar["ar_cproc"] = "Procedures";
 ar["ar_cdsrc"] = "Data Sources";
 ar["ar_cnotify"] = "Notifiers";
 ar["ar_cengine"] = "CD Engines";
//  ar["ar_cservercomptype"] = "Endpoint Types";

 $.getJSON('GroupDetails', "gid=" + objid + "&type=AdminRights", function(data) {
  var title = "";
  var tdedit = "<form id=\"adminrightsform\"><table id=\"adminrightstab\" class=\"dev_table\"><tbody>";
  var td = "";

  for (var r = 0; r < data.length; r++)
  {
   var myid = data[r].key;

   if (!(myid in ar))
    continue;

   var checked = data[r].checked;
   var disabled = data[r].disabled;
   var val = "No";
   var label = ar[myid];

   if (checked.toLowerCase() == "checked")
    val = "Yes";

   td += "<tr id=\"" + myid + "_row\" ><td class=\"summlabel\">";
   td += label;
   td += ":</td><td>";
   td += val;
   td += "</tr>";

   if (disabled)
   {
    tdedit += "<tr>";
    tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
    tdedit += "<td>No</td>";
   }
   else
   {
    tdedit += "<tr>";
    tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
    if (val == "Yes")
      tdedit += "<td><input type=\"checkbox\" id=\"" + myid + "\" checked></td>";
    else
      tdedit += "<td><input type=\"checkbox\" id=\"" + myid + "\"></td>";
    tdedit += "<td><input type=\"hidden\" name=\"target_oldval\" value=\"" + val + "\"/></td>";
    tdedit += "</tr>";
   }
  }

  tdedit += "</tbody></table></form>";

  var pwd = parent.$("#adminrights > tbody");
  pwd.empty().append(td);

  pwd = parent.$("#adminrights_data_edit");
  pwd.empty().append(tdedit);

  saveAdminRights = new Array();
  for (a = 0; a < data.length; a++)
  {
    saveAdminRights.push(data[a].key);
    console.log(data[a]);
    $("#" + data[a].key).prop('disabled', (data[a].disabled.toLowerCase() == "disabled"));
  }
 });
 $("#adminrights_data").show();
 $("#adminrights_data_edit").hide();
}

function SaveAdminRightsData()
{

 var pwd = parent.$("#adminrights_data_edit");

 if (!pwd.is(":visible"))
  return;

 var url = "GroupDetails?gid=" + summSaveobjid + "&f=sv";

 for (i=0;i<saveAdminRights.length;i++)
 {
  var key = saveAdminRights[i];
  key = key.substring(3);

  if ($("#" + saveAdminRights[i]).is(':checked'))
   url += "&" + key + "=y";
  else
   url += "&" + key + "=n";
 }

 console.log(url);

  saveRes = $.ajax(
  {
   url : url,
   type : 'POST',
   dataType : "json"
  });

 saveRes.success(function(data)
 {
 });

 saveRes.fail(function(jqXHR, textStatus)
 {
  alert("Request failed: " + textStatus);
 });
}
