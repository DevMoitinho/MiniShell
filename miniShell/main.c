#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//auxiliar function to count valid elements in a array
int countElements(char a[10][10]){
	int count = 0;
	for(int i; i<10;i++){
		if(strlen(a[i]) == 0){
			return count;
		}
		count++;
	}
	return count;
}

//fixes the stdin and splits into in and args 
void fixIn(char in[100], char out[100], char args[10][10]){
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
	int x = 0;
	
	for(int i = 0; i<10-cntx;i++){
		int j = 0;
		while(in[x+cntx] != ' '){
			if(in[x+cntx] == '\n'){
				return;
			}
			args[i][j] = in[x+cntx];
			x++;
			j++;
		}
		x++;
		//if(in[i+cntx] == '\n'){
		//	return;
		//}

		//args[i] = in[i+cntx];
	}

	return;
}

//Shows current path
void showPath(char *user){
	char path[50] = "";
	char aux[50] = "";
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
	system("clear");
	
	//gets username
	char *user = getlogin();

	//while of the mini shell
	while(1){
		showPath(user); //show current directory
		
		//initializing variables
		char buffer[100] = "";
		char in[100] = "";
		char args[10][10] = {"","","","","","","","","",""};
		
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

		if(strcmp(in, "clear") == 0){
			system("clear");
			continue;
		}

		//fork and execute comand
		if(fork() == 0){
			
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
