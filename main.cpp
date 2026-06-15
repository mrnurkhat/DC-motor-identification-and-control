#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

#include "TB6612.h"
#include "Config.h"
#include "Battery.h"
#include "MotorVoltage.h"
#include "Encoder.h"


int main() {
    stdio_init_all();

    TB6612Config driver_config = {
        .ain1 = Config::MotorDriver::HW::AIN1,
        .ain2 = Config::MotorDriver::HW::AIN2,
        .pwma = Config::MotorDriver::HW::PWMA,

        // Channel B pins is reversed
        .bin1 = Config::MotorDriver::HW::BIN2,
        .bin2 = Config::MotorDriver::HW::BIN1,
        .pwmb = Config::MotorDriver::HW::PWMB,

        .pwm_top = Config::MotorDriver::PWM_TOP,
        .clk_div = Config::MotorDriver::CLK_DIV,
        
        .stby = Config::MotorDriver::HW::STBY
    };

    TB6612 driver(driver_config);

    BatteryConfig batt_config {
        .adc_pin = Config::BatteryManager::HW::ADC2,
        .adc_channel = Config::BatteryManager::HW::ADC_CHAN,
        .convert_factor = Config::BatteryManager::WORD_TO_VOLTAGE_FACTOR,
        .alpha = Config::BatteryManager::LPF_ALPHA
    };

    Battery batt(batt_config);

    MotorVoltageConfig motor_config = {
        .max_batt_v = Config::BatteryManager::MAX_VOLTAGE,
        .min_batt_v = Config::BatteryManager::MIN_VOLTAGE,
        .nom_mot_v = Config::Motor::NOMINAL_VOLTAGE,
        .calibration_factor = Config::Motor::CALIBRATION_FACTOR
    };

    MotorVoltage motor(driver, batt, motor_config);

    motor.init();
    motor.setVoltage(TB6612Channel::A, 2.5f);

    EncoderConfig left_enc_config = {
        .ticks_per_rev = Config::Encoder::TICKS_PER_REVOLUITION,
        .alpha = Config::Encoder::LPF_ALPHA,
        .pio = pio0,
        .sm = Config::Encoder::LEFT_ENCODER_SM,
        .pin_ab = Config::Encoder::HW::LEFT_ENCODER_FIRST_PIN,
        .inverted = Config::Encoder::LEFT_ENCODER_IVERTED
    };

    Encoder left_enc(left_enc_config);
    left_enc.init();
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);

    while (true) {
        gpio_put(25, !gpio_get(25));
        motor.update();
        printf(">rpm: %f\n", left_enc.getRPM(0.05));
        sleep_ms(50);
    }
}
