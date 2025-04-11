#include "pcap_parser.h"
#include "utils.h"

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#define MAX_HOSTS 128
#define MAX_PORTS 64
#define MAX_PROTOCOLS 2

static HostEntry hosts[MAX_HOSTS];
static int host_count = 0;

static HostEntry* find_or_create_host(const char *ip) {
    for (int i = 0; i < host_count; ++i) {
        if (strcmp(hosts[i].ip, ip) == 0) return &hosts[i];
    }

    if (host_count >= MAX_HOSTS) return NULL;

    HostEntry *h = &hosts[host_count++];
    strncpy(h->ip, ip, INET_ADDRSTRLEN);
    h->packet_count = 0;
    h->total_bytes = 0;
    h->port_count_src = 0;
    h->port_count_dst = 0;
    h->proto_count = 0;
    return h;
}

static void add_port(int *ports, int *count, int port) {
    for (int i = 0; i < *count; ++i) {
        if (ports[i] == port) return;
    }
    if (*count < MAX_PORTS) {
        ports[(*count)++] = port;
    }
}

static void add_protocol(HostEntry *h, const char *proto) {
    for (int i = 0; i < h->proto_count; ++i) {
        if (strcmp(h->protocols[i], proto) == 0) return;
    }
    if (h->proto_count < MAX_PROTOCOLS) {
        strncpy(h->protocols[h->proto_count++], proto, 4);
    }
}

static void print_all_hosts() {
    for (int i = 0; i < host_count; ++i) {
        HostEntry *h = &hosts[i];
        print_time_prefix();
        printf("IP: %s %d outgoing packets [%d Bytes]\n", h->ip, h->packet_count, h->total_bytes);

        printf("SourcePorts: ");
        for (int j = 0; j < h->port_count_src; ++j)
            printf("%d%s", h->source_ports[j], j < h->port_count_src - 1 ? "," : "");
        printf("\n");

        printf("DestinationPorts: ");
        for (int j = 0; j < h->port_count_dst; ++j)
            printf("%d%s", h->dest_ports[j], j < h->port_count_dst - 1 ? "," : "");
        printf("\n");

        printf("Protocols: ");
        for (int j = 0; j < h->proto_count; ++j)
            printf("%s%s", h->protocols[j], j < h->proto_count - 1 ? "," : "");
        printf("\n");
    }
}


void parse_pcap(const char *filename, const char *filter_ip) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(filename, errbuf);
    if (!handle) {
        fprintf(stderr, "pcap_open_offline failed: %s\n", errbuf);
        return;
    }

    print_time_prefix();
    printf("Filtering traffic of %s\n", filter_ip);

    struct pcap_pkthdr *header;
    const u_char *packet;

    while (pcap_next_ex(handle, &header, &packet) > 0) {
        const struct ip *ip_header = (struct ip*)(packet + 14); // Skip Ethernet

        // Skip non-IPv4
        if (ip_header->ip_v != 4) continue;

        // Match source IP
        char src_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->ip_src), src_ip, INET_ADDRSTRLEN);
        if (strcmp(src_ip, filter_ip) != 0) continue; // Φιλτράρει μόνο με source IP που ταιριάζει με το filter

        char dst_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->ip_dst), dst_ip, INET_ADDRSTRLEN);

        HostEntry *h = find_or_create_host(dst_ip);
        if (!h) continue;

        h->packet_count++;
        h->total_bytes += header->len;

        const u_char *transport = packet + 14 + (ip_header->ip_hl * 4);
        int proto = ip_header->ip_p;

        if (proto == IPPROTO_TCP) {
            const struct tcphdr *tcp = (struct tcphdr*)transport;
            add_port(h->source_ports, &h->port_count_src, ntohs(tcp->th_sport));
            add_port(h->dest_ports, &h->port_count_dst, ntohs(tcp->th_dport));
            add_protocol(h, "TCP");
        } else if (proto == IPPROTO_UDP) {
            const struct udphdr *udp = (struct udphdr*)transport;
            add_port(h->source_ports, &h->port_count_src, ntohs(udp->uh_sport));
            add_port(h->dest_ports, &h->port_count_dst, ntohs(udp->uh_dport));
            add_protocol(h, "UDP");
        }
    }

    pcap_close(handle);
    print_all_hosts();
}





