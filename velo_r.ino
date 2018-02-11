#define PIN_CONNECT 13
#define PIN_BTN_L 9
#define PIN_BTN_R 11
#define TIME_DROP 3
#define TIME_DEBOUNCE 10
#define TIME_CLICK 200
#define BTNSTATE_DOWN LOW
#define BTNSTATE_UP HIGH
#define TIME_REFRESH_LED 1

#include "LedControl.h"
#include "State.h"


// TEST

LedControl* ledControl = 0;
void setup() {
  
  pinMode(PIN_CONNECT, INPUT_PULLUP);
  pinMode(PIN_BTN_L, INPUT_PULLUP);
  pinMode(PIN_BTN_R, INPUT_PULLUP);
  Serial.begin(9600);
  
  //testMetronome();
  ledControl = new LedControl();
  ledControl->init();
  ledControl->load();
  ledControl->setState(new StateMode());
  ledControl->show();
  
  resetButtons();
}


unsigned long time_sys = 0;
unsigned long refresh_point = 0;
void loop() {
  
  boolean hasConnection = handleConnection();
  if (!hasConnection) return;
  
  unsigned long time = millis();
  unsigned long dime_dif = time-time_sys;
  time_sys = time;
  unsigned long time_cur = ledControl->addTime(dime_dif);
  
  checkButtons();
  
  ledControl->tick();
  show();
  
}

void show(){
  ledControl->show();
  
  // примерная стабилизация времени
  // т.к. show останавливает обработку прерываний.
  // Отключить, если используется PWM
  ledControl->addTime(TIME_DROP);
}

byte connectstate = LOW;
boolean handleConnection(){
  byte connect_value = digitalRead(PIN_CONNECT);
  
  if (connect_value == HIGH && connectstate == LOW){ // disconnect
    ledControl->setState(new StateMode());
    ledControl->setMute(true);
    connectstate = connect_value;
    ledControl->fill(0);
    show();
    resetButtons();
    return false;
  } else if (connect_value == LOW && connectstate == HIGH) { // connect
    ledControl->load();
  }

  connectstate = connect_value;
  return connect_value == LOW;
}


