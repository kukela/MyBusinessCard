// 首页
void handleRoot() {
  if (!isHost()) {
    Serial.println(webServer.uri() + " 重定向host");
    webServer.sendHeader("Location", "http://" + webServer.client().localIP().toString(), true);
    replyServerCode(302);
    return;
  }
  replyFile(getHomeUrl());
}

// 请求不在host中或者fs中找不到资源返回404
void handleNotFound() {
  if (!isHost()) {
    replyServerNotFound(webServer.uri());
    return;
  }
  replyFile(webServer.uri());
}

// 管理页面
void handleAdmin() {
  configSendHeader();
  webServer.send(200, "text/html", adminHTML);
}

// 指令集
void handleIS() {
  ticker.detach();

  int v = webServer.arg("v").toInt();
  switch (v) {
    case 0: { // 格式化SPIFFS
        if (clearFS()) {
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

// 返回版本
void handleVersion() {
  ticker.detach();

  String json;
  json.reserve(32);
  json += "{\"ver\":";
  json += ver;
  json += ",\"wver\":\"";
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
  if (http.begin(client, update_host + "ver.json")) {
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
