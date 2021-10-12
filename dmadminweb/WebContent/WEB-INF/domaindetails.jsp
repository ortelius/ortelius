<%--
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
--%>
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
<body>
<script>
function ptype_chg()
{
	var cv = $("#ptype").val();
	if (cv=="dropdown") $("#dan").show();
	else $("#dan").hide();
}

function varnameUsed(varname)
{
	var res=false;
	$("#tptable tr").each(function(i, tr) {   
		var v = $.trim($(this).find("td").eq(2).html());
		if (v == $.trim(varname)) {
			res=true;
		}
	});
	return res;
}

function validVarname(varname,oldpos)
{
	console.log("validVarname, oldpos="+oldpos);
	var vn=$.trim(varname);
	if (vn=="") {
		alert("Variable Name must be specified");
		return false;
	}
	var tn=vn.replace(/[0-9A-Za-z_]/g,"");
	console.log("tn=["+tn+"] vn=["+vn+"]");
	if (tn!="" || (vn.charAt(0)>='0' && vn.charAt(0)<='9')) {
		alert("Variable "+vn+" is invalid");
		return false;
	}
	var used=false;
	$("#tptable tr").each(function(i, tr) {
		var p = $.trim($(this).find("td").eq(0).html());
		var v = $.trim($(this).find("td").eq(2).html());
		console.log("vn=["+vn+"] v=["+v+"] p="+p+" oldpos="+oldpos);
		if (v == vn && (oldpos == 0 || (oldpos>0 && p != oldpos))) {
			used=true;
		}
	});
	if (used) {
		alert("Variable "+vn+" already in use");
		return false;
	}
	return true;
}

function AddParamButton()
{
	 var pwd = parent.$("#modal");
	 var buttons = [{
			text: "Ok",
			click: function() {
				if (validVarname($("#pvar").val(),0)) {
					$.ajax({
						async: true,
						url: "DomainDetails?f=atp&l="+$("#plabel").val()+"&v="+$("#pvar").val()+"&t="+$("#ptype").val()+"&an="+$("#parr").val()+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
						type: 'POST',
						dataType: "json",
						success: function(data) {
							parent.$("#modal").dialog("close");
							LoadParameters(SelectedTask.substr(1));
						}
					});
				}
			}
		},
		{
			text: "Cancel",
			click: function() {
				parent.$("#modal").dialog("close");
			}
		}];
	 pwd.empty();
	 var h="<table border=0 style=\"width:100%\">";
	 h=h+"<tr><td style=\"width:200px;\">Label</td><td><input id=\"plabel\" width=\"40\"></td></tr>";
	 h=h+"<tr><td>Variable</td><td><input id=\"pvar\" width=\"40\"></td></tr>";
	 h=h+"<tr><td>Type</td><td>";
	 h=h+"<select onchange=\"javascript:ptype_chg();\" id=\"ptype\" width=\"40\">";
	 h=h+"<option value=\"entry\">Entry</option>";
	 h=h+"<option value=\"password\">Password</option>";
	 h=h+"<option value=\"dropdown\">Dropdown</option>";
	 h=h+"</select>";
	 h=h+"</td></tr>";
	 h=h+"<tr id=\"dan\" style=\"display:none;\"><td>Dropdown Array Name</td><td><input id=\"parr\" width=\"40\"></td></tr>";
	 h=h+"</table>";
	 pwd.html(h);
	 pwd.dialog({
		modal : true,
	  	open : function(event, ui) {
	  	}
	 });
	 pwd.dialog("option", "title", "Add Task Parameter");
	 pwd.dialog("option", "width", "300px");
	 pwd.dialog("option", "height", "250");
	 pwd.dialog("option", "buttons", buttons);
	 pwd.dialog('open');
}

