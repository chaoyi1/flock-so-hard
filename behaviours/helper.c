#include "behaviours.h"
#include "math.h"

float dist_squared(float posx, float posy) {
    return posx * posx + posy * posy;
}


Vec2D clamp_magnitude(Vec2D vector, float max_magnitude) {
    float mag_sq = dist_squared(vector.x, vector.y);
    if (mag_sq > max_magnitude * max_magnitude) {
        float scale = max_magnitude / sqrtf(mag_sq);
        vector.x *= scale;
        vector.y *= scale;
    }
    return vector;
}

Vec2D steering_acceleration(Vec2D desired_direction, Vec2D current_velocity, float max_speed, float response_time, float max_acceleration) {
    float length_sq = dist_squared(desired_direction.x,desired_direction.y);
    // No desired direction means no steering.
    if (length_sq < 1.0e-12f) {
        return (Vec2D){0};
    }
    // Convert desired direction into desired velocity.
    float scale = max_speed / sqrtf(length_sq);
    Vec2D desired_velocity = { desired_direction.x * scale, desired_direction.y * scale};
    // Calculate acceleration needed to change velocity.
    Vec2D acceleration = { (desired_velocity.x - current_velocity.x) / response_time, (desired_velocity.y - current_velocity.y) / response_time};
    // Limit how aggressively the boid can steer.
    return clamp_magnitude(acceleration, max_acceleration);
}