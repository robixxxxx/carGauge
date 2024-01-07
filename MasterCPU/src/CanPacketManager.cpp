// CanPacketManager.cpp

#include "CanPacketManager.h"

CanPacketManager::CanPacketManager() : packetArray(NULL), arraySize(0) {
    // Default CAN speed is 500 kbps
    setCanSpeed(500E3);
}

CanPacketManager::CanPacketManager(double CanSpeed) : packetArray(NULL), arraySize(0) {
    // Default CAN speed is 500 kbps
    setCanSpeed(CanSpeed);
}

CanPacketManager::~CanPacketManager() {
    if (packetArray != NULL) {
        free(packetArray);
    }
}

void CanPacketManager::setCanSpeed(unsigned long speed) {
    if (!CAN.begin(speed)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }
    
}

void CanPacketManager::printPacket(packet_t *packet) {
    Serial.print(packet->id);
    Serial.print(SEPARATOR);

    Serial.print(packet->rtr);
    Serial.print(SEPARATOR);

    Serial.print(packet->ide);
    Serial.print(SEPARATOR);

    for (int i = 0; i < packet->dlc; i++) {
        Serial.print(packet->dataArray[i]);
    }

    Serial.print(TERMINATOR);
}

void CanPacketManager::addOrUpdatePacket(packet_t *newPacket) {
    for (int i = 0; i < arraySize; i++) {
        if (packetArray[i].id == newPacket->id) {
            // Update existing packet
            packetArray[i] = *newPacket;
            return;
        }
    }

    // Add new packet to the array
    arraySize++;
    packetArray = (packet_t *)realloc(packetArray, arraySize * sizeof(packet_t));
    packetArray[arraySize - 1] = *newPacket;
}

void CanPacketManager::sendPacketToCan(packet_t *packet) {
    for (int retries = 10; retries > 0; retries--) {
        bool rtr = packet->rtr ? true : false;
        if (packet->ide) {
            CAN.beginExtendedPacket(packet->id, packet->dlc, rtr);
        } else {
            CAN.beginPacket(packet->id, packet->dlc, rtr);
        }
        CAN.write(packet->dataArray, packet->dlc);
        if (CAN.endPacket()) {
            // success
            break;
        } else if (retries <= 1) {
            return;
        }
    }
}

void CanPacketManager::onCANReceive(int packetSize) {
    packet_t rxPacket;
    rxPacket.id = CAN.packetId();
    rxPacket.rtr = CAN.packetRtr() ? 1 : 0;
    rxPacket.ide = CAN.packetExtended() ? 1 : 0;
    rxPacket.dlc = CAN.packetDlc();
    byte i = 0;
    while (CAN.available()) {
        rxPacket.dataArray[i++] = CAN.read();
        if (i >= sizeof(rxPacket.dataArray) / sizeof(rxPacket.dataArray[0])) {
            break;
        }
    }
    addOrUpdatePacket(&rxPacket);
    printPacket(&rxPacket);
}

byte CanPacketManager::getFrame(long id, int byteNo){
    for (int i = 0; i < arraySize; i++) {
        if (packetArray[i].id == id) {
            // Update existing packet
            return packetArray[i].dataArray[byteNo];
        }
    }
    return 0;
}

char CanPacketManager::getNum(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }
    return 0;
}

char *CanPacketManager::strToHex(char *str, byte *hexArray, byte *len) {
    byte *ptr = hexArray;
    char *idx;
    for (idx = str; *idx != SEPARATOR && *idx != TERMINATOR; ++idx, ++ptr) {
        *ptr = (getNum(*idx++) << 4) + getNum(*idx);
    }
    *len = ptr - hexArray;
    return idx;
}

void CanPacketManager::rxParse(char *buf, int len) {
    packet_t rxPacket;
    char *ptr = buf;

    // ID
    byte idTempArray[8], tempLen;
    ptr = strToHex(ptr, idTempArray, &tempLen);
    rxPacket.id = 0;
    for (int i = 0; i < tempLen; i++) {
        rxPacket.id |= (long)idTempArray[i] << ((tempLen - i - 1) * 8);
    }

    // RTR
    ptr = strToHex(ptr + 1, &rxPacket.rtr, &tempLen);

    // IDE
    ptr = strToHex(ptr + 1, &rxPacket.ide, &tempLen);

    // DATA
    ptr = strToHex(ptr + 1, rxPacket.dataArray, &rxPacket.dlc);

#if RANDOM_CAN == 1
    // echo back
    printPacket(&rxPacket);
#else
    sendPacketToCan(&rxPacket);
#endif
}

void CanPacketManager::CANsimulate() {
    packet_t txPacket;

    long sampleIdList[] = {0x110, 0x18DAF111, 0x23A, 0x257, 0x412F1A1, 0x601, 0x18EA0C11};
    int idIndex = random(sizeof(sampleIdList) / sizeof(sampleIdList[0]));
    int sampleData[] = {0xA, 0x1B, 0x2C, 0x3D, 0x4E, 0x5F, 0xA0, 0xB1};

    txPacket.id = sampleIdList[idIndex];
    txPacket.ide = txPacket.id > 0x7FF ? 1 : 0;
    txPacket.rtr = 0;
    txPacket.dlc = random(1, 9);

    for (int i = 0; i < txPacket.dlc; i++) {
        int changeByte = random(4);
        if (changeByte == 0) {
            sampleData[i] = random(256);
        }
        txPacket.dataArray[i] = sampleData[i];
    }

    printPacket(&txPacket);
}

void CanPacketManager::RXcallback() {
    static char *rxBuf = NULL;
    static int rxPtr = 0;

    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == TERMINATOR) {
            rxParse(rxBuf, rxPtr);
            free(rxBuf);
            rxBuf = NULL;
            rxPtr = 0;
        } else {
            rxBuf = (char *)realloc(rxBuf, rxPtr + 1);
            rxBuf[rxPtr++] = c;
        }
    }
}
