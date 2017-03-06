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

function getVersionsBody(id,name,summ,domain)
{
 var img1 = "css/images/appversion_16x.png";
 img1 = img1.replaceAll('-','_');
 var img2 = "css/images/domain_16x.png";
 var st="";
 var dt="";
 if (summ.length > 0) {
	st = "<br><div style=\"white-space: nowrap\">"+summ+"</div>";
 } else {
	st = "<br>&nbsp;&nbsp;";
 }
 if (domain.length > 0) {
	 dt = "<tr><td style=\"height:4px\" colspan=\"2\"><hr style=\"margin-top: 3px; margin-bottom: 3px\"></td></tr><tr><td><img src=\"" +img2 + "\" style=\"padding-right:5px\"></td><td align=\"left\">"+domain+"</td></tr>";
 }
 return "<span onClick=\"aClickElement(" +id+ ");\" style=\"position:relative; top:5px;\">"
 		+"<table border=0 cellpadding=0 cellspacing=0 width=\"100%\"><tr><td style=\"width:20px\"><img src=\"" + img1 + "\" style=\"padding-right:5px\"></td><td align=\"left\" valign=\"center\">" +name+st+"</td></tr>"+dt+ "</table></span>";
}

function getVersionsMarkup(id,name,summ,xpos,ypos,domain)
{
 return '<div class="applicationverbox" id="window'+id+'" name="av'+id+'" '
  + 'style="position: absolute; top: '+ypos+'px; left: '+xpos+'px ">'
  + getVersionsBody(id,name,summ,domain) + '</div>';
}

function SaveVersionsDetails(id)
{
 if (DontSave)
  return;
 
 var url="f=sv&a="+id;
 parent.$("#homeatts :input").each(function() {
  url=url+"&"+$(this).attr("id")+"="+$(this).val();
 });
 $.getJSON('UpdateAttrs',url,
     function(data){
   // Update the fragment window 
   console.log("Updating the window for #window"+id);
   $( ".applicationverbox" ).each(function( index ) {
      console.log(index + ": "+$(this).attr("id"));
   });
   $("#window"+id).html(getVersionsBody(id, data.title, data.summary, ''));
 });
}

function aClickElement(id)
{
 /*
 var att = parent.$("#homeatts");
 att.dialog($attvals)
 .empty()
 .dialog("option","title","Component Details")
 .dialog("option","buttons",
  [
        { text: "Ok",     click: function() { SaveVersionsDetails(id); att.dialog("close").dialog("destroy").hide(); } },
        { text: "Cancel", click: function() { att.dialog("close").dialog("destroy").hide(); } }
     ]
 );
 $(".applicationverbox").css("background-color","#eeeeef"); // turn off any other selected fragment
 $("#window"+id).css("background-color","yellow"); // highlight the one selected
 att.html("Loading...").load("getcompversum?verid="+id,cFrameEnabled(att));
 */
 CurrentlySelectedID=id;
  
}

function aOpenDetails(x)
{
 aClickElement(x.id.substr(6));
}



function aNewVersions(node)
{ 
 if (DontSave)
  return;
 
 var tx = (cCurrentMousePos.x) % 20;
 var ty = (cCurrentMousePos.y) % 20;
 if (tx>10) tx=-(20-tx);
 if (ty>10) ty=-(20-ty);
 
 var xpos = (cCurrentMousePos.x - tx) - 60;
 var ypos = (cCurrentMousePos.y - ty) - 90;

 var fanc = "f=anc&a=" + objid;

 $.getJSON("UpdateAttrs","f=nv&a=" + objid + "&xpos="+xpos+"&ypos="+ypos + "&isRelease=" + isRelease, function(data) {
  $('#innerversions').append(getVersionsMarkup(data.verid, data.vername, '', xpos, ypos, ''));

  //$("#window"+data.verid).dblclick(function() {
  //  aDrillDown($(this));
  //});
  
  aPlumb.draggable(aPlumb.getSelector(".applicationverbox"), {
   grid : [ 20, 20 ],
   containment: '#innerversions',
   cursor: "-webkit-grab",
   stop: aWindowMoved
  });
  
  aPlumb.addEndpoint("window"+data.verid, targetEndpoint, {
   anchor : "TopCenter",
   uuid : "window"+data.verid+"in"
  });
  aPlumb.addEndpoint("window"+data.verid, sourceEndpoint, {
   anchor : "BottomCenter",
   uuid : "window"+data.verid+"out"
  });
  
  console.log("Binding context menu menu1 to window #window"+data.verid);
  if (isRelease == "Y")
    $("#window"+data.verid).dmContextMenu(menuRelVersions, {theme:'xp'});
  else
   $("#window"+data.verid).dmContextMenu(menuVersions, {theme:'xp'});

 // $("#window"+data.verid).resizable({ handles: "se" });
  $("#window"+data.verid).resize(function(){
     aPlumb.repaintEverything();
  });
  // Need this configurable off user profile
  
  var swd = aFindWindowWithNoOutput(data.verid);
  console.log("swd.fuuid="+swd.fuuid);
  if (aPlumb.getEndpoint(swd.fuuid) != null)
  {
   console.log("Connecting to "+swd.fuuid);
   // The connect call will fire the new connection binding which will update the DB
   var connection = aPlumb.connect({uuids:[swd.fuuid, "window"+data.verid+"in"], editable:true});
  }
  
  aClickElement(data.verid);
  Refresh(currenttree);
 });
}

