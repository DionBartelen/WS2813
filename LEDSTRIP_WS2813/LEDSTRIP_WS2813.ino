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
#include<PubSubClient.h>
#include <ESP8266WiFi.h>
#include "Ledstrip.h"
#include "State.h"

CRGB leds[NUM_LEDS];
Ledstrip ledstrip = Ledstrip(NUM_LEDS, 255);;

char ssid[] = "Sjaak Trekhaak";
char pass[] = "Snorfiets1";
char mqtt_server[] = "192.168.178.13";
char topic[] = "LedstripRamon";
char message_buff[20];

int i = 0;
int modi = 0;
boolean onOff = true;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  delay(1000);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  wifiSetup();
  client.setServer(mqtt_server, 1883);
  client.setCallback(onMessageReceived);
  reconnect();
  client.subscribe(topic);
  ledstrip.TurnOn();
}

void loop() {
  // put your main code here, to run repeatedly: 

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if(i % 2 == 0) {
      updateTime(); 
    }
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
    } else if(modi == 8) {
      bpm(leds, &ledstrip, i);
      FastLED.show();
    } else if(modi == 9) {
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

void onMessageReceived(char* topic, byte* payload, unsigned int length) {
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  
  if(msgString == "On" or (msgString == "OnOff" && !onOff)) {
    onOff = true;
    UpdateAllLeds();
  } else if(msgString == "Off" or (msgString == "OnOff" && onOff)) {
    onOff = false;
    ledstrip.TurnOffFast();
    UpdateAllLeds();
  } else if(msgString == "NextMode") {
    modi +=1;
    modi = modi % 9;
  } else if(msgString == "Previous") {
    modi -=1;
    modi = modi % 9;
  } else if(msgString.indexOf("ColorMode") >= 0) { 
    int number = (msgString.substring(msgString.indexOf(":") + 1).toInt());
      if(number == 0) {
        ledstrip.SetRainbow();
      } else if(number == 1) {
        ledstrip.SetWhite();
      }
  } else if(msgString.indexOf("Mode") >= 0) {
    modi = (msgString.substring(msgString.indexOf(":") + 1).toInt());
    i = 0;
  } else if(msgString.indexOf("Hue") >= 0) {
    ledstrip.SetAllHue((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else if(msgString.indexOf("Sat") >= 0) {
    ledstrip.SetAllSat((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else if(msgString.indexOf("Bri") >= 0) {
    ledstrip.maxBrightness = (msgString.substring(msgString.indexOf(":") + 1).toInt());
    ledstrip.SetAllBri((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else if(msgString.indexOf("Hours") >= 0) {
    SetTimeHours((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else if(msgString.indexOf("Minutes") >= 0) {
    SetTimeMinutes((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else if(msgString.indexOf("Seconds") >= 0) {
    SetTimeSeconds((msgString.substring(msgString.indexOf(":") + 1).toInt()));
  } else {
    Serial.println(msgString);
  }
}

void wifiSetup() {
    WiFi.hostname("LEDStripRamon");
    WiFi.begin(ssid, pass);
    while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(50);
    }
  Serial.println(WiFi.localIP());
}

void reconnect() {
  if(WiFi.status() != WL_CONNECTED) {
    wifiSetup();
  }
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("LedstripRamon")) {
      Serial.println("OK");
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }
  }
}

#endif


