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
    uint16_t _cx;
    uint16_t _cy;
    uint16_t _r;
    uint16_t _ir;
    uint16_t _n;
    uint16_t _angle;
    uint16_t _width;
    uint16_t _height;
    uint16_t _value;
    uint16_t _frame;
    uint8_t _byteMSB;
    uint8_t _byteLSB;
    String _name;
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
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft){
        _sprite = sprite;
        _cx = tft->width()/2;
        _cy = tft->height()/2;
        _r = tft->width()<=tft->width()?tft->width()/2:tft->height()/2;
        _ir = _r*0,98;
        _width = tft->width();
        _height = tft->height();
        _angle = 0;
    };
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit,  bool showDigitalValue, 
    uint8_t analogueGaugeFont=1, uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t analogueGaugeFontSize=1, uint8_t digitalGaugeFontSize=1, uint8_t unitGaugeFontSize=1){
        
        _sprite = sprite;
        _cx = tft->width()/2;
        _cy = tft->height()/2;
        _r = tft->width()<=tft->width()?tft->width()/2:tft->height()/2;
        _ir = _r*0,98;
        _width = tft->width();
        _height = tft->height();
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
        _unitGaugeFontSize = unitGaugeFontSize;
        _angle = 0;
    }
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit, 
    uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t digitalGaugeFontSize=1, uint8_t unitGaugeFontSize=1){
        
        _sprite = sprite;
        _cx = tft->width()/2;
        _cy = tft->height()/2;
        _r = tft->width()<=tft->width()?tft->width()/2:tft->height()/2;
        _ir = _r*0,98;
        _width = tft->width();
        _height = tft->height();
        _unit = _unit;
        _showDigitalValue = false;
        _backgroundColor = BACKGROUNDCOLOR_AUDI;
        _textColor=WHITECOLOR_AUDI;
        _digitalGaugeFont = digitalGaugeFont;
        _unitGaugeFont = unitGaugeFont;
        _digitalGaugeFontSize = digitalGaugeFontSize;
        _unitGaugeFontSize = unitGaugeFontSize;
        _angle = 0;
    }
    ~Gauge(){
        
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
    void setUnit(String unit){
        _unit = unit;
    }
    void setShowDigitalValue(bool show){
        _showDigitalValue = show;
    }
    void setValue(uint16_t value){
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
        _analogueGaugeFontSize = size;
    }
    void setDigitalGaugeFontSize(uint8_t size){
        _digitalGaugeFontSize = size;
    }
    void setUnitGaugeFontSize(uint8_t size){
        _unitGaugeFontSize = size;
    }
    void setName(String name){
        _name = name;
    }
    void setSprite(TFT_eSprite * sprite){
        _sprite = sprite;
    }
    void setFrame(uint16_t frame){
        _frame = frame;
    }
    void setByteMSB(uint8_t byte){
        _byteMSB = byte;
    }
    void setByteLSB(uint8_t byte){
        _byteLSB = byte;
    }
    String getName(){
        return _name;
    }
    uint16_t getFrame(){
        return _frame;
    }
    uint8_t getByteMSB(){
        return _byteMSB;
    }
    uint8_t getByteLSB(){
        return _byteLSB;
    }
    uint16_t getBackgroundColor(){
        return _backgroundColor;
    }
    uint16_t getArcColors(){
        return _arcColors;
    }
    uint16_t getScaleColors(){
        return _scaleColors;
    }
    uint16_t getTextColor(){
        return _textColor;
    }
    uint16_t getNeedleColor(){
        return _needleColor;
    }
    String getUnit(){
        return _unit;
    }
    bool getShowDigitalValue(){
        return _showDigitalValue;
    }
    uint16_t getValue(){
        return _value;
    }
    uint8_t getAnalogueGaugeFont(){
        return _analogueGaugeFont;
    }
    uint8_t getDigitalGaugeFont(){
        return _digitalGaugeFont;
    }
    uint8_t getUnitGaugeFont(){
        return _unitGaugeFont;
    }
    uint8_t getAnalogueGaugeFontSize(){
        return _analogueGaugeFontSize;
    }
    uint8_t getDigitalGaugeFontSize(){
        return _digitalGaugeFontSize;
    }
    uint8_t getUnitGaugeFontSize(){
        return _unitGaugeFontSize;
    }

    void serialize(File& file) const {
        file.write(reinterpret_cast<const uint8_t*>(&_cx), sizeof(_cx));
        file.write(reinterpret_cast<const uint8_t*>(&_cy), sizeof(_cy));
        file.write(reinterpret_cast<const uint8_t*>(&_r), sizeof(_r));
        file.write(reinterpret_cast<const uint8_t*>(&_ir), sizeof(_ir));
        file.write(reinterpret_cast<const uint8_t*>(&_n), sizeof(_n));
        file.write(reinterpret_cast<const uint8_t*>(&_angle), sizeof(_angle));
        file.write(reinterpret_cast<const uint8_t*>(&_width), sizeof(_width));
        file.write(reinterpret_cast<const uint8_t*>(&_height), sizeof(_height));
        file.write(reinterpret_cast<const uint8_t*>(&_value), sizeof(_value));
        file.write(reinterpret_cast<const uint8_t*>(&_name), sizeof(_name));
        file.write(reinterpret_cast<const uint8_t*>(&_unit), sizeof(_unit));
        file.write(reinterpret_cast<const uint8_t*>(&_showDigitalValue), sizeof(_showDigitalValue));
        file.write(reinterpret_cast<const uint8_t*>(&_analogueGaugeFont), sizeof(_analogueGaugeFont));
        file.write(reinterpret_cast<const uint8_t*>(&_digitalGaugeFont), sizeof(_digitalGaugeFont));
        file.write(reinterpret_cast<const uint8_t*>(&_unitGaugeFont), sizeof(_unitGaugeFont));
        file.write(reinterpret_cast<const uint8_t*>(&_analogueGaugeFontSize), sizeof(_analogueGaugeFontSize));
        file.write(reinterpret_cast<const uint8_t*>(&_digitalGaugeFontSize), sizeof(_digitalGaugeFontSize));
        file.write(reinterpret_cast<const uint8_t*>(&_unitGaugeFontSize), sizeof(_unitGaugeFontSize));
        file.write(reinterpret_cast<const uint8_t*>(&_backgroundColor), sizeof(_backgroundColor));
        file.write(reinterpret_cast<const uint8_t*>(&_arcColors), sizeof(_arcColors));
        file.write(reinterpret_cast<const uint8_t*>(&_scaleColors), sizeof(_scaleColors));
        file.write(reinterpret_cast<const uint8_t*>(&_textColor), sizeof(_textColor));
        file.write(reinterpret_cast<const uint8_t*>(&_needleColor), sizeof(_needleColor));
    }
    void deserialize(File& file) {
        file.read(reinterpret_cast<uint8_t*>(&_cx), sizeof(_cx));
        file.read(reinterpret_cast<uint8_t*>(&_cy), sizeof(_cy));
        file.read(reinterpret_cast<uint8_t*>(&_r), sizeof(_r));
        file.read(reinterpret_cast<uint8_t*>(&_ir), sizeof(_ir));
        file.read(reinterpret_cast<uint8_t*>(&_n), sizeof(_n));
        file.read(reinterpret_cast<uint8_t*>(&_angle), sizeof(_angle));
        file.read(reinterpret_cast<uint8_t*>(&_width), sizeof(_width));
        file.read(reinterpret_cast<uint8_t*>(&_height), sizeof(_height));
        file.read(reinterpret_cast<uint8_t*>(&_value), sizeof(_value));
        file.read(reinterpret_cast<uint8_t*>(&_name), sizeof(_name));
        file.read(reinterpret_cast<uint8_t*>(&_unit), sizeof(_unit));
        file.read(reinterpret_cast<uint8_t*>(&_showDigitalValue), sizeof(_showDigitalValue));
        file.read(reinterpret_cast<uint8_t*>(&_analogueGaugeFont), sizeof(_analogueGaugeFont));
        file.read(reinterpret_cast<uint8_t*>(&_digitalGaugeFont), sizeof(_digitalGaugeFont));
        file.read(reinterpret_cast<uint8_t*>(&_unitGaugeFont), sizeof(_unitGaugeFont));
        file.read(reinterpret_cast<uint8_t*>(&_analogueGaugeFontSize), sizeof(_analogueGaugeFontSize));
        file.read(reinterpret_cast<uint8_t*>(&_digitalGaugeFontSize), sizeof(_digitalGaugeFontSize));
        file.read(reinterpret_cast<uint8_t*>(&_unitGaugeFontSize), sizeof(_unitGaugeFontSize));
        file.read(reinterpret_cast<uint8_t*>(&_backgroundColor), sizeof(_backgroundColor));
        file.read(reinterpret_cast<uint8_t*>(&_arcColors), sizeof(_arcColors));
        file.read(reinterpret_cast<uint8_t*>(&_scaleColors), sizeof(_scaleColors));
        file.read(reinterpret_cast<uint8_t*>(&_textColor), sizeof(_textColor));
        file.read(reinterpret_cast<uint8_t*>(&_needleColor), sizeof(_needleColor));
    }
};
#endif // DIGITALGAUGE_H