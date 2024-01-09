// #include <CanPacketManager.h>
// #include "SPI.h"
#include <ESPAsyncWebServer.h>
#include <Gauge.h>

const char *ssid = "FunBox3-EA22";
const char *password = "9LYXSMWWKFN7";
const char *characters[] = {"0123456789",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "abcdefghijklmnopqrstuvwxyz"};
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite1 = TFT_eSprite(&tft);
AsyncWebServer server(80);
int j=0;
static int spedometer_size = 0;
Gauge * spedometer[10];
String spedometer_name[10];

int speedAngle = 0;

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

void notFound(AsyncWebServerRequest *request) {
  String content = "<html lang=\"en\">"; 
content += "<head>"; 
content += "<meta charset=\"UTF-8\">"; 
content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"; 
content += "<title>404 NOT FOUND</title>"; 
content += "</head>"; 
content += "<a href='/'><input type='button' value='Home'></a>";  
content += "<body>"; 
content += "<h1>404 NOT FOUND</h1>"; 
content += "</body>"; 
content += "</html>";

request->send(404, "text/html", content);
}
void handleCanSniffer(AsyncWebServerRequest * request){
String content =  "<!DOCTYPE html>"; 
content += "<html lang=\"en\">"; 
content += "<head>"; 
content += "<meta charset=\"UTF-8\">"; 
content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"; 
content += "<title>CAN BUS Data Display</title>"; 
content += "<style>"; 
content += "body {"; 
content += "background-color: #1e1e1e;"; 
content += "color: #ffffff;"; 
content += "font-family: Arial, sans-serif;"; 
content += "margin: 0;"; 
content += "padding: 0;"; 
content += "display: flex;"; 
content += "justify-content: center;"; 
content += "align-items: center;"; 
content += "height: 100vh;"; 
content += "}"; 
content += "iframe {"; 
content += "width: 100%;"; 
content += "height: 100%;"; 
content += "border: none;"; 
content += "}"; 
content += "table {"; 
content += "width: 100%;"; 
content += "border-collapse: collapse;"; 
content += "margin: 20px;"; 
content += "background-color: #2d2d2d;"; 
content += "color: #ffffff;"; 
content += "}"; 
content += "table, th, td {"; 
content += "border: 1px solid #ffffff;"; 
content += "}"; 
content += "th, td {"; 
content += "padding: 10px;"; 
content += "text-align: left;"; 
content += "transition: background-color 1s;"; 
content += "}"; 
content += "th, td.changed {"; 
content += "background-color: #ffcc00; /* Kolor tła dla zmienionych danych */"; 
content += "}"; 
content += "#filter-input, #display-mode {"; 
content += "width: 100%;"; 
content += "padding: 10px;"; 
content += "background-color: #2d2d2d;"; 
content += "color: #ffffff;"; 
content += "border: 1px solid #ffffff;"; 
content += "margin-bottom: 10px;"; 
content += "}"; 
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/'><input type='button' value='Home'></a>";  
content += "<div>"; 
content += "<label for=\"display-mode\">Display Mode:</label>"; 
content += "<select id=\"display-mode\" onchange=\"changeDisplayMode()\">"; 
content += "<option value=\"hex\">Hex</option>"; 
content += "<option value=\"dec\">Dec</option>"; 
content += "<option value=\"bin\">Bin</option>"; 
content += "</select>"; 
content += "</div>"; 
content += "<div>"; 
content += "<label for=\"filter-input\">Filter by ID:</label>"; 
content += "<input type=\"text\" id=\"filter-input\" oninput=\"filterData()\">"; 
content += "</div>"; 
content += "<table id=\"data-table\">"; 
content += "<thead>"; 
content += "<tr>"; 
content += "<th>ID</th>"; 
content += "<th>RTR</th>"; 
content += "<th>IDE</th>"; 
content += "<th>D0</th>"; 
content += "<th>D1</th>"; 
content += "<th>D2</th>"; 
content += "<th>D3</th>"; 
content += "<th>D4</th>"; 
content += "<th>D5</th>"; 
content += "<th>D6</th>"; 
content += "<th>D7</th>"; 
content += "</tr>"; 
content += "</thead>"; 
content += "<tbody>"; 
content += "</tbody>"; 
content += "</table>"; 
content += "<script>"; 
content += "var displayMode = \"hex\";"; 
content += "function changeDisplayMode() {"; 
content += "displayMode = document.getElementById(\"display-mode\").value;"; 
content += "filterData();"; 
content += "}"; 
content += "function updateTable(id, rtr, ide, data) {"; 
content += "var filterValue = document.getElementById(\"filter-input\").value.toLowerCase();"; 
content += "if (id.toString().toLowerCase().includes(filterValue)) {"; 
content += "var table = document.getElementById(\"data-table\");"; 
content += "var tbody = table.getElementsByTagName(\"tbody\")[0];"; 
content += "var rows = tbody.getElementsByTagName(\"tr\");"; 
content += "for (var i = 0; i < rows.length; i++) {"; 
content += "var cells = rows[i].getElementsByTagName(\"td\");"; 
content += "if (cells[0].innerHTML === id.toString()) {"; 
content += "cells[1].innerHTML = rtr;"; 
content += "cells[2].innerHTML = ide;"; 
content += "for (var j = 0; j < data.length; j++) {"; 
content += "var oldValue = cells[j + 3].innerHTML;"; 
content += "cells[j + 3].innerHTML = formatData([data[j]]);"; 
content += "if (oldValue !== cells[j + 3].innerHTML) {"; 
content += "cells[j + 3].classList.add(\"changed\");"; 
content += "setTimeout(function(cell) {"; 
content += "cell.classList.remove(\"changed\");"; 
content += "}, 1000, cells[j + 3]);"; 
content += "}"; 
content += "}"; 
content += "return;"; 
content += "}"; 
content += "}"; 
content += "var newRow = tbody.insertRow();"; 
content += "var cell1 = newRow.insertCell(0);"; 
content += "var cell2 = newRow.insertCell(1);"; 
content += "var cell3 = newRow.insertCell(2);"; 
content += "cell1.innerHTML = id;"; 
content += "cell2.innerHTML = rtr;"; 
content += "cell3.innerHTML = ide;"; 
content += "for (var j = 0; j < data.length; j++) {"; 
content += "var cell = newRow.insertCell(j + 3);"; 
content += "cell.innerHTML = formatData([data[j]]);"; 
content += "}"; 
content += "}"; 
content += "}"; 
content += "function byteArrayToString(bytes) {"; 
content += "var hexString = Array.from(bytes, function(byte) {"; 
content += "return ('0' + (byte & 0xFF).toString(16)).slice(-2);"; 
content += "}).join(' ');"; 
content += "var decString = bytes.join(' ');"; 
content += "var binString = Array.from(bytes, function(byte) {"; 
content += "return ('00000000' + byte.toString(2)).slice(-8);"; 
content += "}).join(' ');"; 
content += "return { hex: hexString, dec: decString, bin: binString };"; 
content += "}"; 
content += "function formatData(data) {"; 
content += "var dataString = byteArrayToString(data);"; 
content += "switch (displayMode) {"; 
content += "case \"hex\":"; 
content += "return dataString.hex;"; 
content += "case \"dec\":"; 
content += "return dataString.dec;"; 
content += "case \"bin\":"; 
content += "return dataString.bin;"; 
content += "default:"; 
content += "return dataString.hex;"; 
content += "}"; 
content += "}"; 
content += "function filterData() {"; 
content += "var filterValue = document.getElementById(\"filter-input\").value.toLowerCase();"; 
content += "var table = document.getElementById(\"data-table\");"; 
content += "var tbody = table.getElementsByTagName(\"tbody\")[0];"; 
content += "var rows = tbody.getElementsByTagName(\"tr\");"; 
content += "for (var i = 0; i < rows.length; i++) {"; 
content += "var cells = rows[i].getElementsByTagName(\"td\");"; 
content += "var id = cells[0].innerHTML.toLowerCase();"; 
content += "rows[i].style.display = id.includes(filterValue) ? \"\" : \"none\";"; 
content += "}"; 
content += "}"; 
content += "</script>"; 
content += "</body>"; 
content += "</html>";
request->send(200, "text/html", content);
}
void handleDeleteOldGauge(AsyncWebServerRequest * request){
  String content = F("<!DOCTYPE html>\n<html lang='en'>\n<head>\n");
  // Add other head elements here
  content += F("</head>\n<body>\n<h2>Object List</h2>\n<ul id='objectList'>\n");

  for (size_t i = 0; i < spedometer_size; ++i) {
    content += "<li>" + String(spedometer_name[i]) + "<input type='submit' id=" + i + " value='Delete'>";
  }

  content += F("</ul>\n<script>\nfunction deleteObject(id) {\n");
  content += F("  var listItem = document.getElementById('object' + id);\n");
  content += F("  listItem.parentNode.removeChild(listItem);\n");
  content += F("}\n</script>\n</body>\n</html>");
  request->send(200, "text/html", content);
}
void handleCreateNewGauge(AsyncWebServerRequest *request){
String content = "<!-- settings.html -->";
content += "<!DOCTYPE html>"; 
content += "<html lang='en'>"; 
content += "<head>"; 
content += "<meta charset='UTF-8'>"; 
content += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"; 
content += "<title>Gauge Management</title>"; 
content += "<style>"; 
content += "body {"; 
content += "background-color: #1e1e1e;"; 
content += "color: #ffffff;"; 
content += "font-family: Arial, sans-serif;"; 
content += "margin: 0;"; 
content += "padding: 20px;"; 
content += "}"; 
content += "h2 {"; 
content += "margin-bottom: 20px;"; 
content += "}"; 
content += "label {"; 
content += "display: block;"; 
content += "margin-bottom: 10px;"; 
content += "}"; 
content += ".setting-input {"; 
content += "width: 100%;"; 
content += "padding: 10px;"; 
content += "background-color: #2d2d2d;"; 
content += "color: #ffffff;"; 
content += "border: 1px solid #ffffff;"; 
content += "margin-bottom: 10px;"; 
content += "}"; 
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/'><input type='button' value='Home'></a>";  
content += "<h2>Create New Gauge Face</h2>"; 
content += "<label for='gauge-type'>Gauge Type</label>"; 
content += "<select id='gauge-type' name='gauge-type' class='setting-input'>"; 
content += "<option value='1'>Analogue Gauge</option>"; 
content += "<option value='2'>Digital Gauge</option>"; 
content += "</select>"; 
content += "<form action='/add-gauge' method='post'>"; 
content += "<label for='gauge-name'>Gauge Name:</label>"; 
content += "<input type='text' id='gauge-name' name='gauge-name' class='setting-input'>"; 
content += "<label for='min-value'>Minimum Value:</label>"; 
content += "<input type='number' id='min-value' name='min-value' class='setting-input' value='0'>"; 
content += "<label for='max-value'>Maximum Value:</label>"; 
content += "<input type='number' id='max-value' name='max-value' class='setting-input' value='240'>"; 
content += "<label for='critical-value'>Critical Value:</label>"; 
content += "<input type='number' id='critical-value' name='critical-value' class='setting-input'>"; 
content += "<label for='unit'>Unit:</label>"; 
content += "<input type='text' id='unit' name='unit' class='setting-input'><br>"; 
content += "<label for='frame-number'>Frame Number:</label>"; 
content += "<input type='number' id='frame-number' name='frame-number' class='setting-input'>"; 
content += "<label for='byte-number'>MSB Byte Number:</label>"; 
content += "<input type='number' id='byte-number-msb' name='byte-number-msb' class='setting-input'>"; 
content += "<label for='byte-number'>LSB Byte Number:</label>"; 
content += "<input type='number' id='byte-number-lsb' name='byte-number-lsb' class='setting-input'>"; 
content += "<label for='value-size'>Value Size:</label>"; 
content += "<input type='submit' value='Add Gauge'>"; 
content += "</form>"; 
content += "</body>"; 
content += "</html>";
request->send(200, "text/html", content);
}

