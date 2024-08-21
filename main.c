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
    char* name;
    int index;
    // c is statically typed so it is just better to do two arrays and a int index
    char* names[2];
    int powers[2];
}Power;

// this is a way to keep track of how many to take away each click from the player
typedef struct Weakness {
    int val;
}Weakness;

// keep count of singular count 
typedef struct Click {
    int val;
}Click;

typedef struct Done {
    bool quit;
}Done;

typedef struct Menu {
    bool check;
}Menu;

typedef struct Type {
    bool update;
}Type;


ECS_COMPONENT_DECLARE(Power);
ECS_COMPONENT_DECLARE(Score);
ECS_COMPONENT_DECLARE(Click);
ECS_COMPONENT_DECLARE(Health);
ECS_COMPONENT_DECLARE(Weakness);
ECS_COMPONENT_DECLARE(Done);
ECS_COMPONENT_DECLARE(Menu);
ECS_COMPONENT_DECLARE(Type);


typedef struct GameState
{
    ecs_world_t* world;
    ecs_entity_t player;
    ecs_entity_t enemy;
} GameState;

// get the user input for that iteration
// void UpdateInput(ecs_iter_t* it){
//     ecs_entity_t* player = ecs_field(it, ecs_entity_t, 1);
//     ecs_entity_t* enemy = ecs_field(it, ecs_entity_t, 2);

//     for (int i = 0; i < it->count; i++){
//         Click* cPlayer = ecs_field(player[i], Click, 3);
//         Click* cEnemy = ecs_field(enemy[i], Click, 2);
//         bool runGame = true;
//         char userInput;
//         char desiredChar = 'a';
//         char menuChar = 'w';
//         char quitChar = 'q';

//         printf("\nType \"%c\": ", desiredChar);
//         userInput = getche();
//         if (userInput == menuChar){
//             // change this later to the menu or something
//             printf("menu");
//         }
//         else if (userInput == desiredChar){
//             cPlayer->didClick = true;
//             cEnemy->didClick = true;
//         }
//         else{
//             cPlayer->didClick = false;
//             cEnemy->didClick = false;
//         }
//     }

// }

// clearing click of user and enemy
// void ClearIteration(ecs_iter_t* it){
//     ecs_entity_t* player = ecs_field(it, ecs_entity_t, 1);
//     ecs_entity_t* enemies = ecs_field(it, ecs_entity_t, 2);

//     for (int i = 0; i < it->count; i++){
//        Click *cPlayer = ecs_field(player[i], Click, 3);
//        Click *cEnemy = ecs_field(enemies[i], Click, 2);
//        cPlayer->didClick = false;
//        cEnemy->didClick = false;
//     }
// }

// *it is pretty much a linkedlist of entities that have the identified components
// You do not directly call this function because it is within a System.
// A system finds the "it" associated with the class and executes the function
void UpdatePlayerByClick(ecs_iter_t *it) {
    // remember that "it" is a list of entities with the following identified components
    ecs_world_t* world = it->world;

    ECS_COMPONENT(world, Type);
    Type* t = ecs_singleton_get(world, Type);

    Score *s = ecs_field(it, Score, 0);
    Power *p = ecs_field(it, Power, 2);
    Click *c = ecs_field(it, Click, 3);
    
    for (int i = 0; i < it->count; i++){
        if (t->update == true){
            c[i].val += 1;
            s[i].val += p[i].strength;
            if (c[i].val == 5){
                p->index += 1;
                p->name = p->names[p->index];
                p->strength = p->powers[p->index];
            }
        }
    }
}

// The enemy health values are decreased if the user attacked, typed the correct char
void UpdateEnemyHealth(ecs_iter_t *it) {
    ecs_world_t* world = it->world;

    ECS_COMPONENT(world, Type);
    Type* t = ecs_singleton_get(world, Type);

    Health *h = ecs_field(it, Health, 0);
    Weakness *w = ecs_field(it, Weakness, 1);
    Click *c = ecs_field(it, Click, 2);

    for (int i = 0; i < it->count; i++){
        if (t->update == true){
            h[i].val -= w->val;
        }
    }

}