function aGotoApplication(node)
{
	SwitchDisplay($(node).attr("name"));
}

function aDeleteNode(node)
{
 console.log("aDelete");

 if (DontSave)
  return;
 
 // alert("Detaching all connections");
 aPlumb.detachAllConnections(node.id);
 // alert("Removing all endpoints");
 aPlumb.removeAllEndpoints(node.id);
 // alert("Detaching the node");
 $("#"+node.id).detach();
 var n = node.id.substr(6);
 
 if (isRelease)
  $(currenttree).jstree("delete_node", "#rv" + n);
 else
  $(currenttree).jstree("delete_node", "#av" + n);
 
  $.getJSON("UpdateAttrs","f=dv&a=" + objid + "&v="+n,
   function(data){
   console.log("in success");
  });
}

function aWindowMoved(event,ui)
{
 if (DontSave)
  return;
 
 console.log("in WindowMoved, ui.position.top="+ui.position.top);
 var windowid=event.target.id;
 var id = windowid.replace("window","");

  $.getJSON("UpdateAttrs","f=vm&a=" + objid + "&v="+id+"&xpos="+Math.round(ui.position.left)+"&ypos="+Math.round(ui.position.top),
   function(data){
    console.log("WindowMoved) in success");
  });
}


function aMenuDeleteConnector(conn)
{
 if (conn)
 {
  aPlumb.detach(conn); // This will call the "cDeleteConnector" event below
 }
}

function UpdateBranchLabel(BranchName,vid)
{
	console.log("UpdateBranchLabel - BranchName="+BranchName+" version="+vid);
	$.getJSON("UpdateAttrs","f=ubl&t=a&a="+vid+"&bn="+BranchName,
		function(data){
			console.log("UpdateBranchLabel) in success");
		}
	);
}

function aChangeLabelOK(conn)
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
	UpdateBranchLabel(labeltext,tn);
}

function aChangeLabel(conn)
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
	       { text: "Ok",     click: function() { aChangeLabelOK(conn); att.dialog("close"); } },
	       { text: "Cancel", click: function() { att.dialog("close"); } }
	    ]
	);
	att.html(
			"<html><body><table border=0><tr><td><img src='images/label-large.png'></td><td>Change Branch Label</td></tr></table><hr>" + 
			"Branch Label:&nbsp;<input type='text' id='labeltext' width='60' value='"+labelname+"'><br></body></html>");
	att.css("overflow-y","hidden");
	att.dialog("open");
}

function aAddLabel(conn)
{
	// var overlay = conn.getOverlay(conn.id+"_label");
	//if (overlay) overlay.setLabel("Branch x");
	//else conn.addOverlay([ "Label", { label: "Branch x", cssClass: "cLabel", location: 0.5, id: conn.id+"_label" }]);
	aChangeLabel(conn);
}

function aRemoveLabel(conn)
{
	console.log("Remove Label");
	conn.removeOverlay(conn.id+"_label");
	var FromUUID = conn.endpoints[0].getUuid();
	var tn;
	if (FromUUID.indexOf("out")>0) {
		tn = conn.targetId.replace("window","");
	} else {
		tn = conn.sourceId.replace("window",""); 
	}
	UpdateBranchLabel("",tn);
}

