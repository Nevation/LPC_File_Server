#include "Lpc.h"
#include "LpcProxy.h"
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

const int PORT[6] = {123, 1234, 12345, 54321, 4321, 321};
int PID[6];

void Init(void)
{
	const int ROOT_PID=getpid();
	for(int i=0;i<6;i++){
		PID[i]=ROOT_PID+i;
	}
}

int getKEY(int pid){
	for (int i=0; i<6; i++){
		if (PID[i]==pid){
			return PORT[i];
		}
	}
}

int OpenFile(char* path, int flags)
{
	int pid = getpid();

	// set shm
	int KEY = getKEY(pid);
	int shmid=shmget((key_t)KEY, 2048, IPC_CREAT|0644);
	void* shared = (void*)0;
	shared=shmat(shmid, (void*)0, 0);
	
	//client to server
	LpcRequest* request = (LpcRequest*)shared;
	
	request->pid=pid;
	request->service=LPC_OPEN_FILE;
	request->numArg=2;
	
	LpcArg PATH, MODE;
	memset(PATH.argData,0,LPC_DATA_MAX); memset(MODE.argData,0,LPC_DATA_MAX);
	strcpy(PATH.argData,path);
	PATH.argSize=strlen(path);
	
	char cpMode[10];
	sprintf(cpMode,"%d",flags);
	strcpy(MODE.argData,cpMode);
	MODE.argSize=strlen(cpMode);

	request->lpcArgs[0]=PATH;
	request->lpcArgs[1]=MODE;
	//client to server complete

	// rcv server
	int shmid2 = shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = (LpcResponse*)shared2;

	while(1){
		if (response -> pid > 0){
			response -> pid = -1;
			break;
		}
	}
	return response->errorno;
}

int ReadFile(int fd, void* pBuf, int size)
{	
	int pid = getpid();

	// set shm
	int KEY = getKEY(pid);
	int shmid=shmget((key_t)KEY, 2048, IPC_CREAT|0644);
	void* shared = (void*)0;
	shared=shmat(shmid, (void*)0, 0);
	
	//client to server
	LpcRequest* request = (LpcRequest*)shared;
	
	request->pid=pid;
	request->service=LPC_READ_FILE;
	request->numArg=3;
	
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

	request->lpcArgs[0]=FD;
	request->lpcArgs[1]=PBUF;
	request->lpcArgs[2]=SIZE;
	//client to server complete

	// rcv server
	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = (LpcResponse*)shared2;

	while(1){
		if (response -> pid > 0){
			strcpy((char*)pBuf,response->responseData);;
			response -> pid = -1;
			break;
		}
	}
	
	return response->errorno;
}


int WriteFile(int fd, void* pBuf, int size)
{
	int pid = getpid();

	// set shm
	int KEY = getKEY(pid);
	int shmid=shmget((key_t)KEY, 2048, IPC_CREAT|0644);
	void* shared = (void*)0;
	shared=shmat(shmid, (void*)0, 0);
	
	//client to server
	LpcRequest* request = (LpcRequest*)shared;
	
	request->pid=pid;
	request->service=LPC_WRITE_FILE;
	request->numArg=3;
	
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


	request->lpcArgs[0]=FD;
	request->lpcArgs[1]=PBUF;
	request->lpcArgs[2]=SIZE;
	//client to server complete

	// rcv server
	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = (LpcResponse*)shared2;

	while(1){
		if (response -> pid > 0){
			response -> pid = -1;
			break;
		}
	}
	
	return response->errorno;
}


int CloseFile(int fd)
{
	int pid = getpid();

	// set shm
	int KEY = getKEY(pid);
	int shmid=shmget((key_t)KEY, 2048, IPC_CREAT|0644);
	void* shared = (void*)0;
	shared=shmat(shmid, (void*)0, 0);
	
	//client to server
	LpcRequest* request = (LpcRequest*)shared;
	
	request->pid=pid;
	request->service=LPC_CLOSE_FILE;
	request->numArg=1;
	
	LpcArg FD;

	char cpFd[10];
	sprintf(cpFd,"%d",fd);
	FD.argSize=strlen(cpFd);
	strcpy(FD.argData,cpFd);

	request->lpcArgs[0]=FD;
	//client to server complete

	// rcv server
	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = (LpcResponse*)shared2;

	while(1){
		if (response -> pid > 0){
			response -> pid = -1;
			break;
		}
	}
	
	return response->errorno;
}

int MakeDirectory(char* path, int mode)
{	
	int pid = getpid();

	// set shm
	int KEY = getKEY(pid);
	int shmid=shmget((key_t)KEY, 2048, IPC_CREAT|0644);
	void* shared = (void*)0;
	shared=shmat(shmid, (void*)0, 0);
	
	//client to server
	LpcRequest* request = (LpcRequest*)shared;
	
	request->pid=pid;
	request->service=LPC_MAKE_DIRECTORY;
	request->numArg=2;
	
	LpcArg PATH, MODE;

	PATH.argSize=strlen(path);
	strcpy(PATH.argData,path);

	char cpMode[10];
	sprintf(cpMode,"%d",mode);
	MODE.argSize=strlen(cpMode);
	strcpy(MODE.argData,cpMode);

	request->lpcArgs[0]=PATH;
	request->lpcArgs[1]=MODE;
	//client to server complete

	// rcv server
	int shmid2=shmget((key_t)pid, 2048, IPC_CREAT|0644);
	void* shared2 = (void*)0;
	shared2=shmat(shmid2, (void*)0, 0);
	LpcResponse* response = (LpcResponse*)shared2;

	while(1){
		if (response -> pid > 0){
			response -> pid = -1;
			break;
		}
	}
	
	return response->errorno;
}


int GetString(void* (*callback)(char*))
{
	/*
	LpcRequest request;
	LpcResponse response;

	int msgid=msgget((key_t)KEY,IPC_CREAT|0644);
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
	*/
}
