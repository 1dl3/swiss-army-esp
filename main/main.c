#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"
#include "lvgl.h"
#include "pmu_axp2101.h"
#include "pmu_power.h"
#include "esp_log.h"

/*
 * TODO:
 *
 */
static const char *TAG = "MAIN";

static void display_on_cb(void)
{
    ESP_LOGI(TAG, "Display ON");
    bsp_display_backlight_on();
}

static void display_off_cb(void)
{
    ESP_LOGI(TAG, "Display OFF");
    bsp_display_backlight_off();
}

void app_main(void)
{
    if (bsp_i2c_init() != ESP_OK)
        return;
    if (bsp_display_start() == NULL)
        return;
    bsp_display_brightness_set(100);

    swiss_army_knife_init("A:/");
    if (bsp_display_lock(2000))
    {
        lv_obj_t *screen = layout_create();
        lv_screen_load(screen);
        bsp_display_unlock();
    }

    /* ---- PMU POWER ---- */
    pmu_power_init(
        display_on_cb,
        display_off_cb);

    pmu_set_display_timeout_ms(10000); // 10 Sekunden

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
