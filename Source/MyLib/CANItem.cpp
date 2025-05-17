#include <iostream>
#include <iomanip>
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