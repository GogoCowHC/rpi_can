#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "GPSItem.h"

using namespace std;

void GPSItem::print()
{
    cout << "[" << std::fixed << std::setprecision(6) << timestamp << "] "
         << data << endl;
};

std::string rpi_getGPSData(void)
{
    std::ostringstream oss;
    int fd;
    char c;
    unsigned int index = 0;
    printf("Rpi is receiving : \n");      

    if((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0 ){
        oss<<  "Unable to open serial device!!\n";
        
    }else{
        do{
            oss << serialGetchar(fd);            
        }while(serialDataAvail(fd));
        
        serialClose(fd);
    }

    return oss.str();
}