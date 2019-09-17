#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


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

  for (int i = 1; i < argc; i++)
  {
    if (fopen(argv[i],"r") == NULL)
    {
      print_error(argv[i], 5 ,errno);
      // return EXIT_SUCCESS;
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
        /* printf("%i\n", num);
        printf("%02x\n", cha); */

        if ((num >= 7 && num <= 13) || (num == 27) || (num >= 32 && num <= 126)) {
          status = 0;
        }

        else if ((num >= 160 && num <= 255)) {
            status = 2;
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
        fileName, (int) (max_length - strlen(fileName)), " ", "UTF-8 Unicode text");
      }
    
  
  }
  
  return EXIT_SUCCESS;
}
