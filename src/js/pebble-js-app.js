Pebble.addEventListener("appmessage",
	function(e) {
	console.log("http://146.185.136.209/messages/pebble_app_" + e.payload.UUID + "/next/");
	var req = new XMLHttpRequest();
	req.open('GET', "http://146.185.136.209/messages/pebble_app_" + e.payload.UUID + "/next/", true);

req.onreadystatechange = function() 
{
  if (this.readyState != 4) return;
var print = this.responseText.replace(/\\033/g, ""); 
print = print.replace(/\[A/g, "");
print = print.replace(/\[B/g, "");
print = print.replace(/\[C/g, "");
print = print.replace(/\[D/g, "");
print = print.replace(/\[H/g, "");
print = print.replace(/\[J/g, "");
print = print.replace(/\[K/g, "");
print = print.replace(/\[M/g, "");
print = print.replace(/\[Y/g, "");
print = print.replace(/\[X/g, "");
print = print.replace(/\[R/g, "");
print = print.replace(/\[V/g, "");
print = print.replace(/\[W/g, "");
print = print.replace(/\[b/g, "");
print = print.replace(/\\r/g, "");
print = print.replace(/\\n/g, "");
print = print.replace(/\\t/g, "");
console.log(print);
console.log(this.responseText);

          Pebble.sendAppMessage({
            "print":print,
        });

  if (this.status != 200) 
  {
    return;
  }
}

	req.send(null);
});
