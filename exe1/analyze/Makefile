CC = gcc
CFLAGS = -Wall -g -Wno-deprecated-declarations
LIBS = -lpcap -lssl -lcrypto

OBJS = analyze.o hash.o utils.o strings.o pcap_parser.o


analyze: $(OBJS)
	$(CC) $(CFLAGS) -o analyze $(OBJS) $(LIBS)


analyze.o: analyze.c hash.h utils.h strings.h pcap_parser.h
hash.o: hash.c hash.h
utils.o: utils.c utils.h
strings.o: strings.c strings.h utils.h
pcap_parser.o: pcap_parser.c pcap_parser.h utils.h


run: analyze
	./analyze ./tests/traffic.pcap 192.168.153.1

clean:
	rm -f analyze *.o report.txt
