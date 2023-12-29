#include <Arduino.h>
#include <Wire.h>
// put function declarations here:

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
void printHex(long num) {
  if ( num < 0x10 ){ Serial.print("0"), Wire.write("0"); }
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
  Serial.print(TERMINATOR);
}

void setup() {
  //Wire.begin(1);
  //Wire.setClock(3400000);
  Serial.begin(9600);
  while (!Serial)
  {
    /* code */
  }  
}

void loop() {
  Wire.read();
  
}