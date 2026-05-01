// stats.c
#include <stdio.h>
#include <string.h>
#include "stats.h"

void get_cpu(CPUData *c) {
    FILE *fp = fopen("/proc/stat", "r");
    unsigned long long nice, system, idle, iowait, irq, softirq, steal;
    fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &c->user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    c->total = c->user + nice + system + idle + iowait + irq + softirq + steal;
    c->idle = idle + iowait;
    fclose(fp);
}

void get_mem(MemData *m) {
    FILE *fp = fopen("/proc/meminfo", "r");
    long mFree, mCached, mBuffers;
    fscanf(fp, "MemTotal: %ld kB\nMemFree: %ld kB\nBuffers: %ld kB\nCached: %ld kB", 
           &m->total, &mFree, &mBuffers, &mCached);
    m->used = m->total - (mFree + mBuffers + mCached);
    fclose(fp);
}

void get_net(NetData *n) {
    FILE *fp = fopen("/proc/net/dev", "r");
    char line[256];
    n->rx = 0; n->tx = 0;
    while(fgets(line, 256, fp)) {
        if (strstr(line, "eth") || strstr(line, "wlan") || strstr(line, "enp") || strstr(line, "virbr")) {
            unsigned long long r, t;
            sscanf(line, " %*[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu", &r, &t);
            n->rx += r; n->tx += t;
        }
    }
    fclose(fp);
}
