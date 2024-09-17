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

function _getFeedTime(timestamp)
{
 var currentTime = ~~(new Date().getTime() / 1000);
 var ago = currentTime - timestamp;
 if (ago < 60)
 {
  return " less than a minute ago";
 }
 else if (ago < 60 * 60)
 {
  return ~~(ago / 60) + " minutes ago";
 }
 else if (ago < 2 * 60 * 60)
 {
  return " about an hour ago";
 }
 else if (ago < 24 * 60 * 60)
 {
  return ~~(ago / (60 * 60)) + " hours ago";
 }
 else if (ago < 2 * 24 * 60 * 60)
 {
  return " a day ago";
 }
 else
 {
  return ~~(ago / (24 * 60 * 60)) + " days ago";
 }
}

function _getFileSize(size)
{
 if (size < 1024)
 {
  return size + '&nbsp;bytes';
 }
 else if (size < 1024 * 1204)
 {
  return ~~(size / 1024) + 'KB';
 }
 else
 {
  return ~~(size / (1024 * 1024)) + 'MB';
 }
}

var newsid = 0;

function addToHistoryFeed($feed, item, otid, prepend, $after)
{

 var icon = item.icon ? item.icon : 'user';
 var text = '';
 var text2 = '';
 var time = _getFeedTime(item.when);
 var cmnt = (item.cmnt == 0) ? 'Comment' : (item.cmnt + ((item.cmnt == 1) ? ' Comment ' : ' Comments '));
 // 9650 is up arrow; 9660 is down arrow
 var user = '<b>' + item.user.name + '</b>';

 var dbid = (item.kind=='bu')?"bu"+otid.substr(2)+"-"+item.id:item.kind + item.id;
 console.log("dbid="+dbid);

 var subsid = '', subs = '', cmntlink = '', attach = '', attachpicker = '', tasks = '';

 if ((item.kind == 'de'))
 {
  if (item.eventid.length > 0)
  {
   text = "Automatic ";
   user = "Scheduler (running as " + user + ")";
  }
  else
  {
   text = " ";
  }
  if (item.id > 0) {
	  text = "Deployment <a id=\"deployment" + item.id + "\" onClick=\"DisplayDeploy(" + item.id + ");\">#" + item.id + '</a>';
	  if (item.app && item.app.name.length > 0)
	  {
		  var linkval = "<a href='javascript:SwitchDisplay(\""+item.app.type+item.app.id+"\");'>"+item.app.name+"</a>";
		  if (item.app.type == "av") {
			  text += ' of Application Version <b>"' + linkval + '"</b>';
		  } else
		  if (item.app.type == "rl") {
			  text += ' of Release <b>"' + linkval + '"</b>';
		  } else {
			  text += ' of Application <b>"' + linkval + '"</b>';
		  }
	  }
	  if (item.env && item.env.name.length > 0)
	  {
		  console.log("item.env");
		  console.log(item.env);
		  var linkval = "<a href='javascript:SwitchDisplay(\""+item.env.type+item.env.id+"\");'>"+item.env.name+"</a>";
		  text += ' into environment <b>"' + linkval + '"</b>';
	  }
  } else {
	  text = "Manually added ";
	  if (item.app && item.app.name.length > 0)
	  {
		  var linkval = "<a href='javascript:SwitchDisplay(\""+item.app.type+item.app.id+"\");'>"+item.app.name+"</a>";
		  text += ' application <b>"' + linkval + '"</b>';
	  }
	  if (item.env && item.env.name.length > 0)
	  {
		  var linkval = "<a href='javascript:SwitchDisplay(\""+item.env.type+item.env.id+"\");'>"+item.env.name+"</a>";
		  text += ' to environment <b>"' + linkval + '"</b>';
	  }
  }
  subsid = dbid;
 }
 else if ((item.kind == 'hn') || (item.kind == 'no'))
 {
  if (item.obj)
  {
   var linkval = "<a href='javascript:SwitchDisplay(\""+item.obj.type+item.obj.id+"\");'>"+item.obj.name+"</a>";
   switch (item.obj.type)
   {
   case 'ap':
    text = 'Application <b>' + linkval + '</b>: ';
    break;
   case 'av':
    text = 'Application Version <b>' + linkval + '</b>: ';
    break;
   case 'co':
	text = 'Component <b>' + linkval + '</b>: ';
	break;
   case 'cv':
	text = 'Component Version <b>' + linkval + '</b>: ';
	break;
   case 'en':
    text = 'Environment <b>' + linkval + '</b>: ';
    break;
   case 're':
    text = 'Repository <b>' + item.obj.name + '</b>: ';
    break;
   case 'se':
    text = 'Server <b>' + linkval + '</b>: ';
    break;
   case 'us':
    text = 'User <b>' + item.obj.name + '</b>: ';
    break;
   }
   subsid = item.obj.type + item.obj.id;
  }
  else
  {
   subsid = otid;
  }
  text += item.text;
 }
 else if (item.kind == 'ae')
 {
  text = 'Application ';
  if (item.app)
  {
	  var linkval = "<a href='javascript:SwitchDisplay(\""+item.app.type+item.app.id+"\");'>"+item.app.name+"</a>";
   text += '<b>' + linkval + '</b>';
  }
  text += ' has been ' + item.icon;
  if (item.dom)
  {
   text += ' for domain <b>' + item.dom.name + '</b>';
  }
  if (item.text)
  {
   text2 += item.text+"<BR>";
  }
 }
 else if (item.kind == 'rq')
 {
  console.log("item.kind=rq");
  if (item.obj)
  {
   console.log("item.obj.type=" + item.obj.type);
   switch (item.obj.type)
   {
   case 'ta':
    text = 'Request for <b>' + item.obj.name + '</b>';
    if (item.app)
    {
     text += ' on Application <b><a href=\'javascript:SwitchDisplay(\"'+item.app.type+item.app.id+'\");\'>' + item.app.name + '</a></b>';
    }
    // text += '<br>';
    text2 = (item.text && item.text.length > 0)?item.text+"<BR>":"";
    break;
   case 'ce':
    text = 'Calendar entry <b><a href=\'javascript:SwitchToCalendar('+item.obj.id+');\'>' + item.obj.name + '</a></b> for environment ' + item.env.name + ' needs approval';
    // text += item.text;
    text2 = (item.text && item.text.length > 0)?item.text+"<BR>":"";
    break;
   }
  }
  // text += item.text;
 }
 else if (item.kind == 'bu')
 {
	 var success = (item.icon=="blue");
	 text = "Build <a id=\"build"+item.buildjobid+"_"+item.id+"\" onClick=\"DisplayBuild("+item.buildjobid+","+item.id+","+success+");\">#"+item.id+"</a> ";
	 if (success) text+=" Successful";
	 else text+=" Failed";
 }
 else
 {
  text = 'undefined note type "' + item.kind + '"';
 }

 newsid++;

 if (item.kind == 'bu')
 {
	 cmntlink = '<a href="#" id="newscomment_' + newsid + ':' + dbid + '" class="newscomment">' + cmnt + '</a> | ';
 } else if (item.kind != 'no')
 {
	 // cmntlink = "<br>" + cmnt;
	 cmntlink = '<a href="#" id="newscomment_' + newsid + ':' + dbid + '" class="newscomment">' + cmnt + '</a> | ';
 }

 if (item.attach > 0)
 {
	 attach = '<img id="attach_' + newsid + '" class="attachpicker-follows" src="images/attachment.png" style="margin-top:5px;opacity:0.5;">' + '<div class="attachpicker"></div>';
 }
 if (item.tasks > 0)
 {
	 tasks = '<img id="tasks_' + newsid + '" src="images/task.png" style="margin-top:-14px;float:right;opacity:0.5;">';
 }

 var tr = '<tr><td class=\"iconcolumn\"><img src="css/images/' + icon + '_24x.png"></td><td>' + text + " - "  + time + ' by ' + user + '<BR>'+text2+'<div id="news_' + newsid + '">'+subs + cmntlink + attach + tasks + '</div></td></tr>';

 console.log(tr);
 if (prepend)
 {
  $feed.prepend(tr);
 }
 else
 {
  $after.after(tr);
 }

 if (item.attach > 0)
 {
  $('#attach_' + newsid).button().hover(function()
  {
   $(this).css('opacity', 1.0);
  }).mouseleave(function()
  {
   $(this).css('opacity', 0.5);
  }).click(
    function()
    {
     var $attachButton = $(this);
     var $attachPicker = $attachButton.next('.attachpicker');
     console.log($attachPicker.data('fadein'));
     var fi = $attachPicker.data('fadein');
     if (typeof fi === 'undefined') fi = false;
     if (!fi) {
	     $.ajax(
	     {
	      url : 'NewsFeed?reason=attachments&otid=' + dbid,
	      dataType : 'json',
	      type : 'GET',
	      success : function(res)
	      {
	       console.log(res);
	       var content = '';
	       try
	       {
	        for (var n = 0; n < res.data.length; n++)
	        {
	         var icon = res.data[n].icon ? ('filetypes/' + res.data[n].icon + '.png') : 'audititem.gif';
	         if (content != '')
	         {
	          content += ' ';
	         }
	         content += '<span class="file-attachment"><img style="vertical-align:-20%;" src="images/' + icon + '">&nbsp;' + '<a href="GetAttachment?attachid=' + res.data[n].id + '">'
	           + res.data[n].filename + '</a>' + '&nbsp(' + _getFileSize(res.data[n].size) + ')</span>&nbsp;&nbsp;';
	        }
	       }
	       catch (e)
	       {
	        alert('Exception: ' + e);
	       }
	       $attachPicker.html(content).fadeIn().position(
	       {
	        at : 'right+16 bottom',
	        of : $attachButton,
	        my : 'right top+15'
	       });
	       $attachPicker.data('fadein', true);
       //$('.attachpicker .file-attachment').click(function()
       //{
    //   alert('Fade Out!');
     //   $attachPicker.fadeOut();
       //});
      }
     });
     } else {
    	 $attachPicker.fadeOut();
    	 $attachPicker.data('fadein', false);
     }
    });
 }
 if (item.tasks > 0)
 {
  $('#tasks_' + newsid).button().hover(function()
  {
   $(this).css('opacity', 1.0);
  }).mouseleave(function()
  {
   $(this).css('opacity', 0.5);
  });
 }
}

