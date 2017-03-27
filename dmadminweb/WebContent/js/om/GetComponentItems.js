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
function getComponentItemsBody(id, name, summ, suffix)
{
 var img = "css/images/component_ver_16x.png";
 img = img.replaceAll('-', '_');
 return "<span style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" + name + "</span>";
}

function getComponentItemsMarkup(id, name, summ, xpos, ypos, suffix)
{
/* if (xpos > 480)
 {
  xpos = 480 - xpos;

  if (xpos < 0)
   xpos = xpos * -1;
 }

 if (ypos < 50)
  ypos += 50;
*/
 return '<div class="drawcomponent componentitembox" id="window' + id + '" ' + 'style="position: absolute; top: ' + ypos + 'px; left: ' + xpos + 'px ">'
   + getComponentItemsBody(id, name, summ, suffix) + '</div>';
}

function SaveComponentItemsDetails(id)
{
 if (DontSave)
  return;

 var url = "f=csv&c=" + id;
 parent.$("#homeatts :input").each(function()
 {
  url = url + "&" + $(this).attr("id") + "=" + $(this).val();
 });
 $.getJSON('UpdateAttrs', url, function(data)
 {
  // Update the fragment window
  console.log("Updating the window for #window" + id);
  $(".componentitembox").each(function(index)
  {
   console.log(index + ": " + $(this).attr("id"));
  });
  $("#window" + id).html(getComponentItemsBody(id, data.title, data.summary, ''));
 });
}

function ciClickElement(id)
{
 summSaveItemobjid = id;
 console.log("GetSummaryData?objtype=14&id=" + id);
 $.ajax(
 {
  url : "GetSummaryData?objtype=14&id=" + id,
  dataType : 'json',
  type : 'GET',
  success : function(res)
  {
   console.log(res);
   saveRes = res;
   var title = "";
   tdedit = "<form id=\"summform\"><table id=\"summtab\" class=\"summtable\">";

   for (var r = 0; r < res.data.length; r++)
   {
    var row = res.data[r];
    var field = row[0];
    var callback = row[1];
    var rtype = row[2];
    var label = row[3];
    var val = row[4];
    var isuser = true;
    var oldval = "";

    if (label == "Summary")
    {
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><input name=\"summary_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"summary_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"summary_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"summary_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
    }
    else if (label == "Target Directory")
    {
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><input name=\"target_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"target_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"target_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"target_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
    }
    else if (label == "Name")
    {
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><input name=\"name_val\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"name_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
     title = val;
    }
    else if (label == "Roll Forward")
    {
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"rollup_val\">";
     if (val == "OFF")
     {
      val = "Off";
      tdedit += "<option value=\"OFF\" selected>Off</option>";
      tdedit += "<option value=\"ON\">On</option>";
      tdedit += "<option value=\"ALL\">All</option>";
     }
     else if (val == "ON")
     {
      val = "On";
      tdedit += "<option value=\"OFF\">Off</option>";
      tdedit += "<option value=\"ON\" selected>On</option>";
      tdedit += "<option value=\"ALL\">All</option>";
     }
     else
     {
      val = "All";
      tdedit += "<option value=\"OFF\">Off</option>";
      tdedit += "<option value=\"ON\">On</option>";
      tdedit += "<option value=\"ALL\" selected>All</option>";
     }
     tdedit += "</td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollup_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollup_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollup_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
    }
    else if (label == "Rollback")
    {
     tdedit += "<tr>";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"rollback_val\">";
     if (val == "OFF")
     {
      val = "Off";
      tdedit += "<option value=\"OFF\" selected>Off</option>";
      tdedit += "<option value=\"ON\">On</option>";
      tdedit += "<option value=\"ALL\">All</option>";
     }
     else if (val == "ON")
     {
      val = "On";
      tdedit += "<option value=\"OFF\">Off</option>";
      tdedit += "<option value=\"ON\" selected>On</option>";
      tdedit += "<option value=\"ALL\">All</option>";
     }
     else
     {
      val = "All";
      tdedit += "<option value=\"OFF\">Off</option>";
      tdedit += "<option value=\"ON\">On</option>";
      tdedit += "<option value=\"ALL\" selected>All</option>";
     }
     tdedit += "</td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollback_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollback_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"rollback_oldval\" value=\"" + val + "\"/></td>";
     tdedit += "</tr>";
    }
    else if (label == "Repository")
    {
     tdedit += "<tr id=\"repository_row\">";
     tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
     tdedit += "<td><select name=\"repository_val\" id=\"repository_val\" onChange=\"ToggleRepoProps(" + id + ",'repository_row',false,true)\" /></td>";
     save_repository_val = "re" + val;
     current_repository_val = save_repository_val;
     tdedit += "<td><input type=\"hidden\" name=\"repository_field\" value=\"" + field + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
     tdedit += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"re" + val + "\"/></td>";
     tdedit += "</tr>";
    }
   }

   tdedit += "</tr>";
   tdedit += "</table></form>";

   var pwd = parent.$("#modal");
   pwd.empty();
   pwd.html(tdedit);

   console.log("GetComponentItemSourceData?ciid=" + id + "&reason=repos");
   
   $.ajax(
   {
    url : "GetComponentItemSourceData?ciid=" + id + "&reason=repos",
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     console.log(res);

     var pwd = parent.$("#modal");
     var myform = pwd.find("#summform");
     var repo = myform.find(":input[name=\"repository_val\"]");

     console.log("res.length="+res.length);
     for (n = 0; n < res.length; n++)
     {
    	 console.log("n="+n+" name="+res[n].name);
      if (save_repository_val == "re" + res[n].id)
       repo.append('<option id="repository' + n + '" selected value=\"re' + res[n].id + "\">" + res[n].name + '</option>');
      else
       repo.append('<option id="repository' + n + '" value=\"re' + res[n].id + "\">" + res[n].name + '</option>');
     }
    }
   });

   var buttons = [
   {
    text : "Ok",
    click : function()
    {
     SaveSummaryItemData(parent.$("#modal"), summSavetablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveItemobjid, summSaveaddParams);
    }
   },
   {
    text : "Cancel",
    click : function()
    {
     parent.$("#modal").dialog("close");
     if (summSaveobjid < 0)
      $("#panel_container.right").hide();
    }
   } ];

   pwd.dialog(
   {
    modal : true,
    open : function(event, ui)
    {
     ToggleRepoProps(id,'repository_row',false,true);
    }
   });

   pwd.dialog("option", "title", "Edit " + title);
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "auto");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog('open');
  }
 });
}

