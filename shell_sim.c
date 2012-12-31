//Thomas Zaorski
//RIN: 660747712
//Email: zaorst@rpi.edu

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

//Kills zombies, as the name implies.
void *kill_zombies( int n )
{
  int status;
  waitpid( 0, &status, WNOHANG);
  return (void *)NULL;
}

int main()
{
  //Variables needed throughout the program
  char *str= (char *)malloc(100);
  char * pch;
  char *argv[5];
  char *new;
  char *buf = NULL;
  int i = 1;
  int size = 1;
  int background = 0;

  //Prints shell intro
  printf("welcome to myshell\n"); 
  char * current;

  //If THEPATH is not set, it is set to /bin:.
  setenv("THEPATH", "/bin:.", 0);
  current = getenv("THEPATH");
  printf("($THEPATH is: %s)\n", current);

  //Initilizes variable that controls the loop
  int looper = 1;

//Loops until variable looper != 1
do{
  //Prints the prompt and recieves user input
  printf("--| ");
  gets(str);
  //Tokenizes the user entered string, while keeping track of the number of arguments entered
  pch = strtok (str," ");
  argv[0] = pch;
  while (pch != NULL)
  {
    pch = strtok (NULL, " ");
    argv[i] = pch;
    i++;
    size++;
  }

//Checks to see if & was added at the end of the string.
if (strcmp(argv[size-2], "&")==0)
{
  //If & was entered at the end of string, this variable will be used in hiding process in the background
  background = 1;
  size--;
}

//Handles the input. First takes care of special cases "exit", "quit", "kill", and "cd"
if((strcmp(argv[0], "exit") == 0) || strcmp(argv[0], "quit") == 0)
{
   looper = 0;
   printf("bye\n");
   //The next few lines hold off on exiting until all children are terminated. Prevents potential odd orphan behavior
    int status;
    int pid= wait(&status);
     if (pid < 0)
     {
        //This space is only here because the compiler threw out warnings if I didn't use pid here
     }
   
}
else if(strcmp(argv[0], "kill") == 0)
{
  int the_pid;
  the_pid = atoi(argv[1]);  //Turns the string of numbers into the int pid
  kill(the_pid, SIGKILL);
}
else if(strcmp(argv[0], "cd") == 0)
{
  //Builds new path to change directory too
  if((size - 1) == 1)
  {
     chdir(getenv("HOME"));
  }
  else
  {
  new = getcwd(buf, 1000);
  new = strcat(new, "/");
  new = strcat(new, argv[1]);
  int success;
  success = chdir(new);
  if (success != 0)
   {
      printf("ERROR: no such directory '%s'.\n", argv[1]);
   }
  }
}
else
{
//Creates the child process, its pid will be childpid
pid_t childpid;
childpid = fork();
if(childpid == 0) //Only the child process will enter here
 {
   //From lines 112 to 147: handles user entered redirects and append-redirection
   if (size - 1 >= 3)
{
   if(strcmp(argv[size-3], "==>") == 0)
   {
      close(1);
      fflush(NULL);
      int rc = open(argv[size-2], O_WRONLY | O_CREAT | O_TRUNC, 0660);
      if (rc < 0)
      {
        perror("Open failed!");
      }
      size = size - 2;
    }
   else if(strcmp(argv[size-3], "-->") == 0)
   {
      close(1);
      fflush(NULL);
      int rc = open(argv[size-2], O_WRONLY | O_CREAT | O_APPEND, 0660);
      if (rc < 0)
      {
        perror("Open failed!");
      }
      size = size - 2;
    }
    else if(strcmp(argv[size-3], "<==") == 0)
   {
      close(0);
      fflush(NULL);
      int rc = open(argv[size-2], O_RDONLY);
      if (rc < 0)
      {
        perror("Open failed!");
      }
      size = size - 2;
    }
}
   //Which execlp() gets called depends on the number of arguments the user supplied
   if(size - 1 == 1)
    {
      execlp(argv[0], current, NULL);
      perror("ERROR");
      return EXIT_FAILURE; //The perror only prints if the execlp fails, and so the child returns failure
    }
   else if(size - 1 == 2)
   {
     execlp(argv[0], current, argv[1], NULL);
     perror("ERROR");
     return EXIT_FAILURE;
  }
  else if(size - 1 == 3)
    {
      execlp(argv[0], current, argv[1], argv[2], NULL);
      perror("ERROR");
      return EXIT_FAILURE;
    }
   else if(size - 1 == 4)
    {
      execlp(argv[0], current, argv[1], argv[2], argv[3], NULL);
      perror("ERROR");
      return EXIT_FAILURE;
    }
   else if(size - 1 == 5)
    {
      execlp(argv[0], current, argv[1], argv[2], argv[3], argv[4], NULL);
      perror("ERROR");
      return EXIT_FAILURE;
    }
 }
 else if (childpid > 0) //Only the parent will enter here
{
   //Does not enter here if & was found at the end of the user input
   if (background == 0)
   {
      int status;
      int pid= wait(&status);
     if (pid < 0)
     {
        printf("PARENT: Something went wrong in child: %d", pid);
     }
    }
   else
   {
      printf("[process running in background with pid %d]\n", childpid);
      //Signal handling to deal with zombies that may be produced by running programs in the background
      signal(SIGCHLD, (void*)kill_zombies);
   }
   
   //resets important variables
   size = 1;
   i = 1;
   background = 0;
}
else //Will only enter here if fork() failed
{
  perror("Fork Failed");
}
}
 
 i = 1;
 size = 1;
 background = 0;

}while(looper == 1);

 return 0;

}
