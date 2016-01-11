#include <stdio.h>
#include <netdb.h>

#define BSIZE 256
#define BASEFOLDER "./data/"
#define UNSET_MODE 0
#define ACTIVE_MODE 1
#define PASSIVE_MODE 2

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

	/* Only for the active mode - keeps ip adresse and port of listening remote server. */
	// ServerAdresse* dataServerAdresse;

	/* Socket for data transmission*/
	int socket;

} State;

int createSpeakingSocket(struct hostent* server, int port);
int createListeningSocket(int port);
void communicateWithClient(int newsockfd);
void communicateWithCmd();
void writeMessage(int socket, char buffer[]);
void readMessage(int socket, char buffer[]);
int startActiveModeDataConnection(struct hostent* server, int port);
ssize_t sendFileOurWay(int out_fd, int in_fd, off_t* offset, size_t count);