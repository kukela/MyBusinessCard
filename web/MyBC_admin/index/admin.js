var mf = getDomById("mf");
var state = getDomById("state");
var channel = getDomById("channel");
var cache = getDomById("cache");
var homeUrl = getDomById("homeUrl");
var ssid = getDomById("ssid");
var pwd = getDomById("pwd");
var scan = getDomById("scan");
var wificonn = getDomById("wificonn");
var hip = getDomById("hip");
var vup = getDomById("vup");
var wup = getDomById("wup");
var fd = getDomById("file");
var fdl = getDomById("fdl");

var ssidV = "";
var wifistate = "";
// 版本
var ver = "",
	nver = "",
	wver = "",
	nwver = "";

btnReset();

reqScanSsid();
reqVersion();
reqConfig();
reqInfo();

// 刷新ssid
function reqScanSsid(bc) {
	disWifiConnBtn(true);
	ssid.innerHTML = "";
	requestGet("/wifiscan", function() {
		if (isReqError(this)) {
			return;
		}
		refreshSsid(req2Json(this));
		reqWificonf();
	});
}

// 刷新选择Wi-Fi
function refreshSsid(list) {
	list = list.reverse();
	var tStr = "<option value=''>请选择 Wi-Fi</option>";
	for (var i = 0; i < list.length; i++) {
		var item = list[i];
		tStr += "<option value='";
		tStr += item.ssid;
		tStr += "'>";
		tStr += item.ssid + "&emsp;" + item.rssi;
		tStr += "</option>";
	}
	ssid.innerHTML = tStr;
	ssid.value = ssidV;
	disWifiConnBtn(false);
}

// 获取wifi配置信息
function reqWificonf() {
	requestGet("/wificonf", function() {
		if (isReqError(this)) return;
		var json = req2Json(this);
		ssid.value = ssidV = json.ssid;
		pwd.value = json.pwd;
		if (ssidV.length == 0 || pwd.value.length == 0) {
			WCBtnReset();
			return;
		}
		if (json.ip.length > 0) {
			disWifiConnBtn(true);
			hip.setAttribute("href", 'http://' + json.ip);
			hip.innerHTML = json.ip;
			WCBtnType(3);
			reqNVersion();
		} else {
			WCBtnType(2);
			disWifiConnBtn(false);
		}
	});
}

// 连接Wi-Fi
function reqConnSsid() {
	WCBtnType(1);
	disWifiConnBtn(true);
	var sv = ssid.value;
	var pv = pwd.value;
	var u = "/wificonn?s=" + sv + "&p=" + pv;
	requestGet(u, function() {
		if (isReqError(this)) return;
		reqWificonnSt();
	});
}

// 获取Wi-Fi连接状态
function reqWificonnSt() {
	defSetTimeout(function() {
		requestGet("/wificonn?st=1", function() {
			if (isReqError2(this)) return;
			var s = this.responseText;
			if (s == "4") {
				WCBtnType(2);
				disWifiConnBtn(false);
				return;
			} else if (s == "3") {
				reqWificonf();
				return;
			}
			reqWificonnSt();
		});
	})
}

// 获取版本信息
function reqVersion() {
	requestGet("/version", function() {
		if (isReqError(this)) return;
		var json = req2Json(this);
		ver = json.ver;
		wver = json.wver;
		refreshVersion();
	});
}

function reqNVersion() {
	requestGet("/nVersion", function() {
		if (isReqError(this)) return;
		var json = req2Json(this);
		nver = json.nv;
		nwver = json.nw;
		refreshVersion();
		refreshUpBtns();
	});
}

function refreshVersion() {
	var s = "固件版本：" + ver;
	s += "<br>";
	s += "最新版本：" + nver;
	getDomById("ver").innerHTML = s;
	s = "网站版本：" + wver;
	s += "<br>";
	s += "最新版本：" + nwver;
	getDomById("wver").innerHTML = s;
}

// 固件升级
function reqUpdate(t) {
	requestGet("/update?t=" + t, function() {
		if (isReqError(this)) return;
		disUpdateDoms(true);
		reqProgress(t);
		UpBtnType(1, t);
	});
}

