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

var ioc=0;


$(document).ready(function()
{
// if (licType == "OSS")
//  return;

 $("#verttab_action").before("<button id=\"verttab_release\" class=\"tablinks verttab_rel\" onclick=\"openList(event, 'release')\">Releases</button>");
 $("#grouplist_buttons > button").after("<button class=\"title_buttons\" onclick=\"addRow(event, 'group')\"><i class=\"fal fa-plus\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Add</button>" +
                                        "<button class=\"title_buttons\" onclick=\"delRow(event, 'group')\"><i class=\"fal fa-trash\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Delete</button>");
 $("#tabs-General").after("<li id=\"tabs-PackageApplications\" onclick=\"ActivateSubTabs()\" style=\"display:none\">Package Applications</li>");

 var cal = "<div id=\"calendar-section\">Calendar</div><div id=\"tabs-Calendar-data\" class=\"div_scroll\">" +
"   <div id=\"dialog-form\" title=\"Create new Event\" class=\"dmdialog\" style=\"display:none\">" +
"    <p class=\"validateTips\">All form fields are required.</p>" +
"    <form>" +
"     <fieldset>" +
"      <label for=\"lstart\">Start Time</label>" +
"      <div style=\"display: inline-block;\">" +
"       <input type=\"text\" id=\"lstart\" value=\"\" />" +
"      </div>" +
"      <br> <br> <label for=\"lend\">End Time</label>" +
"      <div style=\"display: inline-block;\">" +
"       <input type=\"text\" id=\"lend\" value=\"\" />" +
"      </div>" +
"      <hr>" +
"      <div id=\"buttons\" align=\"center\">" +
"       <input type=\"radio\" name=\"sr\" id=\"a\" value=\"Reserved\" onClick=\"da(false);\" /><label for=\"a\">Reserved</label>" +
"       <input type=\"radio\" name=\"sr\" id=\"b\" value=\"Unavailable\" onClick=\"da(true);\" /><label for=\"b\">Unavailable</label>" +
"       <input type=\"radio\" name=\"sr\" id=\"c\" value=\"Auto\" onClick=\"da(false);\" /><label for=\"c\">AutoDeploy</label>" +
"      </div>" +
"      <hr>" +
"      <label for=\"ename\">Event Name</label> <br>" +
"      <input type=\"text\" autofocus name=\"ename\" id=\"ename\" value=\"\" class=\"text ui-widget-content ui-corner-all\" /><br>" +
" " +
"         <label for=\"deploytype\" id=\"deploytypelabel\">Deploy Type</label><br>" +
"      <select name=\"deploytype\" id=\"deploytype\" style=\"width: 350px;\" class=\"text ui-widget-content ui-corner-all\" onChange=\"ToggleRelApp()\">" +
"       <option val=\"application\">Application</option>" +
"       <option val=\"release\" selected>Release</option>" +
"      </select><br>" +
" " +
"      <span id=\"appsec\" style=\"display:none;\">" +
"         <label for=\"app\" id=\"applabel\" >Application</label><br>" +
"         <select name=\"app\" id=\"app\" style=\"width: 350px;\" class=\"text ui-widget-content ui-corner-all\" ></select>" +
"      </span>" +
" " +
"      <span id=\"relsec\">" +
"         <label for=\"rel\" id=\"rellabel\">Release</label><br>" +
"         <select name=\"rel\" id=\"rel\" style=\"width: 350px;\" class=\"text ui-widget-content ui-corner-all\"></select>" +
"       </span>" +
"         <br>" +
"      <label for=\"desc\">Description</label><br>" +
"      <input type=\"text\" name=\"desc\" id=\"desc\" value=\"\" class=\"text ui-widget-content ui-corner-all\" />" +
"     </fieldset>" +
"    </form>" +
"   </div>" +
"      <div id=\"dialog-form-upgrade\" title=\"Create new Event\" class=\"dmdialog\" style=\"display:none\">" +
"    <h2 style=\"padding-right:30px\">Please upgrade to the Pro version in order to schedule events on the calendar.</h2>" +
"    <form>" +
"    </form>" +
"   </div>" +
"   <div id=\"calendar\"></div>" +
"   </div></div>";

 if ($("#tabs-Calendar-data").length == 0)
 	$("#audit-access").before(cal);

 var packApp = "<div id=\"tabs-PackageApplications-data\" style=\"display:none\">" +
"    <div class=\"panel_container versionsandinformation\">" +
"      <div class=\"panel_container appversions\">" +
"        <div id=\"innerappver\" style=\"height:600px;width:100%;\">" +
"       </div>" +
"      </div>" +
"     <div class=\"accordionpanel\">" +
"      <h4 class=\"ui-widget-header\">Component Versions</h4>" +
"      <div id=\"applicationversionlist\" align=\"left\" style=\"margin:0\"></div>" +
"     </div>" +
"    </div>" +
"   </div>";

 $("#tabs-PackageComponents-data").after(packApp);

var relpane = "      <div id=\"rellist_pane\">" +
"        <div id=\"rellist_pane_title\" style=\"display:inline-block;\">Releases</div>" +
"          <div id=\"rellist_buttons\" style=\"display:inline-block;\">" +
"            <button class=\"title_buttons\" onclick=\"openList(event, 'release')\"><i class=\"fal fa-refresh\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Refresh</button>" +
"             <button class=\"title_buttons\" onclick=\"addRow(event, 'release')\"><i class=\"fal fa-plus\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Add Base</button>" +
"             <button class=\"title_buttons\" onclick=\"addRow(event, 'relversion')\"><i class=\"fal fa-plus\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Add Version</button>" +
"             <button class=\"title_buttons\" onclick=\"delRow(event, 'release')\"><i class=\"fal fa-trash\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Delete</button>" +
"             <div class=\"dropdown_menu\">" +
"               <button class=\"title_buttons taskMenuButton\" onclick=\"taskMenu()\"><i class=\"fal fa-tasks\" aria-hidden=\"true\" style=\"padding-right:5px;\"></i>Tasks<i class=\"fal fa-caret-down fa-lg\" aria-hidden=\"true\" style=\"padding-left:5px;\"></i></button>" +
"               <div class=\"dropdown_menu-content\">" +
"               </div>" +
"             </div>" +
"           </div>" +
"           <div id=\"rellist_filter_area\" style=\"height:25px;\">" +
"                  <div id=\"rel_search_box_container\"></div>" +
"          </div>" +
"         <div id=\"rellist_list\">  " +
"    <table id=\"rellist\" class=\"hover row-border nowrap\" height=\"100%\" width=\"100%\">" +
"           <thead>" +
"            <tr>" +
"                <th></th>" +
"    <th style=\"text-align: left;\">Version</th> " +
"    <th style=\"text-align: left;\">Domain</th>" +
"    <th style=\"text-align: left;\">Parent</th>" +
"    <th style=\"text-align: left;\">Environment</th> " +
"    <th style=\"text-align: left;\">Last Deployment</th>" +
"    <th style=\"text-align: left;\">Completed</th> " +
"    <th style=\"text-align: left;\">Result</th>" +
"    <th style=\"text-align: left;\">domainid</th>" +
"    <th style=\"text-align: left;\">appid</th>" +
"            </tr>" +
"          </thead>" +
"         </table>" +
"        </div>" +
"      </div>";
$("#complist_pane").after(relpane);

});

function hookSummary(tablename, objtypeAsInt, objtype, objid, addParams)
{
 if (objtype == "ap" || objtype == "av")
 {
  LoadDefectData("defect", objtypeAsInt, objtype, objid, "");
 }
 else if (objtype == "co" || objtype == "cv")
 {
  LoadDefectData("defect", objtypeAsInt, objtype, objid, "");
 }
 else if (objtype == "rl" || objtype == "rv")
 {
//  $("#defect-panel-20").show();
//  LoadDefectData("defect-20", objtypeAsInt, objtype, objid, "");
 }
 else if (objtype == "en")
 {
//  $("#defect-panel-20").show();
//  LoadDefectData("defect-20", objtypeAsInt, objtype, objid, "");
 }
}

function hookList(listName)
{
 if (listName == "release")
 {
  getRelList("list");
 }
}

function hookTabs(currenttree)
{
 if (currenttree == "#releases_tree" && lastSelectedNode.substring(0, 2) == "rv")
 {
  $("#panel_container_right").hide();
  $("#tabs-Versions").hide();
 }
}

function hookActivateSubTabs(classname)
{
 if (classname == "release version")
  classname = "relversion";

 return classname;
}

