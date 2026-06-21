#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"

#include "ActuatorSetup.h"

volatile bool control_flag = false;
bool control_callback(struct repeating_timer *t) {
    control_flag = true;
    return true;
}

struct LogPacket {
    uint32_t time_ms;
    float current_rpm;
    float target_rpm;
    float control_voltage;
};

queue_t log_queue;

void core1_main() {
    LogPacket packet;

    while(1) {
        queue_remove_blocking(&log_queue, &packet);
        printf(">Target_RPM:%lu:%.2f\n", packet.time_ms, packet.target_rpm);
        printf(">Current_RPM:%lu:%.2f\n", packet.time_ms, packet.current_rpm);
        printf(">Voltage:%lu:%.2f\n", packet.time_ms, packet.control_voltage);
    }
}

int main() {
    stdio_init_all();

    Actuator act = initActuator();

    act.left_wheel.setSampleRate(100.0f);
    act.left_wheel.setTarget(100.0f);
    act.left_wheel.setTunings(0.06f, 0.04f, 30.0f);

    struct repeating_timer control_timer;
    add_repeating_timer_ms(10, control_callback, NULL, &control_timer);

    queue_init(&log_queue, sizeof(LogPacket), 200);
    multicore_launch_core1(core1_main);

    while (true) {
        if (control_flag) {
            control_flag = false;

            act.power_sys.update();
            act.left_wheel.update();

            float control_limit = act.power_sys.getBattCharge();
            control_limit = std::clamp(control_limit, 0.0f, 6.0f);
            act.left_wheel.setControlLimits(control_limit);

            LogPacket packet = {
                .time_ms = to_ms_since_boot(get_absolute_time()),
                .current_rpm = act.left_wheel.getCurrentRPM(),
                .target_rpm = act.left_wheel.getTargetRPM(),
                .control_voltage = act.left_wheel.getControlVoltage()
            };

            queue_try_add(&log_queue, &packet);
        }
    }
}
