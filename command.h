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
	char arg[100];
} Command;

void parseCommand(char* cmdstring, Command* cmd);
void handleCommand(Command* command, char* string, State* state, int arrayLength);
void setResponse(int code, char message[], char buffer[]);
void executeCD(Command* command, char buffer[], State* state, int arrayLength);
void executeCDUP(Command* command, char buffer[], State* state, int arrayLength);
void executeMKD(Command* command, char buffer[], State* state, int arrayLength);