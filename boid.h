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
Vector *Align(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid);
Vector *Cohesion(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid);
void CalculateNetEffect(Boid **boids, int boidsSize, float percepRadius, float maxForce, float maxSpeed, Boid* boid);
void UpdateBoid(float maxSpeed,Boid *boid);

#endif