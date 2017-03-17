#include "AdvancedOTA.h"
#include <WiFiUdp.h>

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

static WiFiUDP wifiUdp;
IPAddress ipBroadcast(192, 168, 230, 255);
unsigned int localPort = 7;

#define MACADDR "000d5ec70e04"

void setup() {
  pinMode(16, INPUT);
  digitalWrite(16, HIGH);
  
  Serial.begin(115200);
  wifiSetup(&WiFiMulti, &server);
  
  server.on("/wol", hndlWol);
  wifiUdp.begin(localPort);
}


void loop() {
  listener();

  if(!digitalRead(16)){
    Serial.println("send WOL Packet");
    char str[20];
    sprintf(str, MACADDR);
    sendWol(str);
  }
 
  delay(1000);
}



void hndlWol() {
  char str[20];
  sprintf(str, MACADDR);
  sendWol(str);

  String message = "Success";
  server.send(500, "text/plain", message);
}


void sendWol(char *addr){
  wifiUdp.beginPacket(ipBroadcast, localPort);

  char macAddr[128];
  char str[20];
  int hexSize;

  sprintf(str, "ffffffffffff");
  hexSize = ctohex((unsigned char*)macAddr, str);
  wifiUdp.write(macAddr, hexSize);

  hexSize = ctohex((unsigned char*)macAddr, addr);

  for (int i = 0; i < 16; i++) {
    wifiUdp.write(macAddr, hexSize);
  }

  wifiUdp.endPacket();
}


int ctohex(unsigned char *dst, const char *src) {
  int j = 0;
  dst[j] = 0x00;
  for (int i = 0; src[i] != '\0'; i++) {

    if (0x30 <= src[i] && src[i] <= 0x39 ) {
      dst[j] |= (src[i] - 0x30);
    }
    else if (0x41 <= src[i] && src[i] <= 0x46) {
      dst[j] |= (src[i] - 0x41 + 0x0A);
    }
    else if (0x61 <= src[i] && src[i] <= 0x66) {
      dst[j] |= (src[i] - 0x61 + 0x0A);
    }

    if (i % 2) {
      j++;
      dst[j] = 0x00;
    }
    else {
      dst[j] <<= 4;
    }

    /*    for(int a=0; a<16; a++){
          Serial.print(a);
          Serial.println(dst[a], HEX);
        }
    */
  }
  return j;

}

