#include "simulation.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

void draw_boids(const BoidsState *state, int num_boids,
                float length, float half_width) {
    for (int i = 0; i < num_boids; i++) {
        float x = state->position_x[i];
        float y = state->position_y[i];

        float vx = state->velocity_x[i];
        float vy = state->velocity_y[i];

        // Normalise velocity
        float magnitude = vector_magnitude(vx, vy);

        if (magnitude < 0.000001f) {
            continue;
        }

        float dx = vx / magnitude;
        float dy = vy / magnitude;

        // Perpendicular direction
        float px = -dy;
        float py = dx;

        // Triangle vertices
        Vector2 tip = {
            x + dx * length * (2.0f / 3.0f),
            y + dy * length * (2.0f / 3.0f)
        };

        Vector2 base1 = {
            x - dx * length / 3.0f + px * half_width,
            y - dy * length / 3.0f + py * half_width
        };

        Vector2 base2 = {
            x - dx * length / 3.0f - px * half_width,
            y - dy * length / 3.0f - py * half_width
        };

        DrawTriangle(tip, base2, base1, RAYWHITE);
    }
}

int main(int argc, char const *argv[])
{
    // Window
    const int width = 2560;
    const int height = 1440;

    // Visual scaling
    float visual_scale = fminf(
        width / 2560.0f,
        height / 1440.0f
    );
    float triangle_length = 18.0f * visual_scale;
    float triangle_half_width = 6.0f * visual_scale;

    // Simulation parameters
    int number_of_boids = 1500;
    float perception_range = 160.0f;
    float separation_radius = 40.0f;
    float cohesion_factor = 0.25f;
    float alignment_factor = 1.5f;
    float separation_factor = 1.5f;
    float min_speed = 120.0f;
    float max_speed = 300.0f;
    float max_acceleration = 600.0f;
    float steering_response_time = 0.5f;
    float x_max = 2560.0f;
    float x_min = 0.0f;
    float y_max = 1440.0f;
    float y_min = 0.0f;

    Parameters parameters = {
        .perception_range = perception_range,
        .separation_radius = separation_radius,
        .cohesion_factor = cohesion_factor,
        .alignment_factor = alignment_factor,
        .separation_factor = separation_factor,
        .min_speed = min_speed,
        .max_speed = max_speed,
        .max_acceleration = max_acceleration,
        .steering_response_time = steering_response_time,
        .x_max = x_max,
        .x_min = x_min,
        .y_max = y_max,
        .y_min = y_min
    };
    
    // Initial state (double buffer so two states)
    BoidsState state = initialise_state(number_of_boids);
    BoidsState new_state = initialise_state(number_of_boids);
    populate_state(&parameters, &state, number_of_boids);

    // Time step to decouple logic from framerate
    const int fps = 240;
    const double step = 4.0 / ((double)fps);
    double accumulator = 0.0;

    InitWindow(width, height, "flockin");

    SetTargetFPS(fps);



    while (!WindowShouldClose()) {
        // Variables should be updated here
        // Measure elapsed real time
        double frame_time = GetFrameTime();

        if (frame_time > 0.1) {
            frame_time = 0.1;
        }

        accumulator += frame_time;

        while (accumulator >= step) {
            update_simulation(&parameters, &state, &new_state, number_of_boids, (float)step);
            swap_states(&state, &new_state);
            accumulator -= step;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        draw_boids(
            &state,
            number_of_boids,
            triangle_length,
            triangle_half_width
        );
        EndDrawing();
    }
    free_state(&state);
    free_state(&new_state);
    CloseWindow();
    return 0;
}