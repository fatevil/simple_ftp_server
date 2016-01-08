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