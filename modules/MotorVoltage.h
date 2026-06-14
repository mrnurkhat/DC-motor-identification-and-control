#pragma once

#include "Battery.h"
#include "TB6612.h"
#include <algorithm>

struct MotorVoltageConfig {
    float max_batt_v;
    float min_batt_v;
    float nom_mot_v;
};

class MotorVoltage {
    TB6612 &driver_;
    Battery &battery_;
    MotorVoltageConfig config_;
    float channel_A_v_ = 0.0f;
    float channel_B_v_ = 0.0f;
public:
    MotorVoltage(TB6612 &driver, Battery &battery, const MotorVoltageConfig config) 
        : driver_(driver), battery_(battery), config_(config) {}

    void init() {
        driver_.init();
        battery_.init();
    }

    void setVoltage(TB6612Channel chan, float v) { 
        if (chan == TB6612Channel::A || chan == TB6612Channel::Both)
            channel_A_v_ = std::clamp(v, -config_.nom_mot_v, config_.nom_mot_v);

        if (chan == TB6612Channel::B || chan == TB6612Channel::Both)
            channel_B_v_ = std::clamp(v, -config_.nom_mot_v, config_.nom_mot_v);
    }

    void update() {
        float batt_v = battery_.getVoltage();

        if (batt_v > config_.max_batt_v || batt_v < config_.min_batt_v) {
            driver_.drive(TB6612Channel::Both, 0.0f);
            return;
        }
        
        driver_.drive(TB6612Channel::A, channel_A_v_ / batt_v);
        driver_.drive(TB6612Channel::B, channel_B_v_ / batt_v);
    }
};