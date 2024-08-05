// main.c
#include <stdio.h>
#include "flecs.h"
#include<conio.h>


typedef struct Health {
    int health;
}Health;

typedef struct Score {
    int val;
}Score;

typedef struct Power {
    int strength;
    // can make this dynamic with malloc later
    char name[50];
}Power;

// *it is pretty much a linkedlist of entities that have the identified components
// You do not directly call this function because it is within a System.
// A system finds the "it" associated with the class and executes the function
void UpdateScore(ecs_iter_t *it) {
    // remember that this might be a list of entities with score
    Score *s = ecs_field(it, Score, 0);
    
    for (int i = 0; i < it->count; i++){
        s[i].val += 1;
    }
}
// ^^^^ later might need to make it so it always does not add one to the score, only if true



ecs_world_t *world;
ecs_entity_t player;
ecs_entity_t enemy;


int main() {
    
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


    // Assigning values
    ecs_add(world, player, Score);
    ecs_add(world, player, Health);
    ecs_add(world, player, Power);
    ecs_add(world, enemy, Health);


    ecs_set(world, player, Score, {0});
    ecs_set(world, player, Health, {100});
    ecs_set(world, player, Power, {2, "Default"});

    ecs_set(world, enemy, Health, {50});

    ECS_SYSTEM(world, UpdateScore, EcsOnUpdate, Score, Health, Power);


    // need to change this later becuase max is 100, thats conditional, make it dynamic
    char userInput;
    int timesOfA = 0;

    printf("Type \"a\": ");
    userInput = getche();
    // scanf("%s", userInput);
    while (userInput == 'a'){
        ecs_progress(world, 0);
        printf("\nType \"a\": ");
        userInput = getche();
    }

    Power *power = ecs_get(world, player, Power);
    Score *score = ecs_get(world, player, Score);
    printf("\nScore after userInput: %d", score->val);
    timesOfA = score->val;
    score->val = power->strength * score->val;

    printf("\nScore after userInput and multiply: %d\n", score->val);


    printf("Number of times pressed: %d \n", timesOfA);

    // must have Power before *power to say what kind of pointer it will be
    const Power *playerPow = ecs_get(world, player, Power);
    printf("Name: %s\n", playerPow->name);
    printf("Strength: %d\n", playerPow->strength);
    printf("Score: %d\n", score->val);






    // printf("Name: %s\n", ecs_get_name(world, player));

    // // state which world and what the entity is
    // bool is_alive = ecs_is_alive(world, player);
    // printf("Is alive: %d\n", is_alive);

    // Clean up
    ecs_fini(world);
    return 0;
}


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


// think about using a system to change values
// also an archetype to make it easier for scaling later
// figure out a way to keep like a scoreboard at the top of the page in your terminal