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
var save_action_id;
var okclicked=false;
var maxinpos=0;

function LoadArgsData(tablename, objtypeAsInt, objtype, objid, objkind, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 summSaveobjkind = objkind;
 maxinpos=0;

 console.log("objkind="+objkind+" Calling GetActionArgsData?id="+objid);
 $.getJSON('GetActionArgsData', "id=" + summSaveobjid, function(res)
   {
  var td = "";
  var inputtd = "";
  var args = "<ul id=\"argspalette\" class=\"actionarglist argConnection\">";
  var rowcnt = 0;
  var inputrowcnt = 0;
  var filepath = res.filepath;
  var interpreter = res.interpreter;
  
  var outputArgs = { };
  if (res.readonly) {
	  $("#rightbuttonsarg").hide();
	  $("#rightbuttonsswitch").hide();
  } else {
	  $("#rightbuttonsarg").show();
	  $("#rightbuttonsswitch").show();
  }
  
  for (a = 0; a < res.data.length; a++)
  {
   var row = res.data[a];
   console.log(row);
   var swmode = "Switch";
   var key = row[0];
   var name = row[4];
   var sw1 = row[11];
   var sw2 = row[12];
   var padded = row[7];
   var inpos = row[8];
   var outpos = row[9];
   var required = row[6];
   var typestr = row[5];
   
   if (inpos == "")  inpos = 0;
   if (outpos == "") outpos = 0;
   
   if (row[10] == "S")      swmode = "Switch";
   else if (row[10] == 'P') swmode = "Prefix";
   else if (row[10] == "A") swmode = "Always";
   else if (row[10] == "B") swmode = "Boolean";
   
   console.log("typestr="+typestr+" objkind="+objkind);
   
   if (inpos > maxinpos) maxinpos = inpos;
   
   if (typestr != "false")
   {
	   if (objkind ==1 || objkind == 2) {
		   inputtd += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\"  id=\"name_" + key +"\">" + name + "</td><td style=\"border: 1px solid #ddd;\" id=\"type_" + name +"\">" + typestr + "</td><td style=\"border: 1px solid #ddd;\" id=\"req_" + name + "\">" + required + "</td><td style=\"display:none\" id=\"pos_" + name + "\">" + outpos + "</td><td style=\"display:none\" id=\"inpos_" + name + "\">" + inpos + "</td></tr>";
	   } else {
		   inputtd += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\"  id=\"name_" + key +"\">" + name + "</td><td style=\"border: 1px solid #ddd;\" id=\"type_" + name +"\">" + typestr + "</td><td style=\"border: 1px solid #ddd;display:none;\" id=\"swmode_" + name + "\">" + swmode + "</td><td style=\"border: 1px solid #ddd;\" id=\"sw1_" + name + "\">" + sw1 + "</td><td style=\"border: 1px solid #ddd;\" id=\"sw2_" + name + "\">" + sw2 + "</td><td style=\"border: 1px solid #ddd;\" id=\"pad_" + name + "\">" + padded + "</td><td style=\"border: 1px solid #ddd;\" id=\"req_" + name + "\">" + required + "</td><td style=\"display:none\" id=\"pos_" + name + "\">" + outpos + "</td><td style=\"display:none\" id=\"inpos_" + name + "\">" + inpos + "</td></tr>";
		   args += getArgsTextWithMarkup(name,swmode,sw1,sw2,required,padded,a+1,outpos,outputArgs);
	   }
	
    inputrowcnt++;
   }
   else
   { 
    td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\" id=\"sw1_" + name + "\">" + sw1 + "</td></tr>";
    args += getArgsTextWithMarkup(name,swmode,sw1,sw2,required,padded,a+1,outpos,outputArgs);
    rowcnt++;
   }
  }

  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\">&nbsp;</td></tr>";
  }

  for (inputrowcnt; inputrowcnt < 5; inputrowcnt++)
  {
   if (objkind == 1 || objkind == 2) {
	   inputtd += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;align:center;\">&nbsp;</td></tr>";
   } else {
	   inputtd += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;width:30%\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;align:center;\">&nbsp;</td></tr>";
   }
}

  if (objkind != 1 && objkind != 2) {
	  args += "</ul><br><h2>Command Line</h2><ul id=\"argsoutput\" class=\"actionarglist argConnection\">";
   args += "<li class=\"actionarg_required ui-state-disabled\"><span class=\"argName\">" + interpreter + "</span></li>";	  
	  args += "<li class=\"actionarg_required ui-state-disabled\"><span class=\"argName\">" + filepath + "</span></li>";
  }
  var keys = Object.keys(outputArgs).sort();
  
  for (var x=0;x < keys.length;x++)
  {
   if (typeof outputArgs[keys[x]] != "undefined")
    args += outputArgs[keys[x]]; 
  }
  
  args += "</ul>";
  
  $("#" + tablename + " > tbody").html(td);
  $("#inputparamstable > tbody").html(inputtd);
  
  $("#argsoutputpane").html(args);
   
  var $table = $("#argstable");
  $table.delegate("tr", "click", function()
    {
   $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
   $(this).addClass('ui-state-highlight');

   $("edit_button").button("option", "disabled", false);
   $("delete_button").button("option", "disabled", false);
    });

  var $inputtable = $("#inputparamstable");
  $inputtable.delegate("tr", "click", function()
    {
   $inputtable.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
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
  
  	if (!res.readonly) {
  		InitArgsDnD();
  	}
   });
 
}

function AddArgsButton(isswitch)
{
 var pwd = parent.$("#modal");
 var inpos = Number(maxinpos)+1;
 
 console.log("summSaveobjkind="+summSaveobjkind+" maxinpos="+maxinpos);
 
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  if (SaveArgsData(parent.$("#modal"))) parent.$("#modal").dialog("close");
                 }
                },
                {
                 text : "Cancel",
                 click : function()
                 {
                  parent.$("#modal").dialog("close");
                 }
                } ];

 var tdedit = "<form id=\"Argsform\"><table id=\"Argstab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\">Name:</td></tr>";
 
 tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"name_val\"></td></tr>";
 tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"\"\></td>";
 
 tdedit += "<tr><td>Argument Type:</td></tr>";
 tdedit += "<tr><td><select style=\"width:100%;\" name=\"type_val\" >";
 tdedit += "<option value=\"entry\" selected>Entry</option>";
 tdedit += "<option value=\"checkbox\">Checkbox</option>";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"type_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";
 
 if (summSaveobjkind != 1 && summSaveobjkind != 2) {
 if (isswitch == "switch")
 { 
  tdedit += "<tr style=\"display:none\"><td>Switch Mode:</td></tr>";
  tdedit += "<tr style=\"display:none\"><td><select style=\"width:100%;\" name=\"switchmode_val\" >";
  tdedit += "<option value=\"switch\" selected>Switch</option>";
  tdedit += "<option value=\"prefix\">Prefix</option>";
  tdedit += "<option value=\"always\">Always</option>";
  tdedit += "<option value=\"boolean\">Boolean</option>";
 }
 else
 {
  tdedit += "<tr><td>Switch Mode:</td></tr>";
  tdedit += "<tr><td><select style=\"width:100%;\" name=\"switchmode_val\" >";  
  tdedit += "<option value=\"prefix\" selected>Prefix</option>";
  tdedit += "<option value=\"always\">Always</option>";
  tdedit += "<option value=\"boolean\">Boolean</option>"; 
 }

 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"switchmode_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr><td style=\"text-align:left;\">Present:</td></tr>";
 tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"present_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"present_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";
 tdedit += "<tr><td style=\"text-align:left;\">Missing:</td></tr>";
 tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"missing_val\">";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"missing_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";

 tdedit += "<tr><td>";
 tdedit += "<input type=\"checkbox\" id=\"pad_val\" name=\"pad_val\">";
 tdedit += "Preserve with \"\" when not present";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"pad_oldval\" value=\"\"\></td>";
 tdedit += "</tr>";
 } else {
	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"switchmode_oldval\" value=\"\"\></td></tr>";
	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"present_oldval\" value=\"\"\></td></tr>";
	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"missing_oldval\" value=\"\"\></td></tr>";
	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"pad_oldval\" value=\"\"\></td></tr>";
 }
 tdedit += "<tr><td>";
 tdedit += "<input type=\"checkbox\" id=\"required_val\" name=\"required_val\">";
 tdedit += "Required";
 tdedit += "</td>";
 tdedit += "<td><input type=\"hidden\" name=\"required_oldval\" value=\"\"\>";
 tdedit += "<input type=\"hidden\" name=\"inpos_val\" value=\""+inpos+"\"\></td>";
 tdedit += "</tr>";
 
 tdedit += "</table><div id=\"errortextdiv\" class=\"fragmentfielderror\">&nbsp;</div></form>";

 console.log(tdedit);
 
 
 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {

    }
   });

     pwd.dialog("option", "title", "Add Argument");
     pwd.dialog("option", "height", "auto");
     pwd.dialog("option", "width", "300");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }

   function EditArgsButton(isswitch)
   {
    $("tr.ui-state-highlight").each(function(i, tr) {       
     var id = $(this).find("td").eq(0).attr("id");
     id = id.substring(5);

     var poskey = "#pos_" + id;  
     var name = $("#name_" + id).html();
     var sw1 = $("#sw1_" + id).html();
     var sw2 = $("#sw2_" + id).html();
     var padded = $("#pad_" + id).html();
     var pos = $("#pos_" + id).html();
     var inpos = $("#inpos_" + id).html();
     var required = $("#req_" + id).html();
     var typestr = $("#type_" + id).html();
     var swmode = $("#swmode_" + id).html();
     
     
     var pwd = parent.$("#modal");
     var buttons = [
                    {
                     text : "Ok",
                     click : function()
                     {
                      if (SaveArgsData(parent.$("#modal"))) parent.$("#modal").dialog("close");
                     }
                    },
                    {
                     text : "Cancel",
                     click : function()
                     {
                      parent.$("#modal").dialog("close");
                     }
                    } ];

     var tdedit = "<form id=\"Argsform\"><table id=\"Argstab\" class=\"summtable\">";
     tdedit += "<tr><td style=\"text-align:left;\">Name:</td></tr>";
     
     tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"name_val\" value=\"" + name + "\"\></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_oldval\"  value=\"" + name + "\"\></td></tr>";
     tdedit += "<tr><td>Argument Type:</td></tr>";
     tdedit += "<tr><td><select style=\"width:100%;\" name=\"type_val\" >";
     
     if (typestr.toLowerCase() == "entry")
      tdedit += "<option value=\"entry\" selected>Entry</option>";
     else
      tdedit += "<option value=\"entry\" >Entry</option>";
     
     if (typestr.toLowerCase() == "checkbox")
      tdedit += "<option value=\"checkbox\" selected>Checkbox</option>";
     else
      tdedit += "<option value=\"checkbox\">Checkbox</option>";
     
     tdedit += "</td>";
     tdedit += "<td><input type=\"hidden\" name=\"type_oldval\"  value=\"" + name + "\"\></td>";
     tdedit += "</tr>";
     
     if (summSaveobjkind != 1 && summSaveobjkind != 2) {
	     if (isswitch == "switch")
	     { 
	      tdedit += "<tr style=\"display:none\"><td>Switch Mode:</td></tr>";
	      tdedit += "<tr style=\"display:none\"><td><select style=\"width:100%;\" name=\"switchmode_val\" >";
	     }
	     else
	     {
	      tdedit += "<tr><td>Switch Mode:</td></tr>";
	      tdedit += "<tr><td><select style=\"width:100%;\" name=\"switchmode_val\" >";      
	     } 
	     
	
	     
	     if (isswitch == "switch")
	     { 
	      if (swmode.toLowerCase() == "switch")
	       tdedit += "<option value=\"switch\" selected>Switch</option>";
	      else
	       tdedit += "<option value=\"switch\" >Switch</option>"; 
	     }
	     
	      if (swmode.toLowerCase() == "prefix")     
	       tdedit += "<option value=\"prefix\" selected>Prefix</option>";
	      else
	       tdedit += "<option value=\"prefix\" >Prefix</option>";
	     
	      if (swmode.toLowerCase() == "always")     
	       tdedit += "<option value=\"always\" selected>Always</option>";
	      else
	       tdedit += "<option value=\"always\" >Always</option>"; 
	     
	      if (swmode.toLowerCase() == "boolean")      
	       tdedit += "<option value=\"boolean\" selected>Boolean</option>";
	      else
	       tdedit += "<option value=\"boolean\">Boolean</option>";
	     
	     tdedit += "</td>";
	     tdedit += "<td><input type=\"hidden\" name=\"switchmode_oldval\"  value=\"" + swmode + "\"\></td>";
	     tdedit += "</tr>";
	
	     tdedit += "<tr><td style=\"text-align:left;\">Present:</td></tr>";
	     tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"present_val\"  value=\"" + sw1 + "\"\>";
	     tdedit += "</td>";
	     tdedit += "<td><input type=\"hidden\" name=\"present_oldval\"  value=\"" + sw1 + "\"\></td>";
	     tdedit += "</tr>";
	     tdedit += "<tr><td style=\"text-align:left;\">Missing:</td></tr>";
	     tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"missing_val\"  value=\"" + sw2 + "\"\>";
	     tdedit += "</td>";
	     tdedit += "<td><input type=\"hidden\" name=\"missing_oldval\"  value=\"" + name + "\"\></td>";
	     tdedit += "</tr>";
	
	     tdedit += "<tr><td>";
	     if (padded.toLowerCase() == "true")  
	      tdedit += "<input type=\"checkbox\" id=\"pad_val\" name=\"pad_val\" checked>";
	     else
	      tdedit += "<input type=\"checkbox\" id=\"pad_val\" name=\"pad_val\">";
	     
	     tdedit += "Preserve with \"\" when not present";
	     tdedit += "</td>";
	     tdedit += "<td><input type=\"hidden\" name=\"pad_oldval\"  value=\"" + padded + "\"\></td>";
	     tdedit += "</tr>";
     } else {
    	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"switchmode_oldval\" value=\"\"\></td></tr>";
    	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"present_oldval\" value=\"\"\></td></tr>";
    	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"missing_oldval\" value=\"\"\></td></tr>";
    	 tdedit += "<tr style=\"display:none\"><td><input type=\"hidden\" name=\"pad_oldval\" value=\"\"\></td></tr>";
     }
     
     tdedit += "<tr><td>";
     if (required.toLowerCase() == "true")  
      tdedit += "<input type=\"checkbox\" id=\"required_val\" name=\"required_val\" checked>";
     else
      tdedit += "<input type=\"checkbox\" id=\"required_val\" name=\"required_val\">";
     tdedit += "Required";
     tdedit += "</td>";
     tdedit += "<td><input type=\"hidden\" name=\"required_oldval\"  value=\"" + required + "\"\\>";
     tdedit += "<input type=\"hidden\" name=\"inpos_val\" value=\""+inpos+"\"\></td>";
     tdedit += "</tr>";
     
     tdedit += "</table><div id=\"errortextdiv\" class=\"fragmentfielderror\">&nbsp;</div></form>";

     pwd.empty();
     pwd.html(tdedit);

     pwd.dialog(
       {
        open : function(event, ui)
        {

        }
       });

         pwd.dialog("option", "title", "Edit Argument");
         pwd.dialog("option", "height", "auto");
         pwd.dialog("option", "width", "300");
         pwd.dialog("option", "buttons", buttons);
         pwd.dialog('open');

    });
     
   }
   
   function EditSwitchButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr) { 
     var name = $(this).find("td").eq(0).html();
     var id = $(this).find("td").eq(0).attr("id");
     id = id.substring(4);
     
     var pwd = parent.$("#modal");
     var buttons = [
                    {
                     text : "Ok",
                     click : function()
                     {
                      SaveSwitchData(parent.$("#modal"));
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

     var tdedit = "<form id=\"Argsform\"><table id=\"Argstab\" class=\"summtable\">";
     tdedit += "<tr><td style=\"text-align:left;\">Name:</td></tr>";
     
     tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"name_val\" value=\"" + name + "\"\></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_oldval\"  value=\"" + name + "\"\></td></tr>";   
     tdedit += "<td><input type=\"hidden\" name=\"id_val\"  value=\"" + id + "\"\></td></tr>";   
     tdedit += "</table></form>";

     pwd.empty();
     pwd.html(tdedit);

     pwd.dialog(
       {
        open : function(event, ui)
        {

        }
       });

         pwd.dialog("option", "title", "Edit Switch");
         pwd.dialog("option", "height", "auto");
         pwd.dialog("option", "width", "300");
         pwd.dialog("option", "buttons", buttons);
         pwd.dialog('open');

    });
     
   }
   
   function AddSwitchButton()
   {
     var name = "";
     var id = "";
     
     var pwd = parent.$("#modal");
     var buttons = [
                    {
                     text : "Ok",
                     click : function()
                     {
                      SaveSwitchData(parent.$("#modal"));
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

     var tdedit = "<form id=\"Argsform\"><table id=\"Argstab\" class=\"summtable\">";
     tdedit += "<tr><td style=\"text-align:left;\">Name:</td></tr>";
     
     tdedit += "<tr><td><input type=\"text\" style=\"width:100%;\" name=\"name_val\" value=\"" + name + "\"\></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_oldval\"  value=\"" + name + "\"\></td></tr>";   
     tdedit += "<td><input type=\"hidden\" name=\"id_val\"  value=\"" + id + "\"\></td></tr>";   
     tdedit += "</table></form>";

     pwd.empty();
     pwd.html(tdedit);

     pwd.dialog(
       {
        open : function(event, ui)
        {

        }
       });

         pwd.dialog("option", "title", "Add Switch");
         pwd.dialog("option", "height", "auto");
         pwd.dialog("option", "width", "300");
         pwd.dialog("option", "buttons", buttons);
         pwd.dialog('open');
   }
   

   
   function EditProcButton(actionid,generate)
   {
	   var pwd = parent.$("#modal");
	   var buttons = generate?
			   [
			   {
			    	text : "Ok",
			    	click : function() {
			    		parent.$("#modal").dialog("close");
			    	}
			    }
			    ]:[
			    {
					text : "Ok",
					click : function() {
						save_action_id = actionid;
						okclicked = true;
						var savebutton = parent.$("#frame_editproc").contents().find("#a_save");
						console.log("save button = "+savebutton.attr("id"));
						savebutton.click();
					}
			    },
			    {
					text : "Cancel",
					click : function() {
						parent.$("#modal").dialog("close");
					}
			    }
			    ];
	
	 var tdedit = "<div style='width:95%;height:350px;'><form id=\"Procform\" style='height:100%;'>"
		 + "<textarea id='editproc' cols='400' wrap='off' spellcheck='false' style='width:100%;height:100%;font-size:14px;font-family:courier;white-space: pre; overflow: auto;'></textarea>";
	 tdedit += "</form></div><div id='parsearea'></div>";

     pwd.empty();
     pwd.html(tdedit);

     pwd.dialog({
        open : function(event, ui) {
        	LoadProcBody("editproc",objid,generate,!generate);
        }
     });

     pwd.dialog("option", "title", generate?"View Generated DMScript":"Edit Procedure Body");
     pwd.dialog("option", "height", "500");
     pwd.dialog("option", "width", "1000");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');   
   }
   
   function DeleteArgsButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr)
    {
     var id = $(this).find("td").eq(0).attr("id");
     id = id.substring(5);

     var poskey = "#pos_" + id;  
     var name = $("#name_" + id).html();
     var sw1 = $("#sw1_" + id).html();
     var sw2 = $("#sw2_" + id).html();
     var padded = $("#pad_" + id).html();
     var pos = $("#pos_" + id).html();
     var inpos = $("#inpos_" + id).html();
     var required = $("#req_" + id).html();
     var typestr = $("#type_" + id).html();
     var swmode = $("#swmode_" + id).html();
     
     if (summSaveobjkind != 1 && summSaveobjkind != 2) {  
	     if (swmode.toLowerCase() == "switch")       swmode = 'S';
	     else if (swmode.toLowerCase() == "prefix")  swmode = 'P';
	     else if (swmode.toLowerCase() == "always")  swmode = 'A';
	     else if (swmode.toLowerCase() == "boolean") swmode = 'B';
	     
	     padded=(padded == "true")?"Y":"N";
     } else {
    	 swmode = "A";
    	 padded = "N";
    	 sw1="";
    	 sw2="";
     }

     required=(required == "true")?"Y":"N";
     
     var outpos = "0"; 
     
     console.log("inpos="+inpos);

     var val = required + padded + swmode + outpos + "&" + inpos + "&" + encodeURIComponent(name) + "&"  + encodeURIComponent(sw1) + "&" + encodeURIComponent(sw2) + "&" + encodeURIComponent(typestr);
     var key = "del_" + name;
     var data = {id : summSaveobjid};
     
     data[key] = val;
     console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);
     $.ajax(
      {
       url: "GetActionArgsData?id=" + summSaveobjid + "&" + val,
       type: 'POST',
       dataType: "json",
       data: data,
       success: function(data)
       {
        LoadArgsData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveobjkind,summSaveaddParams);
       }
      });
    });
   }  
   
   function DeleteSwitchButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr)
    {
     var id = $(this).find("td").eq(0).attr("id");
     id = id.substring(4);

     var sw1 = $("#sw1_" + id).html();
     var swmode = "S";
     var padded = 'N';
     var required = 'Y';
     var outpos = "0";
     var inpos = "0";
     var name = id;
     var typestr = "false";
     var sw2 = "";

     var val = required + padded + swmode + outpos + "&" + inpos + "&" + encodeURIComponent(name) + "&"  + encodeURIComponent(sw1) + "&" + encodeURIComponent(sw2) + "&" + encodeURIComponent(typestr);
     var key = "del_" + name;
     var data = {id : summSaveobjid};
     
     data[key] = val;
     console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);
     
     $.ajax(
      {
       url: "GetActionArgsData?id=" + summSaveobjid + "&" + val,
       type: 'POST',
       dataType: "json",
       data: data,
       success: function(data)
       {
        LoadArgsData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveobjkind,summSaveaddParams);
       }
      });
    });
   }  
   
   function SaveArgsData(instance)
   {
    var form = instance.find("#Argsform");
    var viewArr = form.serializeArray();
    var view = {};
    
    console.log(viewArr);
    
    for ( var i in viewArr)
    {
    	console.log("viewArr["+i+"]="+viewArr[i].name+" val="+viewArr[i].value);
    	
     if (viewArr[i].value == "on")  viewArr[i].value = "Y";
     if (viewArr[i].value == "off") viewArr[i].value = "N";   
     view[viewArr[i].name] = viewArr[i].value;
    }
    
    if (view['name_val'].indexOf(" ") >= 0) {
    	$("#errortextdiv").html("Name cannot include spaces");
    	return false;
    } else {
    	$("#errortextdiv").html("");
    }

    if (view['pad_val'] != "Y" && view['pad_val'] != "N") view['pad_val'] = 'N';
    if (view['required_val'] != "Y" && view['required_val'] != "N") view['required_val'] = 'N';    
    
    if (view['switchmode_val'] == "switch")       view['switchmode_val'] = 'S';
    else if (view['switchmode_val'] == "prefix")  view['switchmode_val'] = 'P';
    else if (view['switchmode_val'] == "always")  view['switchmode_val'] = 'A';
    else if (view['switchmode_val'] == "boolean") view['switchmode_val'] = 'B';
    
    var outpos = "0";
    var inpos = view['inpos_val'];
    
     if (view['type_val'] == "checkbox")
      view['switchmode_val'] = 'B';
     else
      view['switchmode_val'] = 'S';
     
     var val = view['required_val'] + view['pad_val'] + view['switchmode_val'] + outpos + "&" + inpos + "&" + encodeURIComponent(view['name_val']) + "&"  + encodeURIComponent(view['present_val']) + "&" + encodeURIComponent(view['missing_val']) + "&" + encodeURIComponent(view['type_val']);
     var key = "";
     var data = {id : summSaveobjid};
     
     if (view["name_oldval"] == "")
      key = "add_" + view["name_val"];
     else
      key = "chg_" + view["name_oldval"];
     
     data[key] = val;
     console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);
     
     $.ajax(
      {
       url: "GetActionArgsData?id=" + summSaveobjid + "&" + val,
       type: 'POST',
       dataType: "json",
       data: data,
       success: function(data)
       {
        LoadArgsData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveobjkind,summSaveaddParams);
       }
      });
     return true;
   }

   function SaveSwitchData(instance)
   {
    var form = instance.find("#Argsform");
    var viewArr = form.serializeArray();
    var view = {};

    console.log(viewArr);
    
    for ( var i in viewArr)
    {
     if (viewArr[i].value == "on")
      viewArr[i].value = "Y";
     
     if (viewArr[i].value == "off")
      viewArr[i].value = "N";  
     
     view[viewArr[i].name] = viewArr[i].value;
    }
   
    var flag = view['name_val'];
    var name = view['id_val'];
    
    var action = "switchAction=U&"; 
    
    if (view["name_oldval"] == "")
    {
     for (k=1;k<1000;k++)
     {
      name = "arg" + k;
      if ($("#sw1_" + name).length == 0)
       break;
     }
     action = "switchAction=A&";
    } 
    
     var val = "name=" + encodeURIComponent(name) + "&"  + "flag=" + encodeURIComponent(flag);
     var key = name;
     var data = {id : summSaveobjid};

     
     data[key] = val;
     console.log("GetActionAgrsData?id=" + summSaveobjid + "&" + action + val);
     
     $.ajax(
      {
       url: "GetActionArgsData?id=" + summSaveobjid + "&" + action + val,
       type: 'POST',
       dataType: "json",
       data: data,
       success: function(data)
       {
         SaveArgOrdering();
         LoadArgsData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveobjkind,summSaveaddParams);
       }
      });
   }
   
   function getArgsTextWithMarkup(name,swmode,sw1,sw2,required,padded,inpos,outpos,outputArgs)
   {
    var nameMarkup = "<span class=\"argName\" id=\"spanout_" + name + "\">" + name + '</span>';
    var ret = '';
    var mode = swmode;
    
    if (swmode.toLowerCase() == "switch")
     mode = 'S';
    else if (swmode.toLowerCase() == "prefix")
     mode = 'P';
    else if (swmode.toLowerCase() == "always")
     mode = 'A';
    else if (swmode.toLowerCase() == "boolean")
     mode = 'B';
    
    switch(mode) {
    case 'S': //MODE_SWITCH
     if((sw1 != null) && (sw1 != '')) ret += '<b>'+sw1+'</b>&nbsp;';
     ret += nameMarkup;
     if((sw2 != null) && (sw2 != '')) ret += '&nbsp;/&nbsp;<b>'+sw2+'</b>';
     break;
    case 'P': //MODE_PREFIX
     if((sw1 != null) && (sw1 != '')) ret += '<b>'+sw1+'</b>';
     ret += nameMarkup;
     if((sw2 != null) && (sw2 != '')) ret += '&nbsp;/&nbsp;<b>'+sw2+'</b>';
     break;
    case 'A': //MODE_ALWAYS
     ret = '<b>'+sw1+'</b>';
     break;
    case 'B': //MODE_BOOLEAN
     ret = '';
     if((sw1 != null) && (sw1 != '')) ret += '<b>'+sw1+'</b>';
     ret += '&nbsp;/&nbsp;';
     if((sw2 != null) && (sw2 != '')) ret += '<b>'+sw2+'</b>';
     ret += name;
     break;
    default:
     ret = nameMarkup;
     break;
    }
    
    var hasclasses = "";
    
    if (required)
     hasclasses = "actionarg_required ";
    else
     hasclasses = "actionarg_optional ";
        
   if (padded)
    hasclasses += "actionarg_padded ";
   
    ret = "<li id=\"out_" + name + "\" class=\"" + hasclasses + "\">" + ret + "</li>";
  
    if (outpos >= 1)
    {
     outputArgs["key" + zeroPad(outpos,3) + "_" + zeroPad(inpos,3) ] = ret;
     return "";
    }
    
    return ret;
   }
   
 function zeroPad(num, places) {
    var zero = places - num.toString().length + 1;
    return Array(+(zero > 0 && zero)).join("0") + num;
 }
   
