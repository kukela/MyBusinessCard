var httpRequest = new XMLHttpRequest();

var jt = getDomById("jt");
jt.innerText = "JSTest";
var led1 = getDomById("led1");
var led2 = getDomById("led2");
var wl = getDomById("wl");
var off = getDomById("off");
var channel = getDomById("channel");
var ti = new Date().getTime();

window.onload = function() {
	var t = new Date().getTime() - ti;
	jt.innerText = "Load time: " + t + "ms";
}

function mClick(e) {
	switch (e.id) {
		case "led1":
		case "led2":
			wl.checked = false;
			off.checked = false;
			break;
		case "wl":
			led1.checked = true;
			led2.checked = true;
			break;
		case "off":
			led1.checked = false;
			led2.checked = false;
			break;
	}
	if (led1.checked == false && led2.checked == false) {
		off.checked = true;
	}
	var type = [];
	type[0] = led1.checked == true ? 1 : 0;
	type[1] = led2.checked == true ? 1 : 0;
	type[2] = wl.checked == true ? 1 : 0;
	requestGet("/led?type=" + type.join(""), function() {
		if (httpRequest.readyState != 4) {
			return;
		}
		if (httpRequest.status != 200) {
			return;
		}
	});
}

function cChnage() {
	requestGet("/channel?v=" + channel.value, function() {
		if (httpRequest.readyState != 4) {
			return;
		}
		if (httpRequest.status != 200) {
			return;
		}
	});
}

requestGet("/getChannel", function() {
	if (httpRequest.readyState != 4 || httpRequest.status != 200) {
		return;
	}
	var text = httpRequest.responseText;
	if (!isNaN(text)) {
		channel.value = text;
	}
});

//get请求
function requestGet(url, f) {
	httpRequest.open("GET", url, true);
	httpRequest.onreadystatechange = f
	httpRequest.send();
};

function getDomById(name) {
	return document.getElementById(name);
}
