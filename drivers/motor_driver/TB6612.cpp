#include "TB6612.h"
#include "hardware/pwm.h"

void TB6612::init() {
    // --- Channel A setup ---
    gpio_init(config_.ain1);
    gpio_set_dir(config_.ain1, GPIO_OUT);
    gpio_put(config_.ain1, false);

    gpio_init(config_.ain2);
    gpio_set_dir(config_.ain2, GPIO_OUT);
    gpio_put(config_.ain2, false);

    gpio_set_function(config_.pwma, GPIO_FUNC_PWM);
    slice_a_ = pwm_gpio_to_slice_num(config_.pwma); 
    pwm_set_wrap(slice_a_, config_.pwm_top);
    pwm_set_clkdiv(slice_a_, config_.clk_div);
    
    pwm_set_chan_level(slice_a_, pwm_gpio_to_channel(config_.pwma), 0);
    pwm_set_enabled(slice_a_, true);

    // --- Channel B setup ---
    gpio_init(config_.bin1);
    gpio_set_dir(config_.bin1, GPIO_OUT);
    gpio_put(config_.bin1, false);

    gpio_init(config_.bin2); 
    gpio_set_dir(config_.bin2, GPIO_OUT);
    gpio_put(config_.bin2, false);

    gpio_set_function(config_.pwmb, GPIO_FUNC_PWM); 
    slice_b_ = pwm_gpio_to_slice_num(config_.pwmb);
    pwm_set_wrap(slice_b_, config_.pwm_top);
    pwm_set_clkdiv(slice_b_, config_.clk_div);
    
    pwm_set_chan_level(slice_b_, pwm_gpio_to_channel(config_.pwmb), 0);
    pwm_set_enabled(slice_b_, true);

    // --- STBY setup ---
    gpio_init(config_.stby);
    gpio_set_dir(config_.stby, GPIO_OUT);
    gpio_put(config_.stby, true); 
}

void TB6612::setChannelDuty(TB6612Channel chan, TB6612Mode mode, uint16_t duty) {
    bool in1 = false;
    bool in2 = false;

    duty = (duty > config_.pwm_top) ? config_.pwm_top : duty;

    switch (mode) {
        case TB6612Mode::Forward: in1 = true; in2 = false; break;
        case TB6612Mode::Backward: in1 = false; in2 = true; break;
        case TB6612Mode::Brake: in1 = true; in2 = true; break;
        default:
        case TB6612Mode::Coast: in1 = false; in2 = false;
    }

    if (chan == TB6612Channel::A || chan == TB6612Channel::Both) {
        gpio_put(config_.ain1, in1);
        gpio_put(config_.ain2, in2);
        pwm_set_chan_level(slice_a_, pwm_gpio_to_channel(config_.pwma), duty);
    }

    if (chan == TB6612Channel::B || chan == TB6612Channel::Both) {
        gpio_put(config_.bin1, in1);
        gpio_put(config_.bin2, in2);
        pwm_set_chan_level(slice_b_, pwm_gpio_to_channel(config_.pwmb), duty);
    }
}

void TB6612::drive(TB6612Channel chan, float speed) {
    speed = std::clamp(speed, -1.0f, 1.0f);
    float abs_speed = std::abs(speed) * config_.pwm_top;
    uint16_t duty = static_cast<uint16_t>(abs_speed);

    if (speed > 0) {
        setChannelDuty(chan, TB6612Mode::Forward, duty);
    } else if (speed < 0) {
        setChannelDuty(chan, TB6612Mode::Backward, duty);
    } else {
        setChannelDuty(chan, TB6612Mode::Coast, 0);
    }
}

void TB6612::brake(TB6612Channel chan) {
    setChannelDuty(chan, TB6612Mode::Brake, 0);
}