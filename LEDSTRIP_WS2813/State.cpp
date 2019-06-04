#include <arduino.h>
#include <FastLED.h>
#include "State.h"
#include "Ledstrip.h"

int TrailLength = 20;

int VerspringenTijd = 40;
int VerspringenOffset = 2;
int VerspringenHue1 = 0;
int VerspringenHue2 = 110;
bool VerspringenOffsetBool = false;

int TweeTrailIndex = 0;

unsigned long klokLastMillis = millis();
int klokMilli = 0;
int klokSeconds = 0;
int klokMinutes = 0;
int klokHours = 0;


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

void SetTime(int hours, int minutes, int seconds, int milliseconds) {
  klokMilli = milliseconds;
  klokSeconds = seconds;
  klokMinutes = minutes;
  klokHours = hours; 
}

void SetTimeHours(int hours) {
  klokHours = hours;
}
void SetTimeMinutes(int minutes) {
  klokMinutes = minutes;
}
void SetTimeSeconds(int seconds) {
  klokSeconds = seconds;
}
void SetTimeMillis(int milliseconds) {
  klokMilli = milliseconds;
}

void Klok(Ledstrip *ledstrip) {
  int offset = 60;
  ledstrip->TurnOffFast();
  double s = klokSeconds;
  double m = klokMinutes;
  double h = klokHours;
  double all = ledstrip->leds.size();

  int idMil = all - (klokMilli * (all/1000)) + offset;
  idMil = idMil % ledstrip->leds.size();
  if(idMil >= 0 && idMil < ledstrip->leds.size()) {
    ledstrip->SetSingleBriFast(idMil, ledstrip->maxBrightness);
  }
  
  int idSecondes = all - (s * (all/60.0)) + offset;
  idSecondes = idSecondes % ledstrip->leds.size();
  if(idSecondes >= 0 && idSecondes < ledstrip->leds.size()) {
    ledstrip->SetSingleBriFast(idSecondes, ledstrip->maxBrightness);
  }
  int idMinutes = all - (m * (all/60.0)) + offset;
  idMinutes = idMinutes % ledstrip->leds.size();
  if(idMinutes >= 0 && idMinutes < ledstrip->leds.size()) {
    ledstrip->SetSingleBriFast(idMinutes, ledstrip->maxBrightness);
  }
  int idHours = all - (h * (all/12.0)) + offset;
  idHours = idHours % ledstrip->leds.size();
  if(idHours >= 0 && idHours < ledstrip->leds.size()) {
    ledstrip->SetSingleBriFast(idHours - 2, ledstrip->maxBrightness);
    ledstrip->SetSingleBriFast(idHours, ledstrip->maxBrightness);
    ledstrip->SetSingleBriFast(idHours + 2, ledstrip->maxBrightness);
  }
}

void updateTime() {
  unsigned long currentMillis = millis();
  long pastTime = currentMillis - klokLastMillis;
  klokMilli += pastTime;
  klokLastMillis = currentMillis;
  if(klokMilli > 1000) {
    klokMilli -= 1000;
    klokSeconds += 1;
  }
  if(klokSeconds >= 60) {
    klokSeconds -= 60;
    klokMinutes += 1;
  }
  if(klokMinutes >= 60) {
    klokMinutes -= 60;
    klokHours += 1;
  }
  if(klokHours >= 12) {
    klokHours -= 12;
  }
}


void confetti(CRGB *leds, Ledstrip *ledstrip, int gHue) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, ledstrip->leds.size(), 10);
  int pos = random16(ledstrip->leds.size());
  leds[pos] += CHSV( gHue + random8(64), 200, ledstrip->maxBrightness);
}

void bpm(CRGB *leds, Ledstrip *ledstrip, int gHue)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  double factor = ((double) ledstrip->maxBrightness) / 255.0;
  for( int i = 0; i < ledstrip->leds.size(); i++) { //9948
    CRGB color = ColorFromPalette(palette, (gHue+(i*2)), beat-gHue+(i*10), LINEARBLEND);
    color.red = (int)(((double) color.red) * factor);
    color.green = (int)(((double) color.green) * factor);
    color.blue = (int)(((double) color.blue) * factor);
    leds[i] = color;
  }
}

void juggle(CRGB *leds, Ledstrip *ledstrip) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, ledstrip->leds.size(), 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, ledstrip->leds.size()-1 )] |= CHSV(dothue, 200, ledstrip->maxBrightness);
    dothue += 32;
  }
}

