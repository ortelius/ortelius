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

function getComponentVersionsBody(id,name,summ,suffix)
{
 var img = "css/images/components_sm.png";
 img = img.replaceAll('-','_');
 return "<span onClick=\"cvClickElement(" +id+ ");\" style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" +name+ "</span>";
}

function getComponentVersionsMarkup(id,name,summ,xpos,ypos,suffix)
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
 return '<div class="drawcomponent componentbox" id="window'+id+'" '
  + 'style="position: absolute; top: '+ypos+'px; left: '+xpos+'px ">'
  + getComponentVersionsBody(id,name,summ,suffix) + '</div>';
}

function SaveComponentVersionsDetails(id)
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
   $("#window"+id).html(getComponentVersionsBody(id, data.title, data.summary, ''));
 });
}

function cvClickElement(id)
{
 /*
 var att = parent.$("#homeatts");
 att.dialog($attvals)
 .empty()
 .dialog("option","title","Component Details")
 .dialog("option","buttons",
  [
        { text: "Ok",     click: function() { SaveComponentVersionsDetails(id); att.dialog("close").dialog("destroy").hide(); } },
        { text: "Cancel", click: function() { att.dialog("close").dialog("destroy").hide(); } }
     ]
 );
 $(".componentbox").css("background-color","#eeeeef"); // turn off any other selected fragment
 $("#window"+id).css("background-color","yellow"); // highlight the one selected
 att.html("Loading...").load("getcompversum?verid="+id,cFrameEnabled(att));
 */
 CurrentlySelectedID=id;

}

function cvOpenDetails(x)
{
 cvClickElement(x.id.substr(6));
}


