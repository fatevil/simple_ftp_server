#include <stdio.h>
#include <netdb.h>

#define BSIZE 256
#define BASEFOLDER "./data/"
#define UNSET_MODE 0
#define ACTIVE_MODE 1
#define PASSIVE_MODE 2
#define ASCII 0
#define BINARY 0
#define KEEP_CONNECTION 1
#define CUT_CONNECTION 0

typedef struct Message
{
	/* Current directory */
	char pwd[100];

} Message;

typedef struct ServerAdresse
{
	/* IP adresse of given server*/
	char* addresse;

	/* port  on which given server listens*/
	int port;

} ServerAdresse;

typedef struct State
{
	/* Current directory */
	char pwd[100];

	/* 0 for client not logged in yet
	* 1 for client already logged in*/
	int loggedIn;

	/* 1 to keep connection
	* 0  to close connection after dealing with commands */
	int keepConnection;

	/* Passive, none and active mode */
	int mode;

	/* Socket for data transmission*/
	int socket;

	/* Socket for text transmission*/
	int textSocket;

	/* 0 for ASCII,  1 for binary file transfer */
	int type;

	int connectionSocket;

} State;

int createSpeakingSocket(struct hostent* server, int port);
int createListeningSocket(int* port);
void communicateWithClient(int newsockfd);
void communicateWithCmd();
void writeMessage(int socket, char buffer[]);
void readMessage(int socket, char buffer[]);
int startActiveModeDataConnection(struct hostent* server, int port);
int startPassiveModeDataConnection(int* port, State* state);
ssize_t sendFileOurWay(int out_fd, int in_fd, off_t* offset, size_t count);
