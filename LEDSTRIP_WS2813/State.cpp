#include <arduino.h>
#include "State.h"
#include "Ledstrip.h"

int TrailLength = 20;

int VerspringenTijd = 40;
int VerspringenOffset = 2;
int VerspringenHue1 = 0;
int VerspringenHue2 = 110;
bool VerspringenOffsetBool = false;

int TweeTrailIndex = 0;

void Trail(int i, Ledstrip *ledstrip) {
   int index = i % ledstrip->leds.size();
   ledstrip->leds.at(index).Activate(ledstrip->maxBrightness);
    if(index >= TrailLength) {
      ledstrip->leds.at(index-TrailLength).DeActivate();
    } else {
      ledstrip->leds.at(ledstrip->leds.size() - TrailLength + index).DeActivate();
    }
}

void Verspringen(int i, Ledstrip *ledstrip) {
  int offset = 0;
  int color = VerspringenHue1;
  if((i % VerspringenTijd) != 0) {
    return;
  }
  ledstrip->TurnOffFast();
  if(VerspringenOffsetBool) {
    offset = VerspringenOffset;
    color = VerspringenHue2;
  }
  for(int x = 0; x < ledstrip->leds.size(); x+=4) {
    ledstrip->SetSingleHueFast(x + offset, color);
    ledstrip->SetSingleBriFast(x + offset, 255);
  }
  VerspringenOffsetBool = !VerspringenOffsetBool;
}

void Breathing(int i, Ledstrip *ledstrip) {
  if(i % 512 < 256) { 
    ledstrip->SetAllBriFast(i % 256);
  } else {
    ledstrip->SetAllBriFast(256 - (i % 256));
  }
}

void EenKleur(Ledstrip *ledstrip) {
  ledstrip->TurnOn();
}

void RandomKnipperen(Ledstrip *ledstrip) {
  ledstrip->TurnOffFast();
  int amountLeds = random(ledstrip->leds.size()/2);
  for(int x = 0; x < amountLeds; x++) {
    int index = random(ledstrip->leds.size());
    int color = random(255);
    ledstrip->SetSingleHueFast(index, color);
  }
  ledstrip->TurnOnFast();
}

void TweeTrail(Ledstrip *ledstrip) {
    ledstrip->TurnOffFast();
    for(int x = 0; x <= 2; x++) {
    if((TweeTrailIndex + x) >= 0 && (TweeTrailIndex + x) < ledstrip->leds.size()) {
      ledstrip->leds.at(TweeTrailIndex + x).SetBrightnessFast(255);
    }
    if((ledstrip->leds.size() - 1 - x - TweeTrailIndex) >= 0 && (ledstrip->leds.size() - 1 - x - TweeTrailIndex) < ledstrip->leds.size()) {
      ledstrip->leds.at(ledstrip->leds.size() - 1 - x - TweeTrailIndex).SetBrightnessFast(255);
    }
    TweeTrailIndex++;
    TweeTrailIndex = TweeTrailIndex % ledstrip->leds.size();
    }
}

