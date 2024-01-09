#include "SPI.h"
#include <TFT_eSPI.h>
//Audi Color Scheme
#ifndef ANALOGUEGAUGE_H
#define ANALOGUEGAUGE_H

#define BACKGROUNDCOLOR_AUDI 0x0861
#define NEEDLECOLOR_AUDI 0xfaeb
#define REDCOLOR_AUDI 0xf245
#define WHITECOLOR_AUDI 0xffdf
//SKODA Color Scheme
#define GREENCOLOR_SKODA 0x4780

#define rad 0.01745 //double rad=0.01745;

class Gauge{
    protected:
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
    uint8_t _analogueGaugeFont, _digitalGaugeFont, _unitGaugeFont;
    uint8_t _analogueGaugeFontSize, _digitalGaugeFontSize, _unitGaugeFontSize;
    uint16_t _backgroundColor, _arcColors, _scaleColors, _textColor, _needleColor;

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
        _sprite->setTextFont(_digitalGaugeFont);
        _sprite->setTextSize(_digitalGaugeFontSize);
        _sprite->setTextColor(_textColor,_backgroundColor);
        _sprite->drawString(String((int)this->_value),_cx,_cy);
    }

    void drawUnit(){
        _sprite->setTextFont(_unitGaugeFont);
        _sprite->setTextSize(_unitGaugeFontSize);
        _sprite->drawString(_unit,_cx,_cy+32);
    }

    void drawGauge(){
        _sprite->drawSmoothArc(_cx, _cy, _r-5, _r-6, 30, 330, _arcColors, _backgroundColor);
        _sprite->setTextFont(_analogueGaugeFont);
        _sprite->setTextSize(_analogueGaugeFontSize);
        for(int i=0;i<26;i++){
            if(i<20) {color1=_backgroundColor; color2=_scaleColors;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}

        if(i%2==0) {
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],2,1,color1);
            _sprite->setTextColor(color2,_backgroundColor);
            _sprite->drawString(String(i*10),lx[i*12],ly[i*12]);
        }else
            _sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
    }

    void drawNeedle(){
        sA=this->_value*1.2;
        _sprite->drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,_needleColor);
    }

    public:
    Gauge(TFT_eSprite * sprite, int xCenter, int yCenter, int radius, int innerRadius, int width, int height, String _unit,  bool showDigitalValue = true, 
    uint8_t analogueGaugeFont=1, uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t analogueGaugeFontSize=1, uint8_t digitalGaugeFontSize=2, uint8_t unitGaugeFontSize=1){
        
        _sprite = sprite;
        _cx = xCenter;
        _cy = yCenter;
        _r = radius;
        _ir = innerRadius;
        _width = width;
        _height = height;
        _showDigitalValue = showDigitalValue;
        _unit = _unit;
        _backgroundColor = BACKGROUNDCOLOR_AUDI;
        _arcColors=TFT_SILVER;
        _scaleColors=WHITECOLOR_AUDI;
        _textColor=WHITECOLOR_AUDI;
        _needleColor=NEEDLECOLOR_AUDI;
        _analogueGaugeFont = analogueGaugeFont;
        _digitalGaugeFont = digitalGaugeFont;
        _unitGaugeFont = unitGaugeFont;
        _analogueGaugeFontSize = analogueGaugeFontSize;
        _digitalGaugeFontSize = digitalGaugeFontSize;
        _digitalGaugeFontSize = unitGaugeFontSize;
    }
    Gauge(TFT_eSprite * sprite, int xCenter, int yCenter, int width, int height, String _unit, 
    uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t digitalGaugeFontSize=2, uint8_t unitGaugeFontSize=1){
        
        _sprite = sprite;
        _cx = xCenter;
        _cy = yCenter;
        _width = width;
        _height = height;
        _unit = _unit;
        _backgroundColor = BACKGROUNDCOLOR_AUDI;
        _textColor=WHITECOLOR_AUDI;
        _digitalGaugeFont = digitalGaugeFont;
        _unitGaugeFont = unitGaugeFont;
        _digitalGaugeFontSize = digitalGaugeFontSize;
        _digitalGaugeFontSize = unitGaugeFontSize;
    }
    ~Gauge(){
        delete _sprite;
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
    
    virtual void update(){
        _sprite->fillSprite(_backgroundColor);
        drawGauge();
        drawNeedle();
        if (_showDigitalValue)
        {
            drawDigitalValue();
        }
        drawUnit();
        _sprite->pushSprite(0,0);
    }

    void setBackgroundColor(uint16_t color){
        _backgroundColor = color;
    }
    void setArcColors(uint16_t color){
        _arcColors = color;
    }
    void setScaleColors(uint16_t color){
        _scaleColors = color;
    }
    void setTextColor(uint16_t color){
        _textColor = color;
    }
    void setNeedleColor(uint16_t color){
        _needleColor = color;
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
    void setAnalogueGaugeFont(uint8_t font){
        _analogueGaugeFont = font;
    }
    void setDigitalGaugeFont(uint8_t font){
        _digitalGaugeFont = font;
    }
    void setUnitGaugeFont(uint8_t font){
        _unitGaugeFont = font;
    }
    void setAnalogueGaugeFontSize(uint8_t size){
        size;
    }
    void setDigitalGaugeFontSize(uint8_t size){
        size;
    }
    void setUnitGaugeFontSize(uint8_t size){
        size;
    }
};
#endif // DIGITALGAUGE_H