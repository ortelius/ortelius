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
function LoadPropertiesData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 propdefs = [];

 console.log("GetPropertiesData?objtype=" + objtypeAsInt + "&id=" + objid);

 $.ajax(
   {
    url : "GetPropertiesData?objtype=" + objtypeAsInt + "&id=" + objid,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     console.log(res);
     if (res.readOnly)
         $("#title_icon").hide();
      else
         $("#title_icon").show(); 
     var td = "";
     var rowcnt = 0;

     for (a = 0; a < res.defs.length; a++)
     {
      var row = res.defs[a];
      if (row.name == "dsn")
       row.name = "data source name";
      
      console.log("row.name="+row.name+" row.append="+row.append);
      
      propdefs[row.name] =
      {
       "append" : row.append,
       "reqd" : row.reqd,
       "used" : false
      };
     }
     
     console.log(propdefs);

     for (a = 0; a < res.data.length; a++)
     {
      var row = res.data[a];
      var name = row[3];
      var val = row[4];
      var enc = row[5];
      var override = row[6];
      var append = row[7];
      
      if (name == "dsn")
       name = "data source name";

      if (typeof propdefs[name] == "undefined") {
    	  propdefs[name] = {};
      }
      propdefs[name].used = true;

      if (enc == "true")
       td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\"  id=\"name_" + rowcnt + "\">" + name + "</td><td style=\"border: 1px solid #ddd;\" id=\"val_" + rowcnt + "\">"
         + "*******" + "</td><td style=\"border: 1px solid #ddd;\" id=\"enc_" + rowcnt + "\">" + enc + "</td><td style=\"border: 1px solid #ddd;\" id=\"override_" + rowcnt + "\">" + override
         + "</td><td style=\"border: 1px solid #ddd;\" id=\"append_" + rowcnt + "\">" + append + "</td></tr>";
      else
       td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\"  id=\"name_" + rowcnt + "\">" + name + "</td><td style=\"border: 1px solid #ddd;\" id=\"val_" + rowcnt + "\">"
         + ((name=="recursive" || name=="SSL" || name=="HTML" || name=="UseSSL")?(val==1)?"true":"false":val) + "</td><td style=\"border: 1px solid #ddd;\" id=\"enc_" + rowcnt + "\">" + enc + "</td><td style=\"border: 1px solid #ddd;\" id=\"override_" + rowcnt + "\">" + override
         + "</td><td style=\"border: 1px solid #ddd;\" id=\"append_" + rowcnt + "\">" + append + "</td></tr>";

      rowcnt++;
     }

     for (rowcnt; rowcnt < 5; rowcnt++)
     {
      td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
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

function ShowCheckbox(v)
{
	if (v) {
		$("#cbattr").show();
		$("#textattr").hide();
		$("#encattr").hide();
	} else {
		$("#cbattr").hide();
		$("#textattr").show();
		$("#encattr").show();
	}
}

function AddPropertiesButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SavePropertiesData(parent.$("#modal"));
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

 var tdedit = "<form id=\"Propertiesform\"><table id=\"Propertiestab\" class=\"summtable\" onChange=\"ToggleAppend()\">";
 tdedit += "<tr><td style=\"text-align:left;\">Name:</td>";
 tdedit += "<td><select style=\"width:100%;\" id=\"name_val\" name=\"name_val\" >";

 console.log(propdefs);
 var cnt=0;
 for (var pdef in propdefs)
 {
  console.log("pdef="+pdef+" propdefs[pdef]="+propdefs[pdef]);
  if (!propdefs[pdef].used)
  {
   ++cnt;
   tdedit += "<option>" + pdef + "</option>";
  }
 }

 tdedit += "</td></tr>";
 tdedit += "<tr><td><input type=\"hidden\" name=\"name_oldval\" value=\"\"\></td></tr>";
 tdedit += "<tr id=\"textattr\"><td style=\"text-align:left;\">Value:</td>";
 tdedit += "<td><input type=\"text\" style=\"width:100%;\" name=\"value_val_text\"></td></tr>";
 tdedit += "<tr id=\"cbattr\"><td style=\"text-align:left;\">Value:</td>";
 tdedit += "<td><input type=\"checkbox\" name=\"value_val_cb\" value=\"true\"></td><tr>";
 tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"encattr\"><td>";
 tdedit += "<input type=\"checkbox\" id=\"enc_val\" name=\"enc_val\">";
 tdedit += "Encrypted";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"enc_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr><td>";
 tdedit += "<input type=\"checkbox\" id=\"override_val\" name=\"override_val\">";
 tdedit += "Overridable";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"override_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr id=\"append_row\"><td>";
 tdedit += "<input type=\"checkbox\" id=\"append_val\" name=\"append_val\">";
 tdedit += "Appendable";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"append_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "</table></form>";

 if (cnt == 0)
  tdedit = "<p>All properties have been completed</p>";
 
 pwd.empty();
 pwd.html(tdedit);
 

 pwd.dialog(
 {
  open : function(event, ui)
  {
   $("#name_val").change(function() {
	  nv = $("#name_val").val();
	  ShowCheckbox(nv == "recursive" || nv == "SSL" || nv == "HTML" || nv == "UseSSL");
   });
   nv = $("#name_val").val();
   console.log("nv="+nv);
   ShowCheckbox(nv == "recursive" || nv == "SSL" || nv == "HTML" || nv == "UseSSL");
   ToggleAppend();
  }
 });

 pwd.dialog("option", "title", "Add Property");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "300");
 if (cnt == 0)
 {
  buttons = [ { text: "Ok", click: function() { $( this ).dialog( "close" ); } } ];
 }
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');

}

function ToggleAppend()
{
 var key = $("#name_val").val();

 if (typeof propdefs[key] == "undefined")
  return;
 
 console.log("key="+key);
 console.log(propdefs);

 if (propdefs[key].append)
  $("#append_row").show();
 else
  $("#append_row").hide();
}

function EditPropertiesButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var id = $(this).find("td").eq(0).attr("id");
  id = id.substring(5);

  var name = $("#name_" + id).html();
  var val = $("#val_" + id).html();
  var enc = $("#enc_" + id).html();
  var override = $("#override_" + id).html();
  var append = $("#append_" + id).html();

  if (name == "dsn")
   name = "data source name";
  
  var pwd = parent.$("#modal");
  var buttons = [
  {
   text : "Ok",
   click : function()
   {
    SavePropertiesData(parent.$("#modal"));
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

  var tdedit = "<form id=\"Propertiesform\"><table id=\"Propertiestab\" class=\"summtable\" onChange=\"ToggleAppend()\">";
  tdedit += "<tr><td style=\"text-align:left;\">Name:</td>";
  tdedit += "<td><select style=\"width:100%;\" disabled>";

  for ( var pdef in propdefs)
  {
   if (!propdefs[pdef].used)
   {
    tdedit += "<option>" + pdef + "</option>";
   }
   else if (propdefs[pdef].used && pdef == name)
   {
    tdedit += "<option selected>" + pdef + "</option>";
   }
  }

  var checked=(val=="true")?"checked":"";

  tdedit += "</td></tr>";
  tdedit += "<tr><td><input type=\"hidden\" id=\"name_val\" name=\"name_val\" value=\"" + name + "\"\></td></tr>";
  tdedit += "<tr><td><input type=\"hidden\" id=\"name_oldval\" name=\"name_oldval\" value=\"" + name + "\"\></td></tr>";

  tdedit += "<tr id=\"textattr\"><td style=\"text-align:left;\">Value:</td>";
  tdedit += "<td><input type=\"text\" style=\"width:100%;\" name=\"value_val_text\" value=\"" + val + "\"></td></tr>";
  tdedit += "<tr id=\"cbattr\"><td style=\"text-align:left;\">Value:</td>";
  tdedit += "<td><input type=\"checkbox\" style=\"width:100%;\" name=\"value_val_cb\" value=\"true\" "+checked+"></td></tr>";
  tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value=\"" + val + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "<tr id=\"encattr\"><td>";
  if (enc == "true")
   tdedit += "<input type=\"checkbox\" id=\"enc_val\" name=\"enc_val\" checked>";
  else
   tdedit += "<input type=\"checkbox\" id=\"enc_val\" name=\"enc_val\">";

  tdedit += "Encrypted";
  tdedit += "</td>";
  tdedit += "<td><input type=\"hidden\" name=\"enc_oldval\" value=\"" + enc + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "<tr><td>";
  if (override == "true")
   tdedit += "<input type=\"checkbox\" id=\"override_val\" name=\"override_val\" checked>";
  else
   tdedit += "<input type=\"checkbox\" id=\"override_val\" name=\"override_val\">";
  tdedit += "Overridable";
  tdedit += "</td>";
  tdedit += "<td><input type=\"hidden\" name=\"override_oldval\" value=\"" + override + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "<tr id=\"append_row\"><td>";

  if (append == "true")
   tdedit += "<input type=\"checkbox\" id=\"append_val\" name=\"append_val\" checked>";
  else
   tdedit += "<input type=\"checkbox\" id=\"append_val\" name=\"append_val\">";
  tdedit += "Appendable";
  tdedit += "</td>";
  tdedit += "<td><input type=\"hidden\" name=\"append_oldval\" value=\"" + append + "\"\></td>";
  tdedit += "</tr>";

  tdedit += "</table></form>";

  pwd.empty();
  pwd.html(tdedit);

  pwd.dialog(
  {
   open : function(event, ui)
   {
	$("#name_val").change(function() {
		nv = $("#name_val").val();
		ShowCheckbox(nv == "recursive" || nv == "SSL" || nv == "HTML" || nv == "UseSSL");
	});
	nv = $("#name_val").val();
	ShowCheckbox(nv == "recursive" || nv == "SSL" || nv == "HTML" || nv == "UseSSL");
    ToggleAppend();
   }
  });

  pwd.dialog("option", "title", "Edit Property");
  pwd.dialog("option", "height", "auto");
  pwd.dialog("option", "width", "300");
  pwd.dialog("option", "buttons", buttons);
  pwd.dialog('open');

 });

}

function DeletePropertiesButton()
{
 $("tr.ui-state-highlight").each(function(i, tr)
 {
  var id = $(this).find("td").eq(0).attr("id");
  id = id.substring(5);
  var key = $("#name_" + id).html();
  var value = $("#val_" + id).html();

  if (key == "data source name")
   key = "dsn";
  
  var savedata =
  {
   objtype : objtypeAsInt,
   id : objid
  };

  console.log('delete_' + key + ' = ' + value);
  savedata['delete_' + key] = value;

  console.log(savedata);

  $.ajax(
  {
   url : "UpdatePropertiesData",
   type : 'POST',
   dataType : "json",
   data : savedata,
   success : function(data)
   {
    LoadPropertiesData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   }
  });
 });
}

function SavePropertiesData(instance)
{
 var form = instance.find("#Propertiesform");
 var viewArr = form.serializeArray();
 var view = {};

 console.log("VIEWARR");
 console.log(viewArr);
 console.log("END VIEWARR");

 
 for ( var i in viewArr)
 {
  if (viewArr[i].value == "on")
   viewArr[i].value = "Y";

  if (viewArr[i].value == "off")
   viewArr[i].value = "N";
  
  console.log("Setting view["+viewArr[i].name+"]="+viewArr[i].value);
  view[viewArr[i].name] = viewArr[i].value;
 }
 
 if (view['name_val'] == "recursive" || view['name_val'] == "SSL" || view['name_val'] == "HTML" || view['name_val'] == "UseSSL") {
	 view['value_val']=(typeof view['value_val_cb'] == "undefined")?"0":"1";
 } else {
	 view['value_val']=view['value_val_text'];
 }
 
 if (view['name_val'] == "data source name")
  view['name_val'] = 'dsn';

 if (view['append_val'] != "Y" && view['append_val'] != "N")
  view['append_val'] = 'N';

 if (view['enc_val'] != "Y" && view['enc_val'] != "N")
  view['enc_val'] = 'N';

 if (view['override_val'] != "Y" && view['override_val'] != "N")
  view['override_val'] = 'N';

 var outpos = "0";

 var val = view['enc_val'] + view['override_val'] + view['append_val'] + view['value_val'];
 var key = "";
 var data =
 {
  objtype : summSaveobjtypeAsInt,
  id : summSaveobjid
 };

 if (view["name_oldval"] == "")
  key = "add_" + view["name_val"];
 else
  key = "chg_" + view["name_val"];

 data[key] = val;
 console.log(data);

 $.ajax(
 {
  url : "UpdatePropertiesData",
  type : 'POST',
  dataType : "json",
  data : data,
  success : function(data)
  {
   LoadPropertiesData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
  }
 });
}
