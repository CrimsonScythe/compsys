#include "transducers.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


void string_stream(const void *arg, FILE *out) {
  fputs((const char*) arg, out);
}

void save_stream(void *arg, FILE *in) {
  /* We will be writing bytes to this location. */
  unsigned char *d = arg;
  while (fread(d, sizeof(unsigned char), 1, in) == 1) {
    d++; /* Move location ahead by one byte. */
  }
}

int main() {
  // creating the stream, the input and the output
  stream* s[3];
  char *input1 = "Hello, World!";
  char *output1 = malloc(strlen(input1)+1);
  output1[strlen(input1)] = '\0'; /* Ensure terminating NULL. */
  char *output2 = malloc(strlen(input1)+1);
  output2[strlen(input1)] = '\0'; /* Ensure terminating NULL. */
  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  assert(transducers_dup(&s[2], &s[1], s[0]) == 0);
  assert(transducers_dup(&s[2], &s[1], s[0]) != 0);
  assert(transducers_link_sink(save_stream, output2, s[2]) == 0);
  assert(transducers_link_sink(save_stream, output1, s[1]) == 0);
  assert(strcmp(output1, input1)==0);
  assert(strcmp(output2, input1)==0);
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }
  free(output1);
  free(output2);
  return 0;
}  
  