function EditParamButton()
{
	console.log("edit");
	$("#tptable tr.ui-tp-highlight").each(function(i, tr) {   
		var pos = $.trim($(this).find("td").eq(0).html());
		var label = $.trim($(this).find("td").eq(1).html());
		var varname = $.trim($(this).find("td").eq(2).html());
		var typedesc = $.trim($(this).find("td").eq(3).html());
		console.log("pos="+pos);
		console.log("label="+label);
		console.log("varname="+varname);
		console.log("typedesc="+typedesc);
		var arrname = "";
       
		if (label == "&nbsp;") return;
       	
		var pwd = parent.$("#modal");
		var buttons = [{
        	text : "Ok",
        	click : function() {
        		console.log("Calling validVarname, pos="+pos);
        		if (validVarname($("#pvar").val(),pos)) {
	        		$.ajax({
						async: true,
						url: "DomainDetails?f=etp&p="+pos+"&l="+$("#plabel").val()+"&v="+$("#pvar").val()+"&t="+$("#ptype").val()+"&an="+$("#parr").val()+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
						type: 'POST',
						dataType: "json",
						success: function(data) {
							parent.$("#modal").dialog("close");
							LoadParameters(SelectedTask.substr(1));
						}
					});
	        		parent.$("#modal").dialog("close");
        		}
        	}
       	},{
			text : "Cancel",
        	click : function() {
				parent.$("#modal").dialog("close");
        	}
       	}];
		
       var tdedit = "<form id=\"tpform\"><table id=\"tptab\">";
       tdedit=tdedit+"</table></form>";
       
       pwd.empty();
       var h="<table border=0 style=\"width:100%\">";
       h=h+"<tr><td style=\"width:200px;\">Label</td><td><input id=\"plabel\" value=\""+label+"\" width=\"40\"></td></tr>";
       h=h+"<tr><td>Variable</td><td><input id=\"pvar\" value=\""+varname+"\" width=\"40\"></td></tr>";
       h=h+"<tr><td>Type</td><td>";
       h=h+"<select onchange=\"javascript:ptype_chg();\" id=\"ptype\" width=\"40\">";
       var sel1="";
       var sel2="";
       var sel3="";
       if (typedesc.toLowerCase()=="entry") sel1=" selected";
       else
       if (typedesc.toLowerCase()=="password") sel2=" selected";
       else {
    	   sel3=" selected";
    	   arrname = typedesc.substr(11);	// take off "dropdown / "
       }
       console.log("sel1="+sel1);
       console.log("sel2="+sel2);
       console.log("sel3="+sel3);
       h=h+"<option value=\"entry\""+sel1+">Entry</option>";
       h=h+"<option value=\"password\""+sel2+">Password</option>";
       h=h+"<option value=\"dropdown\""+sel3+">Dropdown</option>";
       h=h+"</select>";
       h=h+"</td></tr>";
       h=h+"<tr id=\"dan\" style=\"display:none;\"><td>Dropdown Array Name</td><td><input id=\"parr\" width=\"40\" value=\""+arrname+"\"></td></tr>";
       h=h+"</table>";
       pwd.html(h);
       pwd.dialog({
    	   	modal : true,
    	   	open : function(event, ui) {
  	  		}
       });
       pwd.dialog("option", "title", "Edit Task Parameter");
       pwd.dialog("option", "width", "300px");
       pwd.dialog("option", "height", "250");
       pwd.dialog("option", "buttons", buttons);
       pwd.dialog('open');
       ptype_chg();
   });
}

function DelParamButton()
{
	$("#tptable tr.ui-tp-highlight").each(function(i, tr) {   
		console.log("got a row");
		var varname = $.trim($(this).find("td").eq(2).html());
		console.log("deleting varname "+varname);
		$.ajax({
			async: true,
			url: "DomainDetails?f=dtp&v="+varname+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
			type: 'POST',
			dataType: "json",
			success: function(data) {
				LoadParameters(SelectedTask.substr(1));
			}
		 });
	});
}

function ChangeParamOrder()
{
	 var listItems =$ ("#tptable").find('tr');
	 for (k=0;k<listItems.length;k++) {
		 if (k>0) {
			 // skip header row
			 var li = listItems[k];
			 var id = $(li).find("td").eq(0).html();
			 var varname = $(li).find("td").eq(2).html();
			 console.log("k="+k+" id="+id);
			 if (k!=id) {
				 // Change of position
				 $.ajax({
					async: true,
					url: "DomainDetails?f=ctp&v="+varname+"&np="+k+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
					type: 'POST',
					dataType: "json",
					success: function(data) {					}
				 });
			 }
		 }
	 }
}

