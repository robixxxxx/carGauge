//#include <CanPacketManager.h>
// #include "SPI.h"
#include <EEPROM.h>
#include <webSite.h>
#include <Gauge.h>
#include <LittleFS.h>
#include <simCAN.h>
// #include <ArduinoJson.h>
#define FORMAT 0
#define EEPROM_SIZE 512
#define RANDOM_CAN 1
const char *ssid = "FunBox3-EA22";
const char *password = "9LYXSMWWKFN7";
const char *characters[] = {"0123456789",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "abcdefghijklmnopqrstuvwxyz"};

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite1 = TFT_eSprite(&tft);

int gaugedatasize = 40;

uint8_t j=0;
static int spedometer_size = 0;
Gauge * spedometer[4];
String spedometer_name[4];
uint8_t speedAngle = 0;

packet_t packet;

uint32_t timer;
bool flag; 

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
  
  for (size_t i = 0; i < spedometer_size; ++i) {
    content += "<li id='object" + String(i) + "'>Name:" + String(spedometer[i]->getName()) + " Frame number" + String(spedometer[i]->getFrame()) + 
               "<form action='/delete-gauge' method='post'>"+
               "<input type='hidden' id='" + String(spedometer[i]->getName()) + "' value='" + String(i) + "' name='" + String(spedometer[i]->getName()) + 
               "' class='setting-input'>" + 
               "<input type='submit' class='button button1' value='Delete'></li></form>";
  }

  content += F("</ul>\n</body>\n</html>");

  request->send(200, "text/html", content);
}

void handleDeleteGauge(AsyncWebServerRequest * request){
  for (size_t i = 0; i < request->params(); i++) {
    AsyncWebParameter *param = request->getParam(i);
    Serial.println("Param Name");
    Serial.println(param->name());
    Serial.println("Param Value");
    Serial.println(param->value());
    Serial.println(spedometer[param->value().toInt()]->getName());
    Serial.println(param->name());
    if (param->value().toInt() >= 0 && param->value().toInt() < spedometer_size) {
    if(spedometer[param->value().toInt()]->getName() == param->name()){
        int id = param->value().toInt();
        delete spedometer[id];
      
      for (size_t j = id; j < spedometer_size - 1; ++j) {
        spedometer[j] = spedometer[j + 1];
      }
      int eepromAddress = 2 + (gaugedatasize*id);
      
      int toMove = spedometer_size-(id+1);
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
      spedometer_size--;
      EEPROM.write(1, spedometer_size);
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
  if (unitExists&&gaugeNameExists&&frameNumberExists&&spedometer_size<4) 
{
    // Create a new AnalogueGauge object and update it
    try
    {
      spedometer[spedometer_size] = new Gauge(&sprite1, &tft, unit, true);
    }
    catch(const std::exception& e)
    {
      Serial.println(e.what());
    }
    int eepromAddress = 2 + (gaugedatasize*spedometer_size);
    
    spedometer[spedometer_size]->setName(gaugeName);
    spedometer[spedometer_size]->setBackgroundColor((uint16_t) rand());
    spedometer[spedometer_size]->setArcColors((uint16_t) rand());
    spedometer[spedometer_size]->setScaleColors((uint16_t) rand());
    spedometer[spedometer_size]->setTextColor(TFT_WHITE);
    spedometer[spedometer_size]->setNeedleColor((uint16_t) rand());
    spedometer[spedometer_size]->setFrame(frameNumber);
    spedometer[spedometer_size]->setByteMSB(msbByteNumber);
    spedometer[spedometer_size]->setByteLSB(lsbByteNumber);
    spedometer[spedometer_size]->init();
    for(char i : gaugeName){
      EEPROM.write(eepromAddress, i);
      eepromAddress++;
    }
    if(gaugeName.length()-1<10){
      for (size_t i = 0; i < 10-gaugeName.length(); i++)
      {
        EEPROM.write(eepromAddress, 0);
        eepromAddress++;
      }
    }
    EEPROM.write(eepromAddress, frameNumber>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, frameNumber&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, msbByteNumber);
    eepromAddress++;
    EEPROM.write(eepromAddress, lsbByteNumber);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getBackgroundColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getBackgroundColor()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getArcColors()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getArcColors()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getScaleColors()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getScaleColors()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getTextColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getTextColor()&0xFF);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getNeedleColor()>>8);
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getNeedleColor()&0xFF);
    eepromAddress++;
    for(char i : unit){
      EEPROM.write(eepromAddress, i);
      eepromAddress++;
    }
    if(unit.length()-1<10){
      for (size_t i = 0; i < 10-unit.length(); i++)
      {
        EEPROM.write(eepromAddress, 0);
        eepromAddress++;
      }
    }
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getShowDigitalValue());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getAnalogueGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getDigitalGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getUnitGaugeFont());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getAnalogueGaugeFontSize());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getDigitalGaugeFontSize());
    eepromAddress++;
    EEPROM.write(eepromAddress, spedometer[spedometer_size]->getUnitGaugeFontSize());
    Serial.print(eepromAddress);
    spedometer_size++;
    EEPROM.write(1, spedometer_size);
    EEPROM.commit();
    request->send(200, "text/plain", "Gauge added successfully");
  } 
  else if (spedometer_size==4)
  {
    request->send(400, "text/plain", "To much gauges");
  }
   {
    request->send(400, "text/plain", "Invalid parameters");
  }
}

