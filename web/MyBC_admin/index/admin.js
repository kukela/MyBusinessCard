var httpRequest = new XMLHttpRequest();
var mf = getDomById("mf");
var state = getDomById("state");
var channel = getDomById("channel");

getInfo(function() {
	getChannel();
});

//获取信道数据
function getChannel() {
	requestGet("/getChannel", function() {
		if (httpRequest.readyState != 4 || httpRequest.status != 200) {
			return;
		}
		var text = httpRequest.responseText;
		if (!isNaN(text)) {
			channel.value = text;
		}
	});
}

//获取文件数据
function getInfo(bc) {
	requestGet("/info", function() {
		if (httpRequest.readyState != 4 || httpRequest.status != 200) {
			return;
		}
		var jsonStr = httpRequest.responseText;
		var json = JSON.parse(jsonStr);
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
		bc();
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

//选择信道改变
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

//文件按钮改变
function fileInputChange() {
	showState("正在格式化...");
	requestGet("/erase", function() {
		if (httpRequest.readyState != 4) {
			return;
		}
		if (httpRequest.status != 200) {
			showState("格式化错误...");
			return;
		}
		showState("正在上传...");
		var fd = getDomById("file");
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

//状态点击
function stateClick() {
	state.style.display = "none";
}

if (mf.attachEvent) { //IE
	mf.attachEvent("onload", reload);
} else {
	mf.onload = reload;
}

function reload() {
	var doc = mf.contentDocument || mf.contentWindow.document;
	var mfb = doc.getElementsByTagName("body")[0];
	if (mfb.innerHTML.toString().indexOf("成功") != -1) {
		window.location.reload();
		showState("上传成功...");
		return;
	}
	showState(mfb.innerHTML);
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

//get请求
function requestGet(url, f) {
	httpRequest.open("GET", url, true);
	httpRequest.onreadystatechange = f
	httpRequest.send();
};

function showState(msg, isDelayHide) {
	state.style.display = "block";
	state.innerHTML = msg;
}

function getDomById(name) {
	return document.getElementById(name);
}
