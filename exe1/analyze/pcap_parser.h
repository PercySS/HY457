#ifndef PCAP_PARSER_H
#define PCAP_PARSER_H

#define MAX_PORTS 64
#define MAX_PROTOCOLS 2

#include <pcap.h>

typedef struct {
    char ip[INET_ADDRSTRLEN];
    int packet_count;
    int total_bytes;

    int source_ports[MAX_PORTS];
    int dest_ports[MAX_PORTS];
    int port_count_src;
    int port_count_dst;

    char protocols[MAX_PROTOCOLS][4]; 
    int proto_count;
} HostEntry;

void parse_pcap(const char *filename, const char *filter_ip);

#endif
