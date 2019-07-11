#include "Lpc.h"
#include "LpcStub.h"
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>


pthread_mutex_t mutex_lock;

void Init(void)
{
}

int OpenFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	
	int pid = pRequest->pid;

	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = NULL;
	response = (LpcResponse*)shared2; 
	

	char* path=pRequest->lpcArgs[0].argData;
	int mode=atoi(pRequest->lpcArgs[1].argData);
	int fd=open(path,mode,0644);	
	
	int size = 12;

	response->pid= pid;
	response->errorno=fd;
	response->responseSize=size;

	/////
	/*
	int shmid=shmget((key_t)1234, 2048, IPC_CREAT|0644);
	printf("shmid: %d\n",shmid);

	LpcRequest* request = NULL;
	void* shared = (void*)0;

	shared=shmat(shmid, (void*)0, 0);
	request=(LpcRequest*)shared;

	request->pid=-1;
	request->service=0;
	request->numArg =0;
	*/
	////
	pthread_mutex_unlock(&mutex_lock);
	return fd;
}

int ReadFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	
	int pid = pRequest->pid;

	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = NULL;
	response = (LpcResponse*)shared2; 

	int fd=atoi(pRequest->lpcArgs[0].argData);
	int size=atoi(pRequest->lpcArgs[2].argData);
	char* reader=(char*)malloc(size);

	int nread=read(fd,reader,size);
	response->pid=pRequest->pid;
	response->errorno=nread;
	strcpy(response->responseData,reader);
	response->responseSize=12+nread;

	pthread_mutex_unlock(&mutex_lock);
	return nread;
}

int WriteFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	
	int pid = pRequest->pid;

	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = NULL;
	response = (LpcResponse*)shared2; 

	int fd=atoi(pRequest->lpcArgs[0].argData);
	char* pBuf=pRequest->lpcArgs[1].argData;
	int size=atoi(pRequest->lpcArgs[2].argData);

	int nwrite=write(fd,pBuf,size);

	response->pid=pRequest->pid;
	response->errorno=nwrite;
	response->responseSize=12;
	pthread_mutex_unlock(&mutex_lock);	
	return nwrite;
}


int CloseFile(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	
	int pid = pRequest->pid;

	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = NULL;
	response = (LpcResponse*)shared2; 

	int fd=atoi(pRequest->lpcArgs[0].argData);

	int nclose=close(fd);

	response->pid=pRequest->pid;
	response->errorno=nclose;
	response->responseSize=12;
	pthread_mutex_unlock(&mutex_lock);
	return nclose;
}

int MakeDirectory(LpcRequest* pRequest)
{
	pthread_mutex_lock(&mutex_lock);
	
	int pid = pRequest->pid;

	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = NULL;
	response = (LpcResponse*)shared2; 

	char* path=pRequest->lpcArgs[0].argData;
	int mode=atoi(pRequest->lpcArgs[1].argData);

	int MKDIR=mkdir(path,mode);

	response->pid=pRequest->pid;
	response->errorno=MKDIR;
	response->responseSize=12;

	pthread_mutex_unlock(&mutex_lock);
	return MKDIR;
}

int GetString(LpcRequest* pRequest){
	/*
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
	*/
	return 1;
}