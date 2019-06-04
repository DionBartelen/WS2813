#include "Ledstrip.h"
#include <FastLED.h>

void Trail(int i, Ledstrip *ledstrip);
void Verspringen(int i, Ledstrip *ledstrip);
void Breathing(int i, Ledstrip *ledstrip);
void EenKleur(Ledstrip *ledstrip);
void RandomKnipperen(Ledstrip *ledstrip);
void TweeTrail(Ledstrip *ledstrip);
void Klok(Ledstrip *ledstrip);
void updateTime();
void SetTimeHours(int hours);
void SetTimeMinutes(int minutes);
void SetTimeSeconds(int seconds);
void SetTimeMillis(int milliseconds);
void SetTime(int hours, int minutes, int seconds, int milliseconds);
void confetti(CRGB *leds, Ledstrip *ledstrip, int gHue);
void bpm(CRGB *leds, Ledstrip *ledstrip, int gHue);
void juggle(CRGB *leds, Ledstrip *ledstrip);

