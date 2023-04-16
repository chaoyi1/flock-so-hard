#include <stdlib.h>
#include "boid.h"
#include <math.h>

Boid *CreateBoid(float x, float y)
{
    Boid *b = malloc(sizeof(Boid));
    
    b->pos.x = x;
    b->pos.y = y;
    
    const float pi = 4.0 * atan(1.0);

    float phi = (float) (rand() * (2 * pi)) / (float) RAND_MAX;
    float vx = (float) cos((double) phi);
    float vy = (float) sin((double) phi);
    b->vel.x = vx;
    b->vel.y = vy;

    b->accel.x = 0;
    b->accel.y = 0;

    return b;
}

void UpdateBoid(Boid *boid)
{
    boid->vel.x = boid->vel.x + boid->accel.x;
    boid->vel.y = boid->vel.y + boid->accel.y;
    boid->pos.x = boid->pos.x + boid->vel.x;
    boid->pos.y = boid->pos.y + boid->vel.y;
}