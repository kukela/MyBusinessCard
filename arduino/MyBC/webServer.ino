static const char adminHTML[] PROGMEM = "<!DOCTYPE html><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width,initial-scale=1,minimum-scale=1,maximum-scale=1,user-scalable=no'><title>admin</title><style>body{padding:0;margin:0}header{height:60px}header h1{margin:0 20px 0 20px;line-height:60px;color:#303133}.a-upload{position:absolute;top:8px;right:20px;padding:0 12px;margin:0;height:44px;line-height:44px;cursor:pointer;color:#303133;background:#ebeef5;border:1px solid #e4e7ed;border-radius:4px;overflow:hidden;display:inline-block;text-decoration:none}.a-upload:hover{background:#e4e7ed;border-color:#ccc;text-decoration:none}.a-upload input{position:absolute;font-size:100px;right:0;top:0;opacity:0;cursor:pointer}.state-div{height:30px;margin-top:10px;padding:0 20px}.state-div>div{font-size:13px;color:#303133;line-height:30px}table{width:100%;border-collapse:collapse}#state,td{font-size:14px;color:#303133;line-height:28px}thead td{font-weight:700}tr td:first-letter{padding-left:20px}tr td:last-child{padding-left:20px}table .drak{background-color:#ebeef5}#state{margin:20px;display:none}</style></head><body><header><h1>文件管理</h1><form id='fileForm' method='POST' action='/update' enctype='multipart/form-data' target='mf'><a href='javascript:;' class='a-upload'>覆盖上传 <input id='file' type='file' onchange='fileInputChange()' webkitdirectory multiple='multiple'></a></form><iframe id='mf' name='mf' style='display:none'></iframe></header><div id='state' onclick='stateClick()'></div><div class='state-div'><div>总容量：<span id='cap'></span>；已用容量：<span id='sy'></span></div></div><table><thead class='drak'><tr><td>名称</td><td>大小</td></tr></thead><tbody id='body'></tbody></table><script>var httpRequest=new XMLHttpRequest,mf=getDomById(\"mf\"),state=getDomById(\"state\");function getInfo(){requestGet(\"http://www.hello.com/info\",function(){var t;4==httpRequest.readyState&&200==httpRequest.status&&(t=httpRequest.responseText,refreshPage(JSON.parse(t)))})}function refreshPage(t){getDomById(\"cap\").innerText=bytes2CapStr(t.totalBytes,1);for(var e=t.fList,n=\"\",o=0;o<e.length;o++){var s=e[o];n+=\"<tr\",o%2&&(n+=\" class='drak'\"),n+=\">\",n+=\"<td>\"+s.fn+\"</td>\",n+=\"<td>\"+bytes2CapStr(s.fs,1)+\"</td>\",n+=\"</tr>\"}getDomById(\"body\").innerHTML=n,getDomById(\"sy\").innerText=bytes2CapStr(t.usedBytes,1)}function fileInputChange(){showState(\"正在格式化...\"),requestGet(\"/erase\",function(){if(4==httpRequest.readyState)if(200==httpRequest.status){showState(\"正在上传...\");for(var t=getDomById(\"file\"),e=t.files,n=\"\",o=0;o<e.length;o++)n+=e[o].size,o!=e.length-1&&(n+=\",\");t.name=n,getDomById(\"fileForm\").submit()}else showState(\"格式化错误...\")})}function stateClick(){state.style.display=\"none\"}function reload(){var t=(mf.contentDocument||mf.contentWindow.document).getElementsByTagName(\"body\")[0];if(-1!=t.innerHTML.toString().indexOf(\"成功\"))return window.location.reload(),void showState(\"上传成功...\");showState(t.innerHTML)}function bytes2CapStr(t,e,n){switch(null==n&&(n=2),1==e&&t<1024&&(e=0),e){case 0:return t+\" B\";case 1:return(t/1024).toFixed(n)+\" KB\";case 2:return(t/1024/1024).toFixed(n)+\" MB\"}}function requestGet(t,e){httpRequest.open(\"GET\",t,!0),httpRequest.onreadystatechange=e,httpRequest.send()}function showState(t,e){state.style.display=\"block\",state.innerHTML=t}function getDomById(t){return document.getElementById(t)}getInfo(),mf.attachEvent?mf.attachEvent(\"onload\",reload):mf.onload=reload;</script></body></html>";
static const char TEXT_PLAIN[] PROGMEM = "text/plain;charset=UTF-8";

void replyServerError(String msg) {
  webServer.send(500, FPSTR(TEXT_PLAIN), msg + "\r\n");
}

