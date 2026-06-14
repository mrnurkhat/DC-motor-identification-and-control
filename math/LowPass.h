#pragma once

class LowPass {
    float prev_meas_ = 0.0f;
    float alpha_ = 0.0f;
public:
    float update(float meas) {
        prev_meas_ = meas * alpha_ + (1.0f - alpha_) * prev_meas_;
        return prev_meas_;
    }

    LowPass& setAlpha(float alpha) {
        alpha_ = alpha;
        return *this;
    }

    LowPass& reset() {
        prev_meas_ = 0.0f;
        return *this;
    }
};