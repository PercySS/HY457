CC = gcc
CFLAGS = -Wall -g -Wno-deprecated-declarations
OBJS = monitor.o hash.o report.o utils.o syscall_table.o

monitor: $(OBJS)
	$(CC) $(CFLAGS) -o monitor $(OBJS) -lssl -lcrypto

tests/pha.out: tests/pha.c
	$(CC) $(CFLAGS) -o tests/pha.out tests/pha.c

tests/pha2.out: tests/pha2.c
	$(CC) $(CFLAGS) -o tests/pha2.out tests/pha2.c

run: monitor tests/pha.out
	./monitor ./tests/pha.out

run2: monitor tests/pha2.out
	./monitor ./tests/pha2.out

clean:
	rm -f monitor tests/pha.out tests/pha2.out *.o report.txt
