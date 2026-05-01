#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void get_stats() {
    long memTotal, memFree, uptimeSeconds;
    long user, nice, system, idle;
    char label[32];

    // 1. قراءة الرام (RAM)
    FILE *memInfo = fopen("/proc/meminfo", "r");
    while (fscanf(memInfo, "%s %lu kB", label, &memTotal) != EOF) {
        if (strcmp(label, "MemTotal:") == 0) break;
    }
    while (fscanf(memInfo, "%s %lu kB", label, &memFree) != EOF) {
        if (strcmp(label, "MemFree:") == 0) break;
    }
    fclose(memInfo);

    // 2. قراءة الـ CPU (قراءة أول سطر من /proc/stat)
    FILE *statFile = fopen("/proc/stat", "r");
    fscanf(statFile, "cpu %lu %lu %lu %lu", &user, &nice, &system, &idle);
    fclose(statFile);

    // 3. قراءة الـ Uptime (شحال والماكينة شاعلة)
    FILE *uptimeFile = fopen("/proc/uptime", "r");
    fscanf(uptimeFile, "%lu", &uptimeSeconds);
    fclose(uptimeFile);

    // تنظيف الشاشة وعرض النتائج
    printf("\033[H\033[J"); // ANSI escape code باش يمسح الـ Terminal
    printf("--- 🐧 Mahdi's Advanced System Monitor ---\n");
    printf("RAM Usage: %lu MB / %lu MB\n", (memTotal - memFree) / 1024, memTotal / 1024);
    printf("CPU Raw Stats: User:%lu System:%lu Idle:%lu\n", user, system, idle);
    printf("System Uptime: %lu minutes\n", uptimeSeconds / 60);
    printf("------------------------------------------\n");
    printf("Press Ctrl+C to exit.\n");
}

int main() {
    while (1) {
        get_stats();
        sleep(2); // يتسنى 2 ثواني ويعاود يقرأ
    }
    return 0;
}
