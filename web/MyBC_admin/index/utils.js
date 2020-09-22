//get请求
function requestGet(url, f) {
	var httpRequest = new XMLHttpRequest();
	httpRequest.timeout = 10000;
	// url = "http://hello.local" + url;
	httpRequest.open("GET", url, true);
	httpRequest.onreadystatechange = f
	httpRequest.send();
};

// id转dom
function getDomById(name) {
	return document.getElementById(name);
}

// 请求转json
function req2Json(req) {
	try {
		var jsonStr = req.responseText;
		return JSON.parse(jsonStr);
	} catch (e) {
		return {};
	}
}

// 是否请求错误
function isReqError(req) {
	return isReqError2(req) || req.status != 200;
}

function isReqError2(req) {
	return req.readyState != 4;
}

//bytes转可读字符串
function bytes2CapStr(size, type, fixed) {
	if (fixed == null) {
		fixed = 2;
	}
	if (type == 1 && size < 1024) {
		type = 0;
	}
	switch (type) {
		case 0:
			return size + " B";
		case 1:
			return (size / 1024).toFixed(fixed) + " KB";
		case 2:
			return (size / 1024 / 1024).toFixed(fixed) + " MB";
	}
};

// 连接按钮状态
function WCBtnType(v) {
	var s = "";
	wifistate = "";
	switch (v) {
		case 1:
			s = "连接中...";
			break;
		case 2:
			s = "连接失败！点击重试";
			break;
		case 3:
			s = "连接成功";
			wifistate = "3";
			break;
		default:
			s = "连 接";
			break;
	}
	wificonn.innerText = s;
	refreshUpBtns();
}

// 重置按钮
function btnReset() {
	disWifiConnBtn(false);
	WCBtnType(0);
	UpBtnType(0, 0);
	UpBtnType(0, 1);
}

//是否冻结连接Wi-Fi按钮
function disWifiConnBtn(isDis) {
	disDom(wificonn, isDis);
}

// 冻结dom
function disDom(dom, isDis) {
	if (isDis) {
		dom.setAttribute("disabled", "");
	} else {
		dom.removeAttribute("disabled");
	}
}

// 冻结升级相关控件
function disUpdateDoms(isDis) {
	var ls = [ssid, pwd, scan, vup, wup, channel, fd, fdl];
	for (var i = 0; i < ls.length; i++) {
		disDom(ls[i], isDis);
	}
}

// 刷新升级按钮状态
function refreshUpBtns() {
	var isDis = wifistate != "3";
	disDom(vup, isDis);
	disDom(wup, isDis);
	if (isDis) return;
	if (nver != null && ver == nver) {
		disDom(vup, true);
	}
	if (nwver != null && wver == nwver) {
		disDom(wup, true);
	}
}

// 升级状态
function UpBtnType(v, bt, d) {
	var s = "";
	if (bt == 0) {
		s = "升级";
	} else {
		s = "更新";
	}
	switch (v) {
		case 1:
			s += "中...";
			break;
		case 2:
			s += "失败";
			break;
		case 3:
			s += "成功";
			break;
		case 4:
			s = d + "%";
			break;
		default:
			var t = bt == 0 ? "固件" : "网站";
			s = t + s;
			break;
	}
	if (bt == 0) {
		vup.innerText = s;
	} else {
		wup.innerText = s;
	}
}

//延时操作
function defSetTimeout(bc) {
	setTimeout(bc, 1000);
}

//刷新网页
function wlReload() {
	window.location.reload();
}
