// CanPacketManager.h

#ifndef CAN_PACKET_MANAGER_H
#define CAN_PACKET_MANAGER_H

#include <Arduino.h>
#include <CAN.h>

class CanPacketManager {
private:
    int arraySize;
    const char SEPARATOR = ',';
    const char TERMINATOR = '\n';
    typedef struct {
            long id;
            byte rtr;
            byte ide;
            byte dlc;
            byte dataArray[20];
        } packet_t;
        packet_t *packetArray;
public:
    CanPacketManager();
    CanPacketManager(double CanSpeed);
    ~CanPacketManager();
    
    byte getFrame(long id, int byteNo);
    void setCanSpeed(unsigned long speed);
    void printPacket(packet_t *packet);
    void addOrUpdatePacket(packet_t *newPacket);
    void sendPacketToCan(packet_t *packet);
    void onCANReceive(int packetSize);
    char getNum(char c);
    char *strToHex(char *str, byte *hexArray, byte *len);
    void rxParse(char *buf, int len);
    void RXcallback();
    void CANsimulate();
};

#endif // CAN_PACKET_MANAGER_H
