#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H

#include "boid.h"

float dist_squared(float posx, float posy);
Vec2D cohesion(BoidsState *state, int num_boids, int boid, float cohesion_factor, float perception_range);
Vec2D separation(BoidsState *state, int num_boids, int boid, float separation_radius);
Vec2D alignment(BoidsState *state, int num_boids, int boid, float alignment_factor, float perception_range);

#endif