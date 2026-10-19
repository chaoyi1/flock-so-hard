#include "behaviours.h"


Vec2D alignment(BoidsState *state, int num_boids, int boid, float perception_range) {
    Vec2D vi = { 0 };
    int neighbour_count = 0;
    for(int i = 0; i < num_boids; i++) {
        if (i != boid) {
            float dx = state->position_x[i] - state->position_x[boid];
            float dy = state->position_y[i] - state->position_y[boid];
            if (dist_squared(dx, dy) <= perception_range * perception_range) {
                vi.x += state->velocity_x[i];
                vi.y += state->velocity_y[i];
                neighbour_count += 1;
            }
        }
    }
    if (neighbour_count > 0) {
        vi.x /= neighbour_count;
        vi.y /= neighbour_count;
    }

    return vi;
}