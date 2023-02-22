# ESP32ShiftPWM
A library for using shift registers (typically 74HC595s) with individually-controlled PWM (via bit-bang) for the ESP32. 

Refer to [this blog post](https://urbanmakerkraft.com/2023/02/22/esp32-shiftpwm/) for more in-depth explanation on how to use this library.
Special thanks to [Timo Denk](https://github.com/Simsso/ShiftRegister-PWM-Library) for his code and [excellent writeup](https://timodenk.com/blog/shiftregister-pwm-library/), which I heavily drew inspiration from. 

### Basic Usage
Create an ESP32ShiftPWM object using the constructor: ESP32ShiftPWM(uint32_t data_pin, uint32_t sck_pin, uint32_t latch_pin, uint8_t registerCount, uint8_t resolution);
Where:
- data_pin => Any pin between GPIO0-GPIO31. Connects to the SER pin of the 74HC595
- sck_pin => Any pin between GPIO0-GPIO31. Connects to the SRCLK pin of the 74HC595
- latch_pin => Any pin between GPIO0-GPIO31. Connects to the RCLK pin of the 74HC595

Refer to[Last Minute Engineer's awesome guide](https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/) for detailed pinout instructions. 

- registerCount => Number of daisy chained registers. I have not tested the maximum but you are limited by the overall size of the command array (size of command array = number of registers * 8 * PWM resolution). Exorbitant sizes of the array will lead to sluggish performance or even crashing of the ESP32. Play around with the resolution and the FREQUENCY to find what works for you. 
- resolution => Number of "levels" of PWM (this determines how "fine" your PWM can be and your smallest division). As mentioned, play around with this and the FREQUENCY to find the combination that works best for your application. 

Create a timer object and attach an alarm (interrupt) with an ISR that calls the ESP32ShiftPWM::update() function. 
define FREQUENCY (Hz)
Call ESP32ShiftPWM::begin()

Alternatively just copy this code: 
```C++
#define FREQUENCY 100000 //Frequency of update in Hz
hw_timer_t * my_timer = NULL;

void IRAM_ATTR onTimer(){
  sr.update();
}

void setup() {
  // put your setup code here, to run once:  
  my_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(my_timer, &onTimer, true);
  //since we want the interrupt to be called at (freq)Hz, we need to set the alarm to fire every 1/(this->freq) s = 1/(this->freq)* 10^6 ticks
  timerAlarmWrite(my_timer, 1000000 / (FREQUENCY), true); //set the value in the brackets to be the frequency in Hz
  timerAlarmEnable(my_timer);

  sr.begin();
}
```

To set a PWM value for a particular shift register, call the ESP32ShiftPWM::set(pin, value)
- pin => between 0 - total number of Shift Registers * 8 -1 
- value => 0 - resolution specified

## **WARNING**:
This library is not suited for generating high freq PWM. As such I highly discourage you from trying to drive inductive loads using this. Blinking LEDs is fine.