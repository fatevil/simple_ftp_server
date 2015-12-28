#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Command
{
	char command[5];
	char arg[1024];
} Command;

void parseCommand(char* cmdstring, Command* cmd);
void handleCommand(Command* command, char* string);

