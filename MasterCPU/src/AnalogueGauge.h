#include "SPI.h"
#include <TFT_eSPI.h>
//Audi Color Scheme

#define BACKGROUNDCOLOR_AUDI 0x0861
#define NEEDLECOLOR_AUDI 0xfaeb
#define REDCOLOR_AUDI 0xf245
#define WHITECOLOR_AUDI 0xffdf
//SKODA Color Scheme
#define GREENCOLOR_SKODA 0x4780

#define rad 0.01745 //double rad=0.01745;

class AnalogueGauge{
    private:
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite sprite = TFT_eSprite(&tft);
    int cx;
    int cy;
    int r;
    int ir;
    int n;
    int angle;
    int width;
    int height;
    int value;
    String unit;
    bool showDigitalValue;

    uint16_t backgroundColor, arcColors, scaleColors, textColor, needleColor;

    float x[360]; //outer points of Speed gaouges
    float y[360];
    float px[360]; //ineer point of Speed gaouges
    float py[360];
    float lx[360]; //text of Speed gaouges
    float ly[360];
    float nx[360]; //needle low of Speed gaouges
    float ny[360];

    // double rad=0.01745;
    float sA;


    uint16_t color1;
    uint16_t color2;
    
    void drawDigitalValue(){
        sprite.setTextColor(textColor,backgroundColor);
        sprite.drawString(String((int)this->value),cx,cy,7);
        sprite.drawString(this->unit,cx,cy+32);
    }

    void drawGauge(){
        sprite.drawSmoothArc(cx, cy, r-5, r-6, 30, 330, arcColors, backgroundColor);

        for(int i=0;i<26;i++){
            if(i<20) {color1=backgroundColor; color2=scaleColors;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
            sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
            sprite.setTextColor(color2,backgroundColor);
            sprite.drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
            sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
    }

    void drawNeedle(){
        sA=this->value*1.2;
        sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,needleColor);
    }

    public:
    
    AnalogueGauge(int xCenter, int yCenter, int radius, int innerRadius, int width, int height, String unit,  bool showDigitalValue = true){
        cx = xCenter;
        cy = yCenter;
        r = radius;
        ir = innerRadius;
        width = width;
        height = height;
        this->showDigitalValue = showDigitalValue;
        this->unit = unit;
        this->backgroundColor = BACKGROUNDCOLOR_AUDI;
        this->arcColors=TFT_SILVER;
        this->scaleColors=WHITECOLOR_AUDI;
        this->textColor=WHITECOLOR_AUDI;
        this->needleColor=NEEDLECOLOR_AUDI;
    }

    void init(){
        tft.init();
        tft.setRotation(1);
        tft.fillScreen(BACKGROUNDCOLOR_AUDI);
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
            
            lx[i]=((r-24)*cos(rad*a))+cx;
            ly[i]=((r-24)*sin(rad*a))+cy;
            px[i]=((r-14)*cos(rad*a))+cx;
            py[i]=((r-14)*sin(rad*a))+cy;
            nx[i]=((r-36)*cos(rad*a))+cx;
            ny[i]=((r-36)*sin(rad*a))+cy;
            a++;
                if(a==360)
                a=0;
        }
    }
    
    void update(){
        sprite.fillSprite(BACKGROUNDCOLOR_AUDI);
        drawGauge();
        drawNeedle();
        if (showDigitalValue)
        {
            drawDigitalValue();
        }
        sprite.pushSprite(0,0);
    }

    void update(int value){
        sprite.fillSprite(BACKGROUNDCOLOR_AUDI);
        sprite.drawSmoothArc(cx, cy, r-5, r-6, 30, 330, TFT_WHITE, BACKGROUNDCOLOR_AUDI);

            for(int i=0;i<26;i++){
            if(i<20) {color1=BACKGROUNDCOLOR_AUDI; color2=TFT_WHITE;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
            sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
            sprite.setTextColor(color2,BACKGROUNDCOLOR_AUDI);
            sprite.drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
            sprite.drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
        sA=value*1.2;
        sprite.drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,NEEDLECOLOR_AUDI);
        sprite.setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        sprite.drawString(String((int)value),cx,cy,4);
        sprite.drawString("unit",cx,cy+16);
        sprite.pushSprite(0,0);
    }
    void setBackgroundColor(uint16_t color){
        backgroundColor = color;
    }
    void setArcColors(uint16_t color){
        arcColors = color;
    }
    void setScaleColors(uint16_t color){
        scaleColors = color;
    }
    void setTextColor(uint16_t color){
        textColor = color;
    }
    void setNeedleColor(uint16_t color){
        needleColor = color;
    }
    void setUnit(char * unit){
        unit = unit;
    }
    void setShowDigitalValue(bool show){
        showDigitalValue = show;
    }
    void setValue(int value){
        this->value = value;
    }
};