void handleRoot(AsyncWebServerRequest *request) {
  String content = "<!DOCTYPE html>"; 
content += "<html lang=\"en\">"; 
content += "<head>"; 
content += "<meta charset=\"UTF-8\">"; 
content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"; 
content += "<title>Main HTML</title>"; 
content += "<style>"; 
content += "body {"; 
content += "background-color: #1e1e1e;";
content += "color: #ffffff;";
content += "margin: center;"; 
content += "padding: 0;"; 
content += "font-family: Arial, sans-serif;"; 
content += "}"; 
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/new-gauge'><input type='button' value='Create New Gauge Face'></a>"; 
content += "<a href='/old-gauge'><input type='button' value='Delete Gauge Faces'></a>"; 
content += "<a href='/settings'><input type='button' value='Settings'></a>"; 
content += "<a href='/can-sniffer'><input type='button' value='CanSniffer'></a>";  
content += "</body>"; 
content += "</html>";
  request->send(200, "text/html", content);
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
  if (unitExists&&gaugeNameExists) 
{
    // Create a new AnalogueGauge object and update it
    spedometer[spedometer_size] = new Gauge(&sprite1, 120, 120, 120, 100, 5, 220, unit);
    spedometer[spedometer_size]->init();
    spedometer_name[spedometer_size] = gaugeName;
    spedometer[spedometer_size]->setBackgroundColor((uint16_t) rand());
    spedometer[spedometer_size]->setArcColors((uint16_t) rand());
    spedometer[spedometer_size]->setScaleColors((uint16_t) rand());
    spedometer[spedometer_size]->setTextColor(TFT_WHITE);
    spedometer[spedometer_size]->setNeedleColor((uint16_t) rand());
    spedometer_size++;
    
    request->send(200, "text/plain", "Gauge added successfully");
  } else {
    request->send(400, "text/plain", "Invalid parameters");
  }
}