function hookTabId(tabId, classname)
{
 if (tabId == "tabs-PackageApplications")
 {
  if ((objtype == "rv" || objtype == "rl") && currenttree == "#releases_tree")
  {
   $("#panel_container_right").show();
   if (objtype == "ap" || objtype == "co")
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#header_icon").html("<h1 class=\"" + classname + "\">" + objtypeName + ":  " + objName + "</h1>");
   }
   else
   {
    parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
    parent.$("#header_icon").html("<h1 class=\"" + classname + "\">" + objtypeName + ": " + objName + "</h1>");
   }
   $("#tabs-PackageApplications-data > div > div.accordionpanel > h4").html("Application Versions");
   parent.$("#title_icon").html("");

   LoadApplicationVersionsData();
  }
  else
  {
   parent.$("#right_panel_title_area").html("<div id=\"right_panel_buttons\"></div>");
   parent.$("#title_icon").html("");
   $("#tabs-PackageApplications-data").html(
     "<div class=\"panel_container versionsandinformation\">" + "<div class=\"panel_container appversions\">" + "<div id=\"innerappver\" style=\"height:600px;width:100%;\">" + "</div>" + "</div>"
     + "<div class=\"accordionpanel\" id=\"ag\">" + "<h4 class=\"ui-widget-header\" style=\"margin:0\" >Application Versions</h4><div id=applicationversionlist align=\"left\" style=\"width:256px;overflow-x:auto;height:97%;\"></div>" + "</div>" + "</div>");

   parent.$("#title_icon").html("");
  }
 }
}

function hookHide()
{
 $("#releases_tab").hide();
}

function hookAddRow(type)
{
 if (type == "release")
 {
  currenttree = "#releases_tree";
  objtypeName = "Release";
  NewRelease(null);
 }
 else if (type == "relversion")
 {
  objid = lastSelectedNode.substr(2); //OTID, first two chars are Object Type
  objtype = lastSelectedNode.substr(0,2);
  objkind="";
  currenttree = "#releases_tree";
  objtypeName = "Release";
  NewRelVer(objid);
 }
}

function hookEventOpenRow(frameid)
{
 var tree = "#" + frameid + "_tree";
 if (frameid == "releases")
   tree = "#releases_tree";

 return tree;
}

function hookTaskMenu(currenttree)
{
 if (currenttree == "#releases_tree")
 {
  var data = rellist_table.rows({selected:  true}).data();

  if (!dropdownShowing && data != null && data.length > 0)
  {
   var id = lastsel_id.substr(2);
   $.ajax({
    url: "GetMenu?t=RELEASE&id="+id+"&d=0&a=N&ct=releases",
    async: false,
    cache: false,
    context: document.body
   }).done(function(data) {
   newhtml = "";
    $.each(data,function(key,val){
     $.each(val,function(key2,val2){
      var tmo = {};
      var tmo = {};
      console.log("key2="+key2+" val2="+val2);
      newhtml += "<a onclick='callback(\"" + currenttree + "\",\"" + key2 + "\",\"" + val2 + "\",this);'>" + key2 + '</a>';
      $("#rellist_buttons > div > div").html(newhtml);
      $("#rellist_buttons > div > div").css({display: "block"});
      dropdownShowing = true;
     });
    });
   });
  }
  else
  {
   $("#rellist_buttons > div > div").css({display: "none"});
   dropdownShowing = false;
  }
 }
}

function hookDelRow(type)
{
 var data = null;
 if (type == "release")
  data = rellist_table.rows({selected:  true}).data();

 return data;
}

function CreateCalendar(envid)
{
	console.log(">>> CreateCalendar for envid "+envid+" gotoDataEnvId="+gotoDateEnvId);
	if (gotoDate != null && envid != gotoDateEnvId && typeof $calendar != "undefined") {
		console.log(">>> BAILING");
		$calendar.fullCalendar("destroy");	// remove old calendar (prevents events from firing)
		return;	// ignore this env
	}
 var date = new Date();
 var d = date.getDate();
 var m = date.getMonth();
 var y = date.getFullYear();

 StartEndTimesChanged = false;
 $calendar = $("#calendar");
 $calendar.html("");
 $schedule = $("#schedule");
 if (hideOSS == "Y")
  $dialogform = $("#dialog-form-upgrade");
 else
 {
  $dialogform = $("#dialog-form");
// console.log("Setting de-DE");
// Globalize.culture( "de-DE" );
// console.log( Globalize.culture().name );

 $("#calstart").val(CalendarStartTime);
 $("#calend").val(CalendarEndTime);

 $( "#calstart" ).timespinner({spin: function(event,ui) {SpinStart(event,ui);}});
 $( "#calend" ).timespinner({spin: function(event,ui) {SpinEnd(event,ui);}});
 var url = "GetEnvOwner?envid=" + envid;
 console.log(url);

 $.ajax(
   {
    url : url,
    dataType : 'json',
    async: false,
    type : 'GET',
    success : function(res)
    {
     envOwner = res.isowner;
    }
   });


 $( "#calstart").timespinner( "value", "${css}");
 $( "#calend").timespinner( "value", "${ces}");

 ename = $( "#ename" ); // Event Name
 app = $( "#app" );  // Application
 desc = $( "#desc" ); // description

 $("#app").load("GetApplicationOptions?envid=" + objid + "&isRelease=N");

 $("#rel").load("GetApplicationOptions?envid=" + objid + "&isRelease=Y");

 allFields = $( [] ).add( ename ).add( app );

 tips = $( ".validateTips" );
 }

 $dialogform.dialog({
       autoResize: true,
       autoOpen:false,
       modal:true,
       width: "auto",
       height: "auto",
       resizable:false,
       buttons: [{
       id: "submit",
          click: function() {
           // function will be set depending on whether it's a new event or modifying an existing event
          }
         },
         {
          text: "Cancel",
          click: function() {
           console.log("Attempting to close (1)");
             $( this ).dialog( "close" );
          }
         }],
       close: function() {
        if (typeof allFields !== 'undefined')
         allFields.val( "" ).removeClass( "ui-state-error" );
       }
 });

 $("#buttons").buttonset();

 // var dtformat = getLocaleDateString();
 console.log("getUserTimeString returns "+getUserTimeString());
 $calendar.fullCalendar("destroy");	// remove old calendar (prevents events from firing)
 calendar = $calendar.fullCalendar(
 {
  header :
  {
   left : 'prev,next today',
   center : 'title',
   right : 'month,agendaWeek,agendaDay'
  },
  columnFormat :
  {
   month : 'ddd', // Mon
   week : 'ddd ' + ShortDateFormat(), // Mon 9/7
   day : 'dddd ' + ShortDateFormat() // Monday 9/7
  },
  firstDay : 1,
  theme : true,
  selectable : true,
  selectHelper : true,
  timeFormat :
  {
   // for agendaWeek and agendaDay
   agenda : getUserTimeString() + "{ - " + getUserTimeString() + "}",  // 'H:mm{ - H:mm}', // 5:00 - 6:30

   // for all other views
   '' : getUserTimeString() // 'H:mm' // 24 hr
  },
  axisFormat : getUserTimeString(), // 'H:mm',
  eventSources : [
  {
   url : 'GetCalendarData',
   type : 'POST',
   data :
   {
    envid : objid
   },
   error : function(jqXhr, testStatus, errorThrown)
   {
    alert('there was an error while fetching events!\nstatus: ' + status + '\nerror: ' + errorThrown);
   }
  } ],
  eventRender : function(event, element)
  {

   console.log("eventRender event.description=" + event.description + " id=" + event.id);
   if (event.eventtype == "Unavailable")
   {
    element.find('.fc-event-title').append("<br/>" + event.description);
   }
   else
   {
    element.find('.fc-event-title').append("<br/>" + event.application + "<br/>" + event.description);
   }

   console.log(event);
   console.log("********************************");
   console.log("allDay=", event.allDay);
   console.log("event.id=" + event.id);
   console.log("appid=" + event.appid);
   console.log("title=" + event.title);
   console.log("editable=" + event.editable);
   console.log("color=" + event.color);
   console.log("textColor=" + event.textColor);
   console.log("deployid=" + event.deployid);
   console.log("unavailable=" + event.unavailable);
   console.log("description=" + event.description);
   console.log("application=" + event.application);
   console.log("EventType=" + event.eventtype);
   console.log("start=" + event.start);
   console.log("end=" + event.end);
   console.log("pending=" + event.pending);
   console.log("********************************");
   if (event.end == null) {
		var est = new Date(event.start);
		var eet = new Date(est.getTime());
		eet.setTime(eet.getTime() + 900); // 15 min default
		event.end = eet;
   }
   if (event.id > 0) {
		if (event.end < new Date() || event.eventtype=='Deployment') {
			console.log("in the past, setting draggable to false");
			event.editable = false;
			if (event.deployid == 0) {
				element.dmContextMenu(onpasteventmenu,{theme : 'xp'});
			} else {
				element.dmContextMenu(ondeploymenu,{theme : 'xp'});
			}
		} else {
			console.log("in the future");
			if (!event.editable) {
				console.log("Not editable - setting draggable to false");
				event.editable = false;
				element.dmContextMenu(onpasteventmenu,{theme : 'xp'});
			} else {
				element.dmContextMenu(onfutureeventmenu,{theme : 'xp'});
			}
		}
   }
   else if (event.id == -1)
   {
    console.log("temporary");
    element.dmContextMenu(ontempmenu,
    {
     theme : 'xp'
    });
   }
   else
   {
    console.log("unavailable - draggable = false");
    event.editable = false;
   }
  },
  eventMouseover : function(event, jsevent, view)
  {
   console.log("Mouse over event, event id=" + event.id);
   OverEvent = true;
   LastEvent = event;
  },
  eventMouseout : function(event, jsevent, view)
  {
   console.log("Mouse out");
   OverEvent = false;
  },

  eventClick : function(calEvent, jsEvent, view)
  {
   // nothing yet, maybe delete this event?
  },
  eventResize : function(event, dayDelta, minuteDelta, revertFunc, jsEvent, ui, view)
  {
   UpdateEvent(event, revertFunc);
  },
  eventDrop : function(event, dayDelta, minuteDelta, allDay, revertFunc)
  {
   var now = new Date();
   console.log("eventDrop");
   if (event.start < now)
   {
    revertFunc();
   }
   else
   {
    UpdateEvent(event, revertFunc);

   }
  },

  loading : function(a, b)
  {
	  ioc+=(a?1:-1);
	  console.log(">>>>>>> LOADING a="+a+" ioc="+ioc);
   if (a == false)
   {
	   if (ioc==0 && gotoDate != null) {
		   // End of last rendering event, switch focus
		   // gotoDate is in format YYYY/MM/DD
		   // Where MM is 0-11
		   var parts = gotoDate.split("/");
		   gotoDate=null;
		   $calendar.fullCalendar('gotoDate',parts[0],parts[1],parts[2]);
	   }
    // $calendar.dmContextMenu(eventmenu, {theme:'xp'});
    $calendar.mousedown(function()
    {
     if (!OverEvent)
     {
      // if (CurrentMenu)
      // {
      // CurrentMenu.hide();
      // CurrentMenu = null;
      // }
      $calendar.fullCalendar('removeEvents', -1); // remove all the "temp"
      // events (those with an id of
      // -1)
     }
    });
   }
  },

  select : function(start, end, allDay)
  {
   var now = new Date();
   if (end < now)
   {
    calendar.fullCalendar('unselect');
    return;
   }
   console.log("start=" + start);
   console.log("end=" + end);
   var sd = new Date(start);
   var ed = new Date(end);
   LastStartEvent = Math.floor(sd.getTime() / 1000);
   LastEndEvent = Math.floor(ed.getTime() / 1000);
   console.log("LastStartEvent=" + LastStartEvent);
   console.log("LastEndEvent=" + LastEndEvent);

   LastAllDay = allDay;
   console.log("Setting LastCalendar to calendar");
   LastCalendar = calendar;

   console.log("Getting client events");
   var events = $calendar.fullCalendar('clientEvents');
   console.log("got client events");
   for (i = 0; i < events.length; i++)
   {
    console.log("Checking event[" + i + "].id=" + events[i].id + " unavailable=" + events[i].unavailable);
    if (!(typeof events[i].id === "undefined") && events[i].eventtype == "Unavailable")
    {
     var est = events[i].start;
     var eet = new Date((events[i].end == null) ? est.getTime() : events[i].end);
     if (events[i].allDay)
     {
      eet.setTime(eet.getTime() + 86399000); // put end time to 23:59:59 on same
      // day
     }
     console.log("events[" + i + "].start=" + events[i].start);
     console.log("events[" + i + "].end=" + events[i].end);
     console.log("events[" + i + "].allDay=" + events[i].allDay);
     if ((start >= est && start < eet) || (end > est && end <= eet) || (est > start && eet <= end))
     {
      $calendar.fullCalendar('unselect');
      return;
     }
    }
   }
   //
   // If we get here we've selected something. Render it here
   //
   $calendar.fullCalendar('unselect'); // what we've just selected
   $calendar.fullCalendar('removeEvents', -1); // remove all the "temp" events
   // (those with an id of -1)
   $calendar.fullCalendar('renderEvent',
   {
    id : -1,
    title : "",
    start : LastStartEvent,
    end : LastEndEvent,
    allDay : LastAllDay,
    application : "",
    description : "",
    editable : true
   }, true // sticky, we've not saved to the DB yet....
   );
  },
  editable : true,
  minTime : CalendarStartTime,
  maxTime : CalendarEndTime,
  slotMinutes : 15,
  lazyFetching : false,
  defaultView : 'agendaWeek'
 });
}

