// adrport.h
// Copyright SPORTPLEX, SPORTPLEX by Sportplex Incorporated

int OpenAdrPort (char* sPortNumber);
int WriteAdrPort(char* psOutput);
int ReadAdrPort(char* psResponse, int iMax);
void CloseAdrPort();
