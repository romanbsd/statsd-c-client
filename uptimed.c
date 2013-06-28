#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/sysinfo.h>

#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#include "statsd-client.h"

static int running = 1;

void sigterm(int sig)
{
    running = 0;
}

#define MAX_LINE_LEN 200
#define PKT_LEN 1400

int find_my_mac(const char *iface, char *mac)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (s == -1) {
        perror("socket");
        return -1;
    }

    if ( ioctl(s, SIOCGIFHWADDR, &ifr) == -1 ) {
        perror("ioctl(SIOCGIFHWADDR)");
        close(s);
        return -1;
    }
    unsigned char macaddr[6];
    memcpy(macaddr, ifr.ifr_hwaddr.sa_data, 6);
    sprintf(mac, "%.2X%.2X%.2X%.2X%.2X%.2X",
            macaddr[0], macaddr[1], macaddr[2],
            macaddr[3], macaddr[4], macaddr[5]);
    close(s);
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *net, *stat;
    struct sysinfo si;
    char line[MAX_LINE_LEN], tmp[MAX_LINE_LEN], pkt[PKT_LEN], ns[16];
    char *lasts, *iface, *p;
    unsigned int user, nice, sys, idle, total, busy, old_total=0, old_busy=0;
    statsd_link *link;
    int i;

    if (argc != 3) {
        printf("Usage: %s host interface\n", argv[0]);
        exit(1);
    }
    iface = argv[2];
    strcpy(ns, "host.");
    if (find_my_mac(iface, ns + 5)!=0) {
        fprintf(stderr, "Cannot find mac address\n");
    }

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN); /* will save one syscall per sleep */
    signal(SIGTERM, sigterm);

    if ( (net = fopen("/proc/net/dev", "r")) == NULL) {
        perror("fopen");
        exit(-1);
    }

    if ( (stat = fopen("/proc/stat", "r")) == NULL) {
        perror("fopen");
        exit(-1);
    }

    link = statsd_init_with_namespace(argv[1], 8125, ns);

    daemon(0,0);

    while(running) {
        pkt[0] = 0;
        sysinfo(&si);

        statsd_prepare(link, "load", 100*si.loads[0]/0x10000, "g", 1.0, tmp, MAX_LINE_LEN, 1);
        strcat(pkt, tmp);

        statsd_prepare(link, "freemem", si.freeram/1024, "g", 1.0, tmp, MAX_LINE_LEN, 1);
        strcat(pkt, tmp);

/*
        statsd_prepare("freeswap", si.freeswap/1024, "g", 1.0, tmp, MAX_LINE_LEN, 1);
        strcat(pkt, tmp);
*/

        statsd_prepare(link, "procs", si.procs, "g", 1.0, tmp, MAX_LINE_LEN, 1);
        strcat(pkt, tmp);

        statsd_prepare(link, "uptime", si.uptime, "c", 1.0, tmp, MAX_LINE_LEN, 1);
        strcat(pkt, tmp);

        rewind(net);
        while(!feof(net)) {
            fgets(line, MAX_LINE_LEN, net);
            if (!strstr(line, iface)) {
                continue;
            }
            i = 0;
            for (i = 0, p = strtok_r(line, " ", &lasts); p;
                p = strtok_r(NULL, " ", &lasts), i++) {
                if (i == 1) {
                    statsd_prepare(link, "if-rx", atoi(p), "c", 1.0, tmp, MAX_LINE_LEN, 1);
                    strcat(pkt, tmp);
                } else if (i == 9) {
                    statsd_prepare(link, "if-tx", atoi(p), "c", 1.0, tmp, MAX_LINE_LEN, 1);
                    strcat(pkt, tmp);
                    break;
                }
            }
        }

        /* rewind doesn't do the trick for /proc/stat */
        freopen("/proc/stat", "r", stat);
        fgets(line, MAX_LINE_LEN, stat);
        sscanf(line, "cpu  %u %u %u %u", &user, &nice, &sys, &idle);
        total = user + sys + idle;
        busy = user + sys;

        statsd_prepare(link, "cpu", 100 * (busy - old_busy)/(total - old_total), "g", 1.0, tmp, MAX_LINE_LEN, 0);
        strcat(pkt, tmp);

        // printf("pkt:\n%s\n\n", pkt);
        statsd_send(link, pkt);

        old_total = total;
        old_busy = busy;
        sleep(60);
    }

    fclose(net);
    fclose(stat);
    statsd_finalize(link);

    exit(0);
}
