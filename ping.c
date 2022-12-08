#include "ping.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

int ping_server(char *ip, char* inet, int waittime, int count)
{
	char cmd[128] = {0};
	int ret = 1;
	sprintf(cmd, "ping %s -w %d -c %d -I %s", ip, waittime, count, inet);
	ret = !!system(cmd);
	return ret;
}
