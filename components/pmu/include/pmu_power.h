#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pmu_display_on_cb_t)(void);
typedef void (*pmu_display_off_cb_t)(void);

/* Init power manager */
void pmu_power_init(
    pmu_display_on_cb_t  display_on_cb,
    pmu_display_off_cb_t display_off_cb
);

/* Set display timeout */
void pmu_set_display_timeout_ms(uint32_t timeout_ms);

/* Notify ANY user activity (tap, shake, etc.) */
void pmu_notify_user_activity(void);

/* Query display state */
bool pmu_is_display_on(void);

#ifdef __cplusplus
}
#endif
