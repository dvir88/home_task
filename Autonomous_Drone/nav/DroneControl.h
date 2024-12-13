#ifndef DRONE_CONTROL_H
#define DRONE_CONTROL_H

#include <sim_params.h>

// Defines
#define PI 3.14159265358979323846
#define DEG_TO_RAD(x) ((x) * PI / 180.0)
#define RAD_TO_DEG(x) ((x) * 180.0 / PI)


typedef struct 
{
    double x, y;       // Position (x, y)
    double Vx, Vy;     // Velocity components (Vx, Vy)
    double heading;    // Current heading (yaw) in degrees
    int isHovering;    // Flag to indicate whether the drone is hovering or not
    double angle;      // Angle in the circular path (radians)
} Drone;

// Global time variable (in seconds)
extern double time_sec;


// Function to calculate the desired heading (direction) to the target point (target_x, target_y)
double calculateDesiredHeading(double x, double y, double target_x, double target_y);

// Function to control the drone to follow a straight path towards the target
void controlDroneToFlyStraight(Drone* drone, double target_x, double target_y, const char* filename, SimParams* simParams);

void writeToFile(const char* filename, const char* data);

// Function to make the drone fly in a circle around the target
void flyInCircleAroundTarget(Drone* drone, double target_x, double target_y, const char* filename, SimParams* simParams);

#endif // DRONE_CONTROL_H
