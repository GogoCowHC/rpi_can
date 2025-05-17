#include <iostream>
#include <thread>
#include <iomanip>
#include <fstream>
#include "RecordMngr.h"
#include "../Config.h"
#include "Utilities.h"

using namespace std;

RecordMngr& RecordMngr::getInstance()
{
    static RecordMngr instance;
    return instance;
}

void RecordMngr::init(int size)
{
    lstSize = size;

    idxCANItem = 0;
    idxCANAdding = 0;
    idxCANWriting = 1;
    isCANWriting = false;

    idxGPSItem = 0;
    idxGPSAdding = 0;
    idxGPSWriting = 1;
    isGPSWriting = false;

    lstCANRecords.resize(2);
    lstCANRecords[idxCANAdding].clear();
    lstCANRecords[idxCANWriting].clear();
    
    lstGPSRecords.resize(2);
    lstGPSRecords[idxGPSAdding].clear();
    lstGPSRecords[idxGPSWriting].clear();

    try
    {
        for (int i = 0; i < lstSize; i++)
        {
            lstCANRecords[idxCANAdding].emplace_back();
            lstCANRecords[idxCANWriting].emplace_back();

            lstGPSRecords[idxGPSAdding].emplace_back();
            lstGPSRecords[idxGPSWriting].emplace_back();
        }
    }
    catch (const std::bad_alloc& e)
    {
        cerr << "OUT OF MEMORRY !!!";
    }
}

void RecordMngr::addCANRecord(const int _channel, const uint32_t _can_id, const std::string& _direction, const int _dlc, const std::vector<uint8_t>& _data)
{
    if (idxCANItem >= lstSize)
    {
        if (isCANWriting == true)
        {
            cerr << "BUSY WITH WRITING CAN FILE !!!" << endl;
            return;
        }

        idxCANItem = 0;

        if (idxCANAdding == 0)
        {
            idxCANAdding = 1;
            idxCANWriting = 0;
        }
        else if (idxCANAdding == 1)
        {
            idxCANAdding = 0;
            idxCANWriting = 1;
        }

        cout << "Swapped CAN list" << endl;

        std::thread t(&RecordMngr::writeCANFile, this);
        t.detach();
    }

    lstCANRecords[idxCANAdding][idxCANItem].timestamp = getOSTimestamp();
    lstCANRecords[idxCANAdding][idxCANItem].channel = _channel;
    lstCANRecords[idxCANAdding][idxCANItem].can_id = _can_id;
    lstCANRecords[idxCANAdding][idxCANItem].direction = _direction;
    lstCANRecords[idxCANAdding][idxCANItem].dlc = _dlc;

    lstCANRecords[idxCANAdding][idxCANItem].data.clear();
    for (int i = 0; i < lstCANRecords[idxCANAdding][idxCANItem].dlc; ++i)
    {
        lstCANRecords[idxCANAdding][idxCANItem].data.push_back(_data[i]);
    }

    idxCANItem++;
    if (idxCANItem > 0 && idxCANItem % 10000 == 0)
    {
        cout << "CAN" << idxCANItem << endl;
    }
}

void RecordMngr::addGPSRecord(const std::string& _data)
{
    if (idxGPSItem >= lstSize)
    {
        if (isGPSWriting == true)
        {
            cerr << "BUSY WITH WRITING GPS FILE !!!" << endl;
            return;
        }

        idxGPSItem = 0;

        if (idxGPSAdding == 0)
        {
            idxGPSAdding = 1;
            idxGPSWriting = 0;
        }
        else if (idxGPSAdding == 1)
        {
            idxGPSAdding = 0;
            idxGPSWriting = 1;
        }

        cout << "Swapped GPS list" << endl;

        std::thread t(&RecordMngr::writeGPSFile, this);
        t.detach();
    }

    lstGPSRecords[idxGPSAdding][idxGPSItem].timestamp = getOSTimestamp();
    lstGPSRecords[idxGPSAdding][idxGPSItem].data = _data;

    idxGPSItem++;
    if (idxGPSItem > 0 && idxGPSItem % 10000 == 0)
    {
        cout << "GPS" << idxGPSItem << endl;
    }
}

bool RecordMngr::writeCANFile()
{
    isCANWriting = true;

    std::string filename = generateFilename(LOG_DIRECTORY, LOG_FILE_CAN_EXT);
    
    std::ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Write CAN file error: " << filename << endl;
        isCANWriting = false;

        return false;
    }
    
    cout << "Write CAN file: " << filename << endl;

    // Header
    file << "date " << getCurrentDateString("-") << " time " << getCurrentTimeString(":") << endl;
    file << "base hex timestamps absolute" << endl;
    file << endl;

    // Frames
    for (const CANItem& item : lstCANRecords[idxCANWriting])
    {
        file << std::fixed << std::setprecision(6)
             << std::setw(10) << item.timestamp << " "
             << item.channel << " ";

        file << std::hex << std::uppercase
             << std::setw(8) << std::setfill(' ') << item.can_id << " ";

        file << item.direction << " D "
             << std::dec << item.dlc;

        for (int i = 0; i < item.dlc; ++i)
        {
            file << "  " << std::setw(2) << std::setfill('0')
                 << std::hex << static_cast<int>(item.data[i]);
        }

        file << endl;
    }

    file.close();

    isCANWriting = false;
    cout << "Write CAN file => DONE" << endl;

    return true;
}

bool RecordMngr::writeGPSFile()
{
    isCANWriting = true;

    std::string filename = generateFilename(LOG_DIRECTORY, LOG_FILE_GPS_EXT);
    
    std::ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Write GPS file error: " << filename << endl;
        isGPSWriting = false;

        return false;
    }
    
    cout << "Write GPS file: " << filename << endl;

    for (const GPSItem& item : lstGPSRecords[idxGPSWriting])
    {
        file << std::fixed << std::setprecision(6)
             << std::setw(10) << item.timestamp << " "
             << item.data << endl;
    }

    file.close();

    isGPSWriting = false;
    cout << "Write GPS file => DONE" << endl;

    return true;
}