function ciOpenDetails(x)
{
 ciClickElement(x.id.substr(6));
}

function ciNewComponentItems(node)
{
 if (DontSave)
  return;

 var xpos = cCurrentMousePos.x - 480;
 var ypos = cCurrentMousePos.y - 120;

 $.getJSON("UpdateAttrs", "f=inv&c=" + objid + "&xpos=" + xpos + "&ypos=" + ypos, function(data)
 {
  $('#inneritem').append(getComponentItemsMarkup(data.itemid, data.itemname, '', xpos, ypos, ''));

  $("#window" + data.itemid).dblclick(function()
  {
   ciDrillDown($(this));
  });

  cisplumb.draggable(cisplumb.getSelector(".componentitembox"),
  {
   grid : [ 20, 20 ],
   containment : '#inneritem',
   cursor : "-webkit-grab",
   stop : ciWindowMoved
  });

  cisplumb.addEndpoint("window" + data.itemid, targetEndpoint,
  {
   anchor : "TopCenter",
   uuid : "window" + data.itemid + "in"
  });
  cisplumb.addEndpoint("window" + data.itemid, sourceEndpoint,
  {
   anchor : "BottomCenter",
   uuid : "window" + data.itemid + "out"
  });

  console.log("Binding context menu menu1 to window #window" + data.itemid);
  if (isRelease == "Y")
   $("#window" + data.itemid).dmContextMenu(menuComponentItems,
   {
    theme : 'xp'
   });
  else
   $("#window" + data.itemid).dmContextMenu(menuComponentItems,
   {
    theme : 'xp'
   });

  // $("#window"+data.itemid).resizable({ handles: "se" });
  $("#window" + data.itemid).resize(function()
  {
   cisplumb.repaintEverything();
  });
  // Need this configurable off user profile

  var swd = ciFindWindowWithNoOutput(data.itemid);
  console.log("swd.fuuid=" + swd.fuuid);
  if (cisplumb.getEndpoint(swd.fuuid) != null)
  {
   console.log("Connecting to " + swd.fuuid);
   // The connect call will fire the new connection binding which will update
   // the DB
   var connection = cisplumb.connect(
   {
    uuids : [ swd.fuuid, "window" + data.itemid + "in" ],
    editable : true
   });
  }

  ciClickElement(data.itemid);
  Refresh(currenttree);
 });
}

