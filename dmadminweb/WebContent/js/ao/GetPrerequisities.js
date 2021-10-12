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

function LoadPrerequisitiesData()
{
 console.log("GetPrerequisitiesForAppData?appid=" + objid);
 $.ajax({
  url: "GetPrerequisitiesForAppData?appid=" + objid,
  dataType : 'json',
  type : 'GET',
  success : function(res)
  {
   console.log(res);
   saveRes = res;
   var td = "";
   var rowcnt = 0;

   for (var r = 0; r < res.data.length; r++)
   {
    var row = res.data[r];
    var key = row[2].name;
    var val = row[3];
    rowcnt++;

    td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">" + key + "</td><td style=\"border: 1px solid #ddd;\">" + val + "</td></tr>";
   }

   for (rowcnt; rowcnt < 5; rowcnt++)
   {
    td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
   }

   $("#prereqs_table > tbody").html(td);

   var $table = $("#prereqs_table");
   $table.delegate("tr", "click", function()
   {
    $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
    $(this).addClass('ui-state-highlight');

    $("#prereqs_btnDelete").button("option", "disabled", false);
   });

   $("#prereqs_btnAddApp").button();
   $("#prereqs_btnAddComp").button();
   $("#prereqs_btnDelete").button(
   {
    disabled : true
   });
   

   $("#prereqs_btnAddComp").click(function(evt)
   {
    var pwd = parent.$("#modal");
    var buttons = [
    {
     text : "Ok",
     click : function()
     {
      SavePrerequisitiesCompData(parent.$("#modal"));
      parent.$("#modal").dialog("close");
     }
    },
    {
     text : "Cancel",
     click : function()
     {
      parent.$("#modal").dialog("close");
     }
    } ];

    var tdedit = "<form id=\"prereqs_comp_form\"><table id=\"prereqs_comp_tab\">";
    tdedit += "<tr>";
    tdedit += "<td style=\"text-align:left;padding-right: 5px;white-space: nowrap;\">Components</td>";
    tdedit += "</tr>";
    tdedit += "<tr>";    
    tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select size=\"10\" name=\"prereqs_comp_val\">";  
    tdedit += "</td>";
    tdedit += "</tr></table></form>";
    
    pwd.empty();
    pwd.html(tdedit);
    pwd.dialog(
      {
       open : function(event, ui)
       {
        $.ajax(
        {
         url : "getComponentsInDomain?cvs=Y",
         dataType : 'json',
         type : 'GET',
         success : function(res)
         {
          res = res.Components;
          var myform = pwd.find("#prereqs_comp_form");
          var comp = myform.find(":input[name=\"prereqs_comp_val\"]");
          console.log(res);
          for (n = 0; n < res.length; n++)
          {
            comp.append("<option id=\"prereqs_comp" + n + "\" value=\"" + res[n].id + "\">" + res[n].name + '</option>');
          }
         },
         error : function(jqxhr, status, err)
         {
          console.log(status);
          console.log(err);
         }
        });

       }
      });
    pwd.dialog("option", "title", "Prerequisities");
    pwd.dialog("option", "height", "auto");
    pwd.dialog("option", "width", "auto");
    pwd.dialog("option", "buttons", buttons);
    $("#prereqs_btnAdd").show();
    $("#prereqs_btnEdit").show();    
    $("#prereqs_btnDelete").show();
    pwd.dialog('open');
   });
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
}

function GetSavePrerequisitiesCompData(instance, data)
{
 var form = instance.find("#prereqs_comp_form");
 var viewArr = form.serializeArray();
 var view =
 {};

 for ( var i in viewArr)
 {
  view[viewArr[i].name] = viewArr[i].value;
 }
 console.log(view);
 return view;
}


function SavePrerequisitiesCompData(instance)
{
 var savedata = {};
 
 savedata = GetSavePrerequisitiesCompData(instance, savedata);
 console.log(savedata.prereqs_comp_val);
 console.log(objtype);
 console.log(objid);
 
 $.ajax({
  url: "GetPrerequisitiesForAppData",
  data: {
   reason:  'add',
   objtype: 'c',
   appid:   objid,
   id:   savedata.prereqs_comp_val
  },
     dataType: 'json',
     type: 'POST',
     success: function (res) {
      if(res.result === false) {
       alert("Failed to add object to prerequisite " + (res.error ? res.error : ""));
       return;
      }
      LoadPrerequisitiesData();
     },
  error: function(jqxhr, status, err) {
   console.log(status);
   console.log(err);
  }
 });
}


