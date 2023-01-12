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

var save_action_id;
var okclicked = false;
var maxinpos = 0;

function LoadArgsData(tablename, objtypeAsInt, objtype, objid, objkind, addParams)
{
 $('#inputparams_header_buttons > button.delete_button').css("color", "grey");
 $('#switchargs_header_buttons > button.delete_button').css("color", "grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 inputparams_tdedit = "";
 inputparams_rowcnt = 0;
 switchargs_tdedit = "";
 switchargs_rowcnt = 0;

 maxinpos = 0;

 console.log("objkind=" + objkind + " Calling GetActionArgsData?id=" + objid);
 $.getJSON('GetActionArgsData', "id=" + summSaveobjid, function(res)
 {
  var td = "";
  var switchargs_td = "";
  var inputtd = "";
  var args = "<ul id=\"argspalette\" class=\"actionarglist argConnection\">";
  var rowcnt = 0;
  var inputrowcnt = 0;
  var filepath = res.filepath;
  var interpreter = res.interpreter;
  var outputArgs =
  {};
  var typedropdown = '<option value="entry">entry</option><option value="checkbox">checkbox</option>';

  for (a = 0; a < res.data.length; a++)
  {
   var row = res.data[a];
   console.log(row);
   var swmode = "Switch";
   var key = row[0].replace(/[^0-9A-Za-z_]/g, "");
   var name = row[4];
   var val = row[4];
   var present = row[11];
   var missing = row[12];
   var padded = row[7];
   var inpos = row[8];
   var outpos = row[9];
   var required = row[6];
   var typestr = row[5];

   rowcnt++;

   if (padded)
    padded = "Yes";
   else
    padded = "No";

   if (required)
    required = "Yes";
   else
    required = "No";

   if (inpos == "")
    inpos = 0;
   if (outpos == "")
    outpos = 0;

   if (row[10] == "S")
    swmode = "Switch";
   else if (row[10] == 'P')
    swmode = "Prefix";
   else if (row[10] == "A")
    swmode = "Always";
   else if (row[10] == "B")
    swmode = "Boolean";

   console.log("typestr=" + typestr + " objkind=" + objkind);

   if (inpos > maxinpos)
    maxinpos = inpos;

   if (typestr != "false")
   {
    inputparams_rowcnt++;

    if (objkind == 1 || objkind == 2)
    {
     inputparams_tdedit += '<tr id="inputparams_row_' + inputparams_rowcnt + '">';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_edit_cb_' + inputparams_rowcnt + '" onchange="toggleInputParamsDelete(this);" />&nbsp;</td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_key_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%;"></td>';
     inputparams_tdedit += '<td><select id="inputparams_type_' + inputparams_rowcnt + '">' + typedropdown + '</select></td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_present_' + inputparams_rowcnt + '" value="' + present + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_missing_' + inputparams_rowcnt + '" value="' + missing + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_pad_' + inputparams_rowcnt + '" /></td>';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_req_' + inputparams_rowcnt + '" /></td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_inpos_' + inputparams_rowcnt + '" value="' + inpos + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_pos_' + inputparams_rowcnt + '" value="' + outpos + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldkey_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldreq_' + inputparams_rowcnt + '" value="' + required + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldpad_' + inputparams_rowcnt + '" value="' + padded + '" style="width:100%"></td></tr>';
     td += "<tr><td><input type=\"checkbox\" id=\"inputparams_cb_" + inputparams_rowcnt + "\" onchange=\"toggleInputParamsDelete(this);\" />&nbsp;</td><td>" + name + "</td>";
     td += "<td>" + typestr + "</td>";
     td += "<td>" + present + "</td>";
     td += "<td>" + missing + "</td>";
     td += "<td>" + padded + "</td>"
     td += "<td>" + required + "</td>"
     td += "<td>" + inpos + "</td>"
     td += "</tr>";
    }
    else
    {
     inputparams_tdedit += '<tr id="inputparams_row_' + inputparams_rowcnt + '">';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_edit_cb_' + inputparams_rowcnt + '" onchange="toggleInputParamsDelete(this);" />&nbsp;</td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_key_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%;"></td>';
     inputparams_tdedit += '<td><select id="inputparams_type_' + inputparams_rowcnt + '">' + typedropdown + '</select></td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_present_' + inputparams_rowcnt + '" value="' + present + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="text" id="inputparams_missing_' + inputparams_rowcnt + '" value="' + missing + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_pad_' + inputparams_rowcnt + '" /></td>';
     inputparams_tdedit += '<td><input type="checkbox" id="inputparams_req_' + inputparams_rowcnt + '" /></td>';
     inputparams_tdedit += '<td><input type="text"" id="inputparams_inpos_' + inputparams_rowcnt + '" value="' + inpos + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_swmode_' + inputparams_rowcnt + '" value="' + swmode + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_pos_' + inputparams_rowcnt + '" value="' + outpos + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldkey_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldreq_' + inputparams_rowcnt + '" value="' + required + '" style="width:100%"></td>';
     inputparams_tdedit += '<td><input type="hidden" id="inputparams_oldpad_' + inputparams_rowcnt + '" value="' + padded + '" style="width:100%"></td></tr>';
     td += "<tr><td><input type=\"checkbox\" id=\"inputparams_cb_" + inputparams_rowcnt + "\" onchange=\"toggleInputParamsDelete(this);\" />&nbsp;</td><td>" + name + "</td>";
     td += "<td>" + typestr + "</td>";
     td += "<td>" + present + "</td>";
     td += "<td>" + missing + "</td>";
     td += "<td>" + padded + "</td>"
     td += "<td>" + required + "</td>"
     td += "<td>" + inpos + "</td>"
     td += "</tr>";

     args += getArgsTextWithMarkup(name, swmode, present, missing, required, padded, a + 1, outpos, outputArgs);
    }
   }
   else
   {
    switchargs_rowcnt++;
    switchargs_tdedit += '<tr id="switchargs_row_' + switchargs_rowcnt + '">';
    switchargs_tdedit += '<td><input type="checkbox" id="switchargs_edit_cb_' + switchargs_rowcnt + '" onchange="toggleSwitchArgsDelete(this);" />&nbsp;</td>';
    switchargs_tdedit += '<td><input type="text" id="switchargs_name_' + switchargs_rowcnt + '" value="' + present + '" style="width:100%;"></td>';
    switchargs_tdedit += '<td><input type="hidden" id="switchargs_key_' + switchargs_rowcnt + '" value="' + key + '" style="width:100%;"></td>';
    switchargs_tdedit += '<td><input type="hidden" id="switchargs_swmode_' + switchargs_rowcnt + '" value="' + swmode + '" style="width:100%"></td>';
    switchargs_td += "<tr><td><input type=\"checkbox\" id=\"switchargs_cb_" + switchargs_rowcnt + "\" onchange=\"toggleSwitchArgsDelete(this);\" />&nbsp;</td><td>" + present + "</td>";
    switchargs_td += "</tr>";
    args += getArgsTextWithMarkup(name, swmode, present, missing, required, padded, a + 1, outpos, outputArgs);
   }
  }

  if (objkind != 1 && objkind != 2)
  {
   args += "</ul><br><h2>Command Line</h2><ul id=\"argsoutput\" class=\"actionarglist argConnection\">";
   args += "<li class=\"actionarg_required ui-state-disabled\"><span class=\"argName\">" + interpreter + "</span></li>";
   args += "<li class=\"actionarg_required ui-state-disabled\"><span class=\"argName\">" + filepath + "</span></li>";
  }
  var keys = Object.keys(outputArgs).sort();

  for (var x = 0; x < keys.length; x++)
  {
   if (typeof outputArgs[keys[x]] != "undefined")
    args += outputArgs[keys[x]];
  }

  args += "</ul>";

  $("#" + tablename + " > tbody").html(td);
  $("#" + tablename + "-edit > tbody").html(inputparams_tdedit);
  $("#switchargs > tbody").html(switchargs_td);
  $("#switchargs-edit > tbody").html(switchargs_tdedit);

  rowcnt = 0;

  for (a = 0; a < res.data.length; a++)
  {
   rowcnt++;

   var id = $("#inputparams_oldtype_" + rowcnt).val();
   $("#inputparams_type_" + rowcnt + " option[value=" + id + "]").attr('selected', true);

   id = $("#inputparams_oldreq_" + rowcnt).val();
   if (id == "Yes")
    $("#inputparams_req_" + rowcnt).prop("checked", true);
   else
    $("#inputparams_req_" + rowcnt).prop("checked", false);

   id = $("#inputparams_oldpad_" + rowcnt).val();
   if (id == "Yes")
    $("#inputparams_pad_" + rowcnt).prop("checked", true);
   else
    $("#inputparams_pad_" + rowcnt).prop("checked", false);
  }

  $("#argsoutputpane").html(args);

  if (!res.readonly)
  {
   InitArgsDnD();
  }
 });

 if (objkind == 1 || objkind == 2)
 {
  LoadProcBody("editproc",objid,false,true);
 }
 else
 {
  $("#cmdline-panel").show();
  $("#switchargs-panel").show();
 }
}

//   var savebutton = parent.$("#frame_editproc").contents().find("#a_save");
//   console.log("save button = " + savebutton.attr("id"));
//   savebutton.click();


function DeleteInputParamsData(rowid)
{
 var key = $("#inputparams_key_" + rowid).val();
 var name = $("#inputparams_oldkey_" + rowid).val();
 var typestr = $("#inputparams_type_" + rowid).val();
 var padded = $("#inputparams_pad_" + rowid).is(":checked");
 var required = $("#inputparams_req_" + rowid).is(":checked");
 var pos = $("#inputparams_pos_" + +rowid).val();
 var inpos = $("#inputparams_inpos_" + +rowid).val();
 var swmode = $("#inputparams_swmode_" + +rowid).val();
 var present = $("#inputparams_present_" + +rowid).val();
 var missing = $("#inputparams_missing_" + +rowid).val();

 if (summSaveobjkind != 1 && summSaveobjkind != 2)
 {
  if (swmode.toLowerCase() == "switch")
   swmode = 'S';
  else if (swmode.toLowerCase() == "prefix")
   swmode = 'P';
  else if (swmode.toLowerCase() == "always")
   swmode = 'A';
  else if (swmode.toLowerCase() == "boolean")
   swmode = 'B';

  padded = (padded == "true") ? "Y" : "N";
 }
 else
 {
  swmode = "A";
  padded = "N";
  present = "";
  missing = "";
 }

 required = (required == "true") ? "Y" : "N";

 var outpos = "0";

 console.log("inpos=" + inpos);

 var val = required + padded + swmode + outpos + "&" + inpos + "&" + encodeURIComponent(name) + "&" + encodeURIComponent(present) + "&" + encodeURIComponent(missing) + "&"
   + encodeURIComponent(typestr);
 var key = "del_" + name;
 var data =
 {
  id : summSaveobjid
 };

 data[key] = val;
 console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);
 $.ajax(
 {
  url : "GetActionArgsData?id=" + summSaveobjid + "&" + val,
  type : 'POST',
  dataType : "json",
  async : false,
  data : data,
  success : function(data)
  {
  }
 });
}

