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

function wfDrillDown(x)
{
	console.log("wfDrillDown objid="+objid+" ddwinid="+ddwinid+" windowid="+x.attr("id").substr(6));
	FragmentName = x.children("span").text();
	var image = x.children("span").children("img");
	var imageurl=(image)?image.attr("src"):"images/action.png";
	console.log("image src="+imageurl);
	var windowid = x.attr("id").substr(6);
	var odl="";
	var sep="";
	var selnode = $(currenttree).jstree('get_selected');
	console.log("selnode="+selnode.attr("id"));
	var parent = $.jstree._reference(currenttree)._get_parent(selnode);
	console.log("parentnode="+parent.attr("id"));
	while (parent != -1) {
		sep=(odl.length>0)?" ":"";
		odl=odl+sep+parent.attr("id");
		parent=$.jstree._reference(currenttree)._get_parent(parent);
	}
	console.log("odl="+odl);

	ddwinid = windowid;
	console.log("LoadWorkflowsData("+windowid+")");
	LoadWorkflowsData(windowid);
}

function Navigate(tree,node,windowid,odl)
{
	console.log("Navigate tree="+tree+" node="+node+" windowid="+windowid);
	if (currentmenu != "deploy_menu") EnableTabs("deploy_menu");
	if (oldtabmenu != "actions_tab")  SetActive("actions_tab");
	$(tree).jstree("deselect_all");
	ods = odl.split(" ");
    for (var i=ods.length;i--;i>=0) {
    	console.log("ods["+i+"]="+ods[i]);
    	$(tree).jstree("open_node","#"+ods[i]);
    }
    console.log("highlighting #"+node);
	$(tree).jstree("select_node","#"+node);
	LoadWorkflowsData(windowid);
}

function wfViewAction(x)
{
 var wid = x.id.substr(6);
 // alert("View Action "+x.id.substr(6)+" otid="+Fragments[wid].otid);
 SwitchDisplay(Fragments[wid].otid);
}

function wfOpenDetails(x,nf,useDefault)
{
 console.log("nf="+nf);
 var id =x.id.substr(6);
 var pwd = parent.$("#modal");
 pwd.empty();

 var buttons1 = [
 {
  text : "Ok",
  click : function()
  {
   if (SaveWFDetails(id)) pwd.dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   pwd.dialog("close");
  }
 } ];

 var buttons2 = [
 {
  text : "Ok",
  click : function()
  {
   if (SaveWFDetails(id)) pwd.dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   pwd.dialog("close");
   console.log("Deleting fragment");
   wfDeleteNode(x);
  }
 } ];

 var buttons3 = [
 {
  text : "Cancel",
  click : function()
  {
	  pwd.dialog("close");
  }
  }];

 pwd.dialog(
   {
    modal : true,
    open : function(event, ui) {
    	pwd.css('overflow', 'hidden');
    }
   });

   var pwid = parentwindowid.length>0?parentwindowid:0;
   console.log("getfragatt?typeid=" + Fragments[id].type + "&editmode=Y&windowid=" + id + "&actionid=" + objid + "&useDefault=" + useDefault+"&pw="+pwid);
   pwd.load("getfragatt?typeid=" + Fragments[id].type + "&editmode=Y&windowid=" + id + "&actionid=" + objid + "&useDefault=" + useDefault+"&pw="+pwid,function() {
	   var th = $("#atttable").height();
	   var dh = (th>180)?180:th;
	   $("#attdiv").css("height",dh+"px").css("overflow","auto");
	   var wh = $("#fragheader").height() + 165;
	   if ($("#reqdiv").text().length > 0) wh=wh+30;
	   var mdh = dh + wh;
	   pwd.dialog("option","height",mdh);
   });

   pwd.dialog("option", "title", readonly?"Details (Read Only)":"Details");
   pwd.dialog("option", "height", "440");
   pwd.dialog("option", "width", "auto");
   pwd.dialog("option", "buttons", readonly?buttons3:nf?buttons2:buttons1);
   pwd.dialog("open");
}



function wfMenuDeleteConnector(conn)
{
 if (conn)
 {
  wfPlumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}


function wfMenuShowScript()
{
	EditProcButton(objid,true)
}

function wfExportAsProc()
{
	$.fileDownload("GetActionLayout?f=convert&actionid="+objid)
	.done(function() {
		alert("done!");
	})
	.fail(function() {
		alert("failed!");
	});
}


function wfDeleteConnector(conn)
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
  if (fn=="start") fn="0";
  $.getJSON("UpdateAttrs","f=df&a=" + objid + "&fn="+fn+"&tn="+tn,
  function(data){
  });
 }
}