function addMoreHistoryButton($feed, otid, since, isPending)
{
 if (isPending)
  return;

 $feed.append("<tr id=\"morenews\"><td colspan=\"2\"><a id=\"morenewsbtn\">Click to see ealier items</a></td></tr>");

 $feed.find('#morenews #morenewsbtn').bind('click', function()
 {
  console.log('otid = ' + otid + '; to = ' + since + '; from = ' + ((parseInt(since) - 31 * 24 * 60 * 60)));

  var pending = /* (feed.indexOf('pending') != -1) */(isPending === true) ? '&pending=1' : '';
  $.ajax(
  {
   url : 'NewsFeed?otid=' + otid + '&from=' + (parseInt(since) - 31 * 24 * 60 * 60) + '&to=' + since + pending,
   dataType : 'json',
   type : 'GET',
   success : function(res)
   {
    // console.log(res);
    try
    {
     var oldest = 0;
     for (var r = 0; r < res.data.length; r++)
     {
      if (oldest == 0)
      {
       oldest = res.data[r].when;
      }
      addToHistoryFeed($feed, res.data[r], otid, false, $feed.find('#morenews'));
     }
     $feed.find('#morenews').remove();
     if (oldest != 0)
     {
      addMoreHistoryButton($feed, otid, oldest, isPending);
     }
    }
    catch (e)
    {
     alert('Exception: ' + e);
    }
    // addNewsCallbacks($feed);
   }
  });
 });
}

