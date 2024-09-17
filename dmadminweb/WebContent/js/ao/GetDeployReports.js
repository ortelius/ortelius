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

function CreateDeployReports()
{
 $.jqplot.config.enablePlugins = true;

 console.log("in CreateDeployReports");

 var tooltipContentEditor = function(str, seriesIndex, pointIndex, plot) {
  return '<table class="jqplot-highlighter"><tr><td>deployment:</td><td>#'
   + plot.data[seriesIndex][pointIndex][0] + '</td></tr><tr><td>time:</td><td>'
   + plot.data[seriesIndex][pointIndex][1] + '&nbsp;s</td></tr></table>';
 };

 try {
  plot3 = $.jqplot('deploymenttimebystep', [[['no data', 0]]], {
   title: 'Step Duration',
   height: 800,
   seriesDefaults: {
    renderer: $.jqplot.PieRenderer,
    trendline: { show: false },
    rendererOptions: { padding: 8, showDataLabels: false }
   },
   legend: {show: true, placement: "outside"},

      highlighter: {
    	  showTooltip: false,
          // showTooltip: true,
          tooltipLocation: 'n',
          tooltipAxes: 'y',
          tooltipFormatString: '%d s',
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

  $("#plot3resizable").resizable({delay:20});
  $('#plot3resizable').bind('resize', function(event, ui) {
         plot3.replot( { resetAxes: true } );
     });
 } catch(e) {
  alert('Exception (plot3): ' + e);
 }
}

function ReplotDeployReports(deployid)
{
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

 try {
  var jsonurl1 = "GetTimePerStepForDeployment?deployid=" + deployid;
  plot3._gridPadding = { top: null, right: null, bottom: null, left: null };
  plot3.replot({
   data: jsonurl1,
   dataRenderer : ajaxDataRenderer,
   resetAxes: true
  });

  jQuery(document).ready(function() {
   var $mainTable = $("#deploymenttimebystep > table");
   var splitBy = 15;
   var rows = $mainTable.find ( "tr" ).slice( splitBy );
   var $secondTable = $("#deploymenttimebystep > table").parent().append("<table id='deploymenttimebystep2' class=\"jqplot-table-legend\" style=\"position:absolute;\"><tbody></tbody></table>");
   $secondTable.find("tbody").append(rows);
   $mainTable.find ( "tr" ).slice( splitBy ).remove();
   var top = $("#deploymenttimebystep").offset().top;
   top += $("#deploymenttimebystep").height();
   var left = $("#deploymenttimebystep").offset().left;
   $("#deploymenttimebystep2").css({
       top: "24px",
       left: left + "px"
   });

});

 } catch(e) {
  console.log('Exception (plot3): ' + e);
 }
}


function CreateDeployReports4()
{
 $.jqplot.config.enablePlugins = true;

 console.log("in CreateDeployReports4");

 var tooltipContentEditor = function(str, seriesIndex, pointIndex, plot) {
  return '<table class="jqplot-highlighter"><tr><td>deployment:</td><td>#'
   + plot.data[seriesIndex][pointIndex][0] + '</td></tr><tr><td>time:</td><td>'
   + plot.data[seriesIndex][pointIndex][1] + '&nbsp;s</td></tr></table>';
 };

 try {
  plot4 = $.jqplot('deploymenttimebystep4', [[['no data', 0]]], {
   title: 'Step Duration',
   height: 800,
   seriesDefaults: {
    renderer: $.jqplot.PieRenderer,
    trendline: { show: false },
    rendererOptions: { padding: 8, showDataLabels: false }
   },
   legend: {show: true, placement: "outside"},

      highlighter: {
       showTooltip: false,
          // showTooltip: true,
          tooltipLocation: 'n',
          tooltipAxes: 'y',
          tooltipFormatString: '%d s',
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

  $("#plot4resizable").resizable({delay:20});
  $('#plot4resizable').bind('resize', function(event, ui) {
         plot4.replot( { resetAxes: true } );
     });
 } catch(e) {
  alert('Exception (plot4): ' + e);
 }
}

function ReplotDeployReports4(deployid)
{
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

 try {
  var jsonurl1 = "GetTimePerStepForDeployment?deployid=" + deployid;
  plot4._gridPadding = { top: null, right: null, bottom: null, left: null };
  plot4.replot({
   data: jsonurl1,
   dataRenderer : ajaxDataRenderer,
   resetAxes: true
  });

  var mytbody = $("#deploymenttimebystep4 > table > tbody").html();
   var rows = mytbody.split("</tr>");

   for (i=0;i<rows.length;i++)
   {
    rows[i] = rows[i].replace("<tr class=\"jqplot-table-legend\">","");
   }

   var size = 8;
   var newArray = new Array(Math.ceil(rows.length / size)).fill("")
   .map(function() { return this.splice(0, size) }, rows.slice());

   rows = [];
   for (i=0;i<10;i++)
    rows[i] = "<tr class=\"jqplot-table-legend\">";

   for (i=0;i<newArray.length;i++)
   {
    tds = newArray[i];
    for (j=0;j<tds.length;j++)
    {
     rows[j] += tds[j];
    }
   }

   for (i=0;i<10;i++)
    rows[i] += "</tr>";

   console.log(rows);
   $("#deploymenttimebystep4 > table > tbody").html(rows.join());

   if (!$("#deploymenttimebystep4_scroll").length)
      $("#deploymenttimebystep4 > table").wrap("<div id=\"deploymenttimebystep4_scroll\"></div>");

 } catch(e) {
  console.log('Exception (plot4): ' + e);
 }
}

function updateDeployDiff(event)
{
 var envid;

 envid = event.value.substring(2);;

 $.ajax({
  url: "GetDeployDeps?appid="+objid + "&envid=" + envid,
  type: 'POST',
  dataType: "json",
  success: function(x) {

   // create a network
   var nodes = new vis.DataSet(x.nodes);
   var edges = new vis.DataSet(x.edges);

   if ("labels" in x)
   {
    var labels = x.labels;

    if (labels.prevapp == "never")
     $("#feedback-label").html("<strong>" + labels.currapp + "</strong> has never been deployed to this environment");
    else if (labels.prevapp == "")
     $("#feedback-label").html("First deployed <strong>" + labels.currapp + "</strong> version");
    else
     $("#feedback-label").html("Difference between deployed <strong>" + labels.currapp + "</strong> version and previous deployed <strong>" + labels.prevapp + "</strong> version");
    }

    var data = {
      nodes: nodes,
      edges: edges
    };

    var options = {autoResize: true,
      height: '100%',
      width: '100%',
                   layout:{ hierarchical: {enabled:true, sortMethod: 'directed', nodeSpacing:200, levelSeparation: 300, treeSpacing:200, direction: 'LR'}},
                   physics:{
                    enabled: true,
                    barnesHut: {
                      gravitationalConstant: -2000,
                      centralGravity: 0.3,
                      springLength: 95,
                      springConstant: 0.04,
                      damping: 0.09,
                      avoidOverlap: 0
                    }},
                    edges:{arrows: 'to',smooth: true},
                    nodes:{shape:'box'},
                    interaction:{navigationButtons: true}
   };

    var container = document.getElementById('feedback-data');
    var network = new vis.Network(container, data, options);
  }
 });
}

function CreateCurrentEnv2Comps(objid)
{

 $("#feedback_title").html("<h2>Deployed Environments for Component</h2>");

$.ajax({
 url: "GetDeployDeps?compid="+objid,
 type: 'POST',
 dataType: "json",
 success: function(x) {

  // create a network
  var nodes = new vis.DataSet(x.nodes);
  var edges = new vis.DataSet(x.edges);

   var data = {
     nodes: nodes,
     edges: edges
   };

   var options = {autoResize: true,
     height: '100%',
     width: '100%',
                  layout:{ hierarchical: {enabled:true, sortMethod: 'directed', nodeSpacing:200, levelSeparation: 300, treeSpacing:200, direction: 'LR'}},
                  physics:{
                   enabled: true,
                   barnesHut: {
                     gravitationalConstant: -2000,
                     centralGravity: 0.3,
                     springLength: 95,
                     springConstant: 0.04,
                     damping: 0.09,
                     avoidOverlap: 0
                   }},
                   edges:{arrows: 'to',smooth: true},
                   nodes:{shape:'box'},
                   interaction:{navigationButtons: true}
  };

   var container = document.getElementById('feedback-data');
   var network = new vis.Network(container, data, options);
 }
});
}


function CreateCurrentComp2Envs(objid)
{

 $("#feedback_title").html("<h2>Deployed Components for Environment</h2>");

$.ajax({
 url: "GetDeployDeps?envid="+objid,
 type: 'POST',
 dataType: "json",
 success: function(x) {

  // create a network
  var nodes = new vis.DataSet(x.nodes);
  var edges = new vis.DataSet(x.edges);

   var data = {
     nodes: nodes,
     edges: edges
   };

   var options = {autoResize: true,
     height: '100%',
     width: '100%',
                  layout:{ hierarchical: {enabled:true,  nodeSpacing:200, levelSeparation: 300, treeSpacing:200, direction: 'UD'}},
                  physics:{
                   enabled: true,
                  hierarchicalRepulsion: {
                   centralGravity: 0.0,
                   springLength: 100,
                   springConstant: 0.01,
                   nodeDistance: 350,
                   damping: 0.09
                 }},
                   edges:{arrows: 'to',smooth: true},
                   nodes:{shape:'box'},
                   interaction:{navigationButtons: true}
  };

   var container = document.getElementById('feedback-data');
   var network = new vis.Network(container, data, options);
 }
});
}


function CreateDeployDeps(deployid)
{

 console.log('Setup RESIZE');
 console.log($('#displaylog'));
 var myElement = document.getElementById('displaylog'),
 myResizeFn = function(){
  if ($('#displaylog').height()-90 > 100)
    $('#displaylogtabs-deploydeps-data').height($('#displaylog').height()-90);
  console.log("RESIZING " + $('#displaylog').height() + " x " + $('#displaylog').width() );
 };
addResizeListener(myElement, myResizeFn);

$.ajax({
 url: "GetDeployDeps?deployid="+deployid,
 type: 'POST',
 dataType: "json",
 success: function(x) {

  // create a network
  var nodes = new vis.DataSet(x.nodes);
  var edges = new vis.DataSet(x.edges);

   var data = {
     nodes: nodes,
     edges: edges
   };

   var options = {autoResize: true,
     height: '100%',
     width: '100%',
                  layout:{ hierarchical: {enabled:true, sortMethod: 'directed', nodeSpacing:200, levelSeparation: 300, treeSpacing:200, direction: 'LR'}},
                  physics:{
                   enabled: true,
                   barnesHut: {
                     gravitationalConstant: -2000,
                     centralGravity: 0.3,
                     springLength: 95,
                     springConstant: 0.04,
                     damping: 0.09,
                     avoidOverlap: 0
                   }},
                   edges:{arrows: 'to',smooth: true},
                   nodes:{shape:'box'},
                   interaction:{navigationButtons: true}
  };

   var container = document.getElementById('displaylogtabs-deploydeps-data');
   var network = new vis.Network(container, data, options);
 }
});
}

(function(){
 var attachEvent = document.attachEvent;
 var isIE = navigator.userAgent.match(/Trident/);
 console.log(isIE);
 var requestFrame = (function(){
   var raf = window.requestAnimationFrame || window.mozRequestAnimationFrame || window.webkitRequestAnimationFrame ||
       function(fn){ return window.setTimeout(fn, 20); };
   return function(fn){ return raf(fn); };
 })();

 var cancelFrame = (function(){
   var cancel = window.cancelAnimationFrame || window.mozCancelAnimationFrame || window.webkitCancelAnimationFrame ||
          window.clearTimeout;
   return function(id){ return cancel(id); };
 })();

 function resizeListener(e){
   var win = e.target || e.srcElement;
   if (win.__resizeRAF__) cancelFrame(win.__resizeRAF__);
   win.__resizeRAF__ = requestFrame(function(){
     var trigger = win.__resizeTrigger__;
     trigger.__resizeListeners__.forEach(function(fn){
       fn.call(trigger, e);
     });
   });
 }

 function objectLoad(e){
   this.contentDocument.defaultView.__resizeTrigger__ = this.__resizeElement__;
   this.contentDocument.defaultView.addEventListener('resize', resizeListener);
 }

 window.addResizeListener = function(element, fn){
   if (!element.__resizeListeners__) {
     element.__resizeListeners__ = [];
     if (attachEvent) {
       element.__resizeTrigger__ = element;
       element.attachEvent('onresize', resizeListener);
     }
     else {
       if (getComputedStyle(element).position == 'static') element.style.position = 'relative';
       var obj = element.__resizeTrigger__ = document.createElement('object');
       obj.setAttribute('style', 'display: block; position: absolute; top: 0; left: 0; height: 100%; width: 100%; overflow: hidden; pointer-events: none; z-index: -1;');
       obj.__resizeElement__ = element;
       obj.onload = objectLoad;
       obj.type = 'text/html';
       if (isIE) element.appendChild(obj);
       obj.data = 'about:blank';
       if (!isIE) element.appendChild(obj);
     }
   }
   element.__resizeListeners__.push(fn);
 };

 window.removeResizeListener = function(element, fn){
   element.__resizeListeners__.splice(element.__resizeListeners__.indexOf(fn), 1);
   if (!element.__resizeListeners__.length) {
     if (attachEvent) element.detachEvent('onresize', resizeListener);
     else {
       element.__resizeTrigger__.contentDocument.defaultView.removeEventListener('resize', resizeListener);
       element.__resizeTrigger__ = !element.removeChild(element.__resizeTrigger__);
     }
   }
 }
})();
