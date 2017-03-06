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

function LoadSrv4EnvData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 $.getJSON('GetServerLayout', "envid=" + objid, function(data)
   {
  var td = "";
  var rowcnt = 0;

  for (a = 0; a < data.NodeCount; a++)
  {
   var linkval="<a href='javascript:SwitchDisplay(\"se"+data.Nodes[a].nodeid+"\");'>"+data.Nodes[a].name+"</a>";
   td += "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\srv" + data.Nodes[a].nodeid  +">" + linkval + "</td><td style=\"border: 1px solid #ddd;\">" + data.Nodes[a].hostname + "</td><td style=\"border: 1px solid #ddd;\">" + data.Nodes[a].summary + "</td></tr>";
   rowcnt++;
  }

  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
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

function AddSrv4EnvButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  SaveSrv4EnvData(parent.$("#modal"),false);
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

 var tdedit = "<form id=\"srv4envform\"><table id=\"srv4envtab\" class=\"summtable\" style=\"width:100%\">";
 tdedit += "<tr><td style=\"text-align:left;\"><B>Sever:</B></td></tr>";
 tdedit += "<tr><td><div id=\"addsrv2env\" name=\"addenv2app_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {
     CreateTree_Selection("#addsrv2env","servers",false,"mainframe3");        
    }
   });

     pwd.dialog("option", "title", "Add End Point");
     pwd.dialog("option", "height", "auto");
     pwd.dialog("option", "width", "500");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }



   function DeleteSrv4EnvButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr) {       
     var key = $(this).find("td").eq(0).attr("id");
     key = key.substring(3);

     saveRes = $.ajax(
         {
          url: "UpdateAttrs?f=ds&e=" + summSaveobjid + "&w="+key,
          type: 'GET',
          dataType: "text"
         });
    });  
    
    saveRes.success(function(data){
     LoadSrv4EnvData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);
    });
    
    saveRes.fail(function( jqXHR, textStatus ) {
     alert( "Request failed: " + textStatus );
   });
   }

   function SaveSrv4EnvData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
   {
    var form = instance.find("#srv4envform");
    var srvid = "";
    
    var srvlist = [];
    $('#addsrv2env').jstree("get_checked",null,true).each(function(){
     if (this.id.indexOf("se") == 0)
        srvlist.push(this.id);
    });
    
    var len = srvlist.length;
    var sl="";
    var sep="";
    for (var i = 0; i < len; i++) 
    {
     if (srvlist[i].length == 0)
      continue;
     
     srvid = srvlist[i].substring(2);
     sl=sl+sep+srvid;
     sep="x";
     console.log("UpdateAttrs?f=sm&e="+ summSaveobjid + "&s=" + srvid + "&xpos=20&ypos=20");
     
     $.ajax(
      {
       url: "UpdateAttrs?f=sm&e="+ summSaveobjid + "&s=" + srvid + "&xpos=20&ypos=20",
       type: 'GET',
       dataType: "text",
       async: false,
       success: function(data)
       {
       }
      }); 
    }
    console.log("Calling UpdateAttrs f=asc, sl="+sl);
    $.ajax(
     {
      url: "UpdateAttrs?f=asc&e="+ summSaveobjid + "&sl=" + sl,
      type: 'GET',
      dataType: "text",
      async: false,
      success: function(data)
      {
      }
     });
    DoRefresh();
    LoadSrv4EnvData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);

   }
