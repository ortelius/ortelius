/*
 * jsPlumb
 *
 * Title:jsPlumb 1.4.0
 *
 * Provides a way to visually connect elements on an HTML page, using either SVG, Canvas
 * elements, or VML.
 *
 * This file contains the jQuery adapter.
 *
 * Copyright (c) 2010 - 2013 Simon Porritt (http://jsplumb.org)
 *
 * http://jsplumb.org
 * http://github.com/sporritt/jsplumb
 * http://code.google.com/p/jsplumb
 *
 * Dual licensed under the MIT and GPL2 licenses.
 */
/*
 * the library specific functions, such as find offset, get id, get attribute, extend etc.
 * the full list is:
 *
 * addClass				adds a class to the given element
 * animate				calls the underlying library's animate functionality
 * appendElement		appends a child element to a parent element.
 * bind					binds some event to an element
 * dragEvents			a dictionary of event names
 * extend				extend some js object with another.  probably not overly necessary; jsPlumb could just do this internally.
 * getAttribute			gets some attribute from an element
 * getDragObject		gets the object that is being dragged, by extracting it from the arguments passed to a drag callback
 * getDragScope			gets the drag scope for a given element.
 * getDropScope			gets the drop scope for a given element.
 * getElementObject		turns an id or dom element into an element object of the underlying library's type.
 * getOffset			gets an element's offset
 * getOriginalEvent     gets the original browser event from some wrapper event
 * getPageXY			gets the page event's xy location.
 * getParent			gets the parent of some element.
 * getScrollLeft		gets an element's scroll left.  TODO: is this actually used?  will it be?
 * getScrollTop			gets an element's scroll top.  TODO: is this actually used?  will it be?
 * getSize				gets an element's size.
 * getUIPosition		gets the position of some element that is currently being dragged, by extracting it from the arguments passed to a drag callback.
 * hasClass				returns whether or not the given element has the given class.
 * initDraggable		initializes an element to be draggable
 * initDroppable		initializes an element to be droppable
 * isDragSupported		returns whether or not drag is supported for some element.
 * isDropSupported		returns whether or not drop is supported for some element.
 * removeClass			removes a class from a given element.
 * removeElement		removes some element completely from the DOM.
 * setAttribute			sets an attribute on some element.
 * setDragFilter		sets a filter for some element that indicates areas of the element that should not respond to dragging.
 * setDraggable			sets whether or not some element should be draggable.
 * setDragScope			sets the drag scope for a given element.
 * setOffset			sets the offset of some element.
 * trigger				triggers some event on an element.
 * unbind				unbinds some listener from some element.
 */
