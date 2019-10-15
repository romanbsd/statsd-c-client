#if !defined(__WIFI_EVENT_HANDLER_ESP32__H__)
#define __WIFI_EVENT_HANDLER_ESP32__H__
#include <esp_event.h>

void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif
