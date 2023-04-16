#include <stdlib.h>
#include "boid.h"
#include <math.h>

Boid *CreateBoid(float x, float y)
{
    Boid *b = malloc(sizeof(Boid));
    
    b->pos.x = rand() % (int) x;
    b->pos.y =  rand() % (int) y;
    
    const float pi = 4.0 * atan(1.0);

    float phi = (float) (rand() * (2 * pi)) / (float) RAND_MAX;
    float vx = (float) cos((double) phi);
    float vy = (float) sin((double) phi);
    b->vel.x = vx * 2;
    b->vel.y = vy * 4;

    b->accel.x = 0;
    b->accel.y = 0;

    return b;
}

float CalcDist(Boid *boid, Boid *other)
{
    float dx = boid->pos.x - other->pos.x;
    float dy = boid->pos.y - other->pos.y;
    return sqrt(dx * dx + dy * dy);
}

void Align(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid)
{
    float vxDesired = 0;
    float vyDesired = 0;
    int numberInRadius = 0;
    for (int i = 0; i < boidsSize; i++) {
        Boid *other = boids[i];
        if (other != boid) {
            float dist = CalcDist(boid, other);
            if (dist < percepRadius) {
                vxDesired += other->vel.x;
                vyDesired += other->vel.y;
                numberInRadius += 1;
            }
        }
    }
    if (numberInRadius > 0) {
        vxDesired = vxDesired / numberInRadius;
        vyDesired = vyDesired / numberInRadius;
        
        // Change magnitude of desired velocity to a max value
        // without changing the direction
        float magnitude = sqrt(vxDesired * vxDesired + vyDesired * vyDesired);
        vxDesired = (vxDesired / magnitude) * maxSpeed;
        vyDesired = (vyDesired / magnitude) * maxSpeed;

        // Steering
        boid->accel.x = vxDesired - boid->vel.x;
        boid->accel.y = vyDesired - boid->vel.y;

        // Limit acceleration
        boid->accel.x = boid->accel.x >= maxForce ? maxForce : boid->accel.x;
        boid->accel.y = boid->accel.y >= maxForce ? maxForce : boid->accel.y;
    }
}

void UpdateBoid(Boid *boid)
{
    boid->pos.x = boid->pos.x + boid->vel.x;
    boid->pos.y = boid->pos.y + boid->vel.y;
    boid->vel.x = boid->vel.x + boid->accel.x;
    boid->vel.y = boid->vel.y + boid->accel.y;
}