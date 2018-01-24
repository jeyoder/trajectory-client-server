#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "trajectory.h"
#include "trajectory_generator.h"

/* Transmit a trajectory packet in network format on the given (opened) TCP socket. */
ssize_t send_trajectory(int sockfd, trajectory_t *traj) {

    /* Serialize the trajectory packet to a new buffer */
    char *buf = NULL;
    size_t len = trajectory_serialize(traj, &buf);

    ssize_t sent = send(sockfd, buf, len, 0);

    /* Free the buffer that trajectory_serialize allocated */
    free(buf);

    return sent;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: trajectory_generator [hostname]\n");
        return 255;
    }

    srand(time(NULL));

    /* Generate a trajectory. */
    trajectory_t *traj = trajectory_generate();

    /* Create a TCP socket. */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Unable to create a socket\n");
        return 1;
    }

    struct hostent *host = gethostbyname(argv[1]);
    if (host == NULL) {
        fprintf(stderr, "Unable to resolve host %s\n", argv[1]);
        return 254;
    }

    /* Set the remote host and port on the socket. */
    struct sockaddr_in sock_data;
    memset(&sock_data, 0, sizeof(sock_data));

    sock_data.sin_port = htons(1234);
    inet_aton(host->h_addr_list[0], &sock_data.sin_addr);
    sock_data.sin_family = AF_INET;
   
    /* Start the TCP connection */
    if (connect(sockfd, (struct sockaddr *) &sock_data, sizeof(struct sockaddr_in)) < 0) {
        perror("Connection error\n");
        return 2;
    }
    
    ssize_t sent = send_trajectory(sockfd, traj);

    printf("Transmitted %d bytes\n", (int) sent);

    trajectory_free(traj);
}
