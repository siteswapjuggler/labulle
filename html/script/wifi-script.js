function togglePassword() {
	pwd = document.getElementById("pwd");
	view = document.getElementById("view");
	
	if (pwd.getAttribute("type") == "password") {
		pwd.setAttribute("type","text");
		view.classList.remove("icon-eye-closed");
		view.classList.add("icon-eye");
	}
	else {
		pwd.setAttribute("type","password");
		view.classList.remove("icon-eye");
		view.classList.add("icon-eye-closed");
	}
}

function scanNetwork(prescan) {
  var i, html, options, xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			if (this.response === "false") {
				html += "<option disabled selected>"+translation[lang]['wifi-form-network-nowifi']+"</option>";
			}
			else {
				html += "<option disabled selected>"+translation[lang]['wifi-form-network-selector']+"</option>";
				options = this.response.split(",");
				for (i=0; i < options.length; i++) {
					html += "<option>"+options[i]+"</option>";
				}
			}
			document.getElementById("ssid").innerHTML = html;
		}
  };
  xhttp.open("GET", prescan ? "./prescan" : "./scan", true);
  xhttp.send();
}

scanNetwork(true);