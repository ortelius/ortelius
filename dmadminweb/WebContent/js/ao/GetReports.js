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

function CreateTimeline()
{
	var now=new Date();
	var rangeStart=new Date(now.getTime()-5184000000);		// 60 days in past
	var groups=[];
	var items=[];
	var url="";
	var cbt;
	console.log("CreateTimeline, objtype="+objtype);
	if (objtype=="en") {
		 // environment
		 console.log("GetTimeline?envid="+objid);
		 url = "envid=" + objid;
		 cbt = "ap";
	 } else 
	 if (objtype=="se") {
		 // server
		 console.log("GetTimeline?servid="+objid);
		 url = "servid=" + objid;
		 cbt = "co"; 
	 } else {
		 // Suppress endpoint timeline for apps and releases
		 $("#endpointtimeline").hide();
		 $("#changehistoryheader").hide();
	 }
	
	 if (url != "") {
		 $("#endpointtimeline").show();
		 $("#changehistoryheader").show();
		 $.getJSON('GetTimeline', url, function(res) {
				console.log("*******");
				console.log(res);
				console.log("*******");
				var options = {
				    groupOrder: function (a, b) {
				      return a.value - b.value;
				    },
				    groupOrderSwap: function (a, b, groups) {
				    	var v = a.value;
				    	a.value = b.value;
				    	b.value = v;
				    },
				    orientation: 'both',
				    editable: false,
				    selectable: true,
				    groupEditable: false,
				    stack: false,
				    start: rangeStart,
				    end: now,
				    min: new Date(res.created * 1000),
				    max: now
				};
				
			if ("groups" in res)
			{ 
				for (g=0;g<res.groups.length;g++) {
					groups.push({"content": "<a href='javascript:SwitchDisplay(\""+cbt+res.groups[g].id+"\");'><b>"+res.groups[g].name+"</b></a>", "id": res.groups[g].id, "value": res.groups[g].id});
				}
			}
			
			if ("items" in res)
			{
				for (i=0;i<res.items.length;i++) {
					items.push({start: new Date(res.items[i].from*1000), end: new Date(res.items[i].to*1000), group: res.items[i].group, content: res.items[i].name, id: i,title: res.items[i].name});
				}
			}
				var container = document.getElementById('endpointtimeline');
				container.innerHTML = "";
				 
				var timeline = new vis.Timeline(container);
				timeline.setOptions(options);
				timeline.setGroups(groups);
				timeline.setItems(items);
				timeline.addCustomTime(new Date(now.getTime()-2592000000));	// 30 days in past
				timeline.on('contextmenu', function (props) {
					props.event.preventDefault();
				});
		 });
	 }
}

function CreateReports()
{
 $.jqplot.config.enablePlugins = true;

 var tooltipContentEditor = function(str, seriesIndex, pointIndex, plot) {
  return '<table class="jqplot-highlighter"><tr><td>deployment:</td><td>#'
   + plot.data[seriesIndex][pointIndex][0] + '</td></tr><tr><td>time:</td><td>'
   + plot.data[seriesIndex][pointIndex][1] + '&nbsp;s</td></tr></table>';
 };
  
 try {
  plot1 = $.jqplot('timetodeploylinechart', [[['no data', 0]]], {
   title: 'Time Taken to Deploy (Last 10 Deployments)',
   axes: {
    xaxis: {
     renderer: $.jqplot.CategoryAxisRenderer
    },
    yaxis: {
           tickOptions: {
               formatString: '%d s'
           },
           padMin: 0
       }
   },
   seriesDefaults: {
    trendline: { show: true }
   },
      highlighter: {
          showTooltip: true,
          tooltipLocation: 'n',
          tooltipAxes: 'xy',
          tooltipContentEditor: tooltipContentEditor,
    useAxesFormatters: false
   },
   cursor: {
    show: false,
    showTooltip: false
   },
   grid: {
    backgroundColor: 'white'
}
  });

  $('#plot1resizable').bind('jqplotDataClick',
          function (ev, seriesIndex, pointIndex, data) { 
	  			// alert(data[seriesIndex][pointIndex][0]);
	  			var depid = plot1.axes.xaxis.ticks[pointIndex];
	  			DisplayDeploy(depid);
          });
  $("#plot1resizable").resizable({delay:20});
  $('#plot1resizable').bind('resize', function(event, ui) {
         plot1.replot( { resetAxes: true } );
     });
 } catch(e) {
  alert('Exception: ' + e);
 }
 
 try {
  plot2 = $.jqplot('applicationsuccesspiechart', [[['no data', 0]]], {
   title: 'Application Deployments',
   seriesDefaults: {
    renderer: $.jqplot.PieRenderer,
    trendline: { show: false },
    rendererOptions: { padding: 8, showDataLabels: false }
   },
   seriesColors: [ 'lightcoral', 'darkseagreen' ],
      legend: { show: true },
      highlighter: {
          showTooltip: true,
          tooltipLocation: 'n',
          tooltipAxes: 'y',
    tooltipFormatString: '%d deployments',
    useAxesFormatters: false
   },
   cursor: {
    show: false,
    showTooltip: false
   },
   grid: {
    backgroundColor: 'white'
}
  });

  $("#plot2resizable").resizable({delay:20});
  $('#plot2resizable').bind('resize', function(event, ui) {
         plot2.replot( { resetAxes: true } );
     });
 } catch(e) {
  alert('Exception: ' + e);
 }
}

function ReplotReports(objtype)
{
	console.log("ReportReports("+objtype+")");
 var ajaxDataRenderer = function(url, plot, options) {
  var ret = null;
  $.ajax({
   async: false,
   url: url,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    ret = data;
   }
  });
  if((ret != null) && (ret.ticks != null)) {
   if((options != null) && (options.dataAxis == 'x')) { 
    plot.axes.yaxis.ticks = ret.ticks;
   } else {
    plot.axes.xaxis.ticks = ret.ticks;
   }
  }
  return (ret != null) ? ret.data : null;
 };

 var jsonurl1=null;
 var jsonurl2=null;
 console.log("objtype="+objtype);
 if (objtype=="ap" || objtype=="av") {
	 // application
	 console.log("GetTimeToDeploy?appid="+objid);
	 jsonurl1 = "GetTimeToDeploy?appid=" + objid;
	 jsonurl2 = "GetSuccessFailure?appid=" + objid;
 } else 
 if (objtype=="se") {
	 // server
	 console.log("GetTimeToDeploy?servid="+objid);
	 jsonurl1 = "GetTimeToDeploy?servid=" + objid;
	 jsonurl2 = "GetSuccessFailure?servid=" + objid;
 } else 
 if (objtype=="en") {
	 // environment
	 console.log("GetTimeToDeploy?envid="+objid);
	 jsonurl1 = "GetTimeToDeploy?envid=" + objid;
	 jsonurl2 = "GetSuccessFailure?envid=" + objid;
 }
 
 if (jsonurl1 != null) {
	 try {
	  
	  plot1._gridPadding = { top: null, right: null, bottom: null, left: null }; 
	  plot1.replot({
	   data: jsonurl1,
	   dataRenderer : ajaxDataRenderer,
	   resetAxes: true
	  });
	 } catch(e) {
	  console.log('Exception: ' + e);
	 }
	
	 try {
	  plot2._gridPadding = { top: null, right: null, bottom: null, left: null };
	  plot2.replot({
	   data: jsonurl2,
	   dataRenderer : ajaxDataRenderer,
	   resetAxes: true
	  });
	 } catch(e) {
	  console.log('Exception: ' + e);
	 }
 }
// CreateTimeline();
}
