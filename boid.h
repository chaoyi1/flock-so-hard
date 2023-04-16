#ifndef BOID_H
#define BOID_H

typedef struct { 
    float x;
    float y;
} Vector;

typedef struct {
    Vector pos;
    Vector vel;
    Vector accel;
} Boid;

Boid *CreateBoid(float x, float y);
float CalcDist(Boid *boid, Boid *other);
void Align(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid);
void UpdateBoid(Boid *boid);

#endif