function cvNewComponentVersions(node)
{
 if (DontSave)
  return;

 var xpos = cCurrentMousePos.x - 480;
 var ypos = cCurrentMousePos.y - 120;
 var swd = {};
 swd.fuuid = "window"+objid+"out";
 swd.id=objid;

 $(".componentbox").each(function() {
  var id = $(this).attr("id").substr(6);
  console.log("id = "+id);
  var p = cvPlumb.getConnections({source: "window"+id},true);
  console.log("p.length="+p.length);
  if (p.length == 0) {
    // Found a component with no output connections - return that
   console.log("returning window"+id+"out");
   swd.fuuid = "window"+id+"out";
   swd.id=id;
  }
 });

 $.ajax(
   {
    url : "UpdateAttrs?f=cnv&c=" + swd.id + "&xpos="+xpos+"&ypos="+ypos,
    dataType : "json",
    type : "GET",
    success : function(data)
    {
     $('#innercompversions').append(getComponentVersionsMarkup(data.verid, data.vername, '', xpos, ypos, ''));

     $("#window"+data.verid).dblclick(function() {
      cvDrillDown($(this));
     });

     cvPlumb.draggable(cvPlumb.getSelector(".componentbox"), {
      grid : [ 20, 20 ],
      containment: '#innercompversions',
      cursor: "-webkit-grab",
      stop: cvWindowMoved
     });

     console.log("AddEndPoint (using compEndpoint");

     cvPlumb.addEndpoint("window"+data.verid, compEndpoint, {
      anchor : "TopCenter",
      uuid : "window"+data.verid+"in"
     });

     cvPlumb.addEndpoint("window"+data.verid, compEndpoint, {
      anchor : "BottomCenter",
      uuid : "window"+data.verid+"out"
     });

     console.log("Binding context menu menu1 to window #window"+data.verid);
     if (isRelease == "Y")
      $("#window"+data.verid).dmContextMenu(menuRelComponentVersions, {theme:'xp'});
     else
      $("#window"+data.verid).dmContextMenu(menuComponentVersions, {theme:'xp'});

     // $("#window"+data.verid).resizable({ handles: "se" });
     $("#window"+data.verid).resize(function(){
      cvPlumb.repaintEverything();
     });
     // Need this configurable off user profile

     var swd = cvFindWindowWithNoOutput(data.verid);
     console.log("swd.fuuid="+swd.fuuid);
     if (cvPlumb.getEndpoint(swd.fuuid) != null)
     {
      console.log("Connecting to "+swd.fuuid);
      // The connect call will fire the new connection binding which will update the DB
      var connection = cvPlumb.connect({uuids:[swd.fuuid, "window"+data.verid+"in"], editable:true});
     }

     cvClickElement(data.verid);
     Refresh(currenttree);
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

function cvDeleteNode(node)
{
 console.log("cvDelete");

 if (DontSave)
  return;

 // alert("Detaching all connections");
 cvPlumb.detachAllConnections(node.id);
 // alert("Removing all endpoints");
 cvPlumb.removeAllEndpoints(node.id);
 // alert("Detaching the node");
 $("#"+node.id).detach();
 var n = node.id.substr(6);

 $(currenttree).jstree("delete_node", "#cv" + n);

  $.getJSON("UpdateAttrs","f=cdv&c=" + objid + "&v="+n,
   function(data){
  });
}

function cvWindowMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("in WindowMoved, ui.position.top="+ui.position.top);
 var windowid=event.target.id;
 var id = windowid.replace("window","");

  $.getJSON("UpdateAttrs","f=cvm&c=" + objid + "&v="+id+"&xpos="+Math.round(ui.position.left)+"&ypos="+Math.round(ui.position.top),
   function(data){
    console.log("WindowMoved) in success");
  });
}


function cvMenuDeleteConnector(conn)
{
 if (conn)
 {
  cvPlumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}

function cvUpdateBranchLabel(BranchName,vid)
{
	console.log("UpdateBranchLabel - BranchName="+BranchName+" version="+vid);
	$.getJSON("UpdateAttrs","f=ubl&t=cv&a="+vid+"&bn="+BranchName,
		function(data){
			console.log("UpdateBranchLabel) in success");
		}
	);
}

function cvChangeLabelOK(conn)
{
	var labeltext=parent.$("#modal input:text").val();
	console.log("ChangelabelOK, labeltext="+labeltext);
	var overlay = conn.getOverlay(conn.id+"_label");
	if (!overlay) {
		conn.addOverlay([ "Label", { label: labeltext, cssClass: "cLabel", location: 0.5, id: conn.id+"_label" }]);
		overlay = conn.getOverlay(conn.id+"_label");
	} else {
		overlay.setLabel(labeltext);
	}
	console.log("label="+overlay.getLabel());
	var FromUUID = conn.endpoints[0].getUuid();
	console.log("FromUUID="+FromUUID);
	console.log("conn.targetId="+conn.targetId+" conn.sourceId="+conn.sourceId);
	var tn;
	if (FromUUID.indexOf("out")>0) {
		tn = conn.targetId.replace("window","");
	} else {
		tn = conn.sourceId.replace("window","");
	}
	console.log("tn="+tn);
	cvUpdateBranchLabel(labeltext,tn);
}

function cvChangeLabel(conn)
{
	var labelname="";
	var overlay = conn.getOverlay(conn.id+"_label");
	if (overlay) {
		labelname = overlay.getLabel();
		console.log("labelname="+labelname);
	} else {
		console.log("In change label, overlay is null");
	}
	console.log("Change Label");
	var att = parent.$("#modal");
	att.dialog({ resizable: false, modal: true, dialogClass: "aboutsDialog" });
	att.empty();
	att.dialog("option","title","Branch Label");
	att.dialog("option","buttons",
		[
	       { text: "Ok",     click: function() { cvChangeLabelOK(conn); att.dialog("close"); } },
	       { text: "Cancel", click: function() { att.dialog("close"); } }
	    ]
	);
	att.html(
			"<html><body><table border=0><tr><td><img src='images/label-large.png'></td><td>Change Branch Label</td></tr></table><hr>" +
			"Branch Label:&nbsp;<input type='text' id='labeltext' width='60' value='"+labelname+"'><br></body></html>");
	att.css("overflow-y","hidden");
	att.dialog("open");
}

function cvAddLabel(conn)
{
	// var overlay = conn.getOverlay(conn.id+"_label");
	//if (overlay) overlay.setLabel("Branch x");
	//else conn.addOverlay([ "Label", { label: "Branch x", cssClass: "cLabel", location: 0.5, id: conn.id+"_label" }]);
	cvChangeLabel(conn);
}

function cvRemoveLabel(conn)
{
	console.log("cvRemove Label");
	conn.removeOverlay(conn.id+"_label");
	var FromUUID = conn.endpoints[0].getUuid();
	var tn;
	if (FromUUID.indexOf("out")>0) {
		tn = conn.targetId.replace("window","");
	} else {
		tn = conn.sourceId.replace("window","");
	}
	cvUpdateBranchLabel("",tn);
}

function cvFindWindowWithNoOutput(winid)
{
 var swd = new Object();
 var pos = $("#window"+winid).position();
 var left = + Math.round(pos.left);
 var top = Math.round(pos.top);
 var minDistance = 0;
 swd.fuuid = "window" + objid + "out";
 swd.id = 0;
 console.log("GetNearestWindow winid="+winid+" left="+left+" top="+top);
 $(".componentbox").each(function() {
  var id = $(this).attr("id").substr(6);
  if (id != winid)
  {
   console.log("id = "+id);
   var p = cvPlumb.getConnections({source: "window"+id},true);
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

function cvDeleteConnector(conn)
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
  if (fn.indexOf("start") > -1) fn=0;
  if (tn.indexOf("start") > -1) tn=0;
  $.getJSON("UpdateAttrs","f=cdd&c=" + objid + "&fn="+fn+"&tn="+tn,
  function(data){
   console.log("DeleteConnector) in success");
  });
 }
}


function cvMouseOverLink(val)
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

function cvMouseExitLink()
{
 OverLink=null;
}

function initComponentVersions(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function LoadComponentVersionsData()
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

 cvPlumb.importDefaults({
  // default drag options
  DragOptions : {
   cursor : '-webkit-grab',
   zIndex : 2000
  }
 });

 cvPlumb.bind("contextmenu", function(component, originalEvent) {
        originalEvent.preventDefault();
        return false;
    });

 var menu2 = function(m,t){
  if (DontSave)
   return;

  console.log("in menu2 (contextmenu activated)");
  if (!OverLink)
  {
   if (isRelease == "Y")
    return mainmenuRelComponentVersions;
   else
    return mainmenuComponentVersions;
  }
  // Construct a menu for this connector based on its content
  var ma = [];
  ma.push({'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){cMenuDeleteConnector(LastLink);} }});
  var overlay = LastLink.getOverlay(LastLink.id+"_label");
  console.log("overlay="+overlay);
  if (overlay) {
	var FromUUID = LastLink.endpoints[0].getUuid();
	if (FromUUID.indexOf("bottom")>0) {
		pc=LastLink.endpoints[0].getUuid();
	} else {
		pc=LastLink.endpoints[1].getUuid();
	}
	console.log("pc="+pc);
	var cl = jsPlumb.getConnections();
	var bc = new Array();
	bc[pc]=0;
	// Check that every other flow has an overlay/label.
	$.each(cl,function(key,val){
		var from = val.endpoints[0].getUuid();
		var to = val.endpoints[1].getUuid();
		var ov = val.getOverlay(val.id+"_label");
		console.log("from="+from+" to="+to);
		if (from.indexOf("bottom")>=0 && ov == null) {
			if (typeof bc[from] == 'undefined') bc[from]=0;
			bc[from]++;
		}
		if (to.indexOf("bottom")>=0 && ov == null) {
			if (typeof bc[to] == 'undefined') bc[to]=0;
			bc[to]++;
		}
	});
	console.log("bc["+pc+"]="+bc[pc]);
	if (bc[pc]==0) {
		ma.push({'Remove Branch Label':     {icon:"images/delete.png",onclick:function(menuItem,menu){cvRemoveLabel(LastLink);} }});
	}
	ma.push({'Change Branch Label':     {icon:"images/label.png",onclick:function(menuItem,menu){cvChangeLabel(LastLink);} }});
  } else {
	ma.push({'Add Branch Label':        {icon:"images/label.png",onclick:function(menuItem,menu){cvAddLabel(LastLink);} }});
  }
  return ma;
  };

 console.log("Binding default context menu");

 if (!readonly) {
	 try {
	 $("#innercompversions").dmContextMenu(menu2, {theme:'xp'});
	 } catch(e) { console.log(e); }
 }


 cvPlumb.bind("jsPlumbConnection", function(connInfo, originalEvent) {
  console.log("jsPlumbConnection fired - about to call init");
  initComponentVersions(connInfo.connection);
  console.log("init called");

  connInfo.connection.bind("mouseenter",function(c) {cvMouseOverLink(connInfo.connection);});
  connInfo.connection.bind("mouseexit",function(c) {cvMouseExitLink();});

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

  if (fn.indexOf("start") > -1) fn=0;
  if (tn.indexOf("start") > -1) tn=0;

  console.log("fn="+fn+" tn="+tn);

  if (InitialLoad == false)
  {
   console.log("Updating DB with compid=" + objid + " fn="+fn+" tn="+tn);

   $.getJSON("UpdateAttrs","f=cad&c=" + objid + "&fn="+fn+"&tn="+tn,
   function(data){
    console.log("updateattrs) in success");
   });
  }

 });

 cvPlumb.bind("connectionDetached", function(connInfo, originalEvent) {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  cvDeleteConnector(connInfo.connection);
 });


 $.getJSON('GetCompVersionLayout',"compid=" + objid,function(data){

  readonly = data.readOnly;

  // Start Window
  yo = 140;
  var w = Math.floor($("#innercompversions").width()/2)-63;

  $("#innercompversions").append( "<div class=\"startcomponent\" id=\"window" + objid + "\" " +
    "style=\"position: relative; top: 100px; left: " + w + "px \">" +
    "<span style=\"position:relative; top:5px\"><img src=\"css/images/go_16x.png\" style=\"padding-right:5px\">" + objName + "</div>");

  cvPlumb.addEndpoint("window" + objid, compEndpoint, {
   anchor : "BottomCenter",
   uuid : "window" + objid + "out"
  });

  var MaxWinID=0;
  //
  // insert the components
  //
  console.log("in insert block for component versions");
  for (a=0;a<data.NodeCount;a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);
   if (WindowID > MaxWinID) MaxWinID = WindowID;

   $("#innercompversions").append(getComponentVersionsMarkup(WindowID, data.Nodes[a].name, data.Nodes[a].summary, data.Nodes[a].xpos, data.Nodes[a].ypos, data.Nodes[a].suffix));
   $("#window"+WindowID).dblclick(function() {
     cvDrillDown($(this));
   });

   cvPlumb.addEndpoint("window"+WindowID, compEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+WindowID+"in"
   });
   cvPlumb.addEndpoint("window"+WindowID, compEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+WindowID+"out"
   });

   if (!readonly) {
	   console.log("Binding context menu menu1 to window #window"+WindowID);
	   if (isRelease == "Y")
	    $("#window"+WindowID).dmContextMenu(menuRelComponentVersions, {theme:'xp'});
	   else
	    $("#window"+WindowID).dmContextMenu(menuComponentVersions, {theme:'xp'});

	   $("#window"+WindowID).resize(function(){
	      cvPlumb.repaintEverything();
	   });
	   console.log("done");
   }
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
    srcconn = "window" + objid + "out";
   } else {
    srcconn = "window" + data.Links[a].nodefrom + "out";
   }
   var tgtconn = "window" + data.Links[a].nodeto + "in";

   console.log("Connect srcconn="+srcconn+" tgtconn="+tgtconn);
   console.log(data.Links[a]);

   if (cvPlumb.getEndpoint(srcconn) != null && cvPlumb.getEndpoint(tgtconn) != null)
   {
    var connection = cvPlumb.connect({uuids:[srcconn,tgtconn], detachable:true, editable:true});
    if (data.Links[a].branch.length>0) {
		var overlay = connection.getOverlay(connection.id+"_label");
		if (overlay == null) {
			connection.addOverlay([ "Label", { label: data.Links[a].branch, cssClass: "cLabel", location: 0.5, id: connection.id+"_label" }]);
		}
	}
   }
  }

  cvPlumb.draggable(cvPlumb.getSelector(".componentbox"), {
   grid : [ 20, 20 ],
   containment: '#innercompversions',
   cursor: "-webkit-grab",
   stop: cvWindowMoved
  });

  InitialLoad = false;

 }).fail(function(fdata){
  console.log("Something has gone horribly wrong "+fdata);
  $.each(fdata,function(key,val){
   console.log("fdata) key="+key+" val="+val);
  });
  });
 $("#innercompversions").resizable({ handles: "s" });

 if (readonly)
  $("#innercompversions").block({ message: null });
 else
  $("#innercompversions").unblock();

 DontSave = 0;
}

$(document).mousemove(function(event) {
    cCurrentMousePos.x = event.pageX;
    cCurrentMousePos.y = event.pageY;
});


function cvStartMoved(event,ui)
{
 if (DontSave)
  return;

 console.log("Start Moved ypos="+Math.round(ui.offset.left));
 $.getJSON("UpdateAttrs","f=cvm&c=" + objid + "&v="+id+ "&xpos="+Math.round(ui.offset.left),
 function(data){
  console.log("WindowMoved) in success");
 });
}
