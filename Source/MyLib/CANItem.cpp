#include <iostream>
#include <iomanip>
#include <cstring>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include "CANItem.h"

using namespace std;

void CANItem::print()
{
    cout << "[" << std::fixed << std::setprecision(6) << timestamp << "] "
         << "ID:0x" << std::hex << can_id << " "
         << direction << " "
         << "DLC:" << std::dec << dlc << " DATA:";

    for (std::size_t j = 0; j < data.size(); ++j)
    {
        cout << " " << std::setw(2) << std::setfill('0')
             << std::hex << static_cast<int>(data[j]);
    }

    cout << endl;
}

void setup_can_interface(const char *interface, int bitrate) 
{
    char command[100];

    // Construct the command string
    snprintf(command, sizeof(command), "ip link set %s up type can bitrate %d", interface, bitrate);

    // Execute the command
    int ret = system(command);
    if (ret == -1) {
        perror("Failed to execute command");
    } else {
        cout<<"CAN interface "<<interface<<" set up successfully with bitrate "<<bitrate<<endl;
    }
}


CANItem mcp2515_can_receive(void)
{
    CANItem f;
    int s; 
    sockaddr_can addr;
    ifreq ifr;
    can_frame frame;

    printf("CAN Sockets Demo\r\n");

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket");
    }

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind");
        
    }

    int nbytes = read(s, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
           perror("Read");
       }

    f.can_id = frame.can_id;
    f.dlc = frame.can_dlc;
    f.data.resize(frame.can_dlc);
    memcpy(f.data.data(), frame.data, frame.can_dlc);


   if (close(s) < 0) {
       perror("Close");
   }

   return f;
}