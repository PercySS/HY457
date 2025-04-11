#include "hash.h"
#include "utils.h"
#include "pcap_parser.h"
#include "strings.h"

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr) return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int main(int argc, char *argv[]) {
    /* 2 args pcap file and filter IP */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pcap_file> <filter_ip>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *filter_ip = argv[2];

    print_time_prefix();
    printf("Application Started with argument \'%s\'\n", filename);

    /* compute MD5 and SHA256 hashes */
    char md5[2 * MD5_DIGEST_LENGTH + 1] = {0};
    char sha256[2 * SHA256_DIGEST_LENGTH + 1] = {0};
    compute_hashes(filename, md5, sha256);

    print_time_prefix(); printf("MD5 hash:    %s\n", md5);
    print_time_prefix(); printf("SHA256 hash: %s\n", sha256);
    print_time_prefix(); printf("Initialized data structures\n");

    /* checks if the file is a pcap file or not */
    if (ends_with(filename, ".pcap")) {
        parse_pcap(filename, filter_ip);  /* arg IP filter */
    } else {
        extract_strings(filename);
    }

    return 0;
}
