
#include <EEPROM.h>
#include <webSite.h>
#include <Gauge.h>
#include <simCAN.h>
#include <CAN.h>
#include <WiFi.h>

#define FORMAT 0
#define EEPROM_SIZE 512
#define RANDOM_CAN 0

#define SEPARATOR ','
#define TERMINATOR '\n'
const int MAX_DATA_ARRAY_SIZE = 20;

const char *ssid = "FunBox3-EA22";
const char *password = "9LYXSMWWKFN7";
const char *characters[] = {"0123456789",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "abcdefghijklmnopqrstuvwxyz"};

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite1 = TFT_eSprite(&tft);

int gaugedatasize = 41;

uint8_t j=0;
static int numberOfGauges = 0;
Gauge * gauge[4];
String gaugeName[4];
uint8_t speedAngle = 0;


uint32_t timer;
bool flag; 


/// @brief Random String Generaor for Wifi password
/// @param length 
/// @return 
String randomStringGenerator(int length){
  String result = "";
  char actual;
  for (size_t i = 0; i < length; i++)
  {
    randomSeed(analogRead(A0));
    actual = random(3);
    if(actual==0){
      actual = characters[actual][random(300)%9];
    }
    else{
      actual = characters[actual][random(300)%27];
    }
    result += actual;
  }
  return result;
}

/// @brief Handler for web page for gauge deleting from memory
/// @param request 
void handleDeleteOldGauge(AsyncWebServerRequest * request){
  String content = F("<!DOCTYPE html>\n<html lang='en'>\n<head>\n<style>\n");
  content +=".button {";
  content +="  border: none;";
  content +="  color: white;";
  content +="  padding: 15px 32px;";
  content +="  text-align: center;";
  content +="  text-decoration: none;";
  content +="  display: inline-block;";
  content +="  font-size: 16px;";
  content +="  margin: 4px 2px;";
  content +="  cursor: pointer;";
  content +="}";
  content +=".button1 {background-color: #008CBA;}";
  content +="</style>";

  content += F("<a href='/'><input type='button' class='button button1' value='Home'></a>\n<h2>Object List</h2>\n<ul id='objectList'>\n");
  
  for (size_t i = 0; i < numberOfGauges; ++i) {
    content += "<li id='object" + String(i) + "'>Name:" + String(gauge[i]->getName()) + " Frame number" + String(gauge[i]->getFrame()) + 
               "<form action='/delete-gauge' method='post'>"+
               "<input type='hidden' id='" + String(gauge[i]->getName()) + "' value='" + String(i) + "' name='" + String(gauge[i]->getName()) + 
               "' class='setting-input'>" + 
               "<input type='submit' class='button button1' value='Delete'></li></form>";
  }

  content += F("</ul>\n</body>\n</html>");

  request->send(200, "text/html", content);
}

/// @brief Handler for gauge deleting from meemory
/// @param request 
void handleDeleteGauge(AsyncWebServerRequest * request){
  for (size_t i = 0; i < request->params(); i++) {
    AsyncWebParameter *param = request->getParam(i);
    if (param->value().toInt() >= 0 && param->value().toInt() < numberOfGauges) {
    if(gauge[param->value().toInt()]->getName() == param->name()){
        int id = param->value().toInt();
        delete gauge[id];
      
      for (size_t j = id; j < numberOfGauges - 1; ++j) {
        gauge[j] = gauge[j + 1];
      }
      int eepromAddress = 2 + (gaugedatasize*id);
      
      int toMove = numberOfGauges-(id+1);
      for (size_t i = 0; i < gaugedatasize; i++)
      {
        switch (toMove)
        {
        case 1:
          EEPROM.write(eepromAddress, EEPROM.read(eepromAddress+gaugedatasize));
          EEPROM.write(eepromAddress+gaugedatasize, 0);
          break;
        case 2:
          EEPROM.write(eepromAddress, EEPROM.read(eepromAddress+gaugedatasize));
          EEPROM.write(eepromAddress+gaugedatasize, EEPROM.read(eepromAddress+(2*gaugedatasize)));
          EEPROM.write(eepromAddress+(2*gaugedatasize), 0);
          break;
        case 3:
          EEPROM.write(eepromAddress, EEPROM.read(eepromAddress+gaugedatasize));
          EEPROM.write(eepromAddress+gaugedatasize, EEPROM.read(eepromAddress+(2*gaugedatasize)));
          EEPROM.write(eepromAddress+(2*gaugedatasize), EEPROM.read(eepromAddress+(3*gaugedatasize)));
          EEPROM.write(eepromAddress+(3*gaugedatasize), 0);
          break;
        case 4:
          EEPROM.write(eepromAddress, EEPROM.read(eepromAddress+gaugedatasize));
          EEPROM.write(eepromAddress+gaugedatasize, EEPROM.read(eepromAddress+(2*gaugedatasize)));
          EEPROM.write(eepromAddress+(2*gaugedatasize), EEPROM.read(eepromAddress+(3*gaugedatasize)));
          EEPROM.write(eepromAddress+(3*gaugedatasize), EEPROM.read(eepromAddress+(4*gaugedatasize)));
          EEPROM.write(eepromAddress+(4*gaugedatasize), 0);
          break;
        default:
          EEPROM.write(eepromAddress,0);
          break;
        }
        eepromAddress++;
      }
      
      // Decrement the array size
      numberOfGauges--;
      EEPROM.write(1, numberOfGauges);
      EEPROM.commit();

      // Respond to the client or perform any other necessary actions
      request->send(200, "text/plain", "Object deleted successfully.");
    } else {
      // Invalid id, respond with an error
      request->send(400, "text/plain", "Invalid id.");
    }
    }
  }
}

