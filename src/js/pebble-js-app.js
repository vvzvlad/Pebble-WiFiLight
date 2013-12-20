Pebble.addEventListener("appmessage",
	function(e) {
	console.log("http://" + e.payload.ip + ":" + e.payload.port + "/api.php?channel=" + e.payload.channel + "&command=" + e.payload.command);
	var req = new XMLHttpRequest();
	req.open('GET', "http://" + e.payload.ip + ":" + e.payload.port + "/api.php?channel=" + e.payload.channel + "&command=" + e.payload.command, true);
	req.send(null);
});


