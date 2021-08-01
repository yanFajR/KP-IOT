# KP-IOT
IoT Dashboard based on NodeRED and ESP8266 Microcontroller Repository.
# Table of contents
- [Installation](#install)
    - [Create Docker Volume](#volume)
    - [Generate Self-Signed Certificate](#cert)
    - [Import flows.json](#importFlows)
    - [Microcontroller Scetch](#scetch)
- [Topic Structure](#topic)
    - [Sensor Value Topic](#sensorValue)
    - [Battery Level Topic](#batteryLevel)

# Installation <a name="install"></a>

## Create Docker Volume <a name="volume"></a>
Run command below to create volume for the dashboard services.
```
docker volume create NodeREDdata
docker volume create MariaDBdata
docker volume create Mosquittodata
```
## Generate Self-Signed Certificate <a name="cert"></a>
Generate Self-Signed Certificate by running _cert-gen.sh_ script. Edit CN value inside the script according your server IP address/hostname.

## Import flows.json  <a name="importFlows"></a>
Login to your NodeRED-Dashboard admin page at https://YOUR-IP/admin. Default account is _admin/password_. After logged in, Click 3 Horizontal button, Select Import, Upload _flows.json_ file inside _nodered-dashboard_ folder from this repository.

## Microcontroller Scetch <a name="scetch"></a>
Change _mqtt_server_, Network detail (_IP Address, Subnet Mask, Gateway_) value inside _ino_ file according to your setup. Fingerprint can be copied from _sha1-fingerprint.txt_ file, 
or by running the command below in terminal. Edit fingerprint writing format and match it with fingerprint example inside sketch demo file.
```
openssl x509 -in server.crt -fingerprint -sha1 -noout
```


# Topic Structure <a name="topic"></a>
MQTT topic structure for different value.
X = node number.

## Sensor Value Topic  <a name="sensorValue"></a>
MQTT topic for sending various sensor data (temperature, humidity, barometric pressure, etc.).
```
iot/node/X/sensor_data
```
## Battery Level Topic  <a name="batteryLevel"></a>
MQTT topic for sending battery level in Volts.
```
iot/node/X/battery_level
```
