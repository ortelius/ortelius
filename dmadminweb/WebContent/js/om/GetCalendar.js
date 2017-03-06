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
var ioc=0;
 
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