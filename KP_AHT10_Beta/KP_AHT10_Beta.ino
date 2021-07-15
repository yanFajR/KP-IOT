#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <AHT10.h>
#include <credentials.h>

// WiFi & MQTT Variable
const char* WIFI_SSID = mySSID;
const char* WIFI_PASS = myPASSWORD;
const char* mqtt_server = "192.168.1.100";
const char* topic = "iot/node/2";
uint8_t bssid[] = {0xD8,0x74,0x95,0xE2,0xD5,0x8C};
int32_t ch = 11;
IPAddress ip(192,168,1,51);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);
IPAddress dns(1,1,1,1);

WiFiClient espClient;
PubSubClient client(espClient);

// Sensor
AHT10 aht10(AHT10_ADDRESS_0X38);

// MQTT Reconnect
void reconnect() {
  while (!client.connected()) {
    client.connect(topic);
    yield();
  }
}

void setup() {
// Sensor Initialization
  aht10.begin();
                  
// MQTT Message & Read Sensor Data
  String sensorTopic;
  sensorTopic += topic;
  sensorTopic += "/sensor_data";
  String sensorPayload;
  sensorPayload += "{\"temperature\":";
  sensorPayload += aht10.readTemperature();
  sensorPayload += ",\"humidity\":";
  sensorPayload += aht10.readHumidity();
  sensorPayload += "}";

// Battery Level Monitoring
  float cal = 0.003909; // analog to voltage calibration value
  String batteryTopic;
  batteryTopic += topic;
  batteryTopic += "/battery_level";
  String batteryPayload;
  batteryPayload += "{\"battery_level\":";
  batteryPayload += analogRead(A0)*cal;
  batteryPayload += "}";

// WiFi Initialization
  WiFi.forceSleepWake();
  delay(1);
  WiFi.begin(WIFI_SSID, WIFI_PASS,ch,bssid);
  WiFi.config(ip,dns,gateway,subnet); 
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.reconnect();
  }

// MQTT Connection
  client.setServer(mqtt_server, 1883);
  if (!client.connected()) {
    reconnect();
  }
  client.publish(((char*) sensorTopic.c_str()), ((char*) sensorPayload.c_str()));
  client.publish(((char*) batteryTopic.c_str()), ((char*) batteryPayload.c_str()));
  delay(50);
  ESP.deepSleep(1000000*60,WAKE_RF_DISABLED);
}
void loop() {}