function DeleteSwitchArgsData(rowid)
{
 var key = $("#switchargs_key_" + rowid).val();
 var name = $("#switchargs_name_" + rowid).val();

 var val = "YNS0&0&" + encodeURIComponent(key) + "&" + encodeURIComponent(name) + "&&false";
 var key = "del_" + key;
 var data =
 {
  id : summSaveobjid
 };

 data[key] = val;
 console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);

 $.ajax(
 {
  url : "GetActionArgsData?id=" + summSaveobjid + "&" + val,
  type : 'POST',
  async: false,
  dataType : "json",
  data : data,
  success : function(data)
  {
  }
 });
}

function hasDuplicates(arr)
{
 var counts = [];

 for (var i = 0; i <= arr.length; i++)
 {
  if (counts[arr[i]] === undefined)
  {
   counts[arr[i]] = 1;
  }
  else
  {
   return true;
  }
 }
 return false;
}

function SaveInputParamsData(tablename)
{
 var error = false;
 var duppos = [];
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  var key = $("#inputparams_key_" + rowid).val();
  var tn = key.replace(/[^0-9A-Za-z_]/g, "");
  if (tn != key)
  {
   $.alert(
   {
    boxWidth : '30%',
    useBootstrap : false,
    title : 'Invalid Name',
    content : 'The name must contain only letters, numbers and underscores, starting with a letter.',
   });
   $("#" + tablename + '-edit').show();
   $("#" + tablename).hide();
   error = true;
  }

  var inpos = $("#inputparams_inpos_" + rowid).val();
  duppos.push(inpos);

  if (hasDuplicates(duppos))
  {
   $.alert(
   {
    boxWidth : '30%',
    useBootstrap : false,
    title : 'Duplicate Position',
    content : 'The argument positions need to be unique.',
   });
   $("#" + tablename + '-edit').show();
   $("#" + tablename).hide();
   error = true;
  }
 });

 if (!error)
 {
  duppos.sort(function(a, b)
  {
   return a - b;
  });

  for (var i = 0; i < duppos.length; i++)
  {
   if (duppos[i] != (i + 1))
   {
    $.alert(
    {
     boxWidth : '30%',
     useBootstrap : false,
     title : 'Position Sequence Invalid',
     content : 'The argument positions need to be sequential.',
    });

    $("#" + tablename + '-edit').show();
    $("#" + tablename).hide();
    error = true;
    break;
   }
  }
 }

 if (error)
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  var key = $("#inputparams_key_" + rowid).val();
  var oldkey = $("#inputparams_oldkey_" + rowid).val();
  var type = $("#inputparams_type_" + rowid).val();
  var pad = $("#inputparams_pad_" + rowid).is(":checked");
  var req = $("#inputparams_req_" + rowid).is(":checked");
  var pos = $("#inputparams_pos_" + +rowid).val();
  var inpos = $("#inputparams_inpos_" + +rowid).val();
  var swmode = $("#inputparams_swmode_" + +rowid).val();
  var present = $("#inputparams_present_" + +rowid).val();
  var missing = $("#inputparams_missing_" + +rowid).val();

  if (key == null)
   return;

  if (typeof pad == "undefined")
   pad = false;

  if (typeof req == "undefined")
   req = false;

  if (swmode.toLowerCase() == "switch")
   swmode = 'S';
  else if (swmode.toLowerCase() == "prefix")
   swmode = 'P';
  else if (swmode.toLowerCase() == "always")
   swmode = 'A';
  else if (swmode.toLowerCase() == "boolean")
   swmode = 'B';

  if (type == "checkbox")
   swmode = 'B';
  else
   swmode = 'S';

  req = (req == true) ? "Y" : "N";
  pad = (pad == true) ? "Y" : "N";

  var val = req + pad + swmode + pos + "&" + inpos + "&" + encodeURIComponent(key) + "&" + encodeURIComponent(present) + "&" + encodeURIComponent(missing) + "&" + encodeURIComponent(type);
  var key = "";
  var data =
  {
   id : summSaveobjid
  };

  if (oldkey == "")
   key = "add_" + key;
  else
   key = "chg_" + oldkey;

  data[key] = val;
  console.log("GetActionArgsData?id=" + summSaveobjid + "&" + val);

  $.ajax(
  {
   url : "GetActionArgsData?id=" + summSaveobjid + "&" + val,
   type : 'POST',
   async : false,
   dataType : "json",
   data : data,
   success : function(data)
   {

   }
  });
 });
 LoadArgsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveobjkind, summSaveaddParams);
 return true;
}

