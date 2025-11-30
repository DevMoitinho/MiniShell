#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


//fixes the stdin and splits into in and args 
void fixIn(char in[100], char out[100], char args[100]){
	int cntx; //Saves the pointer of the first for to uses on the second
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

//Shows current path
void showPath(char *user){
	char path[50];
	char aux[50];
	getcwd(aux, 50);
	
	int count = 3;
	int i = strlen(aux);
	do{
		if(aux[i] == '/'){
			i--;
			count--;
		}else{
			i--;
		}
	}while(count > 0);
	
	for(int e = 0 ;e<strlen(aux);e++){
		path[e] = aux[e+i+1];
	}

	printf("\033[35m%s:-%s -> \033[0m", user,path);

}


int main(){
	
	//gets username
	char *user = getlogin();

	//while of the mini shell
	while(1){
		showPath(user); //show current directory
		
		//initializing variables
		char buffer[100] = "";
		char in[100] = "";
		char args[100] = "";
		
		//getting and fixing commands
		fgets(buffer, sizeof(buffer), stdin);
		fixIn(buffer, in,args);
		
		//treats special cases	
		if(strcmp(in,"")==0){
			continue;
		}else if(strcmp(in, "exit") == 0){
			exit(EXIT_SUCCESS);
		}

		//implementing cd
		if(strcmp(in, "cd")==0){
			if(strlen(args) == 0){
				char p[100] = "/home";
				strcat(p,user);
				chdir(p);
			}
			chdir(args);
			continue;
		}

		//fork and execute comand
		pid_t pid = fork();
		if(pid == 0){
			
			if(execlp(in,in,args,NULL) == -1){ //if exec fail show msg and kill
				perror("Erro ao executar comando.\n");
				exit(EXIT_FAILURE);
			}
		}
		else{
			wait(NULL);
		}
		
			
	}
	
}
