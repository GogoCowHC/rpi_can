#ifndef _GPS_ITEM_H_
#define _GPS_ITEM_H_

#include <string>

class GPSItem
{
    public:
        double timestamp;   // Second, e.g. 0.123456
        std::string data;

        GPSItem() {};
        ~GPSItem() {};

        void print();
};

#endif //_GPS_ITEM_H_