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
  json.reserve(1024);
  json = "{\"totalBytes\":";
  json += fs_info.totalBytes;
  json += ",\"usedBytes\":";
  json += fs_info.usedBytes;
  json += ",\"maxPathLength\":";
  json += maxPathLength;
  json += ",\"fList\":[";

  Serial.print("blockSize:");
  Serial.println(fs_info.blockSize);
  Serial.print("pageSize:");
  Serial.println(fs_info.pageSize);
  Serial.print("totalBytes:");
  Serial.println(fs_info.totalBytes);
  Serial.print("usedBytes:");
  Serial.println(fs_info.usedBytes);

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
  webServer.send(200, APP_JSON, json);
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
          filename =  filename.substring(fsPointion, filename.length());
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

// 指令集
void handleIS() {
  ticker.detach();
  
  int v = webServer.arg("v").toInt();
  switch(v) {
    case 0: { // 格式化SPIFFS
      if(clearFS()) {
        webServer.send(200, FPSTR(TEXT_PLAIN), "1");
      } else {
        webServer.send(500, FPSTR(TEXT_PLAIN), "0");
      }
      break;
    }
    case 1: { // 清除EEPROM
      for (int i = 0; i < eepromSize; i++) {
        EEPROM.write(i, 0);
      }
      EEPROM.end();
      EEPROM.begin(eepromSize);
      replyServerCode(200);
    }
  }
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
  json.reserve(76);
  json = "{\"channel\":";
  json += getChangeAp();
  json += ",\"homeUrl\":\"";
  json += getHomeUrl();
  json += "\",\"cache\":";
  json += isCache();
  json += "}";
  webServer.send(200, APP_JSON, json);
}

// 设置配置
void handlePutConfig() {
  ticker.detach();
  
  uint8_t i = 0;
  String v = webServer.arg("c");
  Serial.println("channel: " + v);
  if (!v.isEmpty()) {
    i = v.toInt();
    EEPROM.write(channelAddr, i);
    EEPROM.commit();
    setChangeAP();
  }
  
  v = webServer.arg("hu");
  Serial.println("homeUrl: " + v);
  if (!v.isEmpty()) {
    v.toCharArray(homeUrl, sizeof(homeUrl));
    EEPROM.put(homeUrlStartAddr, homeUrl);
    EEPROM.commit();
  }
  
  v = webServer.arg("ca");
  Serial.println("cache: " + v);
  if (!v.isEmpty()) {
    i = v == "true" ? 1 : 0;
    EEPROM.write(changeAddr, i);
    EEPROM.commit();
  }  
  
  replyServerCode(200);
}

// 返回 Wi-Fi 扫描结果
void handleWifiscan() {
  ticker.detach();

  Serial.println("scan start");
  int n = WiFi.scanNetworks();
  Serial.println("scan done");

  String json;
  json.reserve(1024);
  json += "[";
  for (int i = 0; i < n; i++) {
    if (i != 0) {
      json += ",";
    }
    json += "{\"ssid\":\"";
    json += WiFi.SSID(i);
//    json += "\",\"mac\":\"";
//    json += WiFi.BSSIDstr(i);
    json += "\",\"rssi\":";
    json += WiFi.RSSI(i);
    json += "}";
  }
  json += "]";
  webServer.send(200, APP_JSON, json);
}

// 连接 Wi-Fi
void handleWificonn() {
  ticker.detach();

  String v = webServer.arg("st"); 
  if (v == "1") {
    webServer.send(200, FPSTR(TEXT_PLAIN), String(status));
    return;
  }

  v = webServer.arg("s");
  Serial.println("conn to " + v);
  if (!v.isEmpty()) {
    v.toCharArray(ssid, sizeof(ssid));
  }
  v = webServer.arg("p");
  if (!v.isEmpty()) {
    v.toCharArray(password, sizeof(password));
  }
  saveWifi();

  status = WL_IDLE_STATUS;
  connect = true;
  replyServerCode(200);
}

// 返回 Wi-Fi 配置信息
void handleWificonf() {
  ticker.detach();

  getEEPROM();
  String json;
  json.reserve(128);
  json += "{\"ssid\":\"";
  json += ssid;
  json += "\",\"pwd\":\"";
  json += password;
  json += "\",\"ip\":\"";
//  json += (!connect && strlen(ssid) > 0) ? true : false;
  if (status == 3) {
    json += WiFi.localIP().toString();
  }
  json += "\"}";
  webServer.send(200, APP_JSON, json);
}

// 返回版本

void handleVersion() {
  ticker.detach();

  String json;
  json.reserve(32);
  json += "{\"ver\":";
  json += ver;
  json += ",\"wver\":\"";
//  json += "200914";
  String p = "/v.txt";
  if (SPIFFS.exists(p)) {
    file = SPIFFS.open(p, "r");
    if (file) {
      file.readBytes(wver, 6);
      json += wver;
    }
  }
  json += "\"}";
  webServer.send(200, APP_JSON, json);
}

// 返回最新版本
void handleNVersion() {
  ticker.detach();

  HTTPClient http;
  if (http.begin(client, "http://kukela-bin.oss-cn-shanghai.aliyuncs.com/mybc/ver.json")) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        webServer.send(200, APP_JSON, http.getString());
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      replyServerCode(500);
    }
    http.end();
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
    replyServerCode(500);
  }
  replyServerCode(200);
}