void setup() {
  Serial.begin(115200);
  pinMode(34, INPUT_PULLDOWN);
  pinMode(35, INPUT_PULLDOWN);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(BACKGROUNDCOLOR_AUDI);
  if(digitalRead(34)==true){
    String macAddress = WiFi.macAddress();
    String _ssid = "Gauge" + macAddress.substring(macAddress.length()/2,macAddress.length()-1);
    String _password = randomStringGenerator(8);
    tft.setTextSize(2);
    tft.drawString("Log in to config", 10, tft.height()/2-20);
    tft.drawString(("SSID:" + _ssid), 0, tft.height()/2);
    tft.drawString(("PASS:" + _password), 10, tft.height()/2+20);
    WiFi.softAP("Gauge"+WiFi.macAddress(), password);
  }
  else{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected");
  }
  
  

  

  server.on("/", HTTP_GET, handleRoot);
  server.on("/add-gauge", HTTP_POST, handleAddGauge);
  server.on("/new-gauge", HTTP_GET, handleCreateNewGauge);
  server.on("/can-sniffer", HTTP_GET, handleCanSniffer);
  server.on("/old-gauge", HTTP_GET, handleDeleteOldGauge);
  // server.on("/delete-gauge", HTTP_POST, handleDeleteGauge);

  server.onNotFound(notFound);
  server.begin();
}

// void updateEncoder(){
//   int MSB = digitalRead(PinA); //MSB = most significant bit
//   int LSB = digitalRead(PinB); //LSB = least significant bit

//   int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
//   int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

//   if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos ++;
//   if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos --;

//   lastEncoded = encoded; //store this value for next time
// }
unsigned long long timer;
bool flag;

void loop() 
{
 

  if(spedometer_size==0){}
  else{
  for (size_t i = 0; i < 100; i++)
  {
      if(digitalRead(34)==true){
        j++;
        if(j>spedometer_size-1){
          j=0;
        }
      while(digitalRead(34)==true){};
      }
      speedAngle++;
      if (speedAngle > 260)
        speedAngle = 0;
      spedometer[j]->setValue(speedAngle);
      spedometer[j]->update();
    }
  }
  
}


 // packetManager.CANsimulate();