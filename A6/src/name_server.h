#pragma once
#include "common.h"

#define MAIN_ARGNUM 1
#define MAX_USERS   16

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

} client_t;
