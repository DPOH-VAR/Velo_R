#pragma once


#include "Arduino.h"
#include "LedControl.h"
#include "Mode.h"
#include "Color.h"


class ModeRainbow: public Mode {
  public: 
    ModeRainbow();
    ModeRainbow(boolean pulse);
    void init(LedControl* ctrl);
  
    unsigned short options();
    void tick();
    
  protected:
    boolean _pulse = false;
};

ModeRainbow::ModeRainbow():Mode(){}
ModeRainbow::ModeRainbow(boolean pulse):Mode(){
  _pulse = pulse;
}

unsigned short ModeRainbow::options(){
  return 4;
}

void ModeRainbow::tick(){
  
  LedControlPart* parts[] = {_ctrl->seat, _ctrl->chain, _ctrl->bottom, _ctrl->forkLeft, _ctrl->forkRight};
  unsigned short partsCount = sizeof(parts) / sizeof(*parts);
  
  boolean fastSpeed = (_option & (1 << 0)) > 0;
  boolean doubleFreq = (_option & (1 << 1)) > 0;
  
  double beat = getBeat();
  double beatShift = beat*2.0*PI / 4;
  if (fastSpeed) beatShift = beatShift * 3;
  
  for (unsigned int i=0; i<partsCount; i++){
    const LedControlPart* part = parts[i];
    const unsigned int length = part->getLength();
    
    for (unsigned int px=0; px<length; px++){
      double pixelPos = ((double)px)/((double)length);
      
      uint32_t color;
      if (doubleFreq) {
        color = colorByHue(pixelPos*4.0*PI + beatShift);
      } else {
        color = colorByHue(pixelPos*2.0*PI + beatShift);
      }
      if (_pulse) {
        int floorBeat = (int) beat;
        double brt = ((double)(floorBeat + 1)) - beat;
        color = setBrightness(color, brt*brt*brt);
      }
      part->set(px, color);
    }
  }
}

