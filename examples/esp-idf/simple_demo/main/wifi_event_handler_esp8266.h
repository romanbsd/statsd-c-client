#if !defined(__WIFI_EVENT_HANDLER_ESP8266__H__)
#define __WIFI_EVENT_HANDLER_ESP8266__H__
#include <esp_err.h>
#include <esp_event.h>

esp_err_t event_handler(void *ctx, system_event_t *event);
#endif
