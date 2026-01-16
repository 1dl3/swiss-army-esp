idf_component_register(
    SRCS
        pmu_axp2101.c
        pmu_power.c
    INCLUDE_DIRS "include"
    REQUIRES 
        lvgl
        esp_timer
        esp_driver_i2c
        esp32_c6_touch_amoled_1_8
)