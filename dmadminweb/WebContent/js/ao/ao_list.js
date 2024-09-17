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

/***** Start Application List ****/

 function applist_table_resize(){
  var h = $("#panel_container_menu").height();
 // $("#applist_list").height(h-100);
  $(".dataTables_scrollBody").css('max-height', (h-150) + "px");
 }

 function getAppList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#app_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=appfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       applist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true, "Parent":true};

       applist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Version'])
        {
         applist_filter_keywords["Version"] = false;
         myValue = searchterms[i]['Version']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         applist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(2).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Parent'])
        {
         applist_filter_keywords["Parent"] = false;
         myValue = searchterms[i]['Parent']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(3).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Environment'])
        {
         applist_filter_keywords["Environment"] = false;
         myValue = searchterms[i]['Environment']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(4).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Last Deployment'])
        {
         applist_filter_keywords["Last Deployment"] = false;
         myValue = searchterms[i]['Last Deployment']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(5).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Result'])
        {
         applist_filter_keywords["Result"] = false;
         myValue = searchterms[i]['Result']
         regExSearch = '^' + escapeRegExp(myValue);

         applist_table.column(7).search(regExSearch, true, false);
        }
       }
       applist_table.draw();
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
       case 'environment':
       case 'parent':
       case 'deployid':
       case 'exitcode':
           var data = applist_table.rows( { filter : 'applied'} ).data();

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
       console.log(applist_filter_keywords);

       for (key in applist_filter_keywords)
       {
        if (applist_filter_keywords[key])
         kw.push({label:key});
       }
       callback(kw);
     }
   }
 });

  $("#applist_pane").show();
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
  $("#rellist_pane").hide();

  $("#applist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
  $("#applist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");

   if (hideOption())
    $("#applist_buttons > div:nth-child(5) > button").hide();
   else
    $("#applist_buttons > div:nth-child(5) > button").show();

  $("#applist_buttons > button:nth-child(6)").show();
  $("#applist_buttons > button:nth-child(8)").addClass("title_buttons_disabled");
  $(".taskMenuButton").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof applist_table != "undefined" && applist_table != null)
   {
    applist_table.clear();
    applist_table.destroy();
    applist_table = null;
   }
   var h = $(document).height();
 //  $("#applist_list").height(h-8);

   $("#applist_list").show();
   $("#applist_map").hide();

  applist_table = $('#applist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollResize: true,
   scrollY: "60vh",
   scroller: true,
   pageLength: 25,
   destroy: true,
   sDom: 't<"bottom"lip>',
   "ajax": {
    "url": "/dmadminweb/ReportsData?type=AppList",
    "error": function (jqXHR, textStatus, errorThrown) {
		var token=GetCookie("token");
		console.log(token);

		if (token != null && token != "")
		{
         alert("Connection failed. Resetting your session.");
         window.location = "/dmadminweb/Logout";
        }
    },
    "type": "GET"
  },
  "rowCallback": function (row, data) {
	  		if (data.deployid == "Never Deployed")
			  $('td:eq(4)', row).html(data.deployid);
			else
  			  $('td:eq(4)', row).html("<a href=\"javascript:void(0);\" onclick=\"DisplayDeploy(" + data.deployid + ")\">#" + data.deployid + "</a>");
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
       defaultContent: '',
      orderable: false,
              width: 20,
      className: 'select-checkbox'
     },
     {
      targets: 1,
       "type": "objname",
     },
                 {
                  "targets": [ 7 ],
                  "visible": false
                 },
                 {
                  "targets": [ 8 ],
                  "visible": false
                 }
                ] ,
                select: {
                 style:    'os',
                 selector: 'td:first-child'
             }
  } );

  $('#applist_checkall').click(function() {
        if (applist_table.rows({
                selected: true
            }).count() > 0) {
            applist_table.rows().deselect();
            return;
        }

        applist_table.rows().select();
    });


  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=appfilter",
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

  if (app_dblclick)
  {
   app_dblclick = false;
   $('#applist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = applist_table.row( this ).data();
    eventOpenRow("applications",isAdmin,data);
   });
  }

  applist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#applications_tree";
    var data = applist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#applist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#applist_checkall i').attr('class', 'far fa-minus-square');

     lastsel_id = data[0]['id'];
     lastSelectedNode = data[0]['id'];
     lastset_rel = "";
     objid = lastsel_id.substr(2);
     objtype = lastsel_id.substr(0,2);
     objName = data[0]["name"];
     objDomainId = data[0]["domainid"];
     objtypeAsInt = obj2Int[objtype][0];
     objtypeName = obj2Int[objtype][1];
     objkind="";
     if (objtype == "pr" || objtype == "fn")
     {
      objkind=objid.substr(objid.indexOf("-")+1);
      objid = objid.substr(0,objid.indexOf("-"));
     }
    }
    $("#applist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    $("#applist_buttons > button:nth-child(4)").removeClass("title_buttons_disabled");
    $(".taskMenuButton").removeClass("title_buttons_disabled");

    if (data != null && data.length == 2)
    {
     $("#applist_buttons > button:nth-child(8)").removeClass("title_buttons_disabled");
    }
   }
  });

  applist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#applist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
    $("#applist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");
    $(".taskMenuButton").addClass("title_buttons_disabled");

    $("#applist_buttons > button:nth-child(8)").addClass("title_buttons_disabled");

	if (dt.rows({selected: true}).count() === 0) {
	  $('#applist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#applist_checkall i').attr('class', 'far fa-minus-square');
    }
   }
  });
 }
 else // map
 {
  var h = $(document).height();

  $("#applist_map").height(h-190);

  $("#applist_list").hide();
  $("#applist_map").show();

  var data = applist_table.rows().data();

  var ids = "";

  if (data != null && data.length > 0)
  {
   for (k=0;k<data.length;k++)
   {
    ids = ids + data[k]['id'].substr(2) + ",";
   }
  }

  $.ajax({
   url: "GetApps2Comps?appids=" + ids,
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

                    physics:{
                     enabled: true,
                     barnesHut: {
                       gravitationalConstant: -2000,
                       centralGravity: .1,
                       springLength: 250,
                       springConstant: 0.01,
                       damping: 0.09,
                       avoidOverlap: .2
                     }},
                     edges:{arrows: 'to',smooth: true},
                     nodes:{shape:'box'},
                     interaction:{navigationButtons: true}
    };

     var container = document.getElementById('applist_map');
     var network = new vis.Network(container, data, options);
     network.once('stabilized', function() {
 //     var scaleOption = { scale : .5 };
 //     network.moveTo(scaleOption);
  })
   }
  });
 }
 applist_table_resize();
}