function wfDeleteNode(node)
{
 console.log("wfDelete: node.id="+node.id+" parentwindowid="+parentwindowid);
 DontSave=false;
 //
 // First, find any window that connects to this one
 // and decrement its output count
 //
 var cl = jsPlumb.getConnections({target:node.id});
 if (cl.length > 0)
 {
  if (cl[0].sourceId != "startwindow")
  {
   var sid = cl[0].sourceId.substr(6);
   if (Fragments[sid]) Fragments[sid].outputs--;
  }
 }
 var n = node.id.substr(6);
 $.getJSON("UpdateAttrs","f=dn&a=" + objid + "&w="+n,
 function(data){
	 console.log("in success - detaching connections and endpoints");
	 jsPlumb.detachAllConnections(node.id);
	 jsPlumb.removeAllEndpoints(node.id);
	 $("#"+node.id).detach();
	 //
	 // Remove the fragments record
	 //
	 Fragments[n]=null;
	 $(currenttree).jstree("delete_node", "#ac" + n);
 });


}


var workflowDropOptions = {
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

   console.log("Updating old id "+oldid+" with new id "+newid);

   $("#window"+oldid).html(getWorkflowBody(newid, "", newtitle, newsummary, ''));

   // window id will be changed in StopDrag event
   $("#window"+oldid).removeClass("ui-state-highlight");
   $.getJSON("UpdateAttrs","f=ccv&isRelease=y&a=" + objid + "&c="+oldid+"&nc="+newid,
   function(data){
    console.log("ccv: in success");
    var x = {id: "window"+oldid};
    wfOpenDetails(x,false,"N");
   });

  }
 };


function wfMouseOverLink(val)
{
 //if (EditMode=='Y')
 //{
  console.log("MouseOverLink id="+val.id);
  console.log("MouseOverLink from id ="+val.endpoints[0].getUuid());
  console.log("MouseOverLink to id ="+val.endpoints[1].getUuid());
  OverLink=readonly?null:val;
  LastLink=val;
 //}
}

function wfMouseExitLink()
{
 OverLink=null;
}

function initWorkflow(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function wfStartDrag()
{
 console.log("wfStartDrag");
 Dragging=true;
 oldid=0;
 newid=0;
}

function wfStopDrag()
{
 console.log("wfStopDrag");
 Dragging=false;
 // Get the old end points and connections
 var fromlist = [];
 var tolist = [];

 if (oldid>0 && newid>0) {
  // replacing existing component
  var p = wfPlumb.getConnections({source: "startwindow"},true);
  for (var i=0;i<p.length;i++) {
   if (p[i].targetId.substr(6)==oldid) {
    console.log("FROM #startwindow");
    wfPlumb.detach(p[i]);
    fromlist.push(0);
   }
  }
  $(".fragment").each(function() {
   var id = $(this).attr("id").substr(6);
   if (id != oldid)
   {
    console.log("looking at window id = "+id);
    var p = wfPlumb.getConnections({source: "window"+id},true);
    for (var i=0;i<p.length;i++) {
     if (p[i].targetId.substr(6)==oldid) {
      console.log("FROM #window"+id);
      wfPlumb.detach(p[i]);
      fromlist.push(id);
     }
    }
   }
  });

  var p = wfPlumb.getConnections({source: "window"+oldid},true);
  for (var i=0;i<p.length;i++) {
   console.log("TO: #"+p[i].targetId);
   wfPlumb.detach(p[i]);
   tolist.push(p[i].targetId.substr(6));
  }
  // Now remove the endpoints
  console.log("Removing endpoint window"+oldid+"in");
  wfPlumb.deleteEndpoint("window"+oldid+"in");
  console.log("Removing endpoint window"+oldid+"out");
  wfPlumb.deleteEndpoint("window"+oldid+"out");
  //
  // Change the window id
  //
  $("#window"+oldid).attr("id","window"+newid);
  //
  // Restore the endpoints on the new window id
  //
  wfPlumb.addEndpoint("window"+newid, targetEndpoint, {
   anchor : "TopCenter",
   uuid : "window"+newid+"in"
  });
  wfPlumb.addEndpoint("window"+newid, sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "window"+newid+"out"
  });
  // And restore the connections
  for (var i=0;i<fromlist.length;i++) {
   if (fromlist[i]==0) {
    wfPlumb.connect({uuids:["startwindowout", "window"+newid+"in"], editable:true});
   } else {
    wfPlumb.connect({uuids:["window"+fromlist[i]+"out", "window"+newid+"in"], editable:true});
   }
  }
  for (var i=0;i<tolist.length;i++) {
   wfPlumb.connect({uuids:["window"+newid+"out", "window"+tolist[i]+"in"], editable:true});
  }
  $("#window"+oldid).css("cursor","default");
 }
}

