#pragma once


#include "Arduino.h"
#include "LedControl.h"
#include "Mode.h"
#include "Color.h"
#include "Colors.h"



class ModeStatic: public Mode {
  public: 
    ModeStatic();
    ModeStatic(unsigned long periodOn, unsigned long periodOff);
    void init(LedControl* ctrl);
  
    unsigned short options();
    void tick();
    
  protected:
    unsigned long _periodOn = 0;
    unsigned long _period = 0;
};

ModeStatic::ModeStatic():Mode(){}
ModeStatic::ModeStatic(unsigned long periodOn, unsigned long periodOff):Mode(){
  _periodOn = periodOn;
  _period = periodOn + periodOff;
}

const uint32_t ModeStaticColors[] = {
  0x0000FF, 
  0x00FF00,
  0xFF0000,
  0xB0FF00,
  0xFF00FF,
  0xFFB000,
  0x00FFFF,
  0x404040,
  0xFFFFFF
};
const unsigned int ModeStaticColorsSize = sizeof(ModeStaticColors)/sizeof(*ModeStaticColors);

unsigned short ModeStatic::options(){
  return ModeStaticColorsSize + 1;
}

void ModeStatic::tick(){

  unsigned long time = _ctrl->getTime();
  
  uint32_t color = 0;
  if (_option < ModeStaticColorsSize) {
    color = ModeStaticColors[_option];
  } else {
    color = colorByHue(getBeat());
  }
  
  uint32_t colorChain = color;
  uint32_t colorBottom = color;
  
  if (_period > 0) {
    if (time%_period >= _periodOn){
      colorChain = COLOR_OFF;
    } 
    if ((time+_period/2)%_period >= _periodOn){
      colorBottom = COLOR_OFF;
    }
  }
  
  _ctrl->bottom->fill(colorBottom);
  _ctrl->chain->fill(colorChain);
}