function SaveSwitchArgsData(tablename)
{
 var error = false;
 var duppos = [];
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  var action = "switchAction=U&";
  var key = $("#switchargs_key_" + rowid).val();
  var name = $("#switchargs_name_" + rowid).val();
  var oldkey = $("#switchargs_oldkey_" + rowid).val();
  var swmode = $("#switchargs_swmode_" + +rowid).val();

  if (oldkey == "")
   action = "switchAction=A&";

  var val = "name=" + encodeURIComponent(key) + "&" + "flag=" + encodeURIComponent(name);
  var data =
  {
   id : summSaveobjid
  };

  data[key] = val;
  console.log("GetActionAgrsData?id=" + summSaveobjid + "&" + action + val);

  $.ajax(
  {
   url : "GetActionArgsData?id=" + summSaveobjid + "&" + action + val,
   type : 'POST',
   async: false,
   dataType : "json",
   data : data,
   success : function(data)
   {
   }
  });
 });
 LoadArgsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveobjkind, summSaveaddParams);
}

function getArgsTextWithMarkup(name, swmode, present, missing, required, padded, inpos, outpos, outputArgs)
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

 switch (mode)
 {
 case 'S': // MODE_SWITCH
  if ((present != null) && (present != ''))
   ret += '<b>' + present + '</b>&nbsp;';
  ret += nameMarkup;
  if ((missing != null) && (missing != ''))
   ret += '&nbsp;/&nbsp;<b>' + missing + '</b>';
  break;
 case 'P': // MODE_PREFIX
  if ((present != null) && (present != ''))
   ret += '<b>' + present + '</b>';
  ret += nameMarkup;
  if ((missing != null) && (missing != ''))
   ret += '&nbsp;/&nbsp;<b>' + missing + '</b>';
  break;
 case 'A': // MODE_ALWAYS
  ret = '<b>' + present + '</b>';
  break;
 case 'B': // MODE_BOOLEAN
  ret = '';
  if ((present != null) && (present != ''))
   ret += '<b>' + present + '</b>';
  ret += '&nbsp;/&nbsp;';
  if ((missing != null) && (missing != ''))
   ret += '<b>' + missing + '</b>';
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
  outputArgs["key" + zeroPad(outpos, 3) + "_" + zeroPad(inpos, 3)] = ret;
  return "";
 }

 return ret;
}