void formatEEPROM(){
  #if !FORMAT
  if(EEPROM.read(0)!=0){
    Serial.print("Formatuje");
    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
      Serial.print(".");
      EEPROM.write(i,0);
    }
    EEPROM.commit();
  }
  #else
    Serial.print("Formatuje");
    for (size_t i = 0; i < EEPROM_SIZE; i++)
    {
      Serial.print(".");
      EEPROM.write(i,0);
    }
    EEPROM.commit();
  #endif
}
void runGaugesFromEeprom(){
  int eepromAddress = 0;
  if(EEPROM.read(1)!=0)
  for (size_t i = 0; i < EEPROM.read(1); i++)
  {
    spedometer[i] = new Gauge(&sprite1, &tft);
    eepromAddress = 2 + (i*gaugedatasize);
    String text = "";
    for (size_t i = 0; i < 10; i++)
    {
      if(EEPROM.read(eepromAddress)>0)
      text += (char)EEPROM.read(eepromAddress);
      eepromAddress++;
    }
    spedometer[i]->setName(text);
    spedometer[i]->setFrame((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    spedometer[i]->setByteMSB(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setByteLSB(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setBackgroundColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    spedometer[i]->setArcColors((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    spedometer[i]->setScaleColors((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    spedometer[i]->setTextColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    spedometer[i]->setNeedleColor((EEPROM.read(eepromAddress)<<8)+EEPROM.read(++eepromAddress));
    eepromAddress++;
    text="";
    for (size_t i = 0; i < 10; i++)
    {
      if(EEPROM.read(eepromAddress)>0)
      text += (char)EEPROM.read(eepromAddress);
      eepromAddress++;
    }
    spedometer[i]->setUnit(text);
    spedometer[i]->setShowDigitalValue(EEPROM.read(eepromAddress)&0x01);
    eepromAddress++;
    spedometer[i]->setAnalogueGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setDigitalGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setUnitGaugeFont(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setAnalogueGaugeFontSize(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setDigitalGaugeFontSize(EEPROM.read(eepromAddress));
    eepromAddress++;
    spedometer[i]->setUnitGaugeFontSize(EEPROM.read(eepromAddress));
    Serial.print(eepromAddress);
    eepromAddress++;
    spedometer[i]->init();
    spedometer_size++;
  }
  
}

void wifiInit(uint8_t pin){
  if(digitalRead(pin)==true){
    timer = millis();
    while(digitalRead(pin)==true){
      unsigned long remain = millis() - timer;
      if(remain%1000==0)
        tft.fillScreen(BACKGROUNDCOLOR_AUDI);
        tft.setTextSize(1);
        tft.drawString("Initialising config mode in:", 10, tft.height()/2);
        tft.drawString(String((int)(5000-remain)/100)+"s", tft.width()/2, tft.height()/2+20);
        if(remain>5000){
          tft.drawString("Release to enter config mode", 10, tft.height()/2);
          while (digitalRead(34)==true)
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
      tft.drawString("Log in to config", 20, tft.height()/2-40);
      tft.drawString(("SSID:" + _ssid), 10, tft.height()/2-20);
      tft.drawString(("PASS:" + _password), 0, tft.height()/2);
      
      WiFi.softAP(ssid, _password);
      IPAddress local_IP(192, 168, 1, 1);
      IPAddress gateway(192, 168, 1, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.config(local_IP, gateway, subnet);
      delay(100);
      tft.drawString(("IP:" + WiFi.localIP().toString()), 10, tft.height()/2+20);
      tft.drawString("Press to exit config mode", 20, tft.height()/2+40);
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
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected");
    
  }
}

void onCANReceive(int packetSize){
  packet.id = CAN.packetId();
  packet.rtr = CAN.packetRtr() ? 1 : 0;
  packet.ide = CAN.packetExtended() ? 1 : 0;
  packet.dlc = CAN.packetDlc();
  byte i = 0;
  while (CAN.available()) {
    packet.dataArray[i++] = CAN.read();
    if (i >= (sizeof(packet.dataArray) / (sizeof(packet.dataArray[0])))) {
      break;
    }
  }
}

void canRun(uint32_t canSpeed=500E3){
#if RANDOM_CAN == 1
  randomSeed(12345);
  Serial.println("randomCAN Started");
#else
  if (!CAN.begin(canSpeed)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  // register the receive callback
  CAN.onReceive(onCANReceive);
  Serial.println("CAN RX TX Started");
#endif
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  pinMode(34, INPUT_PULLDOWN);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  // LittleFS.begin(true);
  formatEEPROM();
  
  
  
  wifiInit(34);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  runGaugesFromEeprom();
  canRun();
  CAN.onReceive(onCANReceive);
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
  server.on("/delete-gauge", HTTP_POST, handleDeleteGauge);
  server.onNotFound(notFound);
  server.begin();
}

void test_update_screen(packet_t * _packet){
  if(spedometer_size==0){}
  else{
    if(spedometer[j]->getFrame()==_packet->id){
      if((spedometer[j]->getByteMSB()!=8)&&(spedometer[j]->getByteLSB()!=8)){
        int value = _packet->dataArray[spedometer[j]->getByteMSB()]<<8+_packet->dataArray[spedometer[j]->getByteLSB()];
        spedometer[j]->setValue(value);
        spedometer[j]->update();
      }
      else if((spedometer[j]->getByteMSB()==8&&spedometer[j]->getByteLSB()!=8)){
        int value = _packet->dataArray[spedometer[j]->getByteLSB()];
        spedometer[j]->setValue(value);
        spedometer[j]->update();
      }
      else if((spedometer[j]->getByteMSB()!=8&&spedometer[j]->getByteLSB()==8)){
        int value = _packet->dataArray[spedometer[j]->getByteMSB()];
        spedometer[j]->setValue(value);
        spedometer[j]->update();
      }
    }
    
  }  
}

void rebootCheck(uint8_t pin, uint8_t restart_time){
 timer = millis();
 while(digitalRead(pin)){
    if ((millis() - timer)>restart_time*1000){
      ESP.restart(); 
    }
    else if ((millis() - timer)>2000){
      tft.fillScreen(BACKGROUNDCOLOR_AUDI);
      tft.setTextSize(2);
      tft.drawString("Restart in:", tft.width()/2, tft.height()/2);
      tft.drawString(String((int) restart_time-(millis()-timer)/1000), tft.width()/2, tft.height()/2+20);
    }
    else{
      test_update_screen(&packet);
    }
  }
  if(millis()-timer>50&&millis()-timer<2000){
    j++;
    if(j>=spedometer_size){
      j=0;
    }
  }
}

void loop() 
{
  rebootCheck(34,5);
  test_update_screen(&packet);
  #if RANDOM_CAN
  CANsimulate(&packet);
  #endif
}


 