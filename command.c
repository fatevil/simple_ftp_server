#include <stdio.h>
#include <stdlib.h>
#include <string.h>       //bzero a dalsi
#include <dirent.h>       //dir
#include <fcntl.h>        //open
#include <sys/stat.h>     //mkdir
#include <sys/socket.h>   //hostent
#include <netdb.h>        //hostent
#include <unistd.h>       //access
#include <sys/sendfile.h> //sendfile

#include "connection.h"
#include "command.h"
#include "util.h"

void handleCommand(Command* command, char buffer[], State* state)
{
	bzero(buffer, BSIZE);
	if (strcmp(command->command, "MKD") == 0) {
		executeMKD(command, buffer, state);
	} else if (strcmp(command->command, "SYST") == 0) {
		executeSYST(command, buffer, state);
	} else if (strcmp(command->command, "USER") == 0) {
		executeUSER(command, buffer, state);
	} else if (strcmp(command->command, "PASS") == 0) {
		executePASS(command, buffer, state);
	} else if (strcmp(command->command, "CWD") == 0) {
		executeCD(command, buffer, state);
	} else if (strcmp(command->command, "PWD") == 0) {
		executeLIST(command, buffer, state);
	} else if (strcmp(command->command, "LIST") == 0) {
		executeLS(command, buffer, state);
	} else if (strcmp(command->command, "CDUP") == 0) {
		executeCDUP(command, buffer, state);
	} else if (strcmp(command->command, "QUIT") == 0) {
		executeQUIT(command, buffer, state);
	} else if (strcmp(command->command, "PORT") == 0) {
		executePORT(command, buffer, state);
	} else if (strcmp(command->command, "RETR") == 0) {
		executeRETR(command, buffer, state);
	} else if (strcmp(command->command, "TYPE") == 0) {
		executeTYPE(command, buffer, state);
	} else if (strcmp(command->command, "RMDIR") == 0) {
		executeRMDIR(command, buffer, state);
	} else if (strcmp(command->command, "") == 0) {
		executeNOOP(command, buffer, state);
	} else if (strcmp(command->command, "NOOP") == 0) {
		executeNOOP(command, buffer, state);
	} else {
		setResponseMessage("500 Wrong command!\n", buffer);
	}
}
void bzeroCommand(Command* cmd)
{
	bzero(cmd->arg, 100);
	bzero(cmd->command, 5);
}

void setResponseMessage(char message[], char buffer[])
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

void executeCDUP(Command* command, char buffer[], State* state)
{
	if (strcmp(state->pwd, BASEFOLDER) == 0) {
		setResponseMessage("415 You can't cdup main directory!\n", buffer);
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
				setResponseMessage(message, buffer);
				return;
			} else if (del == 1) {
				state->pwd[i] = '\0';
			}
		}
	}
}

void executeMKD(Command* command, char buffer[], State* state)
{

	char dir[200];
	strcpy(dir, state->pwd);
	strcat(dir, command->arg);
	if (mkdir(dir, S_IRWXU) == 0) {
		char message[] = "200 You have just created folder ";
		strcat(message, dir);
		strcat(message, "\n");
		setResponseMessage(message, buffer);
	} else {
		char message[] = "450 You haven't created anything!\n";
		setResponseMessage(message, buffer);
	}
}

void executeCD(Command* command, char buffer[], State* state)
{
	if (strlen(command->arg) == 0) {
		char message[] = "212 No change. Current folder: ";
		strcat(message, state->pwd);
		strcat(message, "\n");
		setResponseMessage(message, buffer);
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
		setResponseMessage(message, buffer);
	} else if (dir == NULL) {
		/* Directory does not exist. */
		char message[] = "212 It doesnt exist!\n";
		setResponseMessage(message, buffer);
	}
}

void executeLIST(Command* command, char buffer[], State* state)
{
	char message[] = "212 Current folder: ";
	strcat(message, state->pwd);
	strcat(message, "\n");
	setResponseMessage(message, buffer);
}

void executeLS(Command* command, char buffer[], State* state)
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

		setResponseMessage(message, buffer);
	} else {
		/* could not open directory */
		char message[] = "212 Couldn't use ls!\n";
		setResponseMessage(message, buffer);
	}
}