function DownParamButton()
{
	var rowToMove = $("#tptable").find('tr.ui-tp-highlight:first');
	var next = rowToMove.next('tr.clickablerow');
	if (next.length == 1) {
		next.after(rowToMove);
		ChangeParamOrder();
	}
}

function UpParamButton()
{
	console.log("up");
	var rowToMove = $("#tptable").find('tr.ui-tp-highlight:first');
	console.log("rowToMove.length="+rowToMove.length);
	var prev = rowToMove.prev('tr.clickablerow');
	console.log("prev.length="+prev.length);
	if (prev.length == 1) { 
		prev.before(rowToMove);
		ChangeParamOrder();
	}
}

function LoadParameters(tid)
{
	$("#task_param_content").html("");
	$.ajax({
		async: true,
		url: "DomainDetails?f=gp&domainid=${domain.id}&tid="+SelectedTask.substr(1),
		type: 'GET',
		dataType: "json",
		success: function(data) {
			console.log("parameters are:");
			console.log(data);
			console.log(data.length);
			if (data.length > 0) {
				var t="<table id=\"tptable\"  style=\"width:100%;\"><tr style=\"background-color:#d4dde7;border-bottom: 2px solid #c6ccd2\"><td><b>Label</b></td><td><b>Variable</b></td><td><b>Type</b></td></tr>";
				for (var i=0;i<data.length;i++) {
					t=t+"<tr class=\"clickablerow\"><td style=\"display:none\">"+data[i].pos+"</td><td style=\"border-right:1px solid #ddd\" align=\"left\" style=\"width:200px\">"+data[i].label+"</td>";
					t=t+"<td style=\"border-right:1px solid #ddd\" align=\"left\" style=\"width:120px\">"+data[i].var+"</td>";
					if ($.trim(data[i].type.toLowerCase())=="dropdown") {
						t=t+"<td style=\"border-right:1px solid #ddd\" align=\"left\">dropdown / "+data[i].arr+"</td></tr>";
					} else {
						t=t+"<td style=\"border-right:1px solid #ddd\" align=\"left\">"+data[i].type+"</td></tr>";
					}
				}
				t=t+"</table>";
				$("#task_param_content").html(t);
				var $table = $("#tptable");
				$table.delegate("tr", "click", function() {
					console.log("clicked");
					$table.find("tr.ui-tp-highlight").removeClass('ui-tp-highlight');
					$(this).addClass('ui-tp-highlight');
				    // $("edit_button").button("option", "disabled", false);
				    // $("delete_button").button("option", "disabled", false);
				});
			}
		}
	});
}

function over()
{
	CurrentTask = $(this).attr("id");
	if (CurrentTask != SelectedTask)
	{
		$("#hl"+CurrentTask.substr(1)).css("background-color","#e7f4f9");
	}
}

function out()
{
	CurrentTask="";
	if ($(this).attr("id") != SelectedTask)
	{
		$("#hl"+$(this).attr("id").substr(1)).css("background-color","white");
	}
}

