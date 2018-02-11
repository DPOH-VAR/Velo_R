#pragma once


#include "Arduino.h"
#include "State.h"
#include "Mode.h"
#include <Adafruit_NeoPixel.h>

#define LED_CONTROL_STRIP_COUNT 2
#define LED_CONTROL_STRIP_PIN_0 5
#define LED_CONTROL_STRIP_LEN_0 71
#define LED_CONTROL_STRIP_PIN_1 7
#define LED_CONTROL_STRIP_LEN_1 77

#define LED_CONTROL_BOTTOM_STRIP 0
#define LED_CONTROL_BOTTOM_FROM 0
#define LED_CONTROL_BOTTOM_LEN 34
#define LED_CONTROL_BOTTOM_FWD false

#define LED_CONTROL_FRONT_STRIP 0
#define LED_CONTROL_FRONT_FROM 34
#define LED_CONTROL_FRONT_LEN 5
#define LED_CONTROL_FRONT_FWD true

#define LED_CONTROL_FORK_R_STRIP 0
#define LED_CONTROL_FORK_R_FROM 39
#define LED_CONTROL_FORK_R_LEN 16
#define LED_CONTROL_FORK_R_FWD true

#define LED_CONTROL_FORK_L_STRIP 0
#define LED_CONTROL_FORK_L_FROM 55
#define LED_CONTROL_FORK_L_LEN 16
#define LED_CONTROL_FORK_L_FWD false

#define LED_CONTROL_SEAT_L_STRIP 1
#define LED_CONTROL_SEAT_L_FROM 0
#define LED_CONTROL_SEAT_L_LEN 21
#define LED_CONTROL_SEAT_L_FWD true

#define LED_CONTROL_CHAIN_L_STRIP 1
#define LED_CONTROL_CHAIN_L_FROM 21
#define LED_CONTROL_CHAIN_L_LEN 14
#define LED_CONTROL_CHAIN_L_FWD false

#define LED_CONTROL_CHAIN_R_STRIP 1
#define LED_CONTROL_CHAIN_R_FROM 35
#define LED_CONTROL_CHAIN_R_LEN 14
#define LED_CONTROL_CHAIN_R_FWD true

#define LED_CONTROL_SEAT_R_STRIP 1
#define LED_CONTROL_SEAT_R_FROM 49
#define LED_CONTROL_SEAT_R_LEN 21
#define LED_CONTROL_SEAT_R_FWD false

#define LED_CONTROL_BACK_STRIP 1
#define LED_CONTROL_BACK_FROM 70
#define LED_CONTROL_BACK_LEN 5
#define LED_CONTROL_BACK_FWD false

class LedControl;
class LedControlPart;
class State;

class LedControl {
  public:
    LedControl();
    
    const LedControlPart* front;
    const LedControlPart* bottom;
    const LedControlPart* forkLeft;
    const LedControlPart* forkRight;
    const LedControlPart* fork;
    const LedControlPart* seatLeft;
    const LedControlPart* seatRight;
    const LedControlPart* seat;
    const LedControlPart* chainLeft;
    const LedControlPart* chainRight;
    const LedControlPart* chain;
    const LedControlPart* back;
    
    void init();
    void set(unsigned int strip, unsigned int i, uint8_t r, uint8_t g, uint8_t b);
    void set(unsigned int strip, unsigned int i, uint32_t rgb);
    void fill(uint8_t r, uint8_t g, uint8_t b);
    void fill(uint32_t rgb);
    void show();
    
    void click(unsigned long dur_l, unsigned long dur_r);
    void up(unsigned long dur_l, unsigned long dur_r);
    void down(unsigned long dur_l, unsigned long dur_r);
    void tick();
    
    void setTime(unsigned long time);
    void setBeatStart(unsigned long time);
    void setBeatPeriod(unsigned long time);
    unsigned long getTime();
    unsigned long addTime(unsigned long time);
    unsigned long getBeatStart();
    unsigned long getBeatPeriod();
    double getBeat();
    boolean setState(State* state);
    
    void nextMode();
    void prevMode();
    
    Mode* getMode();
    unsigned short getModeCount();
    
    boolean getMute();
    void setMute(boolean mute);
    
    void load();
    void save();
    
  private:
    const Adafruit_NeoPixel* _pixels[LED_CONTROL_STRIP_COUNT];
    ~LedControl();
    
    unsigned long _time = 0;
    unsigned long _beatStart = 0;
    unsigned long _beatPeriod = 2000;
    State* _state = 0;
    boolean _mute;
    
    unsigned short _modeIndex = 0;
    Mode** _modes;
    unsigned short _modeCount;
};


class LedControlPart {
  public:
    LedControlPart();
    
    virtual unsigned int getLength();
    virtual void set(unsigned int i, uint8_t r, uint8_t g, uint8_t b);
    virtual void set(unsigned int i, uint32_t rgb);
    virtual void fill(uint8_t r, uint8_t g, uint8_t b);
    virtual void fill(uint32_t rgb);
};



class LedControlSegment: public LedControlPart {
  public:
    LedControlSegment(LedControl* ctrl, unsigned int strip, unsigned int from, unsigned int len, boolean fwd);
    
    unsigned int getLength();
    void set(unsigned int i, uint8_t r, uint8_t g, uint8_t b);
    void set(unsigned int i, uint32_t rgb);
    void fill(uint8_t r, uint8_t g, uint8_t b);
    void fill(uint32_t rgb);
  private:
    unsigned int _length;
    unsigned int _strip;
    const LedControl* _ctrl;
    unsigned int _from;
    boolean _fwd;

};



class LedControlUnion: public LedControlPart {
  public:
    LedControlUnion(unsigned int count, ...);
    
    unsigned int getLength();
    unsigned int getCount();
    void set(unsigned int i, uint8_t r, uint8_t g, uint8_t b);
    void set(unsigned int i, uint32_t rgb);
    void fill(uint8_t r, uint8_t g, uint8_t b);
    void fill(uint32_t rgb);
    LedControlPart* getPart(unsigned int index);
  private:
    ~LedControlUnion();
    unsigned int _length;
    unsigned int _count;
    LedControlPart** _parts;

};



