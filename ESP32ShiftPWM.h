#ifndef _ESP32ShiftPWM
#define _ESP32ShiftPWM

#include <Arduino.h>
#include <cstdint>

class ESP32ShiftPWM{
public:
  ESP32ShiftPWM(uint32_t data_pin, uint32_t sck_pin, uint32_t latch_pin, uint8_t registerCount, uint8_t resolution);
  //uint16_t freq is the frequency of update in Hz
  void begin();
  void set(uint8_t pin, uint8_t value);
  void update();
  uint32_t data_pin = BIT14;
  uint32_t latch_pin = BIT4;
  uint32_t sck_pin = BIT12;
  uint8_t *commands;
  uint8_t registerCount = 1;
  uint8_t resolution = 255; //0-255
  hw_timer_t * my_timer = timerBegin(0, 80, true);
  inline void ESPShiftOut(uint8_t shiftRegData);
  volatile uint8_t currentResolution = 0; //this is the current resolution "step" we are on
};
#endif
