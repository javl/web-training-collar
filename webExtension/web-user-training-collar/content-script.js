// Check if the protocol is NOT "https" AND NOT "about"
if(location.protocol !== "https:" && location.protocol !== "about:"){
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function() {
		if (xmlhttp.readyState == XMLHttpRequest.DONE ) { // same as XMLHttpRequest.DONE == 4
			if(xmlhttp.status != 200){ // If response is NOT ok, show error in console
				console.log("Web Training Collar xmlhttp.status error: " + xmlhttp.status);
			}
		}
	}
	// Request a shock
	xmlhttp.open("GET", "http://127.0.0.1:3001/shock", true);
	xmlhttp.send();
}