/// @brief Handler for gauge adding
/// @param request 
void handleAddGauge(AsyncWebServerRequest *request) {
  bool unitExists = false;
  bool gaugeNameExists = false;
  bool minValueExists = false;
  bool maxValueExists = false;
  bool criticalValueExists = false;
  bool frameNumberExists = false;
  bool msbByteNumberExists = false;
  bool lsbByteNumberExists = false;
  String unit = "";
  String gaugeName = "";
  int gaugeType = 0;
  int minValue = 0;
  int maxValue = 0;
  int criticalValue = 0;
  int frameNumber = 0;
  int msbByteNumber = 0;
  int lsbByteNumber = 0;

for (size_t i = 0; i < request->params(); i++) {
    AsyncWebParameter *param = request->getParam(i);
    if(param->name()=="unit"){
      unit=param->value();
      unitExists = true;
    }
    if(param->name()=="gauge-type"){
      gaugeType=param->value().toInt();
      Serial.println(gaugeType);
    }
    if(param->name()=="gauge-name"){
      gaugeName=param->value();
      gaugeNameExists = true;
    }
    if(param->name()=="min-value"){
      minValue=param->value().toInt();
      minValueExists = true;
    }
    if(param->name()=="max-value"){
      maxValue=param->value().toInt();
      maxValueExists = true;
    }
    if(param->name()=="critical-value"){
      criticalValue=param->value().toInt();
      criticalValueExists = true;
    }
    if(param->name()=="frame-number"){
      frameNumber=param->value().toInt();
      frameNumberExists = true;
    }
    if(param->name()=="byte-number-msb"){
      msbByteNumber=param->value().toInt();
      msbByteNumberExists = true;
    }
    if(param->name()=="byte-number-lsb"){
      lsbByteNumber=param->value().toInt();
      lsbByteNumberExists = true;
    }
  }
  if (unitExists&&gaugeNameExists&&frameNumberExists&&numberOfGauges<4) 
{
    // Create a new AnalogueGauge object and update it
    gauge[numberOfGauges] = new Gauge(&sprite1, &tft, unit, gaugeType);
    int eepromAddress = 3 + (gaugedatasize*numberOfGauges);
    gauge[numberOfGauges]->setName(gaugeName);
    gauge[numberOfGauges]->setBackgroundColor((uint16_t) rand());
    gauge[numberOfGauges]->setArcColors((uint16_t) rand());
    gauge[numberOfGauges]->setScaleColors((uint16_t) rand());
    gauge[numberOfGauges]->setTextColor(TFT_WHITE);
    gauge[numberOfGauges]->setNeedleColor((uint16_t) rand());
    gauge[numberOfGauges]->setFrame(frameNumber);
    gauge[numberOfGauges]->setByteMSB(msbByteNumber);
    gauge[numberOfGauges]->setByteLSB(lsbByteNumber);
    for(int i=0; i<10; i++){
      if(i<gaugeName.length())
        EEPROM.write(eepromAddress, gaugeName[i]);
      else
        EEPROM.write(eepromAddress, 0);
      eepromAddress++;
    }
    EEPROM.write(eepromAddress, frameNumber>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, frameNumber&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, msbByteNumber);
    eepromAddress++;
    EEPROM.write(eepromAddress, lsbByteNumber);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getBackgroundColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getBackgroundColor()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getArcColors()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getArcColors()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getScaleColors()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getScaleColors()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getTextColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getTextColor()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getNeedleColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getNeedleColor()&0xFF);
    eepromAddress++;
    for(int i=0; i<10; i++){
      if(i<unit.length())
        EEPROM.write(eepromAddress, unit[i]);
      else
        EEPROM.write(eepromAddress, 0);
      eepromAddress++;
    }
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getGaugeType());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getAnalogueGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getDigitalGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getUnitGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getAnalogueGaugeFontSize());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getDigitalGaugeFontSize());
    eepromAddress++;
    EEPROM.write(eepromAddress, gauge[numberOfGauges]->getUnitGaugeFontSize());
    numberOfGauges++;
    EEPROM.write(1, numberOfGauges);
    EEPROM.commit();
    request->send(200, "text/plain", "Gauge added successfully");
  } 
  else if (numberOfGauges==4)
  {
    request->send(400, "text/plain", "To much gauges");
  }
   {
    request->send(400, "text/plain", "Invalid parameters");
  }
}

