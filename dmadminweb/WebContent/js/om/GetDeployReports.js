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
function CreateDeployReports()
{
 $.jqplot.config.enablePlugins = true;
 
 console.log("in CreateDeployReports");
 if ($('#plot3resizable').length){
	 console.log("plot3resizable exists");
 } else {
	 console.log("plot3resizable does not exist");
 }

 var tooltipContentEditor = function(str, seriesIndex, pointIndex, plot) {
  return '<table class="jqplot-highlighter"><tr><td>deployment:</td><td>#'
   + plot.data[seriesIndex][pointIndex][0] + '</td></tr><tr><td>time:</td><td>'
   + plot.data[seriesIndex][pointIndex][1] + '&nbsp;s</td></tr></table>';
 };
   
 try {
  plot3 = $.jqplot('deploymenttimebystep', [[['no data', 0]]], {
   title: 'Deployment Time by Step',
   seriesDefaults: {
    renderer: $.jqplot.PieRenderer,
    trendline: { show: false },
    rendererOptions: { padding: 8, showDataLabels: false }
   },
   legend: {show: true},
   
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
   }
  });

  $("#plot3resizable").resizable({delay:20});
  $('#plot3resizable').bind('resize', function(event, ui) {
         plot3.replot( { resetAxes: true } );
     });
 } catch(e) {
  alert('Exception (plot3): ' + e);
 }
 
 try {
		plot4 = $.jqplot('deploymentstepbarchart', [[0]],
		 	{
			title: 'Deployment Steps',
		    stackSeries: true,
			seriesDefaults: {
				renderer: $.jqplot.BarRenderer,
				rendererOptions: {
         		barDirection: 'horizontal'
				}
			},
			axes: {
				xaxis: {
			        tickOptions: {
			            formatString: '%d s'
			        }
				},
				yaxis: {
					renderer: $.jqplot.CategoryAxisRenderer,
					ticks: ['no data']
				}
			},
		    highlighter: {
		        showTooltip: true,
		        tooltipLocation: 'n',
		        tooltipAxes: 'x',
				tooltipFormatString: '%d s',
				useAxesFormatters: false
			},
			cursor: {
				show: false,
				showTooltip: false
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
 } catch(e) {
  console.log('Exception (plot3): ' + e);
 }

 $.ajax({
		async: false,
		url: "GetDeploymentStepsGantt?deployid="+deployid,
		type: 'POST',
		dataType: "json",
		success: function(x) {
			try {
				plot4._gridPadding = { top: null, right: null, bottom: null, left: 100 }; 
				plot4.replot({
					clear: false,
					data: x.data,
					series: x.series,
					axes: { yaxis: { ticks: x.ticks } },
					canvasOverlay: x.canvasOverlay,
					resetAxes: true
				});		
			} catch(e) {
				console.log('Exception (plot4): ' + e);
			}
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
                  layout:{ hierarchical: {enabled:true, sortMethod: 'directed', nodeSpacing:200, treeSpacing:200, direction: 'LR'}},
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