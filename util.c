#include <stdio.h>
#include <stdlib.h> //exit
#include "connection.h"
#include "command.h"
#include "util.h"

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

void resolvePortArgument(char portArgument[], ServerAdresse* serverAdresse)
{
	char* hostentString = malloc(20 * sizeof(char));
	char p1[10];
	char p2[10];
	int numberOfComma = 0;

	int argSize = strlen(portArgument);
	int i, o, p;
	for (i = 0; i < argSize; i++) {
		if (portArgument[i] == ',') {
			numberOfComma++;
		}

		if (portArgument[i] == ',' && numberOfComma < 4) {
			portArgument[i] = '.';
		} else if (portArgument[i] == ',' && numberOfComma == 4) {
			strncpy(hostentString, portArgument, i);
			hostentString[i] = '\0';
			o = i;
		} else if (numberOfComma == 4) {
			p1[i - o - 1] = portArgument[i];
			p1[i - o] = '\0';
			p = i;
		} else if (numberOfComma == 5 && portArgument[i] != ',') {
			p2[i - p - 2] = portArgument[i];
			p2[i - p - 1] = '\0';
		}
	}
	int port = 0;
	port = atoi(p1);
	port *= 256;
	port += atoi(p2);

	serverAdresse->addresse = hostentString;
	serverAdresse->port = port;
}