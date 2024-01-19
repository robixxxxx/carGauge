    #include <Gauge.h>

    void Gauge::drawDigitalValue(){
        _sprite->setTextFont(_digitalGaugeFont);
        _sprite->setTextSize(_digitalGaugeFontSize);
        _sprite->setTextColor(_textColor,_backgroundColor);
        _sprite->drawString(String((int)this->_value),_cx,_cy);
    }

    void Gauge::drawUnit(){
        _sprite->setTextFont(_unitGaugeFont);
        _sprite->setTextSize(_unitGaugeFontSize);
        _sprite->drawString(_unit,_cx,_cy+32);
    }

    void Gauge::drawGauge(){
        _sprite->drawSmoothArc(_cx, _cy, _r-5, _r-6, 30, 330, _arcColors, _backgroundColor);
        _sprite->setTextFont(_analogueGaugeFont);
        _sprite->setTextSize(_analogueGaugeFontSize);
        for(int i=0;i<26;i++){
            if(i<20) {color1=_backgroundColor; color2=_scaleColors;} else {color1=REDCOLOR_AUDI; color2=REDCOLOR_AUDI;}
            int angle = (i*12+120)%360;
            float ax = ((_r-10)*cos(rad*angle))+_cx;
            float ay = ((_r-10)*sin(rad*angle))+_cy;
            float bx = ((_r-14)*cos(rad*angle))+_cx;
            float by = ((_r-14)*sin(rad*angle))+_cy;
        if(i%2==0) {
            _sprite->drawWedgeLine(ax,ay,bx,by,2,1,color1);
            _sprite->setTextColor(color2,_backgroundColor);
            float tx = ((_r-24)*cos(rad*angle))+_cx;
            float ty = ((_r-24)*sin(rad*angle))+_cy;
            _sprite->drawString(String(i*10),tx,ty);
        }else
            _sprite->drawWedgeLine(ax,ay,bx,by,2,1,color2);
            //_sprite->drawWedgeLine(x[i*12],y[i*12],px[i*12],py[i*12],1,1,color2);
        }
    }

    void Gauge::drawNeedle(){
        //angle multiplier
        sA=this->_value*1.2;
        int angle = sA+120;
        float ax = ((_r-14)*cos(rad*(angle%360)))+_cx;
        float ay = ((_r-14)*sin(rad*(angle%360)))+_cy;
        float bx = ((_r-36)*cos(rad*(angle%360)))+_cx;
        float by = ((_r-36)*sin(rad*(angle%360)))+_cy;

        // _sprite->drawWedgeLine(px[(int)sA],py[(int)sA],nx[(int)sA],ny[(int)sA],2,2,_needleColor);
        _sprite->drawWedgeLine(ax,ay,bx,by,2,2,_needleColor);
    }

    Gauge::Gauge(TFT_eSprite * sprite, TFT_eSPI * tft){
        _sprite = sprite;
        _cx = tft->width()/2;
        _cy = tft->height()/2;
        _r = tft->width()<=tft->width()?tft->width()/2:tft->height()/2;
        _ir = _r*0,98;
        _width = tft->width();
        _height = tft->height();
        _angle = 0;
        _sprite->createSprite(240,220);
        _sprite->setSwapBytes(true);
        _sprite->setTextDatum(4);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->setTextDatum(4);
    }
    
    Gauge::Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit,  bool showDigitalValue, 
    uint8_t analogueGaugeFont, uint8_t digitalGaugeFont, uint8_t unitGaugeFont, uint8_t analogueGaugeFontSize, uint8_t digitalGaugeFontSize, uint8_t unitGaugeFontSize){
        
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
        _sprite->createSprite(240,220);
        _sprite->setSwapBytes(true);
        _sprite->setTextDatum(4);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->setTextDatum(4);
    }

    Gauge::Gauge(TFT_eSprite * sprite, TFT_eSPI * tft, String _unit, 
    uint8_t digitalGaugeFont, uint8_t unitGaugeFont, uint8_t digitalGaugeFontSize, uint8_t unitGaugeFontSize){
        
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
        _sprite->createSprite(240,220);
        _sprite->setSwapBytes(true);
        _sprite->setTextDatum(4);
        _sprite->setTextColor(TFT_WHITE,BACKGROUNDCOLOR_AUDI);
        _sprite->setTextDatum(4);
    }
    Gauge::~Gauge(){}
    
    

    void Gauge::update(){
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

    void Gauge::setBackgroundColor(uint16_t color){
        _backgroundColor = color;
    }
    void Gauge::setArcColors(uint16_t color){
        _arcColors = color;
    }
    void Gauge::setScaleColors(uint16_t color){
        _scaleColors = color;
    }
    void Gauge::setTextColor(uint16_t color){
        _textColor = color;
    }
    void Gauge::setNeedleColor(uint16_t color){
        _needleColor = color;
    }
    void Gauge::setUnit(String unit){
        _unit = unit;
    }
    void Gauge::setShowDigitalValue(bool show){
        _showDigitalValue = show;
    }
    void Gauge::setValue(uint16_t value){
        _value = value;
    }
    void Gauge::setAnalogueGaugeFont(uint8_t font){
        _analogueGaugeFont = font;
    }
    void Gauge::setDigitalGaugeFont(uint8_t font){
        _digitalGaugeFont = font;
    }
    void Gauge::setUnitGaugeFont(uint8_t font){
        _unitGaugeFont = font;
    }
    void Gauge::setAnalogueGaugeFontSize(uint8_t size){
        _analogueGaugeFontSize = size;
    }
    void Gauge::setDigitalGaugeFontSize(uint8_t size){
        _digitalGaugeFontSize = size;
    }
    void Gauge::setUnitGaugeFontSize(uint8_t size){
        _unitGaugeFontSize = size;
    }
    void Gauge::setName(String name){
        _name = name;
    }
    void Gauge::setSprite(TFT_eSprite * sprite){
        _sprite = sprite;
    }
    void Gauge::setFrame(uint16_t frame){
        _frame = frame;
    }
    void Gauge::setByteMSB(uint8_t byte){
        _byteMSB = byte;
    }
    void Gauge::setByteLSB(uint8_t byte){
        _byteLSB = byte;
    }
    String Gauge::getName(){
        return _name;
    }
    uint16_t Gauge::getFrame(){
        return _frame;
    }
    uint8_t Gauge::getByteMSB(){
        return _byteMSB;
    }
    uint8_t Gauge::getByteLSB(){
        return _byteLSB;
    }
    uint16_t Gauge::getBackgroundColor(){
        return _backgroundColor;
    }
    uint16_t Gauge::getArcColors(){
        return _arcColors;
    }
    uint16_t Gauge::getScaleColors(){
        return _scaleColors;
    }
    uint16_t Gauge::getTextColor(){
        return _textColor;
    }
    uint16_t Gauge::getNeedleColor(){
        return _needleColor;
    }
    String Gauge::getUnit(){
        return _unit;
    }
    bool Gauge::getShowDigitalValue(){
        return _showDigitalValue;
    }
    uint16_t Gauge::getValue(){
        return _value;
    }
    uint8_t Gauge::getAnalogueGaugeFont(){
        return _analogueGaugeFont;
    }
    uint8_t Gauge::getDigitalGaugeFont(){
        return _digitalGaugeFont;
    }
    uint8_t Gauge::getUnitGaugeFont(){
        return _unitGaugeFont;
    }
    uint8_t Gauge::getAnalogueGaugeFontSize(){
        return _analogueGaugeFontSize;
    }
    uint8_t Gauge::getDigitalGaugeFontSize(){
        return _digitalGaugeFontSize;
    }
    uint8_t Gauge::getUnitGaugeFontSize(){
        return _unitGaugeFontSize;
    }