#pragma once

#include "LedControl.h"
#include "Arduino.h"
#include "Mode.h"

Mode::Mode(){}
Mode::~Mode(){}

void Mode::init(LedControl* ctrl){
  _ctrl = ctrl;
}

double Mode::getBeat(){
  return _ctrl->getBeat();
};

unsigned short Mode::getOption(){
  return _option;
};

void Mode::setOption(unsigned short value){
  _option = value % options();
};

void Mode::nextOption(){
  unsigned short opt = options();
  if (_option + 1 == opt) {
    _option = 0;
  } else {
    _option++;
  }
};

void Mode::prevOption(){
  unsigned short opt = options();
  if (_option == 0 && opt > 0) {
    _option = opt - 1;
  } else {
    _option--;
  }
};