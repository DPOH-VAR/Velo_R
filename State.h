#pragma once


#include "Arduino.h"
#include "LedControl.h"

class LedControl;

class State {
  public:
    State();
    boolean init(LedControl* ctrl);
    boolean changeState(State* state);
    unsigned long getDuration();
    
    virtual void tick();
    virtual void click(unsigned long durLeft, unsigned long durRight);
    virtual void down(unsigned long durLeft, unsigned long durRight);
    virtual void up(unsigned long durLeft, unsigned long durRight);
    
  protected: 
    virtual boolean onInit();
    const LedControl* _ctrl = 0;
    unsigned long _start = 0;
};



class StateMode: public State {
  public:
    StateMode();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};



class StateTurnLeft: public State {
  public:
    StateTurnLeft();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};



class StateTurnLeftMode: public State {
  public:
    StateTurnLeftMode();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};



class StateTurnRight: public State {
  public:
    StateTurnRight();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};




class StateTurnRightMode: public State {
  public:
    StateTurnRightMode();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};




class StateStop: public State {
  public:
    StateStop();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};




class StateSaveMode: public State {
  public:
    StateSaveMode();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
};




class StateMetronomeMode: public State {
  public:
    StateMetronomeMode();
    
    void tick();
    void click(unsigned long durLeft, unsigned long durRight);
    void down(unsigned long durLeft, unsigned long durRight);
    void up(unsigned long durLeft, unsigned long durRight);
    
  protected:
    boolean onInit();
    void resetMetronome();
    void setMetronome();
    void setMetronome(unsigned long time);
    int long _beat_count = 0;
    unsigned long _last_beat = 0;
};
