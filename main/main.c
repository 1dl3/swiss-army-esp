#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "swiss_army_knife.h"
#include "lvgl.h"
#include "pmu_axp2101.h"
#include "pmu_power.h"
#include "pmu_input.h"
#include "esp_log.h"


/* 
* TODO: 
* Reset von touch timer wird nicht aufgerufen d.h. der screen geht immer nach 10 sekunden aus 
*/
static const char *TAG = "MAIN";

static lv_obj_t *g_touch_overlay = NULL;

static void touch_event_cb(lv_event_t *e)
{
    ESP_LOGI(TAG, "touch_event_cb");
    (void)e;
    pmu_input_notify(PMU_INPUT_TAP);
}


static void create_touch_overlay(void)
{
    if (g_touch_overlay) return;

    g_touch_overlay = lv_obj_create(lv_scr_act());

    lv_obj_set_size(g_touch_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_pos(g_touch_overlay, 0, 0);

    lv_obj_set_style_bg_opa(g_touch_overlay, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(g_touch_overlay, LV_OPA_TRANSP, 0);
    lv_obj_set_style_outline_opa(g_touch_overlay, LV_OPA_TRANSP, 0);
    lv_obj_set_style_shadow_opa(g_touch_overlay, LV_OPA_TRANSP, 0);

    lv_obj_add_flag(g_touch_overlay, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(g_touch_overlay, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_add_event_cb(
        g_touch_overlay,
        touch_event_cb,
        LV_EVENT_PRESSED,
        NULL
    );

    lv_obj_add_flag(g_touch_overlay, LV_OBJ_FLAG_HIDDEN); // start hidden
}


static void show_touch_overlay(bool show)
{
    if (!g_touch_overlay) return;

    if (show) {
        lv_obj_clear_flag(g_touch_overlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(g_touch_overlay);
    } else {
        lv_obj_add_flag(g_touch_overlay, LV_OBJ_FLAG_HIDDEN);
    }
}


static void on_display_on(void)
{
    bsp_display_backlight_on();

    if (bsp_display_lock(50)) {
        show_touch_overlay(false);
        bsp_display_unlock();
    }
}

static void on_display_off(void)
{
    bsp_display_backlight_off();

    if (bsp_display_lock(50)) {
        show_touch_overlay(true);
        bsp_display_unlock();
    }
}

static void global_ui_event_cb(lv_event_t *e)
{
    (void)e;
    ESP_LOGI(TAG, "global_ui_event_cb");
    pmu_notify_user_activity();
}

void app_main(void)
{
    if (bsp_i2c_init() != ESP_OK) return;
    if (bsp_display_start() == NULL) return;
    bsp_display_brightness_set(100);

    swiss_army_knife_init("A:/");

    if (bsp_display_lock(2000)) {
        lv_obj_t *screen = layout_create();
        lv_screen_load(screen);

            lv_obj_add_event_cb(
        screen,
        global_ui_event_cb,
        LV_EVENT_PRESSED,
        NULL
    );
        bsp_display_unlock();
    }

    /* ---- PMU POWER ---- */
pmu_power_init(on_display_on, on_display_off);
    pmu_set_display_timeout_ms(10000); // 10 Sekunden

if (bsp_display_lock(2000)) {
    create_touch_overlay();
    show_touch_overlay(false); // Display ist an
    bsp_display_unlock();
}

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

