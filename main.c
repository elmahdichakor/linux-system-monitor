// main.c
#include <ncurses.h>
#include <unistd.h>
#include "stats.h"

void draw_bar(WINDOW *win, int y, int x, double pct, int color_pair) {
    int width = 20;
    int filled = (int)(pct / 100 * width);
    wattron(win, COLOR_PAIR(color_pair));
    mvwprintw(win, y, x, "[");
    for(int i=0; i<width; i++) {
        if(i < filled) waddch(win, '#'); else waddch(win, '-');
    }
    wprintw(win, "] %.1f%%", pct);
    wattroff(win, COLOR_PAIR(color_pair));
}

int main() {
    initscr(); noecho(); curs_set(0); start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    CPUData p_cpu, c_cpu;
    NetData p_net, c_net;
    MemData m;

    while(1) {
        get_cpu(&p_cpu); get_net(&p_net);
        sleep(1);
        get_cpu(&c_cpu); get_net(&c_net);
        get_mem(&m);

        double cpu_p = 100.0 * (1.0 - (double)(c_cpu.idle - p_cpu.idle) / (c_cpu.total - p_cpu.total));
        double mem_p = (double)m.used / m.total * 100.0;

        clear();
        WINDOW *win = newwin(15, 50, 1, 1);
        box(win, 0, 0);
        
        wattron(win, A_BOLD | COLOR_PAIR(1));
        mvwprintw(win, 1, 2, "--- Mahdi's Industrial Dashboard v2.0 ---");
        wattroff(win, A_BOLD | COLOR_PAIR(1));

        mvwprintw(win, 3, 2, "CPU Usage: "); draw_bar(win, 3, 13, cpu_p, 3);
        mvwprintw(win, 5, 2, "RAM Usage: "); draw_bar(win, 5, 13, mem_p, 2);
        mvwprintw(win, 6, 13, "(%ld / %ld MB)", m.used/1024, m.total/1024);

        mvwprintw(win, 8, 2, "NETWORK ACTIVITY:");
        mvwprintw(win, 10, 4, "Download: %.2f KB/s", (double)(c_net.rx - p_net.rx)/1024.0);
        mvwprintw(win, 11, 4, "Upload  : %.2f KB/s", (double)(c_net.tx - p_net.tx)/1024.0);

        mvwprintw(win, 13, 2, "Press Ctrl+C to terminate safely.");
        
        wrefresh(win);
    }
    endwin();
    return 0;
}
