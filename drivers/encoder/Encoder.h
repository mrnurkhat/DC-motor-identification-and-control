#pragma once

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "quadrature_encoder.pio.h"
#include "LowPass.h"

struct EncoderConfig {
    float ticks_per_rev;
    float alpha;
    PIO pio;
    uint sm;
    uint pin_ab;
    bool inverted;
};

class Encoder {
    EncoderConfig config_;
    int prev_count_ = 0;
    LowPass filter_;

    float rpm_ = 0.0f;
    float pos_ = 0.0f;
public:
    Encoder(const EncoderConfig &config) : config_(config) {
        filter_.setAlpha(config_.alpha);
    }

    void init() {
        if (pio_can_add_program_at_offset(config_.pio, &quadrature_encoder_program, 0)) {
            pio_add_program_at_offset(config_.pio, &quadrature_encoder_program, 0);
        }

        quadrature_encoder_program_init(config_.pio, config_.sm, config_.pin_ab, 0);
        prev_count_ = quadrature_encoder_get_count(config_.pio, config_.sm);
    }

    void update(float dt) {
        int curr_count = quadrature_encoder_get_count(config_.pio, config_.sm);

        float rpm = (curr_count - prev_count_) / dt / config_.ticks_per_rev * 60.0f;
        prev_count_ = curr_count;
        rpm_ = filter_.update(rpm);
        if (config_.inverted) rpm_ = -rpm_;

        float pos = (prev_count_ / config_.ticks_per_rev) * 360.0f;
        pos_ = (config_.inverted) ? -pos : pos;
    }

    float getRPM() const { return rpm_; }
    float getPositionDeg() const { return pos_; }
};