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

