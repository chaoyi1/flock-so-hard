#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#include "boid.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT  1080

#define RADIUS 5

#define NUMBER_OF_BOIDS 100

#define PERCEPTION_RADIUS 100

#define MAX_FORCE 1
#define MAX_SPEED 6

void RenderBoid(Boid *boid)
{
    Vector2 boidScreenPosition = (Vector2) {boid->pos.x, boid->pos.y};
    DrawCircleV(boidScreenPosition, RADIUS, RAYWHITE);
}

void WrapAroundEdge(Boid *boid) 
{
    if (boid->pos.x > SCREEN_WIDTH) {
        boid->pos.x = 0.0;
    }
    else if (boid->pos.x < 0.0) {
        boid->pos.x = SCREEN_WIDTH;
    }
    if (boid->pos.y > SCREEN_HEIGHT) {
        boid->pos.y = 0.0;
    }
    else if (boid->pos.y < 0.0) {
        boid->pos.y = SCREEN_HEIGHT;
    }
}

int main(int argc, char const *argv[])
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "flockin");

    SetTargetFPS(60);

    Boid **boids = calloc(NUMBER_OF_BOIDS, sizeof(Boid));

    for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
        boids[i] = (CreateBoid((float) SCREEN_WIDTH, (float) SCREEN_HEIGHT));
    }

    while (!WindowShouldClose()) {
        // Variables should be updated here
        for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            WrapAroundEdge(boids[i]);
            CalculateNetEffect(boids, NUMBER_OF_BOIDS, PERCEPTION_RADIUS, MAX_FORCE, MAX_SPEED, boids[i]);
            UpdateBoid(MAX_SPEED, boids[i]);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            RenderBoid(boids[i]);
        }
        EndDrawing();
    }
    for (int i = 0; i < NUMBER_OF_BOIDS; i++) {
            free(boids[i]);
    }
    free(boids);
    CloseWindow();
    return 0;
}