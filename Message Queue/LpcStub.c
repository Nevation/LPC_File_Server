#include "Lpc.h"
#include "LpcStub.h"
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutex_lock;

void Init(void)
{
}

int OpenFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	char* path=pRequest->lpcArgs[0].argData;
	int mode=atoi(pRequest->lpcArgs[1].argData);
	int fd=open(path,mode,0644);	
	response.pid=pRequest->pid;
	response.errorno=fd;
	response.responseSize=12;
	msgsnd(msgid2,&response,12,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);
	return fd;
}

int ReadFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	int fd=atoi(pRequest->lpcArgs[0].argData);
	int size=atoi(pRequest->lpcArgs[2].argData);
	char* reader=(char*)malloc(size);

	int nread=read(fd,reader,size);
	response.pid=pRequest->pid;
	response.errorno=nread;
	strcpy(response.responseData,reader);
	response.responseSize=12+nread;
	msgsnd(msgid2,&response,response.responseSize,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);
	return nread;
}

int WriteFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	int fd=atoi(pRequest->lpcArgs[0].argData);
	char* pBuf=pRequest->lpcArgs[1].argData;
	int size=atoi(pRequest->lpcArgs[2].argData);

	int nwrite=write(fd,pBuf,size);

	response.pid=pRequest->pid;
	response.errorno=nwrite;
	response.responseSize=12;
	msgsnd(msgid2,&response,12,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);	
	return nwrite;
}


int CloseFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	int fd=atoi(pRequest->lpcArgs[0].argData);

	int nclose=close(fd);

	response.pid=pRequest->pid;
	response.errorno=nclose;
	response.responseSize=12;
	msgsnd(msgid2,&response,12,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);
	return nclose;
}

int MakeDirectory(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	char* path=pRequest->lpcArgs[0].argData;
	int mode=atoi(pRequest->lpcArgs[1].argData);

	int MKDIR=mkdir(path,mode);

	response.pid=pRequest->pid;
	response.errorno=MKDIR;
	response.responseSize=12;
	msgsnd(msgid2,&response,12,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);
	return MKDIR;
}

int GetString(LpcRequest* pRequest){
	pthread_mutex_lock(&mutex_lock);

	LpcResponse response;
	int msgid2=msgget((key_t)4321,IPC_CREAT|0644);

	char string[512];
	memset(string,0,512);
	printf("Input the text:");
	fgets(string,512,stdin);
	string[strlen(string)-1]=0;

	response.pid=pRequest->pid;
	response.errorno=1;
	strcpy(response.responseData,string);
	response.responseSize=strlen(string)+12;
	msgsnd(msgid2,&response,response.responseSize,IPC_NOWAIT);
	pthread_mutex_unlock(&mutex_lock);
	return 1;
}