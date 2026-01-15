#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui/launcher_screen.h"

void app_main(void) {
    lv_init();
    // Display + Touch init ist bei dir schon drin

    launcher_screen_create();

    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
