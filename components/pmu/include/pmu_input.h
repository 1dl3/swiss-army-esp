#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PMU_INPUT_TAP,
    PMU_INPUT_SHAKE,
} pmu_input_event_t;

/* Configuration */
void pmu_set_double_tap_window_ms(uint32_t ms);
void pmu_set_shake_config(uint8_t count, uint32_t window_ms);

/* Notify input event */
void pmu_input_notify(pmu_input_event_t event);

#ifdef __cplusplus
}
#endif
