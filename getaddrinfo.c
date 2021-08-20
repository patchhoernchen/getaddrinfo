#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {

    if ( argc < 3 ) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    //hints.ai_socktype = SOCK_STREAM;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    int ret = getaddrinfo(argv[1], argv[2], &hints, &result);
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

    return 0;
}

