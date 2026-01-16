#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"
#include "lvgl.h"

void app_main(void)
{
    if (bsp_i2c_init() != ESP_OK) return;
    if (bsp_display_start() == NULL) return;
    bsp_display_brightness_set(100);

    swiss_army_knife_init("A:/"); // passe den Asset-Pfad an

    if (bsp_display_lock(2000)) {
        lv_obj_t *screen = layout_create();
        lv_screen_load(screen);
        bsp_display_unlock();
    }

    while (1) { vTaskDelay(pdMS_TO_TICKS(1000)); }
}