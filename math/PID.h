#pragma once

#include <algorithm>
#include "LowPass.h"

class PID {
    float Kp_ = 0.0, Ki_ = 0.0, Kd_ = 0.0, Ka_ = 0.0;
    float integral_ = 0.0;
    float prev_meas_ = 0.0;
    float output_min_ = 0.0;
    float output_max_ = 0.0;
    float setpoint_;
    LowPass filter_;
public:
    PID& setCoeffs(float kp, float ki, float kd) {
        Kp_ = kp;
        Ki_ = ki;
        Kd_ = kd;
        return *this;
    }

    PID& setAntiWindup(float ka) {
        Ka_ = ka;
        return *this;
    }

    PID& setFilterAlpha(float alpha) {
        filter_.setAlpha(alpha);
        return *this;
    }

    PID& setOutputLimits(float min, float max) {
        output_min_ = min;
        output_max_ = max;
        return *this;
    }

    PID& setSetpoint(float setpoint) {
        setpoint_ = setpoint;
        return *this;
    }

    PID& reset() {
        integral_ = 0.0;
        prev_meas_ = 0.0;
        filter_.reset();
        return *this;
    }

    float update(float measurement, float dt) {
        float error = setpoint_ - measurement;
        float rate = -(measurement - prev_meas_) / dt;
        
        prev_meas_ = measurement;
        rate = filter_.update(rate);

        float output = Kp_ * error + Ki_ * integral_ + Kd_ * rate;
        float output_clamped = std::clamp(output, output_min_, output_max_);

        float aw = Ka_ * (output_clamped - output);
        integral_ += (error + aw) * dt;

        return output_clamped;
    }
};