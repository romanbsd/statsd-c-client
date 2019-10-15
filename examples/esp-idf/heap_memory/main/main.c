#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <statsd-client.h>

#include "esp_idf_lib_helpers.h"

#if HELPER_TARGET_IS_ESP32 && HELPER_TARGET_VERSION >= HELPER_TARGET_VERSION_ESP32_V4
#include "wifi_event_handler_esp32.h"
#else
#include <esp_event_loop.h>
#include "wifi_event_handler_esp8266.h"
#endif

static const char *TAG = "simple_demo";
EventGroupHandle_t wifi_event_group;
int WIFI_CONNECTED_BIT = BIT0;

esp_err_t init_wifi()
{
    esp_err_t err = ESP_FAIL;
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t config = {
        .sta = {
            .ssid = CONFIG_PROJECT_WIFI_SSID,
            .password = CONFIG_PROJECT_WIFI_PASSWORD
        }
    };
    wifi_event_group = xEventGroupCreate();

    if (wifi_event_group == NULL) {
        ESP_LOGE(TAG, "xEventGroupCreate() failed");
        err = ESP_FAIL;
        goto fail;
    }

    tcpip_adapter_init();
#if HELPER_TARGET_IS_ESP32 && HELPER_TARGET_VERSION >= HELPER_TARGET_VERSION_ESP32_V4
    if ((err = esp_event_loop_create_default()) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_loop_create_default() failed: %s", esp_err_to_name(err));
        goto fail;
    }

    if ((err = esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_handler_register(): %s", esp_err_to_name(err));
        goto fail;
    }

    if ((err = esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_handler_register(): %s", esp_err_to_name(err));
        goto fail;
    }
#else
    if ((err = esp_event_loop_init(event_handler, NULL)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_event_loop_init(): %s", esp_err_to_name(err));
        goto fail;
    }
#endif

    if ((err = esp_wifi_init(&init_config)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_init: %s", esp_err_to_name(err));
        goto fail;
    }
    if ((err = esp_wifi_set_mode(WIFI_MODE_STA)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_mode(): %s", esp_err_to_name(err));
        goto fail;
    }

    ESP_LOGI(TAG, "Connecting to SSID: %s", config.sta.ssid);
    if ((err = esp_wifi_set_config(ESP_IF_WIFI_STA, &config)) != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_config()(): %s", esp_err_to_name(err));
        goto fail;
    }
    if ((err = esp_wifi_start()) != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_start(): %s", esp_err_to_name(err));
        goto fail;
    }
    ESP_LOGI(TAG, "init_wifi(): initialization done");
fail:
    return err;
}

void task_statsd_client(void *pvParamters)
{
    statsd_link *link;
    size_t heap_size;

    ESP_LOGI(TAG, "Target IP address: %s", CONFIG_PROJECT_DEST_ADDR);
    ESP_LOGI(TAG, "Target port: %d", CONFIG_PROJECT_DEST_PORT);
    ESP_LOGI(TAG, "Initializing statsd link");
    link = statsd_init_with_namespace(CONFIG_PROJECT_DEST_ADDR, CONFIG_PROJECT_DEST_PORT, "esp");

    ESP_LOGI(TAG, "Starting the loop");
    while (1) {
        heap_size = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        printf("heap size: %d\n", heap_size);
        statsd_gauge(link, "free_heap_size", heap_size);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{

    ESP_ERROR_CHECK(init_wifi());
    ESP_LOGI(TAG, "Configured WiFi. Waiting for WIFI_CONNECTED_BIT...");

    while (1) {
        if (xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdFALSE, 1000)) {
            break;
        }
    }
    xTaskCreate(task_statsd_client, "task_statsd_client", configMINIMAL_STACK_SIZE * 5, NULL, 5, NULL);
}
