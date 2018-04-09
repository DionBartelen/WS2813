#ifndef _LED
#define _LED

class Led {
  public:
  Led(int n, int h, int th, int s, int ts, int b, int tb);
  int number;
  int hue;
  int targetHue;
  int maxHue;
  int saturation;
  int targetSaturation;
  int maxSaturation;
  int brightness;
  int targetBrightness;
  int maxBrightness;
  int GetNumber();
  void Activate(int bri);
  void DeActivate();
  void SetColor(int i);
  void SetColorFast(int i);
  void SetSaturation(int i);
  void SetSaturationFast(int i);
  void SetBrightness(int i);
  void SetBrightnessFast(int i);
  void FastOn(int bri);
  void FastOff();
  void Update();
};

#endif

