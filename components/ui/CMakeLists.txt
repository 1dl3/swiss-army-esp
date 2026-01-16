# 1. Vom Editor generierte Source-Liste
include(${CMAKE_CURRENT_LIST_DIR}/file_list_gen.cmake)

# 2. ESP-IDF Component registrieren
idf_component_register(
    SRCS ${PROJECT_SOURCES}
    INCLUDE_DIRS .
    REQUIRES lvgl
)

# 3. Compile-Definition NACH idf_component_register setzen
target_compile_definitions(${COMPONENT_LIB}
    PUBLIC LV_LVGL_H_INCLUDE_SIMPLE
)