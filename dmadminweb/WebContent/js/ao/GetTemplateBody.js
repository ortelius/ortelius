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
function convertForDisplay(orig) {
    return orig
    	.replace(/&/g, "&amp;").replace(/</g, "&lt;")
    	.replace(/>/g, "&gt;").replace(/"/g, "&quot;")
    	.replace(/'/g, "&#039;").replace(new RegExp('\r?\n','g'), '<br>');;
 }

function LoadBodyData(tablename, objtypeAsInt, objtype, objid, addParams)
{

 console.log("TemplateDetails?id=" + objid);
 
 $.ajax(
   {
    url : "TemplateDetails?id=" + objid,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     summSaveobjid = objid;
     summSaveSubject = res.subject;
     summSaveBody = res.body;
     
     var title = "";
     var tdedit = "<form id=\"emailbodyform\"><table id=\"emailbodytab\" class=\"dev_table\"><tbody>";
     var td = "";
     
     // Subject
     
     field = "";
     callback = "";
     label = "Subject";
     val   = convertForDisplay(summSaveSubject);
     
     myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
     td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
     td += label;
     td += ":</td><td>";
     td += val;
     td += "</tr>";
     
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><input name=\"subject_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"subject_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"subject_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"subject_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
     
     // Body
     
     label = "Body";
     val   = convertForDisplay(summSaveBody);

     myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
     td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
     td += label;
     td += ":</td><td>";
     td += val;
     td += "</tr>";
     
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><textarea name=\"body_val\" style='width:100%'>" + val + "</textarea></td>";
     tdedit += "<td><input type=\"hidden\" name=\"body_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"body_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"body_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
     
     $("#" + tablename + " > tbody").html(td);
     
     pwd = parent.$("#emailbody_data_edit");
     pwd.empty().append(tdedit);
    }
   });
  $("#emailbody_data").show();
  $("#emailbody_data_edit").hide();
}

function SaveBodyData()
{
 var pwd = parent.$("#emailbody_data_edit");
 
 if (!pwd.is(":visible"))
  return;
 
 var myform = pwd.find("#emailbodyform");
 var body = myform.find("textarea[name=\"body_val\"]").val();
 var subject = myform.find(":input[name=\"subject_val\"]").val();
 
 $.ajax({
 url : "TemplateDetails?f=seb&id="+summSaveobjid+"&sub="+encodeURI(subject)+"&b="+encodeURI(body),
 dataType : 'json',
 type : 'POST',
 success : function(res) {
  LoadBodyData("emailbody", objtypeAsInt, objtype, objid, "");
 }
 });

}
