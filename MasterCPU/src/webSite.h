#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  String content = "<html lang=\"en\">"; 
content += "<head>"; 
content += "<meta charset=\"UTF-8\">"; 
content += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"; 
content += "<title>404 NOT FOUND</title>"; 
content += "<style>"; 
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
content += "</style>"; 
content += "</head>"; 
content += "<a href='/'><input type='button' class='button button1' value='Home'></a>";  
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
content += "#filter-input, #display-mode, #toggle-color {"; 
content += "width: 100%;"; 
content += "padding: 10px;"; 
content += "background-color: #2d2d2d;"; 
content += "color: #ffffff;"; 
content += "border: 1px solid #ffffff;"; 
content += "margin-bottom: 10px;"; 
content += "}"; 
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
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/'><input type='button' class='button button1' value='Home'></a><br/>";  
content += "<div>"; 
content += "<label for=\"display-mode\">Display Mode:</label>"; 
content += "<select id=\"display-mode\" onchange=\"changeDisplayMode()\">"; 
content += "<option value=\"hex\">Hex</option>"; 
content += "<option value=\"dec\">Dec</option>"; 
content += "<option value=\"bin\">Bin</option>"; 
content += "</select>"; 
content += "</div><br/>"; 
content += "<div>"; 
content += "<label for=\"filter-input\">Filter by ID:</label>"; 
content += "<input type=\"text\" id=\"filter-input\" oninput=\"filterData()\">"; 
content += "</div><br/>"; 
content += "<div>"; 
content += "<label for=\"toggle-color\">Toggle Color:</label>"; 
content += "<input type=\"checkbox\" id=\"toggle-color\" onclick=\"toggleColoring()\">"; 
content += "</div><br/>"; 
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
content += "var colorEnabled = true;"; 

content += "function changeDisplayMode() {"; 
content += "displayMode = document.getElementById(\"display-mode\").value;"; 
content += "filterData();"; 
content += "}"; 

content += "function toggleColoring() {"; 
content += "colorEnabled = !colorEnabled;"; 
content += "filterData();"; 
content += "}"; 

content += "function updateTable(id, rtr, ide, dlc ,data) {";
content += "var filterValue = document.getElementById('filter-input').value.toLowerCase();";
content += "if (id.toString().toLowerCase().includes(filterValue)) {";
content += "var table = document.getElementById('data-table');";
content += "var tbody = table.getElementsByTagName('tbody')[0];";
content += "var rows = tbody.getElementsByTagName('tr');";
content += "for (var i = 0; i < rows.length; i++) {";
content += "var cells = rows[i].getElementsByTagName('td');";
content += "if (cells[0].innerHTML === id.toString()) {";
content += "cells[1].innerHTML = rtr;";
content += "cells[2].innerHTML = ide;";
content += "for (var j = 0; j < dlc; j++) {";
content += "var oldValue = cells[j + 3].innerHTML;";
content += "var backgroundColor = colorEnabled ? calculateBackgroundColor(data[j]) : 'transparent';";
content += "cells[j + 3].style.background = backgroundColor;";
content += "cells[j + 3].innerHTML = formatData([data[j]]);";
content += "if (oldValue !== cells[j + 3].innerHTML) {";
content += "cells[j + 3].classList.add('changed');";
content += "setTimeout(function(cell) {";
content += "cell.classList.remove('changed');";
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
content += "for (var j = 0; j < dlc; j++) {";
content += "var cell = newRow.insertCell(j + 3);";
content += "var backgroundColor = colorEnabled ? calculateBackgroundColor(data[j]) : 'transparent';";
content += "cell.style.background = backgroundColor;";
content += "cell.innerHTML = formatData([data[j]]);";
content += "}";
content += "}";
content += "}";
content += "function calculateBackgroundColor(value) {";
content += "var percentage = (value / 255) * 100;";
content += "var red = Math.floor((255 * percentage) / 100);";
content += "var green = Math.floor(255 - (255 * percentage) / 100);";
content += "var blue = 0;";
content += "return 'rgb(' + red + ',' + green + ',' + blue + ')';";
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
content += "function fetchData() {";
content += "var xhr = new XMLHttpRequest();";
content += "xhr.onreadystatechange = function() {";
content += "if (xhr.readyState == 4 && xhr.status == 200) {";
content += "var data = JSON.parse(xhr.responseText);";
content += "updateTable(data.id, data.rtr, data.ide, data.dlc, data.dataArray);";
content += "}";
content += "};";
content += "xhr.open('GET', '/getData', true);";
content += "xhr.send();";
content += "}";
content += "setInterval(fetchData, 10); // Periodically fetch data every 1000 ms (1 second)";
content += "</script>"; 
content += "</body>"; 
content += "</html>";

