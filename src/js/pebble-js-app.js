Pebble.addEventListener("ready",
function(e) {
console.log("connect!" + e.ready + e.type);
});

Pebble.addEventListener("appmessage",
	function(e) {
	console.log("message:" + e.payload.channel + e.payload.command);
	var req = new XMLHttpRequest();
	if (e.payload.command) {
			req.open('GET', "http://192.168.1.178/api.php?channel=" + e.payload.channel + "&command=on", true);
	}
	else
	{
			req.open('GET', "http://192.168.1.178/api.php?channel=" + e.payload.channel + "&command=off", true);
	}
	req.send(null);
});


