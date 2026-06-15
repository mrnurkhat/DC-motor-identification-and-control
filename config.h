#pragma once
#include "pico/stdlib.h"

namespace Config {
    namespace MotorDriver {
        namespace HW {
            constexpr uint STBY = 21;

            // Left Motor (A)
            constexpr uint PWMA = 27;
            constexpr uint AIN2 = 26;
            constexpr uint AIN1 = 22;

            // Right Motor (B)
            constexpr uint PWMB = 18;
            constexpr uint BIN2 = 19;
            constexpr uint BIN1 = 20;
        }

        constexpr uint PWM_TOP = 1000;
        constexpr float CLK_DIV = 5.0;
        constexpr float NOMINAL_VOLTAGE = 6.0f;
    }

    namespace BatteryManager {
        namespace HW {
            constexpr uint ADC2 = 28;
            constexpr uint ADC_CHAN = 2;
        }

        constexpr float R101 = 100.0f;
        constexpr float R102 = 47.0f;
        constexpr float REFERENCE_VOLTAGE = 3.3f;
        constexpr float ADC_MAX_WORD = 4095.0f;
        constexpr float LPF_ALPHA = 0.1f;
        constexpr float CALIBRATION_FACTOR = 0.98778f;
        constexpr float WORD_TO_VOLTAGE_FACTOR = (REFERENCE_VOLTAGE / ADC_MAX_WORD * (R101 + R102) / R102 * CALIBRATION_FACTOR);
        constexpr float MIN_VOLTAGE = 6.0f;
        constexpr float MAX_VOLTAGE = 8.4f;
    }

    namespace Motor {
        constexpr float NOMINAL_VOLTAGE = 6.0f;
        constexpr float CALIBRATION_FACTOR = 1.06126f;
    }

    namespace Encoder {
        namespace HW {
            constexpr uint LEFT_ENCODER_FIRST_PIN = 0;
            constexpr uint RIGHT_ENCODER_FIRST_PIN = 14;
        }

        constexpr uint LEFT_ENCODER_SM = 0;
        constexpr bool LEFT_ENCODER_IVERTED = false;
        constexpr float TICKS_PER_REVOLUITION = 937.2f;
        constexpr float LPF_ALPHA = 0.4f;
    }
}
