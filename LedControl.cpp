#pragma once

#include "LedControl.h"
#include "Arduino.h"
#include "State.h"
#include "Mode.h"
#include "ModeStatic.h"
#include "ModeMin.h"
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>


LedControl::LedControl() {
  #if LED_CONTROL_STRIP_COUNT > 0
    _pixels[0] = new Adafruit_NeoPixel(LED_CONTROL_STRIP_LEN_0, LED_CONTROL_STRIP_PIN_0, NEO_GRB + NEO_KHZ800);
  #endif
  #if LED_CONTROL_STRIP_COUNT > 1
    _pixels[1] = new Adafruit_NeoPixel(LED_CONTROL_STRIP_LEN_1, LED_CONTROL_STRIP_PIN_1, NEO_GRB + NEO_KHZ800);
  #endif
  #if LED_CONTROL_STRIP_COUNT > 2
    _pixels[2] = new Adafruit_NeoPixel(LED_CONTROL_STRIP_LEN_2, LED_CONTROL_STRIP_PIN_2, NEO_GRB + NEO_KHZ800);
  #endif
  #if LED_CONTROL_STRIP_COUNT > 3
    _pixels[3] = new Adafruit_NeoPixel(LED_CONTROL_STRIP_LEN_3, LED_CONTROL_STRIP_PIN_3, NEO_GRB + NEO_KHZ800);
  #endif
  
  _mute = true;
  
  bottom     = new LedControlSegment(this, LED_CONTROL_BOTTOM_STRIP,  LED_CONTROL_BOTTOM_FROM,  LED_CONTROL_BOTTOM_LEN,  LED_CONTROL_BOTTOM_FWD  );
  front      = new LedControlSegment(this, LED_CONTROL_FRONT_STRIP,   LED_CONTROL_FRONT_FROM,   LED_CONTROL_FRONT_LEN,   LED_CONTROL_FRONT_FWD   );
  forkLeft   = new LedControlSegment(this, LED_CONTROL_FORK_L_STRIP,  LED_CONTROL_FORK_L_FROM,  LED_CONTROL_FORK_L_LEN,  LED_CONTROL_FORK_L_FWD  );
  forkRight  = new LedControlSegment(this, LED_CONTROL_FORK_R_STRIP,  LED_CONTROL_FORK_R_FROM,  LED_CONTROL_FORK_R_LEN,  LED_CONTROL_FORK_R_FWD  );
  seatLeft   = new LedControlSegment(this, LED_CONTROL_SEAT_L_STRIP,  LED_CONTROL_SEAT_L_FROM,  LED_CONTROL_SEAT_L_LEN,  LED_CONTROL_SEAT_L_FWD  );
  seatRight  = new LedControlSegment(this, LED_CONTROL_SEAT_R_STRIP,  LED_CONTROL_SEAT_R_FROM,  LED_CONTROL_SEAT_R_LEN,  LED_CONTROL_SEAT_R_FWD  );
  chainLeft  = new LedControlSegment(this, LED_CONTROL_CHAIN_L_STRIP, LED_CONTROL_CHAIN_L_FROM, LED_CONTROL_CHAIN_L_LEN, LED_CONTROL_CHAIN_L_FWD );
  chainRight = new LedControlSegment(this, LED_CONTROL_CHAIN_R_STRIP, LED_CONTROL_CHAIN_R_FROM, LED_CONTROL_CHAIN_R_LEN, LED_CONTROL_CHAIN_R_FWD );
  back       = new LedControlSegment(this, LED_CONTROL_BACK_STRIP,    LED_CONTROL_BACK_FROM,    LED_CONTROL_BACK_LEN,    LED_CONTROL_BACK_FWD    );
  
  fork       = new LedControlUnion(2, forkLeft,  forkRight  );
  seat       = new LedControlUnion(2, seatLeft,  seatRight  );
  chain      = new LedControlUnion(2, chainLeft, chainRight );
  
  _modeCount = 3;
  _modes = new Mode*[_modeCount]{
    new ModeMin(),
    new ModeStatic(),
    new ModeStatic(20, 150)
  };
  for (unsigned short i=0; i<_modeCount; i++){
    _modes[i]->init(this);
  }
}
LedControl::~LedControl(){
  delete front;
  delete[] _pixels;
  for (unsigned short i=0; i<_modeCount; i++){
    delete _modes[i];
  }
  delete[] _modes;
}
void LedControl::init() {
  for(unsigned int i=0; i<LED_CONTROL_STRIP_COUNT; i++){
    _pixels[i]->begin();
  }
}
void LedControl::set(unsigned int strip, unsigned int i, uint8_t r, uint8_t g, uint8_t b){
  _pixels[strip]->setPixelColor(i, r, g, b);
}
void LedControl::set(unsigned int strip, unsigned int i, uint32_t rgb){
  _pixels[strip]->setPixelColor(i, rgb);
}
void LedControl::show() {
  for(unsigned int i=0; i<LED_CONTROL_STRIP_COUNT; i++){
    _pixels[i]->show();
  }
}
void LedControl::setTime(unsigned long time){
  _time = time;
}
    
