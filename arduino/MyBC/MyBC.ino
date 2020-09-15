
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>
#include <FS.h>
#include <Ticker.h>
#include <EEPROM.h>

// 引脚地址
#define L1 4
#define L2 5

// EEPROM
unsigned int eepromSize = 66 + 65 + 1;

// 网络相关
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
static const byte DNS_PORT = 53;
ESP8266WebServer webServer(80);
const char *myHostname = "hello";
WiFiClient client;

// Wi-Fi相关
char ssid[33] = "";
char password[65] = "";
bool connect = false;
unsigned long lastConnectTry = 0;
unsigned int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(eepromSize);

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
  webServer.on("/is", HTTP_GET, handleIS); // 指令集
  webServer.on("/led", HTTP_GET, handleLed); // led操作
  webServer.on("/config", HTTP_GET, handleConfig); // 配置信息
  webServer.on("/putConfig", HTTP_GET, handlePutConfig); // 设置配置
  webServer.on("/wifiscan", HTTP_GET, handleWifiscan); // 返回 Wi-Fi 扫描结果
  webServer.on("/wificonn", HTTP_GET, handleWificonn); // 连接 Wi-Fi
  webServer.on("/wificonf", HTTP_GET, handleWificonf); // 返回 Wi-Fi 配置
  webServer.on("/version", HTTP_GET, handleVersion); // 返回版本
  webServer.on("/nVersion", HTTP_GET, handleNVersion); // 返回最新版本
  webServer.on("/update", HTTP_GET, handleUpdate); // 升级
  webServer.on("/progress", HTTP_GET, handleProgress); //获取进度
  webServer.begin();

  ledLight(0);

  if (!SPIFFS.begin()) {
    Serial.println("fs begin error");
    return;
  }

  getEEPROM();
  connect = strlen(ssid) > 0;
}

void loop()
{
  if (connect) {
    Serial.println("Connect requested");
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    unsigned int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 60000)) {
      connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print("Status: ");
      Serial.println(s);
      status = s;
      if (s == WL_CONNECTED) {
        Serial.println("Wi-Fi connection successful");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
    if (s == WL_CONNECTED) {
      MDNS.update();
    }
  }
 
  dnsServer.processNextRequest();
  webServer.handleClient();
}
