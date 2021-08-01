#include <ESP8266WiFi.h>
#include <credentials.h>

// Set WiFi Information
const char* WIFI_SSID = mySSID;
const char* WIFI_PASS = myPASSWORD;
uint8_t bssid[] = {0xD8,0x74,0x95,0xE2,0xD5,0x8C};
int32_t ch = 11;
IPAddress ip(192,168,1,50);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);
IPAddress dns(1,1,1,1);


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  WiFi.config(ip,dns,gateway,subnet);
  WiFi.begin(WIFI_SSID, WIFI_PASS,ch,bssid);
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
}

void loop(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
