#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "boid.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) BOARD_WIDTH)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) BOARD_HEIGHT)

#define RADIUS 5

#define NUMBER_OF_BOIDS 100

void RenderBoid(Boid *boid)
{
    Vector2 boidScreenPosition = (Vector2) {boid->pos.x, boid->pos.y};
    DrawCircleV(boidScreenPosition, RADIUS, RAYWHITE);
}

int main(int argc, char const *argv[])
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "flockin");

    SetTargetFPS(60);

    Boid **flocks = calloc(NUMBER_OF_BOIDS, sizeof(Boid));

    for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
        flocks[i] = (CreateBoid(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
    }

    while (!WindowShouldClose()) {
        // Variables should be updated here
        for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            UpdateBoid(flocks[i]);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            RenderBoid(flocks[i]);
        }
        EndDrawing();
    }
    for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            free(flocks[i]);
    }
    free(flocks);
    CloseWindow();
    return 0;
}