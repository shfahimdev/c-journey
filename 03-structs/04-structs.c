#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[32];
  int health;
  int score;
} Player;

Player *create_player(char name[], int health, int score) {
  Player *p = malloc(sizeof(Player));
  if (p == NULL) return NULL;

  strcpy(p->name , name); 

  p->health = health;
  p->score = score;

  return p;
}

void print_player(Player *p) {
    printf("%s — hp: %d  score: %d\n", p->name, p->health, p->score);
}

void damage(Player *p, int amount) {
  p->health -= amount;
  if (p->health < 0) p->health = 0;
}

int main () {
  Player *p = create_player("fahim", 100, 0);
  
  print_player(p);
  damage(p, 30);
  print_player(p);
  damage(p, 10);
  print_player(p);


  return 0;
}
