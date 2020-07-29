 <%--
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
 --%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<head>

<style type="text/css">
.context-menu-theme-xp .context-menu-item-disabled {
	color:#A7A7A7;
}
.context-menu-item {
	padding:1px; !important
}
.tmain     {white-space:nowrap;cursor: pointer;padding: 0 5px 5px 16px; <c:if test="${em != true}">opacity: 0.5;</c:if>}
.ingroup   {font-weight: bold;}
.denygroup {text-decoration:line-through;}

#leftpanel   {float: left; width: 280px; height:95%;}
#toppanel    {float: left; width: 280px; height:49%;}
#bottompanel {float: left; width: 280px; height:49%;}
#spacer      {width: 10px;}
#access      {float: left; height:100%; clear:both;} 
#midspacer   {float: left; width: 100%; height:10px;clear:both;}
#${vaid}     {width: 250px; overflow-y:scroll; text-align: left;}
#${uaid}     {float: left; width: 250px; overflow-y:scroll; text-align: left;}
#${waid}     {float: left; width: 250px; overflow-y:scroll; text-align: left;}
#${raid}     {float: left; width: 250px; overflow-y:scroll; text-align: left;}
#${agid}_group     {float: left; width: 250px; overflow-y:scroll; text-align: left;}
h4           {line-height: 16px;margin: 0 0 0.4em;}
</style>
</head>
<body>
<div id="access" style="padding-left:8px"><h3 style="font-weight:normal">Groups in <strong>bold</strong> are inherited from parent domains. Groups with a line remove the inherited access.</h3>
	<div id="leftpanel">
		<div id="toppanel">
			<div id="${vaid}">
				<h4 style="padding-left:6px">View Access</h4>
				<c:forEach items="${inviewlist}" var="vl">
				<div id="${vaid}${vl.id}" class="tmain ingroup <c:if test="${em != true}">disablegroup</c:if>">${vl.name}</div>
				</c:forEach>
				<c:forEach items="${denyviewlist}" var="vl">
				<div id="${vaid}${vl.id}" class="tmain denygroup">${vl.name}</div>
				</c:forEach>
				<c:forEach items="${viewlist}" var="vl">
				<div id="${vaid}${vl.id}" class="tmain group">${vl.name}</div>
				</c:forEach>			
			</div>
			<div id="spacer">&nbsp;</div>
			<div id="${uaid}">
				<h4 style="padding-left:6px">Change Access</h4>
				<c:forEach items="${inupdatelist}" var="ul">
				<div id="${uaid}${ul.id}" class="tmain ingroup">${ul.name}</div>
				</c:forEach>
				<c:forEach items="${denyupdatelist}" var="ul">
				<div id="${uaid}${ul.id}" class="tmain denygroup">${ul.name}</div>
				</c:forEach>
				<c:forEach items="${updatelist}" var="ul">
				<div id="${uaid}${ul.id}" class="tmain group">${ul.name}</div>
				</c:forEach>
			</div>
			<div id="spacer">&nbsp;</div>		
			<div id="bottompanel">
		</div>
		<c:if test="${readtitle != null}">
			<div id="${raid}">
				<h4 style="padding-left:6px">${readtitle}</h4>
				<c:forEach items="${inreadlist}" var="rl">
				<div id="${raid}${rl.id}" class="tmain ingroup">${rl.name}</div>
				</c:forEach>
				<c:forEach items="${denyreadlist}" var="rl">
				<div id="${raid}${rl.id}" class="tmain denygroup">${rl.name}</div>
				</c:forEach>
				<c:forEach items="${readlist}" var="rl">
				<div id="${raid}${rl.id}" class="tmain group">${rl.name}</div>
				</c:forEach>
			</div>
			<div id="spacer">&nbsp;</div>
		</c:if>
		<c:if test="${writetitle != null}">
			<div id="${waid}">
				<h4 style="padding-left:6px">${writetitle}</h4>
				<c:forEach items="${inwritelist}" var="wl">
				<div id="${waid}${wl.id}" class="tmain ingroup">${wl.name}</div>
				</c:forEach>
				<c:forEach items="${denywritelist}" var="wl">
				<div id="${waid}${wl.id}" class="tmain denygroup">${wl.name}</div>
				</c:forEach>
				<c:forEach items="${writelist}" var="wl">
				<div id="${waid}${wl.id}" class="tmain group">${wl.name}</div>
				</c:forEach>
			</div>
		</c:if>	
		</div>
	</div>	
	<div id="${agid}_group">
	<h4 style="padding-left:6px">Available Groups</h4>
	<c:forEach items="${grouplist}" var="gl">
	<div id="${agid}${gl.id}" class="tmain group">${gl.name}</div>
	</c:forEach>
	</div>
</div>


<script type="text/javascript">
var mainmenu = [
	{'Edit': function(menuItem,menu) { EnterEditMode(this); } },
	{'Save': function(menuItem,menu) { LeaveEditMode(this); } }
];

var DraggableOptions = {
	// cancel : "a.ui-icon", // clicking an icon won't initiate dragging
	revert : "invalid", // when not dropped, the item will revert back to its initial position
	containment : "document",
	helper : "clone",
	cursor : "move"
};

function SetGroupsDraggable()
{
	$(".group").draggable(DraggableOptions);
	$(".ingroup").draggable(DraggableOptions);
	$(".denygroup").draggable(DraggableOptions);
}