void handleSetCanSpeed(AsyncWebServerRequest * request){
  for (size_t i = 0; i < request->params(); i++) {
    AsyncWebParameter *param = request->getParam(i);
    if (param->name()=="can-speed") {
      if(changeCanBusSpeed(param->value().toInt())==param->value().toInt()){
        EEPROM.write(2, param->value().toInt());
        EEPROM.commit();
        request->send(200, "text/plain", "BUS speed adjusted.");
      }
      else
      request->send(400, "text/plain", "BUS speed adjusted to 500kbps becouse of error.");
    }
    else {
      // Invalid id, respond with an error
      request->send(400, "text/plain", "Invalid id.");
    }
  }
}

void handleBrightness(AsyncWebServerRequest * request){
  for (size_t i = 0; i < request->params(); i++) {
    AsyncWebParameter *param = request->getParam(i);
    if (param->name()=="brightness") {
      analogWrite(TFT_BL, param->value().toInt()*2.5);
      request->send(200, "text/plain", "Brightness adjusted.");
    }
    else {
      // Invalid id, respond with an error
      request->send(400, "text/plain", "Invalid id.");
    }
  }
}

void formatEEPROM(){
  #if !FORMAT
  if(EEPROM.read(0)!=0){
    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
      EEPROM.write(i,0);
    }
    EEPROM.commit();
  }
  #else
    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
      EEPROM.write(i,0);
    }
    EEPROM.commit();
  #endif
}

