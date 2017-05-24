function LoadStuff() {
	backend.tryThis();
}

function mergeJSON(source1,source2){
    var mergedJSON = Object.create(source2);
    for (var attrname in source1) {
	if (mergedJSON.hasOwnProperty(attrname)) {
		if (source1[attrname]!=null && source1[attrname].constructor==Object) {
			mergedJSON[attrname] = mergeJSON(source1[attrname], mergedJSON[attrname]);
		} 
	} else {
		mergedJSON[attrname] = source1[attrname];
	}
    }
    return mergedJSON;
}

