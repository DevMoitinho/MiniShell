#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void trataIn(char in[100], char out[100], char args[100]){
	int cntx;
	for(int i = 0;i<100;i++){
		if(in[i] == '\n'){
			return;
		}
		if(in[i] == ' '){
			cntx = i+1;
			break;
		}
		out[i] = in[i];	
	}
	for(int i = 0; i<100-cntx;i++){
		if(in[i+cntx] == '\n'){
			return;
		}
		args[i] = in[i+cntx];
	}

	return;
}

int main(){
	while(1){
		printf("miniShell-> ");
		char buffer[100] = "";
		char entrada[100] = "";
		char args[100] = "";
		fgets(buffer, sizeof(buffer), stdin);
		trataIn(buffer, entrada,args);
		
			
		if(strcmp(entrada, "exit") == 0 || strcmp(entrada,"")==0){
			break;
		}

		pid_t pid = fork();
		if(pid == 0){
			execlp(entrada,entrada,args,NULL);
		}
		else{
			wait(NULL);
		}
		
			
	}
	
}
