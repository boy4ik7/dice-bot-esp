#include <Arduino.h>

#define WIFI_SSID "" // имя вай-фай сети
#define WIFI_PASS "" // пароль
#define BOT_TOKEN "" // токен бота
#define CHAT_ID "" // оставить пустым

#include <FastBot2.h>
FastBot2 bot;

#include <GyverHTTP.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
BearSSL::WiFiClientSecure client;
String randomNum;

void updateh(fb::Update& u) {
  digitalWrite(LED_BUILTIN, LOW);
  if ((u.message().text() == "/d4") || (u.message().text() == "/d4@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 4));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if (u.message().text() == "/d6") {
    randomNum = String(get_random_num(1, 6));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d8") || (u.message().text() == "/d8@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 8));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d10") || (u.message().text() == "/d10@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 10));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d12") || (u.message().text() == "/d12@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 12));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d20") || (u.message().text() == "/d20@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 20));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d100") || (u.message().text() == "/d100@lucy_dice_bot")) {
    randomNum = String(get_random_num(1, 100));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  bot.attachUpdate(updateh);
  bot.setToken(F(BOT_TOKEN));
  bot.setPollMode(fb::Poll::Long, 20000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  bot.tick();
}
int get_random_num(int minVal, int maxVal) {
  client.setInsecure();
  String url = "/api/v1.0/random?min=" + String(minVal) + "&max=" + String(maxVal) + "&count=1";
  ghttp::Client http(client, "www.randomnumberapi.com", 443);
  if (http.request(url)) {
    ghttp::Client::Response resp = http.getResponse();
    if (resp) {
      String randomNum = resp.body().readString();
      Serial.println("Response: " + randomNum);
      int num = randomNum.substring(1, randomNum.length() - 1).toInt();
      Serial.println("Parsed Number: " + String(num));
      return num;
    }
  }
  return -1;
}
//спустя сутки стал поятоянно давать ответ - 0
/*
int get_random_num(int minVal, int maxVal) {
  client.setInsecure();
  String url = "/integers/?num=1&min=" + String(minVal) + "&max=" + String(maxVal) + "&col=1&base=10&format=plain&rnd=new";
  ghttp::Client http(client, "www.random.org", 443);
  if (http.request(url)) {
    ghttp::Client::Response resp = http.getResponse();
    if (resp) {
      String randomNum = resp.body().readString();
      int num = randomNum.toInt();
      Serial.println(num);
      return num;
    }
  }
  return -1;
}
*/
