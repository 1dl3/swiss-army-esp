#include "app.h"
#include "core/nav.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

static int last_rolls[10];
static int last_rolls_count = 0;
static int dice_count = 1;
static int dice_sides = 6;
static lv_obj_t *result_label;
static lv_obj_t *history_label;

static void update_history_display() {
    char buf[128] = "History: ";
    for (int i = 0; i < last_rolls_count; i++) {
        char val[16];
        snprintf(val, sizeof(val), "%d ", last_rolls[i]);
        strcat(buf, val);
    }
    lv_label_set_text(history_label, buf);
}

static void roll_dice_event_cb(lv_event_t *e) {
    int total = 0;
    for (int i = 0; i < dice_count; i++) {
        total += (rand() % dice_sides) + 1;
    }

    lv_label_set_text_fmt(result_label, "Ergebnis: %d", total);

    // History shift
    for (int i = 9; i > 0; i--) {
        last_rolls[i] = last_rolls[i - 1];
    }
    last_rolls[0] = total;
    if (last_rolls_count < 10) last_rolls_count++;

    update_history_display();
}

static void settings_event_cb(lv_event_t *e) {
    lv_obj_t *roller = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);
    
    char buf[32];
    lv_roller_get_selected_str(roller, buf, sizeof(buf));
    
    if (user_data == (void*)1) { // Count
        dice_count = atoi(buf);
    } else { // Sides
        dice_sides = atoi(buf + 1); // skip 'd'
    }
}

void app_dice_create(void) {
    srand(time(NULL));
    lv_obj_clean(lv_scr_act());

    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "🎲 Würfel");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Rollers for settings
    lv_obj_t *cnt_roller = lv_roller_create(lv_scr_act());
    lv_roller_set_options(cnt_roller, "1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_size(cnt_roller, 60, 80);
    lv_obj_align(cnt_roller, LV_ALIGN_LEFT_MID, 20, -20);
    lv_obj_add_event_cb(cnt_roller, settings_event_cb, LV_EVENT_VALUE_CHANGED, (void*)1);

    lv_obj_t *side_roller = lv_roller_create(lv_scr_act());
    lv_roller_set_options(side_roller, "d4\nd6\nd8\nd10\nd12\nd20", LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(side_roller, 1, LV_ANIM_OFF); // d6 default
    lv_obj_set_size(side_roller, 60, 80);
    lv_obj_align(side_roller, LV_ALIGN_RIGHT_MID, -20, -20);
    lv_obj_add_event_cb(side_roller, settings_event_cb, LV_EVENT_VALUE_CHANGED, (void*)2);

    result_label = lv_label_create(lv_scr_act());
    lv_label_set_text(result_label, "Bereit?");
    lv_obj_align(result_label, LV_ALIGN_CENTER, 0, 40);

    history_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(history_label, &lv_font_montserrat_10, 0);
    lv_obj_align(history_label, LV_ALIGN_BOTTOM_MID, 0, -50);
    update_history_display();

    lv_obj_t *roll_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(roll_btn, 100, 40);
    lv_obj_align(roll_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_event_cb(roll_btn, roll_dice_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *roll_label = lv_label_create(roll_btn);
    lv_label_set_text(roll_label, "WÜRFELN");
    lv_obj_center(roll_label);

    // Home button
    lv_obj_t *home_btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(home_btn, 40, 40);
    lv_obj_align(home_btn, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_event_cb(home_btn, (lv_event_cb_t)nav_go_home, LV_EVENT_CLICKED, NULL);
    lv_obj_t *home_label = lv_label_create(home_btn);
    lv_label_set_text(home_label, LV_SYMBOL_HOME);
    lv_obj_center(home_label);
}