function da(x)
{
 if (x) {
  console.log("disabling app");
  $('#app').attr("disabled", 'disabled').addClass('ui-state-disabled');
  $('#applabel').addClass('ui-state-disabled');
  $('#rel').attr("disabled", 'disabled').addClass('ui-state-disabled');
  $('#rellabel').addClass('ui-state-disabled');
  $('#deploytype').attr("disabled", 'disabled').addClass('ui-state-disabled');
  $('#deploytypelabel').addClass('ui-state-disabled');
 } else {
  console.log("enabling app");
  $('#app').attr("disabled", false).removeClass('ui-state-disabled');
  $('#applabel').removeClass('ui-state-disabled');
  $('#rel').attr("disabled", false).removeClass('ui-state-disabled');
  $('#rellabel').removeClass('ui-state-disabled');
  $('#deploytype').attr("disabled", false).removeClass('ui-state-disabled');
  $('#deploytypelabel').removeClass('ui-state-disabled');
 }
}

function ChangeStartEnd(t, nt)
{
 console.log("nt=" + nt);
 var m = "s";
 if (t == 2)
  m = "e";
 var p = "m=" + m + "&envid=" + objid + "&nt=" + nt;
 $.ajax(
 {
  url : "UpdateAvailability",
  type : "POST",
  data : p
 }).done(function()
 {
  console.log("posted");
 });
 if (t == 1)
  spinTimer1 = null;
 if (t == 2)
  spinTimer2 = null;
}

function onfutureeventmenu(m, t)
{
 var ma = [];
 ma.push(
 {
  'Delete this Event' :
  {
   icon : "images/delete.png",
   onclick : function(menuItem, menu)
   {
    DeleteEvent(this);
   }
  }
 });
 ma.push(
 {
  'Modify this Event' :
  {
   icon : "images/edit.png",
   onclick : function(menuItem, menu)
   {
    ModifyEvent(this);
   }
  }
 });
 if (LastEvent.pending && envOwner)
 {
  ma.push(
  {
   'Approve this Event' :
   {
    icon : "images/edit.png",
    onclick : function(menuItem, menu)
    {
     ApproveEvent(this);
    }
   }
  });
 }
 return ma;
};

function DeleteEvent()
{
 var p = "id=" + LastEvent.id + "&m=D";
 $.ajax(
 {
  url : "ModifyEvent",
  type : "POST",
  data : p
 }).done(function(data)
 {
  calendar.fullCalendar('removeEvents', data.id);
  calendar.fullCalendar("rerenderEvents");
 });
}

function UpdateEvent(event, revertFunc)
{
 var sd = new Date(event.start);
 var ed = new Date(event.end);
 var s = Math.floor(sd.getTime() / 1000);
 var e = Math.floor(ed.getTime() / 1000);
 var p = "m=m&id=" + event.id + "&s=" + s + "&e=" + e + "&ad=" + (event.allDay ? "Y" : "N");
 console.log("ModifyEvent Update p=" + p);
 $.ajax(
 {
  url : "ModifyEvent",
  type : "POST",
  data : p
 }).done(function(data)
 {
  if (!data.success)
  {
   revertFunc();
  }
  else
  {
   console.log("in success post function");
   $calendar.fullCalendar('removeEvents', LastEvent.id);
   console.log("Event " + LastEvent.id + " removed - rendering event (id=" + data.id + ") appname=[" + data.application + "]");
   $calendar.fullCalendar('renderEvent',
   {
    id : data.id,
    title : data.title,
    start : data.start,
    end : data.end,
    allDay : data.allDay,
    application : data.application,
    appid : data.appid,
    color : data.color,
    textColor : data.textcolor,
    description : data.description,
    editable : true
   }, false);
  }
 });
}

