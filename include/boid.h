#ifndef BOID_H
#define BOID_H

typedef struct BoidsState BoidsState;
struct BoidsState {
    float *velocity_x;
    float *velocity_y;
    float *position_x;
    float *position_y;
};

typedef struct Vec2D Vec2D;
struct Vec2D {
    float x;
    float y;
};

#endif