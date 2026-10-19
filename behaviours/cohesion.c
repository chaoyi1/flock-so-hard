#include "behaviours.h"

Vec2D cohesion(BoidsState *state, int num_boids, int boid, float cohesion_factor, float perception_range) {
    Vec2D ci = { 0 };
    int neighbour_count = 0;
    for(int i = 0; i < num_boids; i++) {
        if (i != boid) {
            float dx = state->position_x[i] - state->position_x[boid];
            float dy = state->position_y[i] - state->position_y[boid];
            if (dist_squared(dx, dy) <= perception_range * perception_range) {
                ci.x += dx;
                ci.y += dy;
                neighbour_count++;
            }
        }
    }
    if (neighbour_count == 0) {
        ci.x = 0;
        ci.y = 0;
    } else {
        ci.x = (ci.x / neighbour_count) * cohesion_factor;
        ci.y = (ci.y / neighbour_count) * cohesion_factor;
    }

    return ci;
}