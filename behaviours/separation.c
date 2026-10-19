#include "behaviours.h"
#include <math.h>

Vec2D separation(BoidsState *state, int num_boids, int boid, float separation_radius) {
    Vec2D c = { 0 };
    for(int i = 0; i < num_boids; i++) {
        if (i != boid) {
            float dx = state->position_x[i] - state->position_x[boid];
            float dy = state->position_y[i] - state->position_y[boid];
            float dist_sq = dist_squared(dx, dy);

            if (dist_sq < separation_radius * separation_radius
                && dist_sq > 0.0f) {

                c.x -= dx / dist_sq;
                c.y -= dy / dist_sq;
            }
        }
    }

    return c;
}