#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_log.h>

#define MAX_RETRY (3)
static int retry_num = 0;
extern EventGroupHandle_t wifi_event_group;
extern const int WIFI_CONNECTED_BIT;

static const char *TAG = "event_handler_esp8266";

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    // system_event_info_t *info = &event->event_info;
    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "Got IP address: %s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
        retry_num = 0;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        if (retry_num < MAX_RETRY) {
            ESP_LOGI(TAG, "Retrying to connect to WiFi");
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
            retry_num++;
        }
        break;
    default:
        break;
    }
    return ESP_OK;
}
