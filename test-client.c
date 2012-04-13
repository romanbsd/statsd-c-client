#include <unistd.h>
#include "statsd-client.h"

int main(void)
{
    statsd_init("127.0.0.1", 8125);
    statsd_count("count1", 123, 1.0);
    statsd_init_with_namespace("127.0.0.1", 8125, "mynamespace");
    statsd_count("count2", 125, 1.0);
    statsd_gauge("speed", 10);
    statsd_timing("request", 2400);
    sleep(1);
    statsd_inc("count1", 1.0);
    statsd_dec("count2", 1.0);
    int i;
    for(i=0; i<10; i++) {
        statsd_count("count3", i, 0.8);
    }
    statsd_finalize();

    return 0;
}
