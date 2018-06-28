#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TOK_SIZE 64
#define TOK_DELIMS " \t\r\n\a"
#define MYSH_RL_BUFSIZE 1024

// global variable
char* mysh_builtins[] = { "cd", "help", "exit", "run" };
char cwd[1024];

char* read_line ();
char** split_line (char* line);
int exec_args (char** argv);

int mysh_cd (char** argv);
int mysh_help (char** argv);
int mysh_exit (char** argv);
int mysh_run (char** argv);
int num_builtins();

int (*builtin_funcs[]) (char** argv) = {
  & mysh_cd,
  & mysh_help,
  & mysh_exit,
  & mysh_run
};

int main() {
  char* line;
  char** argv;
  int status;

  do {
    if (getcwd ( cwd, sizeof(cwd) ) == NULL) {
      perror("getcwd() failed..");
    }
    printf("%s > ", cwd);
    line = read_line ();
    argv = split_line (line);
    status = exec_args (argv);

    free (line);
    free (argv);

  } while (status);

  return EXIT_SUCCESS;
}


int exec_args (char** argv)
{
  if (argv[0] == NULL) {
    return 0;
  }

  for (int i=0; i<num_builtins(); i++) {
    if ( strcmp(argv[0], mysh_builtins[i]) == 0 )
      return (*builtin_funcs[i]) (argv);
  }
  printf("Unkown command\n");

  return 1;
}

char* read_line()
{
  int bufferSize = MYSH_RL_BUFSIZE;
  int position = 0;
  char* buffer = malloc(sizeof(char) * bufferSize);
  char c;

  while(1) {
    c = getchar();   // or c = getc(stdin);
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } // Get line

    buffer[position] = c;
    position++;
    if (position == bufferSize){
      bufferSize += MYSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufferSize);
      if(!buffer){
        fprintf(stderr, "realloc() failed..\n");
        exit(EXIT_FAILURE);
      } // Memory allocation failure
    } // Realloc when buffer full
  }
  return NULL;
}

char** split_line (char* line)
{
  int pos = 0, size = TOK_SIZE;
  char** tokens = calloc ( size, sizeof(char*) );
  if (!tokens) { // Memory allocation failure
    perror("calloc() failed..\n");
    exit(EXIT_FAILURE);
  }
  char* token = strtok ( line, TOK_DELIMS );

  while (token != NULL) { // Get all the split arguments
    tokens[pos] = token;
    pos++;
    if (pos + 1 >= size) {
      size += TOK_SIZE;
      tokens = realloc ( tokens, size*sizeof(char*) );
      if (!tokens) {
        perror("realloc() failed..\n");
        exit(EXIT_FAILURE);
      }
    } // Memory reallocation
    token = strtok( NULL, TOK_DELIMS );
  }

  tokens[pos] = NULL;

  return tokens;
}


int mysh_cd(char** argv){
  if (!argv[1]) {
    fprintf(stderr, "ERROR\n");
    return 1;
  }
  if (chdir(argv[1]) != 0){
    perror("cd failed")
    return 1;
  }

  return 1;
}

int mysh_help(char** argv) {
  int content_size = sizeof(mysh_builtins) / sizeof(char*);
  printf("Weiran and Jianhui's shell includes:\n");
  int i;

  for(i = 0; i < content_size; i++) {
    if (strcmp(mysh_builtins[i], "cd") == 0){
      printf("%s : to change directory.\n", mysh_builtins[i]);
    }
    else if (strcmp(mysh_builtins[i], "help") == 0){
      printf("%s : to print out help content.\n", mysh_builtins[i] );
    }
    else if (strcmp(mysh_builtins[i], "exit") == 0){
      printf("%s : to exit the shell.\n", mysh_builtins[i] );
    }
    else if (strcmp(mysh_builtins[i], "run") == 0){
      printf("%s : to run the executables with provided arguments.\n", mysh_builtins[i] );
    }
  }
  printf("Use the man command for information on other programs.\n");

  return 1;
}

int mysh_exit (char** argv) {
  return 0;
}

int mysh_run (char** argv) {
  int status;
  char** args = argv+1;

  pid_t pid = fork();
  if (pid == -1) {
    perror ("fork() failed..\n");
    return 1;
  }
  if (pid == 0) {
    if ( execvp(args[0], args) == -1 ) {
      perror("exec() failed..");
    }
  } else {
    do {
      waitpid (pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int num_builtins () {
  return sizeof(mysh_builtins) / sizeof(char**);
}
