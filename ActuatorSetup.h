#pragma once

#include "Config.h"
#include "TB6612.h"
#include "SpeedLoop.h"
#include "Battery.h"
#include "Encoder.h"

struct Actuator {
    SpeedLoop &left_wheel;
    MotorVoltage &power_sys;
};

inline Actuator& initActuator() {
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

    static TB6612 driver(driver_config);

    BatteryConfig batt_config {
        .adc_pin = Config::BatteryManager::HW::ADC2,
        .adc_channel = Config::BatteryManager::HW::ADC_CHAN,
        .convert_factor = Config::BatteryManager::WORD_TO_VOLTAGE_FACTOR,
        .alpha = Config::BatteryManager::LPF_ALPHA
    };

    static Battery batt(batt_config);

    MotorVoltageConfig motor_config = {
        .max_batt_v = Config::BatteryManager::MAX_VOLTAGE,
        .min_batt_v = Config::BatteryManager::MIN_VOLTAGE,
        .nom_mot_v = Config::Motor::NOMINAL_VOLTAGE,
        .calibration_factor = Config::Motor::CALIBRATION_FACTOR
    };

    static MotorVoltage motor(driver, batt, motor_config);

    EncoderConfig left_enc_config = {
        .ticks_per_rev = Config::Encoder::TICKS_PER_REVOLUITION,
        .alpha = Config::Encoder::LPF_ALPHA,
        .pio = pio0,
        .sm = Config::Encoder::LEFT_ENCODER_SM,
        .pin_ab = Config::Encoder::HW::LEFT_ENCODER_FIRST_PIN,
        .inverted = Config::Encoder::LEFT_ENCODER_IVERTED
    };

    static Encoder left_enc(left_enc_config);

    static SpeedLoop left_loop(left_enc, motor, TB6612Channel::A);

    static bool is_initialized = false;
    if (!is_initialized) {
        motor.init();
        left_enc.init();
        left_loop.setControlLimits(motor_config.nom_mot_v);

        is_initialized = true;
    }

    static Actuator act = { .left_wheel = left_loop, .power_sys = motor };
    return act;
}