function addComment(newsid, cmnt)
{
 // var user = '<a class="tooltip-follows" href="UserDetails?uid=' + cmnt.user.id + '"><b>' + cmnt.user.name + '</b></a>';
 user = cmnt.user.name;
 //if (cmnt.user.tooltip)
 //{
  //user += '<div class="tooltip"><div class="tooltip-text">' + cmnt.user.tooltip + '</div><div class="tooltip-arrow">&#9660;</div></div>';
 //}
 $('#comments_' + newsid).slideDown('slow').append(
   '<div class="comment-content" style="padding:0px;">' + '<div><table border=0><tr><td valign="top"><img src="css/images/user_24x.png"></td><td>' + cmnt.text + '<br><I>' +  _getFeedTime(cmnt.when) + ' by <B>' + user + '</B></I></td></tr></table></div></div>');
}

function addNewsCallbacks($feed)
{
 // use event delegation and attach the handler once
 $feed.off('click', '.newscomment');

 $feed.on('click', '.newscomment', function(event)
 {
  event.stopPropagation();
  var $newscomment = $(this);
  var ID = $newscomment.attr('id');
  var sid = ID.split('newscomment_');
  var ids = sid[1].split(':');
  var newsid = ids[0];
  var dbid = ids[1];
  console.log("addNewsCallbacks dbid="+dbid);
  var expanded = $(this).attr('expanded');
  console.log('news id = ' + newsid+" expanded="+expanded);

  if (expanded == 1)
  {
   $feed.find('#comments_' + newsid).slideUp('slow').remove();
   $(this).attr('expanded', '0');
  }
  else if (expanded == 2)
  {
   console.log("Expand in progress");
  }
  else
  {
   $(this).attr('expanded', '2');

   $feed.find('#news_' + newsid).slideDown('slow').append(
     '<div id="comments_' + newsid + '" style="margin-left:16px;margin-right:16px;margin-top:4px">'
       + ' <div style="width:90%">' + '  <textarea id="cmnttxt_' + newsid
          + '" style="width:90%;height:25px;color:#777;">Say something?</textarea>'
       + '</div>'
          + '<div style="height: 42px;"><button disabled id="cmntbut_' + newsid
             + '" style="float: left">Comment</button>'
          + '</div>'
   + '</div>');

   $feed.find('#cmntbut_' + newsid).button().click(function()
   {
    var messageText = $('#cmnttxt_' + newsid)[0];
    if (messageText.value === messageText.defaultValue)
    {
     alert("Please type something in the box");
     return;
    }
    console.log("Adding comment, otid="+dbid);
    $.ajax(
    {
     url : 'NewsFeed?reason=addcmnt&otid=' + dbid + '&text=' + messageText.value,
     dataType : 'json',
     type : 'GET',
     async : false,
     success : function(res)
     {
      if (!res.result)
      {
       if (res.error)
       {
        alert("Comment failed to post: " + res.error);
       }
       else
       {
        alert("Comment failed to post");
       }
       return;
      }
      addComment(newsid, res.data);
      var numcmnts = $feed.find('#comments_' + newsid + ' .comment-content').length;
      $newscomment.html(numcmnts + ((numcmnts == 1) ? ' Comment' : ' Comments'));
      $feed.find('#cmntbut_' + newsid).button("disable");
      messageText.value = messageText.defaultValue;
      messageText.style.color = '#777';
     }
    });
   });

   $feed.find('#cmnttxt_' + newsid).keypress(function()
   {
    $feed.find('#cmntbut_' + newsid).button(((this.value === this.defaultValue) || (this.value === '')) ? "disable" : "enable");
   }).focus(function()
   {
    if (this.value === this.defaultValue)
    {
     this.value = '';
     this.style.color = '#000';
    }
   }).blur(function()
   {
    if (this.value === '')
    {
     this.value = this.defaultValue;
     this.style.color = '#777';
    }
   });

   $.ajax(
   {
    url : 'NewsFeed?reason=comments&otid=' + dbid,
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     if (!res.result)
     {
      console.log(JSON.stringify(res));
      if (res.error)
      {
       alert("Unable to retrieve comments: " + res.error);
      }
      else
      {
       alert("Unable to retrieve comments");
      }
      return;
     }
     console.log('comments for ' + dbid + ' are ' + JSON.stringify(res));
     try
     {
      for (var r = 0; r < res.data.length; r++)
      {
       addComment(newsid, res.data[r]);
      }
     }
     catch (e)
     {
      console.log('Exception: ' + e);
     }
    }
   });

   $(this).attr('expanded', '1');
  }
 });


 $feed.on('mouseenter', '.tooltip-follows', function()
 {
  $(this).next('.tooltip').fadeIn();
  $(this).next('.tooltip').position(
  {
   at : 'bottom center',
   of : $(this),
   my : 'top-32'
  });
 }).on('mouseleave', '.tooltip-follows', function()
 {
  $('.tooltip').fadeOut();
 });
}

