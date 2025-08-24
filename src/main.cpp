#include <Arduino.h>
#include <FastBot2.h>
#include <GyverNTP.h>
#include <GyverHTTP.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <TimeRandom.h>
#include <Random16.h>
#include <GyverDBFile.h>
#include <LittleFS.h>
#include <SettingsESP.h>
#include <WiFiConnector.h>

FastBot2 bot;
Random16 rnd16;
TimeRandom rnd;
BearSSL::WiFiClientSecure client;
String randomNum;
GyverDBFile db(&LittleFS, "/data.db");
SettingsESP sett("Telegram BOT", &db);
bool LED = true;

DB_KEYS(
    kk,
    wifi_ssid,
    wifi_pass,
    bot_token,
    bot_name,
    random_mode,
    apply);

int get_random_num(int minVal, int maxVal) {
  int num = 0;
  if (db[kk::random_mode] == 0) {
    NTP.tick();
    rnd.update(NTP.hour(), NTP.minute(), NTP.second());
    num = rnd.fromSec(1, 99999);
    rnd16.setSeed(num);
    num = rnd16.get(minVal, maxVal);
  }
  if (db[kk::random_mode] == 1) {
    client.setInsecure();
    String url = "/api/v1.0/random?min=" + String(minVal) + "&max=" + String(maxVal) + "&count=1";
    ghttp::Client http(client, "www.randomnumberapi.com", 443);
    if (http.request(url)) {
      ghttp::Client::Response resp = http.getResponse();
      if (resp) {
        String randomNum = resp.body().readString();
        num = randomNum.substring(1, randomNum.length() - 1).toInt();
      }
    }
  }
  if (db[kk::random_mode] == 2) {
    client.setInsecure();
    String url = "/integers/?num=1&min=" + String(minVal) + "&max=" + String(maxVal) + "&col=1&base=10&format=plain&rnd=new"; 
    ghttp::Client http(client, "www.random.org", 443);
    if (http.request(url)) {
      ghttp::Client::Response resp = http.getResponse();
      if (resp) {
        String randomNum = resp.body().readString();
        num = randomNum.toInt();
      }
    }
  }
  return num;
}

void build(sets::Builder& b) {
  if (b.beginGroup("Wi-Fi")) {
    b.Input(kk::wifi_ssid, "SSID");
    b.Pass(kk::wifi_pass, "PASSWORD");
    if (b.Button(kk::apply, "Connect")) {
      db.update();
      WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
    }
    b.endGroup();
  }
  if (b.beginGroup("Settings")) {
    b.Input(kk::bot_name, "@Bot name");
    b.Pass(kk::bot_token, "Bot token");
    b.Select(kk::random_mode, "Random mode", "local;www.randomnumberapi.com;www.random.org");
    db.update();
    b.endGroup();
  }
}

void updateh(fb::Update& u) {
  if ((u.message().text() == "/d4") || (u.message().text() == "/d4" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 4));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if (u.message().text() == "/d6") {
    randomNum = String(get_random_num(1, 6));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d8") || (u.message().text() == "/d8" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 8));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d10") || (u.message().text() == "/d10" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 10));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d12") || (u.message().text() == "/d12" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 12));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d20") || (u.message().text() == "/d20" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 20));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  } else if ((u.message().text() == "/d100") || (u.message().text() == "/d100" + String(db[kk::bot_name]))) {
    randomNum = String(get_random_num(1, 100));
    if (randomNum == "-1") bot.sendMessage(fb::Message("Попробуйте ещё раз.", u.message().chat().id()));
    else bot.sendMessage(fb::Message(randomNum, u.message().chat().id()));
  }
  digitalWrite(LED_BUILTIN, !LED);
}

void setup() {
#ifdef ESP32
    LittleFS.begin(true);
#else
    LittleFS.begin();
#endif
  db.begin();
  db.init(kk::wifi_ssid, "");
  db.init(kk::wifi_pass, "");
  db.init(kk::bot_token, "");
  db.init(kk::bot_name, "");
  db.init(kk::random_mode, 0);
  WiFiConnector.connect(db[kk::wifi_ssid], db[kk::wifi_pass]);
  sett.begin();
  sett.onBuild(build);
  bot.attachUpdate(updateh);
  bot.setToken(db[kk::bot_token]);
  bot.setPollMode(fb::Poll::Long, 20000);
  pinMode(LED_BUILTIN, OUTPUT);
  NTP.begin(2);
}

void loop() {
  WiFiConnector.tick();
  sett.tick();
  bot.tick();
  digitalWrite(LED_BUILTIN, LED);
}
