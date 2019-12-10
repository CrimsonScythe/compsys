#include <stdio.h>
#include "name_server.h"
#define MAX_REQUEST   5

enum status {LOGIN_S, LOGOUT_S};

char listen_port[PORT_LEN];
int  listen_socket     = -1;       // listen socket. initialized to -1.

int num_active_clients = 0;
client_t *clients[MAX_USERS];      // array of pointers to structs representing active clients.

 int *connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  char client_hostname[MAXLINE], client_port[MAXLINE];
  pthread_t tid;




client_t *cl;

/*
 * TODO: for implementing name_server.c, you will not receive as many
 * TODO: hints as you did for peer.c :) the two TODO's within the main()
 * TODO: function should therefore NOT be considered exhaustible;
 * TODO: you will need to figure out the rest on your own.
 */

void echo(int con) {
    size_t n;
    char buf[MAX_LINE];
    rio_t rio;
    int userID=0;
    enum status status;
    int IPPORT_READ = 0;

    Rio_readinitb(&rio, con);
   
    while ((n=Rio_readlineb(&rio, buf, MAXLINE)) != 0) {     
        if (strncmp("LOGIN", buf, MAX_REQUEST) == 0) {
           status = LOGIN_S;
          
          continue;
        }
        

        switch (status) {
        case LOGIN_S:

           if (IPPORT_READ == 0) {
            int i=0;
            while (buf[i] != '\0') {
              i++;
            }
         
         
            for (int j = 0; j < MAX_USERS; j++)
            {
                if (strncmp(buf, clients[j]->usernameAndPassword, i-1) ==0) {
                  
              Rio_writen(con, ">>Logged in successfully!\n",28);
              // printf("%s", "Logged in successfully!");
              userID = j;

              break;
            }  
          }

          
          IPPORT_READ = 1;
          break;
          }

          if (n==9 && (IPPORT_READ==1)) {
            // cl->port=buf;
            clients[userID]->port = buf; 
            printf("port is: %s\n", buf);
            break;
          }
          if (n==17 && (IPPORT_READ==1)) {
            // cl->ip=buf;
            clients[userID]->ip = buf;
            printf("ip is: %s\n", buf);
            
            break;
          }

//if using strncmp then count actual bytes.
//if using n to compare then use actual bytes + 1 because of terminating null

          break;      
        default:
          break;
        }
    }  
}

void DBSETUP(){

  // cl = malloc(sizeof(client_t));
  // cl->usernameAndPassword = "ab";


// clients[0] = malloc(sizeof(struct client_t));
// clients[0]->usernameAndPassword = "ab";
  
  for (int i = 0; i < MAX_USERS; i++)
  {
    clients[i] = malloc(sizeof(client_t));
  }


  // printf("%s", clients[0]->usernameAndPassword);
  clients[0]->usernameAndPassword = "ab";
  clients[1]->usernameAndPassword = "suhailMeowthIsUgly";
  clients[2]->usernameAndPassword = "haseebMaximumDrift";
  clients[3]->usernameAndPassword = "saraJokerIsUnderrated";
  clients[4]->usernameAndPassword = "nikolaiColaHeist";
  clients[5]->usernameAndPassword = "bb";
  clients[6]->usernameAndPassword = "ss";
  clients[7]->usernameAndPassword = "gg";
  clients[8]->usernameAndPassword = "lollol";
  clients[9]->usernameAndPassword = "troglador";
  clients[10]->usernameAndPassword = "nnn";
  clients[11]->usernameAndPassword = "flak";
  clients[12]->usernameAndPassword = "hahah";
  clients[13]->usernameAndPassword = "hehe";
  clients[14]->usernameAndPassword = "nene";
  clients[15]->usernameAndPassword = "kk";

}

void *thread(void *vargp){
  int confd = *((int *)vargp);
  Pthread_detach(pthread_self());
  Free(vargp);

  echo(confd);

  Close(confd);
  return NULL;
}

int main(int argc, char **argv) { 

  if (argc != MAIN_ARGNUM + 1) {
    fprintf(stderr, "Usage: %s <listening port>\n", argv[0]);
    exit(EXIT_FAILURE);
  } else if (!is_valid_port(argv[1])) {
    fprintf(stderr, ">> Invalid port number: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  

  snprintf(listen_port, PORT_LEN, argv[1]);

  for (int i = 0; i < MAX_USERS; i++)
    clients[i] = NULL;

  DBSETUP();

  printf(">> Accepting peers on port %s ...\n", listen_port);

  /*
   * TODO: SETUP LISTENING SOCKET HERE
   * HINT: remember that you are free to use everything from csapp.c ;) ;)
   */
  listen_socket = Open_listenfd(listen_port);

  int running = 1;
  while (running) {
    clientlen = sizeof(struct sockaddr_storage);
  

    connfd = malloc(sizeof(int));
    *connfd = Accept(listen_socket, (SA*)&clientaddr, &clientlen);
    
    Getnameinfo((SA*)&clientaddr, clientlen, client_hostname, MAXLINE,
          client_port, MAXLINE, 0);

    printf("Connected to (%s, %s)\n", client_hostname, client_port);

    Pthread_create(&tid, NULL, thread,connfd);
    

    // Close(connfd);
    // set running to 0 so it prints
    // running=0;
    /*
     * TODO: LISTEN FOR PEERS HERE
     * HINT: accept new peers on the listen_socket set up earlier.
     * HINT: you may want to represent clients with a struct. a template
     * HINT: struct client_t is given in name_server.h, which you can extend.
     * 
     * HINT: consider why this is wrapped in a loop - once a
     * HINT: client connects, the server should delegate servicing
     * HINT: the client and continue listening for new clients :)
     *
     *
     * HINT: read Beej's guide: http://beej.us/guide/bgnet/html/
     * HINT: (in particular the "System Calls or Bust" section)
     *
     */
  }

  exit(EXIT_SUCCESS);
}