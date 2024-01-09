// #include <CanPacketManager.h>
// #include "SPI.h"

#define code_version 1

#if code_version==0 || code_version==1
#include <AnalogueGauge.h>
TFT_eSPI tft = TFT_eSPI();
AnalogueGauge spedometer = AnalogueGauge(120,120,120,(int)(120*1.2),240,220, "unit",false);
AnalogueGauge spedometer2 = AnalogueGauge(120,120,120,(int)(120*1.2),240,220, "unit",false);
int speedAngle = 0;
void setup() {
  
  spedometer.init();
  spedometer2.init();
}

void loop() 
{
  for (size_t i = 0; i < 100; i++)
  {
    speedAngle++;
    if (speedAngle > 360)
      speedAngle = 0;
    spedometer.setValue(speedAngle);
    spedometer.update();
  }
  delay(2000);
  for (size_t i = 0; i < 100; i++)
  {
    speedAngle++;
    if (speedAngle > 360)
      speedAngle = 0;
    spedometer2.setBackgroundColor((uint16_t)rand());
    spedometer2.setValue(speedAngle);
    spedometer2.update();
  }
  
}


 // packetManager.CANsimulate();

#elif code_version==2
#include <TFT_eSPI.h>
#include <AnalogueGauge.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


#define backColor 0x0026
#define gaugeColor 0x055D
#define dataColor 0x0311
#define purple 0xEA16
#define needleColor 0xF811

//............................dont edit this
int cx=120;
int cy=120;
int r=120;
int ir=r*1.02;
int n=0;
int angle=0;

float x[360]; //outer points of Speed gaouges
float y[360];
float px[360]; //ineer point of Speed gaouges
float py[360];
float lx[360]; //text of Speed gaouges
float ly[360];
float nx[360]; //needle low of Speed gaouges
float ny[360];


double rad=0.01745;
unsigned short color1;
unsigned short color2;
float sA;

float speedAngle=0; //...speed variable 0-240

void setup() {
  
  tft.init();

    tft.init();
        tft.setRotation(1);
        tft.fillScreen(BACKGROUNDCOLOR_AUDI);
        // width = tft.width();
        // height = tft.height();
        // cx=120;//width/2;//120
        // cy=112;//height/2;//112
        // r = 110; //width>=height? height/2: width/2;
        sprite.createSprite(240,220);
        sprite.setSwapBytes(true);
        sprite.setTextDatum(4);
        sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        sprite.setTextDatum(4);
        int a=120;
        for(int i=0;i<360;i++)
        {
            x[i]=((r-10)*cos(rad*a))+cx;
            y[i]=((r-10)*sin(rad*a))+cy;
            px[i]=((r-14)*cos(rad*a))+cx;
            py[i]=((r-14)*sin(rad*a))+cy;
            lx[i]=((r-24)*cos(rad*a))+cx;
            ly[i]=((r-24)*sin(rad*a))+cy;
            nx[i]=((r-36)*cos(rad*a))+cx;
            ny[i]=((r-36)*sin(rad*a))+cy;
            a++;
                if(a==360)
                a=0;
        }
     
}



void draw()
{
  sprite.fillSprite(BACKGROUNDCOLOR_AUDI);

        sprite.drawSmoothArc(cx, cy, r-5, r-6, 30, 330, TFT_WHITE, BACKGROUNDCOLOR_AUDI);

          //.....................................................draw GAUGES
            for(int i=0;i<26;i++){
            if(i<20) {color1=BACKGROUNDCOLOR_AUDI; color2=TFT_WHITE;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
        sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
        sprite.setTextColor(color2,BACKGROUNDCOLOR_AUDI);
        sprite.drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
        sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }

        // ........................................needles draw
        sA=speedAngle*1.2;

        sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,NEEDLECOLOR_AUDI);
   

          //.....................................drawing  TEXT
        sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        sprite.drawString(String((int)speedAngle),cx,cy,4);

        sprite.drawString("KM/H",cx,cy+16);


  
        sprite.pushSprite(0,0);
}


void loop() {
speedAngle++;
  if(speedAngle>360)
    speedAngle=0;
    draw();
}
#endif