byte btnstate_l = BTNSTATE_UP; byte btnstate_r = BTNSTATE_UP;
byte btnpushstate_l = BTNSTATE_UP; byte btnpushstate_r = BTNSTATE_UP;
boolean btndblclick_l = false; boolean btndblclick_r = false;
unsigned long btnstate_changetime_l = 0; unsigned long btnstate_changetime_r = 0;
void resetButtons() {
  btnstate_l = BTNSTATE_UP;
  btnstate_r = BTNSTATE_UP;
  btnpushstate_l = BTNSTATE_UP;
  btnpushstate_r = BTNSTATE_UP;
  btndblclick_l = false;
  btndblclick_r = false;
  btnstate_changetime_l = 0;
  btnstate_changetime_r = 0;
}
void checkButtons() {
  
  byte btnstate_cur_l = digitalRead(PIN_BTN_L);
  byte btnstate_cur_r = digitalRead(PIN_BTN_R);
  
  boolean btn_l_down     = btnstate_cur_l == BTNSTATE_DOWN;
  boolean btn_l_up       = btnstate_cur_l == BTNSTATE_UP;
  boolean btn_r_down     = btnstate_cur_r == BTNSTATE_DOWN;
  boolean btn_r_up       = btnstate_cur_r == BTNSTATE_UP;
  boolean btn_l_set_down = btnstate_l == BTNSTATE_UP && btn_l_down;
  boolean btn_l_set_up   = btnstate_l == BTNSTATE_DOWN && btn_l_up;
  boolean btn_r_set_down = btnstate_r == BTNSTATE_UP && btn_r_down;
  boolean btn_r_set_up   = btnstate_r == BTNSTATE_DOWN && btn_r_up;
  unsigned long dur_l    = durationFrom(btnstate_changetime_l);
  unsigned long dur_r    = durationFrom(btnstate_changetime_r);
  
  // обработка кликов
  boolean btn_l_fast_release = btn_l_set_up && dur_l > TIME_DEBOUNCE && dur_l <= TIME_CLICK;
  boolean btn_r_fast_release = btn_r_set_up && dur_r > TIME_DEBOUNCE && dur_r <= TIME_CLICK;
  
  
  if (btn_l_fast_release && btn_r_down && dur_r > TIME_DEBOUNCE && dur_r < TIME_CLICK) {
    btndblclick_r = true;
    send_event_click(dur_l, dur_r); // двойной клик 
  } else if (btn_r_fast_release && btn_l_down && dur_l > TIME_DEBOUNCE && dur_l < TIME_CLICK) {
    btndblclick_l = true;
    send_event_click(dur_l, dur_r); // двойной клик 
  } else if (btn_l_fast_release && btn_r_fast_release) { // редкий случай, отпустили одновременно
    send_event_click(dur_l, dur_r); // двойной клик 
  } else if (btn_l_fast_release && !btndblclick_l) {
    send_event_click(dur_l, 0); // только левый клик
  } else if (btn_r_fast_release && !btndblclick_r) {
    send_event_click(0, dur_r); // только правый клик
  }
  // очищаем btndblckick
  if (btn_l_up) btndblclick_l = false;
  if (btn_r_up) btndblclick_r = false;
  
  // обработка долгого нажатия
  boolean btn_l_hold = btnstate_l == BTNSTATE_DOWN && dur_l > TIME_CLICK;
  boolean btn_r_hold = btnstate_r == BTNSTATE_DOWN && dur_r > TIME_CLICK;
  if (btn_l_hold && !btn_r_hold && btnpushstate_l == BTNSTATE_UP && btnstate_r == BTNSTATE_DOWN && dur_r > TIME_DEBOUNCE) {
    btnpushstate_l = BTNSTATE_DOWN;
    btnpushstate_r = btn_r_down ? BTNSTATE_DOWN : BTNSTATE_UP;
    send_event_down(dur_l, dur_r); // двойной hold
  } else if (btn_r_hold && !btn_l_hold && btnpushstate_r == BTNSTATE_UP && btnstate_l == BTNSTATE_DOWN && dur_l > TIME_DEBOUNCE) {
    btnpushstate_l = btn_l_down ? BTNSTATE_DOWN : BTNSTATE_UP;
    btnpushstate_r = BTNSTATE_DOWN;
    send_event_down(dur_l, dur_r); // двойной hold
  } else if (btn_l_hold && btnpushstate_l == BTNSTATE_UP && btn_r_hold && btnpushstate_r == BTNSTATE_UP) { // одновременно зажали
    btnpushstate_l = BTNSTATE_DOWN;
    btnpushstate_r = BTNSTATE_DOWN;
    send_event_down(dur_l, dur_r); // двойной hold
  } else if (btn_l_hold && btnpushstate_l == BTNSTATE_UP) {
    btnpushstate_l = BTNSTATE_DOWN;
    send_event_down(dur_l, 0); // зажали левый
  } else if (btn_r_hold && btnpushstate_r == BTNSTATE_UP) {
    btnpushstate_r = BTNSTATE_DOWN;
    send_event_down(0, dur_r); // зажали правый
  }
  
  // ообработка отпускания
  if (btn_l_set_up && btnpushstate_l == BTNSTATE_DOWN){
    btnpushstate_l = BTNSTATE_UP;
    send_event_up(dur_l, 0); // отпустили левый
  }
  if (btn_r_set_up && btnpushstate_r == BTNSTATE_DOWN){
    btnpushstate_r = BTNSTATE_UP;
    send_event_up(0, dur_r); // отпустили правый
  }
  
  // таймер по изменению:
  if (btn_l_set_down) btnstate_changetime_l = ledControl->getTime();
  if (btn_r_set_down) btnstate_changetime_r = ledControl->getTime();
  
  btnstate_l = btnstate_cur_l;
  btnstate_r = btnstate_cur_r;
}

unsigned long durationFrom(unsigned long from){
  return ledControl->getTime() - from;
}

void send_event_click(unsigned long dur_l, unsigned long dur_r){
  if (dur_l && dur_r) {
    Serial.print("CLICK L+R: ");
    Serial.print(dur_l);
    Serial.print(",");
    Serial.println(dur_r);
  } else if (dur_l) {
    Serial.print("CLICK L: ");
    Serial.println(dur_l);
  } else if (dur_r) {
    Serial.print("CLICK R: ");
    Serial.println(dur_r);
  } else  {
    Serial.println("CLICK ERROR");
  }
  ledControl->click(dur_l, dur_r);
}

void send_event_down(unsigned long dur_l, unsigned long dur_r){
  if (dur_l && dur_r) {
    Serial.print("DOWN L+R: ");
    Serial.print(dur_l);
    Serial.print(",");
    Serial.println(dur_r);
  } else if (dur_l) {
    Serial.print("DOWN L: ");
    Serial.println(dur_l);
  } else if (dur_r) {
    Serial.print("DOWN R: ");
    Serial.println(dur_r);
  } else  {
    Serial.println("DOWN ERROR");
  }
  ledControl->down(dur_l, dur_r);
}

void send_event_up(unsigned long dur_l, unsigned long dur_r){
  if (dur_l && dur_r) {
    Serial.print("UP L+R: ");
    Serial.print(dur_l);
    Serial.print(",");
    Serial.println(dur_r);
  } else if (dur_l) {
    Serial.print("UP L: ");
    Serial.println(dur_l);
  } else if (dur_r) {
    Serial.print("UP R: ");
    Serial.println(dur_r);
  } else  {
    Serial.println("UP ERROR");
  }
  ledControl->up(dur_l, dur_r);
}

