function LoadWorkflows()
{
 CreateScriptDiagram();
}

function waitForElement(elementPath, callBack){
 window.setTimeout(function(){
   if($(elementPath).length){
     callBack(elementPath, $(elementPath));
   }else{
     waitForElement(elementPath, callBack);
   }
 },500)
}

function CreateScriptDiagram()
{
 var InitialLoad = true;
 MaxWinID = 0;

 var $main = $("#innerworkflow");

 var menu2 = function(m,t){
  if (OverLink)
   return [{'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){wfMenuDeleteConnector(LastLink);} }} ];
 };

 $("#innerworkflow").dmContextMenu(menu2, {theme:'xp'});

 windowid=1;

 jsPlumb.importDefaults({
  // default drag options
  DragOptions : {
   cursor : '-webkit-grab',
   zIndex : 2000
  },
  // default to blue at one end and green at the other
  EndpointStyles : [ {
   fillStyle : '#225588'
  }, {
   fillStyle : '#558822'
  } ],
  // blue endpoints 7 px; green endpoints 11.
  Endpoints : [ [ "Dot", {
   radius : 7
  } ], [ "Dot", {
   radius : 11
  } ] ],
  // the overlays to decorate each connection with.  note that the label overlay uses a function to generate the label text; in this
  // case it returns the 'labelText' member that we set on each connection in the 'init' method below.
  ConnectionOverlays : [ [ "Arrow", {
   location : 1,
   length: 1,
   width: 1
  } ], [ "Label", {
   location : 0.1,
   id : "label",
   cssClass : "aLabel"
  } ] ]
 });

 var init = function(connection)
 {
  // connection.getOverlay("label").setLabel("success");
  connection.bind("editCompleted", function(o)
  {
   if (typeof console != "undefined")
    console.log("connection edited. path is now ", o.path);
  });
 };

 jsPlumb.bind("contextmenu", function(component, originalEvent) {
           originalEvent.preventDefault();
           return false;
       });

 jsPlumb.bind("jsPlumbConnection", function(connInfo, originalEvent)
 {
  console.log("connInfo.connection.id = " + connInfo.connection.id);

  init(connInfo.connection);

  connInfo.connection.bind("mouseenter",function(c) {wfMouseOverLink(connInfo.connection);});
  connInfo.connection.bind("mouseexit",function(c) {wfMouseExitLink();});

  console.log("a) from id ="+connInfo.connection.endpoints[0].getUuid());
  console.log("a) to id ="+connInfo.connection.endpoints[1].getUuid());
  console.log("b) from id ="+connInfo.sourceId);
  console.log("b) to id ="+connInfo.targetId);

  var FromUUID = connInfo.connection.endpoints[0].getUuid();
  var fn = connInfo.connection.sourceId.replace("window","");
  var tn = connInfo.connection.targetId.replace("window","");

  console.log("fn="+fn+" tn="+tn+"FromUUID="+FromUUID);
  //
  // If the source uuid is part of an "if" or "parallel" then set the pos accordingly.
  //
  var pos=1;
  if (FromUUID.indexOf("leftout")>=0) pos=2;
  if (FromUUID.indexOf("rightout")>=0) pos=3;

  if (InitialLoad == false)
  {
   $.getJSON("UpdateAttrs","f=af&a=" + objid + "&fn="+fn+"&tn="+tn+"&pos="+pos,
   function(data){
    console.log("updateattrs) in success");
    console.log(data);
   });
  }
 });

 jsPlumb.bind("connectionDetached", function(connInfo, originalEvent)
 {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  wfDeleteConnector(connInfo.connection);
 });

 //
 // Now get the current configuration for this action from the DB
 //
 console.log("actionid=" + objid);

 var url = "actionid=" + objid;

 if (ddwinid != "")
  url += "&windowid=" + ddwinid;

 console.log("GetActionLayout url="+url);
 $.getJSON('GetActionLayout',url,function(data){
  //
  // insert the windows
  //
  summSaveobjid = objid;

  yo = 140;
  var w = Math.round(Math.floor($("#innerworkflow").width()/2)-63);

  EditMode="Y";
  MaxWinID = data.MaxWinID;
  console.log("MaxWinID="+MaxWinID+" data.NodeCount="+data.NodeCount);
  console.log(data);
  if (data.startx >0) w = data.startx;
  readonly = data.readonly;

  if (readonly) {
	  $("#innerworkflow").block({ message: null });
  } else {
	  $("#innerworkflow").unblock();
  }

  // load fragment list in here so we know if we're readonly or not....
  CreateTree_Selection("#fragmentlist","fragments",readonly,"mainframe3");

  console.log("w="+w);

  // Start Window
  console.log("Adding start window");
  $('#innerworkflow').append( "<div class=\"startfragment\" id=\"startwindow\" " +
    "style=\"position: relative; top: 5px; left: " + w + "px \">" +
  "<span style=\"position:relative; top:5px\"><img src=\"css/images/go_16x.png\" style=\"padding-right:5px\">Start</div>");

  waitForElement("#innerworkflow",function(){
  var offset = $("#innerworkflow > div.startfragment").offset();
  var start_ypos = Math.round(offset.top - $(window).scrollTop());
  var start_xpos = Math.round(offset.left - $(window).scrollLeft());

  console.log("readonly="+readonly);
  if (!readonly) {
	  jsPlumb.draggable(jsPlumb.getSelector(".fragment, .ddfragment"), {
	  grid : [ 20, 20 ],
	  containment: '#innerworkflow',
	  cursor: "move",
	  stop: wfWindowMoved
	  });
	  jsPlumb.draggable(jsPlumb.getSelector(".startfragment"), {
	   grid : [ 20, 20 ],
	   containment: [0,0,10000,120],
	   cursor: "move",
	   stop: wfStartMoved
	  });
  }
  console.log("Adding startwindowout");
  jsPlumb.addEndpoint("startwindow", sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "startwindowout"
  });

  for (a=0;a<data.NodeCount;a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);

   if (WindowID > MaxWinID) MaxWinID = WindowID;
   if (data.Nodes[a].drilldown == "Y") {
    Class="ddfragment";
   } else {
    Class="fragment";
   }
   var xpos = Math.round(data.Nodes[a].xpos);
   var ypos = Math.round(data.Nodes[a].ypos);

 //  if (ypos < start_ypos)
 //   ypos = start_ypos - ypos;
   console.log("WindowID="+WindowID+" MaxWinID="+MaxWinID+" xpos="+xpos+" ypos="+ypos+" drilldown="+data.Nodes[a].drilldown);
   console.log("title="+data.Nodes[a].title);

   var procid = data.Nodes[a].procid;
   var funcid = data.Nodes[a].functionid;
   var img="script.png";
   if (procid>0) img="proc.png";
   else
   if (funcid>0) img="func.png";
   console.log("procid="+procid+" funcid="+funcid+" img="+img);
   console.log(data.Nodes[a].title+" xpos="+xpos+" ypos="+ypos);

   $('#innerworkflow').append( "<div  class=\""+Class+"\" id=\"window" + WindowID + "\" " +
     "style=\"position: relative; top: " + ypos + "px; left: " + xpos + "px \">" +
     "<span style=\"position:relative; top:5px;\"><img src=\"css/images/"+img+"\" style=\"padding-right:5px\">" +data.Nodes[a].title+ "</span>" +
     "</div>");
   if (data.Nodes[a].drilldown == "Y") {
    $("#window"+WindowID).dblclick(function() {
      wfDrillDown($(this));
    });
   }

   if (!readonly) {
	    jsPlumb.draggable(jsPlumb.getSelector(".fragment, .ddfragment"), {
	     grid : [ 20, 20 ],
	     containment: '#innerworkflow',
	     cursor: "move",
	     stop: wfWindowMoved
	    });
   }

    console.log("Adding endpoint window"+WindowID+"in");

   jsPlumb.addEndpoint("window"+WindowID, targetEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+WindowID+"in"
   });
   if (data.Nodes[a].exitpoints == 1)
   {
	   console.log("Adding endpoint window"+WindowID+"out");
    jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
     anchor : "BottomCenter",
     uuid : "window"+WindowID+"out"
    });
   }
   else
   if (data.Nodes[a].exitpoints == 2)
   {
	   if (data.Nodes[a].style == 1) {
		   // If blocks
			console.log("Adding endpoint window"+WindowID+"leftout");
			jsPlumb.addEndpoint("window"+WindowID, sourceEndpointLeft, {
				anchor : "LeftMiddle",
				uuid : "window"+WindowID+"leftout"
			});
			console.log("Adding endpoint window"+WindowID+"rightout");
			jsPlumb.addEndpoint("window"+WindowID, sourceEndpointRight, {
				anchor : "RightMiddle",
				uuid : "window"+WindowID+"rightout"
			});
	   } else {
		   // All other styles
		   console.log("Adding endpoint window"+WindowID+"leftout");
			jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
				anchor : "LeftMiddle",
				uuid : "window"+WindowID+"leftout"
			});
			console.log("Adding endpoint window"+WindowID+"rightout");
			jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
				anchor : "RightMiddle",
				uuid : "window"+WindowID+"rightout"
			});
	   }

   }
   else
   if (data.Nodes[a].exitpoints == 3)
   {
	   console.log("Adding endpoint window"+WindowID+"leftout");
    jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
     anchor : "LeftMiddle",
     uuid : "window"+WindowID+"leftout"
    });
    console.log("Adding endpoint window"+WindowID+"rightout");
    jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
     anchor : "RightMiddle",
     uuid : "window"+WindowID+"rightout"
    });
    console.log("Adding endpoint window"+WindowID+"out");
    jsPlumb.addEndpoint("window"+WindowID, sourceEndpoint, {
     anchor : "BottomCenter",
     uuid : "window"+WindowID+"out"
    });
   }
   //
   // Add the details
   //
   var otid="";
   if (data.Nodes[a].functionid>0) otid="fn"+data.Nodes[a].functionid+"-"+data.Nodes[a].kind;
   else
   if (data.Nodes[a].procid>0) otid="pr"+data.Nodes[a].procid+"-"+data.Nodes[a].kind;
   Fragments[WindowID]={outputs:0,otid:otid,type:data.Nodes[a].typeid,attributes:{"ExitPoints":data.Nodes[a].exitpoints}};

   console.log("functionid="+data.Nodes[a].functionid);
   console.log("procid="+data.Nodes[a].procid);
   var contextMenu=[];
   console.log("contextMenu.size="+contextMenu.length);
   if (data.Nodes[a].procid > 0) {
	   contextMenu = readonly?menu2WorkflowProc:menu1WorkflowProc;
   } else if (data.Nodes[a].functionid > 0) {
	   contextMenu = readonly?menu2WorkflowFunction:menu1WorkflowFunction;
   } else {
	   contextMenu = readonly?menu2Workflow:menu1Workflow;
   }
   $('#window'+WindowID).dmContextMenu(contextMenu, {theme:'xp'});
  }

  //
  // Now do all the links
  //
  console.log("Inserting Links...");
  for (a=0;a<data.LinkCount;a++)
  {
   var nf = data.Links[a].nodefrom;
   var nt = "#window" + data.Links[a].nodeto;
   if (nf==0)
   {
    nf="#startwindow";
   }
   else
   {
    nf="#window"+nf;
   }

   var srcconn=nf.substr(1)+"out";
   var tgtconn=nt.substr(1)+"in";
   if (data.Links[a].pos==1) srcconn=nf.substr(1)+"out";
   else
   if (data.Links[a].pos==2) srcconn=nf.substr(1)+"leftout";
   else
   if (data.Links[a].pos==3) srcconn=nf.substr(1)+"rightout";
   console.log("Connecting ("+srcconn+") to ("+tgtconn+")");
   if (jsPlumb.getEndpoint(srcconn) != null && jsPlumb.getEndpoint(tgtconn) != null)
   {
    console.log("nodefrom="+data.Links[a].nodefrom);

    var connection = jsPlumb.connect({uuids:[srcconn,tgtconn], detachable:true,editable:true});
    console.log("Connection made");

    if (data.Links[a].nodefrom > 0)
    {
     Fragments[data.Links[a].nodefrom].outputs++;
    }
   }
   else
   {
    console.log("Failed to connect - end points do not exist");
   }
  }
  windowid=parseInt(MaxWinID)+1;
  console.log("done getting action layout - windowid="+windowid);
  InitialLoad = false;
  });
 }).fail(function(fdata){
   console.log("Something has gone horribly wrong "+fdata);
   $.each(fdata,function(key,val){
    console.log("fdata) key="+key+" val="+val);
   });
   });
}

