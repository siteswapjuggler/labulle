//------------------------- WebSocket ----------------------------------------

var connection = new WebSocket('ws://'+location.hostname+':81/');
//var connection = new WebSocket('ws://192.168.1.21:81/');

connection.onerror = function (error) {    
	console.log('WebSocket Error ', error);
};

connection.onmessage = function (e) {
	var items = e.data.split(',');
	//console.log(e.data);
	if (items[0] == "U") {		
		document.getElementById('temp-val').innerHTML=items[1];
		document.getElementById('humidity-val').innerHTML=items[2];
		document.getElementById('soil-val').innerHTML=items[3];
		document.getElementById('tank-val').innerHTML=items[4];
	}
	if (items[0] == "T") {
		document.getElementById('water-toggle').checked=items[1]==1 ? true : false;
		document.getElementById('servo-toggle').checked=items[2]==1 ? true : false;
		document.getElementById('fan-toggle').checked=items[3]==1 ? true : false;
		document.getElementById('light-toggle').checked=items[4]==1 ? true : false;
	}
	if (items[0] == "P") {
		document.getElementById('air-threshold').value=items[1];
		showAir(document.getElementById('air-threshold'),false);
		document.getElementById('water-threshold').value=items[2];
		showWater(document.getElementById('water-threshold'),false);
		document.getElementById('start-time').value=items[3];
		showStart(document.getElementById('start-time'),false);
		document.getElementById('end-time').value=items[4];
		showEnd(document.getElementById('end-time'),false);
		document.getElementById('sleep-time').value=items[5];
		showSleep(document.getElementById('sleep-time'),false);
	}
};

connection.onopen = function () {
	connection.send('Connect ' + new Date()); 
	getSensors();
	getToggle();
	getParameters();
};

//-------------------- Mise à jour des capteurs -------------------------------

function getSensors() {
	connection.send('U0');
}

function getToggle() {
	connection.send('T0');
}

function getParameters() {
	connection.send('P0');
}

setInterval(function() {
	getToggle();
	getParameters();
}, 50);

setInterval(function() {
  getSensors();
}, 2000);

//-------------- Actualisation des données à l'écran --------------------------

function showAir(s, send) {
	document.getElementById('air-threshold-output').innerHTML = Math.round(s.value)+" °C";
	if (send) connection.send('A'+s.value);	
}

function showWater(s, send) {
	document.getElementById('water-threshold-output').innerHTML = Math.round(s.value)+" %";
	if (send) connection.send('W'+s.value);
}

function showStart(s, send) {
	text =  (s.value < 10) ? "0" : "";
	text += Math.floor(s.value);
	text += (s.value == Math.floor(s.value)) ? "h00" : "h30";
	document.getElementById('start-output').innerHTML = text;
	if (send) connection.send('S'+s.value);
}

function showEnd(s, send) {
	text = Math.floor(s.value);
	text += (s.value == Math.floor(s.value)) ? "h00" : "h30";
	document.getElementById('end-output').innerHTML = text;
	if (send) connection.send('E'+s.value);
}

function showSleep(s, send) {
	if (s.value == 0)
		text = "-";
	else {
	text =  (s.value < 10) ? "0" : "";
	text += Math.floor(s.value) + "h00";
	}
	document.getElementById('sleep-output').innerHTML = text;
	if (send) connection.send('D'+s.value);
}

function saveParameters() {
	connection.send('Z0');
}

//----------------------------- Switch -----------------------------------------

function waterToggle(s) {
	val = (s.checked ? 1 : 0);
	connection.send('w'+val);	
}

function servoToggle(s) {
	val = (s.checked ? 1 : 0);
	connection.send('s'+val);	
}

function fanToggle(s) {
	val = (s.checked ? 1 : 0);
	connection.send('f'+val);	
}

function lightToggle(s) {
	val = (s.checked ? 1 : 0);
	connection.send('l'+val);	
}