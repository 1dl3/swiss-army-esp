#include "app.h"
#include "core/nav.h"
#include <stdlib.h>
#include <time.h>

static int min_val = 1;
static int max_val = 100;
static int last_result = -1;
static bool fair_mode = false;

static lv_obj_t *result_label;

static void gen_event_cb(lv_event_t *e) {
    if (min_val >= max_val) {
        lv_label_set_text(result_label, "Err: Min >= Max");
        return;
    }

    int res;
    int attempts = 0;
    do {
        res = (rand() % (max_val - min_val + 1)) + min_val;
        attempts++;
    } while (fair_mode && res == last_result && (max_val - min_val) > 0 && attempts < 10);

    last_result = res;
    lv_label_set_text_fmt(result_label, "%d", res);
}

static void slider_event_cb(lv_event_t *e) {
    lv_obj_t *slider = lv_event_get_target(e);
    int *val = lv_event_get_user_data(e);
    *val = lv_slider_get_value(slider);
    
    lv_obj_t *label = lv_obj_get_child(lv_obj_get_parent(slider), 0); // Quick hack to find title
    // Actually better to have specific labels but for simplicity:
}

static void fair_mode_cb(lv_event_t *e) {
    fair_mode = lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
}

void app_rng_create(void) {
    srand(time(NULL));
    lv_obj_clean(lv_scr_act());

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "🔢 Zufall");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Min Slider
    lv_obj_t *min_slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(min_slider, 150, 10);
    lv_obj_align(min_slider, LV_ALIGN_CENTER, 0, -60);
    lv_slider_set_range(min_slider, 0, 500);
    lv_slider_set_value(min_slider, min_val, LV_ANIM_OFF);
    lv_obj_add_event_cb(min_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, &min_val);

    lv_obj_t *min_l = lv_label_create(lv_scr_act());
    lv_label_set_text(min_l, "Min");
    lv_obj_align_to(min_l, min_slider, LV_ALIGN_OUT_TOP_MID, 0, -5);

    // Max Slider
    lv_obj_t *max_slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(max_slider, 150, 10);
    lv_obj_align(max_slider, LV_ALIGN_CENTER, 0, -10);
    lv_slider_set_range(max_slider, 1, 1000);
    lv_slider_set_value(max_slider, max_val, LV_ANIM_OFF);
    lv_obj_add_event_cb(max_slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, &max_val);

    lv_obj_t *max_l = lv_label_create(lv_scr_act());
    lv_label_set_text(max_l, "Max");
    lv_obj_align_to(max_l, max_slider, LV_ALIGN_OUT_TOP_MID, 0, -5);

    // Fair mode switch
    lv_obj_t *sw = lv_switch_create(lv_scr_act());
    lv_obj_align(sw, LV_ALIGN_CENTER, -40, 30);
    if (fair_mode) lv_obj_add_state(sw, LV_STATE_CHECKED);
    lv_obj_add_event_cb(sw, fair_mode_cb, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_t *sw_l = lv_label_create(lv_scr_act());
    lv_label_set_text(sw_l, "Fair");
    lv_obj_align_to(sw_l, sw, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    result_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(result_label, &lv_font_montserrat_24, 0);
    lv_label_set_text(result_label, "---");
    lv_obj_align(result_label, LV_ALIGN_CENTER, 0, 70);

    lv_obj_t *gen_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(gen_btn, 100, 40);
    lv_obj_align(gen_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(gen_btn, gen_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *gen_l = lv_label_create(gen_btn);
    lv_label_set_text(gen_l, "START");
    lv_obj_center(gen_l);

    // Home button
    lv_obj_t *home_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(home_btn, 40, 40);
    lv_obj_align(home_btn, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_event_cb(home_btn, (lv_event_cb_t)nav_go_home, LV_EVENT_CLICKED, NULL);
    lv_obj_t *home_label = lv_label_create(home_btn);
    lv_label_set_text(home_label, LV_SYMBOL_HOME);
    lv_obj_center(home_label);
}
