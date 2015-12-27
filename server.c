/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sockfd, newsockfd, port, n;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
pid_t pid;

void communicateWithClient(int newsockfd);
void error(const char* msg);
void checkArguments(int argc, char* argv[]);
void checkOpeningSocket();
void setupSocketAdresse();
void bindSocket();
void checkAccept();
void checkRead();
void checkWrite();

int main(int argc, char* argv[])
{
	checkArguments(argc, argv);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	checkOpeningSocket();

	setupSocketAdresse();

	bindSocket();

	clilen = sizeof(cli_addr);

	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
		checkAccept();

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
	while (1) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		checkRead();

		printf("C:	 %s\n", buffer);

		strcat(message, "t");

		n = write(newsockfd, message, 18);
		checkWrite();
		printf("S: 		 %s \n", message);
	}
}

void checkArguments(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	port = atoi(argv[1]);
}

void checkOpeningSocket()
{
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
}

void error(const char* msg)
{
	perror(msg);
	exit(1);
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

void checkAccept()
{
	printf("Connection established!\n");
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
}

void checkRead()
{
	if (n < 0) {
		error("ERROR reading from socket");
	}
}

void checkWrite()
{
	if (n < 0) {
		error("ERROR writing to socket");
	}
}
