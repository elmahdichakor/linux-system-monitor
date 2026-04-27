#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void get_memory_usage() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        return;
    }

    char label[32];
    unsigned long value;
    unsigned long memTotal = 0, memFree = 0;

    while (fscanf(fp, "%31s %lu kB", label, &value) != EOF) {
        if (strcmp(label, "MemTotal:") == 0) memTotal = value;
        if (strcmp(label, "MemFree:") == 0) memFree = value;
    }

    fclose(fp);

    printf("--- Linux Memory Monitor ---\n");
    printf("Total RAM: %lu MB\n", memTotal / 1024);
    printf("Free RAM:  %lu MB\n", memFree / 1024);
    printf("Used RAM:  %lu MB\n", (memTotal - memFree) / 1024);
}

int main() {
    get_memory_usage();
    return 0;
}
