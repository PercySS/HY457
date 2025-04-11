CC = gcc
CFLAGS = -Wall -Wextra -O2

OBJS = vault.o split.o join.o

vault: $(OBJS)
	$(CC) $(CFLAGS) -o vault $(OBJS)

vault.o: vault.c split.h join.h
split.o: split.c split.h
join.o: join.c join.h

run:
	./vault split 17
	./vault join

clean:
	rm -f *.o vault points.txt password.txt
