#include <iostream>
#include <iomanip>
#include "GPSItem.h"

using namespace std;

void GPSItem::print()
{
    cout << "[" << std::fixed << std::setprecision(6) << timestamp << "] "
         << data << endl;
};