#pragma once


#include "Arduino.h"
#include "LedControl.h"


class Mode {
  public: 
    Mode();
    ~Mode();
    void init(LedControl* ctrl);
    double getBeat();
    void setOption(unsigned short option);
    unsigned short getOption();
    void nextOption();
    void prevOption();
    
    virtual void tick();
    virtual unsigned short options();
    
  protected:
    const LedControl* _ctrl;
    unsigned short _option = 0;
    
};