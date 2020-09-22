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

//连接Wi-Fi
void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
  Serial.println(connRes);
}

//保存ssid和密码
void saveWifi() {
  EEPROM.put(ssidAddr, ssid);
  EEPROM.commit();
  EEPROM.put(pwdAddr, password);
  EEPROM.commit();
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
