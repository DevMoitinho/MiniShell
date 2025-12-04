#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//Fixes the stdin and splits into in and args 
//and returns the total count of args.
int fixIn(char in[100], char out[100], char args[10][10]){
	int cntx; //Saves the pointer of the first for to uses on the second
	int x = 0;

	//First loop to put the command of the
	//stdin in a separated string (out)
	for(int i = 0;i<100;i++){
		if(in[i] == '\n'){
			return x;
		}
		if(in[i] == ' '){
			cntx = i+1;
			break;
		}
		out[i] = in[i];	
	}

	//Second loop to put the args in a vector
	//of strings (args) 
	//i = arg
	//j = char in arg
	for(int i = 0; i<10-cntx;i++){
		int j = 0;
		//separate the args by space
		while(in[x+cntx] != ' '){
			//if input ends finish and returns x
			if(in[x+cntx] == '\n'){
				return x;
			}
			
			args[i][j] = in[x+cntx];
			x++;
			j++;
			
		}
		x++;
		
		

	}

	return x;
}

//Shows current path
//user = pointer for username
void showPath(char *user){
	char aux[50] = "";
	getcwd(aux, 50);

	//Special case for home directory
	if(strlen(aux) <= strlen(getenv("HOME"))){
		printf("\033[35m%s:-%s -> \033[0m", user,aux);
		return;
	}

	//makes the path for other cases
	char path[50] = "";
	int count = 3;
	int i = strlen(aux);
	//Uses count = 3 to sets the i
	//variable so the path only shows
	//3 directorys
	do{
		if(aux[i] == '/'){
			i--;
			count--;
		}else{
			i--;
		}
	}while(count > 0);

	//Puts the last 3 directorys in the
	//path string and print it
	for(int e = 0 ;e<strlen(aux);e++){
		path[e] = aux[e+i+1];
	}

	printf("\033[35m%s:-%s -> \033[0m", user,path);

}


int main(){
	//clear terminal before the execution of the shell
	system("clear");
	
	//gets username
	char *user = getlogin();

	//loop of the mini shell
	while(1){
		//show current directory
		showPath(user); 
		
		//Initializing variables;
		//The variables are reset in every loop to its defalt
		//value so theres no conflicts in actual and previous command
		char buffer[100] = "";
		char in[100] = "";
		char args[10][10] = {"","","","","","","","","",""};
		
		//getting and fixing commands
		fgets(buffer, sizeof(buffer), stdin);
		int argCount = fixIn(buffer, in,args);
		
		
		//treats special cases	
		if(strcmp(in,"")==0){
			continue;
		}else if(strcmp(in, "exit") == 0){
			exit(EXIT_SUCCESS);
		}

		//implementing cd
		if(strcmp(in, "cd")==0){
			char *path = args[0];

			if(path==NULL || strcmp(path,"") == 0){
				path = getenv("HOME");
				if(path == NULL){
					char aux[256];
					snprintf(aux,sizeof(aux), "/home/%s",user);
					path = aux;
				}
			}
			
			if(chdir(path) != 0){
				perror("cd");
			}
			continue;
		}

		
		//fork and execute comand
		if(fork() == 0){

			//If theres args, execute with them,
			//else, execute with nothing			
			if(argCount){
				
				if(execlp(in,in,args,NULL) == -1){ 
					perror("Erro ao executar comando.\n");
					exit(EXIT_FAILURE);
				}
			}else{ 
				
				if(execlp(in,in,NULL) == -1){ 
					perror("Erro ao executar comando.\n");
					exit(EXIT_FAILURE);
				}
			}
		}
		else{
			//Principal processes waits the other dies
			wait(NULL);
		}
		
			
	}
	
}