function MoveGroupOut($item)
{
<c:choose>
<c:when test="${em == true}">
	console.log("MoveGroupOut");
	var fromdiv=$item.attr("id").substr(0,2);
	console.log("fromdiv="+fromdiv+"agid=${agid}");
	if (fromdiv != "${agid}" && fromdiv != "${agid}_group") {
		var id=$item.attr("id").substr(2);
		var divid=$item.attr("id").substr(0,2);
		var tid=divid+""+id;
		console.log("GetAccess?f=rg&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}");
		$.ajax({
			async: true,
			url: "GetAccess?f=rg&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}",
			type: 'POST',
			dataType: "json",
			success: function(data) {
				console.log("data="+JSON.stringify(data));
				if ($item.hasClass("denygroup")) {
					console.log("deny permission being dragged");
					// $item.removeClass("denygroup");
					// $item.addClass("ingroup");
					$item.fadeOut(200).remove();
				}
				else
				if ($item.hasClass("ingroup")) {
					console.log("inherited permission being dragged");
					var DoNotAdd=false;
					$("#"+divid).children().each(function( index ) {
						if ($(this).attr("id") == tid) {
							// found matching id indiv
							if ($(this).hasClass("denygroup")) {
								DoNotAdd=true;
								return false;
							}
						}
					});
					if (!DoNotAdd) {
						var clone = $item.clone();
						clone.attr("id",divid+""+id);
						clone.draggable(DraggableOptions);
						clone.removeClass("ingroup");
						clone.addClass("denygroup");
						clone.appendTo("#"+divid).fadeIn(200);
					}
					
				} else {
					$item.fadeOut(200).remove()
				}
			}
		});
	}
</c:when>
<c:otherwise>
	return false;
</c:otherwise>
</c:choose>
}

function MoveGroupIn(divid,$item)
{
<c:choose>
<c:when test="${em == true}">
	var id=$item.attr("id").substr(2);
	var fromdiv=$item.attr("id").substr(0,2);
	var tid=divid+""+id;
	console.log("MoveGroupIn divid="+divid+" fromdiv="+fromdiv);
	if (fromdiv != divid) {
		var add=true;
		var deny=false;
		var rd=false;
		console.log("tid="+tid+" divid="+divid+" agid=${agid}_group");
		console.log("GetAccess?f=ag&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}");
		$("#"+divid).children().each(function( index ) {
			if ($(this).attr("id") == tid) {
				// found matching id indiv
				if ($(this).hasClass("denygroup")) {
					console.log("Already a deny in this div - removing the deny");
					$(this).remove();
					rd=true;
				} 
			}
		});
		
		$("#"+divid).children().each(function( index ) {
			if ($(this).attr("id") == tid) {
				// found matching id indiv
				if ($(this).hasClass("ingroup") && !rd) {
					console.log("Inherited group in this div - adding it as DENY");
					deny=true;
					add=true;
					return false;
				} else {
					add=false;
					return false;
				}
			}
		});
		
		console.log("add="+add+" deny="+deny);
		if (add) {
			console.log("GetAccess?f=ag&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}&st="+(deny?"N":"Y"));
			$.ajax({
				async: true,
				url: "GetAccess?f=ag&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}&st="+(deny?"N":"Y"),
				type: 'POST',
				dataType: "json",
				success: function(data) {
					console.log("data="+JSON.stringify(data));
					if (data.added == "Y") {
						var DoNotAdd=false;
						$("#"+divid).children().each(function( index ) {
							if ($(this).attr("id") == tid) {
								// found matching id indiv
								if ($(this).hasClass("denygroup")) {
									DoNotAdd=true;
									return false;
								}
							}
						});
						if (!DoNotAdd) {
							var clone = $item.clone();
							clone.attr("id",divid+""+id);
							clone.draggable(DraggableOptions);
							clone.appendTo("#"+divid).fadeIn(200);
						}
					}
					if (fromdiv!="${agid}_group") {
						console.log("Calling MoveGroupOut for the item (not from all groups)");
						MoveGroupOut($item);
					}
					if (deny) {
					 if (typeof clone != "undefined")
					  {
						clone.removeClass("group");
						clone.addClass("denygroup");
					  }	
					}
				}
			});
		}
		if (rd) {
			console.log("Replacing a strikethrough");
			$.ajax({
				async: true,
				url: "GetAccess?f=rg&gid="+id+"&t="+divid+"&id=${obj.id}&objtype=${obj.objectTypeAsInt}&din=${din}",
				type: 'POST',
				dataType: "json",
				success: function(data) {
					console.log("data="+JSON.stringify(data));
				}
			});
		}
	}
</c:when>
<c:otherwise>
	return false;
</c:otherwise>
</c:choose>
}
	
$(document).ready(function () {
	<c:if test="${em == true}">
	SetGroupsDraggable();
	</c:if>
	
	$("#${agid}_group").droppable({
		drop : function(event, ui)
		{
			MoveGroupOut(ui.draggable);
		}
	});
	$("#${vaid}").droppable({
		drop : function(event, ui)
		{
			MoveGroupIn("${vaid}",ui.draggable);
		}
	});
	$("#${uaid}").droppable({
		drop : function(event, ui)
		{
			MoveGroupIn("${uaid}",ui.draggable);
		}
	});
	$("#${raid}").droppable({
		drop : function(event, ui)
		{
			MoveGroupIn("${raid}",ui.draggable);
		}
	});
	$("#${waid}").droppable({
		drop : function(event, ui)
		{
			MoveGroupIn("${waid}",ui.draggable);
		}
	});
	console.log("GetAccess.jsp - document ready");
});
</script>
</body>