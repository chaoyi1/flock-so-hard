#include <stdlib.h>
#include "boid.h"
#include <math.h>

Boid *CreateBoid(float x, float y)
{
    Boid *b = malloc(sizeof(Boid));
    
    b->pos.x = rand() % (int) x;
    b->pos.y =  rand() % (int) y;
    
    const float pi = 4.0 * atan(1.0);

    double phi = (double) (rand() * (2.0 * pi)) / (double) (RAND_MAX);
    float vx = (float) cos(phi);
    float vy = (float) sin(phi);
    float speed = (float) ((rand() % (4 - 2 + 1)) + 2);
    b->vel.x = vx * speed;
    b->vel.y = vy * speed;

    b->accel.x = 0;
    b->accel.y = 0;

    return b;
}

float CalcDist(Boid *boid, Boid *other)
{
    float dx = boid->pos.x - other->pos.x;
    float dy = boid->pos.y - other->pos.y;
    return sqrt((dx * dx) + (dy * dy));
}

Vector *Align(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid)
{
    Vector *res = malloc(sizeof(Vector));
    int numberInRadius = 0;
    for (int i = 0; i < boidsSize; i++) {
        Boid *other = boids[i];
        if (other != boid) {
            float dist = CalcDist(boid, other);
            if (dist < percepRadius) {
                res->x += other->vel.x;
                res->y += other->vel.y;
                numberInRadius += 1;
            }
        }
    }
    if (numberInRadius > 0) {
        res->x = res->x / numberInRadius;
        res->y = res->y / numberInRadius;
        
        // Change magnitude of desired velocity to a max value
        // without changing the direction
        float magSquared = (res->x * res->x) + (res->y * res->y);
        float magnitude = sqrt(magSquared);
        res->x = res->x * maxSpeed / magnitude;
        res->y = res->y * maxSpeed / magnitude;

        // Steering
        res->x = res->x - boid->vel.x;
        res->y = res->y - boid->vel.y;

        // Rescale vector when magnitude of force is greater than max force allowed
        magSquared = (res->x * res->x) + (res->y * res->y);
        magnitude = sqrt(magSquared);
        if (magnitude > maxForce) {
            res->x = res->x * maxForce / magnitude;
            res->y = res->y * maxForce / magnitude;
        }
    }

    return res;
}

Vector *Cohesion(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid)
{
    Vector *res = malloc(sizeof(Vector));
    int numberInRadius = 0;
    for (int i = 0; i < boidsSize; i++) {
        Boid *other = boids[i];
        if (other != boid) {
            float dist = CalcDist(boid, other);
            if (dist < percepRadius) {
                res->x += other->pos.x;
                res->y += other->pos.y;
                numberInRadius += 1;
            }
        }
    }
    if (numberInRadius > 0) {
        res->x = res->x / numberInRadius;
        res->y = res->y / numberInRadius;
        
        res->x = res->x - boid->pos.x;
        res->y = res->y - boid->pos.y;

        // Change magnitude of desired velocity to a max value
        // without changing the direction
        float magSquared = (res->x * res->x) + (res->y * res->y);
        float magnitude = sqrt(magSquared);
        res->x = res->x * maxSpeed / magnitude;
        res->y = res->y * maxSpeed / magnitude;

        // Steering
        res->x = res->x - boid->vel.x;
        res->y = res->y - boid->vel.y;

        // Rescale vector when magnitude of force is greater than max force allowed
        magSquared = (res->x * res->x) + (res->y * res->y);
        magnitude = sqrt(magSquared);
        if (magnitude > maxForce) {
            res->x = res->x * maxForce / magnitude;
            res->y = res->y * maxForce / magnitude;
        }
    }
    return res;
}

Vector *Separation(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid)
{
    Vector *res = malloc(sizeof(Vector));
    int numberInRadius = 0;
    for (int i = 0; i < boidsSize; i++) {
        Boid *other = boids[i];
        if (other != boid) {
            float dist = CalcDist(boid, other);
            if (dist < percepRadius) {
                float dx;
                float dy;
                dx = boid->pos.x - other->pos.x;
                dy = boid->pos.y - other->pos.y;
                dx = dx / (dist * dist);
                dy = dy / (dist * dist);
                res->x += dx;
                res->y += dy;
                numberInRadius += 1;
            }
        }
    }
    if (numberInRadius > 0) {
        res->x = res->x / numberInRadius;
        res->y = res->y / numberInRadius;

        // Change magnitude of desired velocity to a max value
        // without changing the direction
        float magSquared = (res->x * res->x) + (res->y * res->y);
        float magnitude = sqrt(magSquared);
        res->x = res->x * maxSpeed / magnitude;
        res->y = res->y * maxSpeed / magnitude;

        // // Steering
        res->x = res->x - boid->vel.x;
        res->y = res->y - boid->vel.y;
        
        // Rescale vector when magnitude of force is greater than max force allowed
        magSquared = (res->x * res->x) + (res->y * res->y);
        magnitude = sqrt(magSquared);
        if (magnitude > maxForce) {
            res->x = res->x * maxForce / magnitude;
            res->y = res->y * maxForce / magnitude;
        }
    }
    return res;
}


void CalculateNetEffect(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid)
{
    Vector *alignment = Align(boids, boidsSize, percepRadius, maxForce, maxSpeed, boid);
    Vector *cohesion = Cohesion(boids, boidsSize, percepRadius, maxForce, maxSpeed, boid);
    Vector *separation = Separation(boids, boidsSize, percepRadius, maxForce, maxSpeed, boid);
    boid->accel.x += alignment->x + cohesion->x + separation->x;
    boid->accel.y += alignment->y + cohesion->y + separation->y;
    free(cohesion);
    free(alignment);
    free(separation);
}

void UpdateBoid(float maxSpeed, Boid *boid)
{
    boid->pos.x = boid->pos.x + boid->vel.x;
    boid->pos.y = boid->pos.y + boid->vel.y;
    boid->vel.x = boid->vel.x + boid->accel.x;
    boid->vel.y = boid->vel.y + boid->accel.y;

    // Rescale if speed exceeds max speed
    float magnitude = sqrt(boid->vel.x * boid->vel.x + boid->vel.y + boid->vel.y);
    if (magnitude > maxSpeed) {
        boid->vel.x = boid->vel.x * maxSpeed / magnitude;
        boid->vel.y = boid->vel.y * maxSpeed / magnitude;
    }
    // Reset acceleration because force doesnt accumulate over time
    boid->accel.x = 0.0;
    boid->accel.y = 0.0;
}