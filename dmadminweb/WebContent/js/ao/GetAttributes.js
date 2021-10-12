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

function EditAttrRow(tablename)
{
 $('#attrs_header_buttons > button.save_button').css("color","#3367d6");
 $('#attrs_header_buttons > button.cancel_button').css("color","#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(attrs_tdedit);
 $("#" + tablename + "-edit").show();
}

function AddAttrRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
   EditAttrRow(tablename);
 
 key = "";
 val = "";
 arrid = "";
 attr_rowcnt++;
 
 var td = '<tr id="attr_row_' + attr_rowcnt + '">';
 td += '<td><input type="checkbox" id="attr_edit_cb_' + attr_rowcnt  + '"  onchange="toggleAttrDelete(this);" />&nbsp;</td>';
 td += '<td><input type="text" id="attr_key_' + attr_rowcnt + '" value="' + key + '" style="width:100%;"></td><td><input type="text" id="attr_val_' + attr_rowcnt + '" value="' + val + '" style="width:100%"></td>';
 td += "<td><input type=\"hidden\" id=\"attr_oldkey_" + attr_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"attr_arrid_" + attr_rowcnt + "\" value=\"" + arrid + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"attr_oldval_" + attr_rowcnt + "\" value=\"" + val + "\"\></td></tr>";
 
 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelAttrRow(tablename)
{
 if ($('#attrs_header_buttons > button.cancel_button').css("color") == "grey")
  return;
 
 $('#attrs_header_buttons > button.save_button').css("color","grey");
 $('#attrs_header_buttons > button.cancel_button').css("color","grey");
 
 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 $("#" + tablename + "-edit > tbody").empty();
}

function SaveAttrRow(tablename)
{
 if ($('#attrs_header_buttons > button.save_button').css("color") == "grey")
  return;
 
 $('#attrs_header_buttons > button.save_button').css("color","grey");
 $('#attrs_header_buttons > button.cancel_button').css("color","grey");
 
 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveAttributesData(tablename);
}

function DeleteAttrRow(tablename)
{
 if ($('#attrs_header_buttons > button.delete_button').css("color") == "grey")
  return;
 
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;
  
  cb  = $("#attr_edit_cb_" + rowid);
  
  if (cb.is( ":checked" ) == true) 
  {
   key = $("#attr_oldkey_" + rowid).val();
   val = $("#attr_oldval_" + rowid).val();
 
   if (key != "")
     DeleteAttributesData(key,val, tablename);
  }
 });
 
 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;
    
    cb  = $("#attr_cb_" + rowid);
    
    if (cb.is( ":checked" ) == true) 
    {
     key = $("#attr_oldkey_" + rowid).val();
     val = $("#attr_oldval_" + rowid).val();
   
     if (key != "")
       DeleteAttributesData(key,val, tablename);
    }
   });
}

function toggleAttrDelete(e)
{
  if (e.checked)
   $('#attrs_header_buttons > button.delete_button').css("color","#3367d6"); 
  else
   $('#attrs_header_buttons > button.delete_button').css("color","grey"); 
}


function LoadAttributesData(tablename,objtypeAsInt, objtype, objid)
{
 var save_preaction_val = "";
 var save_postaction_val = "";
 var save_customaction_val = "";
 attrs_tdedit = "";
 attr_rowcnt = 0;

 $('#attrs_header_buttons > button.delete_button').css("color","grey"); 
 
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
    
    attrs_tdedit += '<tr id="attr_row_' + rowcnt + '">';
    attrs_tdedit += '<td><input type="checkbox" id="attr_edit_cb_' + rowcnt + '" onchange="toggleAttrDelete(this);" />&nbsp;</td>';
    attrs_tdedit += '<td><input type="text" id="attr_key_' + rowcnt + '" value="' + key + '" style="width:100%;"></td><td><input type="text" id="attr_val_' + rowcnt + '" value="' + val + '" style="width:100%"></td>';
    attrs_tdedit += "<td><input type=\"hidden\" id=\"attr_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
    attrs_tdedit += "<td><input type=\"hidden\" id=\"attr_arrid_" + rowcnt + "\" value=\"" + arrid + "\"\></td>";
    attrs_tdedit += "<td><input type=\"hidden\" id=\"attr_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
    td += '<tr><td><input type="checkbox" id="attr_cb_' + rowcnt + '" onchange="toggleAttrDelete(this);" />&nbsp;</td><td>' + key + '</td><td>' + val + '</td></tr>';
    attr_rowcnt = rowcnt;
   }
   
   $("#" + tablename + " > tbody").html(td);
   $("#" + tablename + "-edit > tbody").html(attrs_tdedit);
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
}

function GetSaveAttributesData(rowid, data)
{
 var view = {};
 view.name_val = $("#attr_key_" + rowid).val();
 view.name_oldval = $("#attr_oldkey_" + rowid).val();
 view.value_val = $("#attr_val_" + rowid).val();
 view.value_oldval = $("#attr_oldval_" + rowid).val();
 view.arr_val = $("#attr_arrid_" + rowid).val();
 
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
 else if (view.name_val != view.name_oldval)
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
  else
  {
   delete data["objtype"];
   delete data["id"];
  }
}

function SaveAttributesData(tablename)
{

$("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
{
 rowid++;
 
  var savedata =
 {
  objtype : objtypeAsInt,
  id : objid
 };
 GetSaveAttributesData(rowid,savedata);
 console.log(savedata);

 if (jQuery.isEmptyObject(savedata))
  return;
 
 $.ajax(
 {
  url : "UpdateAttributesData?objtype=" + objtypeAsInt + "&id=" + objid,
  dataType : "json",
  type : "POST",
  async: false,
  data : savedata,
  success : function(data)
  {
   console.log(data);
   if (data && data.saved)
   {
    console.log("Saved data");
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
 });

 LoadAttributesData(tablename,objtypeAsInt, objtype, objid);
}

function DeleteAttributesData(key,value, tablename)
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
    LoadAttributesData(tablename,objtypeAsInt, objtype, objid);
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
