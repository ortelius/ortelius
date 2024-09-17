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

var Dragging = false;


function getComponentBody(id,prefix,name,summ,suffix)
{
 var img = "css/images/component" +suffix+"s_sm.png";
 img = img.replaceAll('-','_');
 return "<span id=\"window"+id+"t\" style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" +name+ "</span>";
}

function getComponentMarkup(id,prefix,name,summ,xpos,ypos,suffix)
{
 return '<div class="componentbox" name="'+prefix+id+'" id="window'+id+'" '
  + 'style="position: absolute; top: '+ypos+'px; left: '+xpos+'px ">'
  + getComponentBody(id,prefix,name,summ,suffix) + '</div>';
}

function SaveDetails(id)
{
 if (DontSave)
  return;

 var url="f=csv&c="+id;
 parent.$("#homeatts :input").each(function() {
  url=url+"&"+$(this).attr("id")+"="+$(this).val();
 });
 $.getJSON('UpdateAttrs',url,
     function(data){
   // Update the fragment window
   console.log("Updating the window for #window"+id);
   $( ".componentbox" ).each(function( index ) {
      console.log(index + ": "+$(this).attr("id"));
   });
   $("#window"+id).html(getComponentBody(id, data.prefix, data.title, data.summary, ''));
 });
}

function cClickElement(id)
{
	console.log("cClickElement("+id+")");
	SwitchDisplay(id);
}


function cNewComponent(node)
{
 if (DontSave)
  return;

 var tx = (cCurrentMousePos.x) % 20;
 var ty = (cCurrentMousePos.y) % 20;
 if (tx>10) tx=-(20-tx);
 if (ty>10) ty=-(20-ty);

 console.log("ScrollTop="+$("#compitem").scrollTop());
 var xpos = (cCurrentMousePos.x - tx) - 60;
 var ypos = ((cCurrentMousePos.y - ty) + $("#compitem").scrollTop()) - 90;

 var fanc = "f=anc&a=" + objid;

 $.getJSON("UpdateAttrs", fanc + "&xpos="+xpos+"&ypos="+ypos, function(data) {
  $('#innercomp').append(getComponentMarkup(data.compid, "co", data.compname, '', xpos, ypos, ''));

  $("#window"+data.compid).click(function() {
	  cClickElement("co"+data.compid);
  });
  cPlumb.draggable(cPlumb.getSelector(".componentbox"), {
   grid : [ 20, 20 ],
   containment: '#innercomp',
   cursor: "-webkit-grab",
   stop: cWindowMoved
  });

  cPlumb.addEndpoint("window"+data.compid, compEndpoint, {
   anchor : "TopCenter",
   uuid : "window"+data.compid+"in"
  });
  cPlumb.addEndpoint("window"+data.compid, compEndpoint, {
   anchor : "BottomCenter",
   uuid : "window"+data.compid+"out"
  });

  console.log("Binding context menu menu1 to window #window"+data.compid);
  try {
  $("#window"+data.compid).dmContextMenu(menu1, {theme:'xp'});
  } catch(e) { console.log(e); }
 // $("#window"+data.compid).resizable({ handles: "se" });
  $("#window"+data.compid).resize(function(){
     cPlumb.repaintEverything();
  });
  // Need this configurable off user profile
  cClickElement("co"+data.compid);
 });
}

function cGotoComponent(node)
{
	console.log("cGotoComponent");
	console.log("node.id="+node.id);
	SwitchDisplay($(node).attr("name"));
}

function cDeleteNode(node)
{
 console.log("cDelete");

 if (DontSave)
  return;

 cPlumb.detachAllConnections(node.id);
 cPlumb.removeAllEndpoints(node.id);
 $("#"+node.id).detach();
 var n = node.id.substr(6);
 console.log(node.id);
 console.log(usedComponents);

 var i = usedComponents.indexOf(node.id);
 if(i != -1) {
  usedComponents.splice(i, 1);
 }

 $(currenttree).jstree("delete_node", "#co" + n);
  // Deleting component from application
  $.getJSON("UpdateAttrs","f=acd&a=" + objid + "&c="+n,
   function(data){
   console.log("in success");
   console.log(currenttree);
   $(currenttree).jstree('select_node', "co" + objid);
  });
}

function cWindowMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("in WindowMoved, ui.position.top="+ui.position.top);
 var windowid=event.target.id;
 var id = windowid.replace("window","");

  // Updating component position for application
  $.getJSON("UpdateAttrs","f=acvm&a=" + objid + "&c="+id+"&xpos="+Math.round(ui.position.left)+"&ypos="+Math.round(ui.position.top),
   function(data){
    console.log("WindowMoved) in success");
  });
}