// // I can't figure out how to use ecs_get cause for example "power" and "Score" are undefined
// void menuScreen(int playerClick, int playerScore, int playerPow, char* name, char* levelName, int levelPower, int enemyHealth, int weakness){
//     printf("Player:\n"
//             "     click:%d\n" 
//             "     score:%d\n" 
//             "     power:%d\n"
//             "     name:%s\n"
//             "     levelName:%s\n"
//             "     levelPower:%d\n"
//             "Enemy:\n"
//             "     health:%d\n"
//             "     weakness:%d\n"
//         , playerClick, playerScore, playerPow, name, levelName, levelPower, enemyHealth, weakness);
// }

void inputUser(ecs_iter_t* it) {
    ecs_world_t* world = it->world;   

    ECS_COMPONENT(world, Done); 
    ECS_COMPONENT(world, Menu); 
    ECS_COMPONENT(world, Type); 
    Done* d = ecs_singleton_get(world, Done);
    Menu* m = ecs_singleton_get(world, Menu);
    Type* t = ecs_singleton_get(world, Type);

    char userInput;
    char desiredChar = 'a';
    char menuChar = 'w';
    char quitChar = 'q';

    printf("\nType \"%c\": ", desiredChar);
    userInput = getche();

    if (userInput == menuChar){
        m->check = true;
    }
    else if (userInput == quitChar){
        d->quit = true;
    }
    else if (userInput == desiredChar){
        t->update = true;
    }

}

void run(GameState* state) {
    ecs_world_t* world = state->world;
    ECS_COMPONENT(world, Done);

    Done *d = ecs_singleton_get(world, Done);
    while (d->quit == false){
        ecs_progress(world, 0);
    }
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

    // Initialize Flecs world
    ecs_world_t* world = ecs_init();

    ecs_entity_t player =  ecs_entity(world, { .name = "User1" });
    ecs_entity_t enemy = ecs_entity(world, { .name = "enemy" });

    GameState state;
    state.world = world;
    state.player = player;
    state.enemy = enemy;

    // declaring Component's existence
    ECS_COMPONENT(world, Score);
    ECS_COMPONENT(world, Health);
    ECS_COMPONENT(world, Power);
    ECS_COMPONENT(world, Click);  
    ECS_COMPONENT(world, Weakness);
    ECS_COMPONENT(world, Done); 
    ECS_COMPONENT(world, Menu); 
    ECS_COMPONENT(world, Type); 

    ecs_singleton_set(world, Done, { false });
    ecs_singleton_set(world, Menu, { false });
    ecs_singleton_set(world, Type, { false });

    // Assigning components to entities
    ecs_add(world, player, Score);
    ecs_add(world, player, Health);
    ecs_add(world, player, Power);
    ecs_add(world, player, Click);

    ecs_add(world, enemy, Health);
    ecs_add(world, enemy, Weakness);
    ecs_add(world, enemy, Click);

    ecs_set(world, player, Score, {0});
    ecs_set(world, player, Health, {100});
    ecs_set(world, player, Power, {
        .index = 0, 
        .names = {"starter", "intermediate"}, 
        .powers = {2, 3},
        .name = "starter", 
        .strength = 2, 
    });
    ecs_set(world, player, Click, {0, false});

    ecs_set(world, enemy, Health, {50});
    ecs_set(world, enemy, Weakness, {2});
    ecs_set(world, enemy, Click, {0});

    ECS_SYSTEM(world, inputUser, EcsOnUpdate);
    // update each player and enemy system
    ECS_SYSTEM(world, UpdatePlayerByClick, EcsOnUpdate, Score, Health, Power, Click);
    ECS_SYSTEM(world, UpdateEnemyHealth, EcsOnUpdate, Health, Weakness, Click);
    // ECS_SYSTEM(world, UpdateVisual, EcsOnUpdate, Health, Weakness, Click);
    // clear all the changes that happened
    // ECS_SYSTEM(world, ClearIteration, EcsOnUpdate, world, player, enemy);

    // put main run function
    run(&state);

    // Clean up
    ecs_fini(world);
    return 0;
}



void menuScreen(int playerClick, int playerScore, int playerPow, char* name, char* levelName, int levelPower, int enemyHealth, int weakness);

void run(GameState* state);

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