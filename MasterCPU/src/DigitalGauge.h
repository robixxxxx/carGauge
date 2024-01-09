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

class DigitalGauge{
    private:
    TFT_eSprite * _sprite;
    int _cx;
    int _cy;
    int _width;
    int _height;
    int _value;
    String _unit;
    uint16_t backgroundColor, textColor;

    void drawDigitalValue(){
        _sprite->setTextColor(textColor,backgroundColor);
        _sprite->drawString(String((int)this->_value),_cx,_cy,7);
        _sprite->drawString(this->_unit,_cx,_cy+32);
    }

    public:
    DigitalGauge(TFT_eSprite * sprite, int xCenter, int yCenter, int width, int height, String unit){
        
        _sprite = sprite;
        _cx = xCenter;
        _cy = yCenter;
        _width = width;
        _height = height;
        this->_unit = unit;
        this->backgroundColor = BACKGROUNDCOLOR_AUDI;
        this->textColor=WHITECOLOR_AUDI;
    }

    void init(){
        _sprite->createSprite(240,220);
        _sprite->setSwapBytes(true);
        _sprite->setTextDatum(4);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->setTextDatum(4);
    }
    
    void update(){
        _sprite->fillSprite(backgroundColor);
        drawDigitalValue();
        _sprite->pushSprite(0,0);
    }

    void setBackgroundColor(uint16_t color){
        backgroundColor = color;
    }
    void setTextColor(uint16_t color){
        textColor = color;
    }
    void setUnit(char * unit){
        _unit = unit;
    }
    void setValue(int value){
        _value = value;
    }
};