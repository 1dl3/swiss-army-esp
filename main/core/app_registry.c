#include "app_registry.h"
#include "apps/app.h"

// Forward declarations
void app_dice_create(void);
void app_timer_create(void);
void app_rng_create(void);

const app_t app_list[] = {
    { "Dice",  "🎲", app_dice_create },
    { "Timer", "⏱️", app_timer_create },
    { "RNG",   "🔢", app_rng_create },
};

const uint8_t app_count = sizeof(app_list) / sizeof(app_list[0]);
