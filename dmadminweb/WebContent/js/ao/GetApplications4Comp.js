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

function LoadApp4CompData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 var url = "";
 var servlet = "";
 
 servlet = "GetComponentLayout";
 url = "compid=" + objid + "&t=a";
 
 $.getJSON(servlet, url, function(res)
   {
  var td = "";
  var rowcnt = 0;

   data = res.data;
   
   for (a = 0; a < data.length; a++)
   {
    var row = data[a];
    
    console.log("row="+row);
    
    if (typeof row[0].type == "undefined")
     continue;
    
    var val = row[3].name;
    if (typeof row[3].when != "undefined") 
    {
    	console.log("Passing when "+row[3].when);
     var d = convertDate(row[3].when);
     val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
     val += " by " + row[3].name;
    } else {
    	val = "<a onClick=\"DisplayDeploy(" + row[3].id + ");\">"+val+"</a> "+row[3].tooltip;
    }
    
    linkval2="<a href='javascript:SwitchDisplay(\""+row[0].type+row[0].id+"\");'>"+row[0].name+"</a>";
    linkval1="<a href='javascript:SwitchDisplay(\""+row[1].type+row[1].id+"\");'>"+row[1].name+"</a>";
    
    if (objid == row[1].id)
    {
     td += "<tr><td id=\"" + row[1].type + row[1].id  +"\">" + linkval2 + "</td></tr>";
     rowcnt++;
    } 
   }

  $("#" + tablename + " > tbody").html(td);

   });
}

