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
