#include <stdio.h>
#include "name_server.h"
#define MAX_REQUEST   5

enum status {NONE, LOGIN_S, LOGIN_S_END, LOOKUP_S, LOOKUP_S_END ,LOGOUT_S};

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

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
//TODO: accessing glocal variale
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
    status = NONE;
    int IPPORT_READ = 0;
    int USERNAME_READ = 0;

    int logged_in = 0;

    Rio_readinitb(&rio, con);
   
    while ((n=Rio_readlineb(&rio, buf, MAXLINE)) != 0) {

        printf("%s\n", buf);

        if ((strncmp("LOGIN", buf, MAX_REQUEST) == 0) && (status==NONE)) {
           status = LOGIN_S;
           
          continue;
        }
                // else if (strncmp("LOOKUP", buf, 6) == 0 && (status != LOGIN_S))

        
        else if (strncmp("LOOKUP", buf, 6) == 0 && ((status == LOGIN_S_END) || (status == LOOKUP_S) || (status == LOOKUP_S_END)))
        {

          printf("%s\n", "YOLO");
          status = LOOKUP_S;
          continue;
        }
         else if ((strncmp("LOGOUT", buf, 6) == 0) && ((status == LOOKUP_S_END) || (status == LOGIN_S_END)))
        {
          printf("%s\n", "LOGGING OUT :)");
          status = LOGOUT_S;
          continue;
        }

        pthread_mutex_lock(&clients_mutex);

        switch (status) {
        case LOGIN_S:

           if (USERNAME_READ == 0) {
            int i=0;
            while (buf[i] != '\0') {
              i++;
            }
            printf("yellow%s", buf);
              for (int j = 0; j < MAX_USERS; j++){
                if (strncmp(buf, clients[j]->username, i-1) ==0) {
                  
                  // Rio_writen(con, ">>Logged in successfully!\n",28);

                  printf("%s", "this works");

                  userID = j;
                  USERNAME_READ = 1;
                  // clients[userID]->logged_in = 1;

                  break;
                }
                
                
                  
                  // break;
                
                  
              //TODO check else case if string is not matched
              }
              if (USERNAME_READ==0)
              {
                  Rio_writen(con, ">>Login error!\n",28);             
              }
              
              
          //   for (int j = 0; j < MAX_USERS; j++)
          //   {
          //       if (strncmp(buf, clients[j]->usernameAndPassword, i-1) ==0) {
                  
          //     Rio_writen(con, ">>Logged in successfully!\n",28);
              
          //     userID = j;

          //     clients[userID]->logged_in = 1;

          //     break;
          //   }  
          // }

          
          
         
          break;
          }

          if ((USERNAME_READ == 1) && (IPPORT_READ == 0)){
            int i=0;
            while (buf[i] != '\0') {
              i++;
            }

            

            if (strncmp(buf, clients[userID]->password, i-1) ==0) {
              printf("%s\n", "works");    
                  Rio_writen(con, ">>Logged in successfully!\n",26);
            
                  clients[userID]->logged_in = 1;
                  IPPORT_READ = 1;

                  break;
            } else
            {
               USERNAME_READ=0;
               status = NONE;
               Rio_writen(con, ">>Login error!\n",15);
            }
            
            //TODO: check else case if string is not matched
            
            break;
          }
          

          if (n==9 && (IPPORT_READ==1)) {
            // cl->port=buf;
            clients[userID]->port = strdup(buf); 
            printf("port is: %s\n", buf);
            break;
          }
          if (n==17 && (IPPORT_READ==1)) {
            // cl->ip=buf;
            clients[userID]->ip = strdup(buf);
            printf("ip is: %s\n", buf);
            
            // chane enum here since login competed
            status = LOGIN_S_END;

            break;
          }

//if using strncmp then count actual bytes.
//if using n to compare then use actual bytes + 1 because of terminating null

          break;   

        case LOOKUP_S:
        // buf != NULL
          printf("%s\n", "we are in");
          if (strlen(buf) > 0)
          {

            int found=0;
            

            int i=0;
            while (buf[i] != '\0') {
              i++;
            }

      


            for (int j = 0; j < MAX_USERS; j++){

              // only check for logged in users
              if (clients[j]->logged_in == 1)
              {
                if (strncmp(buf, clients[j]->username, i-1) ==0) {
                  printf("heh%s\n", clients[j]->ip);
                    // char* t = strdup(clients[j]->username);
                    found=1;
                    Rio_writen(con, "1", strlen("1"));
                    Rio_writen(con, "\n" ,1);

                    Rio_writen(con, ">>UserName:" ,strlen(">>UserName:"));
                    Rio_writen(con, clients[j]->username ,strlen(clients[j]->username));
                    Rio_writen(con, "  ¦  " ,strlen("  ¦  "));

                    Rio_writen(con, ">>IP:" ,strlen(">>IP:"));
                    Rio_writen(con, clients[j]->ip ,strlen(clients[j]->ip));
                    Rio_writen(con, "  ¦  " ,strlen("  ¦  "));

                    Rio_writen(con, ">>Port:" ,strlen(">>Port:"));
                    Rio_writen(con, clients[j]->port ,strlen(clients[j]->port));
                    Rio_writen(con, "\n" ,1);


                  break;
                }  
              }
              

                
              //TODO check else case if string is not matched
              }

              if (found==0)
              {
                Rio_writen(con, ">>No user found" ,strlen(">>No user found"));
                Rio_writen(con, "\n" ,1);
              }
              

            } else{
//TODO mutex in threading

            
            // int i=0;
            // while (buf[i] != '\0') {
            //   i++;
            // }

            

            int count=0;

            for (int j = 0; j < MAX_USERS; j++){

              // only check for logged in users
              if (clients[j]->logged_in == 1)
              {
                //number of logged in users
                count++;

              }

              //TODO check else case if string is not matched
            }

            printf("Amount of users: %d\n", count);

            char* cccc = malloc(sizeof(char));
            sprintf(cccc, "%d", count);
            Rio_writen(con, cccc , strlen(cccc));
            Rio_writen(con, "\n" , 1);

             for (int j = 0; j < MAX_USERS; j++){

              // only check for logged in users
              if (clients[j]->logged_in == 1)
              {

                    Rio_writen(con, ">>UserName:" ,strlen(">>UserName:"));
                    Rio_writen(con, clients[j]->username ,strlen(clients[j]->username));
                    Rio_writen(con, "  ¦  " ,strlen("  ¦  "));

                    Rio_writen(con, ">>IP:" ,strlen(">>IP:"));
                    Rio_writen(con, clients[j]->ip ,strlen(clients[j]->ip));
                    Rio_writen(con, "  ¦  " ,strlen("  ¦  "));

                    Rio_writen(con, ">>Port:" ,strlen(">>Port:"));
                    Rio_writen(con, clients[j]->port ,strlen(clients[j]->port));
                    Rio_writen(con, "\n" ,1);

              }
              

              //TODO check else case if string is not matched
              }            

 

            }

            status = LOOKUP_S_END;
          
          break;

        case LOGOUT_S:

        if (1)
        {
          /* code */  
            
          printf("jdfgjkdfbjkd%s\n", buf);

            int i=0;
            while (buf[i] != '\0') {
              i++;
            }

          for (int j = 0; j < MAX_USERS; j++){

              // only check for logged in users
            if (strncmp(buf, clients[j]->username, i-1) ==0){
                 
                    clients[j]->logged_in = 0;
                    clients[j]->port = 0;
                    clients[j]->ip = 0;
                    Rio_writen(con, ">>Logged out" ,strlen(">>Logged out"));
                    Rio_writen(con, "\n" ,1);

            }
              

              //TODO check else case if string is not matched
          }            

        // how to decide which struct to erase?

          break;  

        }

        default:
          break;
        }

        pthread_mutex_unlock(&clients_mutex);

    }  
}

void DBSETUP(){

  for (int i = 0; i < MAX_USERS; i++)
  {
    clients[i] = malloc(sizeof(client_t));

    clients[i]->logged_in = 0;
  }

  clients[0]->username = "a";
  clients[1]->username = "b";
  clients[2]->username = "c";
  clients[3]->username = "sara";
  clients[4]->username = "nikolai";

  clients[0]->password = "a";
  clients[1]->password = "b";
  clients[2]->password = "c";
  clients[3]->password = "JokerIsUnderrated";
  clients[4]->password = "ColaHeist";

  

  // clients[5]->usernameAndPassword = "bb";
  // clients[6]->usernameAndPassword = "ss";
  // clients[7]->usernameAndPassword = "gg";
  // clients[8]->usernameAndPassword = "lollol";
  // clients[9]->usernameAndPassword = "troglador";
  // clients[10]->usernameAndPassword = "nnn";
  // clients[11]->usernameAndPassword = "flak";
  // clients[12]->usernameAndPassword = "hahah";
  // clients[13]->usernameAndPassword = "hehe";
  // clients[14]->usernameAndPassword = "nene";
  // clients[15]->usernameAndPassword = "kk";

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