#include "transducers.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void transducer_link2_func(const void *arg, FILE *out, FILE *in1, FILE *in2) {     
    unsigned char c;
    unsigned char d;
    // num of elements to compare
    int num_elems = *(int*)arg;
    int count=0;
    while (1) {
      if (fread(&d, sizeof(unsigned char), 1, in2)!=1) {
        break;
      }
      if (fread(&c, sizeof(unsigned char), 1, in1)!=1) {
        break;
      }
      if ((c==d) && (count < num_elems)) {
        fwrite(&c, sizeof(unsigned char), 1, out);
        count++;
      }    
    }       
  }

void increment_stream(const void *arg, FILE *out, FILE *in) {
  int d = *(int*)arg;
  unsigned char c;
  while (fread(&c, sizeof(unsigned char), 1, in) == 1) {
    c += d;
    if (fwrite(&c, sizeof(unsigned char), 1, out) != 1) {
      break;
    }
  }
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
}

int main() {
  stream* s[3];
  char *input1 = "Hello, World!";
  char *input2 = "Hello";
  char *output = malloc(strlen(input1)+1);
  output[strlen(input1)] = '\0';
  int count = 5;
  assert(transducers_link_source(&s[0], string_stream, input1) == 0);
  assert(transducers_link_source(&s[1], string_stream, input2) == 0);
  assert(transducers_link_2(&s[2], transducer_link2_func, &count, s[0], s[1]) == 0);
  assert(transducers_link_2(&s[2], transducer_link2_func, &count, s[0], s[1]) != 0);
  assert(transducers_link_sink(save_stream, output, s[2]) == 0);
  assert(strcmp(output, "Hello")==0);
  /* Note the sizeof()-trick to determine the number of elements in
     the array.  This *only* works for statically allocated arrays,
     *not* ones created by malloc(). */
  for (int i = 0; i < (int)(sizeof(s)/sizeof(s[0])); i++) {
    transducers_free_stream(s[i]);
  }
  free(output);
  return 0;
}