void executeUSER(Command* command, char buffer[], State* state)
{
	state->loggedIn = 1;
	char message[] = "331 Username ok. Now password. \n";
	setResponseMessage(message, buffer);
}

void executePASS(Command* command, char buffer[], State* state)
{
	state->loggedIn = 1;
	char message[] = "230 You're succesfuly logged in!\n";
	setResponseMessage(message, buffer);
}

void executeSYST(Command* command, char buffer[], State* state)
{
	char message[] = "215 UNIX Type: L8\n";
	setResponseMessage(message, buffer);
}

void executeQUIT(Command* command, char buffer[], State* state)
{
	state->keepConnection = 0;
	char message[] = "221 Good bye, I hope we will meet again! \n";
	setResponseMessage(message, buffer);
}

void executePORT(Command* command, char buffer[], State* state)
{

	ServerAdresse* serverAdresse = malloc(sizeof(ServerAdresse));
	resolvePortArgument(command->arg, serverAdresse);
	if (serverAdresse->port <= 0 || serverAdresse->addresse == NULL) {
		char message[] = "230 Couldn't read PORT arguments! \n";
		setResponseMessage(message, buffer);
		return;
	}
	printf("Resolved IP: %s & PORT: %d \n", serverAdresse->addresse, serverAdresse->port);

	struct hostent* server = gethostbyname(serverAdresse->addresse);
	if (server == NULL) {
		char message[] = "425 Couldnt connect to your listener! \n";
		setResponseMessage(message, buffer);
	}

	int socket;

	if ((socket = startActiveModeDataConnection(server, serverAdresse->port)) > 0) {
		char message[] = "230 Succesfully established data connection! \n";
		setResponseMessage(message, buffer);
	} else {
		char message[] = "425 Couldnt connect to your listener! \n";
		setResponseMessage(message, buffer);
	}

	state->socket = socket;

	free(serverAdresse->addresse);
	free(serverAdresse);
}

void executeRETR(Command* command, char buffer[], State* state)
{
	struct stat statBuffer;
	int fd;
	off_t offset = 0;

	char fileName[100];
	strcpy(fileName, state->pwd);
	strcat(fileName, command->arg);

	if (access(fileName, R_OK) == 0 && (fd = open(fileName, O_RDONLY))) {
		fstat(fd, &statBuffer);
	} else {
		setResponseMessage("450 File can't be accessed! \n", buffer);
		return;
	}
	printf("gonna send %d \n", (int)statBuffer.st_size);
	size_t sizeToSend;
	for (sizeToSend = statBuffer.st_size; sizeToSend > 0;) {
		ssize_t sent = sendfile(state->socket, fd, &offset, (int)sizeToSend);
		if (sent <= 0) {
			setResponseMessage("450 Didnt work out! \n", buffer);
			return;
		}

		offset += sent;
		sizeToSend -= sent;
		printf("sent out %d \n", (int)sent);
	}

	close(state->socket);
	close(fd);

	setResponseMessage("226 File send OK.\n", buffer);
}

void executeTYPE(Command* command, char buffer[], State* state)
{
	if (strcmp(command->arg, "I") == 0) {
		state->type = BINARY;
		setResponseMessage("200 Type is set to BINARY! \n", buffer);
	} else if (strcmp(command->arg, "A") == 0) {
		state->type = ASCII;
		setResponseMessage("200 Type is set ASCII! \n", buffer);
	} else {
		setResponseMessage("500 This argument is not supported! \n", buffer);
	}
}

void executeRMDIR(Command* command, char buffer[], State* state)
{
	char dirName[100];
	strcpy(dirName, state->pwd);
	strcat(dirName, command->arg);

	if (-1 == rmdir(dirName)) {
		setResponseMessage("450 The directory is not empty! \n", buffer);
	} else {
		setResponseMessage("200 Directory succesfully removed! \n", buffer);
	}
}

void executeNOOP(Command* command, char buffer[], State* state)
{
	setResponseMessage("200 Nothing to be done.\n", buffer);
}