#pragma once
#include "pico/stdlib.h"
#include <algorithm>

enum class TB6612Mode {
    Forward,
    Backward,
    Brake,
    Coast
};

enum class TB6612Channel {
    A, 
    B, 
    Both
};

struct TB6612Config {
    uint ain1; uint ain2; uint pwma;
    uint bin1; uint bin2; uint pwmb;
    uint pwm_top;
    float clk_div;
    uint stby;
};

class TB6612 {
private:
    TB6612Config config_;
    uint slice_a_, slice_b_;
public:
    TB6612(const TB6612Config &config) : config_(config) {}

    void init();
    void setChannelDuty(TB6612Channel chan, TB6612Mode mode, uint16_t duty = 0);
    void drive(TB6612Channel chan, float speed);
    void brake(TB6612Channel chan);
};