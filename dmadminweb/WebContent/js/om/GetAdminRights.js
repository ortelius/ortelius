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
function LoadAdminRightsData(objtypeAsInt, objtype, objid)
{
	summSaveobjtypeAsInt = objtypeAsInt;
	summSaveobjtype = objtype;
	summSaveobjid = objid;

	$.getJSON('GroupDetails', "gid=" + objid + "&type=AdminRights", function(data) {
		saveAdminRights = new Array();
		for (a = 0; a < data.length; a++) {
			saveAdminRights.push(data[a].key);
			console.log(data[a]);
			$("#" + data[a].key).prop('checked', (data[a].checked.toLowerCase() == "checked"));
			$("#" + data[a].key).prop('disabled', (data[a].disabled.toLowerCase() == "disabled"));
			$("#" + data[a].key).off("click").click(function() {
				SaveAdminRightsData();;
			});
		}
	});
}

function SaveAdminRightsData()
{
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
