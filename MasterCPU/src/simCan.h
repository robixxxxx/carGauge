//CAN MANAER
#include <CAN.h>

typedef struct {
  long id;
  byte rtr;
  byte ide;
  byte dlc;
  byte dataArray[20];
} packet_t;

const char SEPARATOR = ',';
const char TERMINATOR = '\n';
const char RXBUF_LEN = 100;
//------------------------------------------------------------------------------
// Printing a packet to serial
void printHex(long num) {
  if ( num < 0x10 ){ Serial.print("0"); }
  Serial.print(num, HEX);
}

void printPacket(packet_t * packet) {
  // packet format (hex string): [ID],[RTR],[IDE],[DATABYTES 0..8B]\n
  // example: 014A,00,00,1A002B003C004D\n
  printHex(packet->id);
  Serial.print(SEPARATOR);
  printHex(packet->rtr);
  Serial.print(SEPARATOR);
  printHex(packet->ide);
  Serial.print(SEPARATOR);
  // DLC is determinded by number of data bytes, format: [00]
  for (int i = 0; i < packet->dlc; i++) {
    printHex(packet->dataArray[i]);
  }
  Serial.println(TERMINATOR);
}
//------------------------------------------------------------------------------
// CAN packet simulator
void CANsimulate(packet_t * txPacket) {
  long sampleIdList[] = {0x110, 0x18DAF111, 0x23A, 0x257, 0x412F1A1, 0x601, 0x18EA0C11};
  int idIndex = random (sizeof(sampleIdList) / sizeof(sampleIdList[0]));
  int sampleData[] = {0xA, 0x1B, 0x2C, 0x3D, 0x4E, 0x5F, 0xA0, 0xB1};

  txPacket->id = sampleIdList[idIndex];
  txPacket->ide = txPacket->id > 0x7FF ? 1 : 0;
  txPacket->rtr = 0; //random(2);
  txPacket->dlc = random(1, 9);

  for (int i = 0; i < txPacket->dlc ; i++) {
    int changeByte = random(4);
    if (changeByte == 0) {
      sampleData[i] = random(256);
    }
    txPacket->dataArray[i] = sampleData[i];
  }

  //printPacket(txPacket);
}

//------------------------------------------------------------------------------
// Serial parser
char getNum(char c) {
  if (c >= '0' && c <= '9') { return c - '0'; }
  if (c >= 'a' && c <= 'f') { return c - 'a' + 10; }
  if (c >= 'A' && c <= 'F') { return c - 'A' + 10; }
  return 0;
}

char * strToHex(char * str, byte * hexArray, byte * len) {
  byte *ptr = hexArray;
  char * idx;
  for (idx = str ; *idx != SEPARATOR && *idx != TERMINATOR; ++idx, ++ptr ) {
    *ptr = (getNum( *idx++ ) << 4) + getNum( *idx );
  }
  *len = ptr - hexArray;
  return idx;
}

void rxParse(char * buf, int len) {
  packet_t rxPacket;
  char * ptr = buf;
  // All elements have to have leading zero!

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


  // echo back
  printPacket(&rxPacket);

}

void RXcallback(void) {
  static int rxPtr = 0;
  static char rxBuf[RXBUF_LEN];

  while (Serial.available() > 0) {
    if (rxPtr >= RXBUF_LEN) {
      rxPtr = 0;
    }
    char c = Serial.read();
    rxBuf[rxPtr++] = c;
    if (c == TERMINATOR) {
      rxParse(rxBuf, rxPtr);
      rxPtr = 0;
    }
  }
}