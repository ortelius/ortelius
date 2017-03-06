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

function LoadAttributesData()
{
 var save_preaction_val = "";
 var save_postaction_val = "";
 var save_customaction_val = "";

 console.log("GetAttributeData?objtype=" + objtypeAsInt + "&id=" + objid);
 $.ajax(
 {
  url : "GetAttributeData?objtype=" + objtypeAsInt + "&id=" + objid,
  dataType : 'json',
  type : 'GET',
  success : function(res)
  {
   console.log(res);
   readonly = res.readOnly;
   if (readonly) {
	   $("#title_icon").hide();
   } else {
	   $("#title_icon").show();
   }
   saveRes = res;
   var td = "";
   var rowcnt = 0;

   for (var r = 0; r < res.data.length; r++)
   {
    var row = res.data[r];
    var key = row[3];
    var val = row[4];
    var arrid = "";
    
    if (key.indexOf('[') > 0)
     arrid = row[5];
 
    rowcnt++;

    td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">" + key + "</td><td style=\"border: 1px solid #ddd;\">" + val + "</td><td style=\"display:none;\">" + arrid + "</td></tr>";
   }

   for (rowcnt; rowcnt < 5; rowcnt++)
   {
    td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"display:none;\"></td></tr>";
   }

   $("#attrib > tbody").html(td);

   var $table = $("#attrib");
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
 
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
}

function AddAttributeButton()
{
    var pwd = parent.$("#modal");
    var buttons = [
    {
     text : "Ok",
     click : function()
     {
      SaveAttributesData(parent.$("#modal"),false);
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

    var tdedit = "<form id=\"attribform\"><table id=\"attribtab\">";
    tdedit += "<tr>";
    tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Name:</td>";
    tdedit += "<td><input name=\"name_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
    tdedit += "</tr>";
    tdedit += "<tr>";
    tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Value:</td>";
    tdedit += "<td><input name=\"value_val\" style='width:24em' type=\"text\" value=\"\"/></td>";
    tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"\"\></td>";
    tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value=\"\"\></td>";
    tdedit += "</tr></table></form>";
    pwd.load(null);
    pwd.empty();
    pwd.html(tdedit);
    pwd.dialog({open : null});
    pwd.dialog("option", "title", "Attribute");
    pwd.dialog("option", "height", "auto");
    pwd.dialog("option", "width", "auto");
    pwd.dialog("option", "buttons", buttons);
    pwd.dialog('open');
}

function EditAttributeButton()
{
    $("tr.ui-state-highlight").each(function(i, tr) {       
       var key = $(this).find("td").eq(0).html();
       var value = $(this).find("td").eq(1).html();
       var arrid = $(this).find("td").eq(2).html();
       
       if (key == "&nbsp;")
        return;
       
       var pwd = parent.$("#modal");
       var buttons = [
       {
        text : "Ok",
        click : function()
        {
         SaveAttributesData(parent.$("#modal"),true);
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

       var tdedit = "<form id=\"attribform\"><table id=\"attribtab\">";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Name:</td>";
       tdedit += "<td><input name=\"name_val\" style='width:24em' type=\"text\" value='" + key + "'/></td>";
       tdedit += "</tr>";
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">Value:</td>";
       tdedit += "<td><input name=\"value_val\" style='width:24em' type=\"text\" value='" + value + "'/></td>";
       tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value='" + key + "'\></td>";
       tdedit += "<td><input type=\"hidden\" name=\"arr_val\" value='" + arrid + "'\></td>";       
       tdedit += "<td><input type=\"hidden\" name=\"value_oldval\" value='" + value + "'\></td>";
       tdedit += "</tr></table></form>";
       
       pwd.empty();
       pwd.html(tdedit);
       pwd.dialog({open : null});
       pwd.dialog("option", "title", "Attribute");
       pwd.dialog("option", "height", "auto");
       pwd.dialog("option", "width", "auto");
       pwd.dialog("option", "buttons", buttons);
       pwd.dialog('open');
      });
}

function DeleteAttributeButton()
{
    $("tr.ui-state-highlight").each(function(i, tr) {       
       var key = $(this).find("td").eq(0).html();
       var value = $(this).find("td").eq(2).html();
       
       DeleteAttributesData(parent.$("#modal"),key,value);
      });
}

function GetSaveAttributesData(instance, data, isEdit)
{
 var form = instance.find("#attribform");
 var viewArr = form.serializeArray();
 var view =
 {};

 for ( var i in viewArr)
 {
  view[viewArr[i].name] = viewArr[i].value;
 }
 console.log(view);
 
 if (isEdit)
 {
  if (view.name_val != view.name_oldval)
  {
   var prefix = "";
   var arrid  = "";
   
   if (typeof view.arr_val != "undefined")
   {
    prefix = "ele_";
    arrid = "_" + view.arr_val;
   }
   
   console.log(prefix + 'del_' + view.name_oldval + arrid + ' = ' + view.value_val);
   data[prefix + 'del_' + view.name_oldval + arrid] = view.value_val;
   
   if (view.name_val.indexOf('[') >= 0)
   {
    prefix = "ele_";
    arrid  = "_" + view.arr_val;
   }
   else
   {
    prefix = "";
    arrid = "";
   }
   
   console.log(prefix + 'add_' + view.name_val + arrid + ' = ' + view.value_val);
   data[prefix + 'add_' + view.name_val + arrid] = view.value_val;   
  }
  else if (view.value_val != view.value_oldval)
  {
   var prefix = "";
   var arrid  = "";
   
   if (view.name_val.indexOf('[') >= 0)
   {
    prefix = "ele_";
    arrid  = "_" + view.arr_val;
   }
      
   console.log(prefix + 'chg_' + view.name_val + arrid + ' = ' + view.value_val);
   data[prefix + 'chg_' + view.name_val + arrid] = view.value_val;
  }  
 }
 else
 { 
  if (view.name_val != view.name_oldval && view.name_oldval == "")
  {
   var prefix = "";
   var arrid  = "";
   
   if (view.name_val.indexOf('[') >= 0)
   {
    prefix = "ele_";
    arrid  = "_" + view.arr_val;
   }
   
   console.log(prefix + 'add_' + view.name_val + arrid + ' = ' + view.value_val);
   data[prefix + 'add_' + view.name_val + arrid] = view.value_val;
  }
 } 
}

function SaveAttributesData(instance, isEdit)
{
 var savedata =
 {
  objtype : objtypeAsInt,
  id : objid
 };
 GetSaveAttributesData(instance, savedata, isEdit);
 console.log(savedata);

 $.ajax(
 {
  url : "UpdateAttributesData",
  dataType : "json",
  type : "POST",
  data : savedata,
  success : function(data)
  {
   console.log(data);
   if (data && data.saved)
   {
    console.log("Saved data");
    LoadAttributesData();
   }
   else
   {
    if (data && data.error)
    {
     alert("Save failed: " + data.error);
    }
    else
    {
     alert("Save failed!");
    }
   }
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });

}

function DeleteAttributesData(instance,key,value)
{
 var savedata =
 {
  objtype : objtypeAsInt,
  id : objid
 };
 
 var prefix = "";
 var arrid  = "";
 
 if (key.indexOf('[') >= 0)
 {
  prefix = "ele_";
  arrid  = "_" + value;
 }
 
 console.log(prefix + 'del_' + key + arrid + ' = ' + value);
 savedata[prefix + 'del_' + key + arrid] = value;
 
 console.log(savedata);

 $.ajax(
 {
  url : "UpdateAttributesData",
  dataType : "json",
  type : "POST",
  data : savedata,
  success : function(data)
  {
   console.log(data);
   if (data && data.saved)
   {
    console.log("Saved data");
    LoadAttributesData();
   }
   else
   {
    if (data && data.error)
    {
     alert("Save failed: " + data.error);
    }
    else
    {
     alert("Save failed!");
    }
   }
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });

}

function FireSummaryEvent(reason, data)
{
 if (eventcbfn != "")
 {
  try
  {
   var eventCallbackFn = eval(eventcbfn);
   if (typeof eventCallbackFn === "function")
   {
    console.log(eventcbfn + " called");
    return eventCallbackFn(reason, data);
   }
  }
  catch (e)
  {
   console.log('EXCEPTION: ' + e);
  }
 }
 return false;
}
