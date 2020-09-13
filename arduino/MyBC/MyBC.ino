
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Ticker.h>
#include <EEPROM.h>

// 引脚地址
#define L1 4
#define L2 5

// 网络相关
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
static const byte DNS_PORT = 53;
ESP8266WebServer webServer(80);

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(1 + 32 + 1);
  while (!Serial)
    ;
  Serial.println();

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  setChangeAP();

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "www.hello.com", apIP);
  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/", HTTP_GET, handleRoot);
  webServer.on("/generate_204", HTTP_GET, handleRoot);
  webServer.on("/fwlink", handleRoot);
  webServer.on("/admin", HTTP_GET, handleAdmin);
  webServer.onNotFound(handleNotFound);
  webServer.on("/info", HTTP_GET, handleGetInfo); // 文件信息
  webServer.on("/update", HTTP_POST, handleFileUploadAfter, handleFileUpload); // 上传文件
  webServer.on("/erase", HTTP_GET, handleErase); // 格式化
  webServer.on("/led", HTTP_GET, handleLed); // led操作
  webServer.on("/config", HTTP_GET, handleConfig); // 配置信息
  webServer.on("/putConfig", HTTP_GET, handlePutConfig); // 设置配置
  webServer.begin();

  digitalWrite(L1, LOW);
  digitalWrite(L2, LOW);

  if (!SPIFFS.begin()) {
    Serial.println("fs begin error");
    return;
  }
}

void loop()
{
  dnsServer.processNextRequest();
  webServer.handleClient();
}
