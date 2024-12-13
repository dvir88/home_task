#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_params.h"

// Function to read and parse the INI file, storing values for each UAV in an array of SimParams
int initializeSimParams(const char* filename, SimParams** params, int* numUav) 
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        perror("Failed to open INI file");
        return -1;  // Return -1 on error opening the file
    }

    char line[256];  // Buffer to store each line from the file
    int uavCount = 0;

    // First, read the number of UAVs (N_uav)
    while (fgets(line, sizeof(line), file) != NULL) 
    {
        // Trim leading spaces and newlines
        char* trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        size_t len = strlen(trimmed);
        if (len > 0 && trimmed[len - 1] == '\n') 
        {
            trimmed[len - 1] = '\0';  // Remove newline at the end of the line
        }

        // Look for "N_uav=" and store the value
        if (strncmp(trimmed, "N_uav=", 6) == 0) 
        {
            *numUav = atoi(trimmed + 6);  // Extract and store N_uav
            break;
        }
    }

    // Dynamically allocate memory for the UAV parameters based on N_uav
    *params = (SimParams*)malloc(sizeof(SimParams) * (*numUav));
    if (*params == NULL) 
    {
        perror("Failed to allocate memory for UAV parameters");
        fclose(file);
        return -2;  // Return -2 if memory allocation fails
    }

    // Initialize the UAV parameters
    for (int i = 0; i < *numUav; i++) 
    {
        // Read the parameters for each UAV
        while (fgets(line, sizeof(line), file) != NULL && uavCount < *numUav) 
        {
            // Trim leading spaces and newlines
            char* trimmed = line;
            while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
            size_t len = strlen(trimmed);
            if (len > 0 && trimmed[len - 1] == '\n') 
            {
                trimmed[len - 1] = '\0';  // Remove newline at the end of the line
            }

            // Process each key-value pair
            if (strncmp(trimmed, "Dt=", 3) == 0) 
            {
                (*params)[uavCount].Dt = atof(trimmed + 3);  // Extract and store Dt
            }
            else if (strncmp(trimmed, "R=", 2) == 0) 
            {
                (*params)[uavCount].R = atof(trimmed + 2);  // Extract and store R
            }
            else if (strncmp(trimmed, "X0=", 3) == 0) 
            {
                (*params)[uavCount].X0 = atof(trimmed + 3);  // Extract and store X0
            }
            else if (strncmp(trimmed, "Y0=", 3) == 0) 
            {
                (*params)[uavCount].Y0 = atof(trimmed + 3);  // Extract and store Y0
            }
            else if (strncmp(trimmed, "Z0=", 3) == 0) 
            {
                (*params)[uavCount].Z0 = atof(trimmed + 3);  // Extract and store Z0
            }
            else if (strncmp(trimmed, "V0=", 3) == 0) 
            {
                (*params)[uavCount].V0 = atof(trimmed + 3);  // Extract and store V0
            }
            else if (strncmp(trimmed, "Az=", 3) == 0) 
            {
                (*params)[uavCount].Az = atof(trimmed + 3);  // Extract and store Az
            }
            else if (strncmp(trimmed, "TimeLim=", 8) == 0) 
            {
                (*params)[uavCount].TimeLim = atof(trimmed + 8);  // Extract and store TimeLim
                uavCount++;
                if (uavCount >= *numUav) 
                    break;

            }

        }
    }

    fclose(file);
    return 0;
}
