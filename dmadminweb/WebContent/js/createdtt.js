/**
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

function GetDomainData(currentdomain)
{
	var res=null;
	var seldom = (currentdomain!='') ? ('&seldom='+currentdomain) : '';
	$.ajax({
		  dataType: "json",
		  url: "GetDomainContent?typestr=dmtarget&domainid=0"+seldom,
		  async: false
		}).done(function( data ) {
				console.log("data="+data);
				res=data;
		});
	return res;
}


function dtCreateTree(nodename,currentdomain)
{
	var BaseNode=0;
	var Loading=1;
    var $nodename = $(nodename);
    $nodename.bind(
    	"select_node.jstree", function(evt, data){
    		console.log("Tree -> Select Node");
    		dtSelectNode(data,Loading);
    		Loading=0;
    	}).bind(
    		"loaded.jstree", function () {
    		console.log("tree loaded");
    		if(currentdomain != "") {
    			$nodename.jstree("select_node", "#do"+currentdomain); // .trigger("select_node.jstree");
    		}
    		Loading=0;
    	}).jstree({
    		"json_data" : {
    	  		 "data" : GetDomainData(currentdomain),	// Initial data, including opened nodes to the selected target domain
                 "ajax" : {
                	 "url":	function(node) {
                		 return "GetDomainContent?typestr=dmtarget&domainid="+node.attr("id").substr(2);
                	 }
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
              // The `domain` node 
              "Domain" : {
                  "icon" : {
                      "image" : "css/images/domains_sm.png"
                  }
              },
              "Lifecycle" : {
                  "icon" : {
                      "image" : "css/images/lifecycles_sm.png"
                  }
              }
          }
      },
      "core" : {"html_titles" : true,  "load_open" : true, "animation": 50 },
      "plugins" : [ "themes", "json_data", "ui", "crrm", "types" ],
      "ui": {"select_limit" : 1}
      });
 
    var $jstree = $.jstree;
}
	