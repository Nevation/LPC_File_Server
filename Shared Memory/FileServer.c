#include "Lpc.h"
#include "LpcStub.h"
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void main(){
	const int PORT[] = {123, 1234, 12345, 54321, 4321, 321};
	pthread_t tid[1024*512];
	int status[1024*512];
	int index=0;

	while(1){
		for (int i=0; i<6; i++){
			int shmid=shmget((key_t)PORT[i], 2048, IPC_CREAT|0644);

			LpcRequest* requests = NULL;
			void* shared = (void*)0;

			shared=shmat(shmid, (void*)0, 0);
			requests=(LpcRequest*)shared;
			if(requests->pid>0){

				LpcRequest* request = (LpcRequest*)malloc(sizeof(LpcRequest));
				request->pid = requests->pid;;
				request->numArg = requests->numArg;
				request->service = requests->service;

				for (int i = 0; i < request->numArg; i++) {
					request->lpcArgs[i].argSize = requests->lpcArgs[i].argSize;
					strcpy(request->lpcArgs[i].argData, requests->lpcArgs[i].argData);
				}

				int type=request->service;
				/*
				char let[1024];
				switch(type){
					case LPC_OPEN_FILE:
					strcpy(let,"OpenFile");
					break;
					case LPC_READ_FILE:
					strcpy(let,"ReadFile");
					break;
					case LPC_WRITE_FILE:
					strcpy(let,"WriteFile");
					break;
					case LPC_CLOSE_FILE:
					strcpy(let,"CloseFile");
					break;
					case LPC_MAKE_DIRECTORY:
					strcpy(let,"MakeDirectory");
					break;
					case LPC_GET_STRING:
					strcpy(let,"GetString");
					break;
				}

				printf("pid: [%ld] PORT: %d service: %s Arg: %d\n",request->pid, PORT[i], let, request->numArg );
				for (int i=0; i< request->numArg;i++)
					printf("\t[%d] size: %d data: %s\n",i+1, request->lpcArgs[i].argSize, request->lpcArgs[i].argData);
				printf("\n");
				*/
				if(request->service==LPC_OPEN_FILE){
					pthread_create(&tid[index],NULL,(void*)OpenFile,(void *)request);
					pthread_join(tid[index],(void**)&status[index]);
				}
				else if(request->service==LPC_READ_FILE){
					pthread_create(&tid[index],NULL,(void*)ReadFile,(void *)request);
					pthread_join(tid[index],(void**)&status[index]);
				}
				else if(request->service==LPC_WRITE_FILE){
					pthread_create(&tid[index],NULL,(void*)WriteFile,(void *)request);
					pthread_join(tid[index],(void**)&status[index]);
				}
				else if(request->service==LPC_CLOSE_FILE){
					pthread_create(&tid[index],NULL,(void*)CloseFile,(void *)request);
					pthread_join(tid[index],(void**)&status[index]);
				}
				else if(request->service==LPC_MAKE_DIRECTORY){
					pthread_create(&tid[index],NULL,(void*)MakeDirectory,(void *)request);
					pthread_join(tid[index],(void**)&status[index]);
				}
				index++;
				requests->pid=-1;
				requests->service=0;
				requests->numArg =0;
			}
			shmdt(shared);
		}

	}
}