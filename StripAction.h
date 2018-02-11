#pragma once
#include "Arduino.h"
#include "LedControl.h"

namespace stripAction {
  
  void moveFwdBackPixel(LedControlPart* part, uint32_t color, double beat){
    unsigned int length = part->getLength();
    unsigned int backJumpCount = length*2 - 2;
    if (backJumpCount > 0) {
      unsigned int b = ((unsigned int)(beat*(double)backJumpCount))%backJumpCount;
      if (b >= length) b = backJumpCount - b;
      part->set(b, color);
    }
  }
  
  void arrowOpen(LedControlPart* part, uint32_t colorArrow, uint32_t colorFill, double beat){
    unsigned int length = part->getLength();
    unsigned int points = (length+1)/2;
    if (points <= 0) return;
    unsigned int pointTick = ((unsigned int) (beat * (double)points)) % points;
    unsigned int arrowPos = points - pointTick - 1;
    part->set(arrowPos, colorArrow);
    part->set(length-arrowPos-1, colorArrow);
    for (unsigned int i=arrowPos+1; i<pointTick; i++) {
      part->set(i, colorFill);
      part->set(length-i-1, colorFill);
    }
  }
  
  void arrowClose(LedControlPart* part, uint32_t colorArrow, uint32_t colorFill, double beat){
    unsigned int length = part->getLength();
    unsigned int points = (length+1)/2;
    if (points <= 0) return;
    unsigned int pointTick = ((unsigned int) (beat * (double)points)) % points;
    unsigned int arrowPos = points - pointTick - 1;
    part->set(arrowPos, colorArrow);
    part->set(length-arrowPos-1, colorArrow);
    for (unsigned int i=0; i<arrowPos; i++) {
      part->set(i, colorFill);
      part->set(length-i-1, colorFill);
    }
  }
  
}