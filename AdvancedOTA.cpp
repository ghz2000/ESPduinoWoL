#include <Arduino.h>
#include "AdvancedOTA.h"

ESP8266WiFiMulti *m_WiFiMulti;
ESP8266WebServer *m_server;

void handleRoot() {
  m_server->send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += m_server->uri();
  message += "\nMethod: ";
  message += (m_server->method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += m_server->args();
  message += "\n";
  for (uint8_t i=0; i<m_server->args(); i++){
    message += " " + m_server->argName(i) + ": " + m_server->arg(i) + "\n";
  }
  m_server->send(404, "text/plain", message);
}

void handleSearchSSID(){
  String message = "<HTML><HEAD></HEAD>";
  message += "<BODY>";

  int n = WiFi.scanNetworks();
  for(int i = 0; i<n ; ++i){
    message += i;
    message += ":";
    message += WiFi.SSID(i);
    message += " (";
    message += WiFi.RSSI(i);
    message += ")";
    message += (WiFi.encryptionType(i) == ENC_TYPE_NONE)? " ": "*";
    message += "<BR>";
    delay(10);
  }
  message += "</BODY></HTML>";
  m_server->send(200, "text/html", message);
}

void wifiSetup(ESP8266WiFiMulti *WiFiMulti, ESP8266WebServer *server){
  m_WiFiMulti = WiFiMulti;
  m_server = server;

  Serial.begin(115200);
  Serial.println("Booting");


  ////// WiFi Connection

  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ap_ssid, ap_pass);
  Serial.print("AP mode IP:");
  Serial.println(WiFi.softAPIP());




  WiFiMulti->addAP(sta_ssid, sta_pass);
 
//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("Connection Failed! Rebooting...");
//    delay(5000);
//    ESP.restart();
//  }

  Serial.print(sta_ssid);
  Serial.print(":");
  Serial.println(sta_pass);

  for(int i=0; i<10; i++){
    switch(WiFiMulti->run()){
      case WL_CONNECTED:
        Serial.println("Connection Successful");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        break;
      case WL_NO_SSID_AVAIL:
        Serial.println("Searching SSID");
        delay(5000);
        continue;
      case WL_CONNECT_FAILED:
        Serial.println("Connection Failed. wrong password");
        delay(5000);
        continue;
      case WL_IDLE_STATUS:
        Serial.println("Connection Idle Status");
        delay(5000);
        continue;
      case WL_DISCONNECTED:
        Serial.println("Connection Disconnected");
        delay(5000);
        continue;
      default :
        Serial.println("default error");
        delay(5000);
        ESP.restart();
        continue;
    }
    break;
  }




/////// Scan
  int n = WiFi.scanNetworks();
  Serial.print("scan done:");
  Serial.println(n);
  for(int i = 0; i<n ; ++i){
    Serial.print(i);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(")");
    Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)? " ": "*");
    delay(10);
  }

  ////// OTA Function
  
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();


  ////// WebServer
  if (MDNS.begin(mdns_name)) {
    Serial.println("MDNS responder started");
  }
  
  m_server->on("/", handleRoot);
  m_server->on("/ssid", handleSearchSSID);

  m_server->onNotFound(handleNotFound);

  m_server->begin();
  Serial.println("HTTP server started");
  
}

void listener(){
  ArduinoOTA.handle();
  m_server->handleClient();
}