//获取设备信息
void handleGetInfo() {
  FSInfo fs_info;
  if (!SPIFFS.info(fs_info)) {
    webServer.send(500, "fs info error");
    return;
  }
  maxPathLength = fs_info.maxPathLength;

  String json;
  json.reserve(512);

  json = "{\"totalBytes\":";
  json += fs_info.totalBytes;
  json += ",\"usedBytes\":";
  json += fs_info.usedBytes;
  json += ",\"maxPathLength\":";
  json += maxPathLength;
  json += ",\"fList\":[";

  dir = SPIFFS.openDir("");
  bool isFirst = true;
  while (dir.next()) {
    if (isFirst) {
      isFirst = false;
    } else {
      json += ",";
    }
    json += "{\"fn\":\"";
    json += dir.fileName();
    json += "\",\"fs\":";
    json += dir.fileSize();
    json += "}";
  }
  json += "]}";
  webServer.send(200, "application/json", json);
}

//上传文件
bool isError = false;
String fsSizeListStr = "";
int fsPointion = 0;
void handleFileUpload() {
  HTTPUpload& upload = webServer.upload();
  switch (upload.status) {
    case UPLOAD_FILE_START: {
        if (isError) {
          break;
        }
        String filename = upload.filename;
        fsPointion = filename.indexOf("/");
        if (fsPointion != -1) {
          filename =  filename.substring(fsPointion + 1, filename.length());
        }
        if (filename.length() > maxPathLength) {
          replyServerError("路径长度不能大于" + String(maxPathLength) + "字节！");
          isError = true;
          return;
        }

        String fsSize;
        if (fsSizeListStr == "") {
          Serial.println(upload.name);
          fsSizeListStr = upload.name;
        }

        fsPointion = fsSizeListStr.indexOf(',');
        if (fsPointion == -1) {
          fsSize = fsSizeListStr;
        } else {
          fsSize = fsSizeListStr.substring(0, fsPointion);
          fsSizeListStr = fsSizeListStr.substring(fsPointion + 1, fsSizeListStr.length());
        }

        Serial.print(filename);
        Serial.print(" ");
        Serial.print(fsSize);
        Serial.println();

        file = SPIFFS.open(filename, "w");
        break;
      }
    case UPLOAD_FILE_WRITE: {
        if (!file || isError) {
          break;
        }
        uint32_t bytesWritten = file.write(upload.buf, upload.currentSize);
        if (bytesWritten != upload.currentSize) {
          isError = true;
          return replyServerError("写入失败！");
        }
        break;
      }
    case UPLOAD_FILE_END: {
        if (!file) {
          break;
        }
        file.close();
        break;
      }
  }
}

void handleFileUploadAfter() {
  isError = false;
  fsSizeListStr = "";
  webServer.send(200, FPSTR(TEXT_PLAIN), "上传成功！");
}

//格式化
void handleErase() {
  //  if (!SPIFFS.format() || !SPIFFS.begin()) {
  //    webServer.send(500, "erase error");
  //    return;
  //  }
  dir = SPIFFS.openDir("");
  while (dir.next()) {
    if (!SPIFFS.remove(dir.fileName())) {
      webServer.send(500, FPSTR(TEXT_PLAIN), "0");
      return;
    }
  }
  webServer.send(200, FPSTR(TEXT_PLAIN), "1");
}

void handleRoot() {
  if (webServer.method() != HTTP_GET) {
    webServer.send(404);
    return;
  }
  String path = webServer.uri();
  //Serial.println("src " + path);
  if (path.charAt(0) == '/') {
    path = path.substring(1, path.length());
  }
  if (path.lastIndexOf(".") == -1) {
    path = "index.html";
  }
  if (!SPIFFS.exists(path)) {
    webServer.send(204);
    return;
  }
  String contentType = mime::getContentType(path);
  Serial.println(path + " " + contentType);

  file = SPIFFS.open(path, "r");
  if (webServer.streamFile(file, contentType) != file.size()) {
    Serial.println("error file " + path);
    webServer.send(404);
    return;
  }
  file.close();
}

void handleAdmin() {
  webServer.send(200, "text/html", adminHTML);
}

void handleLed() {
  ticker.detach();
  String type = webServer.arg("type");
  Serial.println("led: " + type);
  webServer.send(200);

  if (type.length() != 3) {
    isLightLed1 = false;
    isLightLed2 = false;
    isWLLed = false;
  } else {
    isLightLed1 = type.charAt(0) == '1';
    isLightLed2 = type.charAt(1) == '1';
    isWLLed = type.charAt(2) == '1';
  }

  tickerIndex = 0;
  if (isLightLed1 || isLightLed2) {
    ticker.attach_ms(isWLLed ? 666 : 5, ledLightType);
  } else {
    ticker.once_ms(1, ledLightType);
  }
}

void handleChannel() {
  ticker.detach();
  String v = webServer.arg("v");
  Serial.println("channel: " + v);
  webServer.send(200);

  uint8_t iv = v.toInt();
  EEPROM.write(channelAddr, iv);
  EEPROM.commit();
  WiFi.softAP("aMyCard", "", iv);
}

void handleGetChannel() {
  uint8_t v = getChangeAp();
  webServer.send(200, FPSTR(TEXT_PLAIN), String(v));
}
