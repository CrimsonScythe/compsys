#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#define UTF8_2B(ARG) (((ARG) > 191) && ((ARG) < 224)) 
#define UTF8_3B(ARG) ((ARG) >= 224 && (ARG) <= 239)
#define UTF8_4B(ARG) (((ARG) >= 240) && ((ARG) <= 247))
#define UTF8_CONT(ARG) (((ARG) >= 128) && ((ARG) <= 191))


int print_error(const char *path, int max_length, int errnum) {
  return fprintf(stdout, "%s:%*scannot determine (%s)\n",
    path, (int) (max_length - strlen(path)), " ", strerror(errnum)); 
}

int main(int argc, char *argv[]) {
  
  
  // int retval = EXIT_SUCCESS;
  int cha;

  if (argc < 2) {
    printf("usage: file path \n");
    return EXIT_FAILURE;
  }  
  int max_length = 0;
  for (int j = 1; j < argc; j++)
      {
        if ((int) strlen(argv[j]) > max_length)
        {
          max_length = (int)strlen(argv[j] );
        }
      }
  max_length = max_length +1;

  for (int i = 1; i < argc; i++) {
    if (fopen(argv[i],"r") == NULL) {
      print_error(argv[i], 5 ,errno);
      continue;
    }
    char* fileName = argv[i];
    FILE *file = fopen(fileName,"r");   
      int count = {0};
      int num;
      int status;
      while (1) {
        cha = fgetc(file);
        if (cha == EOF) {break;}
        ++count;
        num = cha;
        // printf("%x\n", num);

        if (num == 255)
        {
          if (fgetc(file) == 254)
          {
            status = 4;
            break;
          }
          
        }

        if (num == 254)
        {
          if (fgetc(file) == 255)
          {
            status = 5;
            break;
          }
          
        }
        if ((num >= 7 && num <= 13) || (num == 27) || (num >= 32 && num <= 126)) {
          status = 0;
        }

        else if ((num >= 160 && num <= 255)) {
            status = 2;
            break;
        } 
        else if (num >= 256 && num <= 1114111){
          status = 3;
          break;
        } 
      
        else {
          status = 1;
          break; 
        }
      }
      
      if (count == 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "empty");
      }
      if (status == 0 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "ASCII text");
      }
      if (status == 1 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "data");
      }
      if (status == 2 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "ISO-8859 text");
      }
      if (status == 3 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "UTF-8 Unicode text");
      }
      if (status == 4 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "Little-endian UTF-16 Unicode text");
      }
      if (status == 5 && count > 0) {
        fprintf(stdout, "%s:%*s%s\n",
        fileName, (int) (max_length - strlen(fileName)), " ", "Big-endian UTF-16 Unicode text");
      }  
  }  
  return EXIT_SUCCESS;
}
