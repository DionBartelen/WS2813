#include <vector>
#include "Led.h"

#ifndef _LEDSTRIP
#define _LEDSTRIP
class Ledstrip {
  public:
    Ledstrip(int ammountLeds, int maxBrightness);
    int amountLeds;
    int maxBrightness;
    std::vector<Led> leds;
    void TurnOn();
    void TurnOnFast();
    void TurnOff();
    void TurnOffFast();
    void SetAllColor(int hue, int sat, int bri);
    void SetAllHue(int hue);
    void SetAllHueFast(int hue);
    void SetSingleHue(int pos, int hue);
    void SetSingleHueFast(int pos, int hue);
    void SetAllSat(int sat);
    void SetAllSatFast(int sat);
    void SetSingleSat(int pos, int sat);
    void SetSingleSatFast(int pos, int sat);
    void SetAllBri(int bri);
    void SetAllBriFast(int bri);
    void SetSingleBri(int pos, int bri);
    void SetSingleBriFast(int pos, int bri);
    void Update();
    void SetRainbow();
    void SetWhite();
    void SetMaxBrightness(int i);
};

#endif

