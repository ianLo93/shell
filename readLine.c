#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define LSH_RL_BUFSIZE 1024
char* readLine();
int lsh_cd(char** argv);
void lsh_help();

char* lsh_help_content[] = {"cd", "help", "exit"};

int main(int args, char** argv){

  char* output;
  output = readLine();
  printf("%s\n", output);

  if (strcmp(output, "help") == 0) lsh_help();
  int bufferSize = LSH_RL_BUFSIZE;
  char* dir = malloc(sizeof(char) * bufferSize);
  dir[0] = 'c';
  dir[1] = 'd';
  dir[2] = '\0';
  char** test = malloc(sizeof(char*) * bufferSize);
  test[0] = dir;
  test[1] = output;


  if (lsh_cd(test) != 0){
    printf("something wrnig with lsh_cd function\n");
  }


  FILE* file = fopen("testfile.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "FAIL TO OPEN THE FILE\n");
    return 1;
  }
  char c;
  while(1){
    // char c;
    c = fgetc(file);
    if (feof(file)) break;

    printf("%c", c);
  }
  free(output);
  free(dir);
  free(test);

  return 0;

}

char* readLine(){
  int bufferSize = LSH_RL_BUFSIZE;
  int position = 0;
  char* buffer = malloc(sizeof(char) * bufferSize);
  char c;

  while(1){
    // c = getchar();   // or c = getc(stdin);
    c = getc(stdin);
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    }

    buffer[position] = c;
    position++;
    if (position == bufferSize){
      bufferSize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufferSize);
      if(!buffer){
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
      }
    }


  }

}

int lsh_cd(char** argv){
  if (strcmp(argv[0], "cd") != 0){
    fprintf(stderr, "not cd command\n");
    exit(EXIT_FAILURE);
  }
  if (!argv[1]) {
    fprintf(stderr, "error\n");
    exit(EXIT_FAILURE);
  }
  if (chdir(argv[1]) != 0){
    fprintf(stderr, "FAIL: cannot load to directory: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  return 0;

}

void lsh_help(){
  int content_size = sizeof(lsh_help_content) / sizeof(char*);
  printf("Weiran's shell content:\n");
  int i;

  for(i = 0; i < content_size; i++){
    if (strcmp(lsh_help_content[i], "cd") == 0){
      printf("%s : to change directory.\n", lsh_help_content[i]);
    }
    else if (strcmp(lsh_help_content[i], "help") == 0){
      printf("%s : to print out help content.\n", lsh_help_content[i] );
    }
    else if (strcmp(lsh_help_content[i], "exit") == 0){
      printf("%s : to exit the shell.\n", lsh_help_content[i] );
    }
  }
  printf("Use the man command for information on other programs.\n");
}
