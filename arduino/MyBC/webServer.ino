// 首页
void handleRoot() {
  if (captivePortal()) {
    return;
  }
  replyFile(getHomeUrl());
}

// 管理页面
void handleAdmin() {
  configSendHeader();
  webServer.send(200, "text/html", adminHTML);
}

// 请求文件
void handleNotFound() {
  if (captivePortal()) {
    return;
  }
  String path = webServer.uri();
  if (path.charAt(0) == '/') {
    path = path.substring(1, path.length());
  }
  replyFile(path);
}

// 获取设备信息
void handleGetInfo() {
  FSInfo fs_info;
  if (!SPIFFS.info(fs_info)) {
    webServer.send(500, FPSTR(TEXT_PLAIN), "fs info error");
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

// 上传文件
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

// 格式化
void handleErase() {
  dir = SPIFFS.openDir("");
  while (dir.next()) {
    if (!SPIFFS.remove(dir.fileName())) {
      webServer.send(500, FPSTR(TEXT_PLAIN), "0");
      return;
    }
  }
  webServer.send(200, FPSTR(TEXT_PLAIN), "1");
}

// led操作
void handleLed() {
  ticker.detach();
  String type = webServer.arg("type");
  Serial.println("led: " + type);
  replyServerCode(200);

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

// 获取配置信息
void handleConfig() {
  String json;
  json.reserve(129);
  json = "{\"channel\":";
  json += getChangeAp();
  json += ",\"homeUrl\":\"";
  json += getHomeUrl();
  json += "\"}";
  webServer.send(200, "application/json", json);
}

// 设置配置
void handlePutConfig() {
  ticker.detach();
  String c = webServer.arg("c");
  Serial.println("channel: " + c);
  String hu = webServer.arg("hu");
  Serial.println("homeUrl: " + hu);
  replyServerCode(200);

  if (!c.isEmpty()) {
    uint8_t iv = c.toInt();
    EEPROM.write(channelAddr, iv);
    EEPROM.commit();
    setChangeAP();
  }
  if (!hu.isEmpty()) {
    hu.toCharArray(homeUrl, sizeof(homeUrl));
    for(uint8_t i = 0; i < sizeof(homeUrl); i++) {
      EEPROM.write(homeUrlStartAddr + i, homeUrl[i]);
    }
    EEPROM.commit();
  }
}