function SetUpFragments()
{
 var $main = $("#innerworkflow");

 var menu2 = function(m,t){
  if (OverLink)
   return [
    {'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){wfMenuDeleteConnector(LastLink);} }} ];
 };

 $("#innerworkflow").dmContextMenu(menu2, {theme:'xp'});
 // var AcceptList = $(".fragmentsource");
 // let the servers items be draggable
 /*
 $(".fragmentsource").draggable({
  //cancel : "a.ui-icon", // clicking an icon won't initiate dragging
  revert : "invalid", // when not dropped, the item will revert back to its initial position
  containment : "document",
  helper : "clone",
  cursor : "-webkit-grab"
 });
*/
 // let the main area be droppable, accepting the fragments
 $main.droppable({
  accept : AcceptList,
 // activeClass : "ui-state-highlight",
  drop : function(event, ui)
  {
   var tx = (event.pageX) % 20;
   var ty = (event.pageY) % 20;
   if (tx>10) tx=-(20-tx);
   if (ty>10) ty=-(20-ty);
   var xpos = Math.round(ui.position.left); // - tx) - 60;
   // var ypos = (event.pageY - ty) - 20;
   var ypos = Math.round(ui.position.top); // - ty) - (ui.offset.top - ui.position.top )) + $("#compitem").scrollTop() - 90;

   var ct="fragment";
   if (ui.draggable.attr("drilldown") == "Y") {
    ct="ddfragment";
   }
   console.log("windowid "+windowid+" dropped at xpos="+xpos+" ypos="+ypos);
   $('#innerworkflow').append( "<div  class=\"workflowbox "+ct+"\" id=\"window" + windowid + "\" " +
     "style=\"position: relative; top: " + ypos + "px; left: " + xpos + "px \">" +
     "<span style=\"position:relative; top:5px;\"><img src=\"css/images/file.png\" style=\"padding-right:5px\">" +ui.draggable.text()+ "</span>" +
     "</div>");

   if (ui.draggable.attr("drilldown") == "Y") {
    $("#window"+windowid).dblclick(function() {
     wfDrillDown($(this));
    });
   }


   jsPlumb.draggable(jsPlumb.getSelector(".fragment, .ddfragment"), {
    grid : [ 20, 20 ],
    containment: '#innerworkflow',
    cursor: "move",
    stop: wfWindowMoved
   });
   jsPlumb.addEndpoint("window"+windowid, targetEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+windowid+"in"
   });
   //
   // Conditional Expressions get two exit points, abort functions get none,
   // parallel gets 3, all others get 1.
   //
   if (ui.draggable.attr("exitpoints") == "1")
   {
    // Normal Fragment
    jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
     anchor : "BottomCenter",
     uuid : "window"+windowid+"out"
    });
   }
   else
   if (ui.draggable.attr("exitpoints") == "2")
   {
    // Conditional Expression
    jsPlumb.addEndpoint("window"+windowid, sourceEndpointLeft, {
     anchor : "LeftMiddle",
     uuid : "window"+windowid+"leftout"
    });
    jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
     anchor : "RightMiddle",
     uuid : "window"+windowid+"rightout"
    });
   }
   else
   if (ui.draggable.attr("exitpoints") == "3")
   {
    // Parallel Processing
    jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
     anchor : "LeftMiddle",
     uuid : "window"+windowid+"leftout"
    });
    jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
     anchor : "RightMiddle",
     uuid : "window"+windowid+"rightout"
    });
    jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
     anchor : "BottomCenter",
     uuid : "window"+windowid+"out"
    });
   }

   return;

    var swd = wfGetNearestWindow(windowid);

    if (jsPlumb.getEndpoint(swd.fuuid) != null)
    {
     console.log("Connecting to "+swd.fuuid);
     connection = jsPlumb.connect({uuids:[swd.fuuid, "window"+windowid+"in"], editable:true});
     if (connection != null)
     {
      if (swd.id > 0) {
       if (Fragments[swd.id]) Fragments[swd.id].outputs++;
      }
      // Do we still need to do this? The binding may take care of it....
      $.getJSON("UpdateAttrs","f=af&a=" + objid + "&fn="+swd.id+"&tn="+windowid+"&pos="+swd.pos,
          function(data){
       console.log("in success");
          });
     }
   }
   //
   // Add the details
   //
   Fragments[windowid]={outputs:0,type:ui.draggable.attr("typeid"),attributes:{"ExitPoints":ui.draggable.attr("exitpoints")}};
   //
   // Add right-click
   //
   $('#window'+windowid).dmContextMenu(menu1Workflow, {theme:'xp'});
   //
   // Update this in the DB
   //
  // ypos+=120; // adjust for tabs at top of screen

   $.ajax({
	   dataType: "json",
	   url: "UpdateAttrs",
	   async: false,
	   data: "f=an&a=" + objid + "&w="+windowid+"&pw=" + parentwindowid + "&xpos="+xpos+"&ypos="+ypos+"&typeid="+ui.draggable.attr("typeid"),
	   });

   /*
   $.getJSON("UpdateAttrs","f=an&a=" + objid + "&w="+windowid+"&pw=" + parentwindowid + "&xpos="+xpos+"&ypos="+ypos+"&typeid="+ui.draggable.attr("typeid"),
      function(data){
      });
   */
   //
   // Now "click" it
   //
   // $('#window'+windowid).trigger('click');
   var x = {id: "window"+windowid};
   wfOpenDetails(x,true,"N");

   windowid++;
  }
 });
}

