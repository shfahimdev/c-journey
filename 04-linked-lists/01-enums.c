#include <stdio.h>

typedef enum {
  ALIVE,
  DEAD,
  UNKNOWN,
} Status;

typedef struct {
  char name[32];
  Status status;
} Player;

const char *status_to_string(Status s) {
  switch (s) {
    case ALIVE: return "is Alive";
    case DEAD:  return "is Dead";
    case UNKNOWN: return "is somewhere Fucked!";
    default: return "invalid";
  }
}

int main() {
  Player party[3] = {
    {"Fahim", ALIVE},
    {"Aurin", DEAD},
    {"Abid",  UNKNOWN},
  };

  for (int i = 0; i < 3; i++) {
    printf("%s %s\n", party[i].name, status_to_string(party[i].status));
  } 

  return 0;
}
