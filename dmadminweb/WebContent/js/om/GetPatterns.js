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
function LoadPatternsData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 propdefs = [];

 console.log("GetRepoTextPatternsData?id=" + objid);

 $.ajax(
   {
    url : "GetRepoTextPatternsData?id=" + objid,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     console.log(res);
     var td = "";
     var rowcnt = 0;

     for (a = 0; a < res.data.length; a++)
     {
      var row = res.data[a];
      var name = row[3];
      var val = row[4];
      var textstr = row[5];

       td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\"  id=\"name_" + rowcnt + "\">" + name + "</td><td style=\"border: 1px solid #ddd;\" id=\"val_" + rowcnt + "\">"
         + val + "</td><td style=\"border: 1px solid #ddd;\" id=\"text_" + rowcnt + "\">" + textstr + "</td></tr>";

      rowcnt++;
     }

     for (rowcnt; rowcnt < 5; rowcnt++)
     {
      td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
     }

     $("#" + tablename + " > tbody").html(td);

     var $table = $("#" + tablename);
     $table.delegate("tr", "click", function()
     {
      $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
      $(this).addClass('ui-state-highlight');

      $("edit_button").button("option", "disabled", false);
      $("delete_button").button("option", "disabled", false);
     });

     $("add_button").button();
     $("delete_button").button(
     {
      disabled : true
     });

     $("edit_button").button(
     {
      disabled : true
     });
    }
   });
}

function AddPatternsButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SavePatternsData(parent.$("#modal"));
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

 var tdedit = "<form id=\"Patternsform\"><table id=\"Patternstab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\">Path:</td></tr>";
 tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" id=\"name_val\" name=\"name_val\" >";

 tdedit += "</td></tr>";
 tdedit += "<tr><td><input type=\"hidden\" name=\"name_oldval\" value=\"\"\></td></tr>";

 tdedit += "<tr><td style=\"text-align:left;\">Pattern:</td></tr>";
 tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"value_val\"></td></tr>";
 tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr><td>";
 tdedit += "<input type=\"checkbox\" id=\"text_val\" name=\"text_val\">";
 tdedit += "Text?";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"text_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
 {
  open : function(event, ui)
  {
  }
 });

 pwd.dialog("option", "title", "Add Pattern");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "300");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}

function EditPatternsButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var id = $(this).find("td").eq(0).attr("id");
  id = id.substring(5);

  var name = $("#name_" + id).html();
  var val = $("#val_" + id).html();
  var textstr = $("#text_" + id).html();

  var pwd = parent.$("#modal");
  var buttons = [
  {
   text : "Ok",
   click : function()
   {
    SavePatternsData(parent.$("#modal"));
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

  var tdedit = "<form id=\"Patternsform\"><table id=\"Patternstab\" class=\"summtable\">";
  tdedit += "<tr><td style=\"text-align:left;\">Name:</td></tr>";
  tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" id=\"name_val\" name=\"name_val\" value=\"" + name + "\"\>";

  tdedit += "</td></tr>";
  tdedit += "<tr><td><input type=\"hidden\" name=\"name_oldval\" value=\"" + name + "\"\></td></tr>";

  tdedit += "<tr><td style=\"text-align:left;\">Value:</td></tr>";
  tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"value_val\" value=\"" + val + "\"></td></tr>";
  tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value=\"" + val + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "<tr><td>";
  if (textstr == "true")
   tdedit += "<input type=\"checkbox\" id=\"text_val\" name=\"text_val\" checked>";
  else
   tdedit += "<input type=\"checkbox\" id=\"text_val\" name=\"text_val\">";

  tdedit += "Text?";
  tdedit += "</td>";
  tdedit += "<td><input type=\"hidden\" name=\"text_oldval\" value=\"" + textstr + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "<tr><td>";
  tdedit += "</table></form>";

  pwd.empty();
  pwd.html(tdedit);

  pwd.dialog(
  {
   open : function(event, ui)
   {
    ToggleAppend();
   }
  });

  pwd.dialog("option", "title", "Edit Pattern");
  pwd.dialog("option", "height", "auto");
  pwd.dialog("option", "width", "300");
  pwd.dialog("option", "buttons", buttons);
  pwd.dialog('open');

 });

}

function DeletePatternsButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var id = $(this).find("td").eq(0).attr("id");
  id = id.substring(5);
  var name = $("#name_" + id).html();
  var value = $("#val_" + id).html();
  var textstr = $("#text_" + id).html();

  var val = name + "&" + value + "&" + textstr;
  var key = "";
  var data =
  {
   objtype : summSaveobjtypeAsInt,
   id : summSaveobjid
  };

   key = "del_" + name;

  data[key] = val;
  console.log(data);

  $.ajax(
  {
   url : "GetRepoTextPatternsData?reason=save&id=" + objid,
   type : 'POST',
   dataType : "json",
   data : data,
   success : function(data)
   {
    LoadPatternsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   }
  });
 });
}

function SavePatternsData(instance)
{
 var form = instance.find("#Patternsform");
 var viewArr = form.serializeArray();
 var view =
 {};

 console.log(viewArr);

 for ( var i in viewArr)
 {
  if (viewArr[i].value == "on")
   viewArr[i].value = "Y";

  if (viewArr[i].value == "off")
   viewArr[i].value = "N";

  view[viewArr[i].name] = viewArr[i].value;
 }

 if (view['text_val'] != "Y" && view['text_val'] != "N")
  view['text_val'] = 'N';

 var outpos = "0";

 var val = view['name_val'] + "&" + view['value_val'] + "&" + view['text_val'];
 var key = "";
 var data =
 {
  objtype : summSaveobjtypeAsInt,
  id : summSaveobjid
 };

 if (view["name_oldval"] == "")
  key = "add_" + view["name_val"];
 else
  key = "chg_" + view["name_oldval"];

 data[key] = val;
 console.log(data);

 $.ajax(
 {
  url : "GetRepoTextPatternsData?reason=save&id=" + objid,
  type : 'POST',
  dataType : "json",
  data : data,
  success : function(data)
  {
   LoadPatternsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
  }
 });
}
