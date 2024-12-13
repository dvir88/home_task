#ifndef FILE_READ_H
#define FILE_READ_H

#include "DroneControl.h"
#include "sim_params.h"

// Function to read data from a TXT file and display it as a table
int readIniFile(const char* filename, SimParams* simParams);

void applyCommandsAtTime(Drone* drones, int numDrones, SimCommand* commands, int numCommands, double currentTime);



#endif // FILE_READ_H
