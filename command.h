#include <stdio.h>
#include <string.h>

typedef struct Command
{
	char command[10];
	char arg[100];
} Command;

void parseCommand(char* cmdstring, Command* cmd);
void handleCommand(Command* command, char* string, State* state);
void setResponseMessage(char message[], char buffer[]);
void executeCD(Command* command, char buffer[], State* state);
void executeCDUP(Command* command, char buffer[], State* state);
void executeMKD(Command* command, char buffer[], State* state);
void executeLIST(Command* command, char buffer[], State* state);
void executeLS(Command* command, char buffer[], State* state);
void executeUSER(Command* command, char buffer[], State* state);
void executePASS(Command* command, char buffer[], State* state);
void executeSYST(Command* command, char buffer[], State* state);
void executeQUIT(Command* command, char buffer[], State* state);
void executePORT(Command* command, char buffer[], State* state);
void executeRETR(Command* command, char buffer[], State* state);
void executeTYPE(Command* command, char buffer[], State* state);
void executeRMDIR(Command* command, char buffer[], State* state);
void executeNOOP(Command* command, char buffer[], State* state);
void executeSIZE(Command* command, char buffer[], State* state);
void bzeroCommand(Command* cmd);
