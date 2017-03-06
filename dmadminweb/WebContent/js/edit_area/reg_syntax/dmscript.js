console.log("Loading dmscript v2");
editAreaLoader.load_syntax["dmscript"] = {
	'DISPLAY_NAME' : 'DMScript'
	,'COMMENT_SINGLE': { 1: '//' }
	, 'COMMENT_MULTI': { '/*': '*/' }
    , 'QUOTEMARKS': { 1: "'", 2: '"' }
	//, 'QUOTEMARKS': []
	, 'KEYWORD_CASE_SENSITIVE': true
	, 'KEYWORDS': {
	    'constants': [
			'null', 'false', 'true'
		]
		//, 'types': [ 'xyz', 'abc'
		//]
		, 'statements': [
            'echo', 'if', 'else', 'while', 'for', 'foreach', 'iterate', 'set', 'incr', 'decr',
            'try', 'catch', 'finally', 'switch', 'case', 'default', 'break', 'continue', 
            'psloop', 'avloop', 'comploop', 'parallel', 'sequential', 'return', 'eval'
		]
 		, 'actions': [
			'deploy', 'checkout', 'transfer', 'setatt',
			'remotescript', 'assert', 'abort', 'query', 'notify', 'attachment', 'modify',
			'text_replace', 'set_text', 'add_element', 'set_attribute', 'remove_element',
			'rename', 'delete', 'create', 'read', 'sleep', 'runtask', 'waitfor',
			'getapplication', 'getcomponent', 'getcredential', 'getenvironment', 'getserver',
			'getuser'
		]
		, 'functions': [
		    'append','appendstd','base64encode','base64decode','basename','date','decimal',
		    'dirname','dropzone','filter','flatten','getapplication','getcomponent','getcredential',
		    'getenvironment','getserver','getuser','hex','instr','length','lower','me','now','octal',
		    'regsub','restful_post','restful_get','random','rtrim','soap','split','stdpath','substr',
		    'sum','to_json','upper','xmlparse'
		]
	}
	, 'OPERATORS': [
		'+', '-', '/', '*', '=', '<', '>', '%', '!', '?', ':', '&', '+='
	]
	, 'DELIMITERS': [
		'(', ')', '[', ']', '{', '}'
	]
	, 'REGEXPS': {
		// first match ends further checks
	    // highlight all variables ${abc.def}
		'vartest2': {
			'search' : '("[^"]*)(\\$\\{[A-Za-z][A-Za-z0-9\\$\\(\\)\\[\\]\\_.]*\\})([^"]*")'
			,'class' : 'variables'
			,'modifiers' : 'g'
			,'execute' : 'after' // before or after
		},
		'var1' : {
			'search' : '()(\\$\\{[A-Za-z][A-Za-z0-9\\$\\(\\)\\[\\]\\_.]*\\})()'
			,'class' : 'variables'
			,'modifiers' : 'g'
			,'execute' : 'before'
		},
		/* THIS IS HIGHLIGHTING $f when between [ ] in a comment. But "var1" does not highlight ${f} */
		'var2' : {
			'search' : '()(\\$\\w+)()'
			,'class' : 'variables'
			,'modifiers' : 'g'
			,'execute' : 'before' // before or after 
		},
		'var3' : {
			'search' : '(set[\\t ]*)(\\w+)()'
			,'class' : 'variables'
			,'modifiers' : 'g'
			,'execute' : 'before' // before or after
		},
		'var4' : {
			'search' : '()(\\w+)(:)'
				,'class' : 'variables'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		},
		/*
		'vartest': {
			'search' : '(\')([^\']*)(\')'
			,'class' : 'string'
			,'modifiers' : 'g'
			,'execute' : 'after' // before or after
		},

		
		*/
		'usingapp' : {
			'search' : '()(using application)()'
				,'class' : 'statements'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		},
		'usingstr' : {
			'search' : '()(using stream)()'
				,'class' : 'statements'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		},
		'usingdz' : {
			'search' : '()(using dropzone)()'
				,'class' : 'statements'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		},
		'usingcomp' : {
			'search' : '()(using component)()'
				,'class' : 'statements'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		},
		'usingenv' : {
			'search' : '()(using environment)()'
				,'class' : 'statements'
				,'modifiers' : 'g'
				,'execute' : 'before' // before or after
		}
 		
	}
	, 'STYLES': {
	    'COMMENTS': 'color: #AAAAAA;'
		, 'QUOTESMARKS': 'color: #6381F8;'
		, 'KEYWORDS': {
		    'constants': 'color: #EE0000;'
			, 'types': 'color: #0000EE;'
			, 'statements': 'color: #60CA00;'
			, 'actions': 'color: #48BDDF;'
			, 'functions': 'color: #A46666;'
		}
		, 'OPERATORS': 'color: #FF00FF;'
		, 'DELIMITERS': 'color: #0038E1;'
		, 'REGEXPS': {
		    'precompiler': 'color: #009900;'
		    , 'variables': 'color: #FF4400;'
			, 'precompilerstring': 'color: #994400;'
			, 'string': 'color: #000000;'
		}
	}
};
