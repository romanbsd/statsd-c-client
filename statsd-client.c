#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "statsd-client.h"

#define MAX_MSG_LEN 100

static int sock = -1;
static struct sockaddr_in server;
static char *ns = NULL;

int statsd_init_with_namespace(const char *host, int port, const char *ns_)
{
    size_t len = strlen(ns_);
    if (ns) {
        free(ns);
    }
    if ( (ns = malloc(len + 2)) == NULL ) {
        perror("malloc");
        return -1;
    }
    strcpy(ns, ns_);
    ns[len++] = '.';
    ns[len] = 0;

    return statsd_init(host, port);
}

int statsd_init(const char *host, int port)
{
    if (sock == -1) {
        if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) {
            perror("socket");
            return -1;
        }
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    struct addrinfo *result = NULL, hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int error;
    if ( (error = getaddrinfo(host, NULL, &hints, &result)) ) {
        fprintf(stderr, "%s\n", gai_strerror(error));
        return -1;
    }
    memcpy(&server.sin_addr, &((struct sockaddr_in*)result->ai_addr)->sin_addr, sizeof(struct in_addr));
    freeaddrinfo(result);

    // if (inet_aton(host, &server.sin_addr)==0) {
    //     perror("inet_aton");
    //     return -1;
    // }
    srandom(time(NULL));

    return 0;
}

void statsd_finalize()
{
    if (sock != -1) {
        close(sock);
        sock = -1;
    }
    if (ns) {
        free(ns);
        ns = NULL;
    }
}

/* will change the original string */
static void cleanup(char *stat)
{
    char *p;
    for (p = stat; *p; p++) {
        if (*p == ':' || *p == '|' || *p == '@') {
            *p = '_';
        }
    }
}

static int should_send(float sample_rate)
{
    if (sample_rate < 1.0) {
        float p = ((float)random() / RAND_MAX);
        return sample_rate > p;
    } else {
        return 1;
    }
}

void statsd_send(const char *message)
{
    int slen = sizeof(server);

    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server, slen)==-1) {
        perror("sendto");
    }
}

static void send_stat(char *stat, size_t value, const char *type, float sample_rate)
{
    char message[MAX_MSG_LEN];
    if (!should_send(sample_rate)) {
        return;
    }

    statsd_prepare(stat, value, type, sample_rate, message, MAX_MSG_LEN, 0);
    statsd_send(message);
}

void statsd_prepare(char *stat, size_t value, const char *type, float sample_rate, char *message, size_t buflen, int lf)
{
    cleanup(stat);
    if (sample_rate == 1.0) {
        snprintf(message, buflen, "%s%s:%zd|%s%s", ns ? ns : "", stat, value, type, lf ? "\n" : "");
    } else {
        snprintf(message, buflen, "%s%s:%zd|%s|@%.2f%s", ns ? ns : "", stat, value, type, sample_rate, lf ? "\n" : "");
    }
}

/* public interface */
void statsd_count(char *stat, size_t value, float sample_rate)
{
    send_stat(stat, value, "c", sample_rate);
}

void statsd_dec(char *stat, float sample_rate)
{
    statsd_count(stat, -1, sample_rate);
}

void statsd_inc(char *stat, float sample_rate)
{
    statsd_count(stat, 1, sample_rate);
}

void statsd_gauge(char *stat, size_t value)
{
    send_stat(stat, value, "g", 1.0);
}

void statsd_timing(char *stat, size_t ms)
{
    send_stat(stat, ms, "ms", 1.0);
}
