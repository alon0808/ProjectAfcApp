#include "Macro_Proj.h"
#include "debugInfor.h"

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>





void debugstring(const char *str)
{
	printf("%s", str);
}

void debugdata(unsigned char *value, unsigned int uclen, unsigned char mode)
{
	unsigned int i;

	if (uclen > 5000) {
		printf("%d.", uclen);
		uclen = 5000;
	}

	for (i = 0; i < uclen; i++)
	{
		printf("%02X", value[i]);
	}

	if ((mode & 0x01) == 1)
		printf("\r\n");
}

