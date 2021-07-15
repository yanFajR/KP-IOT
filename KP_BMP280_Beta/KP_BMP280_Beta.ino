#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <credentials.h>

// WiFi & MQTT Variable
const char* WIFI_SSID = mySSID;
const char* WIFI_PASS = myPASSWORD;
const char* mqtt_server = "192.168.1.100";
const char* topic = "iot/node/1";
uint8_t bssid[] = {0xD8,0x74,0x95,0xE2,0xD5,0x8C};
int32_t ch = 11;
IPAddress ip(192,168,1,50);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);
IPAddress dns(1,1,1,1);

WiFiClient espClient;
PubSubClient client(espClient);

// Sensor
Adafruit_BMP280 bmp;

// MQTT Reconnect
void reconnect() {
  while (!client.connected()) {
    client.connect(topic);
    yield();
  }
}

void setup() {
// Sensor Initialization
  bmp.begin(0x76);              // Initialisation with the alternative I2C address (0x76)
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  
// MQTT Message & Read Sensor Data
  String sensorTopic;
  sensorTopic += topic;
  sensorTopic += "/sensor_data";
  String sensorPayload;
  sensorPayload += "{\"pressure\":";
  sensorPayload += bmp.readPressure()/99.95F;
  sensorPayload += ",\"altitude\":";
  sensorPayload += bmp.readAltitude(1015.8);
  sensorPayload += "}";

// Battery Level Monitoring
  float cal = 0.003895; // analog to voltage calibration value
  String batteryTopic;
  batteryTopic += topic;
  batteryTopic += "/battery_level";
  String batteryPayload;
  batteryPayload += "{\"battery_level\":";
  batteryPayload += analogRead(A0)*cal;
  batteryPayload += ",\"raw_analog\":";
  batteryPayload += analogRead(A0);
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
