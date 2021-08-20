# KP-IOT
IoT Dashboard based on NodeRED and ESP8266 Microcontroller Repository.
# Table of contents
- [Installation](#install)
    - [Generate Self-Signed Certificate, Create Docker Volume, Deploy the Dashboard ](#installer)
    - [Mosquitto Config](#config)
    - [Import flows.json](#importFlows)
    - [Microcontroller Scetch](#scetch)
    - [Android IP Camera Configuration](#ipCam)
- [Topic Structure](#topic)
    - [Sensor Value Topic](#sensorValue)
    - [Battery Level Topic](#batteryLevel)

# # Installation <a name="install"></a>

## Generate Self-Signed Certificate <a name="installer"></a>
Generate Self-Signed Certificate, Create Docker Volume, Deploy the Dashboard by executing _installer.sh_ script. enter IP Server value according your server IP address/hostname.

## Mosquitto Config <a name="config"></a>
- For first time deployment, authentication is disabled. To enable authentication in MQTT, change these lines inside _mosquitto.conf_ file inside container. From 
```
allow_anonymous true
#password_file /mosquitto/config/passwd
```
to 
```
allow_anonymous false
password_file /mosquitto/config/passwd
```
- After _mosquitto-broker_ container running, enter _mosquitto-broker_ container terminal by executing the command below:
```
docker exec -it mosquitto-broker /bin/sh
```
- Create Mosquitto password file by executing the command below inside _mosquitto-broker_ terminal:
```
mosquitto_passwd -c /mosquitto/config/passwd nodered
```
enter the password, this will create username and hashed password used for MQTT authentication and store it inside _passwd_ file.
- To add username and password, execute the command below inside _mosquitto-broker_ terminal:
```
mosquitto_passwd /mosquitto/config/passwd USERNAME
```

## Import flows.json <a name="importFlows"></a>
- Login to your NodeRED-Dashboard admin page at https://YOUR-IP/admin. Default account is _admin/password_.
- After logged in, Click 3 Horizontal button, Select Import, Upload _flows.json_ file inside _nodered-dashboard_ folder from this repository.

## Microcontroller Scetch <a name="scetch"></a>
- Change _mqtt_server_, Network detail (_IP Address, Subnet Mask, Gateway_) value inside _ino_ file according to your setup.
- Change _fingerprint_ value, Fingerprint can be copied from _sha1-fingerprint.txt_ file, or by executing the command below in terminal:
```
openssl x509 -in server.crt -fingerprint -sha1 -noout
```
- Edit fingerprint writing format and match it with fingerprint example inside sketch demo file.
- If you use MQTT authentication, change _user_ and _passwd_ variable inside scetch file according to username and password that exists inside _passwd_ file created in [this](#config) step.
- If you do not use MQTT authentication, just comment _user_ and _passwd_ and remove _user_ and _passwd_ parameter in _connect_ method inside scetch file.

## Android IP Camera Configuration <a name="ipCam"></a>
- Download "IP Webcam" app from Google Play Store.
- Open the app & Click "Start Server".
- On Dashboard, Open "IP Camera Streaming" Tab.
- Enter IP & Port and Click "Submit".

# # Topic Structure <a name="topic"></a>
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
