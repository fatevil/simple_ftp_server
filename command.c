#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <dirent.h>
#include "connection.h"
#include "command.h"
#include "util.h"

void handleCommand(Command* command, char buffer[], State* state, int arrayLength)
{

	/*not sure, jestli pak bude fungovat CDUP - chtel jsem jen demonstrovat, jak pristupovat k prikazum a vracet
	 * zpravy... da se to dal rozhazet do metod a treba pouzit USECASE*/
	if (strcmp(command->command, "mkd") == 0) {
		executeMKD(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "cd") == 0) {
		executeCD(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "pwd") == 0) {
		executePWD(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "ls") == 0) {
		executeLS(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "cdup") == 0) {
		executeCDUP(command, buffer, state, arrayLength);
	} else {
		setResponse(300, "Wrong command!", buffer);
	}
}

/*CODE DOESNT WORK PROPERLY YET*/
void setResponse(int code, char message[], char buffer[])
{
	strcpy(buffer, message);
	// memcpy(buffer, (char*)&code, sizeof(int));
}

/*READS ONLY TWO STRINGS, third string is ignored*/
void parseCommand(char* cmdstring, Command* command)
{
	bzero(command->arg, 100);
	bzero(command->command, 5);
	sscanf(cmdstring, "%s %s", command->command, command->arg);
}

void executeCDUP(Command* command, char buffer[], State* state, int arrayLength)
{
	if (strcmp(state->pwd, BASEFOLDER) == 0) {
		setResponse(100, "You can't cdup main directory!", buffer);
	} else {
		int i;
		int del = 0;
		/*SIZE OF PWD = 100*/
		for (i = strlen(state->pwd); i >= 0; i--) {
			char currentChar = state->pwd[i];
			printf("current char: %c \n", currentChar);
			if (currentChar == '/' && del == 0) {
				state->pwd[i] = '\0';
				del = 1;
				printf("deleted\n");
			} else if (currentChar == '/' && del == 1) {
				char message[] = "Current folder: ";
				strcat(message, state->pwd);
				strcat(message, "\n");
				setResponse(100, message, buffer);
				return;
			} else if (del == 1) {
				state->pwd[i] = '\0';
				printf("deleted\n");
			}
		}
	}
}

void executeMKD(Command* command, char buffer[], State* state, int arrayLength)
{

	char dir[200];
	strcpy(dir, state->pwd);
	strcat(dir, command->arg);
	if (mkdir(dir, S_IRWXU) == 0) {
		char message[] = "You have just created folder ";
		strcat(message, dir);
		strcat(message, "\n");
		setResponse(100, message, buffer);
	} else {
		char message[] = "You haven't created anything!\n";
		setResponse(100, message, buffer);
	}
}

void executeCD(Command* command, char buffer[], State* state, int arrayLength)
{
	if (strlen(command->arg) == 0) {
		char message[] = "No change. Current folder: ";
		strcat(message, state->pwd);
		strcat(message, "\n");
		setResponse(100, message, buffer);
		return;
	}

	char dirname[200];
	strcpy(dirname, state->pwd);
	strcat(dirname, command->arg);

	DIR* dir = opendir(dirname);
	if (dir) {
		/* Directory exists. */
		closedir(dir);
		strcat(state->pwd, command->arg);
		strcat(state->pwd, "/");
		char message[] = "Current folder: ";
		strcat(message, state->pwd);
		strcat(message, "\n");
		setResponse(100, message, buffer);
	} else if (dir == NULL) {
		/* Directory does not exist. */
		char message[] = "It doesnt exist!\n";
		setResponse(100, message, buffer);
	}
}

void executePWD(Command* command, char buffer[], State* state, int arrayLength)
{
	char message[] = "Current folder: ";
	strcat(message, state->pwd);
	strcat(message, "\n");
	setResponse(100, message, buffer);
}

void executeLS(Command* command, char buffer[], State* state, int arrayLength)
{
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(state->pwd)) != NULL) {
		char message[256] = "";
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				strcat(message, ent->d_name);
				strcat(message, " ");
			}
		}
		closedir(dir);

		setResponse(100, message, buffer);
	} else {
		/* could not open directory */
		char message[] = "Couldn't use ls!";
		setResponse(300, message, buffer);
	}
}