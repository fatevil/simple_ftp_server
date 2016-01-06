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

#define BSIZE 256

int sockfd, newsockfd, port = 0, n;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
pid_t pid;

void communicateWithCmd();
void communicateWithClient(int newsockfd);
void setupSocketAdresse();
void bindSocket();

int main(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	port = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	setupSocketAdresse();

	bindSocket();

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
	printf("doing 4");
	close(newsockfd);
	close(sockfd);
	return 0;
}

void communicateWithCmd()
{
	char buffer[BSIZE];
	Command* cmd = malloc(sizeof(Command));
	State* state = malloc(sizeof(State));
	strcpy(state->pwd, "./data/");

	while (1) {
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		parseCommand(buffer, cmd);

		bzero(buffer, 256);
		handleCommand(cmd, buffer, state, BSIZE);
		printf("lS:     %s \n", buffer);
	}
}

void communicateWithClient(int newsockfd)
{
	Command* cmd = malloc(sizeof(Command));
	State* state = malloc(sizeof(State));
	strcpy(state->pwd, "./data/");

	char buffer[BSIZE];
	while (1) {
		bzero(cmd->arg, 100);
		bzero(cmd->command, 5);
		bzero(buffer, BSIZE);

		/*  Read client text input */
		n = read(newsockfd, buffer, BSIZE - 1);
		if (n < 0) {
			error("ERROR reading from socket");
		}
		printf("C:    %s", buffer);
		//			system(buffer);

		/*  Handle client text input  */
		parseCommand(buffer, cmd);
		bzero(buffer, 256);
		handleCommand(cmd, buffer, state, BSIZE);

		/* Send back response */
		n = write(newsockfd, buffer, 60);
		if (n < 0) {
			error("ERROR writing to socket");
		}
		printf("S:     %s \n", buffer);
	}
}
void setupSocketAdresse()
{
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
}

void bindSocket()
{
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}
	listen(sockfd, 5);
}
