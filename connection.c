#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "connection.h"
#include "command.h"
#include "util.h"

/* Send back response */
void writeMessage(int socket, char buffer[])
{
	int n;
	n = write(socket, buffer, strlen(buffer));
	if (n < 0) {
		error("ERROR writing to socket");
	}
	printf("S:     %s \n", buffer);
}

/*  Read client text input */
void readMessage(int socket, char buffer[])
{
	int n;
	n = read(socket, buffer, BSIZE - 1);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	printf("C:    %s", buffer);
}

void startActiveModeDataConnection(int port)
{
}

int createListeningSocket(int port)
{
	int sock;
	int reuse = 1;

	/* Server addess */
	struct sockaddr_in server_address =
	    (struct sockaddr_in) { AF_INET, htons(port), (struct in_addr) { INADDR_ANY } };

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("ERROR on creating socket");
	}

	/* Address can be reused instantly after program exits */
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

	/* Bind socket to server address */
	if (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		error("ERROR on binding");
	}

	listen(sock, 5);
	return sock;
}