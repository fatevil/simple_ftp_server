#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void setupSocketAdresse(struct sockaddr_in serv_addr, int port);
void bindSocket(int sockfd, struct sockaddr_in serv_addr);
void communicateWithClient(int newsockfd);
