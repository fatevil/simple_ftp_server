#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
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

int startActiveModeDataConnection(struct hostent* server, int port)
{
	return createSpeakingSocket(server, port);
}

void startPassiveModeDataConnection(int* port, State* state)
{
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	clilen = sizeof(cli_addr);

	int sockfd = createListeningSocket(port);
	//	pid_t pid;

	//	pid = fork();

	//	if (pid == 0) {
	int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
	printf("Data connection established!\n");
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	state->socket = newsockfd;
	printf("%d \n", state->socket);
	//		exit(0);
	//	}
}

int createListeningSocket(int* port)
{
	int sock;
	int reuse = 1;

	/* Server addess */
	struct sockaddr_in server_address =
	    (struct sockaddr_in) { AF_INET, htons(*port), (struct in_addr) { INADDR_ANY } };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("ERROR on creating socket");
	}
	/* Address can be reused instantly after program exits */
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
	/* Bind socket to server address */
	while (bind(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		//		error("ERROR on binding");
		(*port)++;
		server_address = (struct sockaddr_in) { AF_INET, htons(*port), (struct in_addr) { INADDR_ANY } };
	}

	listen(sock, 5);
	return sock;
}

int createSpeakingSocket(struct hostent* server, int port)
{
	struct sockaddr_in serv_addr;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR connecting");
		return -1;
	}

	return sockfd;
}

ssize_t sendFileOurWay(int out_fd, int in_fd, off_t* offset, size_t count)
{
	printf("NO SHIT");
	off_t orig;
	char buf[BSIZE];
	size_t toRead, numRead, numSent, totSent;

	printf("GOT TO OFFSET \n");
	if (offset != NULL) {
		/* Save current file offset and set offset to value in '*offset' */
		orig = lseek(in_fd, 0, SEEK_CUR);
		if (orig == -1)
			return -1;
		if (lseek(in_fd, *offset, SEEK_SET) == -1)
			return -1;
	}

	printf("START COUNTING BITCH \n");
	totSent = 0;
	while (count > 0) {
		toRead = count < BSIZE ? count : BSIZE;

		numRead = read(in_fd, buf, toRead);
		if (numRead == -1)
			return -1;
		if (numRead == 0)
			break; /* EOF */

		numSent = write(out_fd, buf, numRead);
		if (numSent == -1)
			return -1;
		if (numSent == 0) { /* Should never happen */
			perror("sendfile: write() transferred 0 bytes");
			exit(-1);
		}

		count -= numSent;
		totSent += numSent;
		printf("AT LEAST SENT SOMETHING \n");
	}
	printf("JUST GIVE UP \n");
	if (offset != NULL) {
		/* Return updated file offset in '*offset', and reset the file offset
		   to the value it had when we were called. */
		*offset = lseek(in_fd, 0, SEEK_CUR);
		if (*offset == -1)
			return -1;
		if (lseek(in_fd, orig, SEEK_SET) == -1)
			return -1;
	}
	printf("THIS IS THE END \n");
	return totSent;
}