//配置请求头
void configSendHeader() {
  if (isCache()) {
    webServer.sendHeader("Cache-Control", "max-age=691200, must-revalidate");
  } else {
    webServer.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  }
}

// 请求错误返回
void replyServerError(String msg) {
  webServer.send(500, FPSTR(TEXT_PLAIN), msg + "\r\n");
}

// 没有找到资源
void replyServerNotFound(String msg) {
  Serial.println("File Not Found " + msg);
  webServer.send(404, FPSTR(TEXT_PLAIN), "File Not Found: " + msg);
}

// 返回code
void replyServerCode(int code) {
  // 空内容会禁止Content-length标头，所以需要停止，因为我们没有发送内容长度
  webServer.send(code);
  webServer.client().stop();
}

// 获取首页地址
String getHomeUrl() {
  EEPROM.get(homeUrlStartAddr, homeUrl);
  if (homeUrl[0] == 0) {
    strcpy(homeUrl, "/index.html");
  }
  return homeUrl;
}

// 是否打开缓存
bool isCache() {
  uint8_t v = EEPROM.read(changeAddr);
  return v != 0;
}

// 当前请求是否在host
bool isHost() {
  String hh = webServer.hostHeader();
  return hh == apIP.toString() || hh == WiFi.localIP().toString() ||
         hh == (String(myHostname) + ".com") || hh == (String(myHostname) + ".local");
}

//获取EEPROM中的数据
void getEEPROM() {
  EEPROM.get(ssidAddr, ssid);
  EEPROM.get(pwdAddr, password);
  progress = EEPROM.read(progressAddr);
}
