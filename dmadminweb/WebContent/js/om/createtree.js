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
var ExplorerRefreshing = false;
var lastobj;
var lastsel;
var lastdomain;

var selid=null;
var HighlightNode=null;
var inTree=false;
var dc;
var globalTaskID;
var apdl = [];

var targetmenu = function(m,t)
{
 console.log("TARGETMENU = " + ad_currenttree);
 var ad = document.getElementsByName(ad_currenttree);
 $(currenttree).jstree("deselect_all");
 $(currenttree).jstree("select_node", lastobj);
 var menudata = [];
 var ts = $(currenttree).attr("id");
 ts = ts.substring(0,ts.length-5);
 console.log(ts);
 $.ajax({  
   url: "GetMenu?t="+lastobj.attr("rel")+"&id="+lastobj.attr("id").substr(2)+"&d="+lastdomain+"&a="+isAdmin+"&ct="+ts,
   async: false,
   cache: false,
   context: document.body
 }).done(function(data) {
  $.each(data,function(key,val){
   $.each(val,function(key2,val2){
    var tmo = {};
    console.log("key2="+key2+" val2="+val2);
    tmo[key2] = {onclick:function(menuItem,menu){callback(currenttree,key2,val2,this);},icon:getIcon(val2)};
    menudata.push(tmo);
   });
  });
 }); 
 
 return menudata;
};


function NewID(ot,rt,treeid,at,name,catid,kindid)
{
	var tid = treeid.substring(1);
	var url;
	if (typeof catid != "undefined") {
		if (typeof kindid != "undefined") {
			url = "GetNewID?objtype="+ot+"&pid="+lastsel.attr("id").substr(2)+"&ptype="+lastsel.attr("rel")+"&at="+at+"&name="+name + "&treeid="+tid+"&catid="+catid+"&kindid="+kindid.substr(2);
		} else {
			url = "GetNewID?objtype="+ot+"&pid="+lastsel.attr("id").substr(2)+"&ptype="+lastsel.attr("rel")+"&at="+at+"&name="+name + "&treeid="+tid+"&catid="+catid;
		}
	} else { 
		url = "GetNewID?objtype="+ot+"&pid="+lastsel.attr("id").substr(2)+"&ptype="+lastsel.attr("rel")+"&at="+at+"&name="+name + "&treeid="+tid;
	}
	console.log(url);
	var t=$(treeid);
	if (lastSelectedNode != "") {
		t.jstree('deselect_node',"#" + lastSelectedNode);
	}
 
	$.ajax({
		url: url,
		dataType: 'json',
		async: false,
		success: function(data) {
			console.log(data);
			if (data.result  == false) {
				alert(data.error);
				return;
			}
			cnc=null;
			if (typeof data.newcat != "undefined" && data.newcat) {
				// Need to create a new category in the tree - create_node binding will fire and take over...
				cnc={"data": data};
				t.jstree("create", lastsel, "inside", { "data": data.catname, attr: {"id": data.category, "rel": "Category"}},false,true);
			} else {
				// Not creating new category node
				if (typeof data.category != "undefined") {
					// Creating new object in an existing category
					SwitchDisplay(data.id);
				} else {
					// Standard Creation (Directly in Domain)
					t.jstree("create", lastsel, "inside",  { "data" : data.name, attr: {"id": data.id, "rel": data.rt}},false,true);
					t.jstree('select_node',"#"+data.id);
				}
			}
			
   
			if (data.id.indexOf("co") == 0) {
				currentSubTabsSelection["#components_tree"][0] = "tabs-ComponentItems"; 
				ActivateSubTabs();
			}
   
			summSaveobjid = data.id.substring(2);
			objid = summSaveobjid;
			if (summSaveobjid.indexOf("-") > -1) {
				summSaveobjid=summSaveobjid.substr(0,summSaveobjid.indexOf("-"));
			}
			console.log("summSaveobjid="+summSaveobjid);
		}
	});
}
 
