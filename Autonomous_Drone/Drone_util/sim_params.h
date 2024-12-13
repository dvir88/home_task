#ifndef SIM_PARAMS_H
#define SIM_PARAMS_H

// Structure to store simulation parameters for each UAV
typedef struct 
{
    double Dt;
    int N_uav;
    double R;
    double X0;
    double Y0;
    double Z0;
    double V0;
    double Az;
    double TimeLim;
} SimParams;

// Command structure to hold time, drone number (id), and new position (x, y)
typedef struct 
{
    double time;  // Time at which the command should be executed
    int num;   // Drone number (id)
    double x;  // Target x-coordinate
    double y;  // Target y-coordinate
} SimCommand;

// Function to initialize parameters for multiple UAVs by reading from the INI file
int initializeSimParams(const char* filename, SimParams** params, int* numUav);

#endif // SIM_PARAMS_H