void runGaugesFromEeprom(){
  int eepromAddress = 0;
  changeCanBusSpeed(EEPROM.read(2));
  if(EEPROM.read(1)!=0)
  for (size_t i = 0; i < EEPROM.read(1); i++)
  {
    gauge[i] = new Gauge(&sprite1, &tft);
    eepromAddress = 3 + (i*gaugedatasize);
    String text = "";

    for (size_t i = 0; i < 10; i++)
    {
      if(EEPROM.read(eepromAddress)>0)
      text += (char)EEPROM.read(eepromAddress);
      eepromAddress++;
    }
    gauge[i]->setName(text);
    gauge[i]->setFrame((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    gauge[i]->setByteMSB(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setByteLSB(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setBackgroundColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    gauge[i]->setArcColors((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    gauge[i]->setScaleColors((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    gauge[i]->setTextColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    gauge[i]->setNeedleColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    text="";
    for (size_t i = 0; i < 10; i++)
    {
      if(EEPROM.read(eepromAddress)>0)
      text += (char)EEPROM.read(eepromAddress);
      eepromAddress++;
    }
    gauge[i]->setUnit(text);
    gauge[i]->setGaugeType(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setAnalogueGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setDigitalGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setUnitGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setAnalogueGaugeFontSize(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setDigitalGaugeFontSize(EEPROM.read(eepromAddress));
    eepromAddress++;
    gauge[i]->setUnitGaugeFontSize(EEPROM.read(eepromAddress));
    eepromAddress++;
    numberOfGauges++;
  }
  else{
    tft.fillScreen(BACKGROUNDCOLOR_AUDI);
        tft.setTextSize(1);
        tft.drawString("There's no gauges to init", 10, tft.height()/2);
  }
}

void wifiInit(uint8_t pin){
  if(digitalRead(pin)==true){
    timer = millis();
    while(digitalRead(pin)==true){
      unsigned long remain = millis() - timer;
      if(remain%1000==0&&remain<5000){
        tft.fillScreen(BACKGROUNDCOLOR_AUDI);
        tft.setTextSize(1);
        tft.drawString("Initialising config mode in:", 30, tft.height()/2);
        tft.drawString(String((int)(5000-remain)/1000)+"s", tft.width()/2, tft.height()/2+20);
      }
      if(remain>5000){
          tft.fillScreen(BACKGROUNDCOLOR_AUDI);
          tft.drawString("Release to enter config mode", 30, tft.height()/2);
          while (digitalRead(pin)==true)
          {}
        }
    };
    if(millis()-timer>5000){
      String macAddress = WiFi.macAddress();
      macAddress.remove(0,9);
      macAddress.replace(":","");
      macAddress.toLowerCase();
      String _ssid = "Gauge-" + macAddress;
      String _password = randomStringGenerator(8);
      tft.setTextSize(2);
      tft.fillScreen(BACKGROUNDCOLOR_AUDI);
      tft.drawString("Log in to config", 30, tft.height()/2-40);
      tft.drawString(("SSID:" + _ssid), 20, tft.height()/2-20);
      tft.drawString(("PASS:" + _password), 40, tft.height()/2);
      
      
      WiFi.mode(WIFI_AP);                    // Changing ESP32 wifi mode to AccessPoint  
      WiFi.softAP(_ssid, _password);
      
      delay(100);
      tft.drawString(("IP:" + WiFi.softAPIP().toString()), 40, tft.height()/2+20);
      tft.setTextSize(1);
      tft.drawString("Press to exit", 90, tft.height()/2+40);
      while (digitalRead(pin)==false)
      {
        delay(100);
      }
    }
  }
  else{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
    
  }
}

void setup() {
  Serial.begin(115200);
  
  EEPROM.begin(EEPROM_SIZE);
  pinMode(18, INPUT_PULLDOWN);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  formatEEPROM();
  CAN.setPins(RX_GPIO, TX_GPIO);
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  Serial.println("CAN Started!");
  wifiInit(18);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  runGaugesFromEeprom();
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/add-gauge", HTTP_POST, handleAddGauge);
  server.on("/new-gauge", HTTP_GET, handleCreateNewGauge);
  server.on("/can-sniffer", HTTP_GET, handleCanSniffer);
  server.on("/old-gauge", HTTP_GET, handleDeleteOldGauge);
  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request) {
    String response = "{\"id\":" + String(packet.id) +
                      ",\"rtr\":" + String(packet.rtr) +
                      ",\"ide\":" + String(packet.ide) +
                      ",\"dlc\":" + String(packet.dlc) + ",";
    response += "\"dataArray\":[";
    for (int i = 0; i < packet.dlc; i++) {
      response += String(packet.dataArray[i]);
      if (i < packet.dlc - 1) {
        response += ",";
      }
    }
    response += "]}";

    request->send(200, "application/json", response);
  });
  server.on("/setbrightness", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerRequest * param = request;
    for (size_t i = 0; i < request->params(); i++) {
    if(param->getParam(i)->name()=="brightness")
      analogWrite(TFT_BL, param->getParam(i)->value().toInt());
    }
    String response = "";
    request->send(200, "application/json", response);
  });
  server.on("/delete-gauge", HTTP_POST, handleDeleteGauge);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/set-can-speed", HTTP_POST, handleSetCanSpeed);
  server.on("/set-brightness", HTTP_POST, handleBrightness);
  server.onNotFound(notFound);
  server.begin();
}

void update_screen(packet_t * _packet){
  if(numberOfGauges==0){}
  else{
    if(gauge[j]->getFrame()==_packet->id){
      if((gauge[j]->getByteMSB()!=8)&&(gauge[j]->getByteLSB()!=8)){
        int value = (_packet->dataArray[gauge[j]->getByteMSB()]<<8)+_packet->dataArray[gauge[j]->getByteLSB()];
        gauge[j]->setValue(value);
        gauge[j]->update();
      }
      else if((gauge[j]->getByteMSB()==8&&gauge[j]->getByteLSB()!=8)){
        int value = _packet->dataArray[gauge[j]->getByteLSB()];
        gauge[j]->setValue(value);
        gauge[j]->update();
      }
      else if((gauge[j]->getByteMSB()!=8&&gauge[j]->getByteLSB()==8)){
        int value = _packet->dataArray[gauge[j]->getByteMSB()];
        gauge[j]->setValue(value);
        gauge[j]->update();
      }
    }
  }  
}

void userInput(uint8_t pin, uint8_t restart_time){
 timer = millis();
 while(digitalRead(pin)){
    if ((millis() - timer)>restart_time*1000){
      ESP.restart(); 
    }
    else if ((millis() - timer)>2000){
      tft.fillScreen(BACKGROUNDCOLOR_AUDI);
      tft.setTextSize(2);
      tft.drawString("Restart in:", tft.width()/2-50, tft.height()/2);
      tft.drawString(String((int) restart_time-(millis()-timer)/1000), tft.width()/2, tft.height()/2+20);
    }
    else{
      update_screen(&packet);
    }
  }
  if(millis()-timer>50&&millis()-timer<2000){
    j++;
    if(j>=numberOfGauges){
      j=0;
    }
  }
}

void loop() 
{
  receiveData();
  update_screen(&packet);
  userInput(18,5);
}


 