function SetupAvailability()
{
 // plugin buttonset vertical
 console.log("Setting up buttonsetv");
 $.fn.buttonsetv = function()
 {
  console.log("Doing vertical button set thing");
  $(':radio, :checkbox', this).wrap('<div style="margin: 1px"/>');
  $(this).buttonset();
  $('label:first', this).removeClass('ui-corner-left').addClass('ui-corner-top');
  $('label:last', this).removeClass('ui-corner-right').addClass('ui-corner-bottom');
  mw = 0; // max width
  $('label', this).each(function(index)
  {
   w = $(this).width();
   if (w > mw)
    mw = w;
  });
  $('label', this).each(function(index)
  {
   $(this).width(mw);
  });
  $('input', this).each(function(index)
  {
   if ($(this).val() == "${cu}")
   {
    $(this).click();
   }
  });
 };
 console.log("Done setting up buttonsetv - calling");
 $("#availbuttonset").buttonsetv();
 console.log("end of call");
}

function Updateability(calendar)
{
 var events = calendar.fullCalendar('clientEvents');
 // console.log("events.length="+events.length);
 for (i = 0; i < events.length; i++)
 {
  // console.log("events["+i+"].start="+events[i].start+"
  // events["+i+"].end="+events[i].end);
  for (x = 0; x < events.length; x++)
  {
   // console.log("Comparing to events["+x+"].start="+events[x].start+"
   // events["+x+"].end="+events[x].end);
   if (i != x && events[x].start >= events[i].start && events[x].end <= events[i].end)
   {
    // console.log("Overlap - deleting record "+x+ "(_id="+events[x]._id+")");
    calendar.fullCalendar('removeEvents', events[x]._id);

   }
   if (i != x && events[x].start >= events[i].start && events[x].start <= events[i].end && events[x].end >= events[i].end)
   {
    // console.log("Extending - deleting record "+x);
    // console.log("Extending record "+i);
    events[i].end = events[x].end;
    calendar.fullCalendar('updateEvent', events[i]);
    calendar.fullCalendar('removeEvents', events[x]._id);
    events[x].start = 0;
    events[x].end = 0;
   }
  }
 }
 var events = calendar.fullCalendar('clientEvents');
 var c = 0;
 for (var i = 0; i < events.length; i++)
 {
  if (typeof events[i].start !== 'undefined')
   c++;
 }

 var p = "m=a&envid=" + objid + "&c=" + c + "&";
 var n = 0;
 for (var i = 0; i < events.length; i++)
 {
  if (i > 0)
   p += "&";
  if (typeof events[i].start !== 'undefined')
  {
   var sd = new Date(events[i].start);
   var ed = new Date(events[i].end);
   var start = (sd.getTime() / 1000) - 345600;
   var end = (ed.getTime() / 1000) - 345600;
   p = p + "start" + n + "=" + start + "&end" + n + "=" + end;
   n++;
  }
 }
 $.ajax(
 {
  url : "UpdateAvailability",
  type : "POST",
  data : p
 }).done(function()
 {
  console.log("posted");
 });
}

function CreateSchedule()
{
 var date = new Date();
 var d = date.getDate();
 var m = date.getMonth();
 var y = date.getFullYear();

 var h = $(window).height();

 var calendar = $schedule.fullCalendar(
 {
  header : null,
  columnFormat :
  {
   week : 'ddd' // Mon/Tue etc
  },
  selectable : true,
  selectHelper : true,
  height : (h - 110),
  timeFormat :
  {
   // for agendaWeek and agendaDay
   agenda : 'H:mm{ - H:mm}', // 5:00 - 6:30

   // for all other views
   '' : 'H:mm' // 24 hr
  },
  axisFormat : 'H:mm',
  year : 1970,
  month : 0,
  date : 5, // Monday 5th Jan 1970 is first full week.
  firstDay : 1,
  theme : false,
  eventSources : [

  // your event source
  {
   url : 'GetCalendarData',
   type : 'POST',
   data :
   {
    envid : objid,
    schedule : "Y"
   },
   error : function()
   {
    alert('there was an error while fetching events!');
   },
   color : 'black', // a non-ajax option
   textColor : 'white' // a non-ajax option
  }

  // any other sources...

  ],
  eventDrop : function(event, dayDelta, minuteDelta, allDay, revertFunc)
  {
   Updateability(calendar);
  },

  eventResize : function(event, dayDelta, minuteDelta, revertFunc)
  {
   console.log("Updateability");
   Updateability(calendar);
  },

  select : function(start, end, allDay)
  {
   var sd = new Date(start);
   var ed = new Date(end);
   // 345600 is num of secs from 1/1/1970 to 5/1/1970 (start of calendar)
   var StartEvent = (sd.getTime() / 1000) - 345600;
   var EndEvent = (ed.getTime() / 1000) - 345600;

   calendar.fullCalendar('renderEvent',
   {
    title : "Unavailable",
    start : start,
    end : end,
    allDay : allDay
   }, false // dont't want the event to "stick" since it's now in the DB.
   );
   Updateability(calendar);

   calendar.fullCalendar('unselect');
  },
  editable : true,
  lazyFetching : false,
  defaultView : 'agendaWeek'
 });
}

function updateTips(t)
{
 if (t.length > 0)
 {
  tips.text(t).addClass("ui-state-highlight");
  setTimeout(function()
  {
   tips.removeClass("ui-state-highlight", 1500);
  }, 500);
 }
 else
 {
  if (typeof tips !== 'undefined')
   tips.text("").removeClass("ui-state-highlight");
 }
}

function checkLength(o, n, min, max)
{
 if (o.val().length > max || o.val().length < min)
 {
  o.addClass("ui-state-error");
  updateTips("Length of " + n + " must be between " + min + " and " + max + ".");
  return false;
 }
 else
 {
  return true;
 }
}

function checkRegexp(o, regexp, n)
{
 if (!(regexp.test(o.val())))
 {
  o.addClass("ui-state-error");
  updateTips(n);
  return false;
 }
 else
 {
  return true;
 }
}

function checkAppSet(fieldname,appname,n)
{
	console.log("appname.length="+appname.length);
	if (appname.length < 1) {
		$(fieldname).addClass("ui-state-error");
		updateTips(n);
		return false;
	} else {
		return true;
	}
}

function CreateNewEvent(id)
{
 console.log("CreateNewEvent id=" + id);
 var bValid = true;
 allFields.removeClass("ui-state-error");
 bValid = bValid && checkLength(ename, "Event Title", 1, 40);

 var appname = "";
 var appid = -1;
 var type = $("#deploytype").val();
 var appidstr = "";

 var selectedVal = "";
 var selected = $("#buttons input[type='radio']:checked");
 if (selected.length > 0)
 {
  selectedVal = selected.val();
 }
 console.log("selectedVal=" + selectedVal);

 if (selectedVal == "Unavailable")
 {
	 bValid = true;
 }
 else
 {
	 if (type == "Release")
	 {
		 appname = $('#rel').children("option:selected").text();
		 appid = $("#rel").val();
		 appidstr = "rl" + appid;
		 console.log("relname=["+appname+"]");
		 bValid = bValid && checkAppSet('#rel',appname,"Release must be specified");
	 } else {
		 appname = $('#app').children("option:selected").text();
		 appid = $("#app").val();
		 appidstr = "ap" + appid;
		 console.log("appname=["+appname+"]");
		 bValid = bValid && checkAppSet('#app',appname,"Application must be specified");
	 }
 }

 if (bValid)
 {
  console.log("Posting appid=" + appid + " LastStartEvent=" + LastStartEvent + " LastEndEvent=" + LastEndEvent);
  var sd = $("#lstart").datetimepicker('getDate');
  var ed = $("#lend").datetimepicker('getDate');
  var s = Math.floor(sd.getTime() / 1000);
  var e = Math.floor(ed.getTime() / 1000);
  LastStartEvent = s;
  LastEndEvent = e;
  console.log("s="+s+" e="+e);

  $.ajax(
  {
   url : "AddEvent",
   data :
   {
    envid : objid,
    start : LastStartEvent,
    end : LastEndEvent,
    allDay : LastAllDay,
    Title : ename.val(),
    appid : appid,
    et : selectedVal,
    id : id,
    desc : desc.val()
   }
  }).done(function(res)
  {
   console.log("back from ajax call");
   console.log(res);
   var TextColor = "white";
   var BackgroundColor = "#3a87ad";
   if (res.pending)
   {
    BackgroundColor = "#525564";
   }
   else
   {
    if (selectedVal == "Unavailable")
    {
     BackgroundColor = "#525564";
    }
    else if (selectedVal == "Auto")
    {
     BackgroundColor = "#61838F";
    }
   }
   $calendar.fullCalendar('removeEvents', res.id);
   $calendar.fullCalendar('renderEvent',
   {
    id : res.id,
    title : res.title,
    start : LastStartEvent,
    end : LastEndEvent,
    appid: res.appid,
    allDay : LastAllDay,
    application : appname,
    color : BackgroundColor,
    textColor : TextColor,
    description : res.description,
    eventtype : selectedVal,
    editable : true
   }, false);
  });
  $dialogform.dialog("close");
 }
}

