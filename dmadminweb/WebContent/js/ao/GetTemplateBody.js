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
