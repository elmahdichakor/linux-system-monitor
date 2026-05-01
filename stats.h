// stats.h
#ifndef STATS_H
#define STATS_H

typedef struct {
    unsigned long long user, idle, total;
} CPUData;

typedef struct {
    long total, used;
} MemData;

typedef struct {
    unsigned long long rx, tx;
} NetData;

void get_cpu(CPUData *c);
void get_mem(MemData *m);
void get_net(NetData *n);

#endif