function SaveEvent(id)
{
 $calendar.fullCalendar('removeEvents', -1); // remove any temporary event
 CreateNewEvent(id);
}

function ApproveEvent()
{
 console.log("Approve, id =" + LastEvent.id);
 var p = "id=" + LastEvent.id + "&m=A";
 $.ajax(
 {
  url : "ModifyEvent",
  type : "POST",
  data : p
 }).done(function(data)
 {
  console.log("in success post function");
  $calendar.fullCalendar('removeEvents', LastEvent.id);
  console.log("Event " + LastEvent.id + " removed - rendering event (id=" + data.id + ") appname=[" + data.application + "]");
  $calendar.fullCalendar('renderEvent',
  {
   id : data.id,
   title : data.title,
   start : data.start,
   end : data.end,
   allDay : data.allDay,
   application : data.application,
   appid : data.appid,
   color : data.color,
   textColor : data.textcolor,
   description : data.description,
   editable : true
  }, false);
 });
}

function ShowDeploy()
{
	// alert("ShowDeploy "+LastEvent.deployid);
	DisplayDeploy(LastEvent.deployid);
}

function ModifyEvent()
{
 var ButtonText = "";
 var est = new Date(LastEvent.start);
 var eet = new Date((LastEvent.end == null) ? est.getTime() : LastEvent.end);
 var fmtstr;
 if (LastEvent.allDay)
 {
  eet.setTime(eet.getTime() + 86399000); // put end time to 23:59:59 on same day
  fmtstr = userdatefmt.replace(/M/g,"m");
 }
 else
 {
  fmtstr = userdatefmt.replace(/M/g,"m") + " " + usertimefmt.replace(/m/g,"M").replace(/a/g,"t");
 }
 console.log("est = " + est);
 console.log("eet = " + eet);
 console.log("allDay = " + LastEvent.allDay);
 console.log("EventType=" + LastEvent.eventtype);
 console.log("Editable=" + LastEvent.editable);
 console.log("fmtstr=" + fmtstr);

 var sd = new Date(est);
 var ed = new Date(eet);
 $("#lstart").val(sd.format(fmtstr));
 $("#lend").val(ed.format(fmtstr));
 $('#ename').val(LastEvent.title);
 LastStartEvent = Math.floor(est.getTime() / 1000);
 LastEndEvent = Math.floor(eet.getTime() / 1000);
 LastAllDay = LastEvent.allDay;
 console.log("LastStartEvent = " + LastStartEvent);
 console.log("LastEndEvent = " + LastEndEvent);

 $('#desc').val(LastEvent.description);

 var appreltxt = "Release";

  if (typeof LastEvent.appid != "undefined" && (LastEvent.appid.substring(0,2) == "ap" || LastEvent.appid.substr(0,2) == "av"))
   appreltxt = "Application";

 $("select#deploytype option").each(function()
   {
    this.selected = (this.text == appreltxt);
   });


 ToggleRelApp();
 $("select#app option").each(function()
 {
	 if (LastEvent && LastEvent.appid) this.selected = (this.value == LastEvent.appid.substr(2));
 });

 $("select#rel option").each(function()
   {
	 if (LastEvent && LastEvent.appid) this.selected = (this.value == LastEvent.appid.substr(2));
   });

 if (LastEvent.id == -1)
 {
  // Temp event - user is attempting to make it permanent
  if (envOwner)
   ButtonText = "Create Event";
  else
   ButtonText = "Request Time Slot";
 }
 else
 {
  ButtonText = "Modify Event";
 }
 $('#lstart').datetimepicker("destroy");
 $('#lend').datetimepicker("destroy");

 $('#lstart').datetimepicker(
 {
  dateFormat : userdatefmt.replace(/M/g,"m").replace(/yy/g,"y"),	// yy is 4 digit year in date picker
  timeFormat : usertimefmt.replace(/a/g,"t"),						// am/pm text is represented by "t" in timepicker
  showTimepicker : !LastEvent.allDay,
  stepMinute : 15,
  onClose : function(value)
  {
   $('#lstart').val(value);
  }
 });
 $('#lend').datetimepicker(
 {
  dateFormat : userdatefmt.replace(/M/g,"m").replace(/yy/g,"y"),	// yy is 4 digit year in date picker
  timeFormat : usertimefmt.replace(/a/g,"t"),						// am/pm text is represented by "t" in timepicker
  showTimepicker : !LastEvent.allDay,
  stepMinute : 15,
  onClose : function(value)
  {
   $('#lend').val(value);
  }
 });

 if (hideOSS =="Y")
 {
  $dialogform.dialog("option", "title", "Upgrade to Pro");
  $dialogform.dialog("option", "buttons", [
  {
   text : "Upgrade",
   click : function()
   {
    window.open("https://www.deployhub.com/shopping-cart/", "_blank");
   }
  },
  {
   text : "Cancel",
   click : function()
   {
    $dialogform.dialog("close");
    $calendar.fullCalendar('removeEvents', -1);
   }
  } ]);
 }
 else
 {
 if (LastEvent.editable)
 {
  $dialogform.dialog("option", "title", ButtonText);
  $dialogform.dialog("option", "buttons", [
  {
   text : ButtonText,
   click : function()
   {
    SaveEvent(LastEvent.id);
   }
  },
  {
   text : "Cancel",
   click : function()
   {
    $dialogform.dialog("close");
   }
  } ]);
 }
 else
 {
  $dialogform.dialog("option", "title", "View Event");
  $dialogform.dialog("option", "buttons", [
  {
   text : "Cancel",
   click : function()
   {
    $dialogform.dialog("close");
   }
  } ]);
 }
 }

 $dialogform.dialog("open");
 if (LastEvent.eventtype == "Reserved")
  $("#a").click();
 else if (LastEvent.eventtype == "Unavailable")
  $("#b").click();
 else if (LastEvent.eventtype == "Auto")
  $("#c").click();
 else
  $("#a").click(); // default
 $("#buttons").buttonset('refresh');
 var now = new Date();
 if (LastEvent.end < now)
 {
  updateTips("Event ends in the past and cannot be edited");
 }
 else
 {
  if (!LastEvent.editable)
  {
   updateTips("Event has been approved and cannot be edited");
  }
  else
  {
   updateTips("");
  }
 }
}

$.widget("ui.timespinner", $.ui.spinner,
{
 options :
 {
  // seconds
  step : 60 * 15000, // 15 mins
  // hours
  page : 60
 },
 _parse : function(value)
 {
  if (typeof value === "string")
  {
   // already a timestamp
   if (Number(value) == value)
   {
    return Number(value);
   }
   var h = parseInt(value.substr(0, value.indexOf(":")));
   var m = parseInt(value.substr(value.indexOf(":") + 1, 2));
   var hh, mm;
   if (h < 10)
    hh = "0" + h;
   else
    hh = "" + h;
   if (m < 10)
    mm = "0" + m;
   else
    mm = "" + m;
   return +Globalize.parseDate(hh + ":" + mm, "HH:mm");
  }
  return value;
 },
 _format : function(value)
 {
  return Globalize.format(new Date(value), "t");
 }
});

function SpinStart(event, ui)
{
 var d = new Date(ui.value);
 CalendarStartTime = d.getHours() + ":" + d.getMinutes();
 StartEndTimesChanged = true;
 if (spinTimer1)
  clearTimeout(spinTimer1);
 console.log("hours = " + d.getHours());
 console.log("mins = " + d.getMinutes());
 var nt = new Number((d.getHours() * 60) + d.getMinutes());
 spinTimer1 = setTimeout('ChangeStartEnd(1,"' + nt + '");', 1000);
}

