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

void handleCommand(Command* command, char buffer[], State* state, int arrayLength)
{

	/*not sure, jestli pak bude fungovat CDUP - chtel jsem jen demonstrovat, jak pristupovat k prikazum a vracet
	 * zpravy... da se to dal rozhazet do metod a treba pouzit USECASE*/
	if (strcmp(command->command, "cdup") == 0) {
		if (strcmp(state->pwd, "./") == 0) {
			setResponse(100, "You can't cdup main directory!", buffer);
		} else {
			int i;
			int del = 0;
			/*SIZE OF PWD = 100*/
			for (i = state->pwdLength; i >= 0; i--) {
				char currentChar = state->pwd[i];
				if (currentChar == '/' && del == 0) {
					state->pwd[i] = '\0';
					del = 1;
				} else if (currentChar == '/' && del == 1) {
					char message[] = "Current folder: ";
					strcat(message, state->pwd);
					strcat(message, "\n");
					setResponse(100, message, buffer);
				}
			}
		}
	} else {
		setResponse(300, "Wrong command!", buffer);
	}
}

/*CODE DOESNT WORK YET*/
void setResponse(int code, char message[], char buffer[])
{
	strcpy(buffer, message);
	// memcpy(buffer, (char*)&code, sizeof(int));
}

void parseCommand(char* cmdstring, Command* cmd)
{
	sscanf(cmdstring, "%s %s", cmd->command, cmd->arg);
}
