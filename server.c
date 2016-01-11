/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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

int sockfd, newsockfd, port, n;
socklen_t clilen;
char buffer[BSIZE];
struct sockaddr_in cli_addr;
pid_t pid;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	port = atoi(argv[1]);

	sockfd = createListeningSocket(port);
	pid = fork();
	if (pid == 0) { // child recieving local input

		communicateWithCmd();
		close(sockfd);
		exit(0);
	}

	printf("Listening on port %d \n", port);

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

		if (pid == 0) { // child communitating with client
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

void communicateWithCmd()
{
	char buffer[BSIZE];
	Command* cmd = malloc(sizeof(Command));
	State* state = malloc(sizeof(State));
	strcpy(state->pwd, BASEFOLDER);
	state->loggedIn = 1;

	while (1) {
		bzero(buffer, BSIZE);
		fgets(buffer, BSIZE - 1, stdin);

		parseCommand(buffer, cmd);

		handleCommand(cmd, buffer, state);
		printf("local: %s \n", buffer);
	}
}

void communicateWithClient(int newsockfd)
{
	char buffer[BSIZE];
	Command* cmd = malloc(sizeof(Command));
	State* state = malloc(sizeof(State));
	strcpy(state->pwd, BASEFOLDER);
	state->keepConnection = newsockfd;

	strcpy(buffer, "220 Hi there! \n");
	/*Send back response*/
	writeMessage(newsockfd, buffer);

	while (1) {
		if (state->keepConnection == 0) {
			close(newsockfd);
			exit(0);
		}
		bzeroCommand(cmd);
		bzero(buffer, BSIZE);

		/*Wait and read client*/
		readMessage(newsockfd, buffer);

		/*  Handle client text input  */
		parseCommand(buffer, cmd);
			handleCommand(cmd, buffer, state);

		/*Send back response*/
		writeMessage(newsockfd, buffer);
	}
}
