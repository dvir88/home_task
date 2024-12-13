#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_read.h"
#include "sim_params.h"
#include "DroneControl.h"

#define MAX_LINE_LENGTH 256

// Comparison function for sorting by time (ascending order)
int compareByTime(const void* a, const void* b) {
    SimCommand* commandA = (SimCommand*)a;
    SimCommand* commandB = (SimCommand*)b;

    // Compare based on time
    if (commandA->time < commandB->time) return -1;
    if (commandA->time > commandB->time) return 1;
    return 0;
}


// Function to read the .ini file and initialize parameters
int readIniFile(const char* filename, SimParams* simParams)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error opening ini file: %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "N_uav", 5) == 0) 
        {
            sscanf(line, "N_uav = %d", &simParams->N_uav);
        }
        else if (strncmp(line, "R", 1) == 0) 
        {
            sscanf(line, "R = %lf", &simParams->R);
        }
        else if (strncmp(line, "X", 1) == 0) 
        {
            sscanf(line, "X0 = %lf", &simParams->X0);
        }
        else if (strncmp(line, "Y", 1) == 0) 
        {
            sscanf(line, "Y0 = %lf", &simParams->Y0);
        }
        else if (strncmp(line, "Dt", 2) == 0) 
        {
            sscanf(line, "Dt = %lf", &simParams->Dt);
        }
        else if (strncmp(line, "Z", 1) == 0)
        {
            sscanf(line, "Z0 = %lf", &simParams->Z0);
        }
        else if (strncmp(line, "V", 1) == 0)
        {
            sscanf(line, "V0 = %lf", &simParams->V0);
        }
        else if (strncmp(line, "Az", 1) == 0)
        {
            sscanf(line, "Az = %lf", &simParams->Az);
        }
        else if (strncmp(line, "TimeLim", 2) == 0)
        {
            sscanf(line, "TimeLim = %lf", &simParams->TimeLim);
        }
    }

    fclose(file);
    return 0;
}

// Function to read the SimCmds.txt file and store commands
int readSimCommands(const char* filename, SimCommand** commands, int* numCommands) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error opening SimCmds.txt\n");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int commandCount = 0;

    // count the number of commands
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "time", 4) != 0 && line[0] != '#' && line[0] != '\n') // Skip header, comments, and empty lines
        {  
            commandCount++;
        }
    }
    rewind(file); // Go back to the start of the file

    // Allocate memory to store commands
    *commands = (SimCommand*)malloc(commandCount * sizeof(SimCommand));
    if (*commands == NULL) 
    {
        fprintf(stderr, "Memory allocation failed for SimCommand array.\n");
        fclose(file);
        return -1;
    }

    int i = 0;
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "time", 4) != 0 && line[0] != '#' && line[0] != '\n')
        {  
            if (sscanf(line, "%lf %d %lf %lf", &(*commands)[i].time, &(*commands)[i].num, &(*commands)[i].x, &(*commands)[i].y) == 4)
			{
				i++;
			}
            else
            {
                fprintf(stderr, "Error parsing line: %s", line); // Handle invalid line format
            }
        }
    }

    fclose(file);

    // Sort the commands by time in ascending order
    qsort(*commands, commandCount, sizeof(SimCommand), compareByTime);

    *numCommands = commandCount; // Set the number of commands
    return 0;
}

// Function to apply commands to drones at the correct simulation time
void applyCommandsAtTime(Drone* drones, int numDrones, SimCommand* commands, int numCommands, double currentTime) 
{
    for (int i = 0; i < numCommands; i++) 
    {
        if (commands[i].time == currentTime) 
        {
            int droneIndex = commands[i].num;  // Assuming drone numbers start from 1
            if (droneIndex >= 0 && droneIndex < numDrones) 
            {
                drones[droneIndex].x = commands[i].x;
                drones[droneIndex].y = commands[i].y;
            }
        }
    }
}
