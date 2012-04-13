#ifndef _H_STATSD_CLIENT
#define _H_STATSD_CLIENT
#include <sys/types.h>

int statsd_init(const char *host, int port);
int statsd_init_with_namespace(const char *host, int port, const char *ns);
void statsd_finalize(void);

void statsd_inc(char *stat, float sample_rate);
void statsd_dec(char *stat, float sample_rate);
void statsd_count(char *stat, size_t count, float sample_rate);
void statsd_gauge(char *stat, size_t value);
void statsd_timing(char *stat, size_t ms);
#endif