function zeroPad(num, places)
{
 var zero = places - num.toString().length + 1;
 return Array(+(zero > 0 && zero)).join("0") + num;
}

function InitArgsDnD()
{
 $('#argspalette').sortable(
 {
  axis : "xy",
  connectWith : ".argConnection",
  cursor : "move",
  forcePlaceholderSize : true,
  forceHelperSize : true,
  helper : "clone",
  placeholder : "ui-state-highlight",
  start : function(event, ui)
  {
   // Re-show the item we are cloning
   $(ui.item).show();
  },
  receive : function(event, ui)
  {
   // Disallow anything from being dropped on the palette
   $(ui.sender).sortable("cancel");
  },
  stop : function(event, ui)
  {
   SaveArgOrdering();
  }
 });
 $('#argsoutput').sortable(
 {
  axis : "xy",
  connectWith : ".argConnection",
  cursor : "move",
  items : "li:not(.ui-state-disabled)",
  forcePlaceholderSize : true,
  placeholder : "ui-state-highlight",
  stop : function(event, ui)
  {
   console.log('output stop');
   var idx = $('#argsoutput').children().index($(ui.item[0]));
   if (idx == -1)
   {
    return;
   }
   SaveArgOrdering();
  }
 });
 $('#argsoutput, #argspalette').disableSelection();
 $('#argsoutput li').dmContextMenu(GetArgsMenu,
 {
  theme : 'xp'
 });

 $('#argsoutput').on('mouseenter', '.tooltip-follows', function()
 {
  $(this).next('.tooltip').fadeIn();
  $(this).next('.tooltip').position(
  {
   at : 'bottom center',
   of : $(this),
   my : 'top-24'
  });
 }).on('mouseleave', '.tooltip-follows', function()
 {
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

 for (k = 0; k < listItems.length; k++)
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

function SaveInputArgs()
{
 var listItems = $("#inputparamstable").find('tr');

 var data =
 {
  id : summSaveobjid
 };

 for (k = 0; k < listItems.length; k++)
 {
  if (k > 0) // skip header row
  {
   var li = listItems[k];
   var id = $(li).find("td").eq(0).attr("id");
   if (typeof id != "undefined")
   {
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

function GetArgsMenu(menu, target)
{
 var ret = [];
 if (!$(target).hasClass('ui-state-disabled'))
 {
  ret.push(
  {
   'Remove' :
   {
    icon : 'images/delete.png',
    onclick : function(item, menu)
    {
     RemoveArgument($(this).data('id'), $(this));
    }
   }
  });
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

function SaveProcBody(divid, text)
{
 console.log("SaveProcBody");
 $.ajax(
 {
  url : "GetAttachment",
  type : 'POST',
  dataType : "json",
  data :
  {
   actionid : save_action_id,
   procbody : text
  },
  success : function(data)
  {
   console.log("data=" + data);
   if (data.success)
   {
    console.log("success");
    $("#parsearea").html("");
    okclicked = false;
   }
   else
   {
    console.log("failed with " + data.parseResult);
    $("#parsearea").html(data.parseResult);
    editAreaLoader.setSelectionRange(divid, data.sp, data.ep);
    okclicked = false;
   }
  },
  error : function(data)
  {
   console.log("FAILURE: data=");
   console.log(data);
  }
 });
}

function LoadProcBody(divid, actionid, generate, editmode)
{
 $("#cmdline-panel").hide();
 $("#switchargs-panel").hide();
 $("#procbody-panel").show();
 $("#tabs-General-row-28").show();

 console.log("LoadProcBody " + actionid + " into divid " + divid + " editmode=" + editmode);
 save_action_id = actionid;
 var mtb1 = "search, go_to_line, |, select_font,|, highlight, reset_highlight, word_wrap";
 var mtb2 = "save, search, go_to_line, |, undo, redo, |, select_font,|, highlight, reset_highlight, word_wrap";
 var mtb = (editmode) ? mtb2 : mtb1;
 console.log("mtb=" + mtb);
 editAreaLoader.init(
 {
  id : divid, // textarea id
  syntax : "dmscript", // syntax to be uses for highlighting
  start_highlight : true, // to display with highlight mode on start-up
  toolbar : mtb,
  browsers : "all",
  is_editable : editmode,
  allow_toggle : false,
  allow_resize : "no",
  save_callback : "SaveProcBody"
 });
 $.ajax(
 {
  url : generate ? "ActionDetails?actionid=" + actionid : "GetAttachment?actionid=" + actionid,
  type : 'GET',
  success : function(data)
  {
   console.log("data = [" + data + "]");
   if (data.substr(0, 6) == "ERROR:")
   {
    // Just show first line (ignore all the Unhandled exception gubbins
    var a = data.split('\n');
    console.log("numlines=" + a.length);
    for (var i = 0; i < a.length; i++)
    {
     var errLine = a[i];
     console.log("errLine=" + errLine);
     if (errLine.length > 5 && errLine.indexOf("Unhandled exception") == -1)
     {
      errmsg = errLine.replace("ERROR: ERROR:", "ERROR:");
     }
    }
   }
   else
   {
    editAreaLoader.setValue(divid, data);
   }
  }
 });

}

function AddInputParamsRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditInputParamsRow(tablename);

 var key = "";
 var val = "";
 var present = "";
 var missing = "";
 var swmode = "Switch";
 var outpos = 0;
 var inpos = 0;
 var required = "";
 var padded = "";
 var typedropdown = '<option value="entry">entry</option><option value="checkbox">checkbox</option>';

 inputparams_rowcnt++;

 var td = '<tr id="inputparams_row_' + inputparams_rowcnt + '">';
 td += '<td><input type="checkbox" id="inputparams_edit_cb_' + inputparams_rowcnt + '" onchange="toggleInputParamsDelete(this);" />&nbsp;</td>';
 td += '<td><input type="text" id="inputparams_key_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%;"></td>';
 td += '<td><select id="inputparams_type_' + inputparams_rowcnt + '">' + typedropdown + '</select></td>';
 td += '<td><input type="text" id="inputparams_present_' + inputparams_rowcnt + '" value="' + present + '" style="width:100%"></td>';
 td += '<td><input type="text" id="inputparams_missing_' + inputparams_rowcnt + '" value="' + missing + '" style="width:100%"></td>';
 td += '<td><input type="checkbox" id="inputparams_pad_' + inputparams_rowcnt + '" /></td>';
 td += '<td><input type="checkbox" id="inputparams_req_' + inputparams_rowcnt + '" /></td>';
 td += '<td><input type="text"" id="inputparams_inpos_' + inputparams_rowcnt + '" value="' + inputparams_rowcnt + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="inputparams_swmode_' + inputparams_rowcnt + '" value="' + swmode + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="inputparams_pos_' + inputparams_rowcnt + '" value="' + outpos + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="inputparams_oldkey_' + inputparams_rowcnt + '" value="' + key + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="inputparams_oldreq_' + inputparams_rowcnt + '" value="' + required + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="inputparams_oldpad_' + inputparams_rowcnt + '" value="' + padded + '" style="width:100%"></td></tr>';

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelInputParamsRow(tablename)
{
 if ($('#inputparams_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#inputparams_header_buttons > button.save_button').css("color", "grey");
 $('#inputparams_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditInputParamsRow(tablename)
{
 $('#inputparams_header_buttons > button.save_button').css("color", "#3367d6");
 $('#inputparams_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(inputparams_tdedit);

 for (var r = 0; r < inputparams_rowcnt; r++)
 {
  var rowcnt = r + 1;

  var id = $("#inputparams_oldtype_" + rowcnt).val();
  $("#inputparams_type_" + rowcnt + " option[value=" + id + "]").attr('selected', true);

  id = $("#inputparams_oldreq_" + rowcnt).val();
  if (id == "Yes")
   $("#inputparams_req_" + rowcnt).prop("checked", true);
  else
   $("#inputparams_req_" + rowcnt).prop("checked", false);

  id = $("#inputparams_oldpad_" + rowcnt).val();
  if (id == "Yes")
   $("#inputparams_pad_" + rowcnt).prop("checked", true);
  else
   $("#inputparams_pad_" + rowcnt).prop("checked", false);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteInputParamsRow(tablename)
{
 if ($('#inputparams_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#inputparams_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   DeleteInputParamsData(rowid);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#inputparams_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   DeleteInputParamsData(rowid);
  }
 });

 LoadArgsData(summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveobjkind, summSaveaddParams);
}

function SaveInputParamsRow(tablename)
{
 if ($('#inputparams_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#inputparams_header_buttons > button.save_button').css("color", "grey");
 $('#inputparams_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveInputParamsData(tablename);
}

function toggleInputParamsDelete(e)
{
 if (e.checked)
  $('#inputparams_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#inputparams_header_buttons > button.delete_button').css("color", "grey");
}

/** Switch * */
function AddSwitchArgsRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
  EditSwitchArgsRow(tablename);

 var key = "";
 var val = "";
 var present = "";
 var missing = "";
 var swmode = "Switch";
 var outpos = 0;
 var inpos = 0;
 var required = "";
 var padded = "";
 var name = "";
 var typedropdown = '<option value="entry">entry</option><option value="checkbox">checkbox</option>';

 switchargs_rowcnt++;

 var td = '<tr id="switchargs_row_' + switchargs_rowcnt + '">';
 td += '<td><input type="checkbox" id="switchargs_edit_cb_' + switchargs_rowcnt + '" onchange="toggleSwitchArgsDelete(this);" />&nbsp;</td>';
 td += '<td><input type="text" id="switchargs_name_' + switchargs_rowcnt + '" value="' + name + '" style="width:100%;"></td>';
 td += '<td><input type="hidden" id="switchargs_key_' + switchargs_rowcnt + '" value="arg' + switchargs_rowcnt + '" style="width:100%;"></td>';
 td += '<td><input type="hidden" id="switchargs_swmode_' + switchargs_rowcnt + '" value="' + swmode + '" style="width:100%"></td>';
 td += '<td><input type="hidden" id="switchargs_oldkey_' + switchargs_rowcnt + '" value="' + key + '" style="width:100%"></td>';

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelSwitchArgsRow(tablename)
{
 if ($('#switchargs_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#switchargs_header_buttons > button.save_button').css("color", "grey");
 $('#switchargs_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function EditSwitchArgsRow(tablename)
{
 $('#switchargs_header_buttons > button.save_button').css("color", "#3367d6");
 $('#switchargs_header_buttons > button.cancel_button').css("color", "#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(switchargs_tdedit);

 $("#" + tablename + "-edit").show();
}

function DeleteSwitchArgsRow(tablename)
{
 if ($('#switchargs_header_buttons > button.delete_button').css("color") == "grey")
  return;

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#switchargs_edit_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   DeleteSwitchArgsData(rowid);
  }
 });

 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb = $("#switchargs_cb_" + rowid);

  if (cb.is(":checked") == true)
  {
   DeleteSwitchArgsData(rowid);
  }
 });

 LoadArgsData("inputparams", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveobjkind, summSaveaddParams);
}

function SaveSwitchArgsRow(tablename)
{
 if ($('#switchargs_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#switchargs_header_buttons > button.save_button').css("color", "grey");
 $('#switchargs_header_buttons > button.cancel_button').css("color", "grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveSwitchArgsData(tablename);
}

function toggleSwitchArgsDelete(e)
{
 if (e.checked)
  $('#switchargs_header_buttons > button.delete_button').css("color", "#3367d6");
 else
  $('#switchargs_header_buttons > button.delete_button').css("color", "grey");
}
