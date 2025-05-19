#include <iostream>
#include <iomanip>
#include <random>
#include <thread>
#include <chrono>
#include <sstream> 
#include "Config.h"
#include "Utilities.h"
#include "RecordMngr.h"
#include "CANItem.h"

using namespace std;

#ifdef SIMULATION
CANItem generateRandomCANFrame();
std::string generateRandomGPSData();

void simulate_receive_CAN_frame(RecordMngr& recordMngr, int minutes);
void simulate_receive_GPS_data(RecordMngr& recordMngr, int minutes);
#endif //SIMULATION

void rpi_receive_CAN_frame(RecordMngr& recordMngr);
void rpi_receive_GPS_data(RecordMngr& recordMngr);

int main()
{
    RecordMngr& recordMngr = RecordMngr::getInstance();
    // recordMngr.init(MAX_NUMBER_OF_RECORDS);
    recordMngr.init(100);
    cout << "size: " << recordMngr.getSize() << endl;

    #ifdef SIMULATION
    
    cout << "========== SIMULATION ==========" << endl;

    std::thread t_CAN(simulate_receive_CAN_frame, std::ref(recordMngr), SIMULATION_DURATION);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::thread t_GPS(simulate_receive_GPS_data, std::ref(recordMngr), SIMULATION_DURATION);

    t_CAN.join();
    t_GPS.join();

    while (recordMngr.isWritingFile())
    {
        cout << "Wait for writing file..." << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    #else

    cout << "========== HARDWARE ==========" << endl;
    setup_can_interface("can0", 500000);


    std::thread t_CAN(rpi_receive_CAN_frame, std::ref(recordMngr));
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::thread t_GPS(rpi_receive_GPS_data, std::ref(recordMngr));

    t_CAN.join();
    t_GPS.join();

    while (recordMngr.isWritingFile())
    {
        cout << "Wait for writing file..." << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    #endif

    return 0;
}

#ifdef SIMULATION
CANItem generateRandomCANFrame()
{
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<uint32_t> canIdDist(0x000, 0x1FFFFFFF);   // 29-bit ID
    std::uniform_int_distribution<int> dlcDist(1, 8);
    std::uniform_int_distribution<int> byteDist(0, 255);
    std::uniform_int_distribution<int> dirDist(0, 1);                       // 0 = Rx, 1 = Tx

    CANItem frame;
    frame.channel = 1;
    frame.can_id = canIdDist(rng);
    frame.dlc = dlcDist(rng);
    frame.direction = dirDist(rng) == 0 ? "Rx" : "Tx";

    // Random frame data
    frame.data.clear();
    for (int i = 0; i < frame.dlc; ++i)
    {
        frame.data.push_back(static_cast<uint8_t>(byteDist(rng)));
    }

    return frame;
}

std::string generateRandomGPSData()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
   
    // Random latitude and longitude in Vietname
    std::uniform_real_distribution<> latDist(8.0, 23.5);
    std::uniform_real_distribution<> lonDist(102.0, 109.5);

    double latitude = latDist(gen);
    double longitude = lonDist(gen);

    // Convert to radian (ddmm.mmmm)
    int latDeg = static_cast<int>(latitude);
    double latMin = (latitude - latDeg) * 60.0;

    int lonDeg = static_cast<int>(longitude);
    double lonMin = (longitude - lonDeg) * 60.0;

    // Create GGA string
    std::ostringstream oss;
    oss << "$GPGGA,123519,"
        << std::setw(2) << std::setfill('0') << latDeg
        << std::fixed << std::setprecision(4) << latMin
        << ",N,"
        << std::setw(3) << std::setfill('0') << lonDeg
        << std::fixed << std::setprecision(4) << lonMin
        << ",E,1,08,0.9,545.4,M,46.9,M,,*47";

    return oss.str();
}

void simulate_receive_CAN_frame(RecordMngr& recordMngr, int minutes)
{
    auto start_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::minutes(minutes);

    while (std::chrono::steady_clock::now() - start_time < duration)
    {
        CANItem f = generateRandomCANFrame();
        recordMngr.addCANRecord(f.channel, f.can_id, f.direction, f.dlc, f.data);

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    std::cout << "Thread finished after " << minutes << " minutes" << endl;
}

void simulate_receive_GPS_data(RecordMngr& recordMngr, int minutes)
{
    auto start_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::minutes(minutes);

    while (std::chrono::steady_clock::now() - start_time < duration)
    {
        recordMngr.addGPSRecord(generateRandomGPSData());

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    std::cout << "Thread finished after " << minutes << " minutes" << endl;
}
#endif //SIMULATION

void rpi_receive_CAN_frame(RecordMngr& recordMngr)
{
    CANItem f;

    while (true)
    {
        f = mcp2515_can_receive();
        if (f.can_id != 0)
        {
            recordMngr.addCANRecord(f.channel, f.can_id, f.direction, f.dlc, f.data);
            
        }
    }
    
}


void rpi_receive_GPS_data(RecordMngr& recordMngr)
{
    while (true)
    {
        recordMngr.addGPSRecord(rpi_getGPSData);
    }

}