function _updateAttachments()
{
 var fileInput = document.getElementById('messageAttach');
 $('#messageAttachments').html('');
 var files = fileInput.files;
 for (var n = 0; n < files.length; n++)
 {
  $('#messageAttachments').append(
    '<span class="file-attachment"><img style="vertical-align:-25%;" src="images/audititem.gif">&nbsp;' + files[n].name + '&nbsp(' + _getFileSize(files[n].size) + ')</span>&nbsp;&nbsp;');
 }
 $('#messageAttach').focus(null).hide();
};

$.fn.createNewsMessageBox = function createNewsMessageBox(/* boxid, */feedid, text, otid)
{
	$(this).css("class", "ui-widget-content").prepend(
   '<form id="messageForm">'
	 + '<div id="messageOuter" style="height:70px;width:100%;overflow:d;">'
       + '<div style="background-color:white;border:gray solid 1px;">'
         + '<textarea id="messageText" name="text" style="width:98%;height:100%;color:#777;border:none;">' + text + '</textarea>'
       + '</div>'
       + '<div style="height:26px;padding-top:2px;width:100%;">'
         + '<div id="messageProgress" style="float:left;display:none;">'
           + '<div class="progress">'
             + '<div class="progress-bar"></div>'
             + '<div class="progress-percent">0%</div>'
           + '</div>'
         + '</div>'
         + '<div style="float:left; height:30px;width:100%;overflow:auto;">'
            + '<div style="float:left;height:30px;">'
               + '<button id="addMessage" disabled>Add Message</button>'
               + '<img id="messageAttachButton" src="images/attachment.png" style="opacity:1;padding:4px;margin-right:2px;">'
            + '</div>'
            + '<div id="messageAttachments" style="float:left;height:100%;overflow:hidden;"></div>'
         + '</div>'
       + '</div>'
       + '<input id="messageAttach" name="attachments[]" type="file" style="height:100%;width:100%;position:relative;top:-80px;left:0px;opacity:0.1;filter:alpha(opacity:0.1);display:none;" multiple>'
     + '</div></form>');


 $('.iconpicker-follows').click(function()
 {
  var $messageIcon = $(this);
  var iconPicker = $messageIcon.next('.iconpicker');
  var icons = [ 'user', 'approved', 'rejected' ];
  var selected = $messageIcon.attr('src');
  var content = '';
  for (var n = 0; n < icons.length; n++)
  {
   var src = 'images/' + icons[n] + '-med.png';
   if (content != '')
   {
    content += ' ';
   }
   content += '<img class="iconSelectImage' + ((src === selected) ? ' iconSelectImage-selected' : '') + '" src="' + src + '" data-value="' + icons[n] + '">';
  }
  iconPicker.html(content).fadeIn().position(
  {
   at : 'bottom center',
   of : $(this),
   my : 'top'
  });
  $('.iconSelectImage').click(function()
  {
   $messageIcon.attr('src', $(this).attr('src'));
   $messageIcon.data('value', $(this).data('value'));
   iconPicker.fadeOut();
  });
 });

 $('#messageForm').ajaxForm();

 $('#addMessage').button().click(function()
 {
  $('#addMessage').button("disable");
  var messageText = $('#messageText')[0];
  if (messageText.value === messageText.defaultValue)
  {
   alert("Please type something in the box");
   return;
  }
  var messageIcon = $('#messageIcon').data('value');
  if (messageIcon === 'user')
  {
   messageIcon = null;
  }
  var icon = (messageIcon != null) ? ('&icon=' + messageIcon) : '';
  $('#messageAttach').show();

  var $bar = $('.progress-bar');
  var $percent = $('.progress-percent');
  var hasFileAttachments = ($('#messageAttachments .file-attachment').length > 0);

  $('#messageForm').ajaxSubmit(
  {
   url : 'NewsFeed',
   data :
   {
    reason : "add",
    otid : otid,
    icon : messageIcon
   },
   dataType : 'json',
   type : 'POST',
   async : true,
   clearForm : true,
   beforeSend : function()
   {
    $('#messageAttach').hide();
    if (hasFileAttachments)
    {
     $('#messageAttachments').hide();
     $('#messageProgress').show();
     var percentVal = '0%';
     $bar.width(percentVal);
     $percent.html(percentVal);
    }
   },
   uploadProgress : function(event, position, total, percentComplete)
   {
    if (hasFileAttachments)
    {
     var percentVal = percentComplete + '%';
     $bar.width(percentVal);
     $percent.html(percentVal);
    }
   },
   success : function(res)
   {
    // var percentVal = '100%';
    // $bar.width(percentVal);
    // $percent.html(percentVal);
    $('#messageProgress').hide();
    $('#messageAttachments').html('');
    $('#messageAttachments').show();
    if (!res.result)
    {
     console.log(JSON.stringify(res));
     if (res.error)
     {
      alert("Message failed to post: " + res.error);
     }
     else
     {
      alert("Message failed to post");
     }
     return;
    }

    var tab = feedid.split('-')[0];
    tab = tab.substring(1);

    createNewsFeed(tab, objtype, objid, false);

//    addToHistoryFeed($(feedid), res.data, otid, true);
//    messageText.value = messageText.defaultValue;
//    messageText.style.color = '#777';
   }
  });
 });

 $('#messageAttach').hide().change(function(e)
 {
  console.log("messageAttach - change event");
  _updateAttachments();
 });

 $('#messageAttachButton').button().hover(function()
 {
  $(this).css('opacity', 1.0);
 }).mouseleave(function()
 {
  $(this).css('opacity', 0.5);
 }).click(function()
 {
  $('#messageAttach').show().focus().focus(function(e)
  {
   console.log("messageAttach - focus event");
   _updateAttachments();
   $('#messageText').focus();
  }).click();
 });

 $('#messageText').keypress(function()
 {
  $('#addMessage').button(((this.value === this.defaultValue) || (this.value === '')) ? "disable" : "enable");
 }).focus(function()
 {
  if (this.value === this.defaultValue)
  {
   this.value = '';
   this.style.color = '#000';
  }
 }).blur(function()
 {
  if (this.value === '')
  {
   this.value = this.defaultValue;
   this.style.color = '#777';
  }
 });

 // var textarea = document.getElementById('messageOuter');
 var textarea=$('#messageOuter');
 // console.log(textarea);
 // textarea.ondrop = function(e) {
 // e.preventDefault();
 // $('#messageOuter').css('background-color','white');
 // $('#messageText').css('background-color','white');
 // //var $attachBox = $('#messageAttachments');
 // //for(var i = 0; i < e.dataTransfer.files.length; i++) {
 // // $attachBox.append('<span class="messageAttachment"><img
 // style="vertical-align:-25%;" src="images/audititem.gif">&nbsp;' +
 // e.dataTransfer.files[i].name + '</span>&nbsp;&nbsp;');
 // // $attachBox.last('.messageAttachment').data('file',
 // e.dataTransfer.files[i]);
 // //}
 // fileInput.ondrop(e);
 // };
 textarea.ondragover = function(e)
 {
  // e.preventDefault();
  // $('#messageOuter').css('background-color','yellow');
  // $('#messageText').css('background-color','yellow');
  $('#messageAttach').show();
 };
 textarea.ondragleave = function(e)
 {
  // $('#messageOuter').css('background-color','white');
  // $('#messageText').css('background-color','white');
 };
 return this;
};

