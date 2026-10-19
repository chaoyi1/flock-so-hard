#include "boid.h"
#include "behaviours.h"
#include "simulation.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

float random_range(float min, float max) {
    float r = (float)rand() / RAND_MAX;
    return min + r * (max - min);
}

void swap_states(BoidsState *state, BoidsState *new_state) {
    BoidsState temp = *state;
    *state = *new_state;
    *new_state = temp;
}

BoidsState initialise_state(const int num_boids) {
    float *positions_x = malloc(sizeof(float) * num_boids);
    float *positions_y = malloc(sizeof(float) * num_boids);
    float *velocities_x = malloc(sizeof(float) * num_boids);
    float *velocities_y = malloc(sizeof(float) * num_boids);

    BoidsState state = {
        .position_x = positions_x,
        .position_y = positions_y,
        .velocity_x = velocities_x,
        .velocity_y = velocities_y
    };

    return state;
}

void populate_state(const Parameters *parameters, BoidsState *state, const int num_boids) {
    for (int i = 0; i < num_boids; i++) {
        state->position_x[i] = random_range(parameters->x_min, parameters->x_max);
        state->position_y[i] = random_range(parameters->y_min, parameters->y_max);
        state->velocity_x[i] = random_range(-parameters->speed_limit, parameters->speed_limit);
        state->velocity_y[i] = random_range(-parameters->speed_limit, parameters->speed_limit);
    }
    return;
}

void free_state(BoidsState *state) {
    free(state->position_x);
    free(state->position_y);
    free(state->velocity_x);
    free(state->velocity_y);

    return;
}

void wrap_around_edge(const Parameters *parameters, BoidsState *state, int boid) 
{
    float posx = state->position_x[boid];
    float posy = state->position_y[boid];
    float x_max = parameters->x_max;
    float x_min = parameters->x_min;
    float y_max = parameters->y_max;
    float y_min = parameters->y_min;
    float world_width = x_max - x_min;
    float world_height = y_max - y_min;
    if (posx >= x_max) {
        posx -= world_width;
        state->position_x[boid]= posx;
    }
    else if (posx < x_min) {
        posx += world_width;
        state->position_x[boid] = posx;
    }
    if (posy >= y_max) {
        posy -= world_height;
        state->position_y[boid] = posy;
    }
    else if (posy < y_min) {
        posy += world_height;
        state->position_y[boid] = posy;
    }
    return;
}

float vector_magnitude(float vecx, float vecy) {
    return sqrtf(vecx * vecx + vecy * vecy);
}

void limit_velocity(const float speed_limit, BoidsState *state, int boid) {
    float velx = state->velocity_x[boid];
    float vely = state->velocity_y[boid];
    float speed = vector_magnitude(velx, vely);
    if (speed > speed_limit) {
        state->velocity_x[boid] = speed_limit * state->velocity_x[boid] / speed;
        state->velocity_y[boid] = speed_limit * state->velocity_y[boid] / speed;
    }
    return;
}

void update_simulation(const Parameters *parameters, BoidsState *state, BoidsState *new_state, int num_boids, float dt) {
    Vec2D v1, v2, v3;
    for (int boid = 0; boid < num_boids; boid++) {
        v1 = cohesion(state, num_boids, boid, parameters->cohesion_factor, parameters->perception_range);
        v2 = separation(state, num_boids, boid, parameters->separation_radius);
        v3 = alignment(state, num_boids, boid, parameters->alignment_factor, parameters->perception_range);

        new_state->velocity_x[boid] = state->velocity_x[boid] + (v1.x + v2.x + v3.x)*dt;
        new_state->velocity_y[boid] = state->velocity_y[boid] + (v1.y + v2.y + v3.y)*dt;
        limit_velocity(parameters->speed_limit, new_state, boid);

        new_state->position_x[boid] = state->position_x[boid] + new_state->velocity_x[boid]*dt;
        new_state->position_y[boid] = state->position_y[boid] + new_state->velocity_y[boid]*dt;

        wrap_around_edge(parameters, new_state, boid);
    }
    return;
}