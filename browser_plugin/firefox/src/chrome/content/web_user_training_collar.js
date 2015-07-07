(function() {

	// Function to make string into a hash so the actual URL doesn't need to be sent.
	// Not perfect; hostnames can result in have the same number, but not very likely
	// to happen (often). Got it from stackoverflow.com/questions/7616461
	String.prototype.hashCode = function() {
		var hash = 0, i, chr, len;
		if (this.length == 0) return hash;
		for (i = 0, len = this.length; i < len; i++) {
			chr   = this.charCodeAt(i);
			hash  = ((hash << 5) - hash) + chr;
			hash |= 0; // Convert to 32bit integer
		}
		return hash;
	};

	// Send a hashed version of the hostname to the local server. The server will check
	// if it needs to send a shock or do something else.
	function sendHostname(hostname) {
		// The request needs some lines of code, but still better than loading the whole
		// jQuery or whatever library just for an ajax call
		var xmlhttp = new XMLHttpRequest();
		xmlhttp.onreadystatechange = function() {
			if (xmlhttp.readyState == XMLHttpRequest.DONE ) { // XMLHttpRequest.DONE == 4
				if(xmlhttp.status != 200){ // If response is NOT ok, show error in console
					console.log("Web Training Collar xmlhttp.status error: " + xmlhttp.status);
				}
			}
		}
		// Send hashed version of hostname to local server.
		xmlhttp.open("GET", "http://127.0.0.1:3001/hostname/"+hostname.hashCode()+"/", true);
		xmlhttp.send(); // Send the request
	}

	// Runs when page is loaded. Might be able to replace it with a listener
	// that gets fired sooner?
	function windowLoadHandler() {
		// Next line is from example plugin (cloud to butt). Don't know why needed, but Left it in.
		window.removeEventListener('load', windowLoadHandler);
		document.getElementById('appcontent').addEventListener('DOMContentLoaded', function(e) {
			protocol = content.location.protocol;
			hostname = content.location.hostname;

			// Allow https: and about: protocol (about:blank, about:config, etc.)
			if(protocol != 'https:' && protocol != 'about:'){
				sendHostname(hostname);
			}
		});
	}
	// Attach the load event to the window
	window.addEventListener('load', windowLoadHandler);
}());