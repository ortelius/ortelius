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

(function($) {
//function getGlobalizedDateTime(timestamp)
//{
//	var d = new Date(timestamp*1000);
//	return Globalize.format(d, "d") + ' ' + Globalize.format(d, "T");
//}

function _getFeedTime(timestamp)
{
	var currentTime = ~~(new Date().getTime() / 1000);
	var ago = currentTime - timestamp; 
	if(ago < 60) {
		return "less than a minute ago";
	} else if(ago < 60*60) {
		return ~~(ago/60) + " minutes ago";
	} else if(ago < 2*60*60) {
		return "about an hour ago";
	} else if(ago < 24*60*60) {
		return ~~(ago/(60*60)) + " hours ago";
	} else if(ago < 2*24*60*60) {
		return "a day ago";
	} else {
		return ~~(ago/(24*60*60)) + " days ago";
	}
}

function _getFileSize(size)
{
	if(size < 1024) {
		return size + '&nbsp;bytes';
	} else if(size < 1024*1204) {
		return ~~(size/1024) + 'KB';
	} else {
		return ~~(size/(1024*1024)) + 'MB';
	}
}

var newsid = 0;

function addToHistoryFeed($feed, item, otid, prepend, $after)
{
	var icon = item.icon ? item.icon : 'audititem';
	var text = '';
	var time = _getFeedTime(item.when);
	var cmnt = (item.cmnt == 0) ? 'Comment' : (item.cmnt + ((item.cmnt == 1) ? ' Comment' : ' Comments'));
	// 9650 is up arrow; 9660 is down arrow
	var user = '<a class="tooltip-follows" href="UserDetails?uid=' + item.user.id + '"><b>' + item.user.name + '</b></a>';
	if(item.user.tooltip) {
		user += '<div class="tooltip"><div class="tooltip-text">' + item.user.tooltip + '</div><div class="tooltip-arrow">&#9660;</div></div>';
	}
	var dbid = item.kind + item.id;
	var subsid = '', subs = '', cmntlink = '', attach = '', attachpicker = '', tasks = '';
	
    if((item.kind == 'de')) {
    	text = 'Deployment <a href="DeployDetails?deployid=' + item.id + '"><b>#' + item.id + '</b></a>';
    	if(item.app) {
        	text += ' of application <a href="AppDetails?appid=' + item.app.id + '"><b>' + item.app.name + '</b></a>';
    	}
    	if(item.env) {
    	    text += ' into environment <a href="EnvDetails?envid=' + item.env.id + '"><b>' + item.env.name + '</b></a>';
    	}
    	subsid = dbid;
    } else if((item.kind == 'hn') || (item.kind == 'no')) {
    	if(item.obj) {
    		switch(item.obj.type) {
    		case 'ap': text = 'Application <a href="AppDetails?appid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '; break;
    		case 'av': text = 'Application Version <a href="AppDetails?appid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '; break;
    		case 'en': text = 'Environment <a href="EnvDetails?envid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '; break;
    		case 're': text = 'Repository <a href="RepDetails?repid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '; break;
    		case 'se': text = 'Server <a href="ServerDetails?serverid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '; break;
    		case 'us': text = 'User <a class="tooltip-follows" href="UserDetails?userid=' + item.obj.id + '"><b>' + item.obj.name + '</b></a>: '
    					+ '<div class="tooltip"><div class="tooltip-text">' + item.obj.tooltip + '</div><div class="tooltip-arrow">&#9660;</div></div>';
    			break;
    		}
    		subsid = item.obj.type + item.obj.id;
    	} else {
    		subsid = otid;
    	}
    	text += item.text;
    } else if(item.kind == 'ae') {
    	text = 'Application ';
    	if(item.app) {
    		text += '<a href="AppDetails?appid=' + item.app.id + '"><b>' + item.app.name + '</b></a>';
    	}
    	text += ' has been ' + item.icon;
    	if(item.dom) {
    		text += ' for domain  <a href="EnvDetails?envid=' + item.dom.id + '"><b>' + item.dom.name + '</b></a>';
    	}
    	if(item.text) {
    		text += '<br>' + item.text;
    	}
    } else if(item.kind == 'rq') {
    	console.log("item.kind=rq");
    	if(item.obj) {
    		console.log("item.obj.type=["+item.obj.type+"]");
    		switch(item.obj.type) {
    		case 'ta':
    			console.log("ta seen");
    			text = 'Request for <b>' + item.obj.name + '</b> ';
    			if(item.app) { text += 'on Application <a href="AppDetails?appid=' + item.app.id + '"><b>' + item.app.name + '</b></a> '; }
    			text += '<br>';
    			console.log("text="+text);
    			break;
    		case 'ce':  text = 'Calendar entry <b>' + item.obj.name + '</b> for environment <a href="EnvDetails?envid='+item.env.id+'">'+item.env.name+'</a> needs approval<br>'; console.log("text="+text);break;
    		}    		
    	}
    	text += item.text;
    } else {
    	text = 'undefined note type "' + item.kind + '"';
    }
    
    newsid++;

    if(subsid != '') {
	    subs = ((item.subs == 'true')
	    	? '<a href="#" id="newssub_' + newsid + ':' + subsid + '" class="newssubscribe newssub_' + subsid + '" isunsub="1">Unsubscribe</a>'
	    	: '<a href="#" id="newssub_' + newsid + ':' + subsid + '" class="newssubscribe newssub_' + subsid + '" isunsub="0">Subscribe</a>')
	    	+ ' | ';
    }
    
    if(item.kind != 'no') {
    	cmntlink = '<a href="#" id="newscomment_' + newsid + ':' + dbid + '" class="newscomment">' + cmnt + '</a> | ';
    }
   
    if(item.attach > 0) {
    	attach = '<img id="attach_' + newsid + '" class="attachpicker-follows" src="images/attachment.png" style="margin-top:-14px;float:right;opacity:0.5;">'
    		+ '<div class="attachpicker"></div>';
    }
    if(item.tasks > 0) {
    	tasks = '<img id="tasks_' + newsid + '" src="images/task.png" style="margin-top:-14px;float:right;opacity:0.5;">';
    }

    var li = '<li style="min-height: 48px; padding: 2px; width: 90%; margin-top: 4px" class="ui-widget-content"><div style="padding: 2px">'
		+ ' <img src="images/' + icon + '-med.png" style="float:left" class="tooltip-follows">'
		+ '  <div class="tooltip"><div class="tooltip-text">' + item.whenstr + '</div><div class="tooltip-arrow">&#9660;</div></div>'
		+ ' <div id="news_' + newsid + '" style="margin-left: 60px; padding: 5px">'
		+ '  <div>' + text + '</div>'
		+ '  <div>' + subs + cmntlink + time + ' by ' + user + '</div>'
		+ ' </div></div>' + attach + tasks + '</li>';
    
    if(prepend) {
    	$feed.prepend(li);
    } else {
    	$after.after(li);
    }
	
    if(item.attach > 0) {
		$('#attach_' + newsid).button().hover(function() {
			$(this).css('opacity', 1.0);
		}).mouseleave(function() {
			$(this).css('opacity', 0.5);
		}).click(function() {
			var $attachButton = $(this);
			var $attachPicker = $attachButton.next('.attachpicker');
			$.ajax({
			    url: 'NewsFeed?reason=attachments&otid=' + dbid,
			    dataType: 'json',
			    type: 'GET',
			    success: function (res) {
				    //console.log(res);
					var content = '';
			    	try {
			    		for(var n = 0; n < res.data.length; n++) {
			    			var icon = res.data[n].icon ? ('filetypes/' + res.data[n].icon + '.png') : 'audititem.gif';
							if(content != '') { content += '<br>'; }
							content += '<span class="file-attachment"><img style="vertical-align:-20%;" src="images/' + icon + '">&nbsp;'
								+ '<a href="GetAttachment?attachid=' + res.data[n].id + '">' + res.data[n].filename + '</a>'
								+ '&nbsp(' + _getFileSize(res.data[n].size) + ')</span>&nbsp;&nbsp;';			    			
			    		}
					} catch(e) {
						alert('Exception: ' + e);
					}
					$attachPicker.html(content).fadeIn().position({
						at: 'right+16 bottom',
						of: $attachButton,
						my: 'right top'				
					});
					$('.attachpicker .file-attachment').click(function() {
						$attachPicker.fadeOut();
					});
			    }
			});
		});
    }
    if(item.tasks > 0) {
		$('#tasks_' + newsid).button().hover(function() {
			$(this).css('opacity', 1.0);
		}).mouseleave(function() {
			$(this).css('opacity', 0.5);
		});
    }
}

function addMoreHistoryButton($feed, otid, since, isPending)
{
	$feed.append(
		  '<li id="morenews" style="min-height: 16px; padding: 2px; width: 90%; margin-top: 4px" class="ui-widget-content"><div style="padding: 2px">'
		+ ' <a href="#" id="morenewsbtn">Click to see ealier items</a></li>');

	$feed.find('#morenews #morenewsbtn').bind('click', function() {		
		console.log('otid = ' + otid + '; to = ' + since + '; from = ' + ((parseInt(since) - 31*24*60*60)));
		
		var pending = /*(feed.indexOf('pending') != -1)*/ (isPending === true) ? '&pending=1' : '';
		$.ajax({
		    url: 'NewsFeed?otid=' + otid + '&from=' + (parseInt(since) - 31*24*60*60) + '&to=' + since + pending,
		    dataType: 'json',
		    type: 'GET',
		    success: function (res) {
			    //console.log(res);
		    	try {
		    		var oldest = 0;
		    		for(var r = 0; r < res.data.length; r++) {
		    			if(oldest == 0) { oldest = res.data[r].when; }
		    			addToHistoryFeed($feed, res.data[r], otid, false, $feed.find('#morenews'));
		    		}
		    		$feed.find('#morenews').remove();
		    		if(oldest != 0) { addMoreHistoryButton($feed, otid, oldest, isPending); }
				} catch(e) {
					alert('Exception: ' + e);
				}
				//addNewsCallbacks($feed);
		    }
		});
	});
}

function addComment(newsid, cmnt)
{
	var user = '<a class="tooltip-follows" href="UserDetails?uid=' + cmnt.user.id + '"><b>' + cmnt.user.name + '</b></a>';
	if(cmnt.user.tooltip) {
		user += '<div class="tooltip"><div class="tooltip-text">' + cmnt.user.tooltip + '</div><div class="tooltip-arrow">&#9660;</div></div>';
	}
	$('#comments_' + newsid).slideDown('slow').prepend('<div class="ui-widget-content comment-content" style="padding:4px">'
			+ '<div>' + cmnt.text + '</div>'
			+ '<div>' + _getFeedTime(cmnt.when) + ' by ' + user + '</div>'
			+ '</div>');
}

function addNewsCallbacks($feed)
{
	// use event delegation and attach the handler once
	$feed.on('click', '.newscomment', function() {
		var $newscomment = $(this);
		var ID = $newscomment.attr('id');
		var sid = ID.split('newscomment_');
		var ids = sid[1].split(':');
		var newsid = ids[0];
		var dbid  = ids[1];
		var expanded = $(this).attr('expanded');
		console.log('news id = ' + newsid);

		if(expanded == 1) {			
			$feed.find('#comments_' + newsid).slideUp('slow').remove();
			$(this).attr('expanded', '0');
		} else if(expanded == 2) {
			console.log("Expand in progress");
		} else {
			$(this).attr('expanded', '2');

			$feed.find('#news_' + newsid).slideDown('slow').append(
					  '<div id="comments_' + newsid + '" style="margin-left:16px;margin-right:16px;margin-top:4px">'
					+ ' <div style="width:100%" class="ui-widget-content">'
					+ '  <div><textarea id="cmnttxt_' + newsid + '" style="width:100%;height:25px;color:#777;">Say something?</textarea></div>'
					+ '  <div style="background-color: #eee; height: 26px"><button disabled id="cmntbut_' + newsid + '" style="float: right">Comment</button></div>'
					+ ' </div>'
					+ '</div>');
			
			$feed.find('#cmntbut_' + newsid).button().click(function() {
				var messageText = $('#cmnttxt_' + newsid)[0];
				if(messageText.value === messageText.defaultValue) {
					alert("Please type something in the box");
					return;
				}
				$.ajax({
				    url: 'NewsFeed?reason=addcmnt&otid=' + dbid + '&text=' + messageText.value,
				    dataType: 'json',
				    type: 'GET',
					async: false,
				    success: function (res) {
				    	if(!res.result) {
				    		if(res.error) {
				    			alert("Comment failed to post: " + res.error);
				    		} else {
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
			
			$feed.find('#cmnttxt_' + newsid).keypress(function() {
				$feed.find('#cmntbut_' + newsid).button(((this.value === this.defaultValue) || (this.value === '')) ? "disable" : "enable");
			}).focus(function() {
				if(this.value === this.defaultValue) {
					this.value = '';
					this.style.color = '#000';
				}
			}).blur(function() {
				if(this.value === '') {
					this.value = this.defaultValue;
					this.style.color = '#777';
				}
			});
			
			$.ajax({
			    url: 'NewsFeed?reason=comments&otid=' + dbid,
			    dataType: 'json',
			    type: 'GET',
				async: false,
			    success: function (res) {
			    	if(!res.result) {
			    		console.log(JSON.stringify(res));
			    		if(res.error) {
			    			alert("Unable to retrieve comments: " + res.error);
			    		} else {
			    			alert("Unable to retrieve comments");
			    		}
			    		return;
			    	}
			    	console.log('comments for ' + dbid + ' are ' + JSON.stringify(res));
			    	try {
			    		for(var r = 0; r < res.data.length; r++) {
			    			addComment(newsid, res.data[r]);
			    		}
					} catch(e) {
						console.log('Exception: ' + e);
					}
			    }
			});

			$(this).attr('expanded', '1');
		}
	});

	$feed.on('click', '.newssubscribe', function() {
		var $newssub = $(this);
		var ID = $newssub.attr('id');
		var sid = ID.split('newssub_');
		var ids = sid[1].split(':');
		var newsid = ids[0];
		var subsid  = ids[1];
		var isunsub = $(this).attr('isunsub');
		console.log('news id = ' + newsid);
		
		$.ajax({
		    url: 'NewsFeed?reason=' + ((isunsub == 1) ? 'unsubscribe' : 'subscribe') + '&otid=' + subsid,
		    dataType: 'json',
		    type: 'GET',
			async: false
		});
		
		if(isunsub == 1) {
			$('.newssub_' + subsid).attr('isunsub', 0).html('Subscribe');
		} else {
			$('.newssub_' + subsid).attr('isunsub', 1).html('Unsubscribe');
		}
	});
	
	$feed.on('mouseenter', '.tooltip-follows', function() {
		$(this).next('.tooltip').fadeIn();
		$(this).next('.tooltip').position({
			at: 'bottom center',
			of: $(this),
			my: 'top-32'
		});
	}).on('mouseleave', '.tooltip-follows', function() {
		$('.tooltip').fadeOut();
	});
}

function _updateAttachments() {
	var fileInput = document.getElementById('messageAttach');
	$('#messageAttachments').html('');
	var files = fileInput.files;
	for(var n = 0; n < files.length; n++) {
		$('#messageAttachments').append(
			'<span class="file-attachment"><img style="vertical-align:-25%;" src="images/audititem.gif">&nbsp;'
			+ files[n].name + '&nbsp(' + _getFileSize(files[n].size) + ')</span>&nbsp;&nbsp;');
	}
	$('#messageAttach').focus(null).hide();
};

$.fn.createNewsMessageBox = function createNewsMessageBox(/*boxid,*/ feedid, text, otid) {
	console.log(">>>>>>>>>>>>>>>> fn.createNewsMessageBox");
	/*$(boxid)*/ this.css("class", "ui-widget-content")
		.prepend(
			  '<form id="messageForm"><div id="messageOuter" style="height:100px;overflow:hidden;">'
			+ '<div style="background-color:white;border:black solid 1px;"><img id="messageIcon" class="iconpicker-follows" src="images/audititem-med.png" style="float:left;">'
			+ '<div class="iconpicker"></div>'
			+ '<div style="margin-left:54px;"><textarea id="messageText" name="text" style="width:100%;height:48px;color:#777;border:none;">' + text + '</textarea></div></div>'
			+ '<div style="height:26px;padding-top:4px;width:100%;"><div id="messageAttachments" style="float:left;min-width:150px;height:100%;"></div>'
			+ '<div id="messageProgress" style="float:left;display:none;"><div class="progress"><div class="progress-bar"></div><div class="progress-percent">0%</div></div></div>'
			+ '<div style="float:right"><img id="messageAttachButton" src="images/attachment.png" style="opacity:0.5;padding:4px;margin-right:2px;">'
			+ '<button id="addMessage" disabled>Add Message</button></div></div>'
			+ '<input id="messageAttach" name="attachments[]" type="file" style="font-size:50px;height:100%;width:100%;position:relative;top:-80px;left:0px;opacity:0.1;filter:alpha(opacity:0.1);" multiple>'
			+ '</div></form>'
		);
	
	$('.iconpicker-follows').click(function() {
		var $messageIcon = $(this);
		var iconPicker = $messageIcon.next('.iconpicker');
		var icons = ['audititem', 'approved', 'rejected'];
		var selected = $messageIcon.attr('src');
		var content = '';
		for(var n = 0; n < icons.length; n++) {
			var src = 'images/' + icons[n] + '-med.png';
			if(content != '') { content += '<br>'; }
			content += '<img class="iconSelectImage' + ((src === selected) ? ' iconSelectImage-selected' : '') + '" src="' + src + '" data-value="' + icons[n] + '">';
		}
		iconPicker.html(content).fadeIn().position({
			at: 'bottom center',
			of: $(this),
			my: 'top'				
		});
		$('.iconSelectImage').click(function() {
			$messageIcon.attr('src', $(this).attr('src'));
			$messageIcon.data('value', $(this).data('value'));
			iconPicker.fadeOut();
		});
	});
	
	$('#messageForm').ajaxForm();
	
	$('#addMessage').button().click(function() {
		$('#addMessage').button("disable");
		var messageText = $('#messageText')[0];
		if(messageText.value === messageText.defaultValue) {
			alert("Please type something in the box");
			return;
		}
		var messageIcon = $('#messageIcon').data('value');
		if(messageIcon === 'audititem') {
			messageIcon = null;
		}
		var icon = (messageIcon != null) ? ('&icon=' + messageIcon) : '';
		$('#messageAttach').show();

		var $bar = $('.progress-bar');
		var $percent = $('.progress-percent');
		var hasFileAttachments = ($('#messageAttachments .file-attachment').length > 0);
		
		$('#messageForm').ajaxSubmit({
		    url: 'NewsFeed',
		    data: {
		    	reason: "add",
		    	otid: otid,
		    	icon: messageIcon
		    },
		    dataType: 'json',
		    type: 'POST',
			async: true,
			clearForm: true,
			beforeSend: function() {
				$('#messageAttach').hide();
				if(hasFileAttachments) {
					$('#messageAttachments').hide();
					$('#messageProgress').show();
					var percentVal = '0%';
			        $bar.width(percentVal);
			        $percent.html(percentVal);
				}
			},
			uploadProgress: function(event, position, total, percentComplete) {
				if(hasFileAttachments) {
					var percentVal = percentComplete + '%';
			        $bar.width(percentVal);
			        $percent.html(percentVal);
				}
			},
		    success: function(res) {
		    	//var percentVal = '100%';
		        //$bar.width(percentVal);
		        //$percent.html(percentVal);
		        $('#messageProgress').hide();
		    	$('#messageAttachments').html('');
				$('#messageAttachments').show();
		    	if(!res.result) {
		    		console.log(JSON.stringify(res));
		    		if(res.error) {
		    			alert("Message failed to post: " + res.error);
		    		} else {
		    			alert("Message failed to post");
		    		}
		    		return;
		    	}
		    	addToHistoryFeed($(feedid), res.data, otid, true);
		    	messageText.value = messageText.defaultValue;
		    	messageText.style.color = '#777';
		    }
		});
	});
	
	$('#messageAttach').hide().change(function(e) {
		console.log("messageAttach - change event");
		_updateAttachments();
	});
	
	$('#messageAttachButton').button().hover(function() {
		$(this).css('opacity', 1.0);
	}).mouseleave(function() {
		$(this).css('opacity', 0.5);
	}).click(function() {
		$('#messageAttach').show().focus().focus(function(e) {
			console.log("messageAttach - focus event");
			_updateAttachments();
			$('#messageText').focus();
		}).click();
	});
	
	$('#messageText').keypress(function() {
		$('#addMessage').button(((this.value === this.defaultValue) || (this.value === '')) ? "disable" : "enable");
	}).focus(function() {
		if(this.value === this.defaultValue) {
			this.value = '';
			this.style.color = '#000';
		}
	}).blur(function() {
		if(this.value === '') {
			this.value = this.defaultValue;
			this.style.color = '#777';
		}
	});
	
	var textarea = document.getElementById('messageOuter');
//	console.log(textarea);
//	textarea.ondrop = function(e) {
//		e.preventDefault();
//		$('#messageOuter').css('background-color','white');
//		$('#messageText').css('background-color','white');
//		//var $attachBox = $('#messageAttachments');
//		//for(var i = 0; i < e.dataTransfer.files.length; i++) {
//		//	$attachBox.append('<span class="messageAttachment"><img style="vertical-align:-25%;" src="images/audititem.gif">&nbsp;' + e.dataTransfer.files[i].name + '</span>&nbsp;&nbsp;');
//		//	$attachBox.last('.messageAttachment').data('file', e.dataTransfer.files[i]);
//		//}
//		fileInput.ondrop(e);
//	};
	textarea.ondragover = function(e) {
//		e.preventDefault();
		//$('#messageOuter').css('background-color','yellow');
		//$('#messageText').css('background-color','yellow');
		$('#messageAttach').show();
	};
	textarea.ondragleave = function(e) {
		//$('#messageOuter').css('background-color','white');
		//$('#messageText').css('background-color','white');
	};
	return this;
};

$.fn.createNewsFeed = function createNewsFeed(otid, isPending) {
	var pending = /*(id.indexOf('pending') != -1)*/ (isPending === true) ? '&pending=1' : '';
	var $feed = this;
	$.ajax({
	    url: 'NewsFeed?otid=' + otid + pending,
	    dataType: 'json',
	    type: 'GET',
	    success: function (res) {
		    //console.log(res);
	    	try {
	    		var oldest = 0;
	    		for(var r = 0; r < res.data.length; r++) {
	    			if(oldest == 0) {
	    				oldest = res.data[r].when;
	    			}
	    			addToHistoryFeed($feed, res.data[r], otid, true);
	    		}
	    		addMoreHistoryButton($feed, otid, oldest, isPending);
			} catch(e) {
				alert('Exception: ' + e);
			}
			addNewsCallbacks($feed);
	    }
	});
	return this;
};
})(jQuery);