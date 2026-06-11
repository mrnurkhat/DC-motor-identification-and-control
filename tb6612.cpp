#include "pico/stdlib.h"
#include "hardware/pwm.h"

enum class MotorMode {
    Forward,
    Backward,
    Brake,
    Coast
};

enum class Channel {
    A, 
    B, 
    Both
};

struct MotorDriverConfig {
    uint ain1; uint ain2; uint pwma; uint slice_a;
    uint bin1; uint bin2; uint pwmb; uint slice_b;
    uint pwm_top;
    float clk_div;
    uint stby;
};

class MotorDriver {
private:
    MotorDriverConfig config_;
public:
    MotorDriver(const MotorDriverConfig &config) : config_(config) {}

    void init() {
        // --- Channel A setup ---
        gpio_init(config_.ain1);
        gpio_set_dir(config_.ain1, GPIO_OUT);
        gpio_put(config_.ain1, false);

        gpio_init(config_.ain2);
        gpio_set_dir(config_.ain2, GPIO_OUT);
        gpio_put(config_.ain2, false);

        gpio_set_function(config_.pwma, GPIO_FUNC_PWM);
        config_.slice_a = pwm_gpio_to_slice_num(config_.pwma); 
        pwm_set_wrap(config_.slice_a, config_.pwm_top);
        pwm_set_clkdiv(config_.slice_a, config_.clk_div);
      
        pwm_set_chan_level(config_.slice_a, pwm_gpio_to_channel(config_.pwma), 0);
        pwm_set_enabled(config_.slice_a, true);

        // --- Channel B setup ---
        gpio_init(config_.bin1);
        gpio_set_dir(config_.bin1, GPIO_OUT);
        gpio_put(config_.bin1, false);

        gpio_init(config_.bin2); 
        gpio_set_dir(config_.bin2, GPIO_OUT);
        gpio_put(config_.bin2, false);

        gpio_set_function(config_.pwmb, GPIO_FUNC_PWM); 
        config_.slice_b = pwm_gpio_to_slice_num(config_.pwmb);
        pwm_set_wrap(config_.slice_b, config_.pwm_top);
        pwm_set_clkdiv(config_.slice_b, config_.clk_div);
        
        pwm_set_chan_level(config_.slice_b, pwm_gpio_to_channel(config_.pwmb), 0);
        pwm_set_enabled(config_.slice_b, true);

        // --- STBY setup ---
        gpio_init(config_.stby);
        gpio_set_dir(config_.stby, GPIO_OUT);
        gpio_put(config_.stby, true); 
    }
    
    void setMotor(Channel chan, MotorMode mode, uint duty = 0) {
        
    }
};