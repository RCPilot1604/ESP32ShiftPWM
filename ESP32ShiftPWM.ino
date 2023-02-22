#include "ESP32ShiftPWM.h"
#define FREQUENCY 100000 //Frequency of update in Hz
ESP32ShiftPWM sr(BIT14, BIT12, BIT4, 3, 255); //Data Pin, Clock Pin, Latch Pin, Register Count, Resolution
hw_timer_t * my_timer = NULL;

void IRAM_ATTR onTimer(){
  sr.update();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  my_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(my_timer, &onTimer, true);
  //since we want the interrupt to be called at (freq)Hz, we need to set the alarm to fire every 1/(this->freq) s = 1/(this->freq)* 10^6 ticks
  timerAlarmWrite(my_timer, 1000000 / (FREQUENCY), true); //set the value in the brackets to be the frequency in Hz
  timerAlarmEnable(my_timer);

  sr.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int x = 0; x <=23; x++){
//    for(int y = 0; y <= 127; y++){
//      sr.set(x,y);
//      delay(5);
//    }
//  }
//  for(int x = 0; x <= 23; x++){
//    for(int y = 127; y >= 0; y--){
//      sr.set(x,y);
//      delay(5);
//    }
//  }
    sr.set(0,127);
}
