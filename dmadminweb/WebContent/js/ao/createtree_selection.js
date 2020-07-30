/*
 *
 *  Ortelius for Microservice Configuration Mapping
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

var lastobj_selection;
var lastsel_selection;
var lastdomain_selection;

function eventSelectNode_selection(frameid,readonly,data)
{
	var NodeType = data.inst.get_json()[0].attr.rel;
	console.log("eventSelectNode=");
	console.log(data.inst);

 
	if (lastSelectedNode_selection == data.inst.get_json()[0].attr.id) {
		console.log("returning early");
		return;
	}
	 
	console.log("here");
	
	lastSelectedNode_selection = data.inst.get_json()[0].attr.id;
	
	objid_selection = data.inst.get_json()[0].attr.id.substr(2);	//OTID, first two chars are Object Type
	objtype_selection = data.inst.get_json()[0].attr.id.substr(0,2);
	objkind_selection="";
	console.log("objid="+objid);
	// Procedures and functions have their "Kind" in their OTID (pr-kk-
	if (objtype == "pr" || objtype == "fn") {
		  objkind=objid.substr(objid.indexOf("-")+1);
		  objid = objid.substr(0,objid.indexOf("-"));
	}
	objtypeAsInt_selection = obj2Int[objtype][0];
 objtypeName_selection = obj2Int[objtype][1];	
 objName_selection = data.inst.get_json()[0].data;
 
// $("#" + lastSelectedNode_selection).addClass("compsource ui-draggable droppable drop");
 console.log(lastSelectedNode_selection);
 console.log(data.inst.get_json()[0].attr);
 console.log("id="+objid+" objtype="+objtype_selection + " objtypeAsInt=" + objtypeAsInt_selection + " objtypeName=" + objtypeName_selection + " objName=" + objName_selection + " objkind=" + objkind_selection + " currenttree=" + data.inst.get_container().attr("id"));
 }



function CreateTree_Selection(nodename,typestr,readonly,frameid)
{
 console.log("selobj=" + lastSelectedNode);
 
 // var myparent = $.jstree._reference(currenttree)._get_parent("#" + lastSelectedNode);

 console.log("Creating tree " + nodename + "," + typestr + "," + readonly + "," + frameid);
 
	var BaseNode=0;
	var plugins = ["themes","json_data","ui","types", "cookies"];
	current_tree_selection = $(nodename);
	
	if (nodename == "#allocated" || nodename == "#addenv2app" || nodename == "#addapp" || nodename == "#addsrv2env" || nodename == "#addcomp2srv")
	 plugins.push("checkbox");
	
	if (!readonly) {
		if (nodename == "#componentlist" || nodename == "#applicationversionlist" || nodename == "#fragmentlist") {
			plugins.push("dnd");
			$(document).unbind("drag_start.vakata");
			$(document).unbind("drag_stop.vakata");
			$(document).bind("drag_start.vakata", function (e, data) {
				  if(data.data.jstree) {
				    console.log("Drag start");
				    Dragging=true;
				  }
				}); 
			$(document).bind("drag_stop.vakata", function (e, data) {
				  if(data.data.jstree) {
				    console.log("Drag end");
				    Dragging=false;
				  }
				});
		}
	}

	console.log("GetDomainHierarchy?typestr="+typestr+"&domainid=0");
	
	

	 current_tree_selection.jstree({
   "themes" : {
    "theme" : "apple",
    "dots" : false,
    "icons" : true
   },
	  "json_data" : {
	   "data" : GetDomainDataHierarchy(typestr), // Initial data, including any opened nodes
	   "ajax" : {
	    "url" : function( node ){

	     if( node == -1 )
	     {
	      BaseNode=1;
	      console.log("GetDomainContent?typestr="+typestr+"&domainid=0");
	      return "GetDomainContent?typestr="+typestr+"&domainid=0";
	     }
	     else
	     {
	      BaseNode=0;
	      // node id is OTID - first two chars are type, remainder is id
	      var objtype = node[0].id.substr(0,2);
	      var id = node[0].id.substr(2);
	      var objkind="";
	      if (objtype == "pr" || objtype == "fn") {
	       objkind=id.substr(id.indexOf("-")+1);
	       id = id.substr(0,id.indexOf("-")-1);
	      }

	      console.log("before, id="+id+" objtype="+objtype);
       var catdomain = 1;
       if (objtype == "cy" || objtype == "cp" || objtype == "cf" || objtype == "cc") {
    	   if (id.indexOf("-")>=0) {
    		   catdomain=id.substr(id.indexOf("-")+1);
    		   id = id.substr(0,id.indexOf("-"));
    	   }
       }
       
       console.log("id="+id+" objtype="+objtype+" objkind="+objkind + " catdomain=" + catdomain);

	      if (objtype == "do") {
	       console.log("Calling GetDomainContent?typestr="+typestr+"&domainid="+id);
	       return "GetDomainContent?typestr="+typestr+"&domainid="+id;
	      } else if (objtype == "en") {
	       console.log("Calling GetEnvironmentContent?envid="+id);
	       return "GetEnvironmentContent?envid="+id;
	      } else if (objtype == "se") {
	       return "GetServerContent?serverid="+id;
	      } else if (objtype == "ap") {
	       return "GetApplicationContent?appid="+id;
	      } else if (objtype == "rl") {
	       return "GetApplicationContent?appid="+id + "&isRelease=y"; 
	      } else if (objtype == "no") {
	       return "GetNotifierTemplates?notifierid="+id;
	      } else if (objtype == "co") {
	       return "GetComponentContent?compid="+id;
       } else if (objtype == "cy") {
        return "GetFragmentContent?domainid="+catdomain + "&catid=" + id + "&objtype=" + objtype;
       } else if (objtype == "cp") {
        return "GetFragmentContent?domainid="+catdomain + "&catid=" + id + "&objtype=" + objtype; 
       } else if (objtype == "cf") {
        return "GetFragmentContent?domainid="+catdomain + "&catid=" + id + "&objtype=" + objtype;  
       } else if (objtype == "cc") {
    	   console.log("cc >>> id="+id);
         return "GetCategoryContent?domainid="+catdomain + "&catid=" + id + "&objtype=" + objtype;
	      } else {	       
	       console.log("objtype=" + objtype + "; id=" + id);                        
	      }
	     }
	    },
	    "type" : "get",  // this is a GET not a POST
	    "cache" : false
	   }
	  },
	  "types" : {
	   // I set both options to -2, as I do not need depth and children count checking
	   // Those two checks may slow jstree a lot, so use only when needed
	   "max_depth" : -2,
	   "max_children" : -2,
	   // I want only `domain` nodes to be root nodes 
	   // This will prevent moving or creating any other type as a root node
	   "valid_children" : [ "Domain" ],
	   "types" : {
	    "Environment" : {
	     "icon" : {
	      "image" : "css/images/environments_sm.png"
	     }
	    },
	    "Repository" : {
	     "icon" : {
	      "image" : "images/repository.png"
	     }
	    },
	    "Component" : {
	     "icon" : {
	      "image" : "css/images/components_sm.png"
	     }
	    },
	    "Credentials" : {
	     "icon" : {
	      "image" : "images/credential.png"
	     }
	    },
	    "Domain" : {
	     "icon" : {
	      "image" : "css/images/domains_sm.png"
	     }
	    },
	    "Lifecycle" : {
	     "icon" : {
	      "image" : "css/images/lifecycles_sm.png"
	     }
	    },
	    "Server" : {
	     "icon" : {
	      "image" : "images/server.png"
	     }
	    },
	    "Release" : {
	     "icon" : {
	      "image" : "css/images/applications_sm.png"
	     }
	    },
	    "Application" : {
	     "icon" : {
	      "image" : "css/images/applications_sm.png"
	     }
	    },
	    "AppVersion" : {
	     "icon" : {
	      "image" : "css/images/appversions_sm.png"
	     }
	    },
	    "RelVersion" : {
	     "icon" : {
	      "image" : "css/images/appversions_sm.png"
	     }
	    },             
	    "CompVersion" : {
	     "icon" : {
	      "image" : "css/images/components_sm.png"
	     }
	    },
	    "Action" : {
	     "icon" : {
	      "image" : "images/action.png"
	     }
	    },
	    "Procedure" : {
	     "icon" : {
	      "image" : "images/file.png"
	     }
	    },
	    "Fragment" : {
	     "icon" : {
	      "image" : "css/images/script.png"
	     }
	    },
	    "Fragmentproc" : {
		     "icon" : {
		      "image" : "css/images/proc.png"
		      }
		},
		"Fragmentfunc" : {
			 "icon" : {
			 "image" : "css/images/functions_procedures.png"
			 }
		},    
     "Category" : {
      "icon" : {
       "image" : "css/images/tag-light.svg"
      }
     },	    
	    "Function" : {
	     "icon" : {
	      "image" : "images/file.png"
	     }
	    },              
	    "Datasource" : {
	     "icon" : {
	      "image" : "images/datasource.png"
	     }
	    },
	    "Notify" : {
	     "icon" : {
	      "image" : "images/notify.png"
	     }
	    },
	    "Template" : {
	     "icon" : {
	      "image" : "images/template.png"
	     }
	    },
	    "User" : {
	     "icon" : {
	      "image" : "css/images/users_sm.png"
	     }
	    },
	    "Usergroup" : {
	     "icon" : {
	      "image" : "css/images/groups_sm.png"
	     }
	    }
	   }
	  },
	  "dnd" : {
	   "drop_target" : "#innercomp, #innerappver, #innerworkflow, .componentbox",
	   "drop_finish" : function (data) { console.log(data); DropFromTree(data); return true; }
	  },
	  "core" : {"html_titles" : true,  "load_open" : true, "animation": 50 },
	  "plugins" : plugins,
	  "checkbox": {
	   two_state: true
	  },
	  "ui": {"select_limit" : 1},
	  "cookies" : {
	   "save_opened"    : "jstree_open_"+nodename.substr(5),
	   "save_selected"  : "jstree_selected_"+nodename.substr(5),
	   "save_loaded"    : "jstree_load_"+nodename.substr(5),
	   "cookie_options" : { "path" : "/" }
	  }
	 }); 

      
      $(nodename).bind("hover_node.jstree", function (e, data) 
        {
         lastobj = data.rslt.obj;
        });
      
      
      $(nodename).bind("select_node.jstree", function (e, data) 
      {
       var parent=data.inst._get_parent(data.rslt.obj);
       
       if (parent != -1)
       {
           while (parent && !("attr" in parent) && parent.attr("rel")!="Domain" && parent.attr("rel")!="Lifecycle")
           {
            parent=data.inst._get_parent(parent);
           }
          lastdomain_selection=parent.attr("id").substr(2); // OTID
       }
       else
       {
        lastdomain_selection=1; // Global
       }
      }); 
      
     
      $(nodename).bind("select_node.jstree", function(evt, data){

 			  eventSelectNode_selection(frameid,readonly,data);
      });
      
      $(nodename).bind("open_node.jstree close_node.jstree", function (evt, data) {
    	    var state = evt.type == "open_node" ? "open" : "closed";
    	    console.log("type="+data.rslt.obj.attr("rel")+" id="+data.rslt.obj.attr("id")+" "+state);
      });     
      
      $(nodename).bind("load_node.jstree", function (evt,data) {
    	 var $nodes = data.rslt.obj
         if( $nodes == -1 ){
        	 // Loaded root node
             $nodes = $(nodename).find('ul:first-child > li');
         }
    	 $nodes.each(function(a) {
    		 var rel = $(this).attr("rel");
    		 console.log("id="+$(this).attr("id")+" rel="+rel);
    		 if (rel.toLowerCase() == "domain" || rel.toLowerCase() == "category") {
    			 $(this).find(".jstree-checkbox").first().hide();	// remove checkbox from root node
    			 $(this).find("li").each(function (i2) {
	        		 var rel = $(this).attr("rel");
	        		 if (rel.toLowerCase() == "lifecycle" || rel.toLowerCase() == "domain" || rel.toLowerCase() == "category") {
	        			 $cbs = $(this).find(".jstree-checkbox").first().hide();
	        		 }
	        	 });
    		 }
    	 });
      });
}

function GetDomainDataHierarchy(typestr)
{
 var res=null;
 if (domlist[0] == "1" && domlist.length > 1)
   lastdomain = domlist[1];
 else
   lastdomain = domlist[0];
 
 console.log("GetDomainData("+typestr+")=" +lastdomain);
 var url;
 if (typestr == "components" || typestr == "fragments") {
	 url="GetDomainContent?typestr="+typestr+"&domainid=0&hierarchy=" + lastdomain;
 } else {
	 url="GetDomainContent?typestr="+typestr+"&domainid=0";
 }
 $.ajax({
    dataType: "json",
    url: url,
    async: false
  }).done(function( data ) {
    console.log(data);
    res=data;
  });
 return res;
}

function CheckTarget(data)
{
	console.log("CheckTarget target="+data.r[0].id);
}

function ReplaceComponentVersion(data)
{
	target = data.r;
	var newid = data.o[0].id.substr(2); 
	var oldid = parseInt(target[0].id.substr(6));
	var fromlist = [];
	var tolist = [];

	var p = cPlumb.getConnections({source: "startwindow"},true);
	for (var i=0;i<p.length;i++) {
		if (p[i].targetId.substr(6)==oldid) {
			console.log("Detaching FROM #startwindow");
			cPlumb.detach(p[i]);
			fromlist.push(0);
		}
	}
	$(".componentbox").each(function() {
		var id = $(this).attr("id").substr(6);
		console.log("looking at window id = "+id+" oldid="+oldid);
		if (id != oldid) {
			var p = cPlumb.getConnections({source: "window"+id},true);
			for (var i=0;i<p.length;i++) {
				console.log("From "+id+" to "+p[i].targetId.substr(6));
				if (p[i].targetId.substr(6)==oldid) {
					console.log("Detaching FROM #window"+id);
					cPlumb.detach(p[i]);
					fromlist.push(id);
				}
			}
		}
	});
  
	var p = cPlumb.getConnections({source: "window"+oldid},true);
	for (var i=0;i<p.length;i++) {
		console.log("Detaching flow TO: #"+p[i].targetId);
		cPlumb.detach(p[i]);
		tolist.push(p[i].targetId.substr(6));
	}
	// Now remove the endpoints
	console.log("Removing endpoint window"+oldid+"in");
	cPlumb.deleteEndpoint("window"+oldid+"in");
	console.log("Removing endpoint window"+oldid+"out");
	cPlumb.deleteEndpoint("window"+oldid+"out");
	//
	// Change the window id
	//
	console.log("Changing #window"+oldid+" to #window"+newid)
	$("#window"+oldid).attr("id","window"+newid);
	//
	// Restore the endpoints on the new window id
	//
	console.log("Restoring endpoints");
	cPlumb.addEndpoint("window"+newid, compEndpoint, {
		anchor : "TopCenter",
		uuid : "window"+newid+"in"
	});
	cPlumb.addEndpoint("window"+newid, compEndpoint, {
		anchor : "BottomCenter",
		uuid : "window"+newid+"out"
	});
	// And restore the connections
	for (var i=0;i<fromlist.length;i++) {
		if (fromlist[i]==0) {
			cPlumb.connect({uuids:["startwindowout", "window"+newid+"in"], editable:true});
		} else {
			console.log("Restoring connection from "+fromlist[i]+" to "+newid);
			cPlumb.connect({uuids:["window"+fromlist[i]+"out", "window"+newid+"in"], editable:true});
		}
	}
	for (var i=0;i<tolist.length;i++) {
		console.log("Restoring connection from "+newid+" to "+tolist[i]);
		cPlumb.connect({uuids:["window"+newid+"out", "window"+tolist[i]+"in"], editable:true});
	}
	$("#window"+oldid).css("cursor","default");
	console.log("Updating component id "+oldid+" to newid "+newid);
	$.getJSON("UpdateAttrs","f=ccv&a=" + objid + "&c="+oldid+"&nc="+newid,
	function(data){
		console.log("in success - changing inner text");
		$("#window"+newid).html(getComponentBody(newid, data.prefix, data.title, data.summary, ''));
		$("#window"+newid).attr("name",data.prefix+newid);
	});
	console.log("done swap");
}

function DropFromTree(data)
{
 
  if (DontSave)
   return;
  
  event = data.e;
  target = data.r;
  var droppedobj = $("#" + data.o[0].id);
  var windowid = data.o[0].id.substr(2); 
  var droptype = data.o[0].id.substr(0,2);
  save_fragment_id = data.o[0].id.substr(2);
  
  if ((droptype=="co" || droptype=="cv") && target[0].id.substr(0,6)=="window") {
   var n = parseInt(target[0].id.substr(6));
   ReplaceComponentVersion(data);
   return;
  }
  
  var tx = (event.pageX) % 20;
  var ty = (event.pageY) % 20;
  if (tx>10) tx=-(20-tx);
  if (ty>10) ty=-(20-ty);
  var xpos = Math.round(event.offsetX);
  var ypos = Math.round(event.offsetY);
  
  // Components
  
  console.log("droptype="+droptype);
  
  if (droptype == "co" || droptype == "cv") {
   DontSave = true;
   
   if ($.inArray("window" + windowid,usedComponents) >= 0) {
    alert(droppedobj.text() + " is already defined to the Application Version");
    DontSave=false;
    return;
   }

   usedComponents.push("window" + windowid);

   $("#innercomp").append(getComponentMarkup(windowid, droptype, droppedobj.text(), '', xpos, ypos, ''));
   $("#window"+windowid).dblclick(function() {
    // cDrillDown($(this));
   });
   $("#window"+windowid).droppable(componentDropOptions);
   $("#window"+windowid).droppable("option","accept", $(".compsource, .compversource") );
   $("#window"+windowid).hover(function() {
    if (Dragging) $(this).addClass("ui-state-highlight");
   },
   function() {
    $(this).removeClass("ui-state-highlight");
   });

   cPlumb.draggable(cPlumb.getSelector(".componentbox"), {
    grid : [ 20, 20 ],
    containment: '#innercomp',
    cursor: "-webkit-grab",
    stop: cWindowMoved
   });
   try {
    $('#window'+windowid).dmContextMenu(menu1, {theme:'xp'});
   } catch(e) { console.log(e); }

   // apps have connectors between the components to indicate deployment order
   cPlumb.addEndpoint("window"+windowid, compEndpoint, {
    anchor : "TopCenter",
    uuid : "window"+windowid+"in"
   });
   cPlumb.addEndpoint("window"+windowid, compEndpoint, {
    anchor : "BottomCenter",
    uuid : "window"+windowid+"out"
   });

   $.getJSON("UpdateAttrs","f=acvm&c="+windowid+"&a=" + objid + "&xpos="+xpos+"&ypos="+ypos,
  function(data) {
    // Updated DB - safe to add link
    var swd = FindWindowWithNoOutput(windowid);
    console.log("swd.fuuid="+swd.fuuid);
    if (cPlumb.getEndpoint(swd.fuuid) != null) {
     console.log("Connecting to "+swd.fuuid);
     // The connect call will fire the new connection binding which will update the DB
     var connection = cPlumb.connect({uuids:[swd.fuuid, "window"+windowid+"in"], editable:true});
     DontSave = false;
    }
   });
  }
  else if (droptype == "ap" || droptype == "av") {
   // Applications in a Release
   DontSave = true;
   if ($.inArray("window" + windowid,usedApplicationVersions) >= 0) {
    alert(droppedobj.text()  + " is already defined to the Release");
    DontSave=false;
    return;
   }
     
   usedApplicationVersions.push("window" + windowid);
   
   $("#innerappver").append(getApplicationVersionMarkup(windowid, droptype, droppedobj.text(), '', xpos, ypos, ''));
   $("#window"+windowid).dblclick(function() {
    avDrillDown($(this));
   });
   $("#window"+windowid).droppable(applicationversionDropOptions);
   $("#window"+windowid).droppable("option","accept", $(".appsource, .appversource") );
   $("#window"+windowid).hover(function() {
    if (Dragging) $(this).addClass("ui-state-highlight");
   },
   function() {
    $(this).removeClass("ui-state-highlight");
   });
   
   avPlumb.draggable(avPlumb.getSelector(".applicationbox"), {
    grid : [ 20, 20 ],
    containment: '#innerappver',
    cursor: "-webkit-grab",
    stop: avWindowMoved
   });
   try {
    $('#window'+windowid).dmContextMenu(menuContextApplicationVersion, {theme:'xp'});
   } catch(e) { console.log(e); }
   
   // Releases have connectors between the applications to indicate deployment order
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
    if (avPlumb.getEndpoint(swd.fuuid) != null) {
     console.log("Connecting to "+swd.fuuid);
     // The connect call will fire the new connection binding which will update the DB
     var connection = avPlumb.connect({uuids:[swd.fuuid, "window"+windowid+"in"], editable:true});
     DontSave=false;
    }
   });   
  }
  else if (droptype == "fg")  // Fragment
  {
   DontSave = false;
   windowid = parseInt(MaxWinID)+1;
   MaxWinID=windowid;

   $.getJSON("UpdateAttrs","f=an&a=" + objid + "&w="+windowid+"&pw=" + parentwindowid + "&xpos="+xpos+"&ypos="+ypos+"&typeid="+save_fragment_id,
       function(data) {
    // Updated DB - safe to paste node and add link
    console.log("added, otid="+data.otid+" droppedobj.text()="+droppedobj.text());
    console.log(data);
    $("#innerworkflow").append(getWorkflowMarkup(windowid,data.drilldown,data.otid, droppedobj.text(), droppedobj.text(), xpos, ypos, ''));
    $("#window"+windowid).dblclick(function() {
     wfDrillDown($(this));
    });
    $("#window"+windowid).droppable(workflowDropOptions);
    $("#window"+windowid).droppable("option","accept", $(".fragmentsource") );
    $("#window"+windowid).hover(function() {
     if (Dragging) $(this).addClass("ui-state-highlight");
     },
     function() {
      $(this).removeClass("ui-state-highlight");
     }
    );
    
    jsPlumb.draggable(jsPlumb.getSelector(".fragment, .ddfragment"), {
     grid : [ 20, 20 ],
     containment: '#innerworkflow',
     cursor: "-webkit-grab",
     stop: wfWindowMoved
    });
    
    
    // apps have connectors between the components to indicate deployment order
    jsPlumb.addEndpoint("window"+windowid, targetEndpoint, {
      anchor : "TopCenter",
      uuid : "window"+windowid+"in"
     });
    
    if (data.exitpoints == 1)
    {
     console.log("Adding endpoint window"+windowid+"out");
     jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
      anchor : "BottomCenter",
      uuid : "window"+windowid+"out"
     });
    }
    else
    if (data.exitpoints == 2)
    {
     console.log("style="+data.style);
     if (data.style==1) {
       // If Block 
       console.log("Adding endpoint window"+windowid+"leftout");
       jsPlumb.addEndpoint("window"+windowid, sourceEndpointLeft, {
        anchor : "LeftMiddle",
        uuid : "window"+windowid+"leftout"
       });
       console.log("Adding endpoint window"+windowid+"rightout");
       jsPlumb.addEndpoint("window"+windowid, sourceEndpointRight, {
        anchor : "RightMiddle",
        uuid : "window"+windowid+"rightout"
       });
     } else {
       // All other blocks with two outputs (parallel etc)
      console.log("Adding endpoint window"+windowid+"leftout");
      jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
        anchor : "LeftMiddle",
        uuid : "window"+windowid+"leftout"
      });
      console.log("Adding endpoint window"+windowid+"rightout");
       jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
        anchor : "RightMiddle",
        uuid : "window"+windowid+"rightout"
      });
     }
    }
    else
    if (data.exitpoints == 3)
    {
     console.log("Adding endpoint window"+windowid+"leftout");
     jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
      anchor : "LeftMiddle",
      uuid : "window"+windowid+"leftout"
     });
     console.log("Adding endpoint window"+windowid+"rightout");
     jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
      anchor : "RightMiddle",
      uuid : "window"+windowid+"rightout"
     });
     console.log("Adding endpoint window"+windowid+"out");
     jsPlumb.addEndpoint("window"+windowid, sourceEndpoint, {
      anchor : "BottomCenter",
      uuid : "window"+windowid+"out"
     });
    }

    console.log("ADDED NODE");
    console.log(data);
    var swd = wfGetNearestWindow(windowid);
    if (jsPlumb.getEndpoint(swd.fuuid) != null) {
     console.log("Connecting to "+swd.fuuid);
     connection = jsPlumb.connect({uuids:[swd.fuuid, "window"+windowid+"in"], editable:true});
     if (connection != null) {
      if (swd.id > 0) {
       if (Fragments[swd.id]) Fragments[swd.id].outputs++;
      }
      $.getJSON("UpdateAttrs","f=af&a=" + objid + "&fn="+swd.id+"&tn="+windowid+"&pos="+swd.pos,
        function(data){
       console.log("in success");
      });
     }
     if (typeof Fragments[windowid] === 'undefined' || Fragments[windowid] == null) {
      Fragments[windowid]={outputs:0,otid:data.otid,type:save_fragment_id,attributes:{"ExitPoints":data.exitpoints}};
     } else {
      Fragments[windowid].type = save_fragment_id;
     }
     try {
      console.log("otid="+data.otid);
      console.log("type="+data.otid.substr(0,2));
      if (data.otid.substr(0,2)=="pr") {
       $('#window'+windowid).dmContextMenu(menu1WorkflowProc, {theme:'xp'});
      } else if (data.otid.substr(0,2)=="fn") {
       $('#window'+windowid).dmContextMenu(menu1WorkflowFunction, {theme:'xp'});
      } else {
       $('#window'+windowid).dmContextMenu(menu1Workflow, {theme:'xp'});
      }
     } catch(e) { console.log(e); }
     console.log("Calling UpdateAttrs, f=af");
     

    }
    var x = {id: "window"+windowid};
    wfOpenDetails(x,true);
   });   
  }
}