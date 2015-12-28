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

void handleCommand(Command* command, char* string)
{
	strcpy(string, "bobek");
}
void parseCommand(char* cmdstring, Command* cmd)
{
	sscanf(cmdstring, "%s %s", cmd->command, cmd->arg);
}