function getWorkflowBody(id,otid,name,summ,suffix)
{
 console.log("getWorkflowBody otid="+otid);

 var img = "css/images/script.png";
 if (otid.substr(0,2)=="pr") img="css/images/functions_procedures_sm.png";
 else
 if (otid.substr(0,2)=="fn") img="css/images/functions_procedures_sm.png";
 return "<span style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" +name+ "</span>";
}

function getWorkflowMarkup(id,drilldown,otid,name,summ,xpos,ypos,suffix)
{
 var ddclass=drilldown?"ddfragment":"fragment";
 return '<div class="'+ddclass+'" id="window'+id+'" '
  + 'style="position: relative; top: '+ypos+'px; left: '+xpos+'px ">'
  + getWorkflowBody(id,otid,name,summ,suffix) + '</div>';
}

function wfGetNearestWindow(winid)
{
 // Finds the nearest window to this window id (the one we've just dropped onto the canvas)
 // and looks for an empty endpoint
 var swd = new Object();
 var winobj = $("#window" + winid);
 var pos = $("#window"+winid).position();
 var left = pos.left;
 var top = pos.top;
 var minDistance = 0;
 swd.fuuid = "startwindowout";
 swd.id = 0;
 swd.pos = 1;
 console.log("GetNearestWindow winid="+winid+" left="+left+" top="+top);
 $( ".fragment, .ddfragment" ).each(function() {
    var id = $(this).attr("id").substr(6);
    if (id != winid)
    {
     console.log("id = "+id+" ExitPoints = "+Fragments[id].attributes.ExitPoints+" outputs="+Fragments[id].outputs);
     var f = Fragments[id].attributes.ExitPoints - Fragments[id].outputs;
     if (f>0)
     {
       console.log("id = "+id);
      console.log("position.top="+$(this).position().top);
       console.log("position.left="+$(this).position().left);
       console.log("f="+f);
       var d = calculateDistance($(this),left,top);
       console.log("distance="+d);
       if (d < minDistance || minDistance == 0) {
        swd.fuuid = "window"+id+"out";
        swd.id = id;
        if (jsPlumb.getEndpoint(swd.fuuid) == null) {
         // Not found a normal exit point - connect to left or right depending on left pos
         if (left <= $(this).position().left) {
         swd.fuuid = "window"+id+"leftout";
         swd.pos=2;
         } else {
            swd.fuuid = "window"+id+"rightout";
            swd.pos=3;
         }
        }
        else
         if (jsPlumb.getEndpoint("window"+id+"leftout") != null) {
          // There was a normal exit point AND a left exit point - must be a parallel tag
          // Connect to either bottom, left or right depending on what's closest.
          if (Math.abs(left - $(this).position().left) <= 30)
          {
           // connect to bottom
           swd.fuuid = "window"+id+"out";
           swd.pos = 1;
          }
          else
          if (left <= $(this).position().left) {
           swd.fuuid = "window"+id+"leftout";
         swd.pos=2;
         }
          else
          {
            swd.fuuid = "window"+id+"rightout";
            swd.pos=3;
          }
         }
        minDistance = d;
       }
     }
    }
  });

 return swd;
}


