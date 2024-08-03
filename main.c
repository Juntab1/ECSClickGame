// main.c
#include <stdio.h>
#include "flecs.h"

typedef struct Health {
    int value;
}Health;

typedef struct Power {
    int value;
    // can make this dynamic with malloc later
    char name[50];
}Power;



ecs_world_t *world;
ecs_entity_t player;
ecs_entity_t enemy;


int main() {
    
    printf("Flecs example program\n");

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
    ecs_set(world, player, Power, {5, "Default"});

    // must have Power before *power to say what kind of pointer it will be
    const Power *power = ecs_get(world, player, Power);
    printf("Name: %s\n", power->name);





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