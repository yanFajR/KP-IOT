#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <credentials.h>

//VARIABLE INITIALIZATION
const char* WIFI_SSID = mySSID;
const char* WIFI_PASS = myPASSWORD;
int32_t CH = 11;
uint8_t BSSID[] = {0xD8,0x74,0x95,0xE2,0xD5,0x8C};
#define IP IPAddress(192,168,1,51)
#define DNS IPAddress(1,1,1,1)
#define GATEWAY IPAddress(192,168,1,1)
#define SUBNET IPAddress(255,255,255,0)

#define MQTT_HOST IPAddress(192, 168, 1, 100)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

//WIFI INITIALIZATION
void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS, CH, BSSID);
  WiFi.config(IP,DNS,GATEWAY,SUBNET);
}

//MQTT INITIALIZATION
void connectToMqtt() {
  mqttClient.connect();
}

//WIFI CONNECT
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  connectToMqtt();
}

//WIFI DISCONNECT
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

//MQTT PUBLISH MESSAGE
void onMqttConnect(bool sessionPresent) {
  mqttClient.publish("test/lol", 0, true, "test 1");
  mqttClient.publish("test/lol", 1, true, "test 2");
  mqttClient.publish("test/lol", 2, true, "test 3");
}

//DEEPSLEEP
void onMqttPublish(uint16_t packetId) {
  ESP.deepSleep(1000000*61);
}

void setup() {
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() {
}