function calculateDistance(elem,X,Y)
{
       return Math.floor(Math.sqrt(Math.pow(X - (elem.offset().left+(elem.width()/2)), 2) + Math.pow(Y - (elem.offset().top+(elem.height()/2)), 2)));
}

function MouseOverLink(val)
{
 if (EditMode=='Y')
 {
  console.log("MouseOverLink id="+val.id);
  console.log("MouseOverLink from id ="+val.endpoints[0].getUuid());
  console.log("MouseOverLink to id ="+val.endpoints[1].getUuid());
  console.log("readonly="+readonly);
  OverLink=readonly?null:val;
  console.log("OverLink="+OverLink);
  LastLink=val;
 }
}

function MouseExitLink()
{
 OverLink=null;
}

function wfWindowMoved(event,ui)
{
 event.stopPropagation();
 var windowid=event.target.id;
 var id = windowid.replace("window","");
 var tid = Fragments[id].type;
 console.log("xpos="+ui.position.left+" ypos="+ui.position.top);

 // $.getJSON("UpdateAttrs","f=wm&a=" + objid + "&w="+id+"&pw=" + parentwindowid + "&xpos="+Math.round(ui.position.left)+"&ypos="+(Math.round(ui.position.top) + 20) +"&typeid="+tid,

 $.getJSON("UpdateAttrs","f=wm&a=" + objid + "&w="+id+"&pw=" + parentwindowid + "&xpos="+Math.round(ui.position.left)+"&ypos="+(Math.round(ui.position.top)) +"&typeid="+tid,
 function(data){
  console.log("WindowMoved) in success");
 });
}