function InitArgsDnD()
{
   $('#argspalette').sortable({
    axis: "xy",
    connectWith: ".argConnection",
    cursor: "move",
    forcePlaceholderSize: true,
    forceHelperSize: true,
    helper: "clone",
    placeholder: "ui-state-highlight",
    start: function(event, ui) {
           // Re-show the item we are cloning
              $(ui.item).show();
          },
          receive: function(event, ui) {
           // Disallow anything from being dropped on the palette
           $(ui.sender).sortable("cancel");
          },
          stop: function(event, ui) {
           SaveArgOrdering();
          }
   });
   $('#argsoutput').sortable({
    axis: "xy",
    connectWith: ".argConnection",
    cursor: "move",
    items: "li:not(.ui-state-disabled)",
    forcePlaceholderSize: true,
    placeholder: "ui-state-highlight",
    stop: function( event, ui ) {
     console.log('output stop');
              var idx = $('#argsoutput').children().index($(ui.item[0]));
              if(idx== -1) {
               return;
              }
              SaveArgOrdering();
    }
   });
   $('#argsoutput, #argspalette').disableSelection();
   $('#argsoutput li').dmContextMenu(GetArgsMenu, {theme:'xp'});
   
   $('#argsoutput').on('mouseenter', '.tooltip-follows', function() {
    $(this).next('.tooltip').fadeIn();
    $(this).next('.tooltip').position({
     at: 'bottom center',
     of: $(this),
     my: 'top-24'
    });
   }).on('mouseleave', '.tooltip-follows', function() {
    $('.tooltip').fadeOut();
   });
}  



