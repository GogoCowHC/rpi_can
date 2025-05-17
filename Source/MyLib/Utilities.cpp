#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include "Utilities.h"
#include "../Config.h"

using namespace std;
using namespace std::chrono;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using time_point_sc = chrono::time_point<chrono::system_clock>;
using time_point_hrc = chrono::time_point<chrono::high_resolution_clock>;

time_point_hrc chronoStart = chrono::high_resolution_clock::now();
time_point_hrc chronoEnd = chrono::high_resolution_clock::now();

void logTimeStart()
{
    chronoStart = chrono::high_resolution_clock::now();
}

void logTimeEnd(std::string message)
{
    chronoEnd = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = chronoEnd - chronoStart;

    cout << message << ": " << duration.count() << " ms" << endl;
}

double getOSTimestamp()
{
    steady_clock::duration now = steady_clock::now().time_since_epoch();
    microseconds us = duration_cast<microseconds>(now);

    return static_cast<double>(us.count()) / 1e6;
}

std::string getCurrentTimeString(const std::string& separator)
{
    // Get current time
    time_point_sc now = system_clock::now();
    std::time_t time_now = system_clock::to_time_t(now);

    // Get milliseconds
    milliseconds ms = chrono::duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Convert to local time
    std::tm local_time;
    localtime_r(&time_now, &local_time);  // POSIX: thread-safe

    // Format time string
    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(2) << local_time.tm_hour << separator
       << std::setw(2) << local_time.tm_min << separator
       << std::setw(2) << local_time.tm_sec << separator
       << std::setw(3) << ms.count();

    return ss.str();
}

std::string getCurrentDateString(const std::string& separator)
{
    // Get current time
    time_point_sc now = system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);

    // Convert to local time
    std::tm local_time;
    localtime_r(&time_now, &local_time);  // POSIX thread-safe

    // Format date string
    std::stringstream ss;
    ss << std::setfill('0')
       << (local_time.tm_year + 1900) << separator            // tm_year = year - 1900
       << std::setw(2) << local_time.tm_mon + 1 << separator  // tm_mon = 0-11 → +1
       << std::setw(2) << local_time.tm_mday;

    return ss.str();
}

std::string generateFilename(const std::string& directory, const std::string& extension)
{
    std::string filename = directory + "/" + getCurrentDateString("_") + "-" + getCurrentTimeString("_") + extension;
    return filename;
}


void ensureDirectoryExists(const std::string& directory) 
{
    struct stat info;
    if (stat(directory.c_str(), &info) != 0) {
        // Directory does not exist, create it
        if (mkdir(directory.c_str(), 0777) == 0) {
            std::cout << "Directory created: " << directory << std::endl;
        } else {
            std::cerr << "Failed to create directory: " << directory << std::endl;
        }
    } else if (!(info.st_mode & S_IFDIR)) {
        std::cerr << "Path exists but is not a directory: " << directory << std::endl;
    } else {
        std::cout << "Directory already exists: " << directory << std::endl;
    }
}