function SpinEnd(event, ui)
{
 var d = new Date(ui.value);
 CalendarEndTime = d.getHours() + ":" + d.getMinutes();
 StartEndTimesChanged = true;
 if (spinTimer2)
  clearTimeout(spinTimer2);
 var nt = new Number((d.getHours() * 60) + d.getMinutes());
 spinTimer2 = setTimeout('ChangeStartEnd(2,"' + nt + '");', 1000);
}

function ShortDateFormat()
{
 // var dt = getLocaleDateString();
 var dt = userdatefmt;
 dt = dt.trimChars("y.-/ ");
 dt = dt.replace(/m/g,"M");
 return dt;
}

function getUserTimeString()
{
	var dt = usertimefmt;
	console.log("usertimefmt="+usertimefmt);
	dt = dt.replace(/a/g,"t");	// calendar needs t for am/pm indication
	console.log("dt="+dt);
	return dt;
}

function ToggleRelApp()
{
 var val = $("#deploytype").val();

 if (val == "Release")
 {
  $("#relsec").show();
  $("#appsec").hide();
 }
 else
 {
  $("#relsec").hide();
  $("#appsec").show();
 }
}

function getTableData(res,tablename)
{
 console.log("getTableData");
 data = res.Defects;
 var td="";
 defect_tdedit = "";
 defect_rowcnt = 0;
 var rowcnt = 0;

 data.sort(function (a, b) {
  return a.defectid - b.defectid;
 });

 dups = [];

 for (a = 0; a < data.length; a++)
 {
  var defect = data[a];

  if (dups.includes(defect.defectid))
   continue;

  dups.push(defect.defectid);

  rowcnt++;
  console.log(defect);
  var linkval1="<a href=\""+defect.htmlurl+"\" target=\"_blank\">#"+defect.defectid+"</a>";
  var linkval2="";
  var key = "df" + defect.defectid;
  var val = "df" + defect.defectid;
  var color = "red";

  if (defect.colour != "")
   color = defect.colour;

  defect_tdedit += '<tr id="defect_row_' + rowcnt + '">';
  defect_tdedit += '<td><input type="checkbox" id="defect_edit_cb_' + rowcnt + '" onchange="toggleDefectDelete(this);" />&nbsp;</td>';
  defect_tdedit += '<td><select id="defect_key_' + rowcnt + '" value="' + key + '">' + defectdropdown + '</select></td>';
  defect_tdedit += "<td><input type=\"hidden\" id=\"defect_oldkey_" + rowcnt + "\" value=\"" + key + "\"\></td>";
  defect_tdedit += "<td><input type=\"hidden\" id=\"defect_oldval_" + rowcnt + "\" value=\"" + val + "\"\></td></tr>";
  var comp = "&nbsp;";

  if ('component' in defect)
   comp = defect.component.name;
  if (tablename.includes("-20"))
    td += '<tr><td style="display:none"><input type="checkbox" id="defect_cb_' + rowcnt + '" onchange="toggleDefectDelete(this);" />&nbsp;</td><td style="width:0px">' + linkval1 + '</td><td>' + defect.title + '</td><td>' + comp + '</td><td style="color:'+color+ ';">' + defect.status + '</td></tr>';
  else
    td += '<tr><td><input type="checkbox" id="defect_cb_' + rowcnt + '" onchange="toggleDefectDelete(this);" />&nbsp;</td><td style="width:0px">' + linkval1 + '</td><td>' + defect.title + '</td><td>' + comp + '</td><td style="color:'+color+ ';">' + defect.status + '</td></tr>';

 }
 defect_rowcnt = rowcnt;

 $("#" + tablename + " > tbody").empty().append(td);

 if (!tablename.includes("-20"))
   $("#" + tablename + "-edit > tbody").html(defect_tdedit);

 rowcnt = 0;
 for (var r = 0; r < data.length; r++)
 {
  row = data[r];
  id = row.type + row.id;
  rowcnt++;

  id = $("#defect_oldkey_" + rowcnt).val();
  $("#defect_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 return(td);
}

function LoadDefectData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 $('#defect_header_buttons > button.delete_button').css("color","grey");
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 defect_tdedit = "";
 defect_rowcnt = 0;

 defectdropdown == "";

  var url =  "GetDefects?";

  if (objtype == "ap" || objtype == "av" || objtype == "rl" || objtype == "rv")
    url += "appid="+summSaveobjid+"&a=od";
  else
    url += "compid="+summSaveobjid+"&a=od";

  if (objtype != "en")
  {
  $.ajax(
   {
    url : url,
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     console.log(res);

     res.Defects.sort(function (a, b) {
      return a.defectid - b.defectid;
     });

     defectdropdown = "";
     defectdict = {};
     for (n = 0; n < res.Defects.length; n++)
     {
       defectdropdown += '<option value=\"df' + res.Defects[n].defectid + "\">#" + res.Defects[n].defectid + " - " + res.Defects[n].title + '</option>';
       defectdict[res.Defects[n].defectid] = {'title' : res.Defects[n].title, 'apiurl' : res.Defects[n].apiurl, 'htmlurl' : res.Defects[n].htmlurl, 'status': res.Defects[n].status}
     }
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
  }


 var url = "";
 var servlet = "";

 servlet = "GetDefects";

 if (objtype == "ap" || objtype == "av" || objtype == "rl" || objtype == "rv")
  url = "appid=" + objid;
 else if (objtype == "co" ||objtype == "cv")
  url = "compid=" + objid;
 else if (objtype == "en")
  url = "envid=" + objid;

 console.log("in GetDefects4");

 $.getJSON(servlet, url, function(res) {

 var td=getTableData(res,tablename);
 console.log("adding "+td+" to tablename "+tablename);

 if (td == "")
 {
  var sel = $("#changerequestdatasource_sumrow > td:nth-child(3)").text();
  if (sel == "")
     td = "<tr><td><h1>No Change Request Data Source has been setup.</h1><td></tr>";
  else
     td = "<tr><td><h1>No Change Requests Found.</h1><td></tr>";

  $("#" + tablename + " > tbody").html(td);
 }
 });
}

function SaveDefectData(tablename)
{

 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;

 id = $("#defect_key_" + rowid).val();
 oldid = $("#defect_oldval_" + rowid).val();

 if (id == null || id == oldid)
  return;

 $("#defect_key_" + rowid).val(id);
 $("#defect_oldval_" + rowid).val(id);

 defectid = id.substring(2);
 oldid = oldid.substring(2);

 if (objtype == "av")
  isVersion = "true";
 else
  isVersion = "false";

 if (objtype == "ap" || objtype == "av")
   objname="appid";
 else
   objname = "compid";

 if (oldid != "")
 {
  var url = "";

  if (objtype == "ap" || objtype == "av")
   url="GetDefects?appid="+summSaveobjid+"&a=dd&id="+oldid;
  else
   url = "GetDefects?compid="+summSaveobjid+"&a=dd&id="+oldid;

  $.ajax(
    {
     url : url,
     type : 'GET',
     dataType : "text",
     success : function(res)
     {
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
      s
     }
    });
 }

 var url="GetDefects?" + objname+"="+summSaveobjid+"&a=ad&id="+defectid+"&title="+encodeURI(defectdict[defectid].title)+"&url="+encodeURI(defectdict[defectid].htmlurl)+"&apiurl="+defectdict[defectid].apiurl+"&status="+defectdict[defectid].status;

 $.ajax(
   {
    url : url,
    async : false,
    success : function(res)
    {
    },
    error : function(jqxhr, status, err)
    {
     console.log(status);
     console.log(err);
    }
   });
   });
  LoadDefectData(tablename, summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
}


function DeleteDefectData(key, val,tablename)
{
  key = key.substring(2);

  var url = "";

  if (objtype == "ap" || objtype == "av")
    url="GetDefects?appid="+summSaveobjid+"&a=dd&id="+key;
  else
   url = "GetDefects?compid="+summSaveobjid+"&a=dd&id="+key;

  $.ajax(
  {
   url : url,
   type : 'GET',
   dataType : "text",
   success : function(res)
   {
    LoadDefectData("defect", summSaveobjtypeAsInt, summSaveobjtype, summSaveobjid, summSaveaddParams);
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    s
   }
  });
}

function LoadDefectHistory(divname, objtypeAsInt, objtype, objid)
{
 var container = document.getElementById(divname);
 $("#"+divname).html("");
 var groups = new vis.DataSet();
    groups.add({id: 0, content: "open", className: "openStyle"});
    groups.add({id: 1, content: "closed", className: "closedStyle"});

    objname=(objtype=='co'||objtype=='cv')?"compid":"appid";
 var tgturl=objname+"="+summSaveobjid+"&a=dh";
 console.log("tgturl="+tgturl);
 var items=[];
 var now = new Date();

 // temp
 // now = new Date(now.getTime() - (86400000 * 3));


 $.getJSON("GetDefects", tgturl, function(res) {
  // var rangeStart = new Date(now.getTime()-2592000000); // 30 days in past
  var rangeStart = new Date(now.getTime()-3024000000); // 35 days in past
  /*
  if (res.length>1) {
   if ((res[1].ts * 1000) > rangeStart) rangeStart = (res[1].ts * 1000);
  }
  */
  if (res.length>0) {
   if (res[0].open == 0 && res[0].closed == 0 && res.length>1) {
    ct = (res[1].ts * 1000);
   } else {
    ct = (res[0].ts * 1000);
   }
  } else {
   ct = rangeStart;
  }
  console.log("ct="+ct+" rangeStart="+rangeStart.getTime());
  var tmp = now.getTime() - rangeStart.getTime();
  console.log("tmp="+tmp);

  if (ct>rangeStart.getTime()) {
   var diff = (now.getTime() - ct) / 1000;
   console.log("diff is "+diff);
   if (diff < 259200) {
    // first defect was < 3 days ao
    ct=rangeStart;
    rangeStart = new Date(now.getTime()-302400000); // 3.5 days in past
   } else {
    console.log("Setting rangeStart to ct "+ct);
    rangeStart = ct;
   }
  }

  console.log("rangeStart="+rangeStart);
  console.log("res:");
  console.log(res);
  var options = {
         style:'bar',
         graphHeight: "90px",
         stack:true,
         autoResize: true,
         barChart: {width:35, align:'center', sideBySide:true}, // align: left, center, right
         drawPoints: false,
         dataAxis: {
             icons:false,
             showMinorLabels:true,
             left: {
              range: {min:0},
              format: function(y){return y != Math.round(y)?'':y;}
             }
         },
         orientation:'top',
         start: rangeStart,
      end: now,
      min: ct, // cannot move beyond object creation time
      max: now,
         zoomMin: 288000000
  };

  var lastDate = 0;
  for (a=0;a<res.length;a++) {
   if (a==0 && res[a].open == 0 && res[a].closed == 0) continue;
   if (lastDate > 0) {
    // fill in from last date to this
    for (var x=lastDate+10800;x<res[a].ts;x=x+10800) {
     console.log("b) date is "+new Date(x *1000));
     items.push({x: new Date(x * 1000), y: res[a-1].open, label: {content: res[a-1].open, xOffset: -25}, group:0});
     items.push({x: new Date(x * 1000), y: res[a-1].closed, label: {content: res[a-1].closed, xOffset: -25},group:1});
    }
   }
   console.log("a) date is "+new Date(res[a].ts *1000));
   items.push({x: new Date(res[a].ts * 1000), y: res[a].open, label: {content: res[a].open, xOffset: -25},group:0});
   items.push({x: new Date(res[a].ts * 1000), y: res[a].closed, label: {content: res[a].closed, xOffset: -25},group:1});
   lastDate = res[a].ts;
  }
  // Now from last date to this
  if (res.length>0) {
   console.log("looping from "+(res[res.length-1].ts * 1000)+" to "+now.getTime());
   for (var x=(res[res.length-1].ts * 1000)+10800000;x<now.getTime();x=x+10800000) {
    console.log("x="+x);
    items.push({x: new Date(x), y: res[res.length-1].open, label: {content: res[res.length-1].open, xOffset: -25}, group:0});
    items.push({x: new Date(x), y: res[res.length-1].closed, label: {content: res[res.length-1].closed, xOffset: -25}, group:1});
   }
  }
  console.log("items is");
  console.log(items);
  var dataset = new vis.DataSet(items);
     var graph2d = new vis.Graph2d(container, items, groups, options);
     $("#"+divname).resizable({
   handles: "s",
   resize: function( event, ui ) {
    var gh = ui.size.height - 60;
    console.log("resize "+gh+"px");
    graph2d.setOptions({graphHeight: gh+"px"});
    // graph2d.redraw();
   }
  });
 });


}


/***** Start Release List  ******/
function rellist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#buildjoblist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
}

function getRelList(displaytype)
{
 window.visualSearch = VS.init({
  container  : $('#rel_search_box_container'),
  query      : "",
  showFacets : true,
  readOnly   : false,
  unquotable : [],
  callbacks  : {
    search : function(query, searchCollection) {
      var searchterms = window.visualSearch.searchQuery.facets();

      $.ajax({
       type : "POST",
       url : "TableFilter?filtername=relfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
       dataType: "json"
      }).done(function(data) {
          console.log(data);
      });

      rellist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true, "Parent":true};

      rellist_table.search( '' ).columns().search( '' ).draw();

      for (i=0;i<searchterms.length;i++)
      {
       if (searchterms[i]['Version'])
       {
        rellist_filter_keywords["Version"] = false;
        myValue = searchterms[i]['Version']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(1).search(regExSearch, true, false);
       }
       else if (searchterms[i]['Parent'])
       {
        rellist_filter_keywords["Parent"] = false;
        myValue = searchterms[i]['Parent']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(1).search(regExSearch, true, false);
       }
       else if (searchterms[i]['Domain'])
       {
        rellist_filter_keywords["Domain"] = false;
        myValue = searchterms[i]['Domain']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(2).search(regExSearch, true, false);
       }
       else if (searchterms[i]['Environment'])
       {
        rellist_filter_keywords["Environment"] = false;
        myValue = searchterms[i]['Environment']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(3).search(regExSearch, true, false);
       }
       else if (searchterms[i]['Last Deployment'])
       {
        rellist_filter_keywords["Last Deployment"] = false;
        myValue = searchterms[i]['Last Deployment']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(4).search(regExSearch, true, false);
       }
       else if (searchterms[i]['Result'])
       {
        rellist_filter_keywords["Result"] = false;
        myValue = searchterms[i]['Result']
        regExSearch = '^' + escapeRegExp(myValue);

        rellist_table.column(6).search(regExSearch, true, false);
       }
      }
      rellist_table.draw();
    },
    valueMatches : function(category, searchTerm, callback) {
      category = category.toLowerCase();

      if (category == "version")
       category = "name";
      else if (category == "last deployment")
       category = "deployid";
      else if (category == "result")
       category = "exitcode";

      switch (category) {
      case 'domain':
      case 'name':
      case 'parent':
      case 'environment':
      case 'deployid':
      case 'exitcode':
          var data = rellist_table.rows( { filter : 'applied'} ).data();

          var ids = "";
          var rows = [];

          if (data != null && data.length > 0)
          {
           for (k=0;k<data.length;k++)
           {
            console.log("" + data[k][category]);
            rows.push("" + data[k][category]);
           }
          }
          var urows = [... new Set(rows)];
          var srows = urows.sort(function (s1, s2) {
           var l=s1.toLowerCase(), m=s2.toLowerCase();
           return l===m?0:l>m?1:-1;
          })
          callback(srows, {preserveOrder: true});
          break;
      }
    },
    facetMatches : function(callback) {
      var kw = [];
      for (key in rellist_filter_keywords)
      {
       if (rellist_filter_keywords[key])
        kw.push(key);
      }
      callback(kw);
    }
  }
});

 $("#applist_pane").hide();
 $("#complist_pane").hide();
 $("#envlist_pane").hide();
 $("#domainlist_pane").hide();
 $("#endpointlist_pane").hide();
 $("#buildenglist_pane").hide();
 $("#buildjoblist_pane").hide();
 $("#actionlist_pane").hide();
 $("#procedurelist_pane").hide();
 $("#notifierlist_pane").hide();
 $("#repositorylist_pane").hide();
 $("#datasourcelist_pane").hide();
 $("#credentiallist_pane").hide();
 $("#userlist_pane").hide();
 $("#grouplist_pane").hide();
 $("#servercomptypelist_pane").hide();
 $("#templatelist_pane").hide();
 $("#rellist_pane").show();
 $("#rellist_buttons > button:nth-child(3)").css("color", "lightgrey");
 $(".taskMenuButton").css("color", "lightgrey");

 if (typeof rellist_table != "undefined" && rellist_table != null)
 {
  rellist_table.clear();
  rellist_table.destroy();
  rellist_table = null;
 }

 if (displaytype == "list")
 {
  var h = $(document).height();
  $("#rellist_list").height(h-160);

  $("#rellist_list").show();
  $("#rellist_map").hide();

 rellist_table =$('#rellist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollY: "60vh",
   scroller: true,
   pageLength: 25,
   destroy: true,
   sDom: 't<"bottom"lip>',
  "ajax": {
   "url": "/dmadminweb/ReportsData?type=RelList",
   "type": "GET"
 },
 "order": [[ 1, "asc" ]],
 "columns": [
             { "data": null},
      { "data": "name" },
      { "data": "domain" },
      { "data": "parent" },
      { "data": "environment" },
      { "data": "deployid" },
      { "data": "finished" },
      { "data": "exitcode" },
      { "data": "domainid" },
             { "data": "id" }
           ],
 "columnDefs": [
                {
                 targets: 0,
                 data: null,
                 defaultContent: '',
                orderable: false,
               width: 20,
                className: 'select-checkbox'
               },
                {
                 "targets": [ 8 ],
                 "visible": false
                },
                {
                 "targets": [ 9 ],
                 "visible": false
                }
               ],
   select: {
    style:    'os',
    selector: 'td:first-child'
   }
 } );

 $.ajax({
  type : "GET",
  async: false,
  url : "TableFilter?filtername=relfilter",
  dataType: "json"
 }).done(function(data) {
  var i = 0;
  for (var j = 0; j < data.length; j++)
  {
   var f = data[j];

   for (var key in f)
   {
     window.visualSearch.searchBox.addFacet(key,f[key],i);
     i=i+1;
   }
  }
        window.visualSearch.searchBox.searchEvent({});
 });

 if (rel_dblclick)
 {
  rel_dblclick = false;
  $('#rellist tbody').on('dblclick', 'tr', function (e) {
   e.stopPropagation();
   var data = rellist_table.row( this ).data();
   eventOpenRow("releases",isAdmin,data);
  });
 }

 rellist_table.on( 'select', function ( e, dt, type, indexes ) {
  if ( type === 'row' ) {
   currenttree = "#releases_tree";
   var data = rellist_table.rows({selected:  true}).data();
   if (data != null && data.length > 0)
   {
    lastsel_id = data[0]['id'];
    lastSelectedNode = data[0]['id'];
    lastset_rel = "";
    objid = lastsel_id.substr(2);
    objtype = lastsel_id.substr(0,2);
    objName = data[0]["name"];
    objtypeAsInt = obj2Int[objtype][0];
    objtypeName = obj2Int[objtype][1];
    objkind="";
    if (objtype == "pr" || objtype == "fn")
    {
     objkind=objid.substr(objid.indexOf("-")+1);
     objid = objid.substr(0,objid.indexOf("-"));
    }
   }
   $("#rellist_buttons > button:nth-child(3)").css("color", "#3367d6");
   $(".taskMenuButton").css("color", "#3367d6");
  }
 });

 rellist_table.on( 'deselect', function ( e, dt, type, indexes ) {
  if ( type === 'row' ) {
   $("#rellist_buttons > button:nth-child(3)").css("color", "lightgrey");
   $(".taskMenuButton").css("color", "lightgrey");
  }
 });

 }
 else // map
 {
  $("#rellist_list").hide();
  $("#rellist_map").show();
 }

 window.addEventListener("resize", rellist_table_resize);
 rellist_table_resize();
}
/***** End Release List ****/

