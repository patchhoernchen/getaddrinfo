#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define USAGE(output) fprintf(output, "Usage: %s [-468tu] HOST [PORT]\n", argv[0])


struct params {
    char *hostname;
    char *service;
    int type;
    int protocol;
    int passive;
    int family;
};


void run(struct params hostspec);


int main(int argc, char **argv) {

    if ( argc < 2 ) {
        //fprintf(stderr, "Usage: %s host port\n", argv[0]);
        USAGE(stderr); exit(EXIT_FAILURE);
    }

    struct params hostspec;
    hostspec.hostname = "\0";
    hostspec.service = "\0";
    hostspec.type = SOCK_DGRAM;
    hostspec.protocol = 0;
    hostspec.passive = AI_PASSIVE;
    hostspec.family = AF_UNSPEC;

    opterr = 0;
    char opt;

    //while ( (opt = getopt(argc, argv, "468tuPp:")) != -1 ) {
    while ( (opt = getopt(argc, argv, "468tuh")) != -1 ) {
        switch (opt) {
            case '4': hostspec.family = AF_INET; break;
            case '6': hostspec.family = AF_INET6; break;
            case '8': hostspec.family = AF_UNSPEC; break;
            case 't': hostspec.type = SOCK_STREAM; break;
            case 'u': hostspec.type = SOCK_DGRAM; break;
            //case 'P': hostspec.passive = AI_PASSIVE; break;
            //case 'p': hostspec.protocol = atoi(optarg); break;
            case 'h': USAGE(stdout); exit(EXIT_SUCCESS);
            default: USAGE(stderr); exit(EXIT_FAILURE);
        }
    }

    if (optind == argc-1) {
        hostspec.hostname = argv[optind];
        hostspec.service = NULL;
    } else if (optind == argc-2) {
        hostspec.hostname = argv[optind];
        hostspec.service = argv[optind+1];
    } else {
        USAGE(stderr); exit(EXIT_FAILURE);
    }

    run(hostspec);
    return 0;
}


void run(struct params hostspec) {

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = hostspec.family;
    hints.ai_socktype = hostspec.type;
    hints.ai_flags = hostspec.passive;
    hints.ai_protocol = hostspec.protocol;

    int ret = getaddrinfo(hostspec.hostname, hostspec.service, &hints, &result);
    if ( ret != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        char s[INET6_ADDRSTRLEN > INET_ADDRSTRLEN ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN];
        s[0] = '\0';
        switch (rp->ai_addr->sa_family) {
            case AF_INET: {
                struct sockaddr_in *sa_in = (struct sockaddr_in *)rp->ai_addr;
                inet_ntop(AF_INET, &(sa_in->sin_addr), s, INET_ADDRSTRLEN);
                break;
            }
            case AF_INET6: {
                struct sockaddr_in6 *sa_in = (struct sockaddr_in6 *)rp->ai_addr;
                inet_ntop(AF_INET6, &(sa_in->sin6_addr), s, INET6_ADDRSTRLEN);
                break;
            }
            default:
                break;
        }
        printf("%s\n", s);
    }
}

