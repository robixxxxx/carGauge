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
    TFT_eSprite * _sprite;
    int _cx;
    int _cy;
    int _r;
    int _ir;
    int _n;
    int _angle;
    int _width;
    int _height;
    int _value;
    String _unit;
    bool _showDigitalValue;

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
        _sprite->setTextColor(textColor,backgroundColor);
        _sprite->drawString(String((int)this->_value),_cx,_cy,7);
        _sprite->drawString(this->_unit,_cx,_cy+32);
    }

    void drawGauge(){
        _sprite->drawSmoothArc(_cx, _cy, _r-5, _r-6, 30, 330, arcColors, backgroundColor);

        for(int i=0;i<26;i++){
            if(i<20) {color1=backgroundColor; color2=scaleColors;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
            _sprite->setTextColor(color2,backgroundColor);
            _sprite->drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
    }

    void drawNeedle(){
        sA=this->_value*1.2;
        _sprite->drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,needleColor);
    }

    public:
    AnalogueGauge(TFT_eSprite * sprite, int xCenter, int yCenter, int radius, int innerRadius, int width, int height, String _unit,  bool showDigitalValue = true){
        
        _sprite = sprite;
        _cx = xCenter;
        _cy = yCenter;
        _r = radius;
        _ir = innerRadius;
        _width = width;
        _height = height;
        this->_showDigitalValue = _showDigitalValue;
        this->_unit = _unit;
        this->backgroundColor = BACKGROUNDCOLOR_AUDI;
        this->arcColors=TFT_SILVER;
        this->scaleColors=WHITECOLOR_AUDI;
        this->textColor=WHITECOLOR_AUDI;
        this->needleColor=NEEDLECOLOR_AUDI;
    }

    void init(){
        _sprite->createSprite(240,220);
        _sprite->setSwapBytes(true);
        _sprite->setTextDatum(4);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->setTextDatum(4);
        int a=120;
        for(int i=0;i<360;i++)
        {
            x[i]=((_r-10)*cos(rad*a))+_cx;
            y[i]=((_r-10)*sin(rad*a))+_cy;
            
            lx[i]=((_r-24)*cos(rad*a))+_cx;
            ly[i]=((_r-24)*sin(rad*a))+_cy;
            px[i]=((_r-14)*cos(rad*a))+_cx;
            py[i]=((_r-14)*sin(rad*a))+_cy;
            nx[i]=((_r-36)*cos(rad*a))+_cx;
            ny[i]=((_r-36)*sin(rad*a))+_cy;
            a++;
                if(a==360)
                a=0;
        }
    }
    
    void update(){
        _sprite->fillSprite(backgroundColor);
        drawGauge();
        drawNeedle();
        if (_showDigitalValue)
        {
            drawDigitalValue();
        }
        _sprite->pushSprite(0,0);
    }

    void update(int _value){
        _sprite->fillSprite(BACKGROUNDCOLOR_AUDI);
        _sprite->drawSmoothArc(_cx, _cy, _r-5, _r-6, 30, 330, TFT_WHITE, BACKGROUNDCOLOR_AUDI);

            for(int i=0;i<26;i++){
            if(i<20) {color1=BACKGROUNDCOLOR_AUDI; color2=TFT_WHITE;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
            _sprite->setTextColor(color2,BACKGROUNDCOLOR_AUDI);
            _sprite->drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
        sA=_value*1.2;
        _sprite->drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,NEEDLECOLOR_AUDI);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->drawString(String((int)_value),_cx,_cy,4);
        _sprite->drawString("_unit",_cx,_cy+16);
        _sprite->pushSprite(0,0);
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
        _unit = unit;
    }
    void setShowDigitalValue(bool show){
        _showDigitalValue = show;
    }
    void setValue(int value){
        _value = value;
    }
};