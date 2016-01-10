#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //bzero a dalsi
#include <dirent.h>     //dir
#include <fcntl.h>      //open
#include <sys/stat.h>   //mkdir
#include <sys/socket.h> //hostent
#include <netdb.h>      //hostent
#include "connection.h"
#include "command.h"
#include "util.h"

void handleCommand(Command* command, char buffer[], State* state, int arrayLength)
{
	bzero(buffer, BSIZE);
	if (strcmp(command->command, "MKD") == 0) {
		executeMKD(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "SYST") == 0) {
		executeSYST(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "USER") == 0) {
		executeUSER(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "PASS") == 0) {
		executePASS(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "CWD") == 0) {
		executeCD(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "PWD") == 0) {
		executeLIST(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "LIST") == 0) {
		executeLS(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "CDUP") == 0) {
		executeCDUP(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "QUIT") == 0) {
		executeQUIT(command, buffer, state, arrayLength);
	} else if (strcmp(command->command, "PORT") == 0) {
		executePORT(command, buffer, state, arrayLength);
	} else {
		setResponseMessage(300, "500 Wrong command!\n", buffer);
	}
}
void bzeroCommand(Command* cmd)
{
	bzero(cmd->arg, 100);
	bzero(cmd->command, 5);
}

/*CODE DOESNT WORK PROPERLY YET*/
void setResponseMessage(int code, char message[], char buffer[])
{
	strcpy(buffer, message);
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
		setResponseMessage(100, "415 You can't cdup main directory!\n", buffer);
	} else {
		int i;
		int del = 0;
		/*SIZE OF PWD = 100*/
		for (i = strlen(state->pwd); i >= 0; i--) {
			char currentChar = state->pwd[i];
			if (currentChar == '/' && del == 0) {
				state->pwd[i] = '\0';
				del = 1;
			} else if (currentChar == '/' && del == 1) {
				char message[] = "212 Current folder: ";
				strcat(message, state->pwd);
				strcat(message, "\n");
				setResponseMessage(100, message, buffer);
				return;
			} else if (del == 1) {
				state->pwd[i] = '\0';
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
		char message[] = "200 You have just created folder ";
		strcat(message, dir);
		strcat(message, "\n");
		setResponseMessage(100, message, buffer);
	} else {
		char message[] = "450 You haven't created anything!\n";
		setResponseMessage(100, message, buffer);
	}
}

void executeCD(Command* command, char buffer[], State* state, int arrayLength)
{
	if (strlen(command->arg) == 0) {
		char message[] = "212 No change. Current folder: ";
		strcat(message, state->pwd);
		strcat(message, "\n");
		setResponseMessage(100, message, buffer);
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
		char message[] = "212 Current folder: ";
		strcat(message, state->pwd);
		strcat(message, "\n");
		setResponseMessage(100, message, buffer);
	} else if (dir == NULL) {
		/* Directory does not exist. */
		char message[] = "212 It doesnt exist!\n";
		setResponseMessage(100, message, buffer);
	}
}

void executeLIST(Command* command, char buffer[], State* state, int arrayLength)
{
	char message[] = "212 Current folder: ";
	strcat(message, state->pwd);
	strcat(message, "\n");
	setResponseMessage(100, message, buffer);
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

		setResponseMessage(100, message, buffer);
	} else {
		/* could not open directory */
		char message[] = "212 Couldn't use ls!\n";
		setResponseMessage(300, message, buffer);
	}
}

void executeUSER(Command* command, char buffer[], State* state, int arrayLength)
{
	state->loggedIn = 1;
	char message[] = "331 Username ok. Now password. \n";
	setResponseMessage(331, message, buffer);
}

void executePASS(Command* command, char buffer[], State* state, int arrayLength)
{
	state->loggedIn = 1;
	char message[] = "230 You're succesfuly logged in!\n";
	setResponseMessage(230, message, buffer);
}

void executeSYST(Command* command, char buffer[], State* state, int arrayLength)
{
	char message[] = "215 UNIX Type: L8\n";
	setResponseMessage(230, message, buffer);
}

void executeQUIT(Command* command, char buffer[], State* state, int arrayLength)
{
	state->keepConnection = 0;
	char message[] = "221 Good bye, I hope we will meet again! \n";
	setResponseMessage(230, message, buffer);
}

void executePORT(Command* command, char buffer[], State* state, int arrayLength)
{

	ServerAdresse* serverAdresse = malloc(sizeof(ServerAdresse));
	resolvePortArgument(command->arg, serverAdresse);
	if (serverAdresse->port <= 0 || serverAdresse->addresse == NULL) {
		char message[] = "230 Couldn't read PORT arguments! \n";
		setResponseMessage(230, message, buffer);
		return;
	}
	printf("Resolved IP: %s & PORT: %d \n", serverAdresse->addresse, serverAdresse->port);

	struct hostent* server = gethostbyname(serverAdresse->addresse);
	if (server == NULL) {
		char message[] = "425 Couldnt connect to your listener! \n";
		setResponseMessage(230, message, buffer);
	}
	free(serverAdresse->addresse);

	if (startActiveModeDataConnection(server, serverAdresse->port) > 0) {
		char message[] = "230 Succesfully established data connection! \n";
		setResponseMessage(230, message, buffer);
	} else {
		char message[] = "425 Couldnt connect to your listener! \n";
		setResponseMessage(230, message, buffer);
	}
}
