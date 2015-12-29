#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

// Question 1
//int main ( int argc, char **argv){
	//int pid, status;

	//pid = fork();

	//if(pid == 0)	
		//{
		//printf("Child\n");
		//sleep(10);
		//}
	//else if (pid>0)
		//{
			//printf("Parent\n");
			//wait(&status);
		//}
		////si il y a erreur
	//else
		//{
		//printf("Error when creating children!\n");
		//}
	
	//printf("End, status = %d\n", status);
	//return status;
	///*
	 //* quand tout se passe normalement, le programme retourne 0.
	 //* Si on envoie un kill 15, le programme renvoit un status de 15
	 //* Si on envoie un kill 9, le programme renvoit un status de 9
	 //* Si le père ne se met pas en attente, il ne se passe rien de spécial
	 //* Si le père est tué, le fils continue son execution normalement
	 //* 
	 //*/
//}


// Question 2
//int main ( int argc, char **argv){
	//int pid, status, n, i,d, enable ;
	//enable = 1;
	//n = 4;
	
	//for(i=0;i<n;i++)
	//{
		//if(enable == 1)
		//{
			//printf("Forking,  PID %d\n", getpid());
			//pid = fork();
		//}
	
		//if( (pid == 0) && (enable == 1))	
		//{
			//printf("Child\n");
			//enable = 0;
			//sleep((i+1));
			//status = 0;
			
		//}
		////si il y a erreur
		//else if(pid<0)
		//{
			//printf("Error when creating children!\n");
		//}
	//}
	
	//// code execute par Parent
	//while((d = wait(&status))!=-1){
		//printf("le fils %d est mort\n",d);
		//}
		
		///*
	//if(pid > 0)
	//{
		//printf("Parent, child: %d\n", pid);
		//printf("Parent PID %d\n", getpid());
		//wait(&status);
	//}*/
	
	
	//printf("End, status = %d\n", status);
	//return status;
//}



// Question 3
//int main ( int argc, char **argv){
	//while(1) fork();
//}
// Fork bomb, les processus se fork jusqu'à ce que le système ne réponde
// plus et crash. Il ny a pas de limite théorique, mais en pratique
// le système n'arrivera pas à gérer un trop grand nombre de process.


// Question 4
//int main ( int argc, char **argv){
	//int pid, status;
	//char command[42];
	//char arg[42];
	//printf("Programme à executer :\n");
	//scanf("%s", &command);
	//// pour la simplicité, on demande les args dans une autre string
	//printf("arguments :\n");
	//scanf("%s", &arg);
	//pid = fork();
	//if(pid > 0){
		//wait(&status);
	//}
	//else if (pid==0)
	//{
		//execlp(command, command, arg, NULL);
	//}
	//else
	//{
	//printf("Error\n");
	//}
	//return 0;
//}

// Question 5
//int main ( int argc, char **argv){
	
	//int nread, pid, status;
	//int pi[2];
	//char text[42];
	//char buff[42];
		
	//// creating pipeline	
	//pipe(pi);
	
	//pid = fork();
	//if(pid > 0){
		//close(pi[0]);
		//printf("Text to send?\n");
		//scanf("%s", &text);
		//write(pi[1], text, (strlen(text)+1));
	
		//wait(&status);
	//}
	//else if (pid==0)
	//{
		//close(pi[1]);
		//do{
			//nread = read(pi[0], buff, sizeof(buff));
		//}while(nread == 0);
		
		//printf("Received %d bytes\n", nread);
		//printf("Received text is: %s\n", buff);
	//}
	//else
	//{
		//printf("Error\n");
		//return -1;
	//}
	
	
	//return 0;
//}

// Question 6

void handler( int sign){
	printf("Exit\n");
	exit(0);
}

int main ( int argc, char **argv){
	
	int nread, pid, status, test;
	int pi[2];
	char text[42];
	char buff[42];
		
	// creating pipeline	
	pipe(pi);
	
	pid = fork();
	if(pid > 0){
		close(pi[0]);
		printf("Text to send?\n");
		//fgets(text, sizeof(text), stdin);
		while(gets(text)!=0){		
		write(pi[1], text, strlen(text));
		write(pi[1], "\0", 1);
		}
		
		//write(pi[1], text, strlen(text));
		//write(pi[1], "\0", 1);
			kill(pid, SIGUSR1);
	
		wait(&status);
	}
	else if (pid==0)
	{
		//signal(SIGUSR1, handler);
		close(pi[1]);
		while(1){
			nread = read(pi[0], buff, sizeof(buff));
			if(nread > 0)
			{
				printf("Received %d bytes\n", nread);
				printf("Received text is: %s\n", buff);
			}
		}
		
		
	}
	else
	{
		printf("Error\n");
		return -1;
	}
	
	
	return 0;
}