function ciDeleteNode(node)
{
 console.log("ciDelete");

 if (DontSave)
  return;

 // alert("Detaching all connections");
 cisplumb.detachAllConnections(node.id);
 // alert("Removing all endpoints");
 cisplumb.removeAllEndpoints(node.id);
 // alert("Detaching the node");
 $("#" + node.id).detach();
 var n = node.id.substr(6);

 $(currenttree).jstree("delete_node", "#ci" + n);
 $.getJSON("UpdateAttrs", "f=idv&c=" + objid + "&ci=" + n, function(data)
 {
  console.log("in success");
 });
}

function ciWindowMoved(event, ui)
{
 if (DontSave)
  return;

 console.log("in WindowMoved, ui.position.top=" + ui.position.top);
 var windowid = event.target.id;
 var id = windowid.replace("window", "");

 $.getJSON("UpdateAttrs", "f=ivm&c=" + objid + "&ci=" + id + "&xpos=" + Math.round(ui.position.left) + "&ypos=" + Math.round(ui.position.top), function(data)
 {
  console.log("WindowMoved) in success");
 });
}

function ciMenuDeleteConnector(conn)
{
 if (conn)
 {
  cisplumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}

function ciFindWindowWithNoOutput(winid)
{
 var swd = new Object();
 var pos = $("#window" + winid).position();
 var left = +Math.round(pos.left);
 var top = Math.round(pos.top);
 var minDistance = 0;
 swd.fuuid = "startwindowout";
 swd.id = 0;
 console.log("GetNearestWindow winid=" + winid + " left=" + left + " top=" + top);
 $(".componentitembox").each(function()
 {
  var id = $(this).attr("id");

  if (typeof id != "undefined")
  {

   id = id.substr(6);
   if (id != winid)
   {
    console.log("id = " + id);
    var p = cisplumb.getConnections(
    {
     source : "window" + id
    }, true);
    console.log("p.length=" + p.length);
    if (p.length == 0)
    {
     // Found a component with no output connections - return that
     console.log("returning window" + id + "out");
     swd.fuuid = "window" + id + "out";
     swd.id = id;
    }
   }
  }
 });

 return swd;
}

function ciDeleteConnector(conn)
{
 if (DontSave)
  return;

 if (conn)
 {
  //
  // Update the DB
  //
  var fn = conn.sourceId.replace("window", "");
  var tn = conn.targetId.replace("window", "");
  if (fn.indexOf("start") > -1)
   fn = 0;
  if (tn.indexOf("start") > -1)
   tn = 0;
  $.getJSON("UpdateAttrs", "f=idd&c=" + objid + "&fn=" + fn + "&tn=" + tn, function(data)
  {
   console.log("DeleteConnector) in success");
  });
 }
}

function ciMouseOverLink(val)
{
 // if (EditMode=='Y')
 // {
 console.log("MouseOverLink id=" + val.id);
 console.log("MouseOverLink from id =" + val.endpoints[0].getUuid());
 console.log("MouseOverLink to id =" + val.endpoints[1].getUuid());
 OverLink = val;
 LastLink = OverLink;
 // }
}

function ciMouseExitLink()
{
 OverLink = null;
}

function initComponentItems(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function LoadComponentItemsData()
{
 DontSave = 1;

 cPlumb.reset();
 aPlumb.reset();   
 avPlumb.reset();
 cvPlumb.reset();
 cisplumb.reset();
 wfPlumb.reset();  
 
 $("#innerversions").html("");
 $("#innercomp").html("");
 $("#innerappver").html("");
 $("#inneritem").html("");
 $("innerworkflow").html("");
 $("#innercompversions").html("");

 var InitialLoad = true;

 cisplumb.importDefaults(
 {
  // default drag options
  DragOptions :
  {
   cursor : '-webkit-grab',
   zIndex : 2000
  }
 });

 cisplumb.bind("contextmenu", function(component, originalEvent)
 {
  originalEvent.preventDefault();
  return false;
 });

 var menu2 = function(m, t)
 {
	 if (readonly) return;
  if (DontSave)
   return;

  console.log("in menu2 (contextmenu activated)");
  if (!OverLink)
  {
   if (isRelease == "Y")
    return mainmenuComponentItems;
   else
    return mainmenuComponentItems;
  }
  // Construct a menu for this connector based on its content
  var ma = [];
  ma.push(
  {
   'Delete this Connection' :
   {
    icon : "images/delete.png",
    onclick : function(menuItem, menu)
    {
     cMenuDeleteConnector(LastLink);
    }
   }
  });
  return ma;
 };

 console.log("Binding default context menu");

 try
 {
  $("#inneritem").dmContextMenu(menu2,
  {
   theme : 'xp'
  });
 }
 catch (e)
 {
  console.log(e);
 }

 cisplumb.bind("jsPlumbConnection", function(connInfo, originalEvent)
 {
  console.log("jsPlumbConnection fired - about to call init");
  initComponentItems(connInfo.connection);
  console.log("init called");

  connInfo.connection.bind("mouseenter", function(c)
  {
   ciMouseOverLink(connInfo.connection);
  });
  connInfo.connection.bind("mouseexit", function(c)
  {
   ciMouseExitLink();
  });

  var FromUUID = connInfo.connection.endpoints[0].getUuid();
  var ToUUID = connInfo.connection.endpoints[1].getUuid();
  var fn;
  var tn;
  if (FromUUID.indexOf("out") > 0)
  {
   fn = connInfo.connection.sourceId.replace("window", "");
   tn = connInfo.connection.targetId.replace("window", "");
  }
  else
  {
   tn = connInfo.connection.sourceId.replace("window", "");
   fn = connInfo.connection.targetId.replace("window", "");
  }

  console.log("fn=" + fn + " tn=" + tn);

  if (InitialLoad == false)
  {
   console.log("Updating DB with compid=" + objid + " fn=" + fn + " tn=" + tn);

   $.getJSON("UpdateAttrs", "f=iad&c=" + objid + "&fn=" + fn + "&tn=" + tn, function(data)
   {
    console.log("updateattrs) in success");
   });
  }

 });

 cisplumb.bind("connectionDetached", function(connInfo, originalEvent)
 {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  ciDeleteConnector(connInfo.connection);
 });

 $.getJSON('GetComponentItemLayout', "compid=" + objid, function(data)
 {
	 readonly = data.readOnly;
	 if (readonly) $("#inneritem").block({ message: null });
  // Start Window
  yo = 140;
  var w = Math.floor($("#inneritem").width() / 2) - 63;

  var MaxWinID = 0;
  //
  // insert the components
  //
  console.log("in insert block");
  for (a = 0; a < data.NodeCount; a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);
   if (WindowID > MaxWinID)
    MaxWinID = WindowID;

   $("#inneritem").append(getComponentItemsMarkup(WindowID, data.Nodes[a].name, data.Nodes[a].summary, data.Nodes[a].xpos, data.Nodes[a].ypos, data.Nodes[a].suffix));
   $("#window" + WindowID).dblclick(function()
   {
    ciDrillDown($(this));
   });

   cisplumb.addEndpoint("window" + WindowID, targetEndpoint,
   {
    anchor : "TopCenter",
    uuid : "window" + WindowID + "in"
   });
   cisplumb.addEndpoint("window" + WindowID, sourceEndpoint,
   {
    anchor : "BottomCenter",
    uuid : "window" + WindowID + "out"
   });

   console.log("Binding context menu menu1 to window #window" + WindowID);
   if (isRelease == "Y")
    $("#window" + WindowID).dmContextMenu(menuComponentItems,
    {
     theme : 'xp'
    });
   else
    $("#window" + WindowID).dmContextMenu(menuComponentItems,
    {
     theme : 'xp'
    });

   $("#window" + WindowID).resize(function()
   {
    cisplumb.repaintEverything();
   });
   console.log("done");
  }
  //
  // insert the links between them
  //
  console.log("LinkCount=" + data.LinkCount);
  for (a = 0; a < data.LinkCount; a++)
  {
   console.log("data.Links[" + a + "].nodefrom=" + data.Links[a].nodefrom);
   console.log("data.Links[" + a + "].nodeto=" + data.Links[a].nodeto);
   var srcconn;
   var srcconn = "window" + data.Links[a].nodefrom + "out";
   var tgtconn = "window" + data.Links[a].nodeto + "in";
   
   console.log("Connect srcconn="+srcconn+" tgtconn="+tgtconn);

   if (cisplumb.getEndpoint(srcconn) != null && cisplumb.getEndpoint(tgtconn) != null)
   {
    var connection = cisplumb.connect(
    {
     uuids : [ srcconn, tgtconn ],
     detachable : true,
     editable : true
    });
   }
  }
  

  cisplumb.draggable(cisplumb.getSelector(".componentitembox"),
  {
   grid : [ 20, 20 ],
   containment : '#inneritem',
   cursor : "-webkit-grab",
   stop : ciWindowMoved
  });

  InitialLoad = false;
  DontSave = 0;
 }).fail(function(fdata)
 {
  console.log("Something has gone horribly wrong " + fdata);
  $.each(fdata, function(key, val)
  {
   console.log("fdata) key=" + key + " val=" + val);
  });
 });
 $("#inneritem").resizable(
 {
  handles : "s"
 });
 
 DontSave = 0;
}

function ciStartMoved(event, ui)
{
 if (DontSave)
  return;

 console.log("Start Moved ypos=" + Math.round(ui.offset.left));
 $.getJSON("UpdateAttrs", "f=ivm&c=" + objid + "&ci=" + id + "&xpos=" + Math.round(ui.offset.left), function(data)
 {
  console.log("WindowMoved) in success");
 });
}

function SaveSummaryItemData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
{
 var savedata =
 {
  objtype : 14,
  id : objid
 };

 var sumret = GetSaveSummaryItemData(instance, savedata, '');

 namecbfn = "ChangeSummaryName";

 console.log(savedata);

 if (sumret)
 {
  $.ajax(
  {
   url : "UpdateSummaryData",
   dataType : "json",
   type : "POST",
   data : savedata,
   success : function(data)
   {
    console.log(data);
    if (data && data.saved)
    {
     var propret = GetSavePropertyData(instance, savedata, '');
     SavePropertyData(objid, propret);

     parent.$("#modal").dialog("close");
     LoadComponentItemsData();
    }
    else
    {
     if (data && data.error)
     {
      alert(data.error);
      return;
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
    alert(err);
    return;
   }
  });
 }
 else
 {
  var propret = GetSavePropertyData(instance, savedata, '');
  SavePropertyData(objid, propret);

  parent.$("#modal").dialog("close");
 }
 return;
}

function GetSavePropertyData(instance, data, prefix)
{
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view = {};

 for ( var i in viewArr)
 {
  if (viewArr[i].name.indexOf("prop_") >= 0) {
	  console.log("viewArr["+i+"].name="+viewArr[i].value);
	  view[viewArr[i].name] = viewArr[i].value;
  }
 }
 // Add any unchecked checkboxes
 console.log("prop_val_recursive="+view["prop_val_recursive"]);
 if (typeof view["prop_val_recursive"] == "undefined") {
	 console.log("adding prop_val_recursive");
	 view["prop_val_recursive"]="0";
 }

 console.log(view);

 return view;
}

function GetSaveSummaryItemData(instance, data, prefix)
{
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = instance.find("#summform");
 var viewArr = form.serializeArray();
 var view =
 {};

 for ( var i in viewArr)
 {
  view[viewArr[i].name] = viewArr[i].value;
 }

 console.log(view);

 if (view.name_val != view.name_oldval)
 {
  data.newname = view.name_val;
  data.nameCallbackReqd = true;
  console.log(prefix + 'change_' + view.name_field + ' = ' + view.name_val);
  data[prefix + 'change_' + view.name_field] = view.name_val;
  ret = true;
 }

 if (view.summary_val != view.summary_oldval)
 {
  console.log(prefix + 'change_' + view.summary_field + ' = ' + view.summary_val);
  data[prefix + 'change_' + view.summary_field] = view.summary_val;
  ret = true;
 }

 if (view.rollup_val.toLowerCase() != view.rollup_oldval.toLowerCase())
 {
  console.log(prefix + 'change_' + view.rollup_field + ' = ' + view.rollup_val);
  data[prefix + 'change_' + view.rollup_field] = view.rollup_val;
  ret = true;
 }

 if (view.rollback_val.toLowerCase() != view.rollback_oldval.toLowerCase())
 {
  console.log(prefix + 'change_' + view.rollback_field + ' = ' + view.rollback_val);
  data[prefix + 'change_' + view.rollback_field] = view.rollback_val;
  ret = true;
 }

 if (view.target_val != view.target_oldval)
 {
  console.log(prefix + 'change_' + view.target_field + ' = ' + view.target_val);
  data[prefix + 'change_' + view.target_field] = view.target_val;
  ret = true;
 }

 if (view.repository_val != view.repository_oldval)
 {
  console.log(prefix + 'change_' + view.repository_field + ' = ' + view.repository_val);
  data[prefix + 'change_' + view.repository_field] = view.repository_val;
  ret = true;
 }

 return ret;
}

function SavePropertyData(id, view)
{
	var dor = deleteoldrows;
	console.log("in SavePropertyData, deleteoldrows="+deleteoldrows);
 for ( var prop in view)
 {
  var name = "";
  
  if (prop.indexOf("prop_oldval") >= 0) continue;
  console.log("x) prop="+prop);
  
  if (prop.indexOf("prop_val") >= 0) name = prop.substring(9);

  var val = view[prop];
  console.log("x) view["+prop+"] ="+view[prop]);

  prop = prop.replace("_val", "_oldval");

  var oldval = view[prop];
  
  console.log("x) oldval="+oldval);

  var mask = "NNN";

  if (name.length > 0)
  {
   var key = "";
   var data =
   {
    objtype : 14,
    id : id
   };

   console.log("val="+val+" oldval="+oldval+" dor="+dor);
   key = "";
   if (oldval == "" && val != "")
    key = "prop_add_" + name;
   else if (val == "")
    key = "prop_delete_" + name;
   else if (val != oldval) 
	key = "prop_change_" + name;
   else if (val == oldval && dor)
	key = "prop_add_" + name;
   
   console.log("key="+key);
   
   if (key != "")
   {
    console.log(name + "=" + val + "," + oldval + "," + mask);
    data[key] = val;
    console.log(data);
    console.log("deleteoldrows="+deleteoldrows);
    $.ajax(
    {
     url : "GetComponentItem?ciid=" + id +"&oldrows=" + (deleteoldrows?"Y":"N"),
     type : 'POST',
     dataType : "json",
     data : data,
     async : false,
     success : function(data)
     {
     }
    });
    deleteoldrows = false;	// reset to false after first applied change
   }
  }
 }
 
}

function ToggleRepoProps(objid,t, init,selchanged)
{
	var mrp="<tr class=\"repoprops\"><td colspan=\"2\"><HR></td></tr><tr class=\"repoprops\"><td colspan=\"2\"><I>Modifiable Repository Properties:</I></td></tr>";
	var mrp2="<tr class=\"repoprops\"><td colspan=\"2\"><HR></td></tr>";
	console.log("objid="+objid+" t="+t+" init="+init+" selchanged="+selchanged);
 var field = "#repository_val";
 var trrow = "#" + t;
 var prefix = "";
 
 if (t.indexOf("rb_") == 0)
 {
  field = "#rb_repository_val";
  prefix = "rb_";
 } 
 
 if (t.indexOf("rf_") == 0)
 {
  field = "#rf_repository_val";
  prefix = "rf_";
 }  
 
 if ($(field).length == 0)
  return;
 
 current_repository_val = $(field).val();
 console.log("REPO=" + current_repository_val);
 
 if (selchanged)
  deleteoldrows = (current_repository_val != save_repository_val);
 else
  deleteoldrows = false;
 
 console.log("deleteoldrows="+deleteoldrows);

 
 var repoid = (current_repository_val!=null)?$(field).val().substring(2):0;
 tdedit = "";
 
 var pwd = parent.$("#modal");
 if (init)
 { 
  pwd.find("#summtab").find("tr.repoprops").remove();
  pwd.find("#summtab").find("tr.rf_repoprops").remove();
  pwd.find("#summtab").find("tr.rb_repoprops").remove();
  
  console.log("GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid);
  
  $.ajax(
    {
     url : "GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid,
     dataType : 'json',
     type : 'GET',
     async : false,
     success : function(res)
     {
      console.log(res);
      olditemvalues = [];
      
      saveRes = res.defs;
      
      tdedit += mrp;
      for (var p = 0; p < res.defs.length; p++)
      {
       var label = res.defs[p].name;
       var val = olditemvalues[label];
       var field = "";
       var callback = "";

       if (typeof val == "undefined") val = "";
       
       console.log("a) val="+val+" label="+label);
       
       tdedit += "<tr class=\"" + prefix + "repoprops\">";
       tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
       if (label == "recursive" || label == "UseSSL") {
    	   var checked = val==1?"checked":"";
    	   tdedit += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"checkbox\" value=\"1\" " + checked + "/></td>";   
       } else {
    	   tdedit += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
       }
       tdedit += "<td><input type=\"hidden\" name=\"" + prefix + "prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\"/></td>";
       tdedit += "</tr>";
      }
      tdedit+=mrp2;
      $(trrow).after(tdedit);
     }
    });
 }
 else
 { 
  pwd.find("#summtab").find("tr." + prefix + "repoprops").remove();
 
  if (!deleteoldrows && objid >= 0)
  { 
   $.ajax(
   {
    url : "GetComponentItemSourceData?ciid=" + objid + "&repid=" + repoid,
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     console.log(res);
     olditemvalues = [];
     

     for (var p = 0; p < res.data.length; p++)
     {
      var row = res.data[p];
      if (row[3] != "Repository") {
    	  console.log("Setting values["+row[3]+"] to "+row[4]);
    	  olditemvalues[row[3]] = row[4];
      }
     }

     console.log(olditemvalues);
     tdedit = "";
     tdedit += mrp;
     for (var p = 0; p < res.defs.length; p++)
     {
      var label = res.defs[p].name;
    
      var val = olditemvalues[label];
      var field = "";
      var callback = "";
      
      if (typeof val == "undefined") val = "";
      
      console.log("b) val="+val);
      
      tdedit += "<tr class=\"repoprops\">";
      tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
      if (label == "recursive" || label == "UseSSL") {
    	 var checked = val==1?"checked":"";
    	 tdedit += "<td><input name=\"prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"checkbox\" value=\"1\"" + checked + "/></td>";
      } else {
     	 tdedit += "<td><input name=\"prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
      }
      
      tdedit += "<td><input type=\"hidden\" name=\"prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\" /></td>";
      tdedit += "</tr>";
     }
    }
   });
  }
    if (tdedit == "")
    {
    	console.log("GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid);
    $.ajax(
      {
       url : "GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid,
       dataType : 'json',
       type : 'GET',
       async : false,
       success : function(res)
       {
        console.log(res);
        
        saveRes = res.defs;
        
        tdedit += mrp;
        
        for (var p = 0; p < res.defs.length; p++)
        {
         var label = res.defs[p].name;
         var val = olditemvalues[label];
         var field = "";
         var callback = "";

         if (typeof val == "undefined") val = "";
         console.log("3) label="+label+" val="+val);
         
         tdedit += "<tr class=\"" + prefix + "repoprops\">";
         tdedit += "<td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\">" + label + ":</td>";
         if (label == "recursive" || label == "UseSSL") {
        	 var checked = val==1?"checked":"";
        	 tdedit += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"checkbox\" value=\"1\" " + checked + "/></td>";
         } else {
        	 tdedit += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:24em' type=\"text\" value=\"" + val + "\"/></td>";
         }
         tdedit += "<td><input type=\"hidden\" name=\"" + prefix + "prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\"/></td>";
         tdedit += "</tr>";
        }
       }
      }); 
    }
    tdedit+=mrp2;
    $(trrow).after(tdedit);
 } 
}