function openDiff(dlg)
{
  dlg.dialog("close");

  selectedNode = $('#version_val option:selected').val();

  console.log("selected node = "+selectedNode);
  right_objid = selectedNode.substr(2); //OTID, first two chars are Object Type
  right_objtype = selectedNode.substr(0,2);
  right_objkind="";

  var left = objtype + objid;
  var right = right_objtype + right_objid;

  var url = "/dmadminweb/reports/diff.html?";

  url += "left=" + left;
  url += "&right=" + right;

  var arr =  window.location.href.split("/");

  window.open( arr[0] + "//" + arr[2] + url, "_blank");
}

function openDiffDlg(event, objname, objid) {
	  var pwd = parent.$("#modal");

  var buttons =
    [
          { text: "Compare",     click: function() { openDiff($(this)); } },
          { text: "Cancel", click: function() { $( this ).dialog("close"); } }
       ];

   var tdedit = "<form><table border=0 width=\"100%\" ><tr><td ><select name=\"version_val\" id=\"version_val\" size=\"" + versiondd_list.length + "\" style=\"width:100%;padding-right:40px;max-height:200px\">";

   for (i=0;i<versiondd_list.length;i++)
   {
	var val = versiondd_list[i];
	tdedit += "<option value=\"" + val.id + "\">" + val.name + "</option>";
   }

   tdedit += "</select></td></tr></table></form>";

   tdedit = "<div style=\"white-space: pre-wrap;padding-top:20px;padding-bottom:20px;padding-right:20px;\">Choose version to compare " + objName + " with:</div>" + tdedit;

   pwd.dialog({ resizable: false, modal: true, dialogClass: "aboutsDialog", open: null });
   pwd.empty();
   pwd.html(tdedit);
   pwd.dialog("option", "title", "Compare Versions");
   pwd.dialog("option", "height", "auto");
   pwd.dialog("option", "width", "auto");
   pwd.dialog("option", "buttons", buttons);
   pwd.dialog('open');
   $("#version_val").prop('selectedIndex', 0);
}


