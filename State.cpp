#pragma once

#include "LedControl.h"
#include "Arduino.h"
#include "State.h"
#include "Colors.h"
#include "StripAction.h"
#include <Adafruit_NeoPixel.h>

#define STATE_TURN_PERIOD_UP    100
#define STATE_TURN_PERIOD_ON    120
#define STATE_TURN_PERIOD_DOWN  100
#define STATE_TURN_PERIOD_OFF   120

#define STATE_STOP_PERIOD_ON  25
#define STATE_STOP_PERIOD_OFF 25

State::State(){}
boolean State::init(LedControl* ctrl){
  if (_ctrl) return false;
  _ctrl = ctrl;
  _start = ctrl->getTime();
  return onInit();
}
boolean State::changeState(State* state){
  return _ctrl->setState(state); 
}
unsigned long State::getDuration(){
  return _ctrl->getTime() - _start;
}
boolean State::onInit(){
  return true;
}



StateMode::StateMode(): State(){}
boolean StateMode::onInit(){
  if (_ctrl->getMute()) _ctrl->fill(COLOR_OFF);
  return true;
}
void StateMode::click(unsigned long durLeft, unsigned long durRight){
  if (durLeft && durRight){
    _ctrl->setMute( !_ctrl->getMute() );
    if (_ctrl->getMute()) _ctrl->fill(COLOR_OFF);
  } else if (durLeft) {
    if (_ctrl->getMute()) _ctrl->setMute(false);
    else _ctrl->prevMode();
  } else if (durRight) {
    if (_ctrl->getMute()) _ctrl->setMute(false);
    else _ctrl->nextMode();
  }
}
void StateMode::down(unsigned long durLeft, unsigned long durRight){
  if (durLeft && durRight){
    changeState(new StateStop());
  } else if (durLeft) {
    changeState(new StateTurnLeft());
  } else if (durRight) {
    changeState(new StateTurnRight());
  }
}
void StateMode::up(unsigned long durLeft, unsigned long durRight){ /* imposibru! */ }
void StateMode::tick(){
  if (_ctrl->getMute()) return;
  _ctrl->fill(COLOR_OFF);
  stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
  stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  _ctrl->getMode()->tick();
}



