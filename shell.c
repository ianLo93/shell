#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TOK_SIZE 64
#define TOK_DELIMS " \t\r\n\a"
#define MYSH_RL_BUFSIZE 1024

// global variable
char* mysh_builtins[] = { "cd", "help", "exit" };

char* read_line();
char** split_line (char* line);
int exec_args (char** argv);

int mysh_cd(char** argv);
int mysh_help(char** argv);
int mysh_exit(char** argv);
int num_builtins();

int (*builtin_funcs[]) (char** argv) = {
  & mysh_cd,
  & mysh_help,
  & mysh_exit
};

int main(int argc, char* argv[]) {
  char* line;
  char** args;
  int status = 1;

  do {
    printf("> ");
    line = read_line ();
    args = split_line (line);
    status = exec_args (args);

    free (line);
    free (args);

  } while (status);

  return EXIT_SUCCESS;
}


int exec_args (char** argv)
{
  if (argv[0] == NULL) {
    return 1;
  }

  for (int i=0; i<num_builtins(); i++) {
    if ( strcmp(argv[0], mysh_builtins[i]) == 0 )
      return (*builtin_funcs[i]) (argv);
  }

  return 1;
}



char* read_line()
{
  int bufferSize = MYSH_RL_BUFSIZE;
  int position = 0;
  char* buffer = malloc(sizeof(char) * bufferSize);
  char c;

  while(1){
    c = getchar();   // or c = getc(stdin);
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    }

    buffer[position] = c;
    position++;
    if (position == bufferSize){
      bufferSize += MYSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufferSize);
      if(!buffer){
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }


  }

}

char** split_line (char* line)
{
  int pos = 0, size = TOK_SIZE;
  char** tokens = calloc ( size, sizeof(char*) );
  if (!tokens) {
    perror("memory alloc failed..\n");
    exit(EXIT_FAILURE);
  }
  char* token = strtok ( line, TOK_DELIMS );

  while (token != NULL) {
    printf("%s\n", token);
    tokens[pos] = token;
    pos++;
    if (pos >= size) {
      size += TOK_SIZE;
      tokens = realloc ( tokens, size*sizeof(char*) );
      if (!tokens) {
        perror("memory realloc failed..\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok( NULL, TOK_DELIMS );
  }

  tokens[pos] = NULL;

  return tokens;
}


int mysh_cd(char** argv){
  if (strcmp(argv[0], "cd") != 0){
    fprintf(stderr, "ERROR: not 'cd' command\n");
    exit(EXIT_FAILURE);
  }
  if (!argv[1]) {
    fprintf(stderr, "ERROR\n");
    exit(EXIT_FAILURE);
  }
  if (chdir(argv[1]) != 0){
    fprintf(stderr, "FAIL: cannot load to directory: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  return 1;

}

int mysh_help(char** argv) {
  int content_size = sizeof(mysh_builtins) / sizeof(char*);
  printf("Weiran and Jianhui's shell content:\n");
  int i;

  for(i = 0; i < content_size; i++){
    if (strcmp(mysh_builtins[i], "cd") == 0){
      printf("%s : to change directory.\n", mysh_builtins[i]);
    }
    else if (strcmp(mysh_builtins[i], "help") == 0){
      printf("%s : to print out help content.\n", mysh_builtins[i] );
    }
    else if (strcmp(mysh_builtins[i], "exit") == 0){
      printf("%s : to exit the shell.\n", mysh_builtins[i] );
    }
  }
  printf("Use the man command for information on other programs.\n");

  return 1;
}

int mysh_exit(char** argv) {
  exit(0);
}

int num_builtins () {
  return sizeof(mysh_builtins) / sizeof(char**);
}
