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

function avClickELement(id)
{
	console.log("avClickElement("+id+")");
	SwitchDisplay(id);
}

function getApplicationVersionBody(id,prefix,name,summ,suffix)
{
 var img = (prefix=="ap")?"css/images/applications_sm.png":"css/images/appversions_sm.png";
 img = img.replaceAll('-','_');
 return "<span style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" +name+ "</span>";
}

function getApplicationVersionMarkup(id,prefix,name,summ,xpos,ypos,suffix)
{
 return '<div class="drawcomponent applicationbox" name="'+prefix+id+'" id="window'+id+'" '
  + 'style="position: absolute; top: '+ypos+'px; left: '+xpos+'px ">'
  + getApplicationVersionBody(id,prefix,name,summ,suffix) + '</div>';
}

function avNewApplicationVersion(node)
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

 var fanc = "f=nv&isRelease=N&a=" + objid;

 $.getJSON("UpdateAttrs", fanc + "&xpos="+xpos+"&ypos="+ypos, function(data) {
  $('#innerappver').append(getApplicationVersionMarkup(data.compid, data.prefix, data.compname, '', xpos, ypos, ''));

  $("#window"+data.compid).dblclick(function() {
    avClickElement(data.prefix+data.compid);
  });
  avPlumb.draggable(avPlumb.getSelector(".applicationbox"), {
   grid : [ 20, 20 ],
   containment: '#innerappver',
   cursor: "-webkit-grab",
   stop: avWindowMoved
  });

  avPlumb.addEndpoint("window"+data.compid, targetEndpoint, {
   anchor : "TopCenter",
   uuid : "window"+data.compid+"in"
  });
  avPlumb.addEndpoint("window"+data.compid, sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "window"+data.compid+"out"
  });

  console.log("Binding context menu menu1 to window #window"+data.compid);
  try {
  $("#window"+data.compid).dmContextMenu(menuContextApplicationVersion, {theme:'xp'});
  } catch(e) { console.log(e); }
 // $("#window"+data.compid).resizable({ handles: "se" });
  $("#window"+data.compid).resize(function(){
     avPlumb.repaintEverything();
  });
 });
}

function avDeleteNode(node)
{
 if (DontSave)
  return;

 // alert("Detaching all connections");
 avPlumb.detachAllConnections(node.id);
 // alert("Removing all endpoints");
 avPlumb.removeAllEndpoints(node.id);
 // alert("Detaching the node");
 $("#"+node.id).detach();
 var n = node.id.substr(6);

 console.log(node.id);
 console.log(usedApplicationVersions);

 var i = usedApplicationVersions.indexOf(node.id);
 if(i != -1) {
  usedApplicationVersions.splice(i, 1);
 }


 $(currenttree).jstree("delete_node", "#av" + n);
  // Deleting component from application
  $.getJSON("UpdateAttrs","f=acd&isRelease=y&a=" + objid + "&c="+n,
   function(data){
   console.log("in success");
  });
}

function avWindowMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("in WindowMoved, ui.position.top="+ui.position.top);
 var windowid=event.target.id;
 var id = windowid.replace("window","");

  // Updating component position for application
  $.getJSON("UpdateAttrs","f=acvm&isRelease=y&a=" + objid + "&c="+id+"&xpos="+Math.round(ui.position.left)+"&ypos="+Math.round(ui.position.top),
   function(data){
    console.log("WindowMoved) in success");
  });
}


function avMenuDeleteConnector(conn)
{
 if (conn)
 {
  avPlumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}


function avDeleteConnector(conn)
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
  $.getJSON("UpdateAttrs","f=cadl&isRelease=y&a=" + objid + "&fn="+fn+"&tn="+tn,
  function(data){
   console.log("DeleteConnector) in success");
  });
 }
}

var applicationversionDropOptions = {
  greedy: true,
  drop : function(event, ui)
  {
   if (DontSave)
    return;

   oldid = $(this).attr("id").substr(6);
   newid = ui.draggable.attr("id");
   var newtitle = ui.draggable.text();
   var newsummary = ui.draggable.attr("summary");
   // Update the fragment window

   $("#window"+oldid).html(getApplicationVersionBody(newid, "ap", newtitle, newsummary, ''));

   // window id will be changed in StopDrag event
   $("#window"+oldid).removeClass("ui-state-highlight");
   $.getJSON("UpdateAttrs","f=ccv&isRelease=y&a=" + objid + "&c="+oldid+"&nc="+newid,
   function(data){
    console.log("in success");
   });
  }
 };


