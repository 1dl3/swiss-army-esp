#include "pmu_input.h"
#include "pmu_power.h"
#include "esp_timer.h"
#include "esp_log.h"
/* tap */
static uint64_t last_tap_us = 0;
static uint32_t double_tap_window_us = 300000;

/* shake */
static uint8_t  shake_required = 3;
static uint32_t shake_window_us = 700000;
static uint8_t  shake_count = 0;
static uint64_t shake_start_us = 0;

/* ---------- config ---------- */

void pmu_set_double_tap_window_ms(uint32_t ms)
{
    double_tap_window_us = ms * 1000ULL;
}

void pmu_set_shake_config(uint8_t count, uint32_t window_ms)
{
    shake_required = count;
    shake_window_us = window_ms * 1000ULL;
}

/* ---------- input ---------- */

void pmu_input_notify(pmu_input_event_t event)
{
    ESP_LOGI("TAG", "pmu_input_notify display");

    uint64_t now = esp_timer_get_time();

    if (event == PMU_INPUT_TAP) {
        if (now - last_tap_us <= double_tap_window_us) {
            pmu_notify_user_activity(); // double tap
        }
        last_tap_us = now;
        pmu_notify_user_activity();     // single tap = activity
    }

    if (event == PMU_INPUT_SHAKE) {
        if (shake_count == 0) {
            shake_start_us = now;
        }
        shake_count++;

        if ((now - shake_start_us) > shake_window_us) {
            shake_count = 0;
            return;
        }

        if (shake_count >= shake_required) {
            shake_count = 0;
            pmu_notify_user_activity();
        }
    }
}
