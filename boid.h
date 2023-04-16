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
void UpdateBoid(Boid *boid);

#endif