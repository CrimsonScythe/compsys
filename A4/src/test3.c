#include "transducers.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


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
  
  char *output0 = malloc(strlen(input1)+1);
  output0[strlen(input1)] = '\0'; /* Ensure terminating NULL. */

  char *output1 = malloc(strlen(input1)+1);
  output1[strlen(input1)] = '\0'; /* Ensure terminating NULL. */

  char *output2 = malloc(strlen(input1)+1);
  output2[strlen(input1)] = '\0'; /* Ensure terminating NULL. */
  
  
//create the stream that gets duplicated
  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  
//duplicate stream
  assert(transducers_dup(&s[2], &s[1], s[0]) == 0);

//write the contents of the orginal stream and duplcates to a string
  assert(transducers_link_sink(save_stream, output0, s[0]) == 0);

  assert(transducers_link_sink(save_stream, output1, s[1]) == 0);

  assert(transducers_link_sink(save_stream, output2, s[2]) == 0);

//comparing the two duplicate streams with the original
  
  printf("output0: %s \n", output0);
  printf("output1: %s\n", output1);
  printf("output2: %s\n", output2);
  //assert(strcmp(output0,output1) == 0);
  //assert(strcmp(output0,output2) == 0);

  /* We cannot use the '==' operator for comparing strings, as strings
     in C are just pointers.  Using '==' would compare the _addresses_
     of the two strings, which is not what we want. */
//   assert(strcmp("Ifmmp-!Xpsme\"",output) == 0);

  

  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  // for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
  //   transducers_free_stream(s[i]);
  // }
  // for (int i = 0; i < (int)(sizeof(s)/sizeof(s[1])); i++) {
  //   transducers_free_stream(s[i]);
  // }
  // for (int i = 0; i < (int)(sizeof(s)/sizeof(s[2])); i++) {
  //   transducers_free_stream(s[i]);
  // }
  
  free(output0);
  free(output1);
  free(output2);

  return 0;
}  
  unsigned char b;
  unsigned char c;