function aFindWindowWithNoOutput(winid)
{
 var swd = new Object();
 var pos = $("#window"+winid).position();
 var left = + Math.round(pos.left);
 var top = Math.round(pos.top);
 var minDistance = 0;
 swd.fuuid = "window" + objid + "out";
 swd.id = 0;
 console.log("GetNearestWindow winid="+winid+" left="+left+" top="+top);
 $(".applicationverbox").each(function() {
  var id = $(this).attr("id").substr(6);
  if (id != winid)
  {
   console.log("id = "+id);
   var p = aPlumb.getConnections({source: "window"+id},true);
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

function aDeleteConnector(conn)
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
  $.getJSON("UpdateAttrs","f=dd&a=" + objid + "&fn="+fn+"&tn="+tn,
  function(data){
   console.log("DeleteConnector) in success");
  });
 }
}


function aMouseOverLink(val)
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

function aMouseExitLink()
{
 OverLink=null;
}

function initVersions(connection)
{
 connection.bind("editCompleted", function(o)
 {
  if (typeof console != "undefined")
   console.log("connection edited. path is now ", o.path);
 });
};

function LoadVersionsData()
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
 var hasAppVersionAccess = false;
 var hasCreateVersionTask = false;
 
 console.log("appid="+objid);
 $.ajax({  
  url: "GetAppVersionAccess?appid="+objid,
  async: false,
  dataType : 'json'
}).done(function(data) 
  {
   console.log(data);
   hasAppVersionAccess = data.hasaccess;
   hasCreateVersionTask = data.createversion;
  });
 
 aPlumb.importDefaults({
  // default drag options
  DragOptions : {
   cursor : '-webkit-grab',
   zIndex : 2000
  }
 });
 
 aPlumb.bind("contextmenu", function(component, originalEvent) {
        originalEvent.preventDefault();
        return false;
    });
 
 
 function mainmenuversions(m,t)
 {
  var menudata = [];
  console.log("URL = GetMenu?t=APPLICATION&id="+objid+"&d=0&a=N&ct=applications");
  $.ajax({  
    url: "GetMenu?t=APPLICATION&id="+objid+"&d=0&a=N&ct=applications",
    async: false,
    cache: false,
    context: document.body
  }).done(function(data) {
   $.each(data,function(key,val){
    $.each(val,function(key2,val2){
     var tmo = {};
     console.log("key2="+key2+" val2="+val2);
     tmo[key2] = {
       onclick:function(menuItem,menu)
       {
        callback(currenttree,key2,val2,this);
       },
       icon:getIcon(val2)
      };
     menudata.push(tmo);
    });
   });
  }); 
  
  return menudata;
 };
 
 var menu2 = function(m,t){ 
  if (DontSave)
   return;
  
  console.log("in menu2 (contextmenu activated)");
  console.log("OverLink="+OverLink+" hasAppVersionAccess="+hasAppVersionAccess+" hasCreateVersionTask="+hasCreateVersionTask);
  if (!OverLink)
  {
	  console.log("Not overlink");
   if (hasAppVersionAccess) {
	    if (isRelease == "Y") {
	        return mainmenuRelVersions;
	    } else {
	    	console.log("Calling mainmenuversions");
	    	return mainmenuversions(m,t);
	    	//if (hasCreateVersionTask) {
	    	//	return mainmenuVersions;
	    	//} else return [];
	    }
   }
   else
    return [];
  }
  // Construct a menu for this connector based on its content
  var ma = [];
  ma.push({'Delete this Connection': {icon:"images/delete.png",onclick:function(menuItem,menu){aMenuDeleteConnector(LastLink);} }});
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
		ma.push({'Remove Branch Label':     {icon:"images/delete.png",onclick:function(menuItem,menu){aRemoveLabel(LastLink);} }});
	}
	ma.push({'Change Branch Label':     {icon:"images/label.png",onclick:function(menuItem,menu){aChangeLabel(LastLink);} }});
  } else {
	ma.push({'Add Branch Label':        {icon:"images/label.png",onclick:function(menuItem,menu){aAddLabel(LastLink);} }});
  }
  return ma;
  };
 
 console.log("Binding default context menu");
 
 try {
 $("#innerversions").dmContextMenu(menu2, {theme:'xp'});
 } catch(e) { console.log(e); }
 
 
 aPlumb.bind("jsPlumbConnection", function(connInfo, originalEvent) {
  console.log("jsPlumbConnection fired - about to call init");
  initVersions(connInfo.connection);
  console.log("init called");

  connInfo.connection.bind("mouseenter",function(c) {aMouseOverLink(connInfo.connection);});
  connInfo.connection.bind("mouseexit",function(c) {aMouseExitLink();});

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
   console.log("Updating DB with appid=" + objid + " fn="+fn+" tn="+tn);

   $.getJSON("UpdateAttrs","f=ad&a=" + objid + "&fn="+fn+"&tn="+tn,
   function(data){
    console.log("updateattrs) in success");
   });
  }

 });
 
 aPlumb.bind("connectionDetached", function(connInfo, originalEvent) {
  console.log("DETACHED! connInfo.connection.id = " + connInfo.connection.id);
  aDeleteConnector(connInfo.connection);
 });

 
 $.getJSON('GetAppVersionLayout',"appid=" + objid,function(data){
  
  // Start Window
  yo = 140;
  var w = Math.floor($("#innerversions").width()/2)-63;
  
  $("#innerversions").append( "<div class=\"applicationverbox\" id=\"window" + objid + "\" " +
    "style=\"position: absolute; top: 0px; left: " + w + "px \">" +
    getVersionsBody(objid,objName,data.BaseSummary,data.BaseDomain));
  console.log("(1) Adding endpoint to BottomCenter for window"+objid+"out (app base)");
  aPlumb.addEndpoint("window" + objid, appverEndpoint, {
   anchor : "BottomCenter",
   uuid : "window" + objid + "out"
  });

  var MaxWinID=0;
  //
  // insert the components
  //
  console.log("in insert block");
  for (a=0;a<data.NodeCount;a++)
  {
   WindowID = parseInt(data.Nodes[a].nodeid);
   if (WindowID > MaxWinID) MaxWinID = WindowID;
   $("#innerversions").append(getVersionsMarkup(WindowID, data.Nodes[a].name, data.Nodes[a].summary, data.Nodes[a].xpos, data.Nodes[a].ypos, data.Nodes[a].domain));
   //$("#window"+WindowID).dblclick(function() {
   //  aDrillDown($(this));
   //});
   
   aPlumb.addEndpoint("window"+WindowID, targetEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+WindowID+"in"
   });
   console.log("(2) Adding endpoint to BottomCenter for window"+WindowID);
   aPlumb.addEndpoint("window"+WindowID, appverEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+WindowID+"out",
    maxConnections: -1
   });
 
   console.log("Binding context menu menu1 to window #window"+WindowID);
   if (isRelease == "Y")
    $("#window"+WindowID).dmContextMenu(menuRelVersions, {theme:'xp'});
   else
    $("#window"+WindowID).dmContextMenu(menuVersions, {theme:'xp'});

   $("#window"+WindowID).resize(function(){
      aPlumb.repaintEverything();
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
   console.log("data.Links["+a+"].branch="+data.Links[a].branch);
   var srcconn;
   if (data.Links[a].nodefrom == 0) {
    srcconn = "window" + objid + "out"; 
   } else {
    srcconn = "window" + data.Links[a].nodefrom + "out";
   }
   var tgtconn = "window" + data.Links[a].nodeto + "in";
   
   console.log("Connect srcconn="+srcconn+" tgtconn="+tgtconn);
   
   if (aPlumb.getEndpoint(srcconn) != null && aPlumb.getEndpoint(tgtconn) != null)
   {
    var connection = aPlumb.connect({uuids:[srcconn,tgtconn], detachable:true, editable:true});
    if (data.Links[a].branch.length>0) {
		var overlay = connection.getOverlay(connection.id+"_label");
		if (overlay == null) {
			connection.addOverlay([ "Label", { label: data.Links[a].branch, cssClass: "cLabel", location: 0.5, id: connection.id+"_label" }]);
		}
	}
   }
  }

  aPlumb.draggable(aPlumb.getSelector(".applicationverbox"), {
   grid : [ 20, 20 ],
   containment: '#innerversions',
   cursor: "-webkit-grab",
   stop: aWindowMoved
  });
  
  InitialLoad = false;

 }).fail(function(fdata){
  console.log("Something has gone horribly wrong "+fdata);
  $.each(fdata,function(key,val){
   console.log("fdata) key="+key+" val="+val);
  });
  });
 $("#innerversions").resizable({ handles: "s" });
 
 if (readonly)
  $("#innerversions").block({ message: null });
 else
  $("#innerversions").unblock();
 
 DontSave = 0;
}

$(document).mousemove(function(event) {
    cCurrentMousePos.x = event.pageX;
    cCurrentMousePos.y = event.pageY;
});


function aStartMoved(event,ui)
{
 if (DontSave)
  return;
 
 console.log("Start Moved ypos="+Math.round(ui.offset.left));
 $.getJSON("UpdateAttrs","f=vm&a=" + objid + "&v="+id+ "&xpos="+Math.round(ui.offset.left),
 function(data){
  console.log("WindowMoved) in success");
 });
}


