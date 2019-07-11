#include "Lpc.h"
#include "LpcStub.h"
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void main(){
	int msgid=msgget((key_t)1234,IPC_CREAT|0644);
	printf("msgid: %d\n",msgid);
	int len;
	pthread_t tid[1024*512];
	int status[1024*512];
	int index=0;

	while(1){
		LpcRequest request;
		len=msgrcv(msgid,&request,2048,0,0);
		if(len>0){
			int type=request.service;
			
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
			/*
			printf("rcv\npid:%d\nservice:%s\nnumArg:%d\n",(int)request.pid,let,request.numArg);
			printf("arg[0] size:%d data:%s\n",request.lpcArgs[0].argSize,request.lpcArgs[0].argData);
			printf("arg[1] size:%d data:%s\n",request.lpcArgs[1].argSize,request.lpcArgs[1].argData);
			printf("arg[2] size:%d data:%s\n",request.lpcArgs[2].argSize,request.lpcArgs[2].argData);
			printf("\n");
			*/
			
			if(request.service==LPC_OPEN_FILE){
				pthread_create(&tid[index],NULL,(void*)OpenFile,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);
			}
			else if(request.service==LPC_READ_FILE){
				pthread_create(&tid[index],NULL,(void*)ReadFile,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);
			}
			else if(request.service==LPC_WRITE_FILE){
				pthread_create(&tid[index],NULL,(void*)WriteFile,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);
			}
			else if(request.service==LPC_CLOSE_FILE){
				pthread_create(&tid[index],NULL,(void*)CloseFile,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);
			}
			else if(request.service==LPC_MAKE_DIRECTORY){
				pthread_create(&tid[index],NULL,(void*)MakeDirectory,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);
			}
			else if(request.service==LPC_GET_STRING){
				pthread_create(&tid[index],NULL,(void*)GetString,(void *)&request);
				pthread_join(tid[index],(void**)&status[index]);	
			}
			index++;
		}
	}
}