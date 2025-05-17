#ifndef _CONFIG_H_
#define _CONFIG_H_

#define SIMULATION
#define SIMULATION_DURATION 1

#define MAX_NUMBER_OF_RECORDS 100000

#ifdef SIMULATION
#define LOG_DIRECTORY "/Users/Temp"
#else
#define LOG_DIRECTORY "???"
#endif

#define LOG_FILE_CAN_EXT ".asc"
#define LOG_FILE_GPS_EXT ".gps"

#endif //_CONFIG_H_