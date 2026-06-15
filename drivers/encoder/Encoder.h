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

    float getRPM(float dt) {
        int curr_count = quadrature_encoder_get_count(config_.pio, config_.sm);
        float rpm = (curr_count - prev_count_) / dt / config_.ticks_per_rev * 60.0f;
        prev_count_ = curr_count;

        rpm = filter_.update(rpm);
        if (config_.inverted) rpm = -rpm;
        
        return rpm;
    }
};