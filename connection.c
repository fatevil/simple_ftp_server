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