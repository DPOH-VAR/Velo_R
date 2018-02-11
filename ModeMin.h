#pragma once


#include "Arduino.h"
#include "LedControl.h"
#include "Mode.h"
#include "Colors.h"


class ModeMin: public Mode {
  public: 
    ModeMin();
    void init(LedControl* ctrl);
  
    unsigned short options();
    void tick();
    double getBeat();

};

ModeMin::ModeMin():Mode(){}

unsigned short ModeMin::options(){
  return 1;
}

void ModeMin::tick(){}

