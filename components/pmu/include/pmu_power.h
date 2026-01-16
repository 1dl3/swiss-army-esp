#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*pmu_display_on_cb_t)(void);
typedef void (*pmu_display_off_cb_t)(void);

/**
 * @brief Initialize PMU power manager
 *
 * @param display_on_cb  Called when display should turn ON
 * @param display_off_cb Called when display should turn OFF
 */
void pmu_power_init(
    pmu_display_on_cb_t  display_on_cb,
    pmu_display_off_cb_t display_off_cb
);

/**
 * @brief Set display inactivity timeout in milliseconds
 */
void pmu_set_display_timeout_ms(uint32_t timeout_ms);

/**
 * @brief Returns true if display is currently considered ON
 */
bool pmu_is_display_on(void);

#ifdef __cplusplus
}
#endif
