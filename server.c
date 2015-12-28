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

#define BSIZE 256

int sockfd, newsockfd, port, n;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
pid_t pid;

int main(int argc, char* argv[])
{

	checkArguments(argc, argv);
	port = atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	setupSocketAdresse(serv_addr, port);

	bindSocket(sockfd, serv_addr);

	clilen = sizeof(cli_addr);

	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
		printf("Connection established!\n");
		if (newsockfd < 0) {
			error("ERROR on accept");
		}

		pid = fork();
		if (pid < 0)
			error("ERROR on fork");

		if (pid == 0) { // child
			close(sockfd);
			communicateWithClient(newsockfd);
			exit(0);
		} else {
			close(newsockfd);
		}
	}
	close(newsockfd);
	close(sockfd);
	return 0;
}

void communicateWithClient(int newsockfd)
{
	Command* cmd = malloc(sizeof(Command));

	char buffer[256];
	while (1) {
		bzero(buffer, 256);

		/*  Read client text input */
		n = read(newsockfd, buffer, 255);
		if (n < 0) {
			error("ERROR reading from socket");
		}
		printf("C:    %s", buffer);

		/*  Handle client text input  */
		parseCommand(buffer, cmd);
		bzero(buffer, 256);
		handleCommand(cmd, buffer);

		/* Send back response */
		n = write(newsockfd, buffer, 60);
		if (n < 0) {
			error("ERROR writing to socket");
		}
		printf("S:     %s \n", buffer);
	}
}
