#include "pmu_axp2101.h"

#include "bsp/esp32_c6_touch_amoled_1_8.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

/* ================= CONFIG ================= */

#define TAG "AXP2101"

/* AXP2101 Registers */
#define AXP2101_ADDR        0x34
#define AXP2101_VBAT_H      0x78
#define AXP2101_VBAT_L      0x79
#define AXP2101_POWER_STAT  0x01


/* I2C */
#define PMU_I2C_FREQ_HZ     400000

/* ========================================= */

/* Handles */
static i2c_master_bus_handle_t g_bus = NULL;
static i2c_master_dev_handle_t g_axp = NULL;

/* ---------- I2C read helper ---------- */
static esp_err_t axp_read(uint8_t reg, uint8_t *data, size_t len)
{
    return i2c_master_transmit_receive(
        g_axp,
        &reg, 1,
        data, len,
        100 /* ms */
    );
}

/* ---------- Voltage → Percent ---------- */
static int voltage_to_percent(float v)
{
    if (v >= 4.20f) return 100;
    if (v >= 4.10f) return 90;
    if (v >= 4.00f) return 75;
    if (v >= 3.90f) return 60;
    if (v >= 3.80f) return 40;
    if (v >= 3.70f) return 20;
    if (v >= 3.60f) return 10;
    return 1;
}

/* ================= PUBLIC API ================= */

bool axp2101_init(void)
{
    if (g_axp != NULL) {
        return true; /* already initialized */
    }

    /* 🔑 WICHTIG: BSP-I2C-Bus holen */
    g_bus = bsp_i2c_get_handle();
    if (g_bus == NULL) {
        ESP_LOGE(TAG, "BSP I2C bus not ready (call after bsp_display_start)");
        return false;
    }

    ESP_LOGI(TAG, "Register AXP2101 on BSP I2C bus");

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address  = AXP2101_ADDR,
        .scl_speed_hz    = PMU_I2C_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(g_bus, &dev_cfg, &g_axp));

    ESP_LOGI(TAG, "AXP2101 ready");
    return true;
}

float axp2101_get_battery_voltage(void)
{
    uint8_t buf[2];

    if (g_axp == NULL) {
        return 0.0f;
    }

    if (axp_read(AXP2101_VBAT_H, buf, 2) != ESP_OK) {
        return 0.0f;
    }

    /* 12-bit, 1.1 mV per LSB */
    uint16_t raw = ((uint16_t)buf[0] << 4) | (buf[1] & 0x0F);
    return (raw * 1.1f) / 1000.0f;
}

bool axp2101_is_charging(void)
{
    uint8_t stat = 0;

    if (g_axp == NULL) {
        return false;
    }

    if (axp_read(AXP2101_POWER_STAT, &stat, 1) != ESP_OK) {
        return false;
    }

    /* Bit 6 = charging */
    return (stat & 0x40) != 0;
}

int axp2101_get_battery_percent(void)
{
    float v = axp2101_get_battery_voltage();
    int p = voltage_to_percent(v);

    if (p < 1)   p = 1;
    if (p > 100) p = 100;
    return p;
}
