#include <stdio.h>

#define BSIZE 256
#define BASEFOLDER "./data/"
#define ACTIVE_MODE 0
#define PASSIVE_MODE 1

typedef struct Message
{
	/* Current directory */
	char pwd[100];

} Message;

typedef struct State
{
	/* Current directory */
	char pwd[100];

	/* 0 for client not logged in yet
	1 for client already logged in*/
	int loggedIn;

	/* 1 to keep connection
	0  to close connection after dealing with commands */
	int keepConnection;

	/* Passive and active mode */
	int mode;

} State;

void communicateWithClient(int newsockfd);
void communicateWithCmd();
void writeMessage(int socket, char buffer[]);
void readMessage(int socket, char buffer[]);