function SaveArgOrdering()
{
 var listItems = $("#argsoutput li");
 var pos = 0;
 var data = 
 {
  id : summSaveobjid
 };

 pos = 1;
 
 for (k=0;k<listItems.length;k++)
 {
  var li = listItems[k];

   var id = $(li).attr("id");

   if (typeof id != "undefined")
   { 
    id = id.substr(4);
    data["pos_" + pos] = id;
    pos++;
   }
 }

 
 console.log(data);
  console.log("GetActionAgrsData?id=" + summSaveobjid + "&updateOrder=Y");
  
  $.ajax(
  {
   url : "GetActionArgsData?id=" + summSaveobjid + "&updateOrder=Y",
   type : 'POST',
   aysnc : false,
   dataType : "json",
   data : data,
   success : function(data)
   {
    LoadArgsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveobjkind, summSaveaddParams);
   }
  });
}


function DownArgsButton()
{
 var rowToMove = $("#inputparamstable").find('tr.ui-state-highlight:first');
 var next = rowToMove.next('tr.clickablerow');
 if (next.length == 1) { next.after(rowToMove); SaveInputArgs();}
}

function UpArgsButton()
{
 var rowToMove = $("#inputparamstable").find('tr.ui-state-highlight:first');
 var prev = rowToMove.prev('tr.clickablerow');
 if (prev.length == 1) { prev.before(rowToMove); SaveInputArgs(); }
}

