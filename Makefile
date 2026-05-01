CC=gcc
CFLAGS=-Wall -Wextra
LIBS=-lncurses

all: mahdi_monitor

mahdi_monitor: main.c stats.c
	$(CC) $(CFLAGS) main.c stats.c -o mahdi_monitor $(LIBS)

clean:
	rm -f mahdi_monitor
