#include <windows.h>
#include <stdio.h>
DWORD Sum[1000];

DWORD WINAPI Summation(LPVOID Param){
	DWORD Upper = * (DWORD *)Param;
	DWORD a = 0;
	DWORD b = 1;
	DWORD c = 0;
	Sum[0] = 0;
	if(Upper == 0)
		return 0;
	for (DWORD i = 1; i <= Upper; i++){
		a += b;
		c = b;
		b = a;
		a = c;
		Sum[(int)i] = a;
	}
	return 0;
}

int main(int argc, char * argv[]){
	DWORD ThreadId;
	HANDLE ThreadHandle;
	int Param;
	if(argc != 2){
		fprintf(stderr,"An integer parameter is required.\n");
		return -1;
	}
	Param = atoi(argv[1]);
	if(Param < 0){
		fprintf(stderr ,"An integer >=0 is required.\n");
		return -1;
	}
	
	ThreadHandle = CreateThread(
	NULL,
	0,
	Summation,
	&Param,
	0,
	&ThreadId);
	
	if(ThreadHandle != NULL){
		WaitForSingleObject(ThreadHandle,INFINITE);
		
		CloseHandle(ThreadHandle);
		
		for(int i = 0;i <= Param;i++){
			printf("%d ",Sum[i]);
		}
	}
}
