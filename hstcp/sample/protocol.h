#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	int  nLeng; //package body length
}CLIENT_PACK_HEAD;

typedef struct
{
	int  nRandomNum;
}CLIENT_PACK_BODY;

typedef struct
{
	int nLeng; //package body length
}SERVER_PACK_HEAD;
typedef struct
{
	int nAckNum;
}SERVER_PACK_BODY;
#endif
