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
        state->velocity_x[i] = random_range(-parameters->max_speed, parameters->max_speed);
        state->velocity_y[i] = random_range(-parameters->max_speed, parameters->max_speed);
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

void limit_velocity(float min_speed, float max_speed, BoidsState *state, int boid) {
    float vx = state->velocity_x[boid];
    float vy = state->velocity_y[boid];
    float speed = vector_magnitude(vx, vy);
    // Handle exactly stationary boids.
    if (speed < 1.0e-6f) {
        state->velocity_x[boid] = min_speed;
        state->velocity_y[boid] = 0.0f;
        return;
    }
    float target_speed = speed;
    if (speed < min_speed) {
        target_speed = min_speed;
    }
    if (speed > max_speed) {
        target_speed = max_speed;
    }
    float scale = target_speed / speed;
    state->velocity_x[boid] *= scale;
    state->velocity_y[boid] *= scale;
    return;
}

void update_simulation(const Parameters *parameters, BoidsState *state, BoidsState *new_state, int num_boids, float dt) {
for (int boid = 0; boid < num_boids; boid++) {
        // Current velocity
        Vec2D velocity = {state->velocity_x[boid], state->velocity_y[boid]};
        // Step 1: Calculate desired directions
        Vec2D c_dir = cohesion(state, num_boids, boid, parameters->perception_range);
        Vec2D s_dir = separation(state, num_boids, boid, parameters->separation_radius);
        Vec2D a_dir = alignment(state, num_boids, boid, parameters->perception_range);
        // Step 2: Convert directions into steering accelerations
        Vec2D c = steering_acceleration(c_dir, velocity, parameters->max_speed, parameters->steering_response_time, parameters->max_acceleration);
        Vec2D s = steering_acceleration(s_dir, velocity, parameters->max_speed, parameters->steering_response_time, parameters->max_acceleration);
        Vec2D a = steering_acceleration(a_dir, velocity, parameters->max_speed, parameters->steering_response_time, parameters->max_acceleration);
        // Step 3: Combine the three behaviours
        Vec2D acceleration = {parameters->cohesion_factor * c.x + parameters->separation_factor * s.x + parameters->alignment_factor * a.x,
                                parameters->cohesion_factor * c.y + parameters->separation_factor * s.y + parameters->alignment_factor * a.y};
        // Step 4: Limit total acceleration
        acceleration = clamp_magnitude(acceleration, parameters->max_acceleration);
        // Step 5: Integrate velocity
        new_state->velocity_x[boid] = velocity.x + acceleration.x * dt;
        new_state->velocity_y[boid] = velocity.y + acceleration.y * dt;
        // Step 6: Enforce minimum and maximum speed
        limit_velocity(parameters->min_speed, parameters->max_speed, new_state, boid);
        // Step 7: Integrate position
        new_state->position_x[boid] = state->position_x[boid] + new_state->velocity_x[boid] * dt;
        new_state->position_y[boid] = state->position_y[boid] + new_state->velocity_y[boid] * dt;
        wrap_around_edge(parameters, new_state, boid);
    }
    return;
}