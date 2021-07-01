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
        function removeProp(obj, propToDelete) {
            for (var property in obj) {
                if (typeof obj[property] == "object") {
                    let objectToCheck = obj[property];
                    delete obj.property
                    let newJsonData = this.removeProp(obj[property], propToDelete);
                    obj[property] = newJsonData
                } else {
                    if (property === propToDelete) {
                        delete obj[property];
                    }
                }
            }
            return obj
        }

        function unflatten(arr) {
            var tree = [],
                mappedArr = {},
                arrElem,
                mappedElem;

            // First map the nodes of the array to an object -> create a hash table.
            for (var i = 0, len = arr.length; i < len; i++) {
                arrElem = arr[i];
                mappedArr[arrElem.name] = arrElem;
                mappedArr[arrElem.name]['children'] = [];
            }

            for (var name in mappedArr) {
                if (mappedArr.hasOwnProperty(name)) {
                    mappedElem = mappedArr[name];
                    // If the element is not at the root level, add it to its parent array of children.
                    if (mappedElem.parentid != "") {
                        mappedArr[mappedElem['parentid']]['children'].push(mappedElem);
                    }
                    // If the element is at the root level, add it to first level elements array.
                    else {
                        tree.push(mappedElem);
                    }
                }
            }
            tree = removeProp(tree, "parentid");
            return tree;
        }
        
 function DomNav()
 {
    $("#domnav-panel").html("");   

    $.getJSON("/dmadminweb/ReportsData?type=domainlist", function(flatData) {
        waitForElement("#domnav-panel",function(){
            var data = unflatten(flatData.data);
            data = data[0];
        //    console.log(JSON.stringify(data, null, " "));

            
            partition = data => {
                const root = d3.hierarchy(data)
                    .sum(d => d.children.length + 1)
                    .sort((a, b) => b.value - a.value);
                return d3.partition()
                    .size([2 * Math.PI, root.height + 1])
                    (root);
            }
            
            console.log(data);

            var color = d3.scaleOrdinal().range(d3.quantize(d3.interpolateRainbow, data.children.length + 1));
            var format = d3.format(",d");

            var width = Math.round($("#domnav-panel").width() / 1.2); // Math.round(window.innerHeight / 2);
            var height = width;
            var radius = width / 6;

            var arc = d3.arc()
                .startAngle(d => d.x0)
                .endAngle(d => d.x1)
                .padAngle(d => Math.min((d.x1 - d.x0) / 2, 0.005))
                .padRadius(radius * 1.5)
                .innerRadius(d => d.y0 * radius)
                .outerRadius(d => Math.max(d.y0 * radius, d.y1 * radius - 1));


            const root = partition(data);
            domroot = root;

            root.each(d => d.current = d);

            // const svg = d3.select(DOM.svg(width, width))
            const svg = d3.select("#domnav-panel").append("svg")
                .style("width", width)
                .style("height", width)
                .style("font", "10px sans-serif");

            const g = svg.append("g")
                .attr("transform", `translate(${width / 2},${width / 2})`);

            const path = g.append("g")
            .selectAll("path")
            .data(root.descendants().slice(1))
            .enter().append("path")
            .attr("fill", d => {
                while (d.depth > 1) d = d.parent;
                return color(d.data.name);
            })
            .attr("fill-opacity", d => arcVisible(d.current) ? (d.children ? 0.6 : 0.4) : 0)
            .attr("d", d => arc(d.current));

            path.filter(d => d.current)  // d.children
                .style("cursor", "pointer")
                .on("click", clicked);

            path.append("title")
                .text(d => `${d.ancestors().map(d => d.data.name.split('.').pop()).reverse().join(".")}\n${format(d.value)}`);

            const label = g.append("g")
                .attr("pointer-events", "none")
                .attr("text-anchor", "middle")
                .style("user-select", "none")
                .selectAll("text")
                .data(root.descendants().slice(1))
                .enter().append("text")
                .attr("dy", "0.35em")
                .attr("fill-opacity", d => +labelVisible(d.current))
                .attr("transform", d => labelTransform(d.current))
                .text(d => d.data.name.split('.').pop());

            const parent = g.append("circle")
                .datum(root)
                .attr("r", radius)
                .attr("fill", "none")
                .attr("pointer-events", "all")
                .on("click", clicked);

            var dname = $("#right_panel_header > h1").text();
            dname = dname.substring("Domain: ".length);
            
            domnav_text = g.append("svg:text")
            .attr("class", "aster-score")
            .attr("dy", "1em")
            .attr("text-anchor", "middle")
            .attr("pointer-events", "all")
            .text(dname)
            .on("click", clicked);
            
            domnav_back = g.append("svg:text")
            .attr("class", "aster-score")
            .attr("dy", "2.5em")
            .attr("text-anchor", "middle")
            .attr("pointer-events", "all")
            .text("")
            .on("click", clicked);
            
            function clicked(p) {
             
             if (p.current ? (p.children ? 0 : 1) : 0) 
                $("#domnav-panel > svg > g > circle").attr("fill","rgb(110, 64, 170)").attr("fill-opacity","0.4");
             else
                $("#domnav-panel > svg > g > circle").attr("fill","#fff");

                if (typeof p == "undefined")
                {
                 var middle = $("#domnav-panel > svg > g > circle");
                 middle[0].dispatchEvent(new Event('click'));
                 return;
                } 
                current_dom_data = p;
                current_dom_path = this;
                $("#right_panel_header > h1").html("Domain: " + p.data.name);
                domnav_text.text(p.data.name);
                
                if (p.depth > 0)
                 domnav_back.text("Click to go parent domain");
                else
                 domnav_back.text("");
                
                objid = p.data.domid;
                LoadSummaryData("summ", objtypeAsInt, objtype, p.data.domid, "");
                
                if (p.current ? (p.children ? 0 : 1) : 0)  //condition to check the last node
                  $("#summ_header_buttons > button.delete_button").css("color","#3367d6");
                else
                  $("#summ_header_buttons > button.delete_button").css("color","grey");
               
                parent.datum(p.parent || root);                

                root.each(d => d.target = {
                    x0: Math.max(0, Math.min(1, (d.x0 - p.x0) / (p.x1 - p.x0))) * 2 * Math.PI,
                    x1: Math.max(0, Math.min(1, (d.x1 - p.x0) / (p.x1 - p.x0))) * 2 * Math.PI,
                    y0: Math.max(0, d.y0 - p.depth),
                    y1: Math.max(0, d.y1 - p.depth)
                });

                const t = g.transition().duration(750);

                path.transition(t)
                    .tween("data", d => {
                        const i = d3.interpolate(d.current, d.target);
                        return t => d.current = i(t);
                    })
                    .filter(function(d) {
                        return +this.getAttribute("fill-opacity") || arcVisible(d.target);
                    })
                    .attr("fill-opacity", d => arcVisible(d.target) ? (d.children ? 0.6 : 0.4) : doDisplay(p, d))
                    .attrTween("d", d => () => arc(d.current));

                label.filter(function(d) {
                        return +this.getAttribute("fill-opacity") || labelVisible(d.target);
                    }).transition(t)
                    .attr("fill-opacity", d => +labelVisible(d.target))
                    .attrTween("transform", d => () => labelTransform(d.current));
            }
            
            current_dom_path = clicked;
            
            function doDisplay(p, d)
            {
             var parent = p.data.name.split('.');
             var curr   = d.data.name.split('.');
             
             if ((parent.length == 1 && curr.length > 2) || d.children) 
              return 0.0;
             else
              return 0.6;
            }
            
            function arcVisible(d) {
                return d.y1 <= 3 && d.y0 >= 1 && d.x1 > d.x0;
            }

            function labelVisible(d) {
                return d.y1 <= 3 && d.y0 >= 1 && (d.y1 - d.y0) * (d.x1 - d.x0) > 0.03;
            }

            function labelTransform(d) {
                const x = (d.x0 + d.x1) / 2 * 180 / Math.PI;
                const y = (d.y0 + d.y1) / 2 * radius;
                return `rotate(${x - 90}) translate(${y},0) rotate(${x < 180 ? 0 : 180})`;
            }

            if (lookupDom != "")
            {
             var findpath = $("#domnav-panel > svg > g > g > path > title:contains(\"" + lookupDom + "\")"); 
             if (findpath.length > 0)
             {
              var parent_path = null;
              var allNodes = domroot.descendants();
              for (i=0; i < allNodes.length; i++)
              {
               var name = allNodes[i].data.name;
               if (name.includes(lookupDom))
               {
                parent_path = allNodes[i];
                break;
               }
              }
              
              current_dom_data = parent_path;
              }
             }
            
            if (current_dom_data != null)
             current_dom_path(current_dom_data);
            
            if (lookupDom != "")
            {
                $("#domnav-panel > svg > g > circle").attr("fill","rgb(110, 64, 170)").attr("fill-opacity","0.4");
                
                if (editdom)
				  {
				   setTimeout(
					  () => {
					    $("#summ_header_buttons > button.edit_button").click();
					  },
					  1 * 1000
					);
				   editdom=false;
				  }
			}
        });
    });
        }
 
 function LoadTasks(tablename,objtypeAsInt,objtype, objid, sel)
 {
  
  $('#tasks_header_buttons > button.delete_button').css("color","grey"); 
  
  $.ajax({
   url : "DomainDetails?domainid=" + objid,
   dataType : 'json',
   type : 'GET',
   success : function(res) {
   var td = "";
   var rowcnt = 0;

   for (var i = 0; i < res.length; i++)
   {
    var row = res[i];
    var key = row.id;
    var val = row.name;

    rowcnt++;
    
    td += '<tr id="task_row_' + rowcnt + '"><td><input type="checkbox" id="task_cb_' + rowcnt + '" onchange="toggleTask(this);" />&nbsp;</td><td id="task_val_' + rowcnt + '">' + val + '</td><td id="task_key_' + rowcnt + '" style="display:none;">' + key + '</td></tr>';
    
    task_rowcnt = rowcnt;
   }
   $("#" + tablename + " > tbody").html(td);
   
   for (var i = 0; i < res.length; i++)
   {
    var row = i+1
    
    $("#task_row_" +  row + " > td:nth-child(2)").on('click', function (e) {
     var id = this.id
     id = id.replace("val","cb");
     
     if ($("#" + id).is(":checked"))
      $("#" + id).prop("checked", false).trigger("change");
     else
      $("#" + id).click();
    });
   }
   
   LoadTaskSummary(null);
  },
  error : function(jqxhr, status, err)
  {
   console.log(status);
   console.log(err);
  }
 });
 }

 function DeleteDomain(objtypeAsInt,objid)
 {
  $.ajax({
   url:"RemoveNode", 
   data: { 
              "rel" : "Domain", 
              "id" : objid
   },
   dataType: "json",
   success: function (d) {
    console.log("RemoveNode returns:");
    console.log(d);
    var parent = current_dom_data.parent;
    current_dom_data = parent;
    DomNav();
   }
  });
 }
 
 function toggleTask(e)
 {
  if (prevcb != null && prevcb != e.id)
   $("#" + prevcb).prop("checked", false);
  
  prevcb = e.id;
  
  if (e.checked)
  { 
   $('#tasks_header_buttons > button.delete_button').css("color","#3367d6"); 
   LoadTaskSummary(e.id);
  } 
  else
  {
   $('#tasks_header_buttons > button.delete_button').css("color","grey"); 
   LoadTaskSummary(null);
  } 
 }

 function LoadTaskSummary(cb_id)
 {
  if (cb_id == null)
  {
   $("#task_summ > tbody").empty();
   $("#tasks_assigned_groups").empty();
   $("#tasks_available_groups").empty();
   task_tdedit = "";
   return;
  }
  
  var keyid = cb_id.replace("cb","key");
  var id = $("#" + keyid).text();
  var objtype = id.substring(0,2);
  var objid = id.substring(2);
  var objtypeAsInt = obj2Int[objtype][0];
  
  LoadTaskSummaryData("task_summ", objtypeAsInt, objtype, objid, "");
  LoadTaskAccess();
 }

 function LoadTaskAccess()
 {
  $("#tasks_assigned_groups").html("");
  $("#tasks_available_groups").html("");

  var tid = task_summSaveobjid;

  // Display Groups (ga, gna)
  console.log("Getting group data");
  $.ajax({
   dataType: "json",
   type: 'POST',
   url: "DomainDetails",
   data: "f=gg&domainid=" + summSaveobjid + "&tid="+tid,
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

     var DraggableOptions = {
       // cancel : "a.ui-icon", // clicking an icon won't initiate dragging
       revert : "invalid", // when not dropped, the item will revert back to its initial position
       containment : "document",
       helper : "clone",
       cursor : "move"
      };
     
     $(".icon_inline_Group").draggable(DraggableOptions);
    }
  });
  
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
 }


 function MoveDomGroupIn($item)
 {
  console.log("adding group="+$item.attr("id")+" to task "+ task_summSaveobjid);
  $.ajax({
   async: true,
   url: "DomainDetails?f=ag&gid="+$item.attr("id").substr(1)+"&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
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
  console.log("removing group="+$item.attr("id")+" from task "+task_summSaveobjid);
  $.ajax({
   async: true,
   url: "DomainDetails?f=rg&gid="+$item.attr("id").substr(1)+"&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
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

 function DeleteTaskRow(tablename)
 {
  $.ajax({
   async: true,
   url: "DomainDetails?f=dt&domainid=" + summSaveobjid + "&tid="+task_summSaveobjid,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    LoadTasks(tablename, objtypeAsInt, objtype, objid, "");
    LoadTaskSummary(null);
   }
  });
 }
 
 function AddTaskRow(tablename,tasktype)
 {
  $.ajax({
   async: true,
   url: "DomainDetails?f=at&domainid=" + objid + "&tasktype="+tasktype,
   type: 'POST',
   dataType: "json",
   success: function(data) {
    addTaskMenu();
    LoadTasks(tablename,objtypeAsInt,objtype, objid, data.id);
   }
  });
 }

 function AddDomain(tablename)
 {
  $.ajax({
   url: "GetNewID?objtype=Domain&pid=" + objid + "&ptype=Domain&at=&name=undefined&treeid=domains_tree",
   type: 'POST',
   dataType: "json",
   success: function(data) {
     lookupDom = data.name;
     editdom = true;
     DomNav();
   }
  }); 
 }
        
