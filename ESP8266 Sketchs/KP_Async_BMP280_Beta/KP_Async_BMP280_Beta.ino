#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <credentials.h>

//VARIABLE INITIALIZATION
const char* WIFI_SSID = mySSID;
const char* WIFI_PASS = myPASSWORD;
int32_t CH = 11;
uint8_t BSSID[] = {0xD8,0x74,0x95,0xE2,0xD5,0x8C};
#define IP IPAddress(192,168,1,50)
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

//SENSOR
Adafruit_BMP280 bmp;
String sensorPayload;
String batteryPayload;
float cal = 0.00433; // analog to voltage calibration value

//PUBLISH CHECK
#define SIZE 2 // number of publish mqtt message
Ticker publishTimer;
int publishCompleted = 0;


//GATHER SENSOR & BATTERY DATA
void dataGathering() {
  sensorPayload += "{\"pressure\":";
  sensorPayload += bmp.readPressure()/99.99F;
  sensorPayload += ",\"altitude\":";
  sensorPayload += bmp.readAltitude(1013.25)*1.055F;
  sensorPayload += "}";
  batteryPayload += "{\"battery_level\":";
  batteryPayload += analogRead(A0)*cal;
  batteryPayload += ",\"raw_analog\":";
  batteryPayload += analogRead(A0);
  batteryPayload += "}";
}

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

//MQTT PUBLISH SENSOR & BATTERY DATA
void onMqttConnect(bool sessionPresent) {
  dataGathering();
  mqttClient.publish("iot/node/1/sensor_data", 1, false, ((char*) sensorPayload.c_str()));
  mqttClient.publish("iot/node/1/battery_level", 1, false, ((char*) batteryPayload.c_str()));
}

//MQTT PUBLISH CONFIRMATION
void onMqttPublish(uint16_t packetId) {
  publishCompleted++;
}

void checkPublish() {
  if(publishCompleted == SIZE) {
    publishTimer.detach();
    ESP.deepSleep(1000000*60);
    }
}
void setup() {
//SENSOR INITIALIZATION
  bmp.begin(0x76);              // Initialisation with the alternative I2C address (0x76)
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X1,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X1,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X4,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  
  publishTimer.attach_ms(50, checkPublish);
  
  wifiConnectHandler =WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() {
}
