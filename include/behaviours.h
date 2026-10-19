#ifndef BEHAVIOURS_H
#define BEHAVIOURS_H

#include "boid.h"

float dist_squared(float posx, float posy);
Vec2D clamp_magnitude(Vec2D vector, float max_magnitude);
Vec2D steering_acceleration(Vec2D desired_direction, Vec2D current_velocity, float max_speed, float response_time, float max_acceleration);
Vec2D cohesion(BoidsState *state, int num_boids, int boid, float perception_range);
Vec2D separation(BoidsState *state, int num_boids, int boid, float separation_radius);
Vec2D alignment(BoidsState *state, int num_boids, int boid, float perception_range);

#endif