function cMenuDeleteConnector(conn)
{
 if (conn)
 {
  cPlumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}


function cDeleteConnector(conn)
{
 if (DontSave)
  return;

 if (conn)
 {
  //
  // Update the DB
  //
  var fn = conn.sourceId.replace("window","");
  var tn = conn.targetId.replace("window","");
  if (fn=="start") fn=0;
  if (tn=="start") tn=0;
  console.log("Deleting connector fn="+fn+" tn="+tn+" a=" + objid);
  $.getJSON("UpdateAttrs","f=cadl&a=" + objid + "&fn="+fn+"&tn="+tn,
  function(data){
   console.log("DeleteConnector) in success");
  });
 }
}

var componentDropOptions = {
  greedy: true,
  hoverClass: "ui-state-highlight",
  drop : function(event, ui)
  {
   if (DontSave)
    return;

   oldid = $(this).attr("id").substr(6);
   newid = ui.draggable.attr("id");
   var newtitle = ui.draggable.text();
   var newsummary = ui.draggable.attr("summary");
   // Update the fragment window
   // NOTE THIS WILL NEED TO SELECT CO or CV dependent on if it's a version or base.
   $("#window"+oldid).html(getComponentBody(newid, 'co', newtitle, newsummary, ''));

   // window id will be changed in StopDrag event
   $("#window"+oldid).removeClass("ui-state-highlight");
   $.getJSON("UpdateAttrs","f=ccv&a=" + objid + "&c="+oldid+"&nc="+newid,
   function(data){
    console.log("in success - changing inner text");
    $("#window"+newid).html();
   });
  }
 };


function cMouseOverLink(val)
{
 //if (EditMode=='Y')
 //{
  console.log("MouseOverLink id="+val.id);
  console.log("MouseOverLink from id ="+val.endpoints[0].getUuid());
  console.log("MouseOverLink to id ="+val.endpoints[1].getUuid());
  OverLink=val;
  LastLink=OverLink;
 //}
}

function cMouseExitLink()
{
 OverLink=null;
}

function init(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function FindWindowWithNoOutput(winid)
{
 var swd = new Object();
 var pos = $("#window"+winid).position();
 var left = + Math.round(pos.left);
 var top = Math.round(pos.top);
 var minDistance = 0;
 swd.fuuid = "startwindowout";
 swd.id = 0;
 console.log("getcomp) GetNearestWindow winid="+winid+" left="+left+" top="+top);
 $(".componentbox").each(function() {
  var id = $(this).attr("id").substr(6);
  if (id != winid)
  {
   console.log("id = "+id);
   var p = cPlumb.getConnections({source: "window"+id},true);
   console.log("p.length="+p.length);
   if (p.length == 0) {
    // Found a component with no output connections - return that
    console.log("returning window"+id+"out");
    swd.fuuid = "window"+id+"out";
    swd.id=id;
   }
  }
 });

 return swd;
}

function LoadComponents()
{
	CreateTree_Selection("#componentlist","components",false,"mainframe3");
}


function LoadItems()
{
 var InitialLoad = true;

 cPlumb.importDefaults({
  // default drag options
  DragOptions : {
   cursor : '-webkit-grab',
   zIndex : 2000
  }
 });

 cPlumb.bind("contextmenu", function(component, originalEvent) {
        originalEvent.preventDefault();
        return false;
    });

 var menu2 = function(m,t){
  if (DontSave)
   return;

  console.log("in menu2 (contextmenu activated)");
  if (!OverLink)
  {
    return null; //mainmenu;
  }
  // Construct a menu for this connector based on its content
  var ma = [];
  ma.push({'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){cMenuDeleteConnector(LastLink);} }});
  return ma;
  };

 console.log("Binding default context menu");

 try {
 $("#innercomp").dmContextMenu(menu2, {theme:'xp'});
 } catch(e) { console.log(e); }


 cPlumb.bind("jsPlumbConnection", function(connInfo, originalEvent) {
  console.log("jsPlumbConnection fired - about to call init");
  init(connInfo.connection);
  console.log("init called");

  connInfo.connection.bind("mouseenter",function(c) {cMouseOverLink(connInfo.connection);});
  connInfo.connection.bind("mouseexit",function(c) {cMouseExitLink();});

  var FromUUID = connInfo.connection.endpoints[0].getUuid();
  var ToUUID = connInfo.connection.endpoints[1].getUuid();
  var fn;
  var tn;
  if (FromUUID.indexOf("out")>0) {
   fn = connInfo.connection.sourceId.replace("window","");
   tn = connInfo.connection.targetId.replace("window","");
  } else {
   tn = connInfo.connection.sourceId.replace("window","");
   fn = connInfo.connection.targetId.replace("window","");
  }

  if (fn == "start") fn=0;
  if (tn == "start") tn=0;

  console.log("fn="+fn+" tn="+tn);

  if (InitialLoad == false)
  {
   console.log("Updating DB with appid=" + objid + " fn="+fn+" tn="+tn);
   $.getJSON("UpdateAttrs","f=cal&a=" + objid + "&fn="+fn+"&tn="+tn,
   function(data){
    console.log("updateattrs) in success");
   });
  }

 });

 cPlumb.bind("connectionDetached", function(connInfo, originalEvent) {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  cDeleteConnector(connInfo.connection);
 });


 var MaxWinID=0;
 var s="";
 s="appid=" + objid;


 $.getJSON('GetComponentLayout',s,function(data){
  // Start Window
  yo = 140;
  var w = Math.floor($("#innercomp").width()/2)-63;

  $("#innercomp").append( "<div class=\"startcomponent\" id=\"startwindow\" " +
    "style=\"position: absolute; top: 10px; left: " + w + "px \">" +
    "<span style=\"position:relative; top:5px\"><img src=\"css/images/go_16x.png\" style=\"padding-right:5px\">Start</div>");

  waitForElement("#innerworkflow",function(){
  cPlumb.addEndpoint("startwindow", compEndpoint, {
   anchor : "BottomCenter",
   uuid : "startwindowout"
  });

  //
  // insert the components
  //
  console.log("in insert block");
  console.log(data);
  for (a=0;a<data.NodeCount;a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);
   if (WindowID > MaxWinID) MaxWinID = WindowID;

//   usedComponents.push("window" + WindowID);

   console.log(data.Nodes[a]);
   console.log("windowid="+WindowID+" xpos="+data.Nodes[a].xpos+" ypos="+data.Nodes[a].ypos);
   $("#innercomp").append(getComponentMarkup(WindowID, data.Nodes[a].prefix, data.Nodes[a].name, data.Nodes[a].summary, data.Nodes[a].xpos, data.Nodes[a].ypos, data.Nodes[a].suffix));
   console.log("Setting #window"+WindowID+" to droppable with highlight");
   $("#window"+WindowID).droppable(componentDropOptions);
   $("#window"+WindowID).droppable("option","accept", $(".jstree-leaf") );
   $("#window"+WindowID).hover(function() {
    if (Dragging) $(this).addClass("ui-state-highlight");
    },
    function() {
     $(this).removeClass("ui-state-highlight");
    }
   );

   cPlumb.addEndpoint("window"+WindowID, compEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+WindowID+"in"
   });
   cPlumb.addEndpoint("window"+WindowID, compEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+WindowID+"out"
   });

   console.log("Binding context menu menu1 to window #window"+WindowID);
   try {
   $("#window"+WindowID).dmContextMenu(menu1, {theme:'xp'});
   } catch(e) { console.log(e); }
   $("#window"+WindowID).resize(function(){
      cPlumb.repaintEverything();
   });
   console.log("done");
  }
  //
  // insert the links between them
  //
  console.log("LinkCount="+data.LinkCount);
  for (a=0;a<data.LinkCount;a++)
  {
   console.log("data.Links["+a+"].nodefrom="+data.Links[a].nodefrom);
   console.log("data.Links["+a+"].nodeto="+data.Links[a].nodeto);
   var srcconn;
   if (data.Links[a].nodefrom == 0) {
    srcconn = "startwindowout";
   } else {
    srcconn = "window" + data.Links[a].nodefrom + "out";
   }
   var tgtconn = "window" + data.Links[a].nodeto + "in";

   console.log("Connect srcconn="+srcconn+" tgtconn="+tgtconn);

   if (cPlumb.getEndpoint(srcconn) != null && cPlumb.getEndpoint(tgtconn) != null)
   {
    var connection = cPlumb.connect({uuids:[srcconn,tgtconn], detachable:true, editable:true});
   }
  }

  cPlumb.draggable(cPlumb.getSelector(".componentbox"), {
   grid : [ 20, 20 ],
   containment: '#innercomp',
   cursor: "-webkit-grab",
   stop: cWindowMoved
  });

  InitialLoad = false;
  if (data.readOnly)
    $("#innercomp").block({ message: null });
  else
    $("#innercomp").unblock();
  });
 }).fail(function(fdata){
  console.log("Something has gone horribly wrong "+fdata);
  $.each(fdata,function(key,val){
   console.log("fdata) key="+key+" val="+val);
  });
  });
 $("#innercomp").resizable({ handles: "s" });
}

function StartMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("Start Moved ypos="+Math.round(ui.offset.left));
 $.getJSON("UpdateAttrs","f=cswm&a=" + objid + "&xpos="+Math.round(ui.offset.left),
 function(data){
  console.log("WindowMoved) in success");
 });
}


function LoadComponentsData()
{
 DontSave = 1;
 usedComponents = new Array();

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

 console.log("LoadComponents");


 if ($("#componentlist").hasClass("ui-accordion"))
 {
   $("#componentlist").accordion('destroy');
   $("#componentlist").html("");
   $("#componentlist").removeClass();
 }
 LoadComponents();
 console.log("LoadItems");
 LoadItems();
 DontSave = 0;
}
