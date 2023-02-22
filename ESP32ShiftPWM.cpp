#include "ESP32ShiftPWM.h"
#define toggleLatchPin() GPIO.out_w1ts = latch_pin; GPIO.out_w1tc = latch_pin
#define toggleClockPin() GPIO.out_w1ts = sck_pin; GPIO.out_w1tc = sck_pin
#define setDataPin() GPIO.out_w1ts = data_pin
#define clearDataPin() GPIO.out_w1tc = data_pin
ESP32ShiftPWM::ESP32ShiftPWM(uint32_t data_pin, uint32_t sck_pin, uint32_t latch_pin, uint8_t registerCount, uint8_t resolution){
  this->data_pin = data_pin;
  this->sck_pin = sck_pin;
  this->latch_pin = latch_pin;
  this->registerCount = registerCount;
  this->resolution = resolution;
  this->commands = (uint8_t*)malloc(resolution * registerCount * sizeof(uint8_t)); //allocate memory for the array that will hold the PWM pattern
  for(uint8_t i = 0; i < this->registerCount; i++){
    for(uint8_t j = 0; j < this->resolution; j++){
      commands[j + i * resolution] = 0; //fill the entire array with zeros
    }
  }
}

void ESP32ShiftPWM::begin(){
  gpio_config_t io_conf;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = latch_pin | sck_pin | data_pin;
  gpio_config(&io_conf);
}

void ESP32ShiftPWM::set(uint8_t pin, uint8_t value){
  uint8_t registerNo = pin/8;
  for(uint8_t t = 0; t < resolution; t++){
    commands[registerNo + t * registerCount] ^= (-(value > t) ^  commands[registerNo + t * registerCount]) & (1 << (pin%8));
  }
}
void ESP32ShiftPWM::update(){
  int offset = currentResolution * registerCount;
  for (int i = registerCount - 1; i >= 0; i--){
    ESPShiftOut(commands[i + offset]);
  }
  toggleLatchPin();
  if(++currentResolution == resolution) currentResolution = 0; //reset the current resolution before it overflows the resolution
}
inline void ESP32ShiftPWM::ESPShiftOut(uint8_t shiftRegData)
  {
    if (shiftRegData & 0B10000000) { setDataPin(); } else {clearDataPin(); }
    toggleClockPin();

    // bit 1
    if (shiftRegData & 0B01000000) { setDataPin(); } else {clearDataPin(); }
    toggleClockPin();

    // bit 2
    if (shiftRegData & 0B00100000) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();

    // bit 3
    if (shiftRegData & 0B00010000) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();

    // bit 4
    if (shiftRegData & 0B00001000) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();

    // bit 5
    if (shiftRegData & 0B00000100) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();

    // bit 6
    if (shiftRegData & 0B00000010) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();

    // bit 7
    if (shiftRegData & 0B00000001) { setDataPin(); } else { clearDataPin(); }
    toggleClockPin();
  };
