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

        function removeProp(obj, propToDelete) {
            for (var property in obj) {
                if (typeof obj[property] == "object") {
                    let objectToCheck = obj[property];
                    delete obj.property
                    let newJsonData = this.removeProp(obj[property], propToDelete);
                    obj[property] = newJsonData
                } else {
                    if (property === propToDelete) {
                        delete obj[property];
                    }
                }
            }
            return obj
        }

        function unflatten(arr) {
            var tree = [],
                mappedArr = {},
                arrElem,
                mappedElem;

            // First map the nodes of the array to an object -> create a hash table.
            for (var i = 0, len = arr.length; i < len; i++) {
                arrElem = arr[i];
                mappedArr[arrElem.name] = arrElem;
                mappedArr[arrElem.name]['children'] = [];
            }

            for (var name in mappedArr) {
                if (mappedArr.hasOwnProperty(name)) {
                    mappedElem = mappedArr[name];
                    // If the element is not at the root level, add it to its parent array of children.
                    if (mappedElem.parentid != "") {
                        mappedArr[mappedElem['parentid']]['children'].push(mappedElem);
                    }
                    // If the element is at the root level, add it to first level elements array.
                    else {
                        tree.push(mappedElem);
                    }
                }
            }
            tree = removeProp(tree, "parentid");
            return tree;
        }

 function DomNav()
 {
	var numberOfOptions = 0;

    if ($('#domlist').length) {
        numberOfOptions = $('#domlist option').length;
    }

	if (numberOfOptions == 0)
	{
    $("#domnav-panel").html("<h3 style=\"text-align:left;\">Select a Domain</h3><select size=\"10\" id=\"domlist\" style=\"width:100%;height:90%;box-sizing:border-box;overflow:auto;\"></select>");

    $('#domlist').change(function() {
        var selectedValue = $(this).val();
        var selectedText = $('#domlist option:selected').text();
        $("#header_icon > h1").html("Domain: " + selectedText);
        objid = selectedValue;
        LoadSummaryData("summ", objtypeAsInt, objtype, objid, "");
    });

    $("#domlist").append('<option id="domain' + 1 + '" selected value=\"' + 1+ "\">" + 'GLOBAL' + '</option>');

    $.ajax(
    {
     url : "GetFullDomains",
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {
      var domain = $("#domlist");
      var cnt = 0;

      for (n = 0; n < res.length; n++)
      {
       if (!res[n].name.startsWith(userdom))
        continue;

       if (save_fulldomain_val == res[n].name)
       {
        save_fulldomain_id = res[n].id;
        domain.append('<option id="domain' + n + '" selected value=\"' + save_fulldomain_id + "\">" + res[n].name + '</option>');

        cnt++;
       }
       else
        domain.append('<option id="domain' + n + '" value=\"' + res[n].id + "\">" + res[n].name + '</option>');
      }

	    $('#domlist option:first').prop('selected', true);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }
}

 function LoadTasks(tablename,objtypeAsInt,objtype, objid, sel)
 {

  $('#tasks_header_buttons > button.delete_button').css("color","grey");

  $.ajax({
   url : "DomainDetails?domainid=" + objid,
   dataType : 'json',
   type : 'GET',
   success : function(res) {
   var td = "";
   var rowcnt = 0;

   for (var i = 0; i < res.length; i++)
   {
    var row = res[i];
    var key = row.id;
    var val = row.name;

    rowcnt++;

    td += '<tr id="task_row_' + rowcnt + '"><td><input type="checkbox" id="task_cb_' + rowcnt + '" onchange="toggleTask(this);" />&nbsp;</td><td id="task_val_' + rowcnt + '">' + val + '</td><td id="task_key_' + rowcnt + '" style="display:none;">' + key + '</td></tr>';

    task_rowcnt = rowcnt;
   }
   $("#" + tablename + " > tbody").html(td);

   for (var i = 0; i < res.length; i++)
   {
    var row = i+1

    $("#task_row_" +  row + " > td:nth-child(2)").on('click', function (e) {
     var id = this.id
     id = id.replace("val","cb");

     if ($("#" + id).is(":checked"))
      $("#" + id).prop("checked", false).trigger("change");
     else
      $("#" + id).click();
    });
   }

   LoadTaskSummary(null);
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
 }

 function DeleteDomain(objtypeAsInt,objid)
 {
  $.ajax({
   url:"RemoveNode",
   data: {
              "rel" : "Domain",
              "id" : objid
   },
   dataType: "json",
   success: function (d) {
    console.log("RemoveNode returns:");
    console.log(d);
    if (d.success == false)
    {
	 alert("Delete failed.\n\n" + d.error);
	 return;
	}
    var parent = current_dom_data.parent;
    current_dom_data = parent;
    $('#domlist').empty();
    DomNav();
   }
  });
 }

 function toggleTask(e)
 {
  if (prevcb != null && prevcb != e.id)
   $("#" + prevcb).prop("checked", false);

  prevcb = e.id;

  if (e.checked)
  {
   $('#tasks_header_buttons > button.delete_button').css("color","#3367d6");
   LoadTaskSummary(e.id);
  }
  else
  {
   $('#tasks_header_buttons > button.delete_button').css("color","grey");
   LoadTaskSummary(null);
  }
 }

 function LoadTaskSummary(cb_id)
 {
  if (cb_id == null)
  {
   $("#task_summ > tbody").empty();
   $("#tasks_assigned_groups").empty();
   $("#tasks_available_groups").empty();
   task_tdedit = "";
   return;
  }

  var keyid = cb_id.replace("cb","key");
  var id = $("#" + keyid).text();
  var objtype = id.substring(0,2);
  var objid = id.substring(2);
  var objtypeAsInt = obj2Int[objtype][0];

  LoadTaskSummaryData("task_summ", objtypeAsInt, objtype, objid, "");
  LoadTaskAccess();
 }

 function LoadTaskAccess()
 {
  $("#tasks_assigned_groups").html("");
  $("#tasks_available_groups").html("");

  var tid = task_summSaveobjid;

  // Display Groups (ga, gna)
  console.log("Getting group data");
  $.ajax({
   dataType: "json",
   type: 'POST',
   url: "DomainDetails",
   data: "f=gg&domainid=" + summSaveobjid + "&tid="+tid,
   success: function(data) {
    console.log("in success, data="+JSON.stringify(data));
    $.each(data, function(key, val) {
     if (val.mode=="a") {
      $("#tasks_assigned_groups").append("<div id=\"g"+val.id+"\" class=\"icon_inline_Group adjacent_icon\" style=\"cursor: pointer;\">"+val.name+"</div>");
     }
     else {
      $("#tasks_available_groups").append("<div id=\"g"+val.id+"\" class=\"icon_inline_Group adjacent_icon\" style=\"cursor: pointer;\">"+val.name+"</div>");
     }
    });

     var DraggableOptions = {
       // cancel : "a.ui-icon", // clicking an icon won't initiate dragging
       revert : "invalid", // when not dropped, the item will revert back to its initial position
       containment : "document",
       helper : "clone",
       cursor : "move"
      };

     $(".icon_inline_Group").draggable(DraggableOptions);
    }
  });

  $("#sub_panel_bottomleft").droppable({
   drop : function(event, ui)
   {
    MoveDomGroupIn(ui.draggable);
   }
  });
  $("#sub_panel_bottommiddle").droppable({
   drop : function(event, ui)
   {
    MoveDomGroupOut(ui.draggable);
   }
  });
 }


 function MoveDomGroupIn($item)
 {
  console.log("adding group="+$item.attr("id")+" to task "+ task_summSaveobjid);
  $.ajax({
   async: true,
   url: "DomainDetails?f=ag&gid="+$item.attr("id").substr(1)+"&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    $item.fadeOut(function()
    {
     var $ga = $("#tasks_assigned_groups");
     $item.appendTo($ga).fadeIn(200);
    });
   }
  });
 }

 function MoveDomGroupOut($item)
 {
  console.log("removing group="+$item.attr("id")+" from task "+task_summSaveobjid);
  $.ajax({
   async: true,
   url: "DomainDetails?f=rg&gid="+$item.attr("id").substr(1)+"&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    $item.fadeOut(function()
    {
     var $gna = $("#tasks_available_groups");
     $item.appendTo($gna).fadeIn(200);
    });
   }
  });
 }

 function DeleteTaskRow(tablename)
 {
  $.ajax({
   async: true,
   url: "DomainDetails?f=dt&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    LoadTasks(tablename, objtypeAsInt, objtype, objid, "");
    LoadTaskSummary(null);
   }
  });
 }

 function AddTaskRow(tablename,tasktype)
 {
  $.ajax({
   async: true,
   url: "DomainDetails?f=at&domainid=" + objid + "&tasktype="+tasktype,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    addTaskMenu();
    LoadTasks(tablename,objtypeAsInt,objtype, objid, data.id);
   }
  });
 }

 function AddDomain(tablename)
 {
  $.ajax({
   url: "GetNewID?objtype=Domain&pid=" + objid + "&ptype=Domain&at=&name=undefined&treeid=domains_tree",
   type: 'POST',
   dataType: "json",
   success: function(data) {
     lookupDom = data.name;
     editdom = true;
     $('#domlist').empty();
     DomNav();
   }
  });
 }
