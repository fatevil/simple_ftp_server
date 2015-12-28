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

void checkArguments(int argc, char* argv[])
{
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
}

void error(const char* msg)
{
	perror(msg);
	exit(1);
}