function wfStartMoved(event,ui)
{
	console.log("wfStartMoved, parentwindowid="+parentwindowid);
 //var xpos = ui.position.left;
// $.getJSON("UpdateAttrs","f=wm&a=" + objid + "&w=0&pw=" + parentwindowid + "&xpos="+xpos+"&ypos="+ui.offset.top+"&typeid=0",

 var xpos = ui.offset.left;
 $.getJSON("UpdateAttrs","f=wm&a=" + objid + "&w=0&pw=" + parentwindowid + "&xpos="+Math.round(ui.position.left)+"&ypos="+(Math.round(ui.position.top))+"&typeid=0",
 function(data){
  console.log("WindowMoved) in success");
 });
}


function LoadWorkflowsData(ddwindow)
{
 ddwinid = ddwindow;

 DontSave = 1;
 usedWorkflows = new Array();
 Fragments = new Array();

 cPlumb.reset();
 aPlumb.reset();
 avPlumb.reset();
 cvPlumb.reset();
 cisplumb.reset();
 wfPlumb.reset();
 jsPlumb.reset();

 $("#innerversions").html("");
 $("#innercomp").html("");
 $("#innerappver").html("");
 $("#inneritem").html("");
 $("#innerworkflow").html("");
 $("#innercompversions").html("");
 console.log("LoadWorkflows");

 if ($("#fragmentlist").hasClass("ui-accordion"))
 {
   $("#fragmentlist").accordion('destroy');
   $("#fragmentlist").html("");
   $("#fragmentlist").removeClass();
 }
 LoadWorkflows();
 DontSave = 0;
 parentwindowid=ddwindow;
}