function MoveDomGroupIn($item)
{
	console.log("adding group="+$item.attr("id")+" to task "+SelectedTask.substr(1));
	$.ajax({
		async: true,
		url: "DomainDetails?f=ag&gid="+$item.attr("id").substr(1)+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
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
	console.log("removing group="+$item.attr("id")+" from task "+SelectedTask.substr(1));
	$.ajax({
		async: true,
		url: "DomainDetails?f=rg&gid="+$item.attr("id").substr(1)+"&domainid=${domain.id}&tid="+SelectedTask.substr(1),
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

function taskSummarySaved(newname) {
	// var tp = $("#details h4");
	// tp.html(newname);
	console.log("taskSummarySaved(newname="+newname+")");
	console.log("SelectedTask="+SelectedTask);
	var x=false;
	$("#panel_deploy h1").each(function(i) {
		if (!x) {
			$(this).html(newname);
			x=true;
		}
	});
	$("#"+SelectedTask).html(newname);
	for (n=0;n < TaskList.length;n++) {
		if (TaskList[n].id == SelectedTask.substring(1)) {
			TaskList[n].name = newname;
		}
	}
}



 function SetTasksGroupsDraggable()
 {
  var DraggableOptions = {
   	// cancel : "a.ui-icon", // clicking an icon won't initiate dragging
   	revert : "invalid", // when not dropped, the item will revert back to its initial position
   	containment : "document",
   	helper : "clone",
   	cursor : "move"
   };
  
  $(".icon_inline_Group").draggable(DraggableOptions);
 }
 
function Loaded()
{
 $("#panel_deploy").show();
}

function seltask()
{
	if (SelectedTask!="") 
	  $("#hl"+SelectedTask.substr(1)).css("background-color","white");
	SelectedTask=$(this).attr("id");
	$("#hl"+SelectedTask.substr(1)).css("background-color","#96ebff");
	$("#tasks_assigned_groups").html("");
	$("#tasks_available_groups").html("");

	var tid = SelectedTask.substr(1);
	// Display Groups (ga, gna)
	console.log("Getting group data");
	$.ajax({
		dataType: "json",
		type: 'POST',
		url: "DomainDetails",
		data: "f=gg&domainid=${domain.id}&tid="+tid,
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
            SetTasksGroupsDraggable();
		}
	});
	console.log("Getting task info");
	$("#del_task_button").css("color", "buttontext");
	var tt = $("#"+SelectedTask).attr("class");
	tt=tt.replace(/icon_inline_/,"");
	tt=tt.replace(/adjacent_icon/,"");
	tt=tt.replace(/ /,"");
	tt=tt.toLowerCase();
	var hv =  	"<div class=\"panel_container_title_area tasks\" style=\"overflow:hidden\"><h1 class=\"taskpanels\">Task Detail</h1>"
	<c:if test="${domain.updatable}">
	 			+ "<button style=\"display:none\" class=\"edit_button_sub_small\" onClick=\"javascript:EditSummaryButton()\"><i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit</button>"
	</c:if>
	 			+ "</div><div class=\"deploy_sub_panel_container\"><table id=\"task_summ\" class=\"dev_table\"><tbody></tbody></table></div>";
	$("#panel_deploy").html(hv);
	<c:if test="${domain.updatable}">
	$("#taskparam_edit").show();
	</c:if>
    LoadSummaryData("task_summ",obj2Int["ta"][0],"task",tid,"&suff=task&cbfunc=taskSummarySaved");
    LoadParameters(tid);
    $(".edit_button_sub_small").show();
    $("#title_icon").show();
    <c:if test="${domain.updatable}">
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
	</c:if>
}

function AddTask(tasktype)
{
 $("#new_task_dropdown").hide();
 
	$.ajax({
		async: true,
		url: "DomainDetails?f=at&domainid=${domain.id}&tasktype="+tasktype,
		type: 'POST',
		dataType: "json",
		success: function(data) {
			$.each(data, function(key, val) {
				if (key == "name")
				  name = val;
				if (key == "id") id = val;
			});
	        var t = new Object();
	        t.name = name;
	        t.id = id;
            TaskList.push(t);
			$("#sub_panel_topleft .subpanel").append("<div id=\"hl"+id+"\"><li class=\"icon_inline_"+tasktype+" adjacent_icon\" id=t"+id+">"+name+"</li></div>");
			$("#t"+id).hover(over,out);
			$("#t"+id).click(seltask);
			$("#t"+id).trigger('click');
		}
	});
}

function removeFromArray(arr, item) {
 for(var i = arr.length; i--;) {
     if(arr[i].name === item) {
         arr.splice(i, 1);
     }
 }
}

function DeleteTask()
{

	$.ajax({
		async: true,
		url: "DomainDetails?f=dt&domainid=${domain.id}&tid="+SelectedTask.substr(1),
		type: 'POST',
		dataType: "json",
		success: function(data) {
            removeFromArray(TaskList,$("#"+SelectedTask).html());
			$("#"+SelectedTask).remove();
			$("#hl"+SelectedTask.substr(1)).remove();
			$("#panel_deploy").html("<div class=\"panel_container_title_area tasks\"><h1 class=\"taskpanels\">Task Detail</h1><button style=\"display:none\" class=\"edit_button_sub_small\" onClick=\"javascript:EditSummaryButton()\"><i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i>Edit</button></div><div class=\"deploy_sub_panel_container\"><table id=\"task_summ\" class=\"dev_table\"><tbody></tbody></table></div>");
			SelectedTask="";
			$("#del_task_button").css("color","grey");
		}
	});
}

var delmenu=[
{"Delete this Task":      {icon:"images/delete.png",onclick:function(menuItem,menu){DeleteTask();} }}
];

var taskmenu=[
{"Approve Application Verision for move to next pipeline stage":            {icon:"images/approve.png",onclick:function(menuItem,menu){AddTask("Approve");} }},
{"Move Application Version to the next or previous stage in the pipeline":       {icon:"images/movever.png",onclick:function(menuItem,menu){AddTask("Move");} }},
{"Delete all Application Versions from all Endpoints in an Environment": {icon:"images/remove.gif",onclick:function(menuItem,menu){AddTask("Remove");} }},
{"Deploy Application Version to an Environment":             {icon:"images/deploy.gif",onclick:function(menuItem,menu){AddTask("Deploy");} }},
{"Request Calendar entry for deployment to an Environment":  		    {icon:"images/request.png",onclick:function(menuItem,menu){AddTask("Request");} }},
{"Create new Application Version":     {icon:"images/appver.png",onclick:function(menuItem,menu){AddTask("CreateVersion");} }},
{"Mannually trigger Action to be executed":	        {icon:"images/action.png",onclick:function(menuItem,menu){AddTask("RunAction");} }}
];

function getmenu()
{

}

function toggleTaskMenu()
{
 var taskdd = $("#new_task_dropdown");
 var showing = taskdd.is(":visible");
 
 console.log("showing " + showing);
 if (showing)
  taskdd.hide();
 else
  taskdd.show();
}

$(function()
{	
	<c:choose>
	<c:when test="${domain.updatable == true}">
	$("#sub_panel_topleft").dmContextMenu(getmenu, {theme:'xp'});
	</c:when>
	<c:otherwise>
	$("#sub_panel_topleft").bind('contextmenu', function(e) {
		return false;
	});
	</c:otherwise>
	</c:choose>
	<c:forEach items="${tasklist}" var="tl">
	$("#t${tl.id}").hover(over,out);
	$("#t${tl.id}").click(seltask);
	</c:forEach>
	SelectedTask="";
	
	$("#panel_deploy").html("<div class=\"panel_container_title_area tasks\"><h1 class=\"taskpanels\">Task Detail</h1><button style=\"display:none\" class=\"edit_button_sub_small\" onClick=\"javascript:EditSummaryButton()\"><i class=\"fal fa-pencil\" aria-hidden=\"true\" style=\"padding-right:5px\"></i>Edit</button></div><div class=\"deploy_sub_panel_container\"><table id=\"task_summ\" class=\"dev_table\"><tbody></tbody></table></div>");
	$("#task_params").html(
			"<div class=\"panel_container_title_area\">"
			+ "<h1 class=\"taskpanels\">Additional Parameters</h1>"
			+     "<div style=\"display:none\" id=\"taskparam_edit\">"
			+          "<button class=\"edit_button_param\" onClick=\"javascript:EditParamButton();\"><i class=\"fal fa-pencil\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i></button>"
	        +          "<button class=\"add_button_param\" onClick=\"javascript:AddParamButton();\"><i class=\"fal fa-plus\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i></button>"
	        +          "<button class=\"delete_button_param\" onClick=\"javascript:DelParamButton();\"><i class=\"fal fa-trash\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i></button>"
	        +          "<button class=\"up_button_param\" onClick=\"javascript:UpParamButton();\"><i class=\"fal fa-arrow-up\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i></button>"
	        +          "<button class=\"down_button_param\" onClick=\"javascript:DownParamButton();\"><i class=\"fal fa-arrow-down\" aria-hidden=\"true\"  style=\"padding-right:5px\"></i></button>"
	        +     "</div>"
	        +"</div>");
}); // end anonymous function

TaskList = new Array();
var t;

<c:forEach items="${tasklist}" var="tl">
  t = new Object();
  t.name = "${tl.name}";
  t.id   = "${tl.id}";
  TaskList.push(t);
</c:forEach>

</script>
</body>
</html>
