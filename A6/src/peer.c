#include "peer.h"
char name_server_ip[IP_LEN];       // hostname and port of name server - these

char name_server_port[PORT_LEN];   // are passed as command line arguments.

int  name_server_socket = -1;      // socket to the name server. initialized to -1.

char buf[MAXLINE];

char my_ip[IP_LEN];                // my_ip and my_port are set on /login, and are used for listening.

char my_port[PORT_LEN];

char my_username[USERNAME_LEN];

char usernameAndPassword[MAXLINE];

int logged_in = 0;


int main(int argc, char **argv) {

  if (argc != MAIN_ARGNUM + 1) {
    fprintf(stderr, "Usage: %s <name server IP> <name server port>.\n", argv[0]);
    exit(EXIT_FAILURE);

  } else if (!is_valid_ip(argv[1])) {

    fprintf(stderr, ">> Invalid name server IP: %s\n", argv[1]);
    exit(EXIT_FAILURE);

  } else if (!is_valid_port(argv[2])) {

    fprintf(stderr, ">> Invalid name server port: %s\n", argv[2]);
    exit(EXIT_FAILURE);

  }
  snprintf(name_server_ip,   IP_LEN,   argv[1]);

  snprintf(name_server_port, PORT_LEN, argv[2]);

  printf(">> Connecting to name server at %s:%s ...\n",
      name_server_ip, name_server_port);
   name_server_socket = Open_clientfd(name_server_ip, name_server_port);
  rio_t rio;
  char rio_buf[MAX_LINE];
  ssize_t num_read;
  
  Rio_readinitb(&rio, STDIN_FILENO);
  
  command_t command;          // current command, and array of size MAX_USER_ARGNUM
  
  args_t    args;             // holding arguments to current command (see peer.h).
  
  char *username, *password;  // these pointers will serve different
  
  char *ip, *port;            // purposes based on the current command.
  
  char *message;
  
  char *saveduser;
  
  char buf2[MAXLINE];
  
  char buf3[MAXLINE];

  char baf[MAXLINE];
  
  char buf4[MAXLINE];
  
  rio_t rio2;
  
  rio_t rio3;
  
  int running = 1;
  
  while (running) { 

    if ((num_read = Rio_readlineb(&rio, rio_buf, MAX_LINE)) < 0) {
      fprintf(stderr, "rio_read() error: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
     else if (num_read <= 1) continue; // if input is an empty line or EOF.
    
    command = parse_command(rio_buf, args); // see common.h for a description of parse_command()
    
    switch (command) {

      case LOGIN: 

        Rio_readinitb(&rio2, name_server_socket);
        if (logged_in) {
          printf(">> /login error: already logged in as %s\n", my_username);
          break;
        }
        
        username = args[0];                // username and password to login with.
        password = args[1];
        ip       = args[2];                // ip and port that the name server should respond to
        port     = args[3];                // (eg. with messages from other users).
        
        snprintf(my_ip,   IP_LEN,   ip);   // write ip and port to my_ip and my_port
        snprintf(my_port, PORT_LEN, port);
        
        char LOGIN_REQUEST[] = "LOGIN"; 
        
        Rio_writen(name_server_socket, LOGIN_REQUEST, strlen(LOGIN_REQUEST));   
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, username, strlen(username));
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, password, strlen(password));
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, my_port, PORT_LEN);
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, my_ip, IP_LEN);
        Rio_writen(name_server_socket, "\n", 1);  
        
        size_t n;
        
        while (1) {
          n=Rio_readlineb(&rio2, buf2,MAXLINE);
          if (n>0) {
            if (strncmp(">>Logged in successfully!", buf2, 25) == 0) {
              logged_in = 1;
            } 
            Fputs(buf2, stdout);          
            // save username in variable
            saveduser = strdup(username);
            break;
          }
        
        }
      break;
      
      case LOOKUP:
       
        if (!logged_in) {
          printf(">> /lookup error: not logged onto name server.\n");
          break;
        }
        
        char LOOKUP_REQUEST[] = "LOOKUP"; 
        username = args[0]; // username to lookup (may be null)
        
        Rio_writen(name_server_socket, LOOKUP_REQUEST, strlen(LOOKUP_REQUEST));   
        Rio_writen(name_server_socket, "\n", 1);
       
        if (username != NULL) {
          Rio_writen(name_server_socket, username, strlen(username));     
        } 
        else {
          Rio_writen(name_server_socket, "", 1);
        }
       
        Rio_writen(name_server_socket, "\n", 1);
        int counter=0;
        int numOfUsers=0;
       
        while (1) {
          n=Rio_readlineb(&rio2, buf3,MAXLINE);
          if (n>0) {
            if (strncmp(">>No user found", buf3, 15) == 0) {
              Fputs(buf3, stdout);
              break;
            } 
            if (counter == 0){
              numOfUsers=atoi(buf3);
              counter++;
              continue;
            }
            Fputs(buf3, stdout);
            if (counter == numOfUsers) {
              break;
            }
            counter++;      
          }
        }
       
        break;
      
      case LOGOUT:
        
        if (!logged_in) {
          printf(">> /logout error: not logged onto name server.\n");
          break;
        }
      
        Rio_writen(name_server_socket, "LOGOUT", strlen("LOGOUT"));   
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, saveduser, strlen(saveduser));
        Rio_writen(name_server_socket, "\n", 1);    
       
        while (1) {
          n=Rio_readlineb(&rio2, buf3,MAXLINE);
          if (n>0) {          
            Fputs(buf3, stdout);
            logged_in=0;
            break;
          }
        }        
        break;
     
      case EXIT:
       
        if (logged_in == 1) {
          printf("%s\n", "Please logout first");
        } 
        else {
          free(saveduser);
          running = 0;
        }
        break;
     
      case MSG:
        if (!logged_in) {
          printf(">> /msg error: not logged onto name server.\n");
          break;
        }
        username = args[0]; // username of recipient.
        message  = args[1]; // actual message to send.

        // printf("%s\n", message);

        Rio_writen(name_server_socket, "MSG", strlen("MSG"));   
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, username, strlen(username));
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, message, strlen(message));
        Rio_writen(name_server_socket, "\n", 1);    

        while (1) {
          n=Rio_readlineb(&rio2, buf4,MAXLINE);
          if (n>0) {          
            Fputs(buf4, stdout);
           
            break;
          }
        }      

        break;
     
      case SHOW:

      if (1)
      {
         size_t n1;
      
      

        if (!logged_in) {
          printf(">> /show error: not logged onto name server.\n");
          break;
        }
        username = args[0]; // name of user to show messages from (may be NULL)
        
        Rio_writen(name_server_socket, "SHOW", strlen("SHOW"));   
        Rio_writen(name_server_socket, "\n", 1);
        Rio_writen(name_server_socket, username, strlen(username));
        Rio_writen(name_server_socket, "\n", 1);


        while (1) {
          n1=Rio_readlineb(&rio2, buf4,MAXLINE);
          if (n1>0) {          
            Fputs(buf4, stdout);
           
            break;
          }
        }      
        
        break;

        }
      case ERROR:
        printf(">> Error: unknown command or wrong number of arguments.\n");
        break;
   
    }
  
  }
 
  printf(">> Closing client ...\n");
  Close(name_server_socket);
  exit(EXIT_SUCCESS);
}
