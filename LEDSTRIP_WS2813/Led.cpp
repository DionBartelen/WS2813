#include "Led.h"

int difHue = 2;
int difSat = 2;
int difBri = 8; //fade speed

Led::Led(int n, int h, int th, int s, int ts, int b, int tb) {
  number = n;
  hue = h;
  targetHue = th;
  maxHue = 255;
  saturation = s;
  targetSaturation = ts;
  maxSaturation = 255;
  brightness = b;
  targetBrightness = tb;
  maxBrightness = 255;
}

int Led::GetNumber() {
  return number;
}

void Led::Activate(int bri) {
  targetBrightness = bri; 
}
 
void Led::DeActivate() {
  targetBrightness = 0;
}

void Led::SetColor(int i) {
  targetHue = i % 255;
}

void Led::SetColorFast(int i) {
  targetHue = i % maxHue;
  hue = i % maxHue;
}

void Led::SetSaturation(int i) {
  targetSaturation = i;
}

void Led::SetSaturationFast(int i) {
  targetSaturation = i;
  saturation = i;
}

void Led::SetBrightness(int i) {
  targetBrightness = i;
}

void Led::SetBrightnessFast(int i) {
  targetBrightness = i;
  brightness = i;
}

void Led::FastOn(int bri) {
  brightness = bri;
  targetBrightness = bri;
}

void Led::FastOff() {
  brightness = 0;
  targetBrightness = 0;
}

void Led::Update() {
  if(hue - targetHue > difHue) {
    hue -= difHue;
  } else if(hue - targetHue < difHue) {
    hue += difHue;
  }
  if(hue > maxHue) {
    hue = maxHue;
  }
  if(hue < 0) {
    hue = 0;
  }
  
  if(saturation - targetSaturation > difSat) {
    saturation -= difSat;
  } else if(saturation - targetSaturation < difSat) {
    saturation += difSat;
  }
  if(saturation > maxSaturation) {
    saturation = maxSaturation;
  }
  if(saturation < 0) {
    saturation = 0;
  }
  
  if(brightness - targetBrightness > difBri) {
    brightness -= difBri;
  } else if(brightness - targetBrightness < difBri) {
    brightness += difBri;
  }
  if(brightness > maxBrightness) {
    brightness = maxBrightness;
  }
  if(brightness < 0) {
    brightness = 0;
  }
}

