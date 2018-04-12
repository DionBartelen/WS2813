#define FASTLED_ALLOW_INTERRUPTS 0
#define LED_PIN     D1
#define CLOCK_PIN   D2 
#define NUM_LEDS    120
#define LED_TYPE    WS2813
#define COLOR_ORDER GRB
#define BRIGHTNESS  255
#ifndef _MAIN
#define _MAIN

#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "Ledstrip.h"
#include "State.h"

ESP8266WebServer server(80);
CRGB leds[NUM_LEDS];
Ledstrip ledstrip = Ledstrip(NUM_LEDS, 255);;

char ssid[] = "Pittige Picanto";
char pass[] = "Snorfiets1";
int i = 0;
int modi = 0;
boolean onOff = true;


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  delay(1000);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  wifiSetup();
  serverSetup();
  ledstrip.TurnOn();
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  if(i % 2 == 0) {
      updateTime(); 
    }
  server.handleClient();
  if(!onOff) {
    ledstrip.TurnOff();
  } else {
    HandleMode();
  }
  delay(10);
  i++;
}

void HandleMode() {
  if(modi == 0) {
      Trail(i, &ledstrip);
      UpdateAllLeds();
    } else if(modi == 1) {
      RandomKnipperen(&ledstrip);
      UpdateAllLeds();
    } else if(modi == 2) {
      Verspringen(i, &ledstrip);
      UpdateAllLeds();
    } else if(modi == 3) {
      TweeTrail(&ledstrip);
      UpdateAllLeds();
    } else if(modi == 4) {
      Breathing(i, &ledstrip);
      UpdateAllLeds();
    } else if(modi == 5) {
      EenKleur(&ledstrip);
      UpdateAllLeds();
    } else if (modi == 6) {
      Klok(&ledstrip); 
      UpdateAllLeds();
    } else if(modi == 7) {
      confetti(leds, &ledstrip, i);
      FastLED.show();
    }
     else if(modi == 8) {
      bpm(leds, &ledstrip, i);
      FastLED.show();
    }
     else if(modi == 9) {
      juggle(leds, &ledstrip);
      FastLED.show();
    }
}

void UpdateAllLeds() {
  ledstrip.Update();
  FastLED.clear();
  for(int x = 0; x < ledstrip.leds.size(); x++) {
    Led currentLed = ledstrip.leds.at(x);
    leds[x].setHSV(currentLed.hue, currentLed.saturation, currentLed.brightness);
  }
  FastLED.show();
}

void wifiSetup() {
  if (WiFi.status() != WL_CONNECTED) {
    IPAddress ip(192, 168, 178, 250); // where xx is the desired IP Address
    IPAddress gateway(192, 168, 178, 1); // set gateway to match your network
    IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
    WiFi.config(ip, gateway, subnet);
    WiFi.hostname("LEDStripRamon");
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(50);
    }
  }
  Serial.println(WiFi.localIP());
}

void serverSetup() {
  server.on("/command", commandReceived);
  server.on("/colormode", clrmodeReceived);
  server.on("/OnOff", onOffReceived);
  server.on("/color", clrReceived);
  server.on("/synctime", SyncTimeReceived);
  server.on("/clr", HueReceived);
  server.on("/sat", SatReceived);
  server.on("/bri", BriReceived);
  server.on("/stat", StatusReceived);
  //end
  server.begin();
}

void commandReceived() {
  String command = server.arg("command");
  modi = command.toInt();
  Serial.print("Command received: ");
  Serial.println(command);
  server.send(200, "text/plain", "OK");
  i = 0;
}

void clrmodeReceived() {
  String command = server.arg("colormode");
  int clrMode = command.toInt();
  if(clrMode == 0) {
    ledstrip.SetRainbow();
  } else if(clrMode == 1) {
    ledstrip.SetWhite();
  }
  server.send(200, "text/plain", "OK");
}

void onOffReceived() {
  String st = server.arg("on");
  if(st.toInt() == 1) {
    onOff = true;
    //ledstrip.TurnOn();//Not all light go on when turned on, only the one for the correct mode
  } else {
    onOff = false;
    ledstrip.TurnOff();
  }
  UpdateAllLeds();
  server.send(200, "text/plain", "OK");
}

void clrReceived() {
  String hue = server.arg("hue");
  String sat = server.arg("sat");
  String bri = server.arg("bri");
  ledstrip.SetAllColor(hue.toInt(), sat.toInt(), bri.toInt());
  server.send(200, "text/plain", "OK");
}

void HueReceived() {
  String hue = server.arg("hue");
  int newHue = hue.toInt();
  ledstrip.SetAllHue(newHue);
  server.send(200, "text/plain", "OK");
}

void SatReceived() {
  String sat = server.arg("sat");
  int newSat = sat.toInt();
  ledstrip.SetAllSat(newSat);
  server.send(200, "text/plain", "OK");
}

void BriReceived() {
  String bri = server.arg("bri");
  int newBri = bri.toInt();
  ledstrip.maxBrightness = newBri;
  ledstrip.SetAllBri(newBri);
  server.send(200, "text/plain", "OK");
}

void SyncTimeReceived() {
  Serial.println("Time received");
  String h = server.arg("h");
  String m = server.arg("m");
  String s = server.arg("s");
  String ms = server.arg("ms");
  modi = 6;
  SetTime(h.toInt(), m.toInt(), s.toInt(), ms.toInt());
  i = 0;
  server.send(200, "text/plain", "OK");
}

void StatusReceived() {
  int maxBri = ledstrip.maxBrightness;
  String a = "{\"mod\":\"" + modi;
  String b = "\", \"maxBri\":\"";
  String c = maxBri + "\"}";
  String d = a + b + c;
  server.send(200, "text/plain", d);
}

#endif


