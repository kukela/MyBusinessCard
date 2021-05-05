// 获取FS信息
void handleGetFSInfo() {
  FSInfo fs_info;
  if (!SPIFFS.info(fs_info)) {
    webServer.send(500, FPSTR(TEXT_PLAIN), "fs info error");
    return;
  }
  maxPathLength = fs_info.maxPathLength - 1;

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

// 返回文件
void replyFile(String path) {
  String contentType = mime::getContentType(path);
  Serial.println(path + " - " + contentType);
  
  if (!SPIFFS.exists(path)) {
    if(path == getHomeUrl()) {
      webServer.sendHeader("Location", "http://" + webServer.client().localIP().toString() + "/admin", true);
      replyServerCode(307);
      return;
    }
    replyServerCode(204);
    return;
  }
  file = SPIFFS.open(path, "r");
  configSendHeader();
  if (webServer.streamFile(file, contentType) != file.size()) {
    replyServerNotFound(path);
  }
  file.close();
}

// 格式化fs
bool clearFS() {
  dir = SPIFFS.openDir("");
  while (dir.next()) {
    if (!SPIFFS.remove(dir.fileName())) {
      return false;
    }
  }
  return true;
//  SPIFFS.end();
//  bool isOk = SPIFFS.format();
//  if (!isOk) {
//    return false;
//  }
//  isOk = SPIFFS.begin();
//  return isOk;
}