function createNewsFeed(tablename, objtype, objid, isPending)
{
 $('#' + tablename + '-historyadd').empty();
 $('#' + tablename + '-historyadd').createNewsMessageBox('#' + tablename + '-histfeed', 'Say something about this '+objname+'?', objtype+objid);

 var pending = /* (id.indexOf('pending') != -1) */(isPending === true) ? '&pending=1' : '';
 var $feed = $("#" + tablename + " > tbody");
 $feed.empty();

 console.log('NewsFeed?otid=' + objtype + objid + pending);
 $.ajax(
 {
  url : 'NewsFeed?otid=' + objtype + objid + pending,
  dataType : 'json',
  type : 'GET',
  success : function(res)
  {
   console.log(res);
   if (res == null || res.data == null)
    return;

   try
   {
    var oldest = 0;
    for (var r = 0; r < res.data.length; r++)
    {
     if (oldest == 0)
     {
      oldest = res.data[r].when;
     }
     addToHistoryFeed($feed, res.data[r], objtype + objid, true);
    }
    addMoreHistoryButton($feed, objtype + objid, oldest, isPending);
   }
   catch (e)
   {
    alert('Exception: ' + e);
   }
   addNewsCallbacks($feed);
  }
 });
 return this;
};

function DisplayDeploy(lognum)
{
	saveLognum = lognum;
	var save_objtypeAsInt = objtypeAsInt;

	var h = "<div class=\"tabrow_tabmenu\" id=\"displaylogtabs\">" +
	"<div class=\"moduletable_tabmenu\">" +
	"<ul class=\"menu_tabmenu\">" +
 "<li id=\"displaylogtabs-log\" onclick=\"SetTabActive(1,this.id)\">Log</li>" +
	"<li id=\"displaylogtabs-files\" onclick=\"SetTabActive(1,this.id)\">Files</li>" +
    "<li id=\"displaylogtabs-reports\" onclick=\"SetTabActive(1,this.id)\">Step Duration</li>" +
    "<li id=\"displaylogtabs-deploydeps\" onclick=\"SetTabActive(1,this.id)\">Feedback Loop</li>" +
    "</ul>" +
    "</div>" +
    "<div class=\"right_panel_title_area\" style=\"display:inline-block;\">" +
    "<h1 class=\"build\">Deploy #"+lognum +
    "<div id=\"deprefresh\" style=\"float:right;display:none;\"><input id=\"autorefresh\" type=\"checkbox\" checked onClick=\"javascript:TogglePlayPause(summSaveobjid)\">Auto Refresh&nbsp;&nbsp;&nbsp;</div>"+
 //   "<button id=\"deprefresh\" class=\"edit_button\" style=\"float:right;display:none;\" onClick=\"javascript:TogglePlayPause(summSaveobjid)\"><img alt=\"Refresh\" src=\"images/play.png\"></button>" +
 //    "<button id=\"depmaximize\" class=\"edit_button\" style=\"float:right;\" onClick=\"javascript:MaxLogDetail(summSaveobjid)\"><img alt=\"Maximize\" src=\"images/maximize.png\"></button>" +
 //   "<button id=\"depminimize\" class=\"edit_button\" style=\"float:right;\" onClick=\"javascript:MinLogDetail(summSaveobjid)\"><img alt=\"Minimize\" src=\"images/minimize.png\"></button>" +
    "</h1>" +
    "</div>" +
    "<div id=\"displaylogtabs-general-data\" style=\"height:400px; display:none\"><table id=\"newssum\" class=\"dev_table\"><tbody></tbody></table></div>" +
    "<div id=\"displaylogtabs-log-data\" style=\"height:400px; overflow:auto;display:none\"><table id=\"deploymentlog\"></table></div>" +
    "<div id=\"displaylogtabs-files-data\" style=\"height:400px; display:none;padding-left:0px;padding-right:0px\"></div>" +
    "<div id=\"displaylogtabs-scripts-data\" style=\"height:400px; display:none;padding-left:0px;padding-right:0px\"></div>" +
    "<div id=\"displaylogtabs-defects-data\" style=\"height:400px; display:none;padding-left:0px;padding-right:0px\"></div>" +
    "<div id=\"displaylogtabs-deploydeps-data\" style=\"height:400px; width:100%; display:none\"></div>" +
    "<div id=\"displaylogtabs-reports-data\" style=\"height:400px; display:none\">" +
     "<div id=\"plot3resizable\" style=\"float:left;height:400px;width:400px\">" +
      "<div id=\"deploymenttimebystep\" style=\"height:96%;width:96%;\"></div>" +
     "</div>" +
    "</div>" +
	"</div>";




  var pwd = parent.$("#displaylog");
  pwd.html(h);

  //pwd.bind('DOMSubtreeModified',function() {
//	 alert("resize");
 // });


  pwd.dialog(
    {
     modal : true,
     open : function(event, ui)
     {

      GetNewsDetails(lognum);


      $(this).css("height", "400px").find(".right_panel_title_area").css({cursor: 'move'});
      // $("#displaylogtabs").tabs({ heightStyle: "fill" });
      SetTabActive(1,"displaylogtabs-log");
      // CreateDeployReports();
	  // $(this).parent().draggable();
     },
     dialogClass: 'tabbedDialog'
    }).parent().draggable({cancel:'',handle:'.right_panel_title_area'});


  var buttons = [
                 {
                  text : "Ok",
                  click : function()
                  {
                	  $("#displaylogtabs").remove();
                	  $(this).dialog("close").dialog('destroy');
                	  objtypeAsInt = save_objtypeAsInt;
                  }
                 } ];
    pwd.dialog("option", "title", "Results for #" + lognum);
    // pwd.dialog("option", "height", "450px");
    pwd.dialog("option", "width", "1080px");
    pwd.dialog("option", "buttons", buttons);

    pwd.dialog('open');

}