/**** Change Requests ****/

function AddDefectRow(tablename)
{
 if ($("#" + tablename + "-edit").is(":visible") == false)
   EditDefectRow(tablename);

 key = "";
 val = "";
 defect_rowcnt++;

 var td = '<tr id="defect_row_' + defect_rowcnt + '">';
 td += '<td><input type="checkbox" id="defect_edit_cb_' + defect_rowcnt + '" onchange="toggleCRDelete(this);" />&nbsp;</td>';
 td += '<td><select id="defect_key_' + defect_rowcnt + '" value="' + val + '">' + defectdropdown + '</select></td>';
 td += "<td><input type=\"hidden\" id=\"defect_oldkey_" + defect_rowcnt + "\" value=\"" + key + "\"\></td>";
 td += "<td><input type=\"hidden\" id=\"defect_oldval_" + defect_rowcnt + "\" value=\"" + val + "\"\></td></tr>";

 $("#" + tablename + "-edit > tbody").append(td);
}

function CancelDefectRow(tablename)
{
 if ($('#defect_header_buttons > button.cancel_button').css("color") == "grey")
  return;

 $('#defect_header_buttons > button.save_button').css("color","grey");
 $('#defect_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 // $("#" + tablename + "-edit > tbody").empty();
}

function EditDefectRow(tablename)
{
 $('#defect_header_buttons > button.save_button').css("color","#3367d6");
 $('#defect_header_buttons > button.cancel_button').css("color","#3367d6");
 $("#" + tablename).hide();
 $("#" + tablename + "-edit > tbody").html(defect_tdedit);

 for (var r = 0; r < defect_rowcnt; r++)
 {
  var rowcnt = r+1;
  id = $("#defect_oldval_" + rowcnt).val();
  $("#defect_key_" + rowcnt + " option[value=\"" + id + "\"]").attr('selected', true);
 }

 $("#" + tablename + "-edit").show();

}

function DeleteDefectRow(tablename)
{
 if ($('#defect_header_buttons > button.delete_button').css("color") == "grey")
  return;

rowid = 0;
 $("#" + tablename + '-edit > tbody  > tr').each(function(rowid, tr)
 {
  rowid++;

  cb  = $("#defect_edit_cb_" + rowid);

  if (cb.is( ":checked" ) == true)
  {
   key = $("#defect_oldkey_" + rowid).val();
   val = $("#defect_oldval_" + rowid).val();

   if (key != "")
     DeleteDefectData(key,val, tablename);
  }
 });

 rowid = 0;
 $("#" + tablename + ' > tbody  > tr').each(function(rowid, tr)
   {
    rowid++;

    cb  = $("#defect_cb_" + rowid);

    if (cb.is( ":checked" ) == true)
    {
     key = $("#defect_oldkey_" + rowid).val();
     val = $("#defect_oldval_" + rowid).val();

     if (key != "")
       DeleteDefectData(key,val, tablename);
    }
   });
}

function SaveDefectRow(tablename)
{
 if ($('#defect_header_buttons > button.save_button').css("color") == "grey")
  return;

 $('#defect_header_buttons > button.save_button').css("color","grey");
 $('#defect_header_buttons > button.cancel_button').css("color","grey");

 $("#" + tablename).show();
 $("#" + tablename + "-edit").hide();
 SaveDefectData(tablename);
}

function toggleDefectDelete(e)
{
  if (e.checked)
   $('#defect_header_buttons > button.delete_button').css("color","#3367d6");
  else
   $('#defect_header_buttons > button.delete_button').css("color","grey");
}