function SaveInputArgs()
{
 var listItems =$ ("#inputparamstable").find('tr');
 
 var data =
 {
  id : summSaveobjid
 };

 for (k=0;k<listItems.length;k++)
 {
  if (k>0)  // skip header row
  {
   var li = listItems[k];
   var id = $(li).find("td").eq(0).attr("id");
   if (typeof id != "undefined") {
	   id = id.substring(5);
	   data[id] = k;
   }
  }
 }
 
  console.log("GetActionAgrsData?id=" + summSaveobjid + "&updateInputOrder=Y");
  
  $.ajax(
  {
   url : "GetActionArgsData?id=" + summSaveobjid + "&updateInputOrder=Y",
   type : 'POST',
   aysnc : false,
   dataType : "json",
   data : data,
   success : function(data)
   {
   }
  });
}

function GetArgsMenu(menu,target)
{
 var ret = [];
 if(!$(target).hasClass('ui-state-disabled')) {
  ret.push({'Remove': { icon:'images/delete.png', onclick:function(item, menu) { RemoveArgument($(this).data('id'), $(this)); }}});
 }
 return ret;
}

function RemoveArgument(id, target)
{
 var poskey = "#pos_" + target.attr('id').substring(4);
 $(poskey).html("");
 target.remove();
 
 SaveArgOrdering();
}



