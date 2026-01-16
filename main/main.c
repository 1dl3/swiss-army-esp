#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"



void app_main(void)
{
   bsp_i2c_init();
   bsp_display_start();
   bsp_display_brightness_set(100);
    swiss_army_knife_init("A:/");
}
