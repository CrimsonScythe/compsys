#include <stdio.h>
#include "name_server.h"
#include "buffer.c"
#define MAX_REQUEST   5

enum status {NONE, LOGIN_S, LOGIN_S_END, LOOKUP_S, LOOKUP_S_END ,LOGOUT_S,
 MSG_S, SHOW_S};

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
    int MSG_READY=0;
    int msg_user_index;
    

   
   

    Rio_readinitb(&rio, con);
    
    while ((n=Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        
        if ((strncmp("LOGIN", buf, MAX_REQUEST) == 0) && (status==NONE)) {
           status = LOGIN_S;          
          continue;
        }
        else if (strncmp("LOOKUP", buf, 6) == 0 && ((status == LOGIN_S_END) 
        || (status == LOOKUP_S) || (status == LOOKUP_S_END))) {
          status = LOOKUP_S;
          continue;
        }
        else if ((strncmp("LOGOUT", buf, 6) == 0) && ((status == LOOKUP_S_END) 
        || (status == LOGIN_S_END))) {
          printf("%s\n", "LOGGING OUT :)");
          status = LOGOUT_S;
          continue;
        } else if ((strncmp("MSG", buf, 3) == 0)) {
          if (status==SHOW_S)
          {
            MSG_READY=0;
          }
          
          // MSG_READY=0;
          status = MSG_S;
          continue;
        } else if ((strncmp("SHOW", buf, 4) == 0))
        {
          status = SHOW_S;
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
                  userID = j;
                  USERNAME_READ = 1;
                  break;
                }
              }
              if (USERNAME_READ==0) {
                  Rio_writen(con, ">>Login error!\n",28);             
              }
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
            } 
            else {
               USERNAME_READ=0;
               status = NONE;
               Rio_writen(con, ">>Login error!\n",15);
            }
            break;
          }

          if (n==9 && (IPPORT_READ==1)) {
            clients[userID]->port = strdup(buf); 
            printf("port is: %s\n", buf);
            break;
          }

          if (n==17 && (IPPORT_READ==1)) {
            clients[userID]->ip = strdup(buf);
            printf("ip is: %s\n", buf);
            status = LOGIN_S_END;

            // queue for messages intia here
            

            break;
          }
          break;   

        case LOOKUP_S:
          printf("%s\n", "we are in");
          if (strlen(buf) > 0)
          {
            int found=0;
            int i=0;
            while (buf[i] != '\0') {
              i++;
            }
            for (int j = 0; j < MAX_USERS; j++){
              if (clients[j]->logged_in == 1) {
                if (strncmp(buf, clients[j]->username, i-1) ==0) {
                  printf("heh%s\n", clients[j]->ip);
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
              }

              if (found==0) {
                Rio_writen(con, ">>No user found" ,strlen(">>No user found"));
                Rio_writen(con, "\n" ,1);
              }
            } 

            else {
            int count=0;
            for (int j = 0; j < MAX_USERS; j++){
              // only check for logged in users
              if (clients[j]->logged_in == 1) {
                //number of logged in users
                count++;
              }
            }
            printf("Amount of users: %d\n", count);
            char* cccc = malloc(sizeof(char));
            sprintf(cccc, "%d", count);
            Rio_writen(con, cccc , strlen(cccc));
            Rio_writen(con, "\n" , 1);
            for (int j = 0; j < MAX_USERS; j++) {
              if (clients[j]->logged_in == 1) {
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
            }            
          }

          status = LOOKUP_S_END;        
          break;

          case LOGOUT_S:
          if (1) {
            int i=0;
            while (buf[i] != '\0') {
              i++;
            }
          for (int j = 0; j < MAX_USERS; j++) {
              // only check for logged in users
            if (strncmp(buf, clients[j]->username, i-1) ==0){                 
              clients[j]->logged_in = 0;
              clients[j]->port = 0;
              clients[j]->ip = 0;
              Rio_writen(con, ">>Logged out" ,strlen(">>Logged out"));
              Rio_writen(con, "\n" ,1);
            }
          }            
          break;  
        }
        case MSG_S:

        if (MSG_READY==0)
        {

        //receive username here and make checks on username

        int j=0;
        while (buf[j] != '\0') {
          j++;
        }
        
        for (int i = 0; i < MAX_USERS; i++)
        {
          if (strncmp(buf, clients[i]->username, j-1) == 0)
          {
            if ((clients[i]->logged_in) == 0)
            {

              Rio_writen(con, ">>User is not logged in" ,strlen(">>User is not logged in"));
              Rio_writen(con, "\n" ,1);
              break;
            } else {
              msg_user_index = i; 
              MSG_READY=1;
              break;
            }
            
          }
          
        }

        if (MSG_READY==0)
        {
          /* code */
        
        
        Rio_writen(con, ">>User deos not exist" ,strlen(">>User deos not exist"));
        Rio_writen(con, "\n" ,1);
        
        }

        break;

        } else {
          
          int my_user_index = userID; 

          int j=0;
          while (buf[j] != '\0') {
          j++;
          }

        //TODO

          printf("%imsgidsend\n", msg_user_index);
          printf("%imyidsend\n", my_user_index);

          enqueue(clients[msg_user_index]-> queue[my_user_index], buf, j-1);

          // printf("%limsg ret value send\n", clients[msg_user_index]-> queue[my_user_index] ->head->data_len);

          // printf("%imsgone\n", msg_user_index);
          // printf("%imsgtwo\n", my_user_index);

          Rio_writen(con, ">>msg sent" ,strlen(">>msg sent"));
          Rio_writen(con, "\n" ,1);


          //important to reset this flag
          // ensures multiple msgs to same usr get handled correctely
          // by the protocol
          MSG_READY=0;

          break;

        }
        case SHOW_S:

        if (1)
        {
        
        int j=0;
        while (buf[j] != '\0') {
          j++;
        }
        
        printf("%s\n", buf);
        printf("%s\n", clients[userID]->queue[1] -> name);
        int ID;

        char msg_buf[MAXLINE];
        for (int i = 0; i < MAX_USERS; i++)
        {
          if (strncmp(buf, clients[userID]->queue[i] -> name, j-1)==0)
          {
            
            ID = i;
            break;
          }
          
          
        }

        printf("%imsgidshow\n", ID);
        printf("%imyidshow\n", userID);

        // printf("%limsg ret value show\n", clients[userID]-> queue[ID] ->head->data_len);
        size_t size;
        if (clients[userID] -> queue[ID] -> head == NULL)
        {
          printf("%i\n", 1);
          size=0;
        } else {
          size = clients[userID]-> queue[ID] ->head->data_len;
        }
        if (size!=0)
        {
          int i =dequeue(clients[userID]->queue[ID], msg_buf, size);
        }
        
        
        // printf("%i\n", i);
        // printf("%s\n", msg_buf);

        if (size==0)  
        {
        // Rio_writen(con, "\n" ,1);  
        Rio_writen(con, ">>no new msgs" ,strlen(">>no new msgs"));
        Rio_writen(con, "\n" ,1);
        break;  
        }
        


        Rio_writen(con, msg_buf ,strlen(msg_buf));
        Rio_writen(con, "\n" ,1);
        
        break;

        }
        

       

        default: break;
      }

    pthread_mutex_unlock(&clients_mutex);
  }  
}

