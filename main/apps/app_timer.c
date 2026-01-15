#include "app.h"
#include "core/nav.h"
#include "esp_timer.h"

static int total_seconds = 0;
static int remaining_seconds = 0;
static bool running = false;
static lv_obj_t *time_label;
static lv_obj_t *slider;
static lv_timer_t *timer = NULL;

static void update_label() {
    int m = remaining_seconds / 60;
    int s = remaining_seconds % 60;
    lv_label_set_text_fmt(time_label, "%02d:%02d", m, s);
}

static void timer_cb(lv_timer_t *t) {
    if (running && remaining_seconds > 0) {
        remaining_seconds--;
        update_label();
        if (remaining_seconds == 0) {
            running = false;
            lv_label_set_text(time_label, "FERTIG!");
            // Hier könnte man Sound abspielen
        }
    }
}

static void start_stop_cb(lv_event_t *e) {
    running = !running;
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    lv_label_set_text(label, running ? "STOP" : "START");
}

static void slider_cb(lv_event_t *e) {
    if (!running) {
        remaining_seconds = lv_slider_get_value(slider) * 10; // 10s steps
        update_label();
    }
}

static void preset_cb(lv_event_t *e) {
    int mins = (int)lv_event_get_user_data(e);
    remaining_seconds = mins * 60;
    lv_slider_set_value(slider, remaining_seconds / 10, LV_ANIM_ON);
    update_label();
}

void app_timer_create(void) {
    lv_obj_clean(lv_scr_act());

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "⏱️ Timer");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    time_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(time_label, &lv_font_montserrat_32, 0);
    lv_label_set_text(time_label, "00:00");
    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, -40);

    slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, 180, 15);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(slider, 0, 180); // Up to 30 mins
    lv_obj_add_event_cb(slider, slider_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Presets
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 220, 50);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    int presets[] = {1, 3, 5, 10};
    for(int i=0; i<4; i++) {
        lv_obj_t *p_btn = lv_btn_create(cont);
        lv_obj_set_size(p_btn, 45, 35);
        lv_obj_add_event_cb(p_btn, preset_cb, LV_EVENT_CLICKED, (void*)presets[i]);
        lv_obj_t *l = lv_label_create(p_btn);
        lv_label_set_text_fmt(l, "%d'", presets[i]);
        lv_obj_center(l);
    }

    lv_obj_t *main_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(main_btn, 100, 40);
    lv_obj_align(main_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(main_btn, start_stop_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *main_l = lv_label_create(main_btn);
    lv_label_set_text(main_l, "START");
    lv_obj_center(main_l);

    if (timer == NULL) {
        timer = lv_timer_create(timer_cb, 1000, NULL);
    }

    // Home button
    lv_obj_t *home_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(home_btn, 40, 40);
    lv_obj_align(home_btn, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_event_cb(home_btn, (lv_event_cb_t)nav_go_home, LV_EVENT_CLICKED, NULL);
    lv_obj_t *home_label = lv_label_create(home_btn);
    lv_label_set_text(home_label, LV_SYMBOL_HOME);
    lv_obj_center(home_label);
}