function SaveProcBody(divid,text)
{
	console.log("SaveProcBody");
	   $.ajax(
	      {
	    	  url: "GetAttachment",
	    	  type: 'POST',
	    	  dataType: "json",
	    	  data: {
	    		  actionid:	save_action_id,
	    		  procbody:	text
	    	  },
	    	  success: function(data) {
	    		  console.log("data="+data);
	    		  if (data.success) {
	    			  console.log("success");
	    			  LoadProcBody("ProcBodyTextArea",save_action_id,false,false);
	    			  $("#parsearea").html("");
	    			  if (okclicked) parent.$("#modal").dialog("close");
	    			  okclicked=false;
	    		  } else {
	    			  console.log("failed with "+data.parseResult);
	    			  $("#parsearea").html(data.parseResult);
	    			  editAreaLoader.setSelectionRange(divid, data.sp, data.ep);
	    			  okclicked=false;
	    		  }
	    	  },
	    	  error: function(data) {
	    		  console.log("FAILURE: data=");
	    		  console.log(data);
	    	  }
	   });
}

function LoadProcBody(divid,actionid,generate,editmode)
{
	console.log("LoadProcBody "+actionid+" into divid "+divid+" editmode="+editmode);
	save_action_id = actionid;
	var mtb1 = "search, go_to_line, |, select_font,|, highlight, reset_highlight, word_wrap";
	var mtb2 = "save, search, go_to_line, |, undo, redo, |, select_font,|, highlight, reset_highlight, word_wrap";
	var mtb = (editmode)?mtb2:mtb1;
	console.log("mtb="+mtb);
	editAreaLoader.init({
		id : divid,					// textarea id
		syntax: "dmscript",			// syntax to be uses for highlighting
		start_highlight: true,		// to display with highlight mode on start-up
		toolbar: mtb,
		browsers: "all",
		is_editable: editmode,
		allow_toggle: false,
		save_callback: "SaveProcBody"
	});
	$.ajax(
      {
       url: generate?"ActionDetails?actionid=" + actionid:"GetAttachment?actionid=" + actionid,
       type: 'GET',
       success: function(data)
       {
    	   console.log("data = ["+data+"]");
    	   if (data.substr(0,6)=="ERROR:") {
    		   // Just show first line (ignore all the Unhandled exception gubbins
    		   var a = data.split('\n');
    		   console.log("numlines="+a.length);
    		   for (var i=0;i<a.length;i++) {
    			   var errLine = a[i];
    			   console.log("errLine="+errLine);
    			   if (errLine.length >5 && errLine.indexOf("Unhandled exception") == -1) {
    				   errmsg = errLine.replace("ERROR: ERROR:","ERROR:");
    			   }
    		   }
    		   parent.$("#modal").dialog("close");
    		   alert(errmsg);
    	   } else {
    		   editAreaLoader.setValue(divid,data);
    	   }
       }
      });
	
}