/**** Comp List ****/
 function complist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#complist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getCompList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#comp_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=compfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       complist_filter_keywords = {"Version":true,"Domain":true,"Environment":true,"Last Deployment":true,"Result":true, "Parent":true};

       complist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Version'])
        {
         complist_filter_keywords["Version"] = false;
         myValue = searchterms[i]['Version']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         complist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(2).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Parent'])
        {
         complist_filter_keywords["Parent"] = false;
         myValue = searchterms[i]['Parent']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(3).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Environment'])
        {
         complist_filter_keywords["Environment"] = false;
         myValue = searchterms[i]['Environment']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(4).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Last Deployment'])
        {
         complist_filter_keywords["Last Deployment"] = false;
         myValue = searchterms[i]['Last Deployment']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(5).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Result'])
        {
         complist_filter_keywords["Result"] = false;
         myValue = searchterms[i]['Result']
         regExSearch = '^' + escapeRegExp(myValue);

         complist_table.column(7).search(regExSearch, true, false);
        }
       }
       complist_table.draw();
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
       case 'environment':
       case 'parent':
       case 'deployid':
       case 'exitcode':
           var data = complist_table.rows( { filter : 'applied'} ).data();

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
       for (key in complist_filter_keywords)
       {
        if (complist_filter_keywords[key])
         kw.push(key);
       }
       callback(kw);
     }
   }
 });

  $("#applist_pane").hide();
  $("#complist_pane").show();
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
  $("#rellist_pane").hide();
  $("#complist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
  $("#complist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");
  $("#complist_buttons > button:nth-child(5)").hide();
  $("#complist_buttons > button:nth-child(6)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof complist_table != "undefined" && complist_table != null)
   {
    complist_table.clear();
    complist_table.destroy();
    complist_table = null;
   }
   var h = $(document).height();
   $("#complist_list").height(h-160);

   $("#complist_list").show();
   $("#complist_map").hide();

  complist_table =$('#complist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=CompList",
    "type": "GET"
  },
    "rowCallback": function (row, data) {
			if (data.deployid == "Never Deployed")
			  $('td:eq(4)', row).html(data.deployid);
			else
  			  $('td:eq(4)', row).html("<a href=\"javascript:void(0);\" onclick=\"DisplayDeploy(" + data.deployid + ")\">#" + data.deployid + "</a>");
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
               targets: 1,
       		   "type": "objname",
     		  },
                 {
                  "targets": [ 7 ],
                  "visible": false
                 },
                 {
                  "targets": [ 8 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#complist_checkall').click(function() {
        if (complist_table.rows({
                selected: true
            }).count() > 0) {
            complist_table.rows().deselect();
            return;
        }

        complist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=compfilter",
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

  if (comp_dblclick)
  {
   comp_dblclick = false;
   $('#complist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = complist_table.row( this ).data();
    eventOpenRow("components",isAdmin,data);
   });
  }

  complist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#components_tree";
    var data = complist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#complist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#complist_checkall i').attr('class', 'far fa-minus-square');

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
    $("#complist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    $("#complist_buttons > button:nth-child(4)").removeClass("title_buttons_disabled");
    $("#complist_buttons > button:nth-child(6)").removeClass("title_buttons_disabled");

    if (data != null && data.length == 2)
    {
     $("#complist_buttons > button:nth-child(6)").removeClass("title_buttons_disabled");
    }
   }
  });

  complist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#complist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
    $("#complist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");

    $("#complist_buttons > button:nth-child(6)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#complist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#complist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
  else // map
  {
   var h = $(document).height();

   $("#complist_map").height(h-190);

   $("#complist_list").hide();
   $("#complist_map").show();

   var data = complist_table.rows().data();

   var ids = "";

   if (data != null && data.length > 0)
   {
    for (k=0;k<data.length;k++)
    {
     ids = ids + data[k]['id'].substr(2) + ",";
    }
   }

   $.ajax({
    url: "GetComps2Apps?compids=" + ids,
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

                     physics:{
                      enabled: true,
                      barnesHut: {
                        gravitationalConstant: -2000,
                        centralGravity: .1,
                        springLength: 250,
                        springConstant: 0.01,
                        damping: 0.09,
                        avoidOverlap: .2
                      }},
                      edges:{arrows: 'to',smooth: true},
                      nodes:{shape:'box'},
                      interaction:{navigationButtons: true}
     };

      var container = document.getElementById('complist_map');
      var network = new vis.Network(container, data, options);
      network.once('stabilized', function() {
//       var scaleOption = { scale : .5 };
//       network.moveTo(scaleOption);
//       network.fit();
   })
    }
   });
  }
 complist_table_resize();
}
/***** End Comp List  *****/
/***** Start Env List *****/

 function envlist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#envlist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getEnvList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#env_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=envfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       envlist_filter_keywords = {"Environment":true,"Domain":true};

       envlist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Environment'])
        {
         envlist_filter_keywords["Environment"] = false;
         myValue = searchterms[i]['Environment']
         regExSearch = '^' + escapeRegExp(myValue);

         envlist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         envlist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         envlist_table.column(2).search(regExSearch, true, false);
        }
       }
       envlist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "environment")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = envlist_table.rows( { filter : 'applied'} ).data();

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
       for (key in envlist_filter_keywords)
       {
        if (envlist_filter_keywords[key])
         kw.push(key);
       }
       callback(kw);
     }
   }
 });

  $("#applist_pane").hide();
  $("#complist_pane").hide();
  $("#envlist_pane").show();
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
  $("#rellist_pane").hide();
  $("#envlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
  $("#envlist_buttons > button:nth-child(5)").addClass("title_buttons_disabled");
  $("#envlist_buttons > button:nth-child(5)").show();

  if (displaytype == "list")
  {
   if (typeof envlist_table != "undefined" && envlist_table != null)
   {
    envlist_table.clear();
    envlist_table.destroy();
    envlist_table = null;
   }
   var h = $(document).height();
   $("#envlist_list").height(h-160);

  envlist_table =$('#envlist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=EnvList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#envlist_checkall').click(function() {
        if (envlist_table.rows({
                selected: true
            }).count() > 0) {
            envlist_table.rows().deselect();
            return;
        }

        envlist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=envfilter",
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

  if (env_dblclick)
  {
   env_dblclick = false;
   $('#envlist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = envlist_table.row( this ).data();
    eventOpenRow("environments",isAdmin,data);
   });
  }

  envlist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#environments_tree";
    var data = envlist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#envlist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#envlist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#envlist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");

     if (data != null && data.length == 2)
     {
      $("#envlist_buttons > button:nth-child(5)").removeClass("title_buttons_disabled");
     }
    }
   }
  });

  envlist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#envlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
    $("#envlist_buttons > button:nth-child(5)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#envlist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#envlist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }

 envlist_table_resize();
}
 /***** End Env List  *****/
 /***** Start Endpoint List *****/

 function endpointlist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#endpointlist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getEndpointList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#endpoint_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=endpointfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       endpointlist_filter_keywords = {"Endpoint":true,"Domain":true};

       endpointlist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Endpoint'])
        {
         endpointlist_filter_keywords["Endpoint"] = false;
         myValue = searchterms[i]['Endpoint']
         regExSearch = '^' + escapeRegExp(myValue);

         endpointlist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         endpointlist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         endpointlist_table.column(2).search(regExSearch, true, false);
        }
       }
       endpointlist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "endpoint")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = endpointlist_table.rows( { filter : 'applied'} ).data();

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
       for (key in endpointlist_filter_keywords)
       {
        if (endpointlist_filter_keywords[key])
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
  $("#endpointlist_pane").show();
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
  $("#rellist_pane").hide();
  $("#endpointlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof endpointlist_table != "undefined" && endpointlist_table != null)
   {
    endpointlist_table.clear();
    endpointlist_table.destroy();
    endpointlist_table = null;
   }
   var h = $(document).height();
   $("#endpointlist_list").height(h-160);

   $("#endpointlist_list").show();

  endpointlist_table =$('#endpointlist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=EndPointList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#endpointlist_checkall').click(function() {
        if (endpointlist_table.rows({
                selected: true
            }).count() > 0) {
            endpointlist_table.rows().deselect();
            return;
        }

        endpointlist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=endpointfilter",
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

  if (endpoint_dblclick)
  {
   endpoint_dblclick = false;
   $('#endpointlist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = endpointlist_table.row( this ).data();
    eventOpenRow("endpoints",isAdmin,data);
   });
  }

  endpointlist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#servers_tree";
    var data = endpointlist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#endpointlist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#endpointlist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#endpointlist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  endpointlist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#endpointlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#endpointlist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#endpointlist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }

endpointlist_table_resize();
}
 /***** End Endpoint List  *****/
 /***** Start Build Engine List *****/

 function buildenglist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#buildenglist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getBuildEngList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#buildeng_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=buildengfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       buildenglist_filter_keywords = {"Build Engine":true,"Domain":true};

       buildenglist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]["Build Engine"])
        {
         buildenglist_filter_keywords["Build Engine"] = false;
         myValue = searchterms[i]["Build Engine"]
         regExSearch = '^' + escapeRegExp(myValue);

         buildenglist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         buildenglist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         buildenglist_table.column(2).search(regExSearch, true, false);
        }
       }
       buildenglist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "build engine")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = buildenglist_table.rows( { filter : 'applied'} ).data();

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
       for (key in buildenglist_filter_keywords)
       {
        if (buildenglist_filter_keywords[key])
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
  $("#buildenglist_pane").show();
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
  $("#rellist_pane").hide();
  $("#buildenglist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof buildenglist_table != "undefined" && buildenglist_table != null)
   {
    buildenglist_table.clear();
    buildenglist_table.destroy();
    buildenglist_table = null;
   }
   var h = $(document).height();
   $("#buildenglist_list").height(h-160);

   $("#buildenglist_list").show();

  buildenglist_table =$('#buildenglist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=buildengList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#buildenglist_checkall').click(function() {
        if (buildenglist_table.rows({
                selected: true
            }).count() > 0) {
            buildenglist_table.rows().deselect();
            return;
        }

        buildenglist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=buildengfilter",
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

  if (buildeng_dblclick)
  {
   buildeng_dblclick = false;
   $('#buildenglist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = buildenglist_table.row( this ).data();
    eventOpenRow("buildengs",isAdmin,data);
   });
  }

  buildenglist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#builders_tree";
    var data = buildenglist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#buildenglist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#buildenglist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#buildenglist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  buildenglist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#buildenglist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#buildenglist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#buildenglist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 buildenglist_table_resize();
}
 /***** End Build Engine List  *****/
  /***** Start Actions List *****/

 function actionlist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#actopmlist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getActionList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#action_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=actionfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       actionlist_filter_keywords = {"Action":true,"Domain":true};

       actionlist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Action'])
        {
         actionlist_filter_keywords["Action"] = false;
         myValue = searchterms[i]['Action']
         regExSearch = '^' + escapeRegExp(myValue);

         actionlist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         actionlist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         actionlist_table.column(2).search(regExSearch, true, false);
        }
       }
       actionlist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "action")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = actionlist_table.rows( { filter : 'applied'} ).data();

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
       for (key in actionlist_filter_keywords)
       {
        if (actionlist_filter_keywords[key])
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
  $("#actionlist_pane").show();
  $("#procedurelist_pane").hide();
  $("#notifierlist_pane").hide();
  $("#repositorylist_pane").hide();
  $("#datasourcelist_pane").hide();
  $("#credentiallist_pane").hide();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#actionlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof actionlist_table != "undefined" && actionlist_table != null)
   {
    actionlist_table.clear();
    actionlist_table.destroy();
    actionlist_table = null;
   }
   var h = $(document).height();
   $("#actionlist_list").height(h-160);

   $("#actionlist_list").show();

  actionlist_table =$('#actionlist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=actionList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#actionlist_checkall').click(function() {
        if (actionlist_table.rows({
                selected: true
            }).count() > 0) {
            actionlist_table.rows().deselect();
            return;
        }

        actionlist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=actionfilter",
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

  if (action_dblclick)
  {
   action_dblclick = false;
   $('#actionlist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = actionlist_table.row( this ).data();
    eventOpenRow("actions",isAdmin,data);
   });
  }

  actionlist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#actions_tree";
    var data = actionlist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#actiolist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#actionlist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#actionlist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  actionlist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#actionlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#actionlist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#actionlist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
}
 /***** End Actions List  *****/
  /***** Start Procedure List *****/

 function exportFuncProc()
 {
  var data = procedurelist_table.rows({selected:  true}).data();

  if (data == null || data.length == 0)
   return;

  var newarray=[];
  var msg = "";

  for (var i=0; i < data.length ;i++)
  {
   var workid = data[i]['id'].substr(2).split("-");

   newarray.push(workid[0]);

   if (i==0)
     msg += data[i]['name'];
   else
     msg += ", " + data[i]['name'];
  }

  for (var i=0;i<newarray.length;i++)
  {
   var id = newarray[i];

   $.fileDownload("GetActionLayout?f=export&actionid="+id).done(function() {
   }).fail(function() {
    alert("failed!");
   });
  }
actionlist_table_resize();
}

 function procedurelist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#procedurelist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getProcedureList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#procedure_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=procedurefilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       procedurelist_filter_keywords = {"Name":true,"Domain":true,"Type":true};

       procedurelist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Name'])
        {
         procedurelist_filter_keywords["Name"] = false;
         myValue = searchterms[i]['Name']
         regExSearch = '^' + escapeRegExp(myValue);

         procedurelist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         procedurelist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         procedurelist_table.column(2).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Type'])
        {
         procedurelist_filter_keywords["Type"] = false;
         myValue = searchterms[i]['Type']
         regExSearch = '^' + escapeRegExp(myValue);

         procedurelist_table.column(2).search(regExSearch, true, false);
        }
       }
       procedurelist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       switch (category) {
       case 'domain':
       case 'type':
       case 'name':
           var data = procedurelist_table.rows( { filter : 'applied'} ).data();

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
       for (key in procedurelist_filter_keywords)
       {
        if (procedurelist_filter_keywords[key])
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
  $("#procedurelist_pane").show();
  $("#notifierlist_pane").hide();
  $("#repositorylist_pane").hide();
  $("#datasourcelist_pane").hide();
  $("#credentiallist_pane").hide();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#procedurelist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");
  $("#procedurelist_buttons > button:nth-child(5)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof procedurelist_table != "undefined" && procedurelist_table != null)
   {
    procedurelist_table.clear();
    procedurelist_table.destroy();
    procedurelist_table = null;
   }
   var h = $(document).height();
   $("#procedurelist_list").height(h-160);

   $("#procedurelist_list").show();

  procedurelist_table =$('#procedurelist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=procedureList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
       { "data": "type"},
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
                  "targets": [ 4 ],
                  "visible": false
                 },
                 {
                  "targets": [ 5 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#procedurelist_checkall').click(function() {
        if (procedurelist_table.rows({
                selected: true
            }).count() > 0) {
            procedurelist_table.rows().deselect();
            return;
        }

        procedurelist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=procedurefilter",
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

  if (procedure_dblclick)
  {
   procedure_dblclick = false;
   $('#procedurelist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = procedurelist_table.row( this ).data();
    eventOpenRow("procedures",isAdmin,data);
   });
  }

  procedurelist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#procedures_tree";
    var data = procedurelist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#procedurelist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#procedurelist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#procedurelist_buttons > button:nth-child(4)").removeClass("title_buttons_disabled");
     $("#procedurelist_buttons > button:nth-child(5)").removeClass("title_buttons_disabled");
    }
   }
  });

  procedurelist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#procedurelist_buttons > button:nth-child(4)").addClass("title_buttons_disabled");
    $("#procedurelist_buttons > button:nth-child(5)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#procedurelist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#procedurelist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 procedurelist_table_resize();
}
 /***** End Procedure List  *****/
 /***** Start Notifier List *****/

 function notifierlist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#notifierlist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getNotifierList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#notifier_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=notifierfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       notifierlist_filter_keywords = {"Notifier":true,"Domain":true};

       notifierlist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Notifier'])
        {
         notifierlist_filter_keywords["Notifier"] = false;
         myValue = searchterms[i]['Notifier']
         regExSearch = '^' + escapeRegExp(myValue);

         notifierlist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         notifierlist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         notifierlist_table.column(2).search(regExSearch, true, false);
        }
       }
       notifierlist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "notifier")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = notifierlist_table.rows( { filter : 'applied'} ).data();

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
       for (key in notifierlist_filter_keywords)
       {
        if (notifierlist_filter_keywords[key])
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
  $("#notifierlist_pane").show();
  $("#repositorylist_pane").hide();
  $("#datasourcelist_pane").hide();
  $("#credentiallist_pane").hide();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#notifierlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof notifierlist_table != "undefined" && notifierlist_table != null)
   {
    notifierlist_table.clear();
    notifierlist_table.destroy();
    notifierlist_table = null;
   }
   var h = $(document).height();
   $("#notifierlist_list").height(h-160);

   $("#notifierlist_list").show();

  notifierlist_table =$('#notifierlist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=notifierList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#notifierlist_checkall').click(function() {
        if (notifierlist_table.rows({
                selected: true
            }).count() > 0) {
            notifierlist_table.rows().deselect();
            return;
        }

        notifierlist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=notifierfilter",
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

  if (notifier_dblclick)
  {
   notifier_dblclick = false;
   $('#notifierlist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = notifierlist_table.row( this ).data();
    eventOpenRow("notifiers",isAdmin,data);
   });
  }

  notifierlist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#notifiers_tree";
    var data = notifierlist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#notifierlist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#notifierlist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#notifierlist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  notifierlist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#notifierlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#notifierlist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#notifierlist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 notifierlist_table_resize();
}
 /***** End Notifier List  *****/
 /***** Start Repository List *****/

 function repositorylist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#repositorylist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getRepositoryList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#repository_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=repositoryfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       repositorylist_filter_keywords = {"Repository":true,"Domain":true};

       repositorylist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Repository'])
        {
         repositorylist_filter_keywords["Repository"] = false;
         myValue = searchterms[i]['Repository']
         regExSearch = '^' + escapeRegExp(myValue);

         repositorylist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         repositorylist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         repositorylist_table.column(2).search(regExSearch, true, false);
        }
       }
       repositorylist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "repository")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = repositorylist_table.rows( { filter : 'applied'} ).data();

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
       for (key in repositorylist_filter_keywords)
       {
        if (repositorylist_filter_keywords[key])
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
  $("#repositorylist_pane").show();
  $("#datasourcelist_pane").hide();
  $("#credentiallist_pane").hide();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#repositorylist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof repositorylist_table != "undefined" && repositorylist_table != null)
   {
    repositorylist_table.clear();
    repositorylist_table.destroy();
    repositorylist_table = null;
   }
   var h = $(document).height();
   $("#repositorylist_list").height(h-160);

   $("#repositorylist_list").show();

  repositorylist_table =$('#repositorylist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=repositoryList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );


  $('#repositorylist_checkall').click(function() {
        if (repositorylist_table.rows({
                selected: true
            }).count() > 0) {
            repositorylist_table.rows().deselect();
            return;
        }

        repositorylist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=repositoryfilter",
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

  if (repository_dblclick)
  {
   repository_dblclick = false;
   $('#repositorylist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = repositorylist_table.row( this ).data();
    eventOpenRow("repositories",isAdmin,data);
   });
  }

  repositorylist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#builders_tree";
    var data = repositorylist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
     if (dt.rows().count() == data.length)
	   $('#repositorylist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#repositorylist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#repositorylist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  repositorylist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#repositorylist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#repositorylist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#repositorylist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 repositorylist_table_resize();
}
 /***** End Repository List  *****/
 /***** Start Datasource List *****/

 function datasourcelist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#datasourcelist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getDatasourceList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#datasource_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=datasourcefilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       datasourcelist_filter_keywords = {"Data Source":true,"Domain":true};

       datasourcelist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Data Source'])
        {
         datasourcelist_filter_keywords["Data Source"] = false;
         myValue = searchterms[i]['Data Source']
         regExSearch = '^' + escapeRegExp(myValue);

         datasourcelist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         datasourcelist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         datasourcelist_table.column(2).search(regExSearch, true, false);
        }
       }
       datasourcelist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "data source")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = datasourcelist_table.rows( { filter : 'applied'} ).data();

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
       for (key in datasourcelist_filter_keywords)
       {
        if (datasourcelist_filter_keywords[key])
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
  $("#datasourcelist_pane").show();
  $("#credentiallist_pane").hide();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#datasourcelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof datasourcelist_table != "undefined" && datasourcelist_table != null)
   {
    datasourcelist_table.clear();
    datasourcelist_table.destroy();
    datasourcelist_table = null;
   }
   var h = $(document).height();
   $("#datasourcelist_list").height(h-160);

   $("#datasourcelist_list").show();

  datasourcelist_table =$('#datasourcelist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=datasourceList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#datasourcelist_checkall').click(function() {
        if (datasourcelist_table.rows({
                selected: true
            }).count() > 0) {
            datasourcelist_table.rows().deselect();
            return;
        }

        datasourcelist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=datasourcefilter",
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

  if (datasource_dblclick)
  {
   datasource_dblclick = false;
   $('#datasourcelist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = datasourcelist_table.row( this ).data();
    eventOpenRow("datasources",isAdmin,data);
   });
  }

  datasourcelist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#datasources_tree";
    var data = datasourcelist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	if (dt.rows().count() == data.length)
	   $('#datasourcelist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#datasourcelist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#datasourcelist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  datasourcelist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#datasourcelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#datasourcelist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#datasourcelist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
datasourcelist_table_resize();
}
 /***** End Datasource List  *****/
 /***** Start Credential List *****/

 function credentiallist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#credentiallist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getCredentialList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#credential_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=credentialfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       credentiallist_filter_keywords = {"Credential":true,"Domain":true};

       credentiallist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Credential'])
        {
         credentiallist_filter_keywords["Credential"] = false;
         myValue = searchterms[i]['Credential']
         regExSearch = '^' + escapeRegExp(myValue);

         credentiallist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         credentiallist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         credentiallist_table.column(2).search(regExSearch, true, false);
        }
       }
       credentiallist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "credential")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = credentiallist_table.rows( { filter : 'applied'} ).data();

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
       for (key in credentiallist_filter_keywords)
       {
        if (credentiallist_filter_keywords[key])
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
  $("#credentiallist_pane").show();
  $("#userlist_pane").hide();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#credentiallist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof credentiallist_table != "undefined" && credentiallist_table != null)
   {
    credentiallist_table.clear();
    credentiallist_table.destroy();
    credentiallist_table = null;
   }
   var h = $(document).height();
   $("#credentiallist_list").height(h-160);

   $("#credentiallist_list").show();

  credentiallist_table =$('#credentiallist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=credentialList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#credentiallist_checkall').click(function() {
        if (credentiallist_table.rows({
                selected: true
            }).count() > 0) {
            credentiallist_table.rows().deselect();
            return;
        }

        credentiallist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=credentialfilter",
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

  if (credential_dblclick)
  {
   credential_dblclick = false;
   $('#credentiallist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = credentiallist_table.row( this ).data();
    eventOpenRow("credentials",isAdmin,data);
   });
  }

  credentiallist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#credentials_tree";
    var data = credentiallist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#credentiallist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#credentiallist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#credentiallist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  credentiallist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#credentiallist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#credentiallist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#credentiallist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 credentiallist_table_resize();
}
 /***** End Credential List  *****/
 /***** Start User List *****/

 function userlist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#userlist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getUserList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#user_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=userfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       userlist_filter_keywords = {"User":true,"Domain":true};

       userlist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]["User"])
        {
         userlist_filter_keywords["User"] = false;
         myValue = searchterms[i]["User"]
         regExSearch = '^' + escapeRegExp(myValue);

         userlist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         userlist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         userlist_table.column(2).search(regExSearch, true, false);
        }
       }
       userlist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "user")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = userlist_table.rows( { filter : 'applied'} ).data();

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
       for (key in userlist_filter_keywords)
       {
        if (userlist_filter_keywords[key])
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
  $("#userlist_pane").show();
  $("#grouplist_pane").hide();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#userlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof userlist_table != "undefined" && userlist_table != null)
   {
    userlist_table.clear();
    userlist_table.destroy();
    userlist_table = null;
   }
   var h = $(document).height();
   $("#userlist_list").height(h-160);

   $("#userlist_list").show();

  userlist_table =$('#userlist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=userList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#userlist_checkall').click(function() {
        if (userlist_table.rows({
                selected: true
            }).count() > 0) {
            userlist_table.rows().deselect();
            return;
        }

        userlist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=userfilter",
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

  if (user_dblclick)
  {
   user_dblclick = false;
   $('#userlist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = userlist_table.row( this ).data();
    eventOpenRow("users",isAdmin,data);
   });
  }

  userlist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#users_tree";
    var data = userlist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#userlist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#userlist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#userlist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  userlist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#userlist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#userlist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#userlist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 userlist_table_resize();
}
 /***** End User List  *****/
 /***** Start Group List *****/

 function grouplist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#grouplist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getGroupList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#group_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=groupfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       grouplist_filter_keywords = {"Group":true,"Domain":true};

       grouplist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]["Group"])
        {
         grouplist_filter_keywords["Group"] = false;
         myValue = searchterms[i]["Group"]
         regExSearch = '^' + escapeRegExp(myValue);

         grouplist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         grouplist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         grouplist_table.column(2).search(regExSearch, true, false);
        }
       }
       grouplist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "group")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = grouplist_table.rows( { filter : 'applied'} ).data();

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
       for (key in grouplist_filter_keywords)
       {
        if (grouplist_filter_keywords[key])
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
  $("#grouplist_pane").show();
  $("#servercomptypelist_pane").hide();
  $("#templatelist_pane").hide();
  $("#rellist_pane").hide();
  $("#grouplist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof grouplist_table != "undefined" && grouplist_table != null)
   {
    grouplist_table.clear();
    grouplist_table.destroy();
    grouplist_table = null;
   }
   var h = $(document).height();
   $("#grouplist_list").height(h-160);

   $("#grouplist_list").show();

  grouplist_table =$('#grouplist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=groupList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );


  $('#grouplist_checkall').click(function() {
        if (grouplist_table.rows({
                selected: true
            }).count() > 0) {
            grouplist_table.rows().deselect();
            return;
        }

        grouplist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=groupfilter",
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

  if (group_dblclick)
  {
   group_dblclick = false;
   $('#grouplist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = grouplist_table.row( this ).data();
    eventOpenRow("groups",isAdmin,data);
   });
  }

  grouplist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#groups_tree";
    var data = grouplist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#grouplist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#grouplist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#grouplist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  grouplist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#grouplist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#grouplist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#grouplist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 grouplist_table_resize();
}
 /***** End Group List  *****/
 /***** Start Server Comp Type List *****/

 function servercomptypelist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#servercomptypelist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getServerCompTypeList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#servercomptype_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=servercomptypefilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       servercomptypelist_filter_keywords = {"Server Comp Type":true,"Domain":true};

       servercomptypelist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Server Comp Type'])
        {
         servercomptypelist_filter_keywords["Server Comp Type"] = false;
         myValue = searchterms[i]['Server Comp Type']
         regExSearch = '^' + escapeRegExp(myValue);

         servercomptypelist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Domain'])
        {
         servercomptypelist_filter_keywords["Domain"] = false;
         myValue = searchterms[i]['Domain']
         regExSearch = '^' + escapeRegExp(myValue);

         servercomptypelist_table.column(2).search(regExSearch, true, false);
        }
       }
       servercomptypelist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "server comp type")
        category = "name";

       switch (category) {
       case 'domain':
       case 'name':
           var data = servercomptypelist_table.rows( { filter : 'applied'} ).data();

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
       for (key in servercomptypelist_filter_keywords)
       {
        if (servercomptypelist_filter_keywords[key])
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
  $("#servercomptypelist_pane").show();
  $("#rellist_pane").hide();
  $("#servercomptypelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof servercomptypelist_table != "undefined" && servercomptypelist_table != null)
   {
    servercomptypelist_table.clear();
    servercomptypelist_table.destroy();
    servercomptypelist_table = null;
   }
   var h = $(document).height();
   $("#servercomptypelist_list").height(h-160);

   $("#servercomptypelist_list").show();

  servercomptypelist_table =$('#servercomptypelist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=servercomptypeList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "domain" },
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
                  "targets": [ 3 ],
                  "visible": false
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#servercomptypelist_checkall').click(function() {
        if (servercomptypelist_table.rows({
                selected: true
            }).count() > 0) {
            servercomptypelist_table.rows().deselect();
            return;
        }

        servercomptypelist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=servercomptypefilter",
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

  if (servercomptype_dblclick)
  {
   servercomptype_dblclick = false;
   $('#servercomptypelist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = servercomptypelist_table.row( this ).data();
    eventOpenRow("servercomptypes",isAdmin,data);
   });
  }

  servercomptypelist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#types_tree";
    var data = servercomptypelist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#servercomptypelist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#servercomptypelist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#servercomptypelist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  servercomptypelist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#servercomptypelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#servercomptypelist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#servercomptypelist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 servercomptypelist_table_resize();
}
 /***** End Server Comp Type List  *****/
 /***** Start Template List *****/

 function templatelist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#templatelist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getTemplateList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#template_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=templatefilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       templatelist_filter_keywords = {"Template":true,"Notifier":true};

       templatelist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]['Template'])
        {
         templatelist_filter_keywords["Template"] = false;
         myValue = searchterms[i]['Template']
         regExSearch = '^' + escapeRegExp(myValue);

         templatelist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]['Notifier'])
        {
         templatelist_filter_keywords["Notifier"] = false;
         myValue = searchterms[i]['Notifier']
         regExSearch = '^' + escapeRegExp(myValue);

         templatelist_table.column(2).search(regExSearch, true, false);
        }
       }
       templatelist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "template")
        category = "name";

       switch (category) {
       case 'notifier':
       case 'name':
           var data = templatelist_table.rows( { filter : 'applied'} ).data();

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
       for (key in templatelist_filter_keywords)
       {
        if (templatelist_filter_keywords[key])
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
  $("#templatelist_pane").show();
  $("#rellist_pane").hide();
  $("#templatelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof templatelist_table != "undefined" && templatelist_table != null)
   {
    templatelist_table.clear();
    templatelist_table.destroy();
    templatelist_table = null;
   }
   var h = $(document).height();
   $("#templatelist_list").height(h-160);

   $("#templatelist_list").show();

  templatelist_table =$('#templatelist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=templateList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "notifier" },
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
                  "targets": [ 3 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#templatelist_checkall').click(function() {
        if (templatelist_table.rows({
                selected: true
            }).count() > 0) {
            templatelist_table.rows().deselect();
            return;
        }

        templatelist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=templatefilter",
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

  if (template_dblclick)
  {
   template_dblclick = false;
   $('#templatelist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = templatelist_table.row( this ).data();
    eventOpenRow("templates",isAdmin,data);
   });
  }

  templatelist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#templates_tree";
    var data = templatelist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#templatelist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#templatelist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#templatelist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  templatelist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#templatelist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#templatelist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#templatelist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 templatelist_table_resize();
}
 /***** End Template List  *****/
 /***** Start Build Job List *****/

 function buildjoblist_table_resize(){
  var h = $("#panel_container_menu").height();
  $("#buildjoblist_list").height(h-60);
  $(".dataTables_scrollBody").css('max-height', (h-144) + "px");
 }

 function getBuildJobList(displaytype)
 {
  window.visualSearch = VS.init({
   container  : $('#buildjob_search_box_container'),
   query      : "",
   showFacets : true,
   readOnly   : false,
   unquotable : [],
   callbacks  : {
     search : function(query, searchCollection) {
       var searchterms = window.visualSearch.searchQuery.facets();

       $.ajax({
        type : "POST",
        url : "TableFilter?filtername=buildjobfilter&filtervalue=" + encodeURIComponent(JSON.stringify(searchterms)),
        dataType: "json"
       }).done(function(data) {
           console.log(data);
       });

       buildjoblist_filter_keywords = {"Build Job":true,"Build Engine":true};

       buildjoblist_table.search( '' ).columns().search( '' ).draw();

       for (i=0;i<searchterms.length;i++)
       {
        if (searchterms[i]["Build Job"])
        {
         buildjoblist_filter_keywords["Build Job"] = false;
         myValue = searchterms[i]["Build Job"]
         regExSearch = '^' + escapeRegExp(myValue);

         buildjoblist_table.column(1).search(regExSearch, true, false);
        }
        else if (searchterms[i]["Build Engine"])
        {
         buildjoblist_filter_keywords["Build Engine"] = false;
         myValue = searchterms[i]["Build Engine"]
         regExSearch = '^' + escapeRegExp(myValue);

         buildjoblist_table.column(2).search(regExSearch, true, false);
        }
       }
       buildjoblist_table.draw();
     },
     valueMatches : function(category, searchTerm, callback) {
       category = category.toLowerCase();

       if (category == "build job")
        category = "name";

       switch (category) {
       case 'build engine':
       case 'name':
           var data = buildjoblist_table.rows( { filter : 'applied'} ).data();

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
       for (key in buildjoblist_filter_keywords)
       {
        if (buildjoblist_filter_keywords[key])
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
  $("#buildjoblist_pane").show();
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
  $("#rellist_pane").hide();
  $("#buildjoblist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");

  if (displaytype == "list")
  {
   if (typeof buildjoblist_table != "undefined" && buildjoblist_table != null)
   {
    buildjoblist_table.clear();
    buildjoblist_table.destroy();
    buildjoblist_table = null;
   }
   var h = $(document).height();
   $("#buildjoblist_list").height(h-160);

   $("#buildjoblist_list").show();

  buildjoblist_table =$('#buildjoblist').DataTable( {
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
    "url": "/dmadminweb/ReportsData?type=buildjobList",
    "type": "GET"
  },
  "order": [[ 1, "asc" ]],
  "columns": [
              { "data": null},
       { "data": "name" },
       { "data": "build engine" },
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
                  "targets": [ 3 ],
                  "visible": false
                 }
                ],
                select: {
                 style:    'os',
                 selector: 'td:first-child'
                }
  } );

  $('#buildjoblist_checkall').click(function() {
        if (buildjoblist_table.rows({
                selected: true
            }).count() > 0) {
            buildjoblist_table.rows().deselect();
            return;
        }

        buildjoblist_table.rows().select();
    });

  $.ajax({
   type : "GET",
   async: false,
   url : "TableFilter?filtername=buildjobfilter",
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

  if (buildjob_dblclick)
  {
   buildjob_dblclick = false;
   $('#buildjoblist tbody').on('dblclick', 'tr', function (e) {
    e.stopPropagation();
    var data = buildjoblist_table.row( this ).data();
    eventOpenRow("buildjobs",isAdmin,data);
   });
  }

  buildjoblist_table.on( 'select', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    currenttree = "#builders_tree";
    var data = buildjoblist_table.rows({selected:  true}).data();
    if (data != null && data.length > 0)
    {
	 if (dt.rows().count() == data.length)
	   $('#buildjoblist_checkall i').attr('class', 'far fa-check-square');
     else
	   $('#buildjoblist_checkall i').attr('class', 'far fa-minus-square');

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
     $("#buildjoblist_buttons > button:nth-child(3)").removeClass("title_buttons_disabled");
    }
   }
  });

  buildjoblist_table.on( 'deselect', function ( e, dt, type, indexes ) {
   if ( type === 'row' ) {
    $("#buildjoblist_buttons > button:nth-child(3)").addClass("title_buttons_disabled");
   }

	if (dt.rows({selected: true}).count() === 0) {
	  $('#buildjoblist_checkall i').attr('class', 'far fa-square');
    }
    else
    {
	 $('#buildjoblist_checkall i').attr('class', 'far fa-minus-square');
    }
  });
 }
 buildjoblist_table_resize();
}
 /***** End Build Job List  *****/

/***** Start Provides List *****/

 function getProvidesList(displaytype)
 {
  if (displaytype == "list")
  {
   if (typeof provideslist_table != "undefined" && provideslist_table != null)
   {
    provideslist_table.clear();
    provideslist_table.destroy();
    provideslist_table = null;
   }
   var h = $(document).height();
   $("#provideslist_list").height(h-160);

   $("#provideslist_list").show();

  provideslist_table =$('#provideslist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   scrollCollapse: true,
   scroller: true,
   pageLength: 25,
   destroy: true,
   sDom: 't<"bottom"lip>',
   "ajax": {
    "url": "/dmadminweb/API/provides/" + objid,
    "type": "GET"
  },
  "columnDefs": [
    {
        targets: 0,
        className: 'dt-body-left'
    },
    {
        targets: 1,
        className: 'dt-body-left'
    }
  ],
  "columns": [
       { "data": "verb" },
       { "data": "path" },
   ]
   });
  }
 }

/***** End Provides List  *****/
/***** Start Consumes List *****/

 function getConsumesList(displaytype)
 {
  if (displaytype == "list")
  {
   if (typeof consumeslist_table != "undefined" && consumeslist_table != null)
   {
    consumeslist_table.clear();
    consumeslist_table.destroy();
    consumeslist_table = null;
   }
   var h = $(document).height();
   $("#consumeslist_list").height(h-160);

   $("#consumeslist_list").show();

  consumeslist_table =$('#consumeslist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   scrollCollapse: true,
   scroller: true,
   pageLength: 25,
   destroy: true,
   sDom: 't<"bottom"lip>',
   "ajax": {
    "url": "/dmadminweb/API/consumes/" + objid,
    "type": "GET"
  },
  "columnDefs": [
    {
        targets: 0,
        className: 'dt-body-left'
    },
    {
        targets: 1,
        className: 'dt-body-left'
    },
    {
        targets: 2,
        className: 'dt-body-left'
    }
  ],
  "columns": [
       { "data": "verb" },
       { "data": "path" },
       { "data": "comp" },
   ]
   });
  }
 }

/***** End Consumes List  *****/

/***** Start License List *****/

 function getLicenseList(displaytype)
 {
  if (displaytype == "list")
  {
   var url = "";

	var data = {};

    $.ajax({
        url: "/msapi/sbomtype",
        type: "GET",
        async: false,
        dataType: "json",
        success: function(response) {
            data = response;
        },
        error: function(xhr, status, error) {
        }
    });

   console.log(data);

   if (data.SBOMType == "fullfile")
   {
    if (objtype == "ap" || objtype == "av")
      url = "/msapi/package?deptype=license&appid=" + getCompIds4App(objid);
    else
      url = "/msapi/package?deptype=license&compid=" + objid;

    var saveData = [];

    $.ajax({
        url: url,
        type: "GET",
        dataType: "json",
        async: false,
        success: function(response) {
            saveData = response.data;
        },
        error: function(xhr, status, error) {
        }
    });

    purls = []
	for (let i = 0; i < saveData.length; i++) {
		if (saveData[i].purl != "" && saveData[i].purl != null)
	    	purls.push(saveData[i].purl);
	}

	$.ajax({
        url: "/dmadminweb/API/getscore",
        type: "POST",
        dataType: "json",
        data: JSON.stringify(purls),
        async: false,
        success: function(response) {
			 var data = response;
             for (let i = 0; i < saveData.length; i++) {
				if (saveData[i].purl != null && saveData[i].purl != "" && saveData[i].purl in data)
				 {
					saveData[i].score = data[saveData[i].purl];
	    		 }
			}
        },
        error: function(xhr, status, error) {
        }
    });

    if (objtype == "ap" || objtype == "av")
      url = "/msapi/deppkg?deptype=license&appid=" + objid;
    else
      url = "/msapi/deppkg?deptype=license&compid=" + objid;

    $.ajax({
        url: url,
        type: "GET",
        dataType: "json",
        success: function(response) {
            var data2 = response.data;
            var rows =  saveData.concat(data2);
            displayLicTable(rows);
        },
        error: function(xhr, status, error) {
        }
    });
   }
   else
   {
    if (objtype == "ap" || objtype == "av")
      url = "/msapi/deppkg?deptype=license&appid=" + objid;
    else
      url = "/msapi/deppkg?deptype=license&compid=" + objid;

    $.ajax({
        url: url,
        type: "GET",
        dataType: "json",
        success: function(response) {
            rows = response.data;
            displayLicTable(rows);
        },
        error: function(xhr, status, error) {
        }
    });
   }
  }
 }

 function displayLicTable(data)
 {
   if (typeof licenselist_table != "undefined" && licenselist_table != null)
   {
    licenselist_table.clear();
    licenselist_table.destroy();
    licenselist_table = null;
   }
   var h = $(document).height();
   $("#licenselist_list").height(h-160);

   $("#licenselist_list").show();

   for (let i = 0; i < data.length; i++)
   {
    const row = data[i];
    if ('key' in row)
    	row['fullcompname'] = getCompName(row['key']);
    delete row.key;
   }

  licenselist_table =$('#licenselist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollY: "30vh",
   scrollX: true,
   scroller: true,
   pageLength: -1,
   destroy: true,
   sDom: 't',
   data: data,
   "language": {
      "emptyTable": "No package details available."
    },
  "order": [[ 0, "asc" ], [1, "asc"]],
  "columns": [
       { "data": "packagename" },
       { "data": "packageversion"},
       { "data": "name" },
	   { "data": "fullcompname"},
	   { "data": "url" },
	   { "data": "score" }],
   "columnDefs": [
                 {
                  "targets": [ 0 ],
                 },
                 {
                  "targets": [ 1 ],
                 },
                 {
                  "targets": [ 2 ],
				  "className": "dt-nowrap",
		          "fnCreatedCell": function (nTd, sData, oData, iRow, iCol) {
						if (oData.url.length > 0 && oData.name.length > 0)
            	     	   $(nTd).html("<a href='"+oData.url+"' target=_blank >"+oData.name +"</a>");
            	     	else if (oData.name.length > 0)
            	     	   $(nTd).html(+oData.name);
                        else
                           $(nTd).html("No License");
                   }
                 },
                 {
                  "targets": [ 3 ],
                 },
                 {
                  "targets": [ 4 ],
                  "visible": false,
                 },
                 {
                  "targets": [ 5 ],
                   "width": "25px",
                 },
                ],
                'rowCallback': function(row, data, index){
					var score = parseFloat(data.score)

					 if (score < 0)
					   score = 0

					 base = round(10 - score,1);

				     var risklevel = "";
				     if (base >= 0 && base <= 3.9)
				      risklevel = "risklevel_low";
				     else if (base >= 4.0 && base <= 6.9)
				      risklevel = "risklevel_medium";
				     else if (base >= 7.0 && base <= 8.9)
				      risklevel = "risklevel_high";
				     else if (base >= 9.0)
				      risklevel = "risklevel_critical";

				     $(row).find('td:nth-child(5)').addClass("dt-center");
				     if (risklevel != '')
	         	  		$(row).find('td:nth-child(5)').addClass(risklevel);
            		}
   });

  if (objtype == "cv" || objtype == "co")
   licenselist_table.column(3).visible(false);
  else
   licenselist_table.column(3).visible(true);

 }

 function round(value, precision) {
    var multiplier = Math.pow(10, precision || 0);
    return Math.round(value * multiplier) / multiplier;
}

 function getCompIds4App(objid)
 {
    $.ajax({
        url: "/dmadminweb/API/compids?appid=" + objid,
        type: "GET",
        async: false,
        dataType: "json",
        success: function(data) {
            compid2Name = data.result;
        },
        error: function(xhr, status, error) {
        }
    });

    return Object.keys(compid2Name).join(',');
 }

/***** End License List  *****/

/***** Start Scorecard List ******/
function displayScorecardTable(data)
 {
   if (typeof scorecardlist_table != "undefined" && scorecardlist_table != null)
   {
    scorecardlist_table.clear();
    scorecardlist_table.destroy();
    scorecardlist_table = null;
   }
   var h = $(document).height();
   $("#scorecardlist_list").height(h-160);

   $("#scorecardlist_list").show();

  scorecardlist_table =$('#scorecardlist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  false,
   ordering: false,
   pageLength: -1,
   destroy: true,
   sDom: 't',
   data: data,
   "language": {
      "emptyTable": "No scorecard details available."
    },
  "columns": [
	   { "data": "risklevel" },
	   { "data": "score"},
       { "data": "check" }],
   "columnDefs": [
	             {
                  "targets": [ 0 ],
                  "visible": false,
                 },
                 {
                  "targets": [ 1 ],
                  "width": "15px",
                 },
                 {
                  "targets": [ 2 ],
                 }
                ],
                'rowCallback': function(row, data, index){

				     var risklevel = data.risklevel;

				     $(row).find('td:nth-child(1)').addClass("dt-center");
				     if (risklevel != '')
	         	  		$(row).find('td:nth-child(1)').addClass(risklevel);
            		}
   });
 }


/***** End Scorecard List ******/

/***** Start CVE List *****/

 function getCVEList(displaytype)
 {
  if (displaytype == "list")
  {
   var url = "";

	var data = {};

    $.ajax({
        url: "/msapi/sbomtype",
        type: "GET",
        async: false,
        dataType: "json",
        success: function(response) {
            data = response;
        },
        error: function(xhr, status, error) {
        }
    });

   console.log(data);

   if (data.SBOMType == "fullfile")
   {
    if (objtype == "ap" || objtype == "av")
      url = "/msapi/package?deptype=cve&appid=" + getCompIds4App(objid);
    else
      url = "/msapi/package?deptype=cve&compid=" + objid;

    var saveData = [];

    $.ajax({
        url: url,
        type: "GET",
        dataType: "json",
        async: false,
        success: function(response) {
            saveData = response.data;
        },
        error: function(xhr, status, error) {
        }
    });

	if (objtype == "ap" || objtype == "av")
	  url = "/msapi/deppkg?deptype=cve&appid=" + objid;
	else
	  url = "/msapi/deppkg?deptype=cve&compid=" + objid;

	$.ajax({
	    url: url,
	    type: "GET",
	    dataType: "json",
	    success: function(response) {
	        var data2 = response.data;
	        var rows =  saveData.concat(data2);
	        displayCVETable(rows);
	    },
	    error: function(xhr, status, error) {
	    }
	});
   }
   else
   {
    if (objtype == "ap" || objtype == "av")
      url = "/msapi/deppkg?deptype=cve&appid=" + objid;
    else
      url = "/msapi/deppkg?deptype=cve&compid=" + objid;

    $.ajax({
        url: url,
        type: "GET",
        dataType: "json",
        success: function(response) {
            rows = response.data;
            displayCVETable(rows);
        },
        error: function(xhr, status, error) {
        }
    });
   }
  }
 }


function displayCVETable(data)
{
   if (typeof cvelist_table != "undefined" && cvelist_table != null)
   {
    cvelist_table.clear();
    cvelist_table.destroy();
    cvelist_table = null;
   }
   var h = $(document).height();
   $("#cvelist_list").height(h-160);

   $("#cvelist_list").show();

   $.fn.dataTable.ext.type.order['risklevel-pre'] = function ( d ) {
     switch ( d ) {
        case 'Low':    return 1;
        case 'Medium': return 2;
        case 'High':   return 3;
        case 'Critical':   return 4;
    }
    return 0;
  };

  for (let i = 0; i < data.length; i++)
  {
   const row = data[i];
   if ('key' in row)
     row['fullcompname'] = getCompName(row['key']);
   delete row.key;
  }

  cvelist_table =$('#cvelist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollY: "30vh",
   scrollX: true,
   scroller: true,
   pageLength: -1,
   destroy: true,
   sDom: 't',
   data: data,
  "language": {
      "emptyTable": "Congratulations, no CVE issues found."
    },
  "order": [[ 1, "asc" ], [2, "asc"]],
  "initComplete": function(settings, json) {
        $("#cvelist tr.dtrg-start").hide();
  },
  "columns": [
	   { "data": "risklevel"},
       { "data": "packagename" },
       { "data": "packageversion" },
       { "data": "name" },
       { "data": "url" },
       { "data": "summary" },
	   { "data": "fullcompname" }
   ],
   "columnDefs": [
                 {
                  "targets": [ 0 ],
				  "width": "5px",
				  "type": "risklevel"
                 },
                 {
                  "targets": [ 1 ],
                 },
                 {
                  "targets": [ 2 ],
                 },
                 {
                  "targets": [ 3 ],
				  "className": "dt-nowrap",
		          "fnCreatedCell": function (nTd, sData, oData, iRow, iCol) {
            	     	$(nTd).html("<a href='https://osv.dev/vulnerability/"+oData.name+"' target=_blank >"+oData.name +"</a>");
                   }
                 },
                 {
                  "targets": [ 4 ],
				  "visible": false
                 },
                 {
                  "targets": [ 5 ],
                 },
                 {
                  "targets": [ 6 ],
                 }
                ],
            'rowCallback': function(row, data, index){
	             if (data.risklevel == "Critical" )
                 {
	              $(row).find('td:nth-child(1)').html("");
	         	  $(row).find('td:nth-child(1)').addClass('cvss_critical');
                 }
	             else if (data.risklevel == "High")
                 {
		          $(row).find('td:nth-child(1)').html("");
	         	  $(row).find('td:nth-child(1)').addClass('cvss_high');
                 }
	             else if (data.risklevel == "Medium")
                 {
		          $(row).find('td:nth-child(1)').html("");
	         	  $(row).find('td:nth-child(1)').addClass('cvss_medium');
                 }
	             else if (data.risklevel  == "Low")
                 {
		          $(row).find('td:nth-child(1)').html("");
	         	  $(row).find('td:nth-child(1)').addClass('cvss_low');
                 }
				 else
			        $(row).find('td:nth-child(1)').html("")
            }
   });

  if (objtype == "cv" || objtype == "co")
   cvelist_table.column(6).visible(false);
  else
   cvelist_table.column(6).visible(true);
}

/***** End CVE List  *****/

/***** Start AppComp List *****/

 function getAppCompList(displaytype)
 {
  if (displaytype == "list")
  {
   if (typeof appcomplist_table != "undefined" && appcomplist_table != null)
   {
    appcomplist_table.clear();
    appcomplist_table.destroy();
    appcomplist_table = null;
   }
   var h = $(document).height();
   $("#appcomplist_list").height(h-160);

   $("#appcomplist_list").show();

  appcomplist_table =$('#appcomplist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollY: "40vh",
   scroller: true,
   pageLength: -1,
   paging: false,
   destroy: true,
   sDom: 't',
   "ajax": {
    "url": "/dmadminweb/ReportsData?type=appcompList&appid=" + objid,
    "type": "GET",
    "async": false
  },
  "rowCallback": function (row, data) {
            $('td:eq(0)', row).html("<a href=\"javascript:void(0);\" onclick=\"jump2Comp('" + data.id + "','" + data.name + "')\">" + data.name + "</a>");
        },
  "columns": [
       { "data": "name" },
       { "data": "domain" },
       { "data": "id"}],
   "columnDefs": [
                 {
                  "targets": [ 0 ]
                 },
                 {
                  "targets": [ 1 ]
                 },
                 {
                  "targets": [ 2 ],
                  "visible": false
                 }
                ]
   });
  }
}

/***** End AppComp List  *****/

/***** Start Apps4Comp List *****/

 function getApps4Comp(displaytype)
 {
  if (displaytype == "list")
  {
   if (typeof apps4complist_table != "undefined" && apps4complist_table != null)
   {
    apps4complist_table.clear();
    apps4complist_table.destroy();
    apps4complist_table = null;
   }
   var h = $(document).height();
   $("#apps4complist_list").height(h-160);

   $("#apps4complist_list").show();

  apps4complist_table =$('#apps4complist').DataTable( {
   lengthMenu: [[15, 25, 50, -1], [15, 25, 50, "All"]],
   select:  true,
   ordering: true,
   scrollCollapse: true,
   scrollY: "40vh",
   scroller: true,
   pageLength: -1,
   paging: false,
   destroy: true,
   sDom: 't',
   "ajax": {
    "url": "/dmadminweb/API/apps4comp/" + objid,
    "type": "GET",
    "async": false
  },
  "rowCallback": function (row, data) {
            $('td:eq(0)', row).html("<a href=\"javascript:void(0);\" onclick=\"jump2App('" + data.id + "','" + data.name + "')\">" + data.name + "</a>");
  			$('td:eq(3)', row).html("<a href=\"javascript:void(0);\" onclick=\"DisplayDeploy(" + data.deployid + ")\">#" + data.deployid + "</a>");
  },
  "order": [[ 0, "desc" ],  [ 3, "desc" ]],
  "columns": [
       { "data": "name" },
       { "data": "domain" },
       { "data": "environment" },
       { "data": "deployid" },
       { "data": "finished" },
       { "data": "exitcode" },
       { "data": "id"}],
   "columnDefs": [
                 {
                  "targets": [ 0 ]
                 },
                 {
                  "targets": [ 1 ]
                 },
                 {
                  "targets": [ 2 ]
                 },
                 {
                  "targets": [ 3 ]
                 },
                 {
                  "targets": [ 4 ]
                 },
                 {
                  "targets": [ 5 ]
                 },
                 {
                  "targets": [ 6 ],
                  "visible": false
                 }
                ],
       });
  }
}

/***** End Apps4Comp List  *****/


 function HideSetup()
 {
  $("#verttab_credential").hide();
  $("#verttab_repository").hide();
  $("#verttab_datasource").hide();
  $("#verttab_notifier").hide();
  $("#verttab_template").hide();
  $("#verttab_action").hide();
  $("#verttab_release").hide();
  $("#verttab_procedure").hide();
  $("#verttab_servercomptype").hide();
 }

 function toggleSetup()
 {
  if ($("#verttab_credential").is(":visible"))
  {
   $("#verttab_setup > i").removeClass("fa-chevron-double-up");
   $("#verttab_setup > i").addClass("fa-chevron-double-down");
   $("#verttab_action").hide();
   $("#verttab_release").hide();
   $("#verttab_procedure").hide();
   $("#verttab_servercomptype").hide();
   $("#verttab_credential").hide();
   $("#verttab_repository").hide();
   $("#verttab_datasource").hide();
   $("#verttab_notifier").hide();
   $("#verttab_template").hide();
  }
  else
  {
   $("#verttab_setup > i").addClass("fa-chevron-double-up");
   $("#verttab_setup > i").removeClass("fa-chevron-double-down");
   $("#verttab_action").show();
   $("#verttab_release").show();
   $("#verttab_procedure").show();
   $("#verttab_servercomptype").show();
   $("#verttab_credential").show();
   $("#verttab_repository").show();
   $("#verttab_datasource").show();
   $("#verttab_notifier").show();
   $("#verttab_template").show();
  }
 }

 function isNumeric(num){
    num = "" + num; //coerce num to be a string
    return !isNaN(num) && !isNaN(parseFloat(num));
}

 $.fn.dataTable.ext.type.order['objname-pre'] = function(data, settings) {
    var parts = data.split(';')

    for (var k=0;k<parts.length;k++)
    {
     var version = parts[k].trim();

     if (isNumeric(version))
      parts[k] = 100000 - Number(version);
     else if (version.includes("_g"))
     {
      var schemantic_git = version.split("_g");
      var schemantic = schemantic_git[0].replace(/^v|V/, '');
      var verparts = schemantic.split('_')
      for (var i=0;i<verparts.length;i++)
       verparts[i] = 100000 - Number(verparts[i]);

      parts[k] = "v" + verparts.join('_') + schemantic_git[1]
     }
	 else if (version.includes("_"))
     {
      var verparts = version.split("_");
      for (var i=0;i<verparts.length;i++)
      {
       var verpart = verparts[i].replace(/^v|V/, '');
       verparts[i] = 100000 - Number(verpart);
      }
      parts[k] = "v" + verparts.join('_');
     }
    }

    var result = parts.join(";");
    return result;
};


function getCompName(id)
{
  var res = compid2Name[id];

  if (res)
    return res;
  return "";
}
