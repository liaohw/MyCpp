#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> 
#include <pthread.h>
#include <sys/syscall.h>

#define _LINE_LENGTH 300 
#define _ALLOC_SIZE (1*1024*1024) 
int ThreadNumber = 3;

#ifdef TRACE_LOG
	#define trace(...)	printf(__VA_ARGS__)
#else
	#define trace(...)	
#endif

void AllocMem(int n = 1){
	const long long size = n * _ALLOC_SIZE;
    trace("\nNew Alloc %lld Memory!\n",size);
    char *szChar = new char[size];
    for(size_t j = 0; j < size; j++){
        szChar[j] = 'a' + (j %10);
    }
}

bool GetCpuMem(float &cpu,size_t &mem, pid_t pid,pthread_t tid = -1){ 
    char cmd[128]; 
    sprintf(cmd, "ps -o %%cpu,rss,%%mem,pid,tid -mp %lld", pid); 
    FILE *pFile = popen(cmd, "r"); 
    if (pFile == NULL){ 
        printf("popen return NULL, error:%s\n",strerror(errno)); 
        return false; 
    }
 
    char line[_LINE_LENGTH]; 
    float l_cpuPrec=0; 
    int l_mem=0; 
    float l_memPrec=0; 
    pid_t l_pid=0; 
    pthread_t l_tid=0; 
    bool ret = false; 
    if (fgets(line, _LINE_LENGTH, pFile) != NULL){ 
        trace("1st line:%s",line); 
        if (fgets(line, _LINE_LENGTH, pFile) != NULL){ 
		    trace("2nd line:%s",line); 
            sscanf( line, "%f %d %f %lld -", &l_cpuPrec, &l_mem, &l_memPrec, &l_pid ); 
            cpu = l_cpuPrec; 
            mem = l_mem/1024; 
            if( tid == -1 ) 
                ret = true; 
            else{ 
                while( fgets(line, _LINE_LENGTH, pFile) != NULL ){ 
              		trace("xnd line:%s ",line); 
                    sscanf( line, "%f - - - %lld", &l_cpuPrec, &l_tid ); 
                    if( l_tid == tid ){ 
                        trace("cpuVal is tid:%lld\n",tid); 
                        cpu = l_cpuPrec; 
                        ret = true; 
                        break; 
                    } 
                }
                if( l_tid != tid ) 
                    trace("TID not exist\n"); 
            } 
        } 
        else 
            trace("PID not exist\n"); 
    } 
    else 
        trace("popen Command or Parameter exec error!\n"); 
    pclose(pFile); 
    return ret; 
} 

void PrintCpuMem(pid_t pid,pthread_t tid){
    float cpu = 0; 
    size_t mem = 0; 
    if( GetCpuMem( cpu, mem, pid, tid ) ){ 
    	if(tid == -1)
        	printf("[PID:%lld]GetCpuMem: %%CPU:%.1f\tMEM:%dMB\n",pid, cpu, mem); 
        else
        	printf("[TID:%lld]GetCpuMem: %%CPU:%.1f\tMEM:%dMB\n",tid, cpu, mem); 
    } 
    else 
        printf("return GetCpuMem false\n"); 
}

void* ThreadTest(void* arg)
{
	pid_t pid = getpid();
	//pthread_t tid = pthread_self();
	pthread_t lw_tid = syscall(SYS_gettid);
	printf("PID(%lld) create thread %s with TID(%lld)\n", pid,__func__,lw_tid);
	int i = 0;
	while (i < 5){
		sleep(2);
		AllocMem(10);
		i++;
		PrintCpuMem(pid,lw_tid);
	}
	printf("TID(%lld) pthread_exit!\n",lw_tid);
	ThreadNumber++;
	pthread_exit((void*)1);
	return (void*)1;
}

void CreateTT(){
	int iRet = 0;
	pthread_attr_t  attr;
	if((iRet=pthread_attr_init(&attr)) != 0 ) {
		printf("...[error] pthread init, code:%d, errmsg:%s\n", iRet, strerror(iRet));		
		return ;
	}
	if((iRet=pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) ) != 0 ){
		printf("...[error] pthread detach, code:%d, errmsg:%s\n", iRet, strerror(iRet));
		return ;			
	}
	pthread_t tid;
	if(pthread_create(&tid, &attr, ThreadTest, NULL)){
		printf("...pthread create failed!\n");
		return ;
	}
	pthread_attr_destroy(&attr);
}
int main(int argc, char** argv) 
{ 
    pid_t pid = 0; 
    pthread_t tid = -1; 
    if( argc > 1 ) 
        pid = atoi(argv[1]); 
    else 
        pid = getpid(); 
    if( argc > 2 ) 
        tid = atoi(argv[2]); 
        
    printf("---------------------process:\n");
    PrintCpuMem(pid,tid);
    int i = 0;
    while(i < 2){
        //sleep(1);
        AllocMem();
        PrintCpuMem(pid,tid);
        i++;
    }
 	printf("\n---------------------thread:\n");
 	int oriThreadNumber = ThreadNumber;
 	while(ThreadNumber > 0){
	 	CreateTT();
		sleep(1);
		ThreadNumber--;
	}
 	while(ThreadNumber < oriThreadNumber){
		sleep(1);
	}

	printf("\n---------------------ending\n");
    return 0; 
} 
