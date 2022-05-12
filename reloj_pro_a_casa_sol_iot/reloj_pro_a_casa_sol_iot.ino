#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

const char* ssid     = "ORT-IoT";
const char* password = "OrtIOTnew22";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

Adafruit_NeoPixel digito_1 = Adafruit_NeoPixel(7, 16, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_2 = Adafruit_NeoPixel(7, 17, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_3 = Adafruit_NeoPixel(7, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_4 = Adafruit_NeoPixel(7, 18, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel puntitos = Adafruit_NeoPixel(7, 19, NEO_GRB + NEO_KHZ800);

volatile unsigned long color = 0X009688;
void setup(){
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  digito_1.begin();
  digito_2.begin();
  digito_3.begin();
  digito_4.begin();
  puntitos.begin();
  
  digito_1.setBrightness(100);
  digito_2.setBrightness(100);
  digito_3.setBrightness(100);
  digito_4.setBrightness(100);
  puntitos.setBrightness(100);

  puntitos.setPixelColor(0, 0X009688);
  puntitos.setPixelColor(1, 0X009688);
  puntitos.show();
}

void loop(){
  delay(1000);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  char hour[3];
  char minute[3];
  strftime(hour,3, "%H", &timeinfo);
  Serial.println(hour);
  strftime(minute,3, "%M", &timeinfo);
  Serial.println(minute);

  puntitos.setPixelColor(0, color);
  puntitos.setPixelColor(1, color);
  puntitos.show();
  
  switch (calcularDecena(atoi(hour))) {
    case 0: numero_0 (&digito_1); break;
    case 1: numero_1 (&digito_1); break;
    case 2: numero_2 (&digito_1); break;
    case 3: numero_3 (&digito_1); break;
    case 4: numero_4 (&digito_1); break;
    case 5: numero_5 (&digito_1); break;
    case 6: numero_6 (&digito_1); break;
    case 7: numero_7 (&digito_1); break;
    case 8: numero_8 (&digito_1); break;
    case 9: numero_9 (&digito_1); break;
  }

  switch (calcularUnidad(atoi(hour))) {
    case 0: numero_0 (&digito_2); break;
    case 1: numero_1 (&digito_2); break;
    case 2: numero_2 (&digito_2); break;
    case 3: numero_3 (&digito_2); break;
    case 4: numero_4 (&digito_2); break;
    case 5: numero_5 (&digito_2); break;
    case 6: numero_6 (&digito_2); break;
    case 7: numero_7 (&digito_2); break;
    case 8: numero_8 (&digito_2); break;
    case 9: numero_9 (&digito_2); break;
  }
  switch (calcularDecena(atoi(minute))) {
    case 0: numero_0 (&digito_3); break;
    case 1: numero_1 (&digito_3); break;
    case 2: numero_2 (&digito_3); break;
    case 3: numero_3 (&digito_3); break;
    case 4: numero_4 (&digito_3); break;
    case 5: numero_5 (&digito_3); break;
    case 6: numero_6 (&digito_3); break;
    case 7: numero_7 (&digito_3); break;
    case 8: numero_8 (&digito_3); break;
    case 9: numero_9 (&digito_3); break;
  }

  switch (calcularUnidad(atoi(minute))) {
    case 0: numero_0 (&digito_4); break;
    case 1: numero_1 (&digito_4); break;
    case 2: numero_2 (&digito_4); break;
    case 3: numero_3 (&digito_4); break;
    case 4: numero_4 (&digito_4); break;
    case 5: numero_5 (&digito_4); break;
    case 6: numero_6 (&digito_4); break;
    case 7: numero_7 (&digito_4); break;
    case 8: numero_8 (&digito_4); break;
    case 9: numero_9 (&digito_4); break;
  }
  delay(2000);
}

int calcularUnidad(int num) {
  return num % 10;
}

int calcularDecena(int num) {
  return (num/10) % 10;
}


void numero_0 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_1 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, 0, 0, 0);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_2 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, 0, 0, 0);
  
  tira->show();
}

void numero_3 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_4 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, 0, 0, 0);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_5 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, 0, 0, 0);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_6 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, 0, 0, 0);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_7 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);
  
  tira->show();
}
void numero_8 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}

void numero_9 (Adafruit_NeoPixel *tira){
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);
  
  tira->show();
}
