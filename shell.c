#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define TOK_SIZE 64
#define TOK_DELIMS " \t\r\n\a"

char* read_line ();
char** split_line (char* line);
int exec_args (char** args);

int main(int argc, char* argv[]) {
  char* line;
  char** args;
  int status;

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

char* read_line () { }

int exec_args (char** args) { }

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

  return tokens;
}
