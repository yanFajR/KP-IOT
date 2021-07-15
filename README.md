# KP-IOT
IoT Dashboard based on NodeRED and ESP8266 Microcontroller Repository.
# Table of contents
- [Topic Structure](#topic)
    - [Sensor Value Topic](#sensorValue)
    - [Battery Level Topic](#batteryLevel)

# Topic Structure <a name="topic"></a>
MQTT topic structure for different value.
X = node number

### Sensor Value Topic  <a name="sensorValue"></a>
MQTT topic for sending various sensor data (temperature, humidity, barometric pressure, etc.).
```
iot/node/X/sensor_data
```
### Battery Level Topic  <a name="batteryLevel"></a>
MQTT topic for sending battery level in Volts.
```
iot/node/X/battery_level
```