// 更新进度
function reqProgress(t) {
	defSetTimeout(function() {
		requestGet("/progress", function() {
			if (isReqError2(this)) return;
			var v = this.responseText;
			if (v == "100") {
				UpBtnType(3, t);
				disUpdateDoms(false);
				wlReload();
				return;
			} else if (v == "255") {
				UpBtnType(2, t);
				disUpdateDoms(false);
				return;
			}
			if (v.length > 0) {
				UpBtnType(4, t, v);
			}
			reqProgress(t);
		});
	})
}

// 获取配置信息
function reqConfig() {
	requestGet("/config", function() {
		if (isReqError(this)) {
			return;
		}
		var json = req2Json(this);
		channel.value = json["channel"];
		cache.checked = json["cache"];
		homeUrl.value = json["homeUrl"];
	});
}

// 修改信道
function cChnage() {
	reqPutConfig("c=" + channel.value);
}

// 开启缓存
function caChange() {
	reqPutConfig("ca=" + cache.checked);
}

// 修改首页地址
function huChange() {
	reqPutConfig("hu=" + homeUrl.value);
}

// 修改配置
function reqPutConfig(v) {
	requestGet("/putConfig?" + v, function() {});
}

// 重置所有配置
function resetConf() {
	requestGet("/is?v=1", function() {
		wlReload();
	});
}

//获取文件数据
function reqInfo() {
	requestGet("/info", function() {
		if (isReqError(this)) {
			return;
		}
		var json = req2Json(this);
		// var json = {
		// 	"totalBytes": 2949250,
		// 	"usedBytes": 0,
		// 	"maxPathLength": 32, //最大文件路径长度
		// 	"fList": [{ //文件列表
		// 		"fn": "index.html",
		// 		"fs": 256
		// 	}]
		// }
		refreshPage(json);
	});
};

//刷新文件页面
function refreshPage(json) {
	var cap = getDomById("cap");
	cap.innerText = bytes2CapStr(json.totalBytes, 1);
	var list = json.fList;
	var tBodyStr = "";
	// var yy = 0;
	for (var i = 0; i < list.length; i++) {
		var item = list[i];
		tBodyStr += "<tr";
		if (i % 2) {
			tBodyStr += " class='drak'";
		}
		tBodyStr += ">";
		tBodyStr += "<td>" + item.fn + "</td>";
		tBodyStr += "<td>" + bytes2CapStr(item.fs, 1) + "</td>";
		tBodyStr += "</tr>";
		// yy += json.fnSize + item.fs;
	}
	var tBody = getDomById("body");
	tBody.innerHTML = tBodyStr;

	var sy = getDomById("sy");
	sy.innerText = bytes2CapStr(json.usedBytes, 1);
};

//文件按钮改变
function fileInputChange() {
	disUpdateDoms(true);
	showState("正在格式化...");
	requestGet("/is?v=0", function() {
		if (isReqError(this)) {
			showState("格式化错误...");
			disUpdateDoms(false);
			return;
		}
		disUpdateDoms(true);
		disDom(fd, false);
		showState("正在上传...");
		var fls = fd.files;
		var fdn = "";
		for (var i = 0; i < fls.length; i++) {
			fdn += fls[i].size;
			if (i != fls.length - 1) {
				fdn += ",";
			}
		}
		fd.name = fdn;
		getDomById("fileForm").submit();
	})
};

if (mf.attachEvent) { //IE
	mf.attachEvent("onload", reload);
} else {
	mf.onload = reload;
}

function reload() {
	var doc = mf.contentDocument || mf.contentWindow.document;
	var mfb = doc.getElementsByTagName("body")[0];
	if (mfb.innerHTML.toString().indexOf("成功") != -1) {
		wlReload();
		showState("上传成功...");
		return;
	}
	showState(mfb.innerHTML);
}

//显示状态
function showState(msg, isDelayHide) {
	state.style.display = "block";
	state.innerHTML = msg;
}
