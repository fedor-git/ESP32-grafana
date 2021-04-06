#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "HTTPClient.h"

// Display Pins:
#define SCLK 12
#define RCLK 13  
#define DIO 14
// display buffer
byte digitBuffer[8];
// On boot number:
String display_number = "0";

// Timer1
uint32_t myTimer1;
int period = 60000;
// Timer 2
uint32_t myTimer2;
int period2 = 1000;

// Allocate the JSON document
// Wi-Fi & WEB
const char* ssid     = "";     
const char* password = "";
const char* server_rq = "";
const char* server_auth = "";
//
String json_str;

WiFiClientSecure client;
StaticJsonDocument<200> doc;
  
void setup(){
  pinMode(RCLK, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(DIO, OUTPUT);
  // Initialize serial port
  Serial.begin(115200);
  showLoad();
  WiFi.begin(ssid, password);
  // while (!Serial) continue;
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  // wait 1 second for re-trying
  delay(1000);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  showLoad();
  http_get();
}

void loop(){
  if (millis() - myTimer2 >= period2) {
   myTimer2 += period2;
  displayjson();
  }

  if (millis() - myTimer1 >= period) {
  myTimer1 += period;
  showLoad();
  http_get();
  showLoad();
  }
  // On display show data:
  shownumber();
}

void displayjson(){
   // Deserialize the JSON document
   int json_len = json_str.length() + 1; 
   char char_array[json_len];
   json_str.toCharArray(char_array, json_len);
  DeserializationError error = deserializeJson(doc, char_array);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  // Fetch values.
  const char* msg_status = doc["status"];
  const char* resultType = doc["data"]["resultType"];
  String datavalue = doc["data"]["result"][0]["value"][1];
  // Print values.
  Serial.println("-------------------------------------");
  Serial.print("Message received: ");
  Serial.println(msg_status);
  Serial.print("Resault type: ");
  Serial.println(resultType);
  Serial.print("Data: ");
  Serial.println(datavalue);

  display_number = datavalue;
}

void http_get(){
  if(WiFi.status()== WL_CONNECTED){
  
  HTTPClient http;   
  
  http.begin(server_rq);
  http.addHeader("Authorization", server_auth);            
 
  int httpCode = http.GET();   
   if(httpCode != 0){
    json_str = http.getString();
    Serial.println(httpCode);
    Serial.print("Data: ");
    Serial.println(json_str);
   }else{
     Serial.print("Error on sending Request");
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
 }
}

void shownumber(){
  int show_data = display_number.toInt();
  for(int d = 0; d <= display_number.length()-1; d++){
    char n = display_number[d];
    int dd = n-'0';
    digitBuffer[d] = dd;
  }
  showDisplay();
}

void showDisplay(){
  
  const byte digit[10] = {  // маска для 7 сигментного индикатора  
      0b11000000, // 0
      0b11111001, // 1
      0b10100100, // 2
      0b10110000, // 3
      0b10011001, // 4
      0b10010010, // 5
      0b10000010, // 6
      0b11111000, // 7
      0b10000000, // 8
      0b10010000, // 9 
  };

  const byte chr[8] = { // маска для разряда
      0b00010000, // 0 digit ( 0 buffer )
      0b00100000, // 1
      0b01000000, // 2
      0b10000000, // 3
      0b00000001, // 4
      0b00000010, // 5
      0b00000100, // 6
      0b00001000  // 7 
  };
  // отправляем в цикле по два байта в сдвиговые регистры
  for(byte i = 0; i <= display_number.length()-1; i++){ 
    digitalWrite(RCLK, LOW); // открываем защелку
      if ( digitBuffer[i] != -1) { // пустой разряд ХЗ использовать или нет
      shiftOut(DIO, SCLK, MSBFIRST, digit[digitBuffer[i]]);  // отправляем байт с "числом"
      shiftOut(DIO, SCLK, MSBFIRST, chr[i]);   // включаем разряд
      }
    digitalWrite(RCLK, HIGH); // защелкиваем регистры
  }
}

void showLoad(){
 digitalWrite(RCLK, LOW);
 shiftOut(DIO, SCLK, MSBFIRST, 0b01000111);
 shiftOut(DIO, SCLK, MSBFIRST, 0b00010000);
 digitalWrite(RCLK, HIGH);
}
