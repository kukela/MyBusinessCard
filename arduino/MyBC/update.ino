uint8_t progress = 0;
static const String update_host = "http://kukela-bin.oss-cn-shanghai.aliyuncs.com/mybc/";

// 升级
void handleUpdate() {
  ticker.detach();
  if(status != 3) {
    replyServerCode(500);
    return;
  }
  int t = webServer.arg("t").toInt();
  
  ESPhttpUpdate.onStart(update_started);
  ESPhttpUpdate.onEnd(update_finished);
  ESPhttpUpdate.onProgress(update_progress);
  ESPhttpUpdate.onError(update_error);

  t_httpUpdate_return ret;
  
  if (t == 0) {
    replyServerCode(200);
    ret = ESPhttpUpdate.update(client, update_host + "MyBC.ino.generic.bin");
  } else {
    if (!clearFS()) {
      replyServerCode(500);
      return;
    }
    replyServerCode(200);
    ret = ESPhttpUpdate.updateFS(client, update_host + "spiffs.bin");
    SPIFFS.end();
    delay(10);
    SPIFFS.begin();
  }
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
    }
}

void update_started() {
  progress = 0;
  writeProgress();
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  progress = 100;
  writeProgress();
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  progress = (uint8_t)(((double)cur / total) * 100.0);
  writeProgress();
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  progress = 255;
  writeProgress();
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

// 进度
void handleProgress() {
  webServer.send(200, FPSTR(TEXT_PLAIN), String(progress));
}

void writeProgress() {
  EEPROM.write(progressAddr, progress);
  EEPROM.commit();
}
