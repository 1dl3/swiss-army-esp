#pragma once
#include "lvgl.h"

typedef struct {
    const char *name;
    const char *icon;      // UTF-8 Emoji oder LVGL Symbol
    void (*create)(void);  // erstellt Screen
} app_t;
