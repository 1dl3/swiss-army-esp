#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialisiert I2C + AXP2101 (einmal beim Start aufrufen) */
bool axp2101_init(void);

/* Batteriespannung in Volt (z. B. 3.92) */
float axp2101_get_battery_voltage(void);

/* Ladezustand */
bool axp2101_is_charging(void);

/* Batterie in Prozent (1–100, UI-tauglich) */
int axp2101_get_battery_percent(void);

#ifdef __cplusplus
}
#endif
