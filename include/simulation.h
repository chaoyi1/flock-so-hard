#ifndef SIMULATION_H
#define SIMULATION_H

#include "boid.h"

typedef struct Parameters Parameters;
struct Parameters {
    float perception_range;
    float separation_radius;
    float cohesion_factor;
    float alignment_factor;
    float speed_limit;
    float x_max;
    float x_min;
    float y_max;
    float y_min;
};
float random_range(float min, float max);
void swap_states(BoidsState *state, BoidsState *new_state);
BoidsState initialise_state(const int num_boids);
void populate_state(const Parameters *parameters, BoidsState *state, const int num_boids);
void free_state(BoidsState *state);
float vector_magnitude(float vecx, float vecy);
void limit_velocity(const float speed_limit, BoidsState *state, int boid);
void update_simulation(const Parameters *parameters, BoidsState *state, BoidsState *new_state, int num_boids, float dt);
void wrap_around_edge(const Parameters *parameters, BoidsState *state, int boid);


#endif