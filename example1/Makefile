# gdb ./clients
# r / run
# bt / backtrace
# f / frame
# print var

CFLAGS ?= -g #debug
# CFLAGS = -O3
override CFLAGS += -Wall -Wpedantic -Wextra

.PHONY: all clean

all: example

example: src/main.c
	gcc $(CFLAGS) -o $@ $< -lpthread

clean:
	rm -f example