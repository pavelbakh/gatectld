// gatectld.c - Serial Port Test Example
// Copyright SPORTPLEX, SPORTPLEX by Sportplex Incorporated

// WARNING: Example only. Lacks error checking!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "adrport.h"

// this is the mainline thingee
int main(int argc, char *argv[])
{
	char sCmd[254];
	char sResult[254];
	if (argc < 2 || argc > 2)
	{
		printf("adrserial needs 1 parameter for the serial port\n");
		printf("  ie. use 'adrserial 0' to connect to /dev/ttyS0\n");
		return 0;
	} // end if
	printf("Type q to quit.\n\n");

	if (OpenAdrPort(argv[1]) < 0) return 0;

	while (1)
	{
		int iSpot;

		printf("?:");
		gets(sCmd);

		if (sCmd[0] == 'q' || sCmd[0] == 'Q') return 0;

		iSpot = strlen(sCmd);
		sCmd[iSpot] = 0x0d; // stick a <CR> after the command
		sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
		sCmd[iSpot+2] = 0x00; // terminate the string properly

		if (WriteAdrPort(sCmd) < 0) return 0;
		sleep(1); // give the ADR card some time to respond

		if (ReadAdrPort(sResult,254) > 0)
		{
			printf("****Response is %s\n", sResult);
		} // end if
	} // end while

	CloseAdrPort();

} // end main
