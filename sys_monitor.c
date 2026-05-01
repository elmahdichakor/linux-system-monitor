#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/utsname.h>

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CPUStats;

void handle_exit(int sig) {
    (void)sig;
    printf("\n\033[0;32m[+] Mahdi's Dashboard closed. Happy Engineering!\033[0m\n");
    exit(0);
}

void get_cpu_stats(CPUStats *s) {
    FILE *fp = fopen("/proc/stat", "r");
    if (fp) {
        fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
               &s->user, &s->nice, &s->system, &s->idle, &s->iowait, &s->irq, &s->softirq, &s->steal);
        fclose(fp);
    }
}

double calculate_cpu_usage(CPUStats *prev, CPUStats *curr) {
    unsigned long long prev_idle = prev->idle + prev->iowait;
    unsigned long long curr_idle = curr->idle + curr->iowait;
    unsigned long long prev_total = prev_idle + prev->user + prev->nice + prev->system + prev->irq + prev->softirq + prev->steal;
    unsigned long long curr_total = curr_idle + curr->user + curr->nice + curr->system + curr->irq + curr->softirq + curr->steal;
    if (curr_total <= prev_total) return 0.0;
    return (double)((curr_total - prev_total) - (curr_idle - prev_idle)) / (curr_total - prev_total) * 100.0;
}

void print_bar(double percent, char* color) {
    int width = 20;
    int filled = (int)(percent / 100 * width);
    if (filled > width) filled = width;
    printf("%s[", color);
    for(int i=0; i<width; i++) {
        if(i < filled) printf("#"); else printf("-");
    }
    printf("] %.1f%%\033[0m", percent);
}

int main() {
    signal(SIGINT, handle_exit);
    CPUStats prev_cpu, curr_cpu;
    unsigned long long p_rx, p_tx, c_rx, c_tx;
    char iface[32];

    while (1) {
        get_cpu_stats(&prev_cpu);
        
        // قراءة النيتورك (Smart Search)
        FILE *fn = fopen("/proc/net/dev", "r");
        char line[256];
        while(fgets(line, 256, fn)) {
            if (strstr(line, "eth") || strstr(line, "wlan") || strstr(line, "enp") || strstr(line, "virbr1")) {
                sscanf(line, " %[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu", iface, &p_rx, &p_tx);
                break;
            }
        }
        fclose(fn);

        sleep(1);

        get_cpu_stats(&curr_cpu);
        double cpu_p = calculate_cpu_usage(&prev_cpu, &curr_cpu);

        fn = fopen("/proc/net/dev", "r");
        while(fgets(line, 256, fn)) {
            if (strstr(line, iface)) {
                sscanf(line, " %[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu", iface, &c_rx, &c_tx);
                break;
            }
        }
        fclose(fn);

        long mTotal, mFree;
        FILE *f = fopen("/proc/meminfo", "r");
        fscanf(f, "MemTotal: %ld kB\nMemFree: %ld kB", &mTotal, &mFree);
        fclose(f);

        struct utsname sys; uname(&sys);
        
        printf("\033[H\033[J");
        printf("\033[1;36m--- 🐧 Mahdi's Ultimate System Dashboard ---\033[0m\n");
        printf("OS: %s | Kernel: %s\n", sys.sysname, sys.release);
        printf("------------------------------------------\n");
        
        printf("CPU  : "); print_bar(cpu_p, "\033[0;31m"); printf("\n");
        double mem_p = (double)(mTotal - mFree) / mTotal * 100;
        printf("RAM  : "); print_bar(mem_p, "\033[0;32m"); printf(" (%ld/%ld MB)\n", (mTotal-mFree)/1024, mTotal/1024);
        
        printf("------------------------------------------\n");
        printf("\033[1;33mNETWORK (%s):\033[0m\n", iface);
        printf("  Download: %.2f KB/s\n", (double)(c_rx - p_rx) / 1024.0);
        printf("  Upload  : %.2f KB/s\n", (double)(c_tx - p_tx) / 1024.0);
        printf("------------------------------------------\n");
    }
    return 0;
}
