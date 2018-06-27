#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define TOK_SIZE 64
#define TOK_DELIMS " \t\r\n\a"

char** split_line (char* line);

int main(int argc, char* argv[]) {
  size_t size = TOK_SIZE;
  char* a = NULL;
  char** tokens;

  getline( &a, &size, stdin );
  // printf("%s\n", a);
  tokens = split_line(a);
  if (!tokens) {
    perror("get Nothing\n");
    return 1;
  }

  return 0;
}

char** split_line (char* line)
{
  int pos = 0, size = TOK_SIZE;
  char** tokens = calloc ( size, sizeof(char*) );
  char* token = strtok ( line, TOK_DELIMS );

  while (token != NULL) {
    printf("%s\n", token);
    tokens[pos] = token;
    pos++;
    if (pos >= size) {
      size += TOK_SIZE;
      tokens = realloc ( tokens, size*sizeof(char*) );
    }
    token = strtok( NULL, TOK_DELIMS );
  }

  return tokens;
}
