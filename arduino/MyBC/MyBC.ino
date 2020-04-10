#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <Ticker.h>

#define L1 4
#define L2 5

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

Dir dir;
File file;
uint8_t maxPathLength;// 最大文件路径长度

Ticker ticker;
uint8_t tickerIndex = 0;
bool isLightLed1 = false;
bool isLightLed2 = false;
bool isWLLed = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println();

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("aMyCard");

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, "www.hello.com", apIP);
  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/info", HTTP_GET, handleGetInfo);
  webServer.on("/update", HTTP_POST, handleFileUploadAfter, handleFileUpload);
  webServer.on("/erase", HTTP_GET, handleErase);
  webServer.onNotFound(handleRoot);
  webServer.on("/admin", HTTP_GET, handleAdmin);
  webServer.on("/led", HTTP_GET, handleLed);
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
