#include <stdio.h>
#include "pico/stdlib.h"

#include "tb6612.h"
#include "config.h"
#include "Battery.h"

int main() {
    stdio_init_all();
    
    TB6612Config config = {
        .ain1    = Config::MotorDriver::HW::AIN1,
        .ain2    = Config::MotorDriver::HW::AIN2,
        .pwma    = Config::MotorDriver::HW::PWMA,

        // Channel B pins is reversed
        .bin1    = Config::MotorDriver::HW::BIN2,
        .bin2    = Config::MotorDriver::HW::BIN1,
        .pwmb    = Config::MotorDriver::HW::PWMB,

        .pwm_top = Config::MotorDriver::PWM_TOP,
        .clk_div = Config::MotorDriver::CLK_DIV,
        
        .stby    = Config::MotorDriver::HW::STBY
    };


    while (true) {
        sleep_ms(100);
    }
}
