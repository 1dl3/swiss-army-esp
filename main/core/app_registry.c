#include "app_registry.h"
#include "apps/app.h"

// Forward declarations
void app_dummy1_create(void);
void app_dummy2_create(void);
void app_dummy3_create(void);

const app_t app_list[] = {
    { "Dice",  "🎲", app_dummy1_create },
    { "Timer", "⏱️", app_dummy2_create },
    { "RNG",   "🔢", app_dummy3_create },
};

const uint8_t app_count = sizeof(app_list) / sizeof(app_list[0]);
