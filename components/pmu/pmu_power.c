#include "pmu_power.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "PMU_PWR";

/* callbacks */
static pmu_display_on_cb_t  cb_display_on  = NULL;
static pmu_display_off_cb_t cb_display_off = NULL;

/* state */
static uint64_t last_activity_us = 0;
static uint32_t display_timeout_ms = 30000;
static bool display_on = true;

/* timer */
static esp_timer_handle_t pmu_timer;

/* ---------- timer ---------- */

static void pmu_timer_cb(void *arg)
{
    uint64_t now = esp_timer_get_time();
    uint64_t diff_ms = (now - last_activity_us) / 1000;

    if (display_on && diff_ms >= display_timeout_ms) {
        ESP_LOGI(TAG, "Display timeout (%ums)", display_timeout_ms);
        if (cb_display_off) {
            cb_display_off();
        }
        display_on = false;
    }
}

/* ---------- API ---------- */

void pmu_power_init(
    pmu_display_on_cb_t  display_on_cb,
    pmu_display_off_cb_t display_off_cb
)
{
    cb_display_on  = display_on_cb;
    cb_display_off = display_off_cb;

    last_activity_us = esp_timer_get_time();
    display_on = true;

    esp_timer_create_args_t args = {
        .callback = pmu_timer_cb,
        .name = "pmu_pwr"
    };
    esp_timer_create(&args, &pmu_timer);
    esp_timer_start_periodic(pmu_timer, 500 * 1000); // 500 ms

    ESP_LOGI(TAG, "PMU power manager ready");
}

void pmu_set_display_timeout_ms(uint32_t timeout_ms)
{
    display_timeout_ms = timeout_ms;
    ESP_LOGI(TAG, "Display timeout set to %ums", timeout_ms);
}

void pmu_notify_user_activity(void)
{
    last_activity_us = esp_timer_get_time();
        ESP_LOGI(TAG, "Last Activity %d ",last_activity_us);

    if (!display_on) {
        ESP_LOGI(TAG, "Wake display");
        if (cb_display_on) {
            cb_display_on();
        }
        display_on = true;
    }
}

bool pmu_is_display_on(void)
{
    return display_on;
}
