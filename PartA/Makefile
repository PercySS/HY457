all: demo

demo: demo.o cs457_crypto.o
	$(CC) -o demo demo.o cs457_crypto.o

demo.o: demo.c cs457_crypto.h
	$(CC) -c demo.c

cs457_crypto.o: cs457_crypto.c cs457_crypto.h
	$(CC) -c cs457_crypto.c

clean:
	rm -f *.o demo
