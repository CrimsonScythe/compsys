#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>



void increment_stream(const void *arg, FILE *out, FILE *in) {
  int d = *(int*)arg;

  unsigned char c;
  while (fread(&c, sizeof(unsigned char), 1, in) == 1) {
    c += d;
    if (fwrite(&c, sizeof(unsigned char), 1, out) != 1) {
      break;
    }
  }

//   printf("%s", "incrementstream");
}

void string_stream(const void *arg, FILE *out) {
  fputs((const char*) arg, out);
//   printf("%s", "string stream");
}


void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;

  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }

//   printf("%s", "savestream");
}

int main() {
  stream* s[3];

  char *input1 = "Hello, World!";
  char *input2 = "l";
  
  char *output = malloc(strlen(input1)+strlen(input2)+1);
  output[strlen(input1)+strlen(input2)] = '\0'; /* Ensure terminating NULL. */
  


  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  

  assert(transducers_dup(&s[2], &s[1], s[0]) == 0);

  

  
  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
//   assert(strcmp("Ifmmp-!Xpsme\"",output) == 0);

  

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[1])); i++) {
    transducers_free_stream(s[i]);
  }

  free(output);

  return 0;
}