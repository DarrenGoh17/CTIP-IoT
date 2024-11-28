# CTIP-IoT

## poacher.ino
This file act as a bridge between IoT sensor's datas and cameratrap.js.

## How to set up
1. Change the ssid, "const char* ssid = "HUAWEI-079E";" to your connected WiFi/Hotspot.
2. Change the password, "const char* password = "19468559";" to your connected WiFi/Hotspot.
3. Change the serverURL, "const char* serverURL = "http://192.168.8.103:3002/addSensorData";" to your connected WiFi/Hotspot.
4. On the top left, click "Select Other Board and Port" and find "ESP32 Dev Module".
5. In the terminal, change the selection to "Both NL & CR".
6. Compile the code by clicking "Right Arrow".

## cameratrap.js
This file act as a bridge between poacher.ino and database by collecting data collected from sensors and upload to database for real-time tracking

## Dependencies
- express
- mysql
- body-parser
- cors
- nodemailer
Full installation:
**npm install express mysql body-parser cors nodemailer**

## How to set up
1. Run Apache and MySQL in XAAMP.
2. cd into directory of cameratrap.js
3. type "node cameratrap.js"
4. Real-time data collection should work as expected.
