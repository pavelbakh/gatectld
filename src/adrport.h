//============================================================================
// Name        : adrport.h
// Author      : Pavel Bakhmetiev
// Version     : 1.0
// Copyright   : Sportplex 2010 Copyright
// Description : GATE control program in C, Ansi-style
//============================================================================

int OpenAdrPort (char* sPortNumber);
int WriteAdrPort(char* psOutput);
int ReadAdrPort(char* psResponse, int iMax);
void CloseAdrPort();
