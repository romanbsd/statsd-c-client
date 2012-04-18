#ifndef _H_STATSD_CLIENT
#define _H_STATSD_CLIENT
#include <sys/types.h>

int statsd_init(const char *host, int port);
int statsd_init_with_namespace(const char *host, int port, const char *ns);
void statsd_finalize(void);

/*
  write the stat line to the provided buffer,
  type can be "c", "g" or "ms"
  lf - whether line feed needs to be added
 */
void statsd_prepare(char *stat, size_t value, const char *type, float sample_rate, char *buf, size_t buflen, int lf);
/* manually send a message, which might be composed of several lines. Must be null-terminated */
void statsd_send(const char *message);

void statsd_inc(char *stat, float sample_rate);
void statsd_dec(char *stat, float sample_rate);
void statsd_count(char *stat, size_t count, float sample_rate);
void statsd_gauge(char *stat, size_t value);
void statsd_timing(char *stat, size_t ms);
#endif
