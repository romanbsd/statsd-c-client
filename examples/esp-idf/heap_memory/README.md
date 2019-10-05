## `heap_caps_get_free_size` example

A slightly more realistic example for `esp-idf`. This example send the size
returned by `heap_caps_get_free_size(MALLOC_CAP_8BIT)` every 1 sec. The values
can be found under `esp` name space.

## Tested builds

| `esp-idf` version | hardware |
|-------------------|----------|
| ESP8266 RTOS SDK (master branch) | NodeMCU |

## Project configuration

`make menuconfig` has `Project configuration` menu to modify default
configuration.

## Example log output

```
...
I (262) boot: Loaded app from partition at offset 0x10000
I (299) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (299) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (489) phy_init: phy ver: 1055_12
I (489) reset_reason: RTC reset 2 wakeup 0 store 0, reason is 2
I (509) simple_demo: Connecting to SSID: My SSID
I (519) simple_demo: init_wifi(): initialization done
I (519) simple_demo: Configured WiFi. Waiting for WIFI_CONNECTED_BIT...
I (1699) event: sta ip: 192.168.1.33, mask: 255.255.255.0, gw: 192.168.1.1
I (1699) event_handler_esp8266: Got IP address: 192.168.1.33
I (1699) simple_demo: Target IP address: 192.168.1.63
I (1709) simple_demo: Target port: 8125
I (1719) simple_demo: Initializing statsd link
I (1729) simple_demo: Starting the loop
heap size: 47000
heap size: 50696
heap size: 50696
heap size: 50696
heap size: 50696
heap size: 50696
heap size: 50696
...
```

### Example measurements in `influxdb`

This measurements are created by `telegraf` with `statsd` input.

```
> SELECT value FROM "esp_free_heap_size" LIMIT 10
name: esp_free_heap_size
time                value
----                -----
1570254180000000000 50564
1570254190000000000 50696
1570254200000000000 50696
1570254210000000000 50696
1570254220000000000 50696
1570254230000000000 50696
1570254240000000000 50696
1570254250000000000 50696
1570254260000000000 50696
1570254270000000000 50696
```

The excerpt from `telegraf.conf`.

```
[[inputs.statsd]]
  service_address = ":8125"
```
