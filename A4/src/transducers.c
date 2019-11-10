#include "transducers.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct stream {
  int hasReader;
  FILE* file;
};

void transducers_free_stream(stream *s) {
  // A stream
// may only be freed once the transducer tree in which it is used has finished
  //fclose
  // if (s->hasReader == 1){
  //   exit(0);
  // }

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
  file_pipe(files);

  if(fork() == 0){
    // read
          // printf("%s", "child");     
          fclose(files[0]);    
        
        s(arg, files[1]);
        exit(0); 
      

  } else {
    //  printf("%s", "parent");  
    //write
    //close other pipe end here

    fclose(files[1]);
     *out = malloc(sizeof(stream));
     (*out)->hasReader = 0;
    (*out)->file = files[0];

  }
  
  // exit(0);
  // DON#T EXIT IN PARENT, SINK FUNCTION AND
  // SUBSEQUENT FUNCTION CALLS DO NOT GET THROUGH

//will create a stream object and write its address to the given pointer variable

  return 0;
}

int transducers_link_sink(transducers_sink s, void *arg,
                          stream *in) {
    
  // hasreader check
  assert(in->hasReader == 0);
  in->hasReader=1;
  s(arg,in->file);    
  

  return 0;
}

int transducers_link_1(stream **out,
                       transducers_1 t, const void *arg,
                       stream* in) {

  // assert(transducers_link_1(&s[1], increment_stream, &inc, s[0]) == 0);
  // stream* s[2]
  // out  = &s[1]
  // t = increment_stream()
  // arg = &inc
  // in = s[0]
  
  assert(in->hasReader == 0);
  FILE* files[2];
  file_pipe(files);
  if (fork() == 0)
  {
    fclose(files[0]);
    in->hasReader = 1;
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
  //out=out; /* unused */
  //t=t; /* unused */
  //arg=arg; /* unused */
  //in1=in1; /* unused */
  //in2=in2; /* unused */
  
  FILE* files[2];
  file_pipe(files);

  if (fork() == 0)
  {
    fclose(files[0]);
    // in->hasReader = 1;
    t(arg, files[1], in1->file, in2->file);
    exit(0);
  }
  else {

    fclose(files[1]);  
    *out = malloc(sizeof(files));
    (*out)->file = files[0];
  }
  return 0;
}

int transducers_dup(stream **out1, stream **out2,
                    stream *in) {
  out1=out1; /* unused */
  out2=out2; /* unused */
  in=in; /* unused */
  return 1;
}
