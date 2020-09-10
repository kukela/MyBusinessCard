// 返回文件
void replyFile(String path) {
  String contentType = mime::getContentType(path);
  Serial.println(path + " - " + contentType);
  
  if (!SPIFFS.exists(path)) {
    replyServerCode(204);
    return;
  }
  file = SPIFFS.open(path, "r");
  configSendHeader();
  if (webServer.streamFile(file, contentType) != file.size()) {
    Serial.println("File Not Found " + path);
    webServer.send(404, FPSTR(TEXT_PLAIN), "File Not Found: " + path);
  }
  file.close();
}

//配置请求头
void configSendHeader() {
  webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webServer.sendHeader("Pragma", "no-cache");
  webServer.sendHeader("Expires", "-1");
}

// 请求错误返回
void replyServerError(String msg) {
  webServer.send(500, FPSTR(TEXT_PLAIN), msg + "\r\n");
}

// 返回code
void replyServerCode(int code) {
  // 空内容会禁止Content-length标头，所以需要停止，因为我们没有发送内容长度
  webServer.send(code);
  webServer.client().stop();
}

// 设置信道
void setChangeAP() {  
  uint8_t v = getChangeAp();
  Serial.println("channel: " + String(v));
  WiFi.softAP(MSSID, MPSK, v);
}

// 获取信道
uint8_t getChangeAp() {
  uint8_t v = EEPROM.read(channelAddr);
  if (v < 1 || v > 14) {
    v = 7;
  }
  return v;
}

// 获取首页地址
String getHomeUrl() {
  for (int i = 0; i < sizeof(homeUrl); i++) {
    homeUrl[i] = EEPROM.read(homeUrlStartAddr + i);
  }
  if (homeUrl[0] == 0) {
    strcpy(homeUrl, "index.html");
  }
  return homeUrl;
}

// 是IP吗
boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

// IP转字符串
String ip2String(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}

// 如果我们收到另一个域的请求，重定向到专属门户。在这种情况下返回true，这样页面处理程序就不会再次尝试处理请求。
boolean captivePortal() {
  if (!isIp(webServer.hostHeader()) && webServer.hostHeader() != (String(myHostname) + ".local")) {
    Serial.println("请求重定向到 captive portal");
    webServer.sendHeader("Location", String("http://") + ip2String(webServer.client().localIP()), true);
    replyServerCode(302);
    return true;
  }
  return false;
}

// led类型
void ledLightType() {
  tickerIndex++;
  if (isWLLed && tickerIndex == 1) {
    tickerIndex++;
  }
  switch (tickerIndex) {
    case 1:
      ledLight(0);
      break;
    case 2:
      if (isLightLed1) {
        ledLight(1);
      }
      break;
    case 3:
      if (isLightLed2) {
        ledLight(2);
      }
      tickerIndex = 0;
      break;
  }
}

// led操作
void ledLight(uint8_t type) {
  switch (type) {
    case 0:
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
      break;
    case 1:
      digitalWrite(L1, HIGH);
      digitalWrite(L2, LOW);
      break;
    case 2:
      digitalWrite(L1, LOW);
      digitalWrite(L2, HIGH);
      break;
  }
}
