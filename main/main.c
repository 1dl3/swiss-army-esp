#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"

void app_main(void) {
    bsp_i2c_init();
    bsp_display_start();
    bsp_display_brightness_set(100);
    
    lv_init();
    // Display + Touch init ist bei dir schon drin
    // In einer echten BSP Umgebung müssten hier bsp_lvgl_port_init() etc. aufgerufen werden
    // Ich setze voraus, dass das LVGL-Porting bereits im Hintergrund läuft oder initialisiert ist.

    swiss_army_knife_init("/");   

    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
