Trivial statsd client in C

See statsd-client.h

Included is a daemon for monitoring a Linux system.
It'll wake up every minute and monitor the following:

* Load
* CPU
* Free memory
* Free swap (disabled)
* Received bytes
* Transmitted bytes
* Procs
* Uptime

The stats sent to statsd will be in "host.MACAddress" namespace.

Usage:

    uptimed statsd-host interface-to-monitor

e.g.

    uptimed 172.16.42.1 eth0
