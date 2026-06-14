#pragma once 

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "lowpass.h"

struct BatteryConfig {
    uint adc_pin, adc_channel;
    float convert_factor;
    float alpha;
};

class Battery {
    BatteryConfig config_;
    float meas_ = 0.0f;
    LowPass filter_;
public:
    Battery(BatteryConfig &config) : config_(config) {
        filter_.setAlpha(config_.alpha);
    }

    void init() {
        adc_init();
        adc_gpio_init(config_.adc_pin);

        adc_select_input(config_.adc_channel);
        meas_ = adc_read() * config_.convert_factor;

        filter_.update(meas_);
    }

    void update() {
        adc_select_input(config_.adc_channel);

        uint16_t raw_word = adc_read();
        float raw_voltage = raw_word * config_.convert_factor;

        meas_ = filter_.update(raw_voltage);
    }

    float getVoltage() const {
        return meas_;
    }
};