#include "server.h"

#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "list.h"

// Forward declarations

static struct addrinfo *getaddrinfo_for_localhost(int ai_family,
                                                  int ai_socktype);

static int bind_to_first_addrinfo_possible(struct addrinfo *res);

static void enable_immediate_reuse_for_socket(int fd);

int main(int argc, char *argv[]) {
    int listener_fd = create_listener_socket();

    // Create thread to accept new connections.
    //start_acceptor_thread();

    return 0;
}

static int create_listener_socket() {
    struct addrinfo *res = getaddrinfo_for_localhost(AF_UNSPEC, SOCK_STREAM);

    int listener_fd = bind_to_first_addrinfo_possible(res);

    if (listen(listener_fd, MAX_QUEUED_CONNECTIONS) < 0) {
        perror("listen");
        exit(1);
    }

    freeaddrinfo(res);

    return 0;
}

// NB: Return pointer is dynamically allocated.
static struct addrinfo *getaddrinfo_for_localhost(int ai_family,
                                                  int ai_socktype) {
    int status;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = ai_family;
    hints.ai_socktype = ai_socktype;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, PORT, &hints, &res)) < 0) {
        fprintf("getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    return res;
}

static int bind_to_first_addrinfo_possible(struct addrinfo *res) {
    assert(res != NULL);

    int fd;
    int status;
    struct addrinfo *current;
    for (current = res; current != NULL; current = current->ai_next) {
        if ((fd = socket(current->ai_family, current->ai_socktype,
                         current->ai_protocol)) < 0) {
            continue;
        }

        enable_immediate_reuse_for_socket(fd);

        if ((status = bind(fd, current->ai_addr, current->ai_addrlen)) < 0) {
            close(fd);
            continue;
        }

        return fd;
    }

    perror("bind_to_first_addrinfo_possible");
    exit(1);
}

static void enable_immediate_reuse_for_socket(int fd) {
    int yes = 1;
    int status;
    if ((status = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) <
        0) {
        perror("setsockopt");
    }
}
