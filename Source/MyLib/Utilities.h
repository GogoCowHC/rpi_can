#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <string>
#include <ctime>

void logTimeStart();
void logTimeEnd(std::string message);

double getOSTimestamp();

std::string getCurrentTimeString(const std::string& separator);
std::string getCurrentDateString(const std::string& separator);

std::string generateFilename(const std::string& directory, const std::string& extension);

#endif //_UTILITIES_H_