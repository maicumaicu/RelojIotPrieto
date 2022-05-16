#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Adafruit_BMP280.h>

#define BOTtoken "5307113029:AAHMJXpGUNAtd0ggSIvvm6fTZ4UC6ZygjD4"
#define CHAT_ID "5363319517"

#define HourState 0
#define TempState 1

const char* ssid     = "ORT-IoT";
const char* password = "OrtIOTnew22";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

int tempChangeDelay = 500;
unsigned long lastTempChange;
int ClockState = 0;

char hour[3];
char minute[3];

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
Adafruit_BMP280 bmp;

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

int red;
int green;
int blue;

Adafruit_NeoPixel digito_1 = Adafruit_NeoPixel(7, 16, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_2 = Adafruit_NeoPixel(7, 17, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_3 = Adafruit_NeoPixel(7, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel digito_4 = Adafruit_NeoPixel(7, 18, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel puntitos = Adafruit_NeoPixel(7, 19, NEO_GRB + NEO_KHZ800);

volatile unsigned long color = 0X009688;
void setup() {
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

  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  bot.sendMessage(CHAT_ID, "Bot Started", "");

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


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

void loop() {
  changeLocalTime();
  ShowControl();
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void ShowControl() {
  switch (ClockState) {
      case TempState:
        printLocalTemp();
        break;
      case HourState:
        printLocalTime();
        break;
    }
  if (millis() > lastTimeBotRan + botRequestDelay) {
    ClockState = !ClockState;
    lastTimeBotRan = millis();
  }
}

void printLocalTemp() {
  int temp = bmp.readTemperature();
  switch (calcularDecena(temp)) {
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

  switch (calcularUnidad(temp)) {
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

  letra_C(&digito_3);
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome." + from_name + ".\n";
      welcome += "Introduce the values just like this (red),(green),(blue).";
      bot.sendMessage(chat_id, welcome, "");
    }
    if (text.substring(0, 1) == "/" && text.substring(text.length() - 1, text.length()) == "/") {
      int firstComma = text.indexOf(",");
      int secondComma = text.indexOf(",", firstComma + 1);
      red = text.substring(1, firstComma).toInt();
      green = text.substring(firstComma + 1, secondComma).toInt();
      blue = text.substring(secondComma + 1, text.length() - 1).toInt();
      String confirmationMessage = "Color changed to R: ";
      confirmationMessage.concat(red);
      confirmationMessage.concat(" G: ");
      confirmationMessage.concat(green);
      confirmationMessage.concat(" B: ");
      confirmationMessage.concat(blue);
      bot.sendMessage(chat_id, confirmationMessage, "");
      color = puntitos.Color(red, green, blue);
    }
  }
}


void changeLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(hour, 3, "%H", &timeinfo);
  Serial.println(hour);
  strftime(minute, 3, "%M", &timeinfo);
  Serial.println(minute);
}

void printLocalTime() {

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
  return (num / 10) % 10;
}


void numero_0 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_1 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, 0, 0, 0);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_2 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, 0, 0, 0);

  tira->show();
}

void numero_3 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_4 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, 0, 0, 0);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_5 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, 0, 0, 0);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_6 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, 0, 0, 0);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_7 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, 0, 0, 0);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, 0, 0, 0);
  tira->setPixelColor(6, color);

  tira->show();
}
void numero_8 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void numero_9 (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, color);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, color);
  tira->setPixelColor(4, 0, 0, 0);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, color);

  tira->show();
}

void letra_C (Adafruit_NeoPixel *tira) {
  tira->setPixelColor(0, 0, 0, 0);
  tira->setPixelColor(1, color);
  tira->setPixelColor(2, color);
  tira->setPixelColor(3, 0, 0, 0);
  tira->setPixelColor(4, color);
  tira->setPixelColor(5, color);
  tira->setPixelColor(6, 0, 0, 0);

  tira->show();
}
