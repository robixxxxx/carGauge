#include <TFT_eSPI.h>
//Audi Color Scheme

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
    
    // double rad=0.01745;
    float sA;


    uint16_t color1;
    uint16_t color2;
    
    void drawDigitalValue();

    void drawUnit();

    void drawGauge();

    void drawNeedle();

    public:
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft);
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit,  bool showDigitalValue, 
    uint8_t analogueGaugeFont=1, uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t analogueGaugeFontSize=1, uint8_t digitalGaugeFontSize=1, uint8_t unitGaugeFontSize=1);
    Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit, 
    uint8_t digitalGaugeFont=7, uint8_t unitGaugeFont=1, uint8_t digitalGaugeFontSize=1, uint8_t unitGaugeFontSize=1);
    ~Gauge();
    void update();

    void setBackgroundColor(uint16_t color);
    void setArcColors(uint16_t color);
    void setScaleColors(uint16_t color);
    void setTextColor(uint16_t color);
    void setNeedleColor(uint16_t color);
    void setUnit(String unit);
    void setShowDigitalValue(bool show);
    void setValue(uint16_t value);
    void setAnalogueGaugeFont(uint8_t font);
    void setDigitalGaugeFont(uint8_t font);
    void setUnitGaugeFont(uint8_t font);
    void setAnalogueGaugeFontSize(uint8_t size);
    void setDigitalGaugeFontSize(uint8_t size);
    void setUnitGaugeFontSize(uint8_t size);
    void setName(String name);
    void setSprite(TFT_eSprite * sprite);
    void setFrame(uint16_t frame);
    void setByteMSB(uint8_t byte);
    void setByteLSB(uint8_t byte);
    String getName();
    uint16_t getFrame();
    uint8_t getByteMSB();
    uint8_t getByteLSB();
    uint16_t getBackgroundColor();
    uint16_t getArcColors();
    uint16_t getScaleColors();
    uint16_t getTextColor();
    uint16_t getNeedleColor();
    String getUnit();
    bool getShowDigitalValue();
    uint16_t getValue();
    uint8_t getAnalogueGaugeFont();
    uint8_t getDigitalGaugeFont();
    uint8_t getUnitGaugeFont();
    uint8_t getAnalogueGaugeFontSize();
    uint8_t getDigitalGaugeFontSize();
    uint8_t getUnitGaugeFontSize();
};