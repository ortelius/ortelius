var $attvals = {
	height: 350,
	width: 450,
	draggable: true,
	modal: false,
	autoOpen: false,
    buttons: {
          "save": function() {
            $( this ).dialog( "close" );
            },
          "cancel": function() {
        	  $( this ).dialog( "close" );
          }  
     },
     Cancel: function() {
        $( this ).dialog( "close" );
     }
};

var $modalatts = {
    height: 350,
    width: 450,
    draggable: true,
    modal: true,
    autoOpen: false,
    buttons: {
        "ok": function() {
          $( this ).dialog( "close" );
          },
        "cancel": function() {
      	  $( this ).dialog( "close" );
        }  
        },
    Cancel: function() {
       $( this ).dialog( "close" );
    }
};
