window.onload=function() {
	var adiv = document.getElementById("comp");
	var mres = {};
	adiv.innerHTML = "Loading...";
	backend.loadComponent(function(t) {
		var resps = t.responseObject();
		console.log(resps);
		for (var t=0;t<resps.length;t++) {
			console.log("resps["+t+"]="+resps[t]);
			var project = resps[t];
			console.log("project="+project);
			t++;
			console.log("resps["+t+"]="+resps[t]);
			var d = JSON.parse(resps[t]);
			if (d.success) {
				for (var q=0;q<d.result.length;q++) d.result[q].project = project;
				mres = mergeJSON(mres,d);
			} else {
				console.log(d.error);
			}
		}
		console.log("mres = ");
		console.log(mres);
		var d = mres.result;
		var td="<table class=\"stripped-odd\"><tr><td><i>Name:</i></td><td>"+d.name+"</td></tr>";
		td+="<tr><td><i>Summary:</i></td><td>"+d.summary+"</td></tr>";
		td+="<tr><td><i>Domain:</i></td><td>"+d.domain+"</td></tr>";
		var owner="N/A";;
		if (typeof d.owneruser != "undefined") {
			owner=d.owneruser;
		} 
		if (typeof d.ownergroup != "undefined") {
			owner = d.ownergroup;
		}
		td+="<tr><td><i>Owner:</i></td><td>"+owner+"</td></tr>";
		var bp="<a href=\"../../../job/"+d.project+"\">"+d.project+"</a>";
		var lb="<a href=\"../../../job/"+d.project+"/"+d.lastbuild+"\">#"+d.lastbuild+"</a>";
		td+="<tr><td><i>Build Project:</i></td><td>"+bp+"</td></tr>";
		td+="<tr><td><i>Last Build:</i></td><td>"+lb+"</td></tr>";
		td+="</table>";
		console.log("td="+td);
		adiv.innerHTML = td;
	});
}

