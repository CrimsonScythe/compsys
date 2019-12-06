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

  int conn_socket;

} client_t;
