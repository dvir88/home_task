#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "file_read.h"
#include "sim_params.h"
#include "DroneControl.h"
#include <python.h>

void initializeDrones(Drone* drones, SimParams* simParams)
{
    for (int i = 0; i < simParams->N_uav; i++) 
    {
        // Initialize each drone
        drones[i].x = simParams->X0;  // Spread the drones across x-axis
        drones[i].y = simParams->Y0;  // Spread the drones across y-axis
        drones[i].Vx = 0;             // Initial velocity in the x direction
        drones[i].Vy = 0;             // Initial velocity in the y direction
        drones[i].heading = 0;        // Initial heading is 0 degrees
        drones[i].isHovering = 0;     // Not hovering initially
        drones[i].angle = 0.0;        // Initial angle for circular motion
    }
}

int main() 
{
    // Read the simulation parameters from the .ini file
    SimParams* simParams = (SimParams*)malloc(sizeof(SimParams));
    if (simParams == NULL || readIniFile("C:\\Users\\dvir\\source\\repos\\Autonomous_Drone\\drone_external_files\\SimParams.ini", simParams) != 0)
    {
        free(simParams);
        return 1; // Exit if there was an error reading the .ini file
    }

    // Read the simulation commands from the SimCmds.txt file
    SimCommand* commands = (SimCommand*)malloc(sizeof(SimCommand));
    int numCommands = 0;
    if (commands == NULL || readSimCommands("C:\\Users\\dvir\\source\\repos\\Autonomous_Drone\\drone_external_files\\SimCmds.txt", &commands, &numCommands) != 0) {
        
        return 1;  // Exit if there was an error reading the SimCmds.txt file
    }

    // Allocate memory for the drones based on the number of UAVs (N_uav)
    Drone* drones = (Drone*)malloc(simParams->N_uav * sizeof(Drone));
    if (drones == NULL)
    {
        free(simParams);
        free(drones);
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Initialize all drones
    initializeDrones(drones, simParams);

    for (int t = 0; t <= simParams->TimeLim; t++)   // Loop for the time limit specified in the .ini file
    {
        // Apply commands for the current time step
        applyCommandsAtTime(drones, simParams->N_uav, commands, numCommands, t);

        // Control each drone (control to fly straight or hover)
        for (int i = 0; i < simParams->N_uav; i++) 
        {
            char filename[20];
            snprintf(filename, sizeof(filename), "UAV[%d].txt.txt", i + 1);

            if (time_sec == simParams->TimeLim)
            {
                // Format the output string including time
                char output[100];
                snprintf(output, sizeof(output), "Simulation time limit reached: %lf\n", simParams->TimeLim);
                writeToFile("UAV[1].txt.txt", output);
                writeToFile("UAV[2].txt.txt", output);
                writeToFile("UAV[3].txt.txt", output);
                exit(0);  // Exit the loop if the time limit is reached
            }

            if (drones[i].isHovering == 0) 
            {
                controlDroneToFlyStraight(&drones[i], commands->x, commands->y, filename, simParams);
                double distanceToTarget = sqrt(pow(commands->x - drones[i].x, 2) + pow(commands->y - drones[i].y, 2));
                if (distanceToTarget <= simParams->R)
                {
                    drones[i].isHovering = 1;
                }
            }
            else {
                flyInCircleAroundTarget(&drones[i], commands->x, commands->y, filename, simParams);
            }
        }

        // Increment the global time
        time_sec += 1.0;
    }

    // Free the allocated memory for drones and commands
    free(drones);
    free(commands);

   //Calling the python initilization function
    Py_Initialize();

    //Seting the library of the py project
	PySys_SetPath("C:\\Python310\\include");

	//Importing the python script
    PyObject* name, * load_module, * func, * callfunc, * args;

	name = PyUnicode_DecodeFSDefault("DroneControl");
	load_module = PyImport_Import(name);
	func = PyObject_GetAttrString(load_module,(char *)"__main__");
	callfunc = PyObject_GetAttrString(func, NULL);
	double PyDrew_output = PyFloat_AsDouble(callfunc);


    return 0;
}