function avMouseOverLink(val)
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

function avMouseExitLink()
{
 OverLink=null;
}

function initApplicationVersion(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function avFindWindowWithNoOutput(winid)
{
 var swd = new Object();
 var pos = $("#window"+winid).position();
 var left = Math.round(pos.left);
 var top = Math.round(pos.top);
 var minDistance = 0;
 swd.fuuid = "startwindowout";
 swd.id = 0;
 console.log("GetNearestWindow winid="+winid+" left="+left+" top="+top);
 $(".applicationbox").each(function() {
  var id = $(this).attr("id").substr(6);
  if (id != winid)
  {
   console.log("id = "+id);
   var p = avPlumb.getConnections({source: "window"+id},true);
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

function avStartDrag()
{
 console.log("avStartDrag");
 Dragging=true;
 oldid=0;
 newid=0;
}

function avStopDrag()
{
 console.log("avStopDrag");
 Dragging=false;
 // Get the old end points and connections
 var fromlist = [];
 var tolist = [];

 if (oldid>0 && newid>0) {
  // replacing existing component
  var p = avPlumb.getConnections({source: "startwindow"},true);
  for (var i=0;i<p.length;i++) {
   if (p[i].targetId.substr(6)==oldid) {
    console.log("FROM #startwindow");
    avPlumb.detach(p[i]);
    fromlist.push(0);
   }
  }
  $(".applicationbox").each(function() {
   var id = $(this).attr("id").substr(6);
   if (id != oldid)
   {
    console.log("looking at window id = "+id);
    var p = avPlumb.getConnections({source: "window"+id},true);
    for (var i=0;i<p.length;i++) {
     if (p[i].targetId.substr(6)==oldid) {
      console.log("FROM #window"+id);
      avPlumb.detach(p[i]);
      fromlist.push(id);
     }
    }
   }
  });

  var p = avPlumb.getConnections({source: "window"+oldid},true);
  for (var i=0;i<p.length;i++) {
   console.log("TO: #"+p[i].targetId);
   avPlumb.detach(p[i]);
   tolist.push(p[i].targetId.substr(6));
  }
  // Now remove the endpoints
  console.log("Removing endpoint window"+oldid+"in");
  avPlumb.deleteEndpoint("window"+oldid+"in");
  console.log("Removing endpoint window"+oldid+"out");
  avPlumb.deleteEndpoint("window"+oldid+"out");
  //
  // Change the window id
  //
  $("#window"+oldid).attr("id","window"+newid);
  //
  // Restore the endpoints on the new window id
  //
  avPlumb.addEndpoint("window"+newid, targetEndpoint, {
   anchor : "TopCenter",
   uuid : "window"+newid+"in"
  });
  avPlumb.addEndpoint("window"+newid, sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "window"+newid+"out"
  });
  // And restore the connections
  for (var i=0;i<fromlist.length;i++) {
   if (fromlist[i]==0) {
    avPlumb.connect({uuids:["startwindowout", "window"+newid+"in"], editable:true});
   } else {
    avPlumb.connect({uuids:["window"+fromlist[i]+"out", "window"+newid+"in"], editable:true});
   }
  }
  for (var i=0;i<tolist.length;i++) {
   avPlumb.connect({uuids:["window"+newid+"out", "window"+tolist[i]+"in"], editable:true});
  }
  $("#window"+oldid).css("cursor","default");
 }
}


function SetUpApplicationVersions()
{
 var AcceptList = $(".appsource, .appversource");
 // let the component "source" items be draggable

  $(".appsource, .appversource").draggable({
   //cancel : "a.ui-icon", // clicking an icon won't initiate dragging
   revert : "invalid", // when not dropped, the item will revert back to its initial position
   containment : "document",
   helper : "clone",
   cursor : "-webkit-grab",
   start : avStartDrag,
   stop : avStopDrag
  });

  $("#innerappver").droppable({
  accept : AcceptList,
  // activeClass : "ui-state-highlight",
  drop : function(event, ui)
  {
   if (DontSave)
    return;

   var tx = (event.pageX) % 20;
   var ty = (event.pageY) % 20;
   if (tx>10) tx=-(20-tx);
   if (ty>10) ty=-(20-ty);
   var xpos = (event.pageX-430); // - tx) - 60;
   // var ypos = (event.pageY - ty) - 20;
   console.log("compitem.scrolltop="+$("#compitem").scrollTop());
   var ypos = ((event.pageY) - 124); // - ty) - (ui.offset.top - ui.position.top )) + $("#compitem").scrollTop() - 90;

   var windowid = ui.draggable.attr("id");

   if ($.inArray("window" + windowid,usedApplicationVersions) >= 0)
   {
    alert(ui.draggable.text() + " is already defined to the Application Version");
    return;
   }

   usedApplicationVersions.push("window" + windowid);;
   $("#innerappver").append(getApplicationVersionMarkup(windowid, "ap", ui.draggable.text(), ui.draggable.attr("summary"), xpos, ypos, ''));
   $("#window"+windowid).dblclick(function() {
    avClickElement("ap"+ui.draggable.attr("id"));
   });
   $("#window"+windowid).droppable(applicationversionDropOptions);
   $("#window"+windowid).droppable("option","accept", $(".appsource, .appversource") );
   $("#window"+windowid).hover(function() {
    if (Dragging) $(this).addClass("ui-state-highlight");
    },
    function() {
     $(this).removeClass("ui-state-highlight");
    }
   );

   avPlumb.draggable(avPlumb.getSelector(".applicationbox"), {
    grid : [ 20, 20 ],
    containment: '#innerappver',
    cursor: "-webkit-grab",
    stop: avWindowMoved
   });
   try {
   $('#window'+windowid).dmContextMenu(menuContextApplicationVersion, {theme:'xp'});
   } catch(e) { console.log(e); }

   // apps have connectors between the components to indicate deployment order
   avPlumb.addEndpoint("window"+windowid, targetEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+windowid+"in"
   });
   avPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+windowid+"out"
   });

   $.getJSON("UpdateAttrs","f=acvm&isRelease=y&c="+windowid+"&a=" + objid + "&xpos="+xpos+"&ypos="+ypos,
       function(data){
    // Updated DB - safe to add link
    var swd = avFindWindowWithNoOutput(windowid);
    console.log("swd.fuuid="+swd.fuuid);
    if (avPlumb.getEndpoint(swd.fuuid) != null)
    {
     console.log("Connecting to "+swd.fuuid);
     // The connect call will fire the new connection binding which will update the DB
     var connection = avPlumb.connect({uuids:[swd.fuuid, "window"+windowid+"in"], editable:true});
    }
   });
  }
 });
}

function LoadApplicationVersions()
{
 CreateTree_Selection("#applicationversionlist","applications",false,"mainframe3");
 SetUpApplicationVersions();
}


function LoadApplicationVersionItems()
{
 var InitialLoad = true;

 avPlumb.importDefaults({
  // default drag options
  DragOptions : {
   cursor : '-webkit-grab',
   zIndex : 2000
  }
 });

 avPlumb.bind("contextmenu", function(component, originalEvent) {
        originalEvent.preventDefault();
        return false;
    });

 var menu2 = function(m,t){
  if (DontSave)
   return;

  console.log("in menu2 (contextmenu activated)");
  if (!OverLink)
  {
    return;
  }
  // Construct a menu for this connector based on its content
  var ma = [];
  ma.push({'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){avMenuDeleteConnector(LastLink);} }});
  return ma;
  };

 console.log("Binding default context menu");

 try {
 $("#innerappver").dmContextMenu(menu2, {theme:'xp'});
 } catch(e) { console.log(e); }


 avPlumb.bind("jsPlumbConnection", function(connInfo, originalEvent) {
  console.log("jsPlumbConnection fired - about to call init");
  initApplicationVersion(connInfo.connection);
  console.log("init called");

  connInfo.connection.bind("mouseenter",function(c) {avMouseOverLink(connInfo.connection);});
  connInfo.connection.bind("mouseexit",function(c) {avMouseExitLink();});

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
   $.getJSON("UpdateAttrs","f=cal&isRelease=y&a=" + objid + "&fn="+fn+"&tn="+tn,
   function(data){
    console.log("updateattrs) in success");
   });
  }

 });

 avPlumb.bind("connectionDetached", function(connInfo, originalEvent) {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  avDeleteConnector(connInfo.connection);
 });

 var MaxWinID=0;
 var s="";
 s="appid=" + objid + "&isRelease=y";


 $.getJSON('GetComponentLayout',s,function(data){
	 console.log("data is");
	 console.log(data);

  // Start Window
  yo = 140;
  var w = Math.floor($("#innerappver").width()/2)-63;

  $("#innerappver").append( "<div class=\"startcomponent\" id=\"startwindow\" " +
    "style=\"position: absolute; top: 10px; left: " + w + "px \">" +
    "<span style=\"position:relative; top:5px\"><img src=\"css/images/go_16x.png\" style=\"padding-right:5px\">Start</div>");

  avPlumb.addEndpoint("startwindow", sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "startwindowout"
  });

  //
  // insert the components
  //
  console.log("in insert block");
  for (a=0;a<data.NodeCount;a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);
   if (WindowID > MaxWinID) MaxWinID = WindowID;

   usedApplicationVersions.push("window" + WindowID);

   console.log("data.Nodes["+a+"]");
   console.log(data.Nodes[a]);
   console.log("prefix="+data.Nodes[a].prefix);

   $("#innerappver").append(getApplicationVersionMarkup(WindowID, data.Nodes[a].prefix, data.Nodes[a].name, data.Nodes[a].summary, data.Nodes[a].xpos, data.Nodes[a].ypos, data.Nodes[a].suffix));
   var x=data.Nodes[a].prefix+data.Nodes[a].nodeid;
   $("#window"+WindowID).dblclick(function() {
     // alert(data.Nodes[a].prefix + data.Nodes[a].nodeid);
	   avClickElement(x);
   });
   console.log("Setting #window"+WindowID+" to droppable");
   $("#window"+WindowID).droppable(applicationversionDropOptions);
   $("#window"+WindowID).droppable("option","accept", $(".appsource, .appversource") );
   $("#window"+WindowID).hover(function() {
    if (Dragging) $(this).addClass("ui-state-highlight");
    },
    function() {
     $(this).removeClass("ui-state-highlight");
    }
   );

   avPlumb.addEndpoint("window"+WindowID, targetEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+WindowID+"in"
   });
   avPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+WindowID+"out"
   });

   console.log("Binding context menu menu1 to window #window"+WindowID);
   try {
   $("#window"+WindowID).dmContextMenu(menuContextApplicationVersion, {theme:'xp'});
   } catch(e) { console.log(e); }
 //  $("#window"+WindowID).resizable({ handles: "se" });
   $("#window"+WindowID).resize(function(){
      avPlumb.repaintEverything();
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

   if (avPlumb.getEndpoint(srcconn) != null && avPlumb.getEndpoint(tgtconn) != null)
   {
    var connection = avPlumb.connect({uuids:[srcconn,tgtconn], detachable:true, editable:true});
   }
  }

  avPlumb.draggable(avPlumb.getSelector(".applicationbox"), {
   grid : [ 20, 20 ],
   containment: '#innerappver',
   cursor: "-webkit-grab",
   stop: avWindowMoved
  });

  InitialLoad = false;

  if (data.readOnly) {
	  $("#innerappver").block({ message: null });
  } else {
	  $("#innerappver").unblock();
  }

 }).fail(function(fdata){
  console.log("Something has gone horribly wrong "+fdata);
  $.each(fdata,function(key,val){
   console.log("fdata) key="+key+" val="+val);
  });
  });
 $("#innerappver").resizable({ handles: "s" });




}

$(document).mousemove(function(event) {
    cCurrentMousePos.x = event.pageX;
    cCurrentMousePos.y = event.pageY;
});

function avStartMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("Start Moved ypos="+Math.round(ui.offset.left));
 $.getJSON("UpdateAttrs","f=cswm&isRelease=y&a=" + objid + "&xpos="+Math.round(ui.offset.left),
 function(data){
  console.log("WindowMoved) in success");
 });
}


function LoadApplicationVersionsData()
{
 DontSave = 1;
 usedApplicationVersions = new Array();

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


 console.log("LoadApplicationVersions");

 if ($("#applicationversionlist").hasClass("ui-accordion"))
 {
   $("#applicationversionlist").accordion('destroy');
   $("#applicationversionlist").html("");
   $("#applicationversionlist").removeClass();
 }

 $("#tabs-PackageApplications-data").show();
 $("#innerappver").show();
 $("#tabs-PackageApplications-data > div > div.accordionpanel").show();

 LoadApplicationVersions();
 console.log("LoadApplicationVersionItems");
 LoadApplicationVersionItems();
 DontSave = 0;
}
