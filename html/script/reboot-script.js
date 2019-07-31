function reboot() {
	document.addEventListener("DOMContentLoaded", function(event) { 
		var xhttp = new XMLHttpRequest();
		xhttp.open("GET", "./reboot", true);
		xhttp.send();
	});
}

reboot();