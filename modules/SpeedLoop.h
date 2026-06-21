#pragma once

#include "TB6612.h"
#include "Battery.h"
#include "MotorVoltage.h"
#include "Encoder.h"
#include "PID.h"

class SpeedLoop {
    PID pid_;
    Encoder &encoder_;
    MotorVoltage &motor_;
    TB6612Channel channel_;

    float target_rpm_ = 0.0f;
    float current_rpm_ = 0.0f;
    float control_v_ = 0.0f;
    float dt_ = 0.0f;
public: 
    SpeedLoop(Encoder &encoder, MotorVoltage &motor, TB6612Channel channel) 
    : encoder_(encoder), motor_(motor), channel_(channel) {}

    void setTunings(float kp, float ki, float aw) {
        pid_.setCoeffs(kp, ki, 0.0f);
        pid_.setAntiWindup(aw);
    }

    void setControlLimits(float max_v) {
        pid_.setOutputLimits(-max_v, max_v);
    }

    void setTarget(float target_rpm) {
        target_rpm_ = target_rpm;
        pid_.setSetpoint(target_rpm);
    }

    void setSampleRate(float freq) {
        dt_ = 1.0f / freq;
    }

    void update() {
        encoder_.update(dt_);
        current_rpm_ = encoder_.getRPM();
        control_v_ = pid_.update(current_rpm_, dt_);

        motor_.setVoltage(channel_, control_v_);
    } 

    float getCurrentRPM() const { return current_rpm_; }
    float getTargetRPM() const { return target_rpm_; }
    float getControlVoltage() const { return control_v_; }
};