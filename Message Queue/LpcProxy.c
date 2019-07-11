#include "Lpc.h"
#include "LpcProxy.h"
#include <string.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void Init(void)
{
}


int OpenFile(char* path, int flags)
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	
	request.pid=getpid();
	request.service=LPC_OPEN_FILE;
	request.numArg=2;

	LpcArg PATH, MODE;
	memset(PATH.argData,0,LPC_DATA_MAX); memset(MODE.argData,0,LPC_DATA_MAX);
	strcpy(PATH.argData,path);
	PATH.argSize=strlen(path);
	
	char cpMode[10];
	sprintf(cpMode,"%d",flags);
	strcpy(MODE.argData,cpMode);
	MODE.argSize=strlen(cpMode);

	request.lpcArgs[0]=PATH;
	request.lpcArgs[1]=MODE;
	msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);

	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	return response.errorno;
}

int ReadFile(int fd, void* pBuf, int size)
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	
	request.pid=getpid();
	request.service=LPC_READ_FILE;
	request.numArg=3;

	LpcArg FD, PBUF, SIZE;
	memset(FD.argData,0,LPC_DATA_MAX);memset(PBUF.argData,0,LPC_DATA_MAX);memset(SIZE.argData,0,LPC_DATA_MAX);

	char cpFd[10];
	sprintf(cpFd,"%d",fd);
	FD.argSize=strlen(cpFd);
	strcpy(FD.argData,cpFd);
	
	PBUF.argSize=0;

	char cpSize[10];
	sprintf(cpSize,"%d",size);
	SIZE.argSize=strlen(cpSize);
	strcpy(SIZE.argData,cpSize);

	request.lpcArgs[0]=FD;
	request.lpcArgs[1]=PBUF;
	request.lpcArgs[2]=SIZE;
	msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);
	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	if(result>0){
		strcpy((char*)pBuf,response.responseData);;
	}
	return response.errorno;
}


int WriteFile(int fd, void* pBuf, int size)
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	
	request.pid=getpid();

	request.service=LPC_WRITE_FILE;
	request.numArg=3;

	LpcArg FD, PBUF, SIZE;
	memset(FD.argData,0,LPC_DATA_MAX);memset(PBUF.argData,0,LPC_DATA_MAX);memset(SIZE.argData,0,LPC_DATA_MAX);

	char cpFd[10];
	sprintf(cpFd,"%d",fd);
	FD.argSize=strlen(cpFd);
	strcpy(FD.argData,cpFd);

	strcpy(PBUF.argData,(char*)pBuf);
	PBUF.argSize=strlen((char*)pBuf);
	
	char cpSize[10];
	sprintf(cpSize,"%d",size);
	SIZE.argSize=strlen(cpSize);
	strcpy(SIZE.argData,cpSize);

	request.lpcArgs[0]=FD;
	request.lpcArgs[1]=PBUF;
	request.lpcArgs[2]=SIZE;
	msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);
	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	return response.errorno;
}


int CloseFile(int fd)
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	
	request.pid=getpid();
	request.service=LPC_CLOSE_FILE;
	request.numArg=1;

	LpcArg FD;

	char cpFd[10];
	sprintf(cpFd,"%d",fd);
	FD.argSize=strlen(cpFd);
	strcpy(FD.argData,cpFd);

	request.lpcArgs[0]=FD;
	msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);
	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	return response.errorno;
}

int MakeDirectory(char* path, int mode)
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	request.pid=getpid();
	request.service=LPC_MAKE_DIRECTORY;
	request.numArg=2;

	LpcArg PATH, MODE;

	PATH.argSize=strlen(path);
	strcpy(PATH.argData,path);

	char cpMode[10];
	sprintf(cpMode,"%d",mode);
	MODE.argSize=strlen(cpMode);
	strcpy(MODE.argData,cpMode);

	request.lpcArgs[0]=PATH;
	request.lpcArgs[1]=MODE;
	int re=msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);
	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	return response.errorno;	
}


int GetString(void* (*callback)(char*))
{
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);
	request.pid=getpid();
	request.service=LPC_GET_STRING;
	request.numArg=0;

	msgsnd(msgid,(void*)&request,2048,IPC_NOWAIT);
	int result=-1000;
	while(result==-1000){
		result=msgrcv(msgid2,&response,2048,0,0);
	}
	callback(response.responseData);
	return response.errorno;
}
