#include <stdio.h>

typedef struct {
    char name[32];
    int health;
} Player;

int main() {
    Player party[3] = {
        {"Fahim", 100},
        {"Abid", 50},
        {"Aurin", 40},
    };

    for (int i = 0; i < 3; i++) {
        printf("%s - health : %d\n", party[i].name, party[i].health);
    }

    return 0;
}
