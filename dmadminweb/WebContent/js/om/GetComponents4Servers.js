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

function LoadComp4SrvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 var url = "";
 var servlet = "";
 
 servlet = "GetComponentLayout";
 url = "servid=" + objid;
 
 $.getJSON(servlet, url, function(res)
   {
  var td = "";
  var rowcnt = 0;

   data = res.data;
   
   for (a = 0; a < data.length; a++)
   {
    var row = data[a]; 
    var val = row[3].name;
    if (typeof val == "undefined") {
        val = "";
    } else {
	    if (typeof row[3].when != "undefined")
	    {
	     var d = convertDate(row[3].when);
	     val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
	     val += " by " + row[3].name;
	    } else {
	    	val = "<a onClick=\"DisplayDeploy(" + row[3].id + ");\">Deployment "+val+"</a>";
	    }
    }
    
    var val2 = row[2].name;
    
    if (typeof row[2].name == "undefined")
     val2 = "";
            
    if (val == "" && val2 == "")
     continue;
    console.log("row");
    console.log(row);
    
    var linkval1="<a href='javascript:SwitchDisplay(\""+row[1].type+row[1].id+"\");'>"+row[1].name+"</a>";
    var linkval2="<a href='javascript:SwitchDisplay(\""+row[2].type+row[2].id+"\");'>"+val2+"</a>";
    var linkval3=row[4]>0?"<a href='javascript:DisplayBuild("+row[2].id+","+row[4]+");'>#"+row[4]+"</a>":"&nbsp;";
    
    // td += "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[1].type + row[1].id  +"\">" + row[1].name + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + val2  + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
    td += "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[1].type + row[1].id  +"\">" + linkval1 + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + linkval2  + "</td><td style=\"border: 1px solid #ddd;\">"+linkval3+"</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
    rowcnt++;
   }
  
  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
  }

  $("#" + tablename + " > tbody").html(td);

  var $table = $("#" + tablename);
  $table.delegate("tr", "click", function()
    {
   $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
   $(this).addClass('ui-state-highlight');

   $("edit_button").button("option", "disabled", false);
   $("delete_button").button("option", "disabled", false);
    });

  $("add_button").button();
  $("delete_button").button(
    {
     disabled : true
    });

  $("edit_button").button(
    {
     disabled : true
    });
   });
}

function AddComp4SrvButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  SaveComp4SrvData(parent.$("#modal"),false);
                  parent.$("#modal").dialog("close");
                 }
                },
                {
                 text : "Cancel",
                 click : function()
                 {
                  parent.$("#modal").dialog("close");
                 }
                } ];

 var tdedit = "<form id=\"Comp4Srvform\"><table id=\"Comp4Srvtab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\"><I>Associate Component(s) with this server. This tells DeployHub which components should be placed on this server during a deployment.</I></td></tr>";
 tdedit += "<tr><td style=\"text-align:left;\"><B>Component:</B></td></tr>";
 tdedit += "<tr><td><div id=\"addcomp2srv\" name=\"addcomp2srv_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {
     CreateTree_Selection("#addcomp2srv","components",false,"mainframe3");           
    }
   });

     pwd.dialog("option", "title", "Add Component Association");
     pwd.dialog("option", "height", "auto");
     pwd.dialog("option", "width", "400");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }

function AddCompVersion4SrvButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  SaveCompVersion4SrvData(parent.$("#modal"),false);
                  parent.$("#modal").dialog("close");
                 }
                },
                {
                 text : "Cancel",
                 click : function()
                 {
                  parent.$("#modal").dialog("close");
                 }
                } ];

 var tdedit = "<form id=\"Comp4Srvform\"><table id=\"Comp4Srvtab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\"><I>Manually add Component Version(s) to this end point. Use this if the end point has had the component version installed outside of DeployHub.</td></tr>";
 tdedit += "<tr><td style=\"text-align:left;\"><B>Component Version:</B></td></tr>";
 tdedit += "<tr><td><div id=\"addcomp2srv\" name=\"addcomp2srv_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {
     CreateTree_Selection("#addcomp2srv","components",false,"mainframe3");         
    }
   });

     pwd.dialog("option", "title", "Manually Add Component Version(s)");
     pwd.dialog("option", "height", "400");
     pwd.dialog("option", "width", "400");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }


   function DeleteComp4SrvButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr) {       
     var key = $(this).find("td").eq(0).attr("id");
     key = key.substring(2);

     saveRes = $.ajax(
         {
          url: "UpdateAttrs?f=scd&s=" + summSaveobjid + "&c="+key,
          type: 'GET',
          dataType: "text"
         });
    });  
    
    saveRes.success(function(data){
     LoadComp4SrvData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);
    });
    
    saveRes.fail(function( jqXHR, textStatus ) {
     alert( "Request failed: " + textStatus );
   });
   }

   function SaveComp4SrvData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
   {
    var form = instance.find("#Comp4Srvform");
    
    var srvid = "";
    
    var srvlist = [];
    $('#addcomp2srv').jstree("get_checked",null,true).each(function(){
     if (this.id.indexOf("co") == 0)
        srvlist.push(this.id);
    });
    
    var len = srvlist.length;
    for (var i = 0; i < len; i++) 
    {
     if (srvlist[i].length == 0)
      continue;
     
     srvid = srvlist[i].substring(2);
     
     url = "UpdateAttrs?f=scvm&s="+ summSaveobjid + "&c=" + srvid + "&xpos=20&ypos=20";
     
     console.log(url);
     
     $.ajax(
      {
       url: url,
       type: 'GET',
       dataType: "text",
       async: false,
       success: function(data)
       {
       }
      });
    }
    LoadComp4SrvData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);

   }
   
   function SaveCompVersion4SrvData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
   {
    var form = instance.find("#Comp4Srvform");
    var srvid = "";
    
    var srvlist = [];
    $('#addcomp2srv').jstree("get_checked",null,true).each(function(){
     if (this.id.indexOf("cv") == 0)
        srvlist.push(this.id);
    });
    
    if (srvlist.length == 0)
    {
     $('#addcomp2srv').jstree("get_checked",null,true).each(function(){
      if (this.id.indexOf("co") == 0)
        srvlist.push(this.id);
     });
    } 
    
    var len = srvlist.length;
    for (var i = 0; i < len; i++) 
    {
     if (srvlist[i].length == 0)
      continue;
     
     srvid = srvlist[i].substring(2);
     
     url = "UpdateAttrs?f=scam&s="+ summSaveobjid + "&c=" + srvid + "&xpos=20&ypos=20";
     
     console.log(url);
     
     $.ajax(
      {
       url: url,
       type: 'GET',
       dataType: "text",
       async: false,
       success: function(data)
       {
       }
      });
    }
    LoadComp4SrvData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);

   }