void DBSETUP(){
  for (int i = 0; i < MAX_USERS; i++) {
    clients[i] = malloc(sizeof(client_t));
    clients[i]->logged_in = 0;

    //TODO set empty queue?

  }
  clients[0]->username = "a";
  clients[1]->username = "b";
  clients[2]->username = "c";
  clients[3]->username = "LOGIN";
  clients[4]->username = "LOOKUP";

  clients[0]->password = "a";
  clients[1]->password = "b";
  clients[2]->password = "c";
  clients[3]->password = "LOGIN";
  clients[4]->password = "LOOKUP";

  for (int i = 0; i < MAX_USERS; i++) {
    

    for (int j = 0; j < MAX_USERS; j++){
      clients[i] -> queue[j] = malloc(sizeof(linked_queue));
      clients[i] -> queue[j] -> name = clients[j]->username;
      // clients[i] -> queue[j] -> head ->data_len = 0;
      //TODO check these two
      // clients[i] -> queue[j] -> head = malloc(sizeof(queue_node));
      // clients[i] -> queue[j] -> tail = malloc(sizeof(queue_node));
    }

  }

  // printf("%s\n", clients[0]->queue[1] ->name);
  // printf("%s\n", clients[1]->queue[0] ->name);

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
  } 
  else if (!is_valid_port(argv[1])) {
    fprintf(stderr, ">> Invalid port number: %s\n", argv[1]);
    exit(EXIT_FAILURE);

  }
  snprintf(listen_port, PORT_LEN, argv[1]);

  for (int i = 0; i < MAX_USERS; i++)
    clients[i] = NULL;

  DBSETUP();
  
  printf(">> Accepting peers on port %s ...\n", listen_port);
  
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
  }
  
  exit(EXIT_SUCCESS);
}