function DisplayBuild(compid,buildno,success)
{
	var cls=(success)?"build_ok":"build_fail";
	var save_objtypeAsInt = objtypeAsInt;
	var h = "<div class=\"tabrow_tabmenu\" id=\"displaybuildtabs\">" +
	"<div class=\"moduletable_tabmenu\">" +
	"<ul class=\"menu_tabmenu\">" +
	"<li id=\"dbt-buildlog\" onclick=\"SetTabActive(2,this.id)\">Build Log</li>" +
	"<li id=\"dbt-comps\" onclick=\"SetTabActive(2,this.id)\">Deployed Servers</li>" +
    "<li id=\"dbt-general\" onclick=\"SetTabActive(2,this.id)\">General</li>" +
    "<li id=\"dbt-files\" style=\"display:none;\" onclick=\"SetTabActive(2,this.id)\">Files</li>" +
    "<li id=\"dbt-defects\" style=\"display:none;\" onclick=\"SetTabActive(2,this.id)\">Change Requests</li>" +
    "</ul>" +
    "</div>" +
    "<div class=\"right_panel_title_area\" style=\"display:inline-block;\">" +
    "<h1 class=\""+cls+"\">Build #"+buildno+"</h1>" +
    "</div>" +
	"<div id=\"dbt-buildlog-data\" style=\"height:400px; overflow:auto;display:none\"></div>" +
	"<div id=\"dbt-comps-data\" style=\"height:400px; display:none\">" +
	"<table cellspacing=\"0\" width=\"100%\" id=\"dbt-comp-tab\"><thead class=\"ui-widget-header\"><tr><th align=\"left\">Component</th><th align=\"left\">Server</th><th align=\"left\">Deployment ID</th><th align=\"left\">Date/Time</th></tr><tbody></tbody>" +
	"</table>" +
	"</div>" +
	"<div id=\"dbt-general-data\" style=\"height:400px; display:none\"><table id=\"buildsum\" class=\"dev_table\"><tbody></tbody></table></div>" +
	"<div id=\"dbt-files-data\" style=\"height:400px; display:none\"><table id=\"buildfiles\" class=\"dev_table\"><tbody></tbody></table></div>" +
	"<div id=\"dbt-defects-data\" style=\"height:400px; display:none\"><table id=\"builddefects\"><thead class=\"ui-widget-header\"><tr><td style=\"width:100px\">CR ID</td><td>Title</td></tr></thead><tbody></tbody></table></div>" +
	"</div>";

  var pwd = parent.$("#buildlog");
  pwd.html(h);

  pwd.dialog({
	  modal : true,
	  open : function(event, ui) {
		  getBuildLogDetails(compid,buildno);
		  SetTabActive(2,"dbt-buildlog");
		  $(this).css("height", "400px");
		 //  $(this).parent().draggable();
	  },
	  dialogClass: 'tabbedDialog'
  }).css({cursor: 'move'}).parent().draggable({cancel:'',handle:''});

  var buttons = [
                 {
                  text : "Ok",
                  click : function()
                  {
                	  $("#displaybuildtabs").remove();
                	  $(this).dialog("close").dialog('destroy');
                	  objtypeAsInt = save_objtypeAsInt;
                  }
                 } ];
    pwd.dialog("option", "width", "650px");
    pwd.dialog("option", "buttons", buttons);

    pwd.dialog('open');

}
