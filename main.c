// main.c
#include <stdio.h>
#include "flecs.h"
#include <conio.h>
#include <ncurses/ncurses.h>

typedef struct Health {
    int val;
}Health;

// score is created by click multiplied by power
typedef struct Score {
    int val;
}Score;

typedef struct Power {
    int strength;
    // can make this dynamic with malloc later
    char name[50];
}Power;

// this is a way to keep track of how many to take away each click from the player
typedef struct Weakness {
    int val;
}Weakness;

// keep count of singular count 
typedef struct Click {
    int val;
}Click;

// *it is pretty much a linkedlist of entities that have the identified components
// You do not directly call this function because it is within a System.
// A system finds the "it" associated with the class and executes the function
void UpdatePlayerByClick(ecs_iter_t *it) {
    // remember that "it" is a list of entities with the following identified components
    Score *s = ecs_field(it, Score, 0);
    Power *p = ecs_field(it, Power, 2);
    Click *c = ecs_field(it, Click, 3);
    
    for (int i = 0; i < it->count; i++){
        c[i].val += 1;
        s[i].val += p[i].strength;
    }
}
// ^^^^ later might need to make it so it always does not add one to the score, only if true

void UpdateEnemyHealth(ecs_iter_t *it) {
    Health *h = ecs_field(it, Health, 0);
    Weakness *w = ecs_field(it, Weakness, 1);

    for (int i = 0; i < it->count; i++){
        h[i].val -= w->val;
    }

}

ecs_world_t *world;
ecs_entity_t player;
ecs_entity_t enemy;


// I can't figure out how to use ecs_get cause for example "power" and "Score" are undefined
void menuScreen(int playerClick, int playerScore, int playerPow, char* name, int enemyHealth, int weakness){
    printf("Player:\n"
            "     click:%d\n" 
            "     score:%d\n" 
            "     power:%d\n"
            "     name:%s\n"
            "Enemy:\n"
            "     health:%d\n"
            "     weakness:%d\n"
        , playerClick, playerScore, playerPow, name, enemyHealth, weakness);
}

int main() {
    // // init screen and sets up screen
    // initscr();
    // // print to screen
    // printw("Hello World");
    // // refreshes the screen
    // refresh();
    // // pause the screen output
    // getch();

    printf("Flecs example program\n");

    // create function later to just initialize the world rather than doing it all in main

    // Initialize Flecs world
    world = ecs_init();

    // giving name parameter to the player
    player = ecs_entity(world, { .name = "User1" });
    enemy = ecs_entity(world, { .name = "enemy" });

    // declaring Component
    ECS_COMPONENT(world, Score);
    ECS_COMPONENT(world, Health);
    ECS_COMPONENT(world, Power);
    ECS_COMPONENT(world, Click);  
    ECS_COMPONENT(world, Weakness);  


    // Assigning values
    ecs_add(world, player, Score);
    ecs_add(world, player, Health);
    ecs_add(world, player, Power);
    ecs_add(world, player, Click);

    ecs_add(world, enemy, Health);
    ecs_add(world, enemy, Weakness);

    ecs_set(world, player, Score, {0});
    ecs_set(world, player, Health, {100});
    ecs_set(world, player, Power, {2, "Default"});
    ecs_set(world, player, Click, {0});

    ecs_set(world, enemy, Health, {50});
    ecs_set(world, enemy, Weakness, {2});

    ECS_SYSTEM(world, UpdatePlayerByClick, EcsOnUpdate, Score, Health, Power, Click);
    ECS_SYSTEM(world, UpdateEnemyHealth, EcsOnUpdate, Health, Weakness);


    // need to change this later becuase max is 100, thats conditional, make it dynamic
    char userInput;
    int timesOfA = 0;

    // dynamic pointer and must have * for it to be a pointer
    Power *power = ecs_get(world, player, Power);
    Click *click = ecs_get(world, player, Click);
    Score *score = ecs_get(world, player, Score);
    Health *health = ecs_get(world, enemy, Health);
    Weakness *weakness = ecs_get(world, enemy, Weakness);

    menuScreen(click->val, score->val, power->strength, power->name, health->val, weakness->val);

    printf("Type \"a\": ");
    userInput = getche();
    // scanf("%s", userInput);
    while (userInput == 'a'){
        ecs_progress(world, 0);
        printf("\nType \"a\": ");
        userInput = getche();
    }
    printf("\n");

    menuScreen(click->val, score->val, power->strength, power->name, health->val, weakness->val);

    // // state which world and what the entity is
    // bool is_alive = ecs_is_alive(world, player);
    // printf("Is alive: %d\n", is_alive);

    // Clean up
    ecs_fini(world);
    return 0;
}

void menuScreen(int playerClick, int playerScore, int playerPow, char* name, int enemyHealth, int weakness);


// Compiling, turns them into object files but not linking them into executable (good when you only change something in one file)
/*
gcc -c flecs.c -std=gnu99
gcc -c main.c -std=gnu99
*/

// Linking: to create it into one .exe because main function is in main.c
/*
gcc -o my_program main.o flecs.o -lws2_32
*/

// both commands in one line
/*
gcc -o my_program main.c flecs.c -std=gnu99 -lws2_32
*/

// add -lncurses -DNCURSES_STATIC after .c and .o config for ncurses


// think about using a system to change values
// also an archetype to make it easier for scaling later
// figure out a way to keep like a scoreboard at the top of the page in your terminal