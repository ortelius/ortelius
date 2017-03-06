/*jshint browser:true*/
//
// jquery.sessionTimeout.js
//
// After a set amount of time, a dialog is shown to the user with the option
// to either log out now, or stay connected. If log out now is selected,
// the page is redirected to a logout URL. If stay connected is selected,
// a keep-alive URL is requested through AJAX. If no options is selected
// after another set amount of time, the page is automatically redirected
// to a timeout URL.
//
//
// USAGE
//
//   1. Include jQuery
//   2. Include jQuery UI (for dialog)
//   3. Include jquery.sessionTimeout.js
//   4. Call $.sessionTimeout(); after document ready
//
//
// OPTIONS
//
//   message
//     Text shown to user in dialog after warning period.
//     Default: 'Your session is about to expire.'
//
//   keepAliveUrl
//     URL to call through AJAX to keep session alive. This resource should do something innocuous that would keep the session alive, which will depend on your server-side platform.
//     Default: '/keep-alive'
//
//   redirUrl
//     URL to take browser to if no action is take after warning period
//     Default: '/timed-out'
//
//   logoutUrl
//     URL to take browser to if user clicks "Log Out Now"
//     Default: '/log-out'
//
//   warnAfter
//     Time in milliseconds after page is opened until warning dialog is opened
//     Default: 900000 (15 minutes)
//
//   redirAfter
//     Time in milliseconds after page is opened until browser is redirected to redirUrl
//     Default: 1200000 (20 minutes)
//

(function( $ ) {
	jQuery.sessionTimeout = function( options ) {
		var defaults = {
			message      : 'Your session is about to expire.',
			keepAliveUrl : '/keep-alive',
			redirUrl     : '/timed-out',
			logoutUrl    : '/log-out',
			warnAfter    : 900000, // 15 minutes
			redirAfter   : 1200000 // 20 minutes
		};
		
		console.log("sessionTimeout called");
		var DialogOpen=false;

		// Extend user-set options over defaults
		var o = defaults,
				dialogTimer,
				redirTimer;

		if ( options ) { o = $.extend( defaults, options ); }

		// Create timeout warning dialog
		$('body').append('<div title="Session Timeout" id="sessionTimeout-dialog">'+ o.message +'</div>');
		$('#sessionTimeout-dialog').dialog({
			autoOpen: false,
			width: 400,
			modal: true,
			closeOnEscape: false,
			open: function() { $(".ui-dialog-titlebar-close").hide(); },
			buttons: {
				// Button one - takes user to logout URL
				"Log Out Now": function() {
					window.location = o.logoutUrl;
				},
				// Button two - closes dialog and makes call to keep-alive URL
				"Stay Connected": function() {
					$(this).dialog('close');

					$.ajax({
						type: 'POST',
						url: o.keepAliveUrl
					});
					DialogOpen=false;

					// Stop redirect timer and restart warning timer
					controlRedirTimer('stop');
					controlDialogTimer('start');
				}
			}
		});

		function controlDialogTimer(action){
			switch(action) {
				case 'start':
					// After warning period, show dialog and start redirect timer
					dialogTimer = setTimeout(function(){
						$('#sessionTimeout-dialog').dialog('open');
						DialogOpen=true;
						controlRedirTimer('start');
					}, o.warnAfter);
					break;

				case 'stop':
					// console.log("Stopping popup timer");
					clearTimeout(dialogTimer);
					break;
			}
		}

		function controlRedirTimer(action){
			switch(action) {
				case 'start':
					// Dialog has been shown, if no action taken during redir period, redirect
					redirTimer = setTimeout(function(){
						window.location = o.redirUrl;
					}, o.redirAfter - o.warnAfter);
					break;

				case 'stop':
					clearTimeout(redirTimer);
					break;
			}
		}

		// Begin warning period
		controlDialogTimer('start');
		//
		// PAG mod - reset the timer on mouse movement or other interaction.
		//
		$(document).on('keyup mouseup mousemove touchend touchmove', function() {
			if (!DialogOpen) {
				controlDialogTimer('stop');
				controlDialogTimer('start');
			}
		});
	};
})( jQuery );