function OpenStartParameters(x)
{
 var pwd = parent.$("#modal");
 pwd.empty();

 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   SaveStartParameters();
   pwd.dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   pwd.dialog("close");
  }
 } ];

 pwd.dialog(
   {
    modal : true
   });

   pwd.load("getstartparams?actionid=" + objid + "&editmode=Y");

   pwd.dialog("option", "title", "Start Parameters");
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "auto");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog("open");
}

function SaveWFDetails(id)
{
 var url="f=sd&a=" + objid + "&w="+id;
 var title = "";
 var resultvar = "";
 var mmf=false;
 parent.$('.atttable td').each(function (i, row) {
	 $(this).removeClass("fragmenterror").removeClass("fragmentfielderror");
 });
 parent.$("#errtext").html("&nbsp;");
 parent.$("#fragattrs :input").each(function() {
	 console.log("type="+$(this).attr("type"));
  if ($(this).attr("type") == "checkbox") {
   var c=$(this).is(':checked')?"Y":"N";
   url=url+"&"+$(this).attr("id")+"="+c;
  } else if ($(this).attr("type") != "hidden") {
	  console.log("id="+$(this).attr("id")+" resultvar=["+resultvar+"]");
	  if ($(this).attr("id") == "title") title = $(this).val();
	  if ($(this).attr("id") == "resultvar") {
		  if ($(this).val() == "") {
			  parent.$("#rresultvar").addClass("fragmenterror");
			  parent.$("#cresultvar").addClass("fragmentfielderror");
			  parent.$("#errtext").html("Please enter value(s) for the highlighted field(s)");
			  mmf=true;
		  }
	  }
	  if ($(this).val()=="" && $(this).attr("id").charAt(0)=='f') {
		  var id = "h"+$(this).attr("id").substr(1);
		  var rowid = "r"+$(this).attr("id").substr(1);
		  var tid = "c"+$(this).attr("id").substr(1);
		  var h = parent.$("#"+id).val();
		  if (h=='Y') {
			  parent.$("#"+rowid).addClass("fragmenterror");
			  parent.$("#"+tid).addClass("fragmentfielderror");
			  parent.$("#errtext").html("Please enter value(s) for the highlighted field(s)");
			  mmf=true;
		  }
	  }
	  url=url+"&"+$(this).attr("id")+"="+encodeURIComponent($(this).val());
  }
 });
 if (!mmf) {
	 $.getJSON('UpdateAttrs',url,
	     function(data) {
		 	if (title != "") $("#window"+id).find("span").html("<span style=\"position:relative; top:5px;\"><img src=\"css/images/proc.png\" style=\"padding-right:5px\">" + title + "</span>");
	 	 });
 }
 return !mmf;
}
