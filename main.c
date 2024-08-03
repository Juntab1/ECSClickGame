// main.c
#include <stdio.h>
#include "flecs.h"

typedef struct Health {
    int health;
}Health;

typedef struct Power {
    int strength;
    int score;
    // can make this dynamic with malloc later
    char name[50];
}Power;



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
    ECS_COMPONENT(world, Health);
    ECS_COMPONENT(world, Power);

    // Assigning values
    ecs_add(world, player, Health);
    ecs_add(world, player, Power);
    ecs_add(world, enemy, Health);

    ecs_set(world, player, Health, {100});
    ecs_set(world, enemy, Health, {50});
    ecs_set(world, player, Power, {2, 0, "Default"});

    // need to change this later becuase max is 100, thats conditional, make it dynamic
    char userInput[100];

    printf("Type \"a\": ");

    scanf("%s", userInput);

    int timesOfA;

    for (int i = 0; userInput[i] != '\0'; i++){
        if (userInput[i] == 'a'){
            timesOfA += 1;
        }
    }

    Power *power = ecs_get(world, player, Power);
    power->score = power->strength * timesOfA;
    printf("Score after userInput: %d\n", power->score);


    printf("Number of times pressed: %d \n", timesOfA);

    // must have Power before *power to say what kind of pointer it will be
    const Power *playerPow = ecs_get(world, player, Power);
    printf("Name: %s\n", playerPow->name);
    printf("Strength: %d\n", playerPow->strength);
    printf("Score: %d\n", playerPow->score);






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