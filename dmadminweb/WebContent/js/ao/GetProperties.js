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
function LoadPropertiesData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 propdefs = [];

 var deftype = "";
 if (objid < 0)
 {
  if (objtype == "no")
   deftype = "&deftype=" + notifiertype + "&defonly=Y";
  else if (objtype == "re")
   deftype = "&deftype=" + repotype + "&defonly=Y";
  else if (objtype == "be")
   deftype = "&deftype=" + buildenginetype + "&defonly=Y";
  else if (objtype == "co")
   deftype = "&deftype=" + addcomptype + "&defonly=Y";
  else
   deftype = "&deftype=" + dstype + "&defonly=Y";
 }
 
 console.log("GetPropertiesData?objtype=" + objtypeAsInt + "&id=" + objid + deftype);

 $.ajax(
   {
    url : "GetPropertiesData?objtype=" + objtypeAsInt + "&id=" + objid + deftype,
    dataType : 'json',
    type : 'GET',
    success : function(res)
    {
     console.log(res);
     saveRes = res;
     var title = "";
     var tdedit = "<form id=\"propsform\"><table id=\"propstab\" class=\"dev_table\"><tbody>";
     var td = "";
     
     propslist = [];
     
     for (var r = 0; r < res.defs.length; r++)
     {
      var row = res.defs[r];
      var data_required = "";
      var data_name = "";
      var data_val = "";
      var data_append = "";
      var data_over = "";
      var data_encrypt = "";
      var def_name = row.name;
      var def_required = row.reqd;
      var def_append = row.append;

      propslist.push(row.name);

      found = false;
      for (j=0; j < res.data.length; j++)
      {
       data = res.data[j];
       if (data[3] == def_name)
       {
        data_required = data[1];
        data_name = data[3];
        data_val = data[4];
        data_enc = data[5];
        data_over = data[6];
        data_append = data[7];
        found = true;
        break;
       }
      }
      
      if (!found)
      {
       data_name = def_name;
       data_required = def_required;
       data_append   = def_append;
      }
      
      if (data_required)
       data_required = "Yes";
      else
       data_required = "No";
      
      if (data_encrypt)
       data_encrypt = "Yes";
      else
       data_encrypt = "No";
      
      if (data_over)
       data_over = "Yes";
      else
       data_over = "No";
           
      if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL") 
      {
       if (data_val == "1")
        data_val = "Yes";
       else
        data_val = "No";
      }
      
      var field = data_name.toLowerCase().replace(/ /g, "");
      var callback = "";
      var rtype = "";
      var label = data_name.charAt(0).toUpperCase() + data_name.slice(1);
      var cleanid = label.toLocaleLowerCase().replace(/ /g, "");
      
      if (label.toLowerCase() == "uri")
       label = "URL";
      else if (label.toLowerCase() == "buildno")
       label = "Build Number"; 
 //     else if (label.toLowerCase() == "product")
 //      label = "Organization"; 
      
      if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL")
      { 
       myid = cleanid + "_sumrow";
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += data_val;
       td += "</tr>";
      }
      else
      {
       myid = cleanid + "_sumrow";
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label;
       td += ":</td><td>";
       td += data_val;
       td += "</tr>";      
      
        myid = cleanid + "_encrypt_sumrow";
        td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
        td += label + " Encrypted";
        td += ":</td><td>";
        td += data_encrypt;
        td += "</tr>";
       }
       
       myid = cleanid + "_override_sumrow";
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
       td += label + " Override";
       td += ":</td><td>";
       td += data_over;
       td += "</tr>";
       
       if (def_append == "true")
       {
        myid = cleanid + "_append_sumrow";
        td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
        td += label + " Appendable";
        td += ":</td><td>";
        td += data_append;
        td += "</tr>";
       }
       
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
       if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL")
       {
        if (data_val == "Yes")
         tdedit += "<td><input name=\"" + field + "_val\" type=\"checkbox\" checked /></td>";
        else
         tdedit += "<td><input name=\"" + field + "_val\" type=\"checkbox\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"" + field + "_oldval\" value=\"" + data_val + "\"/></td>";
       }
       else
       {
        tdedit += "<td><input name=\"" + field + "_val\" style='width:100%' type=\"text\" value=\"" + data_val + "\"/></td>";
        tdedit += "<td><input type=\"hidden\" name=\"" + field + "_oldval\" value=\"" + data_val + "\"/></td>";
        tdedit += "</tr>";
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + " Encrypted:</td>";
        if (data_encrypt == "Yes")
          tdedit += "<td><input name=\"" + field + "_encrypt_val\" type=\"checkbox\" checked /></td>";
        else
          tdedit += "<td><input name=\"" + field + "_encrypt_val\" type=\"checkbox\"/></td>";
        tdedit += "</tr>";
       } 
       tdedit += "<tr>";
       tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + " Override:</td>";
       if (data_over == "Yes")
         tdedit += "<td><input name=\"" + field + "_over_val\" type=\"checkbox\" checked/></td>";
       else
         tdedit += "<td><input name=\"" + field + "_over_val\" type=\"checkbox\"/></td>";
       tdedit += "</tr>";
       
       if (def_append == "true")
       {
        tdedit += "<tr>";
        tdedit += "<td style=\"text-align:left; white-space: nowrap;\">" + label + " Appendable:</td>";
        if (data_append == "Yes")
         tdedit += "<td><input name=\"" + field + "_over_val\" type=\"checkbox\" checked/></td>";
        else
          tdedit += "<td><input name=\"" + field + "_over_val\" type=\"checkbox\"/></td>";
        tdedit += "</tr>";
       } 
     }
     
     tdedit += "</tbody></table></form>";
     
     var pwd = parent.$("#props > tbody");
     pwd.empty().append(td);
     
     pwd = parent.$("#props_data_edit");
     pwd.empty().append(tdedit);
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

function SavePropertiesData()
{
 var pwd = parent.$("#props_data_edit");
 
 if (!pwd.is(":visible"))
  return;
 
 var form = parent.$("#propsform");

 var data =
 {
  objtype : summSaveobjtypeAsInt,
  id : summSaveobjid
 };
 
 for (i=0;i<propslist.length;i++)
 {
  var data_required = "";
  var data_name = propslist[i];
  var data_val = "";
  var data_enc = "";
  var data_over = "";
  var data_encrypt = "";
  var data_append = "";
  var field = data_name.toLowerCase().replace(/ /g, "");
  var label = data_name.charAt(0).toUpperCase() + data_name.slice(1);
  var cleanid = label.toLocaleLowerCase().replace(/ /g, "");

  data_val = ":input[name=\"" + field + "_val\"]";
  data_oldval = ":input[name=\"" + field + "_oldval\"]";
  data_encrypt_val = ":input[name=\"" + field + "_encrypt_val\"]";
  data_over_val = ":input[name=\"" + field + "_over_val\"]";
  data_append_val = ":input[name=\"" + field + "_append_val\"]";

  if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL") 
  {
   if (form.find(data_val).length > 0)
    data_val = form.find(data_val).prop('checked');
   else
    data_val = false;  
  }
  else
  {
   if (form.find(data_val).length > 0)
    data_val = form.find(data_val).val(); 
   else
    data_val = "";
  }
  
  if (form.find(data_oldval).length > 0)
   data_oldval = form.find(data_oldval).val(); 
  else
   data_oldval = "";
   
  if (form.find(data_encrypt_val).length > 0)
   data_encrypt_val = form.find(data_encrypt_val).prop('checked');
  else
   data_encrypt_val = false;
  
  data_encrypt_val = (data_encrypt_val) ? "Y" : "N";
  
  if (form.find(data_over_val).length > 0)
   data_over_val = form.find(data_over_val).prop('checked');
  else
   data_over_val = false;
  
  data_over_val = (data_over_val) ? "Y" : "N";
  
  if (form.find(data_append_val).length > 0)
   data_append_val = form.find(data_append_val).prop('checked'); 
  else
   data_append_val = false;
  
  data_append_val = (data_append_val) ? "Y" : "N";
 
  if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL") 
  {
	   data_val = (data_val) ? "1" : "0";
	   data_oldval = (data_oldval == "Yes") ? "1" : "0";
  }  
 
  if (data_name == "data source name")
    data_name = 'dsn';
 
  var outpos = "0";

  var val = data_encrypt_val + data_over_val + data_append_val + data_val;
  var key = "";

  if (data_name == "recursive" || data_name == "SSL" || data_name == "HTML" || data_name == "UseSSL") 
  {
    if (data_oldval == "0" && data_val == "1")
     key = "add_" + data_name;
    else if (data_oldval == "1" && data_val == "1")
     key = "chg_" + data_name;
    else
     key = "delete_" + data_name;
  }
  else
  {
   if (data_oldval == "")
    key = "add_" + data_name;
   else if (data_val == "")
    key = "delete_" + data_name;
   else 
    key = "chg_" + data_name;
  }
  
  data[key] = val;
 } 
 console.log(data);
 
 $.ajax(
 {
   url : "UpdatePropertiesData",
   type : 'POST',
   dataType : "json",
   data : data,
   success : function(data)
   {
    $("#props_data").show();
    $("#props_data_edit").hide(); 
    LoadPropertiesData("propstab", objtypeAsInt, objtype, objid, ""); 
   }
 });
}