StateTurnLeft::StateTurnLeft(): State(){}
boolean StateTurnLeft::onInit(){
  _ctrl->fill(COLOR_OFF);
  return true;
}
void StateTurnLeft::click(unsigned long durLeft, unsigned long durRight){
  if (_ctrl->getMute()) return;
  changeState(new StateTurnLeftMode());
}
void StateTurnLeft::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateTurnLeft::up(unsigned long durLeft, unsigned long durRight){
  if (durLeft){
    changeState(new StateMode());
    return;
  }
}
void StateTurnLeft::tick(){
  _ctrl->fill(COLOR_OFF);
  if (!_ctrl->getMute()) {
    stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
    stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  }
  LedControlPart* seatOrt = _ctrl->seatLeft;
  LedControlPart* forkOrt = _ctrl->forkLeft;
  unsigned long time = getDuration();
  unsigned long perTime = time % (STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON + STATE_TURN_PERIOD_DOWN + STATE_TURN_PERIOD_OFF);
  seatOrt->fill(COLOR_OFF);
  forkOrt->fill(COLOR_OFF);
  if (perTime < STATE_TURN_PERIOD_UP) {
    double turnArrowBeat = ((double)perTime) / ((double)STATE_TURN_PERIOD_UP);
    stripAction::arrowOpen(seatOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
    stripAction::arrowOpen(forkOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
  } else if (perTime < STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON) {
    seatOrt->fill(COLOR_TURN);
    forkOrt->fill(COLOR_TURN);
  } else if (perTime < STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON + STATE_TURN_PERIOD_DOWN) {
    double turnArrowBeat = ((double)perTime - STATE_TURN_PERIOD_UP - STATE_TURN_PERIOD_ON) / ((double)STATE_TURN_PERIOD_DOWN);
    stripAction::arrowClose(seatOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
    stripAction::arrowClose(forkOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
  }
}




StateTurnLeftMode::StateTurnLeftMode(): State(){}
boolean StateTurnLeftMode::onInit(){
  _ctrl->fill(COLOR_OFF);
  //_ctrl->getMode()->nextOption();
  return true;
}
void StateTurnLeftMode::click(unsigned long durLeft, unsigned long durRight){
  _ctrl->getMode()->nextOption();
}
void StateTurnLeftMode::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateTurnLeftMode::up(unsigned long durLeft, unsigned long durRight){
  changeState(new StateMode());
}
void StateTurnLeftMode::tick(){
  if (_ctrl->getMute()) return;
  _ctrl->fill(COLOR_OFF);
  stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
  stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  
  _ctrl->getMode()->tick();
}




StateTurnRight::StateTurnRight(): State(){}
boolean StateTurnRight::onInit(){
  _ctrl->fill(COLOR_OFF);
  return true;
}
void StateTurnRight::click(unsigned long durLeft, unsigned long durRight){
  if (_ctrl->getMute()) return;
   changeState(new StateTurnRightMode());
}
void StateTurnRight::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateTurnRight::up(unsigned long durLeft, unsigned long durRight){
  if (durRight){
    changeState(new StateMode());
    return;
  }
}
void StateTurnRight::tick(){
  _ctrl->fill(COLOR_OFF);
  if (!_ctrl->getMute()) {
    stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
    stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  }
  LedControlPart* seatOrt = _ctrl->seatRight;
  LedControlPart* forkOrt = _ctrl->forkRight;
  unsigned long time = getDuration();
  unsigned long perTime = time % (STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON + STATE_TURN_PERIOD_DOWN + STATE_TURN_PERIOD_OFF);
  seatOrt->fill(COLOR_OFF);
  forkOrt->fill(COLOR_OFF);
  if (perTime < STATE_TURN_PERIOD_UP) {
    double turnArrowBeat = ((double)perTime) / ((double)STATE_TURN_PERIOD_UP);
    stripAction::arrowOpen(seatOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
    stripAction::arrowOpen(forkOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
  } else if (perTime < STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON) {
    seatOrt->fill(COLOR_TURN);
    forkOrt->fill(COLOR_TURN);
  } else if (perTime < STATE_TURN_PERIOD_UP + STATE_TURN_PERIOD_ON + STATE_TURN_PERIOD_DOWN) {
    double turnArrowBeat = ((double)perTime - STATE_TURN_PERIOD_UP - STATE_TURN_PERIOD_ON) / ((double)STATE_TURN_PERIOD_DOWN);
    stripAction::arrowClose(seatOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
    stripAction::arrowClose(forkOrt, COLOR_TURN_A, COLOR_TURN, turnArrowBeat);
  }
  
}







StateTurnRightMode::StateTurnRightMode(): State(){}
boolean StateTurnRightMode::onInit(){
  _ctrl->fill(COLOR_OFF);
  //_ctrl->getMode()->prevOption();
  return true;
}
void StateTurnRightMode::click(unsigned long durLeft, unsigned long durRight){
  _ctrl->getMode()->prevOption();
}
void StateTurnRightMode::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateTurnRightMode::up(unsigned long durLeft, unsigned long durRight){
  changeState(new StateMode());
}
void StateTurnRightMode::tick(){
  if (_ctrl->getMute()) return;
  _ctrl->fill(COLOR_OFF);
  stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
  stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  
  _ctrl->getMode()->tick();
}




StateStop::StateStop(): State(){}
boolean StateStop::onInit(){
  _ctrl->fill(COLOR_OFF);
  return true;
}
void StateStop::click(unsigned long durLeft, unsigned long durRight){
  // impossibru!
}
void StateStop::down(unsigned long durLeft, unsigned long durRight){
  // umpossibru!
}
void StateStop::up(unsigned long durLeft, unsigned long durRight){
  if (durLeft) {
    changeState(new StateSaveMode());
  } else if (durRight) {
    changeState(new StateMetronomeMode());
  }
}
void StateStop::tick(){
  _ctrl->fill(COLOR_OFF);
  if (!_ctrl->getMute()) {
    stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
    stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  }
  
  unsigned long time = getDuration();
  unsigned long perTime = time % (STATE_STOP_PERIOD_ON + STATE_STOP_PERIOD_OFF);
  uint32_t colorBack = perTime < STATE_STOP_PERIOD_ON ? COLOR_STOP : COLOR_OFF;
  uint32_t colorChain = perTime < STATE_STOP_PERIOD_ON ?COLOR_OFF : COLOR_STOP;
  _ctrl->back->fill(colorBack);
  _ctrl->chain->fill(colorChain);
}




StateSaveMode::StateSaveMode(): State(){}
boolean StateSaveMode::onInit(){
  _ctrl->fill(COLOR_OFF);
  return true;
}
void StateSaveMode::click(unsigned long durLeft, unsigned long durRight){
  _ctrl->save();
}
void StateSaveMode::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateSaveMode::up(unsigned long durLeft, unsigned long durRight){
  changeState(new StateMode());
}
void StateSaveMode::tick(){
  if (_ctrl->getMute()) return;
  _ctrl->fill(COLOR_OFF);
  stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
  stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());
  
  _ctrl->getMode()->tick();
}




StateMetronomeMode::StateMetronomeMode(): State(){}
boolean StateMetronomeMode::onInit(){
  _ctrl->fill(COLOR_OFF);
  resetMetronome();
  return true;
}
void StateMetronomeMode::resetMetronome(){
  _beat_count = 0;
}
void StateMetronomeMode::setMetronome(){
  setMetronome(_ctrl->getTime());
}
void StateMetronomeMode::setMetronome(unsigned long value){
  if (_beat_count == 0) {
    _ctrl->setBeatStart(value);
  } else if (_beat_count == 1){
    _ctrl->setBeatPeriod(value - _last_beat);
  } else {
    const unsigned long beat_period = _ctrl->getBeatPeriod();
    unsigned long period = value - _last_beat;
    long period_dif = (long)(period - beat_period);
    long period_aver_dif = period_dif/(_beat_count);
    _ctrl->setBeatPeriod(beat_period + period_aver_dif);
  }
  _last_beat = value;
  _beat_count++;
}
void StateMetronomeMode::click(unsigned long durLeft, unsigned long durRight){
  setMetronome(_ctrl->getTime() - durRight);
}
void StateMetronomeMode::down(unsigned long durLeft, unsigned long durRight){
  changeState(new StateStop());
}
void StateMetronomeMode::up(unsigned long durLeft, unsigned long durRight){
  changeState(new StateMode());
}
void StateMetronomeMode::tick(){
  if (_ctrl->getMute()) return;
  _ctrl->fill(COLOR_OFF);
  stripAction::moveFwdBackPixel(_ctrl->front, COLOR_LIGHT, _ctrl->getBeat());
  stripAction::moveFwdBackPixel(_ctrl->back, COLOR_STOP, _ctrl->getBeat());

  _ctrl->getMode()->tick();
}