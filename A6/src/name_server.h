#pragma once
#include "common.h"
#include "buffer.h"

#define MAIN_ARGNUM 1
#define MAX_USERS   5

typedef struct client_t {
  
  /*
   * template for a struct which can be
   * used to represent a connected client.
   *
   * a client will of course have a socket associated with it;
   * extend this declaration with any other fields you
   * find necessary to maintain across a user session.
   *
   */
  char* usernameAndPassword;
  char* username;
  char* password;
  char* port;
  char* ip;
  
  //needed when quering logged-in users
  int logged_in;
  
  int conn_socket;

  linked_queue *queue[MAX_USERS];

} client_t;
