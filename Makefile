CC=gcc
CFLAGS=-Wall -Wextra

all: sys_monitor

sys_monitor: sys_monitor.c
	$(CC) $(CFLAGS) sys_monitor.c -o sys_monitor

clean:
	rm -f sys_monitor
