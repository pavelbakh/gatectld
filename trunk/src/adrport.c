/// adrport.c - Serial Port Handler
// Copyright SPORTPLEX, SPORTPLEX by Sportplex Incorporated

// Permission is hereby granted to freely copy,
// modify, utilize and distribute this example in
// whatever manner you desire without restriction.

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "adrport.h"
static int fd = 0;

// opens the serial port
// return code:
//   > 0 = fd for the port
//   -1 = open failed
int OpenAdrPort(char* sPortNumber)
{
    char sPortName[64];
    printf("in OpenAdrPort port#=%s\n", sPortNumber);
    sprintf(sPortName, "/dev/ttyACM%s", sPortNumber);
    printf("sPortName=%s\n", sPortName);

    // make sure port is closed
    CloseAdrPort(fd);

    fd = open(sPortName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        printf("open error %d %s\n", errno, strerror(errno));
    }
    else
    {
        struct termios my_termios;
        printf("fd is %d\n", fd);
        tcgetattr(fd, &my_termios);
        // NOTE: you may want to save the port attributes
        //       here so that you can restore them later
        printf("old cflag=%08x\n", my_termios.c_cflag);
        printf("old oflag=%08x\n", my_termios.c_oflag);
        printf("old iflag=%08x\n", my_termios.c_iflag);
        printf("old lflag=%08x\n", my_termios.c_lflag);
        printf("old line=%02x\n", my_termios.c_line);

        tcflush(fd, TCIFLUSH);

        my_termios.c_cflag = B9600 | CS8 |CREAD | CLOCAL | HUPCL;

        cfsetospeed(&my_termios, B9600);
        tcsetattr(fd, TCSANOW, &my_termios);

        printf("new cflag=%08x\n", my_termios.c_cflag);
        printf("new oflag=%08x\n", my_termios.c_oflag);
        printf("new iflag=%08x\n", my_termios.c_iflag);
        printf("new lflag=%08x\n", my_termios.c_lflag);
        printf("new line=%02x\n", my_termios.c_line);
    } // end if
    return fd;
} // end OpenAdrPort

// writes zero terminated string to the serial port
// return code:
//   >= 0 = number of characters written
//   -1 = write failed
int WriteAdrPort(char* psOutput)
{
    int iOut;
    if (fd < 1)
    {
        printf(" port is not open\n");
        return -1;
    } // end if
    iOut = write(fd, psOutput, strlen(psOutput));
    if (iOut < 0)
    {
        printf("write error %d %s\n", errno, strerror(errno));
    }
    else
    {
    	printf("wrote %d chars: %s\n", iOut, psOutput);
    } // end if
    return iOut;
} // end WriteAdrPort

// read string from the serial port
// return code:
//   >= 0 = number of characters read
//   -1 = read failed
int ReadAdrPort(char* psResponse, int iMax)
{
    int iIn;
    printf("in ReadAdrPort iMax=%d\n", iMax);

    if (fd < 1)
    {
        printf(" port is not open\n");
        return -1;
    } // end if

    strncpy (psResponse, "N/A", iMax<4?iMax:4);
    printf("psResponse: %s\n", psResponse);

    iIn = read(fd, psResponse, iMax-1);
    printf("read %d chars: %s\n", iIn, psResponse);
    if (iIn < 0)
    {
    	if (errno == EAGAIN)
    	{
    		return 0; // assume that command generated no response
		}
		else
		{
			printf("read error %d %s\n", errno, strerror(errno));
		} // end if
    }
    else
    {
    	psResponse[iIn<iMax?iIn:iMax] = '\0';
	    printf("read %d chars: %s\n", iIn, psResponse);
    } // end if

    return iIn;
} // end ReadAdrPort

// closes the serial port
void CloseAdrPort()
{
	// you may want to restore the saved port attributes
    if (fd > 0)
    {
        close(fd);
    } // end if
} // end CloseAdrPort