(function($) {

	//var getBoundingClientRectSupported = "getBoundingClientRect" in document.documentElement;

	var _getElementObject = function(el) {
		return typeof(el) == "string" ? $("#" + el) : $(el);
	};

	jsPlumb.CurrentLibrary = {

		/**
		 * adds the given class to the element object.
		 */
		addClass : function(el, clazz) {
			el = _getElementObject(el);
			try {
				if (el[0].className.constructor == SVGAnimatedString) {
					jsPlumbUtil.svg.addClass(el[0], clazz);
				}
			}
			catch (e) {
				// SVGAnimatedString not supported; no problem.
			}
            try {
                el.addClass(clazz);
            }
            catch (e) {
                // you probably have jQuery 1.9 and Firefox.
            }
		},

		/**
		 * animates the given element.
		 */
		animate : function(el, properties, options) {
			el.animate(properties, options);
		},

		/**
		 * appends the given child to the given parent.
		 */
		appendElement : function(child, parent) {
			_getElementObject(parent).append(child);
		},

		/**
		* executes an ajax call.
		*/
		ajax : function(params) {
			params = params || {};
			params.type = params.type || "get";
			$.ajax(params);
		},

		/**
		 * event binding wrapper.  it just so happens that jQuery uses 'bind' also.  yui3, for example,
		 * uses 'on'.
		 */
		bind : function(el, event, callback) {
			el = _getElementObject(el);
			el.bind(event, callback);
		},

		/**
         * mapping of drag events for jQuery
         */
		dragEvents : {
			'start':'start', 'stop':'stop', 'drag':'drag', 'step':'step',
			'over':'over', 'out':'out', 'drop':'drop', 'complete':'complete'
		},

		/**
		 * wrapper around the library's 'extend' functionality (which it hopefully has.
		 * otherwise you'll have to do it yourself). perhaps jsPlumb could do this for you
		 * instead.  it's not like its hard.
		 */
		extend : function(o1, o2) {
			return $.extend(o1, o2);
		},

		/**
		 * gets the named attribute from the given element object.
		 */
		getAttribute : function(el, attName) {
			return el.attr(attName);
		},

		getClientXY : function(eventObject) {
			return [eventObject.clientX, eventObject.clientY];
		},

		/**
		 * takes the args passed to an event function and returns you an object representing that which is being dragged.
		 */
		getDragObject : function(eventArgs) {
			return eventArgs[1].draggable || eventArgs[1].helper;
		},

		getDragScope : function(el) {
			return el.draggable("option", "scope");
		},

		getDropEvent : function(args) {
			return args[0];
		},

		getDropScope : function(el) {
			return el.droppable("option", "scope");
		},

		/**
		* gets a DOM element from the given input, which might be a string (in which case we just do document.getElementById),
		* a selector (in which case we return el[0]), or a DOM element already (we assume this if it's not either of the other
		* two cases).  this is the opposite of getElementObject below.
		*/
		getDOMElement : function(el) {
			if (typeof(el) == "string") return document.getElementById(el);
			else if (el.context || el.length != null) return el[0];
			else return el;
		},

		/**
		 * gets an "element object" from the given input.  this means an object that is used by the
		 * underlying library on which jsPlumb is running.  'el' may already be one of these objects,
		 * in which case it is returned as-is.  otherwise, 'el' is a String, the library's lookup
		 * function is used to find the element, using the given String as the element's id.
		 *
		 */
		getElementObject : _getElementObject,

		/**
		  * gets the offset for the element object.  this should return a js object like this:
		  *
		  * { left:xxx, top: xxx }
		 */
		getOffset : function(el) {
			return el.offset();
		},

		getOriginalEvent : function(e) {
			return e.originalEvent;
		},

		getPageXY : function(eventObject) {
			return [eventObject.pageX, eventObject.pageY];
		},

		getParent : function(el) {
			return _getElementObject(el).parent();
		},

		getScrollLeft : function(el) {
			return el.scrollLeft();
		},

		getScrollTop : function(el) {
			return el.scrollTop();
		},

		getSelector : function(context, spec) {
            if (arguments.length == 2)
                return _getElementObject(context).find(spec);
            else
                return $(context);
		},

		/**
		 * gets the size for the element object, in an array : [ width, height ].
		 */
		getSize : function(el) {
			return [el.outerWidth(), el.outerHeight()];
		},

        getTagName : function(el) {
            var e = _getElementObject(el);
            return e.length > 0 ? e[0].tagName : null;
        },

		/**
		 * takes the args passed to an event function and returns you an object that gives the
		 * position of the object being moved, as a js object with the same params as the result of
		 * getOffset, ie: { left: xxx, top: xxx }.
		 *
		 * different libraries have different signatures for their event callbacks.
		 * see getDragObject as well
		 */
		getUIPosition : function(eventArgs, zoom) {

			zoom = zoom || 1;
			// this code is a workaround for the case that the element being dragged has a margin set on it. jquery UI passes
			// in the wrong offset if the element has a margin (it doesn't take the margin into account).  the getBoundingClientRect
			// method, which is in pretty much all browsers now, reports the right numbers.  but it introduces a noticeable lag, which
			// i don't like.

			/*if ( getBoundingClientRectSupported ) {
				var r = eventArgs[1].helper[0].getBoundingClientRect();
				return { left : r.left, top: r.top };
			} else {*/
			if (eventArgs.length == 1) {
				ret = { left: eventArgs[0].pageX, top:eventArgs[0].pageY };
			}
			else {
				var ui = eventArgs[1],
				  _offset = ui.offset;

				ret = _offset || ui.absolutePosition;

				// adjust ui position to account for zoom, because jquery ui does not do this.
				ui.position.left /= zoom;
				ui.position.top /= zoom;
			}
            return { left:ret.left / zoom, top: ret.top / zoom };
		},

		hasClass : function(el, clazz) {
			return el.hasClass(clazz);
		},

		/**
		 * initialises the given element to be draggable.
		 */
		initDraggable : function(el, options, isPlumbedComponent, _jsPlumb) {
			options = options || {};

/*
			// css3 transforms
			// http://gungfoo.wordpress.com/2013/02/15/jquery-ui-resizabledraggable-with-transform-scale-set/
			options.start = _jsPlumb.wrap(options["start"], function(e, ui) {
				// TODO why is this 0?
			    ui.position.left = 0;
			    ui.position.top = 0;
			});

			options.drag = _jsPlumb.wrap(options["drag"], function(e, ui) {

				console.log("original", ui.originalPosition.left, ui.originalPosition.top);
				console.log("current", ui.position.left, ui.position.top);

				//var changeLeft = ui.position.left - ui.originalPosition.left; // find change in left
			    //var newLeft = ui.originalPosition.left + (changeLeft * _jsPlumb.getZoom()); // adjust new left by our zoomScale

			    //var changeTop = ui.position.top - ui.originalPosition.top; // find change in top
			    //var newTop = ui.originalPosition.top + (changeTop * _jsPlumb.getZoom()); // adjust new top by our zoomScale

			    //ui.position.left = newLeft;
			    //ui.position.top = newTop;

			    ui.position.left *= _jsPlumb.getZoom();
			    ui.position.top *= _jsPlumb.getZoom();

			});
*/


			// remove helper directive if present and no override
			if (!options.doNotRemoveHelper)
				options.helper = null;
			if (isPlumbedComponent)
				options['scope'] = options['scope'] || jsPlumb.Defaults.Scope;
			el.draggable(options);
		},

		/**
		 * initialises the given element to be droppable.
		 */
		initDroppable : function(el, options) {
			options['scope'] = options['scope'] || jsPlumb.Defaults.Scope;
			el.droppable(options);
		},

		isAlreadyDraggable : function(el) {
			return _getElementObject(el).hasClass("ui-draggable");
		},

		/**
		 * returns whether or not drag is supported (by the library, not whether or not it is disabled) for the given element.
		 */
		isDragSupported : function(el, options) {
			return el.draggable;
		},

		/**
		 * returns whether or not drop is supported (by the library, not whether or not it is disabled) for the given element.
		 */
		isDropSupported : function(el, options) {
			return el.droppable;
		},

		/**
		 * removes the given class from the element object.
		 */
		removeClass : function(el, clazz) {
			el = _getElementObject(el);
			try {
				if (el[0].className.constructor == SVGAnimatedString) {
					jsPlumbUtil.svg.removeClass(el[0], clazz);
                    return;
				}
			}
			catch (e) {
				// SVGAnimatedString not supported; no problem.
			}
			el.removeClass(clazz);
		},

		removeElement : function(element) {
			_getElementObject(element).remove();
		},

		setAttribute : function(el, attName, attValue) {
			el.attr(attName, attValue);
		},

		setDragFilter : function(el, filter) {
			if (jsPlumb.CurrentLibrary.isAlreadyDraggable(el))
				el.draggable("option", "cancel", filter);
		},

		setDraggable : function(el, draggable) {
			el.draggable("option", "disabled", !draggable);
		},

		setDragScope : function(el, scope) {
			el.draggable("option", "scope", scope);
		},

		setOffset : function(el, o) {
			_getElementObject(el).offset(o);
		},

		/**
		 * note that jquery ignores the name of the event you wanted to trigger, and figures it out for itself.
		 * the other libraries do not.  yui, in fact, cannot even pass an original event.  we have to pull out stuff
		 * from the originalEvent to put in an options object for YUI.
		 * @param el
		 * @param event
		 * @param originalEvent
		 */
		trigger : function(el, event, originalEvent) {
			var h = jQuery._data(_getElementObject(el)[0], "handle");
            h(originalEvent);
		},

		unbind : function(el, event, callback) {
			el = _getElementObject(el);
			el.unbind(event, callback);
		}
	};

	$(document).ready(jsPlumb.init);

})(jQuery);