request->send(200, "text/html", content);




}

void handleSettings(AsyncWebServerRequest *request){
String content =  "<!DOCTYPE html>";
content += "<html lang='en'>";
content += "<head>";
content += "<meta charset='UTF-8'>";
content += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
content += "<title>Settings</title>";
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
content += ".button {";
content += "border: none;";
content += "color: white;";
content += "padding: 15px 32px;";
content += "text-align: center;";
content += "text-decoration: none;";
content += "display: inline-block;";
content += "font-size: 16px;";
content += "margin: 4px 2px;";
content += "cursor: pointer;";
content += "}";
content += ".button1 {";
content += "background-color: #008CBA;";
content += "}";
content += "</style>";
content += "</head>";
content += "<body>";
content += "<h2>Settings</h2>";
content += "<form action='/setbrightness' method='post'>";
content += "<label for='brightness'>Screen Brightness (%):</label>";
content += "<input type='number' id='brightness' name='brightness' class='setting-input' min='0' max='100' required>";
content += "<button type='submit' class='button button1'>Set Brightness</button>";
content += "</form>";
content += "<form action='/set-can-speed' method='post'>";
content += "<label for='can-speed'>CAN Bus Speed:</label>";
content += "<select id='can-speed' name='can-speed' class='setting-input' required>";
content += "<option value='1'>10 kbps</option>";
content += "<option value='2'>20 kbps</option>";
content += "<option value='3'>33.3 kbps</option>";
content += "<option value='4'>50 kbps</option>";
content += "<option value='5'>83.3 kbps</option>";
content += "<option value='6'>100 kbps</option>";
content += "<option value='7'>125 kbps</option>";
content += "<option value='8'>250 kbps</option>";
content += "<option value='9'>500 kbps</option>";
content += "<option value='10'>1 Mbps</option>";
content += "  <!-- Add more options as needed -->";
content += "</select>";
content += "<button type='submit' class='button button1'>Set CAN Speed</button>";
content += "</form>";
content += "<a href='/'><input type='button' class='button button1' value='Home'></a>";
content += "</body>";
content += "</html>";
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
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/'><input type='button' class='button button1' value='Home'></a>";  
content += "<h2>Create New Gauge Face</h2>"; 
content += "<form action='/add-gauge' method='post'>";
content += "<label for='gauge-type'>Gauge Type</label>"; 
content += "<select id='gauge-type' name='gauge-type' class='setting-input'>"; 
content += "<option value='1'>Mixed Gauge</option>";
content += "<option value='2'>Analogue Gauge</option>"; 
content += "<option value='3'>Digital Gauge</option>"; 
content += "</select>";  
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
content += "<input type='number' id='byte-number-msb' name='byte-number-msb' min='0' max='8' value='8' class='setting-input'>"; 
content += "<p>If you dont want to use MSB value left 8</p>";
content += "<label for='byte-number'>LSB Byte Number:</label>"; 
content += "<input type='number' id='byte-number-lsb' name='byte-number-lsb' min='0' max='8' value='8' class='setting-input'>"; 
content += "<p>If you dont want to use LSB value left 8</p>";
content += "<button type='submit' class='button button1' value='Add Gauge'>Add Gauge</button>"; 
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
content += "</style>"; 
content += "</head>"; 
content += "<body>"; 
content += "<a href='/new-gauge'><input type='button' class='button button1' value='Create New Gauge Face'></a>"; 
content += "<a href='/old-gauge'><input type='button' class='button button1' value='Delete Gauge Faces'></a>"; 
content += "<a href='/settings'><input type='button' class='button button1' value='Settings'></a>"; 
content += "<a href='/can-sniffer'><input type='button' class='button button1' value='CanSniffer'></a>";  
content += "</body>"; 
content += "</html>";
  request->send(200, "text/html", content);
}