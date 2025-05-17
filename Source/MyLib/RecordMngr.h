#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_

#include <vector>
#include <string>
#include "CANItem.h"
#include "GPSItem.h"

class RecordMngr
{
    private:
        int lstSize = 0;

        int idxCANItem = 0;
        int idxCANAdding = 0;
        int idxCANWriting = 1;
        bool isCANWriting = false;

        int idxGPSItem = 0;
        int idxGPSAdding = 0;
        int idxGPSWriting = 1;
        bool isGPSWriting = false;

        std::vector<std::vector<CANItem>> lstCANRecords;
        std::vector<std::vector<GPSItem>> lstGPSRecords;

        // Private constructor to enforce Singleton
        RecordMngr() {};

        // Delete copy constructor and assignment
        RecordMngr(const RecordMngr&) = delete;
        RecordMngr& operator=(const RecordMngr&) = delete;

    public:
        static RecordMngr& getInstance();

        void init(int size);
        int getSize() { return lstSize; }
        
        void addCANRecord(const int _channel, const uint32_t _can_id, const std::string& _direction, const int _dlc, const std::vector<uint8_t>& _data);
        void addGPSRecord(const std::string& _data);

        bool writeCANFile();
        bool writeGPSFile();
        bool isWritingFile() { return (isCANWriting == true || isGPSWriting == true); }
};

#endif //_RECORD_MANAGER_H_