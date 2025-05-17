#ifndef _CAN_ITEM_H_
#define _CAN_ITEM_H_

#include <string>
#include <vector>

class CANItem
{
    public:
        double timestamp;           // Second, e.g. 0.123456
        int channel;                // Usually 1
        uint32_t can_id;            // Hex CAN ID
        std::string direction;      // Rx or Tx
        int dlc;                    // Data Length Code
        std::vector<uint8_t> data;  // Payload

        CANItem() {};
        ~CANItem() {};

        void print();
};

#endif //_CAN_ITEM_H_