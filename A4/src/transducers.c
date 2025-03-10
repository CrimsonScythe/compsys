#include "transducers.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
struct stream {
  int hasReader;
  FILE* file;
};

void transducers_free_stream(stream *s) {
  fclose(s->file);
  free(s);
}

static int file_pipe(FILE* files[2]) {
  int fds[2];
  int r = pipe(fds);
  if (r == 0) {
    files[0] = fdopen(fds[0], "r");
    files[1] = fdopen(fds[1], "w");
    if (files[0] && files[1]) {
      return 0;
    } else {
      return 1;
    }
  } else {
    return r; 
  }
}

int transducers_link_source(stream **out,
                            transducers_source s, const void *arg) { 

  FILE* files[2];
  assert(file_pipe(files)!=-1);
  if(fork() == 0) {   

    fclose(files[0]);    
    s(arg, files[1]);
    
    exit(0); 
  } 
  else {

    fclose(files[1]);
    *out = malloc(sizeof(stream));
    (*out)->hasReader = 0;
    (*out)->file = files[0];

  }
  return 0;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
  // hasreader check
  if (in->hasReader != 0) {
    return 1;
  }
  in->hasReader=1;
  s(arg,in->file);     
  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {
    if (in->hasReader == 1) {
      return 1;
    } 
    else {
      in->hasReader=1;
    } 
  FILE* files[2];
  assert(file_pipe(files)!=-1);
  if (fork() == 0) {
    fclose(files[0]);
    t(arg, files[1], in->file);
    exit(0);
  }
  else {
    fclose(files[1]);  
    *out = malloc(sizeof(stream));
    (*out)->hasReader = 0;
    (*out)->file = files[0];
  }
  return 0;
}

int transducers_link_2(stream **out,
                       transducers_2 t, const void *arg,
                       stream* in1, stream* in2) {

  if (in1->hasReader == 1 || in2->hasReader == 1) {
    return 1;
  } 
  else {
    in1->hasReader=1;
    in2->hasReader=1;
  }
  FILE* files[2];
  assert(file_pipe(files)!=-1);
  if (fork() == 0) {  
    fclose(files[0]);
    t(arg, files[1], in1->file, in2->file);
    exit(0);
  }
  else {
    fclose(files[1]);  
    *out = malloc(sizeof(stream));
    (*out)->file = files[0];
  }
  return 0;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  if (in->hasReader == 1) {
    return 1;  
  } 
  else {
    in->hasReader = 1;
  }
  FILE* files[2];
  assert(file_pipe(files)!=-1);
  FILE* filez[2];
  assert(file_pipe(filez)!=-1);
  unsigned char c;
  if (fork() == 0) {
    fclose(files[0]);
    fclose(filez[0]);
    while (fread(&c, sizeof(unsigned char), 1, in->file) == 1) {
      fwrite(&c, sizeof(unsigned char), 1, files[1]); 
      fwrite(&c, sizeof(unsigned char), 1, filez[1]);
    }
    exit(0);
    } 
    else {
      fclose(files[1]);
      fclose(filez[1]);
      *out2 = malloc(sizeof(stream));
      (*out2)->file = filez[0];     
      *out1 = malloc(sizeof(stream));
      (*out1)->file = files[0];       
    }
  return 0;
 }