void LedControl::setBeatStart(unsigned long time){
  _beatStart = time;
}
    
void LedControl::setBeatPeriod(unsigned long time){
  _beatPeriod = time;
}

unsigned long LedControl::getTime(){
  return _time;
}
unsigned long LedControl::addTime(unsigned long time){
  _time = _time + time;
  return _time;
}

unsigned long LedControl::getBeatStart(){
  return _beatStart;
}

unsigned long LedControl::getBeatPeriod(){
  return _beatPeriod;
}

double LedControl::getBeat(){
  return ((double)(_time-_beatStart)) / ((double)_beatPeriod);
}
void LedControl::fill(uint32_t rgb){
  for (unsigned int i=0; i<LED_CONTROL_STRIP_COUNT; i++){
    Adafruit_NeoPixel* pixels = _pixels[i];
    unsigned int num = pixels->numPixels();
    for (unsigned int i=0; i<num; i++){
      pixels->setPixelColor(i, rgb);
    }
  }
}

void LedControl::fill(uint8_t r, uint8_t g, uint8_t b){
  for (unsigned int i=0; i<LED_CONTROL_STRIP_COUNT; i++){
    Adafruit_NeoPixel* pixels = _pixels[i];
    unsigned int num = pixels->numPixels();
    for (unsigned int i=0; i<num; i++){
      pixels->setPixelColor(i, r, g, b);
    }
  }
}

boolean LedControl::setState(State* state){
  boolean initResult = state->init(this);
  if (!initResult) return false;
  delete _state;
  _state = state;
  return true;
}

void LedControl::click(unsigned long dur_l, unsigned long dur_r){
  _state->click(dur_l, dur_r);
}

void LedControl::up(unsigned long dur_l, unsigned long dur_r){
  _state->up(dur_l, dur_r);
}

void LedControl::down(unsigned long dur_l, unsigned long dur_r){
  _state->down(dur_l, dur_r);
}

void LedControl::tick(){
  _state->tick();
}
boolean LedControl::getMute(){
  return _mute;
}
void LedControl::setMute(boolean mute){
  _mute = mute;
}
Mode* LedControl::getMode(){
  return _modes[_modeIndex];
}
unsigned short LedControl::getModeCount(){
  return _modeCount;
}
void LedControl::nextMode(){
  if (_modeIndex + 1 == _modeCount) {
    _modeIndex = 0;
  } else {
    _modeIndex++;
  }
};

void LedControl::prevMode(){
  if (_modeIndex == 0 && _modeCount > 0) {
    _modeIndex = _modeCount - 1;
  } else {
    _modeIndex--;
  }
};

