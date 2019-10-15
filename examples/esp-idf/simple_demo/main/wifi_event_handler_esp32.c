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

static const char *TAG = "event_handler_esp32";
void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    esp_err_t err = ESP_OK;
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_num < MAX_RETRY) {
                    ESP_LOGI(TAG, "Retrying to connect to WiFi");
                    esp_wifi_connect();
                    xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
                    retry_num++;
                }
            default:
                break;
        }

    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);

                ip_event_got_ip_t *event = (ip_event_got_ip_t*) event_data;
                ESP_LOGI(TAG, "Got IP address: %s", ip4addr_ntoa(&event->ip_info.ip));
                retry_num = 0;
            default:
                break;
        }
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "event_handler(): %s", esp_err_to_name(err));
    }
}


