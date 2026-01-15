#include "launcher_screen.h"
#include "core/app_registry.h"

static void app_btn_event(lv_event_t *e) {
    app_t *app = lv_event_get_user_data(e);
    lv_obj_clean(lv_scr_act());
    app->create();
}

void launcher_screen_create(void) {
    lv_obj_clean(lv_scr_act());

    lv_obj_t *grid = lv_obj_create(lv_scr_act());
    lv_obj_set_size(grid, 240, 240);
    lv_obj_center(grid);

    static lv_coord_t cols[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t rows[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST};

    lv_obj_set_grid_dsc_array(grid, cols, rows);

    for (uint8_t i = 0; i < app_count; i++) {
        lv_obj_t *btn = lv_btn_create(grid);
        lv_obj_set_grid_cell(
            btn,
            LV_GRID_ALIGN_CENTER, i % 3, 1,
            LV_GRID_ALIGN_CENTER, i / 3, 1
        );

        lv_obj_add_event_cb(btn, app_btn_event, LV_EVENT_CLICKED, (void*)&app_list[i]);

        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text_fmt(label, "%s\n%s", app_list[i].icon, app_list[i].name);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_center(label);
    }
}
