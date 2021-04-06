# ESP32-grafana
ESP32 module working with grafana and print var to 7seg display

## For install
 - download and install lib to Arduino IDE
 - setup ssid / password
 - get URL from gerafana
 - setup grafana token
 - upload firmware to your ESP32 module

## Hardware
 - ESP32 module
 - 8-Digit LED display (2x74HC595) Common Anode

## Helpful links
 - Lib Arduino Json: https://github.com/bblanchon/ArduinoJson
 - All libs for ESP8266: https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
 - Lib for HTTP client: https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient

## Notes
In code I use "L." on display because http lib interrupt other code, I mean it freeze code when it get data from url.

## Images
![alt text](https://github.com/fedorHub/ESP32-grafana/blob/main/images/IMG_0705.jpg?raw=false)
![alt text](https://github.com/fedorHub/ESP32-grafana/blob/main/images/IMG_0712.jpg?raw=false)
