#include <stdio.h>
#include <string.h>

typedef struct Command
{
	char command[5];
	char arg[100];
} Command;

void parseCommand(char* cmdstring, Command* cmd);
void handleCommand(Command* command, char* string, State* state, int arrayLength);
void setResponseMessage(int code, char message[], char buffer[]);
void executeCD(Command* command, char buffer[], State* state, int arrayLength);
void executeCDUP(Command* command, char buffer[], State* state, int arrayLength);
void executeMKD(Command* command, char buffer[], State* state, int arrayLength);
void executeLIST(Command* command, char buffer[], State* state, int arrayLength);
void executeLS(Command* command, char buffer[], State* state, int arrayLength);
void executeUSER(Command* command, char buffer[], State* state, int arrayLength);
void executePASS(Command* command, char buffer[], State* state, int arrayLength);
void executeSYST(Command* command, char buffer[], State* state, int arrayLength);
void executeQUIT(Command* command, char buffer[], State* state, int arrayLength);
void executePORT(Command* command, char buffer[], State* state, int arrayLength);
void bzeroCommand(Command* cmd);
