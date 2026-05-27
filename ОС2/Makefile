# makefile for scheduling program for Linux/Ubuntu

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lc

all: fcfs sjf rr priority priority_rr

clean:
	rm -f *.o
	rm -f fcfs sjf rr priority priority_rr

fcfs: driver.o list.o CPU.o schedule_fcfs.o
	$(CC) $(CFLAGS) -o fcfs driver.o list.o CPU.o schedule_fcfs.o $(LDFLAGS)

sjf: driver.o list.o CPU.o schedule_sjf.o
	$(CC) $(CFLAGS) -o sjf driver.o list.o CPU.o schedule_sjf.o $(LDFLAGS)

rr: driver.o list.o CPU.o schedule_rr.o
	$(CC) $(CFLAGS) -o rr driver.o list.o CPU.o schedule_rr.o $(LDFLAGS)

priority: driver.o list.o CPU.o schedule_priority.o
	$(CC) $(CFLAGS) -o priority driver.o list.o CPU.o schedule_priority.o $(LDFLAGS)

priority_rr: driver.o list.o CPU.o schedule_priority_rr.o
	$(CC) $(CFLAGS) -o priority_rr driver.o list.o CPU.o schedule_priority_rr.o $(LDFLAGS)

driver.o: driver.c schedulers.h task.h list.h
	$(CC) $(CFLAGS) -c driver.c

schedule_fcfs.o: schedule_fcfs.c task.h list.h cpu.h
	$(CC) $(CFLAGS) -c schedule_fcfs.c

schedule_sjf.o: schedule_sjf.c task.h list.h cpu.h
	$(CC) $(CFLAGS) -c schedule_sjf.c

schedule_priority.o: schedule_priority.c task.h list.h cpu.h
	$(CC) $(CFLAGS) -c schedule_priority.c

schedule_rr.o: schedule_rr.c task.h list.h cpu.h
	$(CC) $(CFLAGS) -c schedule_rr.c

schedule_priority_rr.o: schedule_priority_rr.c task.h list.h cpu.h
	$(CC) $(CFLAGS) -c schedule_priority_rr.c

list.o: list.c list.h task.h
	$(CC) $(CFLAGS) -c list.c

CPU.o: CPU.c cpu.h task.h
	$(CC) $(CFLAGS) -c CPU.c
