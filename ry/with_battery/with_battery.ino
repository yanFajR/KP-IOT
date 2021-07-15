//include semua librari yang diperlukan
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Adafruit_BMP085.h>

//definnisikan ssid dan password wifi
#define WIFI_SSID "HUAWEI"
#define WIFI_PASSWORD "tanggal20"

//definisikan address mqtt dan portnya
#define MQTT_HOST IPAddress(192, 168, 100, 143)
#define MQTT_PORT 1883

//definiskan topik sensor
#define MQTT_PUB_TEMP "esp/dht/temperature"
#define MQTT_PUB_HUM "esp/dht/humidity"
#define MQTT_PUB_PRS "esp/bmp/pressure"
#define MQTT_PUB_BAT "esp/anl/battery"

// pin terkoneksi ke sensor dht11
#define DHTPIN 14  
#define DHTTYPE DHT11

// Initialize DHT dan bmp
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp; 

// Variables untuk simpan nilai sensor
float temp;
float hum;
int prs;
float bat;
int volt;

//objek untuk menghandle mqtt client dan waktu reconnect ketika disconnect
AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TEMP, packetIdPub1);
  Serial.printf("Message: %.2f \n", temp);

  uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
  Serial.printf("Message: %.2f \n", hum);

  uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_PRS, 1, true, String(prs).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_PRS, packetIdPub3);
  Serial.printf("Message: %i \n", prs);

  uint16_t packetIdPub4 = mqttClient.publish(MQTT_PUB_BAT, 1, true, String(prs).c_str());                            
  Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_BAT, packetIdPub4);
  Serial.printf("Message: %.1f \n", bat);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);

  if (packetId == 4){
     Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
     ESP.deepSleep(30e6);  
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  dht.begin();
  bmp.begin();
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  prs = bmp.readPressure();
  volt = analogRead(0);
  bat = 0.0048*volt;
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();

}

void loop() {
}