function NewDomain(tv)
{
 summSavedomid = lastdomain;	
 NewID("Domain","Domain",currenttree,"");
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewEnvironment(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 2;
 objtype = "en";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewServer(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 4;
 objtype = "se";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs(); 
}

function NewComponent(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 13;
 objtype = "co";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewCompType(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 67;
 objtype = "ct";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewCredential(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 19;
 objtype = "cr";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs(); 
}

function NewRepository(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 8;
 objtype = "re";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewGroup(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 7;
 objtype = "gr";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewApp(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 5;
 objtype = "ap";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewBuilder(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 24;
 objtype = "be";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewRelease(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 21;
 objtype = "rl";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewAction(tv,at)
{
 summSavedomid = lastdomain;
 switch(at) {
 case "G":
  {
   objtypeAsInt = 11;
   objtype = "ac";
   objid = -1;
   currentSubTabsSelection[currenttree][0] = "tabs-General";
   ActivateSubTabs();
  }
  break;
 
 case "F": 
  {
   objtypeAsInt = 91;
   objtype = "fn";
   objid = -1;
   currentSubTabsSelection[currenttree][0] = "tabs-General";
   ActivateSubTabs();
  }
  break;
  
 default:
  {
   objtypeAsInt = 90;
   objtype = "pr";
   objid = -1;
   currentSubTabsSelection[currenttree][0] = "tabs-General";
   ActivateSubTabs();
  }
  break;
 }
}

function NewUser(tv,at)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 6;
 objtype = "us";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewDatasource(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 10;
 objtype = "ds";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewNotifier(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 9;
 objtype = "no";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewTemplate(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 20;
 objtype = "te";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function NewBuildJob(tv)
{
 summSavedomid = lastdomain;
 objtypeAsInt = 25;
 objtype = "bj";
 objid = -1;
 currentSubTabsSelection[currenttree][0] = "tabs-General";
 ActivateSubTabs();
}

function RenameNode(tv)
{
 $(currenttree).jstree("rename");
}

function CopyNode(tv)
{
	console.log("CopyNode id="+lastobj.attr("id"));
	var id = lastobj.attr("id").substr(2).replace(/-[0-9]*$/,"");
	console.log("id="+id);
 $.ajax({
    url: "CopyAndPaste?f=c&t="+lastobj.attr("rel")+"&id="+id+"&d="+lastdomain,
    async: false,
    cache: false,
    context: document.body
  }).done(function(data) {
  });
}

function PasteNode(tv)
{
	console.log("PasteNode, lastdomain="+lastdomain+" lastobj.attr(\"id\")="+lastobj.attr("id"));
	pie=(lastobj.attr("id").substr(0,2)=="en")?"Y":"N";
	console.log("pie="+pie);
 $.ajax({
    url: "CopyAndPaste?f=p&d="+lastdomain+"&id="+lastobj.attr("id").substr(2)+"&pie="+pie,
    async: false,
    cache: false,
    context: document.body
  }).done(function(data) {
	  if (data.error != "") {
		  console.log("error="+data.error);
		  alert("Could not paste "+data.objtype+": "+data.error+" is not available in target domain");
	  } else {
		  console.log("id = "+data.id+" rel="+data.rt+" newcat="+data.newcat+" category="+data.category);
		  
		  cnc=null;
		  if (typeof data.newcat != "undefined" && data.newcat) {
			// Need to create a new category in the tree - create_node binding will fire and take over...
			console.log("Creating new category "+data.catname);
			cnc={"data": data};
			$(currenttree).jstree("create", lastsel, "inside", { "data": data.catname, attr: {"id": data.category, "rel": "Category"}},false,true);
		  } else {
			// Not creating new category node
			if (typeof data.category != "undefined") {
				// Creating new object in an existing category
				SwitchDisplay(data.id,pie);
			} else {
				// Standard Creation (Directly in Domain)
				$(currenttree).jstree("create", lastsel, "inside",  { "data" : data.name, attr: {"id": data.id, "rel": data.rt}},false,true);
				console.log("b");
				SwitchDisplay(data.id);
			}
		  }  
	  }
  });
}

function ExportNode(tv)
{
	var otid = lastobj.attr("id").substr(2);
	var id=otid.replace(/-.*$/,"");
	console.log("id="+id);
	$.fileDownload("GetActionLayout?f=export&actionid="+id)
		.done(function() {
			alert("done!");
		})
		.fail(function() {
			alert("failed!");
		});
}

function sendTestNotify()
{
	var recipient = document.getElementById('recipient').value;
	$.ajax({
		dataType : "text",
		url : "RunProviderTest?objtype=9&rcpt="+recipient+"&id="+lastobj.attr("id").substr(2),
		async : true
	}).done(function(data) {
		console.log(data);
		parent.$("#modal").dialog("close");
	});
}

function uploadFile()
{
	$('#importForm').ajaxForm();
	var $bar = $('.progress-bar');
	var $percent = $('.progress-percent');
	console.log("uploadFile, lastdomain="+lastdomain);
	$('#importForm').ajaxSubmit({
	    url: "ImportProc?domain="+lastdomain,
	    dataType: 'json',
	    type: 'POST',
		async: true,
		clearForm: true,
		beforeSend: function() {
			$('#messageProgress').show();
			var percentVal = '0%';
	        $bar.width(percentVal);
	        $percent.html(percentVal);
		},
		uploadProgress: function(event, position, total, percentComplete) {
			var percentVal = percentComplete + '%';
	        $bar.width(percentVal);
	        $percent.html(percentVal);
		},
	    success: function(res) {
	        $('#messageProgress').hide();
	    	if(!res.result) {
	    		console.log(JSON.stringify(res));
	    		if(res.error) {
	    			alert(res.error);
	    		} else {
	    			alert("Message failed to post");
	    		}
	    		$('.ui-dialog button:nth-child(1)').button('disable');
	    		return;
	    	} else {
	    		// all good
	    		parent.$("#modal").dialog("close");
	    		console.log("Switching display to "+res.otid);
	    		SwitchDisplay(res.otid);
	    	}
	    }
	});
}
function fs()
{
	$('.ui-dialog button:nth-child(1)').button('enable');
}

function fs2(sms,chat)
{
	var addr = document.getElementById('recipient').value;
	if (sms) {
		if (addr.length>8) {
			$('.ui-dialog button:nth-child(1)').button('enable');
		} else {
			$('.ui-dialog button:nth-child(1)').button('disable');
		}
	} else if (!chat) {
		var ac = addr.indexOf("@");
		if (ac>=1 && ac < addr.length-1) {
			$('.ui-dialog button:nth-child(1)').button('enable');
		} else {
			$('.ui-dialog button:nth-child(1)').button('disable');
		}
	}
}

function ShowActionRefs(tv)
{
	$.ajax({
		dataType : "json",
		url : "GetActions?reason=getrefs&actionid="+lastobj.attr("id").substr(2),
		async : true
	}).done(function(res) {
		console.log(res);
		console.log(res.refs.length);
		var td;
		if (res.refs.length > 0) {
			td="<table width=\"100%\">";
			var bgc=["#dddddd","#eeeeee"];
			var c=0;
			for (var i=0;i<res.refs.length;i++) {
				var r = res.refs[i].desc;
				var d = res.refs[i].data;
				console.log(d);
				var img="";
				if (d.type=='co') img="css/images/component_16x.png";
				if (d.type=='cv') img="css/images/component_ver_16x.png";
				if (d.type=='ap') img="css/images/application_16x.png";
				if (d.type=='av') img="css/images/appversion_16x.png";
				if (d.type=='do') img="css/images/domain_16x.png";
				var linkval="<a href='javascript:SwitchDisplay(\""+d.type+d.id+"\");'>"+d.name+"</a>";
				td+="<tr style=\"background-color:"+bgc[c]+";\"><td><img src=\""+img+"\">"+linkval+"</td><td style=\"width:250px;\">"+r+"</td></tr>";
				c=(c==0)?1:0;
			}
		} else {
			td="No References found";
		}
		var pwd = parent.$("#modal");
		var buttons = [
		{
			text : "OK",
			click : function() {
				parent.$("#modal").dialog("close");
			}
		}];
		pwd.empty();
		pwd.html(td);
		pwd.dialog({open : null}); // remove any old open event
		pwd.dialog("option", "title", "References to "+res.name);
		pwd.dialog("option", "height", "300");
		pwd.dialog("option", "width", "600px");
		pwd.dialog("option", "buttons", buttons);
		pwd.dialog('open');
	});
}

function ImportProc(tv)
{
	var pwd = parent.$("#modal");
	var buttons = [
	{
		text : "Import",
		click : function() {
			uploadFile();
		}
	},
	{
		text : "Cancel",
		click : function() {
			parent.$("#modal").dialog("close");
		}
	}];
	pwd.empty();
	pwd.html(
			"<form id=\"importForm\">"
			+ "Please Select the file to import<br><br>"
			+ "<input onChange=\"fs();\" name=\"impfile\" id=\"impfname\" accept=\".re\" type=\"file\"><br><br>"
			+ "<div id=\"messageProgress\" style=\"float:left;display:none;\"><div class=\"progress\"><div class=\"progress-bar\"></div><div class=\"progress-percent\">0%</div></div></div>"
			+ "</form>"
	);
	pwd.dialog({open : null}); // remove any old open event
	pwd.dialog("option", "title", "Import Function/Procedure");
	pwd.dialog("option", "height", "auto");
	pwd.dialog("option", "width", "400px");
	pwd.dialog("option", "buttons", buttons);
	$('.ui-dialog button:nth-child(1)').button('disable');
	pwd.dialog('open');
}

function TestDataSource(tv)
{
	$.ajax({
		dataType : "text",
		url : "RunProviderTest?objtype=10&id="+lastobj.attr("id").substr(2),
		async : true
	}).done(function(data) {
		var pwd = parent.$("#modal");
		var buttons = [
		{
			text : "OK",
			click : function() {
				parent.$("#modal").dialog("close");
			}
		}];
		pwd.empty();
		pwd.html(data);
		pwd.dialog({open : null});
		pwd.dialog("option", "title", "Test Datasource Results");
		pwd.dialog("option", "height", "auto");
		pwd.dialog("option", "width", "400px");
		pwd.dialog("option", "buttons", buttons);
		pwd.dialog('open');
	});
}

function TestNotify(tv)
{
	var pwd = parent.$("#modal");
	
	$.ajax({
		dataType : "json",
		url : "UserDetails?type=getemail&notid="+lastobj.attr("id").substr(2),
		async : false
	}).done(function(data) {
		console.log(data);
		var sms = (data.provider == "txtlocal");
		var chat = (data.provider == "slack" || data.provider == "hipchat");
		var sbt = sms?"Send SMS":chat?"Send Chat Message":"Send Email";
		var rcpt = sms?data.number:chat?"":data.email;
		var buttons = [
           	{
           		text : sbt,
           		click : function() {
           			sendTestNotify();
           		}
           	},
           	{
           		text : "Cancel",
           		click : function() {
           			parent.$("#modal").dialog("close");
           		}
           	}];
		pwd.empty();
		pwd.html(
				"<form id=\"notifyForm\">"
				+ "Send Test Notification to:<br><br>"
				+ "<input onkeyup=\"fs2("+sms+","+chat+");\" name=\"recipient\" value=\""+rcpt+"\" id=\"recipient\" size=\"40\" type=\"input\"><br><br>"
				+ "</form>"
		);
		pwd.dialog({open : null});
		pwd.dialog("option", "title", "Send Test Message");
		pwd.dialog("option", "height", "auto");
		pwd.dialog("option", "width", "400px");
		pwd.dialog("option", "buttons", buttons);
		fs2(sms,chat);
		pwd.dialog('open');
	});
	
}

function DeleteNode(tv)
{
 selection = $(currenttree+' .jstree-clicked').text().trim().replaceAll(" ","&nbsp;");
 ot = lastobj.attr("rel");
 objtype = ot.substr(0,1).toUpperCase()+ot.substr(1).toLowerCase();
 if (objtype=="Credentials") objtype="Credential";
 if (objtype=="Notify") objtype="Notifier";
 if (objtype=="Servercomptype") objtype="Component Type"; 
 var pwd = parent.$("#modal");
 var buttons = [
 {
  text : "Ok",
  click : function()
  {
   $(currenttree).jstree("remove");
   parent.$("#modal").dialog("close");
  }
 },
 {
  text : "Cancel",
  click : function()
  {
   parent.$("#modal").dialog("close");
  }
 } ];
 
 pwd.empty();
 pwd.html("<table border=0><tr><td valign='center'><img src='css/images/question_36px.png'></td><td><b>Are you sure?</b><br><hr>Please confirm you want to delete the "+objtype+" \""+selection+"\"?</td></tr></table>");
 pwd.dialog({open : null});
 pwd.dialog("option", "title", "Delete Confirmation");
 pwd.dialog("option", "height", "auto");
 pwd.dialog("option", "width", "400px");
 pwd.dialog("option", "buttons", buttons);
 pwd.dialog('open');
}

function ShowTaskOutput(taskname,retdata)
{
	var buttons = [{ text: "Ok", click: function(){$(this).dialog("close");}}];
	var pwd = parent.$("#modal");
	pwd.empty();	 
	pwd.html(retdata);
		 
	pwd.dialog({
		  autoResize: true,
		  autoOpen:false,
		  modal:true,
		  width: "auto",
		  height: 500,
		  resizable:true,
		  open: null
		 });

	pwd.dialog("option", "title", taskname);
	pwd.dialog("option", "buttons", buttons);
	pwd.dialog('open');
}

function Execute(taskname,taskid,otype,id,pid,tasktype,params,domainid)
{
 for (var i=0;i<params.length;i++) {
	 $("#tpa"+i).removeClass("fragmentfielderror");
	 $("#tpb"+i).removeClass("fragmenterror");
 }
 $("#execute_err").html("");
 var pwd = parent.$("#modal");
 var hasApps = $("#apps").val();
 console.log("TASKTYPE=" + tasktype);
 var mp=false;
 var aps="";
 for (var i=0;i<params.length;i++) {
	 var fld = params[i].varname;
	 var val = $("#"+params[i].varname).val();
	 console.log("fld=["+fld+"] val=["+val+"]");
	 if (val=="") {
		 $("#tpa"+i).addClass("fragmentfielderror");
		 $("#tpb"+i).addClass("fragmenterror");
		 mp=true;
	 } else {
		 aps+="&ap"+fld+"="+encodeURIComponent(val);
	 }
	 console.log("aps="+aps);
 }
 if (mp) {
	 $("#execute_err").html("Please enter value(s) for the highlighted field(s)");
	 return;
 }
 
 
 var url = "";
 
 if (typeof hasApps != "undefined" && tasktype == "DEPLOY")
 {
  var envid = $("#envs").val();
  var appid = $("#apps").val();
  id = appid.substring(3);  
  envid = envid.substring(3);
  
  console.log("Executing taskid "+taskid + " envid=" + envid + " appid=" + id);
  url = "RunTask?f=run&tid="+taskid+"&id="+id + "&envid=" + envid + "&pid=" + pid + aps;
 }
 else if (tasktype == "DEPLOY")
 {
  var envid = $("#envs").val();
  envid = envid.substring(3);
 
  console.log("Executing taskid "+taskid + " envid=" + envid);
  url = "RunTask?f=run&tid="+taskid+"&id="+id + "&envid=" + envid + "&pid=" + pid + aps;
 }
 else if (tasktype == "REMOVE")
 {
  var envid = $("#envs").val();
  envid = envid.substring(3);
 
  console.log("Executing taskid "+taskid + " envid=" + envid);
  url = "RunTask?f=run&tid="+taskid+"&id="+id + "&envid=" + envid + "&pid=" + pid + aps;
 } 
 else if (tasktype == "APPROVE")
 {
  var approve = $("#acceptreject").val();
  
  if (approve == "Accept")
   approve = "Y";
  else
   approve = "N";
  
  var notes = $("#notes").val();
  
  url = "RunTask?f=run&tid="+taskid+"&id="+id + "&approve=" + approve + "&notes=" + encodeURIComponent(notes) + "&pid=" + pid + aps;
 } 
 else if (tasktype == "MOVE")
 {
  var notes = $("#notes").val();
  
  url = "RunTask?f=run&tid="+taskid+"&id="+id + "&notes=" + encodeURIComponent(notes) + "&pid=" + pid + aps;
 }
 else if (tasktype == "CREATE_VERSION")
 {
	 var pred = $("#predlist").val();
	 if (pred != 0 && pred != -1) {
		 pred = pred.substr(2);	// PAG MOD: for specific versions, val is an OTID
	 }
	 url = "RunTask?f=run&tid="+taskid+"&id="+id + "&pred=" + pred + "&pid=" + pid + aps;
 } 
 else if (tasktype == "REQUEST")
 {
	 var notes = $("#notes").val();
	 url = "RunTask?f=run&tid="+taskid+"&id="+id + "&notes=" + encodeURIComponent(notes) + "&pid=" + pid + aps;
 }
 else if (tasktype == "RUN_ACTION")
 {
	 url = "RunTask?f=run&objtype="+otype+"&tid="+taskid+"&id="+id+"&pid="+pid + aps;
 }
   
   $.getJSON(url, function(data) {
    // success - close the dialog
     console.log(data);
     if (data.error != "" && typeof data.error != "undefined") {
    	 $("#execute_err").html(data.error); 
     } else if (data.depno > 0 || data.result == true) {
    	 // Success
    	 pwd.dialog("close"); 
    	 if (save_showoutput && typeof data.output != "undefined" && data.output != "") {
    		 ShowTaskOutput(taskname,data.output);
         }
    	 if (tasktype == "CREATE_VERSION") {
    		 DoRefresh();
    		 LoadVersionsData();
    	 }
    	 if (tasktype == "MOVE" || tasktype == "RUN_ACTION") DoRefresh();
    	 if (save_showoutput == "true") DisplayDeploy(data.depno);
    	 // If we're on timeline, refresh it
		 if (currentSubTabsSelection[currenttree][0]=='tabs-History') {
			 console.log("Refreshing the timeline!");
			 ActivateSubTabs();
		 }
     } else if (data.result == false) {
    	 if (data.output != "" && typeof data.output != "undefined") {
    		 $("#execute_err").html(data.output);
    	 } else {
    		 $("#execute_err").html("Task failed to execute");  
    	 }
     }
   }); 
  }

function RefreshDropDowns()
{
	var envs = $("#envs");
	var apps = $("#apps");
	var envid = envs.length>0?envs.val().substr(3):0;
	var appid = apps.length>0?apps.val().substr(3):0;
	console.log("appid="+appid);
	console.log("envid="+envid);
	for (var i=0;i<apdl.length;i++) {
		var url="f=getarrs&pid=0&tid="+globalTaskID+"&id=0&arrname="+apdl[i].an;
		if (appid>0) url+="&appid="+appid;
		if (envid>0) url+="&envid="+envid;
		console.log("url="+url);
 	 	console.log("apdl["+i+"].vn=["+apdl[i].vn+"] apdl["+i+"].an=["+apdl[i].an+"]");
 	 	var sid = $("#"+apdl[i].vn);
 	 	sid.find('option').remove();
 	 	$.getJSON('RunTask',url, function(res) {
 	 		console.log(res);
 	 		for (var t=0;t<res.length;t++) {
 	 			console.log("Appending name="+res[t].name);
 	 			sid.append("<option id=\""+res[t].name+"\">"+res[t].val+"</option>");
 	 		}
 	 	});
	}
}

function RunTask(taskname,otid,taskid)
{
 globalTaskID = taskid;
 var pwd = parent.$("#modal");
 var otype = lastSelectedNode.substring(0,2);
 var deployname = objName;
 var domid = $("#" + lastSelectedNode).attr("id");
 var id = lastSelectedNode.substring(2);
 var parent_id;
 if (otype == "do") {
	parent_id = $("#" + lastSelectedNode).attr("id"); 
 } else {
	parent_id = $("#" + lastSelectedNode).parent().parent().attr("id");
 }
 parent_id = parent_id.substring(2);
 
 console.log("Running task taskid="+taskid + " otype="+otype+" id="+id);
 var tasktype = "";
 var domainname = "";
 var domainid = "";
 var envs = "";
 var applications = "";
 var ap = "";
 var params = null;

 $.ajax(
 {
  dataType : "json",
  url : "RunTask?f=tasktype&tid=" + taskid + "&id=" + id + "&pid=" + parent_id + "&otid=" + otype + id,
  async : false
 }).done(function(data)
 {
  console.log(data);
  applications = data.Applications;
  envs = data.Environments;
  tasktype = data.tasktype;
  domainname = data.domain;
  domainid = data.domainid;  
  save_showoutput = data.showoutput;
  // process additional parameters
  params = data.params;
  apdl = [];
  for (var i=0;i<params.length;i++) {
	  var typestr=$.trim(params[i].type.toLowerCase());
	  if (typestr=="entry") typestr="text";
	  if (typestr=="dropdown") {
		  var str="<select id=\"ap"+params[i].varname+"\"></select>";
		  apdl.push({"vn": "ap"+params[i].varname, "an": params[i].arrname});
	  } else {
		  var str="<input id=\""+params[i].varname+"\" type=\""+typestr+"\" width=\"40\">";
	  }
	  ap=ap+"<tr><td id=\"tpa"+i+"\" style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">"+ params[i].label +":</td><td id=\"tpb"+i+"\">"+str+"</td></tr>";
  }
 });
 
 
 if (taskid == "Unsubscribe" || taskid == "Subscribe")
  return;
 
 var buttons = 
  [
        { text: "Ok",     click: function() { Execute(taskname,taskid,otype,id, parent_id, tasktype, params, domainid); } },
        { text: "Cancel", click: function() { $( this ).dialog("close"); } }
     ];
 
 pwd.empty();
 
 var h = "<form id=\"runtaskform\"><table id=\"runtask\" width=\"100%\" ><tbody></tbody></table></form>";
 h += "<div class=\"error\" id=\"execute_err_box\"><p class=\"error_txt\" id=\"execute_err\"></p></div>";        
 
 pwd.html(h);
 
 pwd.dialog(
 {
  autoResize: true,
  autoOpen:false,
  modal:true,
  width: "auto",
  height: "auto",
  resizable:false,
  
  open : function(event, ui)
  {
   var td = "";
   
   if (tasktype == "DEPLOY" && typeof applications != "undefined")
   {
    td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Environment:</td><td><select onchange=\"javascript:RefreshDropDowns();\" name=\"envs\" id=\"envs\">";   
    
    var rowcnt = 0;
    var dups = [];
    
    for (a = 0; a < envs.length; a++)
    {
     if (typeof dups[envs[a].name] == "undefined")
     {
      dups[envs[a].name] = 1;
      
      td += "<option  value=\"env" + envs[a].id + "\">" + envs[a].name + "</option>";
     } 
    }    
    
    td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td><select onchange=\"javascript:RefreshDropDowns();\" name=\"apps\" id=\"apps\">"; 
    rowcnt = 0;
    dups = [];
    
    for (a = 0; a < applications.length; a++)
    {
     if (typeof dups[applications[a].name] == "undefined")
     {
      dups[applications[a].name] = 1;
      
      td += "<option  value=\"app" + applications[a].id + "\">" + applications[a].name + "</option>";
      for (b = 0; b < applications[a].versions.length; b++)
      {
       if (typeof dups[applications[a].versions[b].name] == "undefined")
       {
        dups[applications[a].versions[b].name] = 1;
        td += "<option  value=\"app" + applications[a].versions[b].id + "\">" + applications[a].versions[b].name + "</option>";
       } 
      }
     } 
    }
    td += "</td></tr>";
    $("#runtask > tbody").html(td+ap);
    RefreshDropDowns();
   }
   else if (tasktype == "DEPLOY" || tasktype == "REMOVE")
   {
	   var rsn=(tasktype=="DEPLOY")?"tgtenvlist":"depenvlist";
    $.getJSON('GetAppVersInEnvData', "appid=" + id + "&reason="+rsn+"&tid="+taskid, function(res)
     {
      td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td>" + objName + "<input type=\"hidden\" id=\"apps\" value=\"app"+id+"\"></td></tr>";
      td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Environment:</td><td><select onchange=\"javascript:RefreshDropDowns();\" name=\"envs\" id=\"envs\">"; 
       
      var rowcnt = 0;
      console.log(res['data']);
      for (a = 0; a < res['data'].length; a++)
      {
       var data = res['data'][a][1];
       // var lastapp = res['data'][a][2];
       // var details = res['data'][a][3];
       td += "<option  value=\"env" + data.id + "\">" + data.name + "</option>";
      }
      td += "</td></tr>";
      appid=id;
      $("#runtask > tbody").html(td+ap);
      RefreshDropDowns();
     });
       
    }
    else if (tasktype == "APPROVE")
    {
     td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Domain:</td><td>" + domainname +  "</td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td>" + objName + "<input type=\"hidden\" id=\"apps\" value=\"app"+id+"\"></td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Accept/Reject:</td><td><select name=\"acceptreject\" id=\"acceptreject\">";
     td += "<option  value=\"Accept\" select>Accept</option>";
     td += "<option  value=\"Reject\">Reject</option>";
     td += ap;
     td += "<tr><td style=\"text-align:left;\" style=\"width:30%\">Notes:</td><td>&nbsp;</td></tr>";
     td += "<tr><td colspan=\"2\" width=\"100%\"><textarea rows=\"5\" id=\"notes\" name=\"notes\" /></td></tr>";   
     td += "</td></tr>";
     $("#runtask > tbody").html(td);
     RefreshDropDowns();
    }
    else if (tasktype == "MOVE")
    {
     td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Target Domain:</td><td>" + domainname +  "</td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td>" + objName + "<input type=\"hidden\" id=\"apps\" value=\"app"+id+"\"></td></tr>";
     td += ap;
     td += "<tr><td style=\"text-align:left;\" style=\"width:30%\">Notes:</td><td>&nbsp;</td></tr>";
     td += "<tr><td colspan=\"2\" width=\"100%\"><textarea rows=\"5\" id=\"notes\" name=\"notes\" /></td></tr>";   
     td += "</td></tr>";
     $("#runtask > tbody").html(td);
     RefreshDropDowns();
    }   
    else if (tasktype == "CREATE_VERSION")
    {
     td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Domain:</td><td>" + domainname +  "</td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td>" + objName + "<input type=\"hidden\" id=\"apps\" value=\"app"+id+"\"></td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Predecessor Version:</td><td><select name=\"predlist\" id=\"predlist\">";
     td += "<option  value=\"-1\">Latest version</option>";
     td += "<option  value=\"0\" select>Application Base Version</option>";
     
     $.ajax(
       {
        url:"GetApplicationContent?appid=" + id, 
        async: false,
        dataType: "json",
        success: function (res)
        {         
         var rowcnt = 0;

         console.log(res);
         for (a = 0; a < res.length; a++)
         {
          var data = res[a].data;
          var id   = res[a].attr.id;

          td += "<option  value=\"" + id + "\">" + data + "</option>";
         }
        } 
      });     
     td += "</select></td></tr>";     
     td += ap;
     td += "<tr><td style=\"text-align:left;\" style=\"width:30%\">Notes:</td><td>&nbsp;</td></tr>";
     td += "<tr><td colspan=\"2\" width=\"100%\"><textarea rows=\"5\" id=\"notes\" name=\"notes\" /></td></tr>";   
     td += "</td></tr>";
     $("#runtask > tbody").html(td);
     RefreshDropDowns();
    }
    else if (tasktype == "REQUEST")
    {
     td = "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Domain:</td><td>" + domainname +  "</td></tr>";
     td += "<tr><td style=\"text-align:right;padding-right: 5px;white-space: nowrap;\" style=\"width:30%\">Application:</td><td>" + objName + "<input type=\"hidden\" id=\"apps\" value=\"app"+id+"\"></td></tr>";
     td += ap;
     td += "<tr><td style=\"text-align:left;\" style=\"width:30%\">Notes:</td><td>&nbsp;</td></tr>";
     td += "<tr><td colspan=\"2\" width=\"100%\"><textarea rows=\"5\" id=\"notes\" name=\"notes\" /></td></tr>";   
     td += "</td></tr>";
     $("#runtask > tbody").html(td);
     RefreshDropDowns();
    }
    else if (tasktype == "RUN_ACTION")
    {
    	$("#runtask > tbody").html(ap);
    }
   
    
   $("edit_button").button(
   {
    disabled : true
   });  
  }
 });

 pwd.dialog("option", "title", taskname);
 pwd.dialog("option", "buttons", buttons);
 if (taskid != "Audit" && taskid != "AuditEnv")
   pwd.dialog('open');

}

function WorkbenchTask(taskname,taskid)
{
 console.log("WorkbenchTask, taskname="+taskname+" taskid="+taskid);
 var tid=taskid.substr(taskid.lastIndexOf("-")+1); 
 console.log("tid="+tid+" id of selected object="+lastsel.attr("id"));
 RunTask(taskname,lastsel.attr("id"),tid);
}

function DoRefresh()
{
	console.log("DoRefresh()");
	Refresh(currenttree);
}

function callback(currenttree,taskname,taskid,tv)
{
 lastsel = $(currenttree).jstree('get_selected');
 switch(taskid) {
 case "Rename":        RenameNode(tv); break;
 case "Delete":        DeleteNode(tv); break;
 case "Export":		   ExportNode(tv); break;
 case "Import":	       ImportProc(tv); break;
 case "Copy":          CopyNode(tv); break;
 case "Paste":         PasteNode(tv); break;
 case "NewDomain":     NewDomain(tv); break;
 case "NewEnv":        NewEnvironment(tv); break;
 case "NewComp":       NewComponent(tv); break;
 case "NewCred":       NewCredential(tv); break;
 case "NewCompType":   NewCompType(tv); break;
 case "NewServ":       NewServer(tv); break;
 case "NewRep":        NewRepository(tv); break;
 case "NewGroup":      NewGroup(tv); break;
 case "NewApp":        NewApp(tv); break;
 case "NewBuilder":    NewBuilder(tv); break;
 case "NewRelease":    NewRelease(tv); break; 
 case "NewUser":       NewUser(tv); break;
 case "NewAction1":    NewAction(tv,"G"); break;
 case "NewAction2":    NewAction(tv,"F"); break;
 case "NewAction3":    NewAction(tv,"N"); break;
 case "NewDataSource": NewDatasource(tv); break;
 case "NewNotifier":   NewNotifier(tv); break;
 case "NewTemplate":   NewTemplate(tv); break;
 case "NewBuildJob":   NewBuildJob(tv); break;
 case "TestNotify":	   TestNotify(tv); break;
 case "TestDataSource":TestDataSource(tv); break;
 case "Refs":          ShowActionRefs(tv); break;
 default:         WorkbenchTask(taskname,taskid); break;
 }
}

function getIcon(taskid)
{
 console.log("taskid="+taskid);
 switch(taskid) {
 case "Rename":        return "images/rename.png";
 case "Delete":        return "images/delete.png";
 case "Copy":          return "images/copy.png";
 case "Paste":         return "images/paste.png";
 case "NewDomain":     return "images/domain.png";
 case "NewEnv":        return "images/environment.png";
 case "NewComp":       return "images/component.png";
 case "NewCompType":   return "images/component.png";
 case "NewCred":       return "images/credential.png";
 case "NewServ":       return "images/server.png";
 case "NewRep":        return "images/repository.png";
 case "NewGroup":      return "images/usergroup.png";
 case "NewApp":        return "images/application.png";
 case "NewBuilder":    return "css/images/buildserv_16x.png";
 case "NewBuildJob":   return "images/buildjob_16x.png";
 case "NewRelease":    return "images/application.png";
 case "NewUser":       return "css/images/user_16x.png";
 case "NewAction1":    return "images/action.png";
 case "NewAction2":    return "css/images/func.png";
 case "NewAction3":    return "css/images/proc.png";
 case "NewDataSource": return "images/datasource.png";
 case "NewNotifier":   return "images/notify.png";
 case "NewTemplate":   return "images/template.png";
 case "TestNotify":    return "images/notify.png";
 case "TestDataSource":return "images/datasource.png";
 case "Export":        return "css/images/export.png";
 case "Import":        return "css/images/import.png";
 case "Refs":          return "css/images/references.png";
 case "Subscribe":     return "css/images/subscribe.png";
 case "Unsubscribe":   return "css/images/unsubscribe.png";
 default: {
  var udf=taskid.substr(0,taskid.lastIndexOf("-"));
  console.log("udf="+udf);
  switch(udf) {
  case "Approve": return "images/approve.png";
  case "Remove": return "images/remove.gif";
  case "Deploy": return "images/deploy.gif";
  case "Move": return "images/movever.png";
  case "Request": return "images/request.png";
  case "CreateVersion": return "images/appver.png";
  case "RunAction": return "images/task.png";
  }
 }
 }
}

function jsSelectNode(frameid,admin,nodetype,id, objtypeAsInt)
{
	console.log("in jsSelectNode("+frameid+", "+admin+", "+nodetype+","+id+", "+objtypeAsInt);
	var mainframe = parent.document.getElementById(frameid);
}

function eventSelectNode(frameid,admin,data)
{
	// var NodeType = data.inst.get_json()[0].attr.rel;
	if (lastobj) console.log("eventSelectNode, lastobjid="+lastobj.attr("id"));
	var NodeType = data.rslt.obj.attr("rel");
		
	// if (lastSelectedNode == data.inst.get_json()[0].attr.id) {
	if (lastSelectedNode == data.rslt.obj.attr("id")) {
		console.log("returning early");
		return;
	}
	
	// lastSelectedNode = data.inst.get_json()[0].attr.id;
	lastSelectedNode = data.rslt.obj.attr("id");
	
	console.log("selected node = "+lastSelectedNode);
	objid = data.rslt.obj.attr("id").substr(2);	//OTID, first two chars are Object Type
	objtype = data.rslt.obj.attr("id").substr(0,2);
	objkind="";
	console.log("*** SelectNode objtype="+objtype+" objid="+objid);
	  if (objtype == "pr" || objtype == "fn") {
		  objkind=objid.substr(objid.indexOf("-")+1);
		  objid = objid.substr(0,objid.indexOf("-"));
	  }
	objtypeAsInt = obj2Int[objtype][0];
	objtypeName = obj2Int[objtype][1];	
	if (typeof data.inst.get_json()[0] == "undefined") {
		$(currenttree).jstree("select_node","#"+objtype+objid);
		objName = $(currenttree+' .jstree-clicked').text();
	} else {
		objName = data.inst.get_json()[0].data;
	}
 // objName = $(currenttree+' .jstree-clicked').text();
 console.log("objName");
 console.log(data);
 console.log("id="+objid+" objtype="+objtype + " objtypeAsInt=" + objtypeAsInt + " objtypeName=" + objtypeName + " objName=" + objName + " objkind=" + objkind+ " oldmenu=" + oldmenu + " currenttree=" + data.inst.get_container().attr("id"));
 
 jsSelectNode(frameid,admin,NodeType,objid, objtypeAsInt);

 console.log("data.inst.get_container().attr(\"id\")="+data.inst.get_container().attr("id"));
 crumb = currentSubTabsSelection["#" + data.inst.get_container().attr("id")];
 console.log(crumb);
 if (typeof crumb != "undefined") {
	 crumb[1] = lastSelectedNode;
 }
 console.log("currentSubTabsSelection[#"+data.inst.get_container().attr("id")+"]="+crumb);
 currentSubTabsSelection["#" + data.inst.get_container().attr("id")] = crumb;

 ActivateSubTabs();
 console.log("gotoDate="+gotoDate);
 if (gotoDate != null) $("#tabs-Calendar").click();	// Switch to Calendar Tab
 if ((objtype=="do" || objtype=="cp" || objtype=="cy" || objtype=="cc") && oldmenu != "domains_menu") displayhome=true;
 ShowHome(false,ShowingInitialHelp);
 displayhome=false;
}

function SubTabBreadCrumb(id)
{
	console.log("SubTabBreadCrumb("+id+")");
 crumb = currentSubTabsSelection[currenttree];
 crumb[0] = id;
 currentSubTabsSelection[currenttree] = crumb;
 }

function jsRemoveNode(frameid,admin,data)
{
	var otid=data.rslt.obj.attr("id");
	console.log("remove_node.jstree id="+otid);
	if (otid.substr(0,2)!="cc" && otid.substr(0,2)!="cy" && otid.substr(0,2)!="cp") {
		// Do not fire if we're removing a category
		$.ajax({
	    	   url:"RemoveNode", 
	    	   data: { 
                    "rel" : data.rslt.obj.attr("rel"), 
                    "id" : data.rslt.obj.attr("id").substr(2)
	    	   },
	    	   dataType: "json",
	    	   success: function (d) {
	    		   console.log("RemoveNode returns:");
	    		   console.log(d);
	    		   if (d.errcode > 0 || d.success == false) {
	    			   $.jstree.rollback(data.rlbk);
	    			   if (typeof d.errtext != "undefined") {
		    			   d.errtext = d.errtext.replaceAll(",","\n");
		    			   alert(d.errtext);
	    			   } else {
	    				   d.error = d.error.replaceAll(",","\n");
	    				   alert(d.error);
	    			   }
	    		   } else {
	    			   if (typeof d.remcat != "undefined" && d.remcat != "") {
	    				   // Remove the category.
	    				   console.log("Removing category #"+d.remcat);
	    				   $(currenttree).jstree("remove","#"+d.remcat);
	    			   }
	    			   console.log("Deleting record from breadcrumb list");
	    			   $("#footer_container").children().each(function(i){
	    				   console.log("id="+this.id);
	    			    	if (this.id == "f"+data.rslt.obj.attr("id")) {
	    			    		console.log("Found "+this.id+": removing");
	    			    		$(this).remove();
	    			    	}
	    			   });
	    			   console.log("node deleted");
	    		   }
	    	   }
	    });
	} 
	console.log("done calling RemoveNode");
}

function MoveNode(data)
{
	console.log('moving node of type '+data.rslt.o[0].getAttribute("rel")+" id="+data.rslt.o[0].id+' to target of type '+data.rslt.np[0].getAttribute("rel")+" id="+data.rslt.np[0].id);
	var nodeid = data.rslt.o[0].id.substr(2).replace(/-.*$/,"");
	var parenttype = data.rslt.op[0].id.substr(0,2);
	var parentid = data.rslt.op[0].id.substr(2);
	var targetid = data.rslt.np[0].id.substr(2);
	
	console.log("targetid="+targetid); 
	console.log("parentid="+parentid);
	console.log("parenttype="+parenttype);
	var newcat=null;
	var catname=null;
	if (parenttype=="cc" || parenttype=="cy" || parenttype=="cp") {
		// Moving a node with a category
		var newdom = data.rslt.np[0].id;
		console.log("newdom="+newdom);
		newcat=parenttype+parentid.substr(0,parentid.indexOf("-"))+"-"+targetid;
		console.log("looking for #"+data.rslt.op[0].id);
		catname=$("#"+data.rslt.op[0].id).first().text().trim();
		console.log("newcat="+newcat);
		console.log("catname=["+catname+"]");
	}
	$.post(
	            "MoveNode", 
	            {
	            		"nodetype"   : data.rslt.o[0].getAttribute("rel"),
	            		"parentid"	 : parentid,
	                    "nodeid"     : nodeid,
	                    "targettype" : data.rslt.np[0].getAttribute("rel"),
	                    "targetid"   : data.rslt.np[0].id.substr(2)
	            }, 
	            function (d)
	            {
	                  if (d.errtext.length > 0)
	                  {
	                	  alert(d.errtext);
	                	  $.jstree.rollback(data.rlbk);
	                  } else {
	                	  // Successful move - remove from breadcrumb
	                	  $("#footer_container").children().each(function(i){
		    				   console.log("id="+this.id);
		    			    	if (this.id == "f"+data.rslt.o[0].id) {
		    			    		console.log("Found "+this.id+": removing");
		    			    		$(this).remove();
		    			    	}
		    			  });
	                	  if (catname != null) {
		                	  console.log("Looking for "+newcat+" in "+currenttree);
		                	  var n = $(currenttree).find("#"+newcat).length;
		                	  if (n>0) {
		                		  // Category already exists in this target domain
		                		  console.log("Category already exists - switching to "+data.rslt.o[0].id);
		                		  SwitchDisplay(data.rslt.o[0].id);
		                	  } else {
		                		  // Category does not exist - create it - create_node binding will fire and call back to refresh display
		                		  console.log("Creating new category ("+catname+") in #"+newdom);
		                		  cnc={"data": data};
		                		  $(currenttree).jstree("create","#"+newdom,"inside",{ "data" : catname, attr: {"id": newcat, "rel": "Category"}},false,true);
		                	  }
	                	  }
	                  }
	            }
		);
}

function jsRenameNode(frameid,admin,data)
{
	console.log("rename_node.jstree newname="+data.rslt.name);
	$.post(
	            "RenameNode", 
	            { 
	                    "rel" : data.rslt.obj.attr("rel"), 
	                    "id" : data.rslt.obj.attr("id").substr(2),
	                    "newname" : data.rslt.name
	            }, 
	            function (d)
	            {
	                  console.log("d.name="+d.errtext+" d.id="+d.errcode);

	                  if (d.errcode > 0)
	                  {
	                   var h = $("#" + data.rslt.obj.attr("id")).find("a").html();
	                   h = h.replace(data.rslt.name,d.newname);
	                   $("#" + data.rslt.obj.attr("id")).find("a").html(h);
	                   lastSelectedNode = "";
	                	  alert(d.errtext);
	                  }

	                	 eventSelectNode(frameid,admin,data);
	                	 
	            }
	  );
	  console.log("done calling RenameNode");
}

function GetDomainData(typestr)
{
	var res=null;
	
	console.log("GetDomainData("+typestr+")");
	
	$.ajax({
		  dataType: "json",
		  url: "GetDomainContent?typestr="+typestr+"&domainid=0",
		  error: function(a,textStatus,errorThrown) {
			  	console.log("ERROR: status=["+textStatus+"] errorThrown=["+errorThrown+"]"); 
		  },
		  async: false
		}).done(function( data ) {
				console.log(data);
				res=data;
		});
	return res;
}

function chgsel(sel)
{
	var ct = "#"+sel.t.substring(0, sel.t.length-4)+"_tree";
	// zap the jstree cookie that holds the selected state
	console.log("setting cookie jstree_selected_"+ct.substr(1)+"=#"+sel.id);
	// document.cookie = "jstree_selected_"+ct.substr(1)+ '=; expires=Thu, 01 Jan 1970 00:00:01 GMT;';
	document.cookie = "jstree_selected_"+ct.substr(1)+"=#"+sel.id;
	selid=sel.id;
	console.log("chgsel t="+sel.t+" id="+sel.id+" tm="+sel.tm+" odl="+sel.odl);
	console.log("EnableTabs: currentmenu="+currentmenu);
	if (sel.tm != currentmenu) {
		EnableTabs(sel.tm);		// Change top level menu
	}
	console.log("Done EnableTabs - SetActive");
	SetActive(sel.t);
	console.log("Done SetActive");
	
	HighlightNode="#"+sel.id;
	console.log("chgsel: HighlightNode="+HighlightNode);
	$(ct).bind("load_node.jstree", function (e,data) {
        // reopen all nodes that were open to the selected node
		console.log("sel.odl=["+sel.odl+"]");
        ods = sel.odl.split(" ");
        for (var i=ods.length;i--;i>=0) {
        	console.log("ods["+i+"]="+ods[i]);
        	if (ods[i]!="") {
        		$(ct).jstree("open_node","#"+ods[i]);
        	}
        }
	    $(ct).jstree("select_node", HighlightNode);
	});
}

function SwitchDisplay(otid,pie)
{
	console.log("SwitchDisplay("+otid+")");
	$.ajax({
		url: "GetDomainContent?typestr=dh&otid="+otid,
		dataType: 'text',
		success: function(data) {
			  console.log("data="+data);
			  console.log("pie="+pie);
			  var tld = Object.create(menuForType[otid.substr(0,2)]);
			  tld.id = otid;
			  tld.odl = data;
			  if (typeof pie != "undefined" && pie.toLowerCase()=="y") {
				  // Pasting a server into an environment. Override normal
				  // switching
				  console.log("Pasting in environment, overriding defaults");
				  tld.t = "environments_tab";	// Stay in environments tab
				  tld.odl = lastobj.attr("id") + " " + tld.odl;	// Add Environment to domain heirarchy
			  }
			  console.log(tld);
			  chgsel(tld);
		}
	});
}

function SwitchToCalendar(calid)
{
	console.log("SwitchToCalendar("+calid+")");
	$.ajax({
		url: "GetDomainContent?typestr=dh&calid="+calid,
		dataType: 'text',
		success: function(data) {
			  console.log(">>> data="+data);
			  var n=data.indexOf(",");
			  var st=data.substring(0,n);
			  data=data.substring(n+1);
			  console.log(">>> data now="+data);
			  n=data.indexOf(",");
			  otid=data.substring(0,n);
			  console.log(">>> st="+st+" otid="+otid);
			  var d = new Date(st*1000);
			  var tld = menuForType[otid.substr(0,2)];
			  tld.id = otid;
			  tld.odl = data.substr(n+1);
			  console.log(tld);
			  // Note, month is 0-11 but calendar also requires this.
			  gotoDate = String(d.getYear()+1900)+"/"+String(d.getMonth())+"/"+String(d.getDate());
			  gotoDateEnvId = otid.substr(2);
			  chgsel(tld);
			  $("#tabs-Calendar").click();	// Switch to Calendar Tab
		}
	});
}

function AddToFooter(t)
{
	var cbcl = $("#footer_container").html();
	var lastd=null;
	var pw=$("#footer_container").width();
	do {
		  $("#footer_container").html(cbcl + t);
		  var tw=0;
		  $("#footer_container").children().each(function(i){
			 tw=tw+$(this).width()+20;	// 20px padding around each div
			 lastd=this;
		  });
		  if (tw>(pw-50)) {
			  var ed = cbcl.indexOf("</div>");    	 
			  cbcl = cbcl.substr(ed+6);
		  }
	} while (tw>(pw-50));
	if (lastd) $(lastd).addClass('ui-breadcrumb-highlight');
}

function HighlightFooterLink(id)
{
	console.log("HighlightFooterLink("+id+")");
	var addLink=true;
	$("#footer_container").children().each(function(i){
		console.log("this.id="+this.id);
    	if (this.id == "f"+id) {
    		console.log("match, adding highlight");
    		$(this).addClass("ui-breadcrumb-highlight");
    		addLink=false;
    	} else {
    		$(this).removeClass("ui-breadcrumb-highlight");
    	}
      });
	console.log("HighlightFooterLink exits with "+addLink);
	return addLink;
}

scrollit=false;

function Scroll(lpt,offset)
{
	lpt.scrollTop(lpt.scrollTop()+offset);
	setTimeout(function() {
		if (scrollit) Scroll(lpt,offset);
	},100);
}

function CreateTree(nodename,typestr,admin,frameid)
{
	console.log("Creating tree " + nodename + "," + typestr + "," + admin + "," + frameid);
	var lpt = $("#left_panel_tree");
	lpt.off("mousemove").mousemove(function( event ) {
		var downpt = lpt.height() + 45;
		if (event.pageY < 100 && Dragging) {scrollit=true;Scroll(lpt,-2);}
		else
		if (event.pageY > downpt && Dragging) {scrollit=true;Scroll(lpt,2);}
		else scrollit=false;
	});
 
	var BaseNode=0;
	var plugins = ["themes","json_data","ui","crrm","types", "cookies"];
	var current_tree = $(nodename);
	
      console.log(current_tree);
      if (admin=="Y") plugins.push("dnd");
	 
      console.log("GetDomainContent?typestr="+typestr+"&domainid=0");
      current_tree.jstree("destroy");
      current_tree.jstree({
            "json_data" : {
            	 "data" : GetDomainData(typestr),	// Initial data, including any opened nodes
                 "ajax" : {
                      "url" : function( node ){
                      if( node == -1 )
                      {
                    	  BaseNode=1;
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
                    		  id = id.substr(0,id.indexOf("-"));
                    	  }
                    
                    	  console.log("objtype="+objtype+" id="+id);
                    	  var catdomain = 0;
                       if (objtype == "cy" || objtype == "cp" || objtype == "cf" || objtype == "cc") {
                        catdomain=id.substr(id.indexOf("-")+1);
                        console.log("catdomain="+catdomain);
                        id = id.substr(0,id.indexOf("-"));
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
                    	  } else if (objtype == "pr" || objtype == "fn" || objtype == "ac") {
                    		  return "GetActionContent?actionid="+id;
                    	  } else if (objtype == "be") {
                    		  return "GetBuildEngineJobs?builderid="+id;
                    	  } else if (objtype == "cy" || objtype == "cp" || objtype == "cf" || objtype == "cc") {
                    		  console.log("GetCategoryContent?domainid="+catdomain + "&catid="+id+"&objtype="+objtype);
                        return "GetCategoryContent?domainid="+catdomain + "&catid=" + id + "&objtype=" + objtype;
                    	  }
                    	  else {
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
                      "image" : "css/images/environment_16x.png"
                  }
              },
              "Repository" : {
                  "icon" : {
                      "image" : "images/repository.png"
                  }
              },
              "Component" : {
                  "icon" : {
                      "image" : "css/images/component_16x.png"
                  }
              },
              "Credentials" : {
                  "icon" : {
                      "image" : "images/credential.png"
                  }
              },
              "Domain" : {
                  "icon" : {
                      "image" : "css/images/domain_16x.png"
                  }
              },
              "Lifecycle" : {
                  "icon" : {
                      "image" : "css/images/lifecycle_16x.png"
                  }
              },
              "Server" : {
                  "icon" : {
                      "image" : "images/server.png"
                  }
              },
              "Release" : {
               "icon" : {
                   "image" : "css/images/application_16x.png"
               }
              },
              "Application" : {
                  "icon" : {
                      "image" : "css/images/application_16x.png"
                  }
              },
              "AppVersion" : {
                  "icon" : {
                      "image" : "css/images/appversion_16x.png"
                  }
              },
              "RelVersion" : {
               "icon" : {
                   "image" : "css/images/appversion_16x.png"
               }
              },             
              "CompVersion" : {
               "icon" : {
                   "image" : "css/images/component_ver_16x.png"
               }
              },
              "Category" : {
               "icon" : {
                   "image" : "images/bluetag.png"
               }
              },              
              "Action" : {
                  "icon" : {
                      "image" : "images/action.png"
                  }
              },
              "Procedure" : {
                  "icon" : {
                      "image" : "css/images/proc.png"
                  }
              },
              "Function" : {
                  "icon" : {
                      "image" : "css/images/func.png"
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
              "ServerCompType" : {
               "icon" : {
                   "image" : "images/component.png"
               }
              },
              "User" : {
                  "icon" : {
                      "image" : "css/images/user_16x.png"
                  }
              },
              "Usergroup" : {
                  "icon" : {
                      "image" : "css/images/group_16x.png"
                  }
              },
              "BUILDER" : {
                  "icon" : {
                      "image" : "css/images/buildserv_16x.png"
                  }
              },
              "BUILDJOB" : {
                  "icon" : {
                      "image" : "images/buildjob_16x.png"
                  }
              }
          }
      },
      "crrm" : { 
          "move" : {
              "check_move" : function (m) {
            	console.log("in check_move");
            	if(m.p=="before"|| m.p=="after") return false;
            	var sid = m.o.attr("id").substr(2);
            	console.log("sid="+sid)
            	var st=m.o.attr("rel").toUpperCase();
            	var tt=m.np.attr("rel").toUpperCase();
            	st=(st=="LIFECYCLE")?"DOMAIN":st;
            	tt=(tt=="LIFECYCLE")?"DOMAIN":tt;
            	console.log("Moving "+st+" to "+tt);
              	if(st=="REPOSITORY" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="SERVER" && tt=="ENVIRONMENT") return true;	// valid
      		  	if(st=="DOMAIN" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="ENVIRONMENT" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="USER" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="ACTION" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="PROCEDURE" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="FUNCTION" && tt=="DOMAIN") return true;	// valid
      		  	if(st=="COMPONENT" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="USERGROUP" && tt=="DOMAIN" && sid>2) return true;	// valid if not "Everyone" or "Administrators"
    		  	if(st=="SERVER" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="NOTIFY" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="TEMPLATE" && tt=="NOTIFY") return true;	// valid
    		  	if(st=="DATASOURCE" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="CREDENTIALS" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="SERVERCOMPTYPE" && tt=="DOMAIN") return true;	// valid
    		  	if(st=="COMPONENT" && tt=="DOMAIN") return true;	// valid
      		  	console.log("invalid!");
       		  	return false;	// invalid
              }
          }
      },
      "core" : {"html_titles" : true,  "load_open" : true, "animation": 50 },
      "plugins" : plugins,
      "ui": {"select_limit" : 1},
      "cookies" : {
    	  "save_opened"    : "jstree_open_"+nodename.substr(1),
    	  "save_selected"  : "jstree_selected_"+nodename.substr(1),
    	  "save_loaded"    : "jstree_load_"+nodename.substr(1),
    	  "cookie_options" : { "path" : "/dmadminweb" }
      }
      });
      $(document).unbind("drag_start.vakata");
      $(document).unbind("drag_stop.vakata");
      $(document).bind("drag_start.vakata", function (e, data) {
		  if(data.data.jstree) {
		    console.log("Drag start (1)");
		    Dragging=true;
		  }
		}); 
	  $(document).bind("drag_stop.vakata", function (e, data) {
		  if(data.data.jstree) {
		    console.log("Drag end (1)");
		    Dragging=false;
		  }
		});
      
      $(nodename).dmContextMenu(targetmenu, {theme:'xp'});
      $(nodename).bind("hover_node.jstree", function (e, data) 
      {
         lastobj = data.rslt.obj;
         inTree=true;
      });
            
      $(nodename).bind("reopen.jstree", function (e, data) 
      {
    	  console.log("LOADING COMPLETE");
      });
      
      $(nodename).bind("dehover_node.jstree", function (e, data) 
      {
    	 lastobj = data.rslt.obj;
    	 inTree=false;
      });
      
      $(nodename).bind("rename_node.jstree", function (e, data)
      {
    	  jsRenameNode(frameid,admin,data); 
      });
      
      $(nodename).bind("select_node.jstree", function (e, data) 
      {
    	console.log("select_node.jstree entry");
    	console.log(e);
    	console.log(data);
    	var id = data.rslt.obj.attr("id");
    	console.log("id="+id+" selid="+selid);

    	if (selid) {
    		// cookie will reselect old values after we've selected our node
    		// this fires select_node again. If we're not selecting the node
    		// we want then deselect it again.
    		if (selid != id && !inTree) {
    			console.log("select_node: selid="+selid+" deselecting "+id);
    			$(nodename).jstree("deselect_node",data.node);
    			selid=null;
    		}
    	}
    	console.log("select_node enter (id="+id+")");
        var ds=false;
        var odl="";
    	if (data.rslt.obj.attr("rel")!="Domain" && data.rslt.obj.attr("rel")!="Lifecycle") {
    		var parent=data.inst._get_parent(data.rslt.obj);
    		lastdomain=0;
    		if (parent != -1) {
    			while (parent != -1) {
    				sep=(odl.length>0)?" ":"";
    				odl=odl+sep+parent.attr("id");
    				if (lastdomain==0 && (parent.attr("rel")=="Domain" || parent.attr("rel")=="Lifecycle")) lastdomain=parent.attr("id").substr(2); // OTID
    				parent=data.inst._get_parent(parent);
    			}
    		} else {
    			lastdomain=1; // Global
    		}
    	} else {
    		lastdomain=id.substr(2);	// already selected a domain
    		ds=true;
    	}
    	var selection="";
    	if ($(nodename+' .jstree-clicked').length == 1) {
    	    selection = $(nodename+' .jstree-clicked').text();
    	}
    	var ct = nodename.substring(1, nodename.length-5)+"_tab";
    	console.log("lastdomain="+lastdomain+" odl="+odl+" inTree="+inTree+" ds="+ds+" selection="+selection);
    	var objtype=id.substr(0,2);
    	var cs=(objtype=="cc" || objtype=="cp" || objtype=="cy");
    	if (!ds && !cs && selection.length>0) {
    	  var otid=data.rslt.obj.attr("id");
    	  var image = $("#" + id + " > a > ins.jstree-icon").css("background-image");
    	  if (image) image=image.replace(/"/g,"'");
    	  console.log("image="+image);
    	  var addLink=HighlightFooterLink(id);
    	  if (addLink) {
    		  AddToFooter(
    				"<div id=\"f"+otid+"\" style=\"float:left; cursor: pointer; background-image:"
  		    		+image
  		    		+";background-repeat: no-repeat;background-position:0px 2px;padding:0px 3px 0px 16px;\" "
  		    		+"<a href=\"javascript:void(0);\" "
  		    		+"onClick='chgsel({t: \""+ct+"\", id: \""+otid+"\", odl: \""+odl+"\", tm: \""+currentmenu+"\"})'>"+selection+"|</a></div>"
    		  );  
    	  }
    	}
    	console.log("select_node exits (id="+id+")");
      }); 
      
      $(nodename).bind("create_node.jstree", function (e, data) {
    	  // fired whenever a new node is created - cnc is set if we've created a new category node
	     console.log("Node created - name = "+data.rslt.obj.text()+" id="+data.rslt.obj[0].id);
	     console.log(data);
	     console.log("cnc:");
	     console.log(cnc);
	     if (cnc != null) {
	    	 if (typeof cnc.data.rslt != "undefined") {
	    		 console.log("drag and drop - id="+cnc.data.rslt.o[0].id);
	    		 SwitchDisplay(cnc.data.rslt.o[0].id)
	    	 } else {
	    		 console.log("paste or new item id="+cnc.data.id);
	    		 SwitchDisplay(cnc.data.id);
	    	 }
	     }
	  });
      
      $(nodename).bind("remove.jstree", function (e, data) 
      {
    	  console.log("remove fired: "+data.rslt);
    	  jsRemoveNode(frameid,admin,data); 
    	  return true;
      }); 
     
      $(nodename).bind("select_node.jstree", function(evt, data)
      {
    	  console.log("Tree -> Select Node ("+nodename+") lastobj="+lastobj+" lastSelectedNode="+lastSelectedNode);
    	  console.log(lastobj);
    	  eventSelectNode(frameid,admin,data);
      });
      
      $(nodename).bind("open_node.jstree close_node.jstree", function (evt, data) {
    	    var state = evt.type == "open_node" ? "open" : "closed";
    	    /*
    	    console.log("type="+data.rslt.obj.attr("rel")+" id="+data.rslt.obj.attr("id")+" "+state+" ic="+ic);
    	    if (ic != null) {
    	    	console.log("ic="+ic);
    	    	if ($("#"+ic).length) {
    	    		// Exists in tree - select it
    	    		console.log("exists in tree - selecting");
    	    		$(currenttree).jstree('select_node',"#"+ic);
    	    		// ic = null;
    	    	}
    	    }
    	    */
      });
      
      $(nodename).bind('move_node.jstree',function(event,data){

      
      // .o - the node being moved 
      // .r - the reference node in the move 
      // .ot - the origin tree instance 
      // .rt - the reference tree instance 
      // .p - the position to move to (may be a string - "last", "first", etc) 
      // .cp - the calculated position to move to (always a number) 
      // .np - the new parent 
      // .oc - the original node (if there was a copy) 
      // .cy - boolen indicating if the move was a copy 
      // .cr - same as np, but if a root node is created this is -1 
      // .op - the former parent 
      // .or - the node that was previously in the position of the moved node 

      	MoveNode(data);
      
     
  });
}

function Refresh(nodename)
{
	// Is anything selected? - if so suppress the reselect event
	var sel = $(nodename).jstree('get_selected').attr('id');
	if (typeof sel == "undefined") {
		selid = sel;
	}
	
	
	console.log("Refresh("+nodename+") - sel="+sel);
	ExplorerRefreshing = (sel && (sel.length > 0));
	
	// Refresh all open nodes
	var n = new Array();
	$(nodename).find("> ul > li").each(function (i) {
		$(this).find("> ul > li").each(function (i2) {
			if ($(this).hasClass("jstree-open")) {
				n[$(this).attr("rel")+$(this).attr("id")]='Y';
			}
			if ($(this).hasClass("jstree-leaf")) {
				$(this).removeClass("jstree-leaf");
				$(this).addClass("jstree-closed");
			}
		});
	});
	$(nodename).find("> ul > li").each(function (i) {
		$(this).find("> ul > li").each(function (i2) {
			if (n[$(this).attr("rel")+$(this).attr("id")]=='Y') {
				$.jstree._reference($(nodename)).refresh(this,-1);
			}
		});
	});
 if (sel != "")
  $('a', '#'+sel).click();
}

function RefreshName(nodename,newname)
{
	var sel = $(nodename).jstree('get_selected').attr('id');
	ExplorerRefreshing = (sel && (sel.length > 0));
	if (ExplorerRefreshing) {
		$(nodename).jstree('set_text',"#" + sel,newname);
	}
	$("#footer_container").children().each(function(i){
    	if (this.id == "f"+sel) {
    		$(this).text(newname);
    	}
      });
}
