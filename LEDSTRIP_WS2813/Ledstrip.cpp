#include <stdlib.h>
#include <stdio.h>
#include "Ledstrip.h"
#include "Led.h"

Ledstrip::Ledstrip(int al, int mb) {
  amountLeds = al;
  maxBrightness = mb;
  for(int x = 0; x < amountLeds; x++) {
     leds.push_back(Led(x, 0, 255, 0, 255, 0, 255));
  }
}

void Ledstrip::TurnOn() {
  for(int x = 0; x < amountLeds; x++) {
    leds[x].Activate(maxBrightness);
  }
}

void Ledstrip::TurnOnFast() {
  for(int x = 0; x < amountLeds; x++) {
    leds[x].FastOn(maxBrightness);
  }
}

void Ledstrip::TurnOff() {
  for(int x = 0; x < amountLeds; x++) {
    leds[x].DeActivate();
  }
}

void Ledstrip::TurnOffFast() {
  for(int x = 0; x < amountLeds; x++) {
    leds[x].FastOff();
  }
}

void Ledstrip::SetAllHue(int hue) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetColor(hue);
  }
}

void Ledstrip::SetAllHueFast(int hue) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetColorFast(hue);
  }
}

void Ledstrip::SetSingleHue(int pos, int hue) {
  leds[pos].SetColor(hue);
}

void Ledstrip::SetSingleHueFast(int pos, int hue) {
  leds[pos].SetColorFast(hue);
}

void Ledstrip::SetAllSat(int sat) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetSaturation(sat);
  }
}

void Ledstrip::SetAllSatFast(int sat) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetSaturationFast(sat);
  }
}

void Ledstrip::SetSingleSat(int pos, int sat) {
  leds[pos].SetSaturation(sat);
}

void Ledstrip::SetSingleSatFast(int pos, int sat) {
  leds[pos].SetSaturationFast(sat);
}

void Ledstrip::SetAllBri(int bri) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetBrightness(bri);
  }
}

void Ledstrip::SetAllBriFast(int bri) {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].SetBrightnessFast(bri);
  }
}

void Ledstrip::SetSingleBri(int pos, int bri) {
  leds[pos].SetBrightness(bri);
}

void Ledstrip::SetSingleBriFast(int pos, int bri) {
  leds[pos].SetBrightnessFast(bri);
}

void Ledstrip::Update() {
  for(int x = 0; x< amountLeds; x++) {
    leds[x].Update();
  }
}

void Ledstrip::SetAllColor(int hue, int sat, int bri){
  SetAllHue(hue);
  SetAllSat(sat);
  SetAllBri(bri);
}

void Ledstrip::SetRainbow() {
   for(int x = 0; x < leds.size(); x++) {
    double id = leds[x].number;
    double allLeds = leds.size();
    SetSingleHue(id, (id / allLeds) * 255);
    }
    SetAllSat(255);
}

void Ledstrip::SetWhite() {
  SetAllSat(0);
}

void Ledstrip::SetMaxBrightness(int i) {
  maxBrightness = i;
  //todo check if leds are above maxbrightness
}

