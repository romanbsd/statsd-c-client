## An example for `esp-idf`

This is a modified version of `test-client.c` for `esp-idf`.

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
I (263) boot: Loaded app from partition at offset 0x10000
I (301) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (301) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (491) phy_init: phy ver: 1055_12
I (491) reset_reason: RTC reset 2 wakeup 0 store 0, reason is 2
I (511) simple_demo: Connecting to SSID: My SSID
I (521) simple_demo: init_wifi(): initialization done
I (521) simple_demo: Configured WiFi. Waiting for WIFI_CONNECTED_BIT...
I (1731) event: sta ip: 192.168.1.33, mask: 255.255.255.0, gw: 192.168.1.1
I (1731) event_handler_esp8266: Got IP address: 192.168.1.33
I (1731) simple_demo: Target IP address: 192.168.1.118
I (1741) simple_demo: Target port: 8125
I (1751) simple_demo: Initializing statsd link
I (1761) simple_demo: Sending count1 with values `123` and 1.0)
I (1771) simple_demo: Initializing statsd link2 with namespace `mynamespace`
I (1781) simple_demo: Sending count2 with values 125 and 1.0
I (1791) simple_demo: Sending guage `speed` with value 10
I (1801) simple_demo: Sending timing `request` with value 2400
I (2811) simple_demo: Increment count1
I (2811) simple_demo: Decriment count2
I (2821) simple_demo: Sending count3 10 times in a loop
I (2831) simple_demo: Cleanup link and link2
I (2841) simple_demo: End of the example
```
