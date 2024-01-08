// #include <CanPacketManager.h>
#include "SPI.h"
#include <TFT_eSPI.h>

// #include "myfile.c"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
//Audi Color Scheme
#define BACKGROUNDCOLOR_AUDI 0x0861
#define NEEDLECOLOR_AUDI 0xfaeb
#define REDCOLOR_AUDI 0xf245
#define WHITECOLOR_AUDI 0xffdf
//SKODA Color Scheme
#define GREENCOLOR_SKODA 0x4780
class analogueGauge{
  analogueGauge(){
  sprite.createSprite(240,220);
  sprite.setSwapBytes(true);
  sprite.setTextDatum(10);
  sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
  sprite.setTextDatum(10);
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
  sprite.fillSprite(BACKGROUNDCOLOR_AUDI);
  sprite.drawSmoothArc(cx, cy, r-5, r-6, 30, 330, TFT_WHITE, BACKGROUNDCOLOR_AUDI);
  sprite.drawSmoothArc(cx, cy, r-9, r-8, 270, 330, REDCOLOR_AUDI, BACKGROUNDCOLOR_AUDI);
  }
  void drawNeedle(float angle,float multilier=1.2){
    sA=speedAngle*1.2;
    rA=2*rpmAngle*1.6;
    sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,NEEDLECOLOR_AUDI);

  }

  void drawGauge(){
    for(int i=0;i<26;i++){
    if(i<20) {color1=BACKGROUNDCOLOR_AUDI; color2=TFT_WHITE;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

    if(i%2==0) {
      sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
      sprite.setTextColor(color2,BACKGROUNDCOLOR_AUDI);
      sprite.drawString(String(i*10),lx[i*12],ly[i*12]);
    }else
      sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
    }
  }

  void drawSpeed(){

  }
};
// int cx=75;
// int cy=75;
int cx=120;
int cy=112;
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
float rA;
int blinkPeriod=500;
unsigned long currentTimeL=0;
unsigned long currentTimeR=0;
int brightnesses[5]={40,80,120,150,240};
int selectedBrightness=3;
int deb1=0;
int deb2=0;
int debB=0;

unsigned short blockColor[4]={0x0312,0x0290,0x01EC,0x016A};
unsigned short dirColor[2]={0x0312,TFT_ORANGE};
unsigned short lightColor[3]={0x01EC,0x0FA8,0xB79F};

bool leftPointer=0; 
bool rightPointer=0;
bool braking;
int lights=0;  //0 is lights off, 1 is short light, 2 is long lights
float speedAngle=0; //...speed variable 0-240
float rpmAngle=5;  //.....RPM variable 0-9

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  sprite.createSprite(240,220);
  sprite.setSwapBytes(true);
  sprite.setTextDatum(10);
  sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
  sprite.setTextDatum(10);
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
  sprite.drawSmoothArc(cx, cy, r-9, r-8, 270, 330, REDCOLOR_AUDI, BACKGROUNDCOLOR_AUDI);
  
  
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
  rA=2*rpmAngle*1.6;
  sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,NEEDLECOLOR_AUDI);



  //.....................................drawing  TEXT
  sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
  sprite.drawString(String((int)speedAngle),tft.width()/2,tft.height()/2,7);

  sprite.drawString("KM/H",cx,cy+16);


  //..............................................push Sprite to screen  
  sprite.pushSprite(0,10);
  
}
 

void loop() 
{
  speedAngle++;
  if(speedAngle>360)
    speedAngle=0;
    draw();
}

 // packetManager.CANsimulate();