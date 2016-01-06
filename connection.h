#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BSIZE 256
#define BASEFOLDER "./data/"

typedef struct State
{
	/* Current directory */
	char pwd[100];

} State;



void communicateWithClient(int newsockfd);
void communicateWithCmd();