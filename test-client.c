#include <unistd.h>
#include "statsd-client.h"

int main(void)
{

    statsd_link *link, *link2, *link3;

    link = statsd_init("127.0.0.1", 8125);
    statsd_count(link, "count1", 123, 1.0);
    link2 = statsd_init_with_namespace("127.0.0.1", 8125, "mynamespace");
    statsd_count(link, "count2", 125, 1.0);
    statsd_gauge(link, "speed", 10);
    statsd_timing(link2, "request", 2400);
    link3 = statsd_init_with_tags("127.0.0.1", 8125, "mytag:myvalue");
    statsd_count(link3, "tagcount", 42, 1.0);
    sleep(1);
    statsd_inc(link, "count1", 1.0);
    statsd_dec(link2, "count2", 1.0);
    statsd_inc(link3, "tagcount", 1.0);
    int i;
    for(i=0; i<10; i++) {
        statsd_count(link2, "count3", i, 0.8);
    }
    statsd_finalize(link);
    statsd_finalize(link2);
    statsd_finalize(link3);

    return 0;
}
