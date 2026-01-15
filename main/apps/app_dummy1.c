#include "app.h"
#include "core/nav.h"

void app_dummy1_create(void) {
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "🎲 Dice App");
    lv_obj_center(label);

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(btn, (lv_event_cb_t)nav_go_home, LV_EVENT_CLICKED, NULL);

    lv_label_create(btn);
}
