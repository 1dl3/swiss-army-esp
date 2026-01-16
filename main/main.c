#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"

static void lvgl_task(void *arg)
{
    while (true) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void app_main(void)
{
    bsp_i2c_init();
    bsp_display_start();
    bsp_display_brightness_set(100);

    // lv_init();
    // bsp_lvgl_port_init();

    swiss_army_knife_init("/");

    xTaskCreate(
        lvgl_task,
        "lvgl",
        4096,
        NULL,
        5,
        NULL
    );

}
