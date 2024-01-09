// #include <CanPacketManager.h>
// #include "SPI.h"

#include <AnalogueGauge.h>
#include <DigitalGauge.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite1 = TFT_eSprite(&tft);

AnalogueGauge spedometer = AnalogueGauge(&sprite1, 120,120,120,(int)(120*1.2),240,220, "unit",true);
DigitalGauge spedometer2 = DigitalGauge(&sprite1, 120,120,240,220, "unit");
int speedAngle = 0;
void setup() {
  tft.init();
  spedometer.init();
  spedometer2.init();
}

// void updateEncoder(){
//   int MSB = digitalRead(PinA); //MSB = most significant bit
//   int LSB = digitalRead(PinB); //LSB = least significant bit

//   int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
//   int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

//   if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos ++;
//   if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos --;

//   lastEncoded = encoded; //store this value for next time
// }

void loop() 
{
  for (size_t i = 0; i < 100; i++)
  {
    speedAngle++;
    if (speedAngle > 260)
      speedAngle = 0;
    spedometer.setValue(speedAngle);
    spedometer.update();
  }
  for (size_t i = 0; i < 100; i++)
  {
    speedAngle++;
    if (speedAngle > 260)
      speedAngle = 0;
    spedometer2.setBackgroundColor((uint16_t)rand());
    spedometer2.setValue(speedAngle);
    spedometer2.update();
  }
  
}


 // packetManager.CANsimulate();