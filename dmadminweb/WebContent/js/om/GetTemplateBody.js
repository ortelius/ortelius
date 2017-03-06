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
     console.log("success, res="+res);
     summSaveobjid = objid;
     summSaveSubject = res.subject;
     summSaveBody = res.body;
     $("#" + tablename + " > thead").html("<tr><td>Subject: "+convertForDisplay(summSaveSubject)+"</td></tr>");
     $("#" + tablename + " > tbody").html("<tr><td>&nbsp;</td></tr><tr><td>"+convertForDisplay(summSaveBody)+"</td></tr>");
    }
   });
}
