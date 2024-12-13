#include <stdio.h>
#include <math.h>
#include "DroneControl.h"
#include "file_read.h"

// Define global time and hover radius variables
double time_sec = 0.0;  // Start time at 0 seconds


void writeToFile(const char* filename, const char* data) 
{
    FILE* file = fopen(filename, "a");  // Open the file in append mode
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        return;
    }
    fprintf(file, "%s\n", data);  // Write data to the file
    fclose(file);
}

// Function to calculate the desired heading (direction) to the target point (target_x, target_y)
double calculateDesiredHeading(double x, double y, double target_x, double target_y) 
{
    double deltaX = target_x - x;
    double deltaY = target_y - y;
    double angle = atan2(deltaY, deltaX);  // Angle in radians
    return RAD_TO_DEG(angle);  // Convert to degrees
}

// Function to control the drone to follow a straight path towards the target
void controlDroneToFlyStraight(Drone* drone, double target_x, double target_y, const char* filename, SimParams* simParams)
{
    // Check if the drone is within the hover radius
    double distanceToTarget = sqrt(pow(target_x - drone->x, 2) + pow(target_y - drone->y, 2));
    if (distanceToTarget <= simParams->R)
    {
        // If within the hover radius, call hover function
        flyInCircleAroundTarget(drone, target_x, target_y, filename, simParams);
        return;  // Exit the function once hovering
    }

    // Calculate the desired heading to the target point
    double desiredHeading = calculateDesiredHeading(drone->x, drone->y, target_x, target_y);

    // Normalize the headings to be between -180 and 180 degrees
    double headingDifference = desiredHeading - drone->heading;
    if (headingDifference > 180) 
    {
        headingDifference -= 360;
    }
    else if (headingDifference < -180) 
    {
        headingDifference += 360;
    }

    // Adjust the drone's heading based on the heading difference
    double correction = 0.1 * headingDifference;  // Proportional control, 0.1 is the gain
    drone->heading += correction;

    // Ensure the heading stays within the -180 to 180 degrees range
    if (drone->heading > 180) 
    {
        drone->heading -= 360;
    }
    else if (drone->heading < -180) 
    {
        drone->heading += 360;
    }

    // Update the velocity components (Vx, Vy) based on the new heading
    double speed = sqrt(drone->Vx * drone->Vx + drone->Vy * drone->Vy); // Calculate speed (magnitude of velocity)
    drone->Vx = speed * cos(DEG_TO_RAD(drone->heading));  // Update Vx based on the new heading
    drone->Vy = speed * sin(DEG_TO_RAD(drone->heading));  // Update Vy based on the new heading

    // Increment the global time variable by 1 second for each call
    time_sec += 1.0;  // Assuming the control function runs every second

    // Format the output string including time
    char output[256];
    snprintf(output, sizeof(output), "Time: %.2f sec, Corrected Heading: %.2f degrees, Current drone location: (%.2f, %.2f), Drone's new velocity: Vx = %.2f, Vy = %.2f",
        time_sec, drone->heading, drone->x, drone->y ,drone->Vx, drone->Vy);

    writeToFile(filename, output);
}

// Function to make the drone fly in a circle around the target
void flyInCircleAroundTarget(Drone* drone, double target_x, double target_y, const char* filename, SimParams* simParams)
{
	drone->isHovering = 1;  // Set the hovering flag to 1
    // Calculate the drone's position on the circle
    // The drone will move in a circle by incrementing its angle
    drone->angle += 0.1;  // Increment the angle by 0.1 radians for each time step (controls speed of circular motion)

    if (drone->angle > 2 * PI) 
    {
        drone->angle -= 2 * PI;  // Reset angle if it exceeds 2π to maintain circular motion
    }

    // Calculate new x and y positions based on the angle and hover radius
    drone->x = target_x + simParams->R * cos(drone->angle);
    drone->y = target_y + simParams->R * sin(drone->angle);

    // Set the drone's velocity to move tangentially around the circle
    drone->Vx = -0.1 * simParams->R * sin(drone->angle);  // Tangential velocity in x direction
    drone->Vy = 0.1 * simParams->R * cos(drone->angle);   // Tangential velocity in y direction

    // Increment the global time variable by 1 second for each call
    time_sec += 1.0;

    // Format the output string including time and position
    char output[256];
    snprintf(output, sizeof(output), "Time: %.2f sec, Drone is orbiting around target at (%.2f, %.2f), Vx = %.2f, Vy = %.2f",
        time_sec, drone->x, drone->y, drone->Vx, drone->Vy);

    writeToFile(filename, output);
}