void LedControl::save(){
  unsigned int addr = 0;
  EEPROM.put(addr, _modeIndex);
  addr += sizeof(_modeIndex);
  EEPROM.put(addr, _beatPeriod);
  addr += sizeof(_beatPeriod);
  for (unsigned short i = 0; i<_modeCount; i++) {
    Mode* mode = _modes[i];
    EEPROM.put(addr, mode->getOption());
    addr += sizeof(mode->getOption());
  }
};

void LedControl::load(){
  unsigned int addr = 0;
  EEPROM.get(addr, _modeIndex);
  addr += sizeof(_modeIndex);
  EEPROM.get(addr, _beatPeriod);
  addr += sizeof(_beatPeriod);
  for (unsigned short i = 0; i<_modeCount; i++) {
    Mode* mode = _modes[i];
    auto option = mode->getOption();
    EEPROM.get(addr, option);
    mode->setOption(option);
    addr += sizeof(option);
  }
};




LedControlPart::LedControlPart(){}



LedControlSegment::LedControlSegment(LedControl* ctrl, unsigned int strip, unsigned int from, unsigned int len, boolean fwd): LedControlPart(){
  _length = len;
  _strip = strip;
  _ctrl = ctrl;
  _from = from;
  _fwd = fwd;
}
unsigned int LedControlSegment::getLength(){
  return _length;
}
void LedControlSegment::set(unsigned int i, uint8_t r, uint8_t g, uint8_t b){
  if (i < 0 || i >= _length) return;
  if (!_fwd) i = _length - 1 - i;
  _ctrl->set(_strip, _from + i, r, g, b);
}
void LedControlSegment::set(unsigned int i, uint32_t rgb){
  if (i < 0 || i >= _length) return;
  if (!_fwd) i = _length - 1 - i;
  _ctrl->set(_strip, _from + i, rgb);
}
void LedControlSegment::fill(uint8_t r, uint8_t g, uint8_t b){
  unsigned int to = _from + _length;
  for(unsigned int i=_from; i<to; i++){
    _ctrl->set(_strip, i, r, g, b);
  }
}
void LedControlSegment::fill(uint32_t rgb){
  unsigned int to = _from + _length;
  for(unsigned int i=_from; i<to; i++){
    _ctrl->set(_strip, i, rgb);
  }
}




LedControlUnion::LedControlUnion(unsigned int count, ...): LedControlPart(){
  _count = count;
  va_list args;
  va_start(args,count);
  _parts = malloc(sizeof(LedControlPart*) * count);
  for (unsigned int i=0; i<_count; i++){
    _parts[i] = va_arg(args, LedControlPart*);
  }
  va_end(args);
  _length = 0;
  for (unsigned int i=0; i<_count; i++){
    unsigned int len = _parts[i]->getLength();
    if (len > _length) _length = len;
  }
}
LedControlUnion::~LedControlUnion(){
  free(_parts);
}
LedControlPart* LedControlUnion::getPart(unsigned int i){
  if (i < 0 || i >= _count) return NULL;
  return _parts[i];
}
unsigned int LedControlUnion::getLength(){
  return _length;
}
unsigned int LedControlUnion::getCount(){
  return _count;
}
void LedControlUnion::set(unsigned int i, uint8_t r, uint8_t g, uint8_t b){
  for (unsigned int i=0; i<_count; i++){
    _parts[i]->set(i, r, g, b);
  }
}
void LedControlUnion::set(unsigned int i, uint32_t rgb){
  for (unsigned int i=0; i<_count; i++){
    _parts[i]->set(i, rgb);
  }
}
void LedControlUnion::fill(uint8_t r, uint8_t g, uint8_t b){
  for (unsigned int i=0; i<_count; i++){
    _parts[i]->fill(r, g, b);
  }
}
void LedControlUnion::fill(uint32_t rgb){
  for (unsigned int i=0; i<_count; i++){
    _parts[i]->fill(rgb);
  }
}






