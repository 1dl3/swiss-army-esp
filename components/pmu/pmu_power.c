#include "pmu_power.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "lvgl.h"

static const char *TAG = "PMU_PWR";

static pmu_display_on_cb_t cb_on = NULL;
static pmu_display_off_cb_t cb_off = NULL;

static uint32_t display_timeout_ms = 30000;
static bool display_on = true;

static esp_timer_handle_t pmu_timer;

static void pmu_timer_cb(void *arg)
{
    uint32_t inactive = lv_display_get_inactive_time(NULL);

    if (display_on && inactive >= display_timeout_ms)
    {
        ESP_LOGI(TAG, "Display OFF (inactive %ums)", inactive);
        display_on = false;
        if (cb_off)
            cb_off();
    }

        /* Display ON (wake by activity) */
    if (!display_on && inactive < 200) {   // <- KEY LINE
        ESP_LOGI(TAG, "Display ON (activity)");
        display_on = true;
        if (cb_on) cb_on();
    }
}

void pmu_power_init(
    pmu_display_on_cb_t on_cb,
    pmu_display_off_cb_t off_cb)
{
    cb_on = on_cb;
    cb_off = off_cb;

    esp_timer_create_args_t args = {
        .callback = pmu_timer_cb,
        .name = "pmu_pwr"};

    ESP_ERROR_CHECK(esp_timer_create(&args, &pmu_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(pmu_timer, 500 * 1000));

    ESP_LOGI(TAG, "PMU power manager ready");
}

void pmu_set_display_timeout_ms(uint32_t timeout_ms)
{
    display_timeout_ms = timeout_ms;
}

void pmu_wake_display(void)
{
    if (!display_on)
    {
        ESP_LOGI(TAG, "Display ON (wake)");
        display_on = true;
        if (cb_on)
            cb_on();
    }
}

bool pmu_is_display_on(void)
{
    return display_on;
}
