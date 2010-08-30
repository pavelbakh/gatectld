//============================================================================
// Name        : gatectld.c
// Author      : Pavel Bakhmetiev
// Version     : 1.0
// Copyright   : Sportplex 2010 Copyright
// Description : GATE control program in C, Ansi-style
//============================================================================

// WARNING: Example only. Lacks error checking!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "adrport.h"

int init_gatectl(int nCom)
{
//		char sCom[3];
		char sCmd[254];
		char sResult[254];
		char sMess[254];
		int iSpot;
	// Open port for control devices
//		sprintf(sCom, "%d", nCom);
//		if (OpenAdrPort(sCom) < 0)
//		{
//			log_message(LOGFILE, "ERROR open com port for control devices");
//			return -1;
//		}

		strcpy(sCmd, "$KE,RST");
		iSpot = strlen(sCmd);
		sCmd[iSpot] = 0x0d; // stick a <CR> after the command
		sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
		sCmd[iSpot+2] = 0x00; // terminate the string properly
		if (WriteAdrPort(sCmd) < 0)
		{
//			log_message(LOGFILE, "ERROR write command to com port");
			return -1;
		}
		sleep(1); // give the ADR card some time to respond
		if (ReadAdrPort(sResult,254) > 0)
		{
			sprintf(sMess, "****Response is %s\n", sResult);
//			log_message(LOGFILE, sMess);
		} // end if

//		strcpy(sCmd, "$KE,WRA,111111111111111111111111");
		strcpy(sCmd, "$KE,WRA,000000000111100000000000");
		iSpot = strlen(sCmd);
		sCmd[iSpot] = 0x0d; // stick a <CR> after the command
		sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
		sCmd[iSpot+2] = 0x00; // terminate the string properly
		if (WriteAdrPort(sCmd) < 0)
		{
//			log_message(LOGFILE, "ERROR write command to com port");
			return -1;
		}

		return 0;
}

int open_gate(int nGate)
{
	char sCmd[254];
	int iSpot;

//	printf("Begin open gate...");
	sprintf(sCmd, "$KE,WR,%u,1", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

	sprintf(sCmd, "$KE,WR,%u,0", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

	sleep(1); // pause

	sprintf(sCmd, "$KE,WR,%u,1", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

//	printf("End open gate...\n");

	return 0;
}

int close_gatectl()
{
	char sCmd[254];
	int iSpot;

	strcpy(sCmd, "$KE,WRA,000000000000000000000000");
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}
	// Close com port
		CloseAdrPort();

		return 0;
}

// this is the mainline thingee
int main(int argc, char *argv[])
{
//	char sCmd[254];
//	char sResult[254];
	int nret = 0;
	if (argc < 2 || argc > 4)
	{
		printf("adrserial needs 3 parameters for the serial port\n");
		printf("  ie. use 'gatectld port command pin' 'gatectld 0' to connect to /dev/ttyS0\n");
		return 0;
	} // end if
//	printf("Type q to quit.\n\n");

	if (OpenAdrPort(argv[1]) < 0) return 0;

	if(argc == 2) return init_gatectl(atoi(argv[1]));

//	strcpy(sCmd, argv[2]);

//	printf("command is: %s \n", argv[2]);

	if(strcmp(argv[2], "init") == 0)
		nret = init_gatectl(atoi(argv[1]));
	else if(strcmp(argv[2], "open") == 0)
		nret = open_gate(atoi(argv[3]));
	else if(strcmp(argv[2], "exit") == 0)
		nret = close_gatectl();

	CloseAdrPort();

	return nret;

/*
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
*/


} // end main
