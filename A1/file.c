#include <stdio.h>  // fprintf, stdout
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS.
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


int print_error(char* fileName, int errnum) {
  return fprintf(stdout, "%s: cannot determine (%s)\n",
    fileName, strerror(errnum)); 
}

int main(int argc, char *argv[]) {
  int retval = EXIT_SUCCESS;
  struct stat buffer;
  int cha;
  if (argc < 2) {
    retval = EXIT_FAILURE;
  }
  if (argv[1] == NULL) {
    printf("usage: file path \n");
    retval = EXIT_SUCCESS;
  }
  for (int i = 1; argv[i] != '\0'; i++) {
    char* fileName = argv[i];
    FILE *file = fopen(fileName,"r");
    int notExist = stat(fileName,&buffer);
    if (notExist && fileName != NULL && file == NULL) {
      print_error(fileName, 2);
      retval = EXIT_SUCCESS;
    } 
    if (fileName != NULL && !notExist) {
      int count = {0};
      int num;
      int status;
      while (1) {
        cha = fgetc(file);
        if (cha == EOF) {break;}
        ++count;
        num = cha;

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
        printf("%s", fileName);
        printf(": empty\n");
      }
      if (status == 0 && count > 0) {
        printf("%s", fileName);
        printf(": ASCII text\n");
      }
      if (status == 1 && count > 0) {
        printf("%s", fileName);
        printf(": data\n");
      }
      if (status == 2 && count > 0) {
        printf("%s", fileName);
        printf(": ISO-8859 text\n");
      }
    }
  }
  return retval;
}
