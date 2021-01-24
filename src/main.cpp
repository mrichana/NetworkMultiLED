#include <Arduino.h>
#include <LittleFS.h>
// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid     = "mrichana";
const char* password = "2106009557";

// Set web server port number to 80
//WiFiServer server(80);
ESP8266WebServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputRedState = "off";
String outputYellowState = "off";
String outputGreenState = "off";
String outputBlueState = "off";

// Assign output variables to GPIO pins
const int outputRed = D5;
const int outputYellow = D6;
const int outputGreen = D7;
const int outputBlue = D8;

String html;

String filterHtml(String html) {
  String tmp = String(html);
  tmp.replace("{{red}}", (outputRedState == "off") ? "<a href=\"/red/on\"><button class=\"button buttonDarkRed\">O</button></a>" : "<a href=\"/red/off\"><button class=\"button buttonLightRed\">X</button></a>" );
  tmp.replace("{{yellow}}", (outputYellowState == "off") ? "<a href=\"/yellow/on\"><button class=\"button buttonDarkYellow\">O</button></a>" : "<a href=\"/yellow/off\"><button class=\"button buttonLightYellow\">X</button></a>" );
  tmp.replace("{{green}}", (outputGreenState == "off") ? "<a href=\"/green/on\"><button class=\"button buttonDarkGreen\">O</button></a>" : "<a href=\"/green/off\"><button class=\"button buttonLightGreen\">X</button></a>" );
  tmp.replace("{{blue}}", (outputBlueState == "off") ? "<a href=\"/blue/on\"><button class=\"button buttonDarkBlue\">O</button></a>" : "<a href=\"/blue/off\"><button class=\"button buttonLightBlue\">X</button></a>" );
  return tmp;
}

void handleRootPath() {
  server.send(200, "text/html", filterHtml(html).c_str());
}

void handleRedOn() {
  outputRedState = "on";
  digitalWrite(outputRed, HIGH);

  handleRootPath();
}

void handleRedOff() {
  outputRedState = "off";
  digitalWrite(outputRed, LOW);

  handleRootPath();
}

void handleYellowOn() {
  outputYellowState = "on";
  digitalWrite(outputYellow, HIGH);

  handleRootPath();
}

void handleYellowOff() {
  outputYellowState = "off";
  digitalWrite(outputYellow, LOW);

  handleRootPath();
}

void handleGreenOn() {
  outputGreenState = "on";
  digitalWrite(outputGreen, HIGH);

  handleRootPath();
}

void handleGreenOff() {
  outputGreenState = "off";
  digitalWrite(outputGreen, LOW);

  handleRootPath();
}

void handleBlueOn() {
  outputBlueState = "on";
  digitalWrite(outputBlue, HIGH);

  handleRootPath();
}

void handleBlueOff() {
  outputBlueState = "off";
  digitalWrite(outputBlue, LOW);

  handleRootPath();
}

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(outputRed, OUTPUT);
  pinMode(outputYellow, OUTPUT);
  pinMode(outputGreen, OUTPUT);
  pinMode(outputBlue, OUTPUT);
  // Set outputs to LOW
  digitalWrite(outputRed, LOW);
  digitalWrite(outputYellow, LOW);
  digitalWrite(outputGreen, LOW);
  digitalWrite(outputBlue, LOW);

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  File file = LittleFS.open("/index.html", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    html = file.readString();
  }
  file.close();

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRootPath);
  server.on("/red/on", handleRedOn);
  server.on("/red/off", handleRedOff);
  server.on("/yellow/on", handleYellowOn);
  server.on("/yellow/off", handleYellowOff);
  server.on("/green/on", handleGreenOn);
  server.on("/green/off", handleGreenOff);
  server.on("/blue/on", handleBlueOn);
  server.on("/blue/off", handleBlueOff);
  server.begin();
}

void loop() {
  server.handleClient();
}
// void loop(){
//   WiFiClient client = server.available();   // Listen for incoming clients

//   if (client) {                             // If a new client connects,
//     Serial.println("New Client.");          // print a message out in the serial port
//     String currentLine = "";                // make a String to hold incoming data from the client
//     currentTime = millis();
//     previousTime = currentTime;
//     while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
//       currentTime = millis();         
//       if (client.available()) {             // if there's bytes to read from the client,
//         char c = client.read();             // read a byte, then
//         Serial.write(c);                    // print it out the serial monitor
//         header += c;
//         if (c == '\n') {                    // if the byte is a newline character
//           // if the current line is blank, you got two newline characters in a row.
//           // that's the end of the client HTTP request, so send a response:
//           if (currentLine.length() == 0) {
//             // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//             // and a content-type so the client knows what's coming, then a blank line:
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println("Connection: close");
//             client.println();
            
//             // turns the GPIOs on and off
//             if (header.indexOf("GET /red/on") >= 0) {
//               Serial.println("Red LED on");
//               outputRedState = "on";
//               digitalWrite(outputRed, HIGH);
//             } else if (header.indexOf("GET /red/off") >= 0) {
//               Serial.println("Red LED off");
//               outputRedState = "off";
//               digitalWrite(outputRed, LOW);
//             } else if (header.indexOf("GET /yellow/on") >= 0) {
//               Serial.println("Yellow LED on");
//               outputYellowState = "on";
//               digitalWrite(outputYellow, HIGH);
//             } else if (header.indexOf("GET /yellow/off") >= 0) {
//               Serial.println("Yellow LED off");
//               outputYellowState = "off";
//               digitalWrite(outputYellow, LOW);
//             } else if (header.indexOf("GET /green/on") >= 0) {
//               Serial.println("Green LED on");
//               outputGreenState = "on";
//               digitalWrite(outputGreen, HIGH);
//             } else if (header.indexOf("GET /green/off") >= 0) {
//               Serial.println("Green LED off");
//               outputGreenState = "off";
//               digitalWrite(outputGreen, LOW);
//             } else if (header.indexOf("GET /blue/on") >= 0) {
//               Serial.println("Blue LED on");
//               outputBlueState = "on";
//               digitalWrite(outputBlue, HIGH);
//             } else if (header.indexOf("GET /blue/off") >= 0) {
//               Serial.println("Blue LED off");
//               outputBlueState = "off";
//               digitalWrite(outputBlue, LOW);
//             }
            
//             // Display the HTML web page
//             client.println("<!DOCTYPE html><html>");
//             client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
//             client.println("<link rel=\"icon\" href=\"data:,\">");
//             // CSS to style the on/off buttons 
//             // Feel free to change the background-color and font-size attributes to fit your preferences
//             client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; color: white; background-color:#202040; }");
//             client.println(".button { border: none; color: white; padding: 16px 40px;");
//             client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
//             client.println(".buttonDarkRed {background-color: #770000;}");
//             client.println(".buttonLightRed {background-color: #FF0000;}");
//             client.println(".buttonDarkYellow {background-color: #777700;}");
//             client.println(".buttonLightYellow {background-color: #FFFF00;}");
//             client.println(".buttonDarkGreen {background-color: #007700;}");
//             client.println(".buttonLightGreen {background-color: #00FF00;}");
//             client.println(".buttonDarkBlue {background-color: #000077;}");
//             client.println(".buttonLightBlue {background-color: #0000FF;}");
//             client.println("</style></head>");
            
//             // Web Page Heading
//             client.println("<body><h1>MultiLED</h1>");
            
//             // Display current state, and ON/OFF buttons for Red
//             // If the outputRedState is off, it displays the ON button       
//             if (outputRedState=="off") {
//               client.println("<p><a href=\"/red/on\"><button class=\"button buttonDarkRed\">O</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/red/off\"><button class=\"button buttonLightRed\">X</button></a></p>");
//             } 
               
//             // If the outputYellowState is off, it displays the ON button       
//             if (outputYellowState=="off") {
//               client.println("<p><a href=\"/yellow/on\"><button class=\"button buttonDarkYellow\">O</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/yellow/off\"><button class=\"button buttonLightYellow\">X</button></a></p>");
//             }
               
//             // If the outputGreenState is off, it displays the ON button       
//             if (outputGreenState=="off") {
//               client.println("<p><a href=\"/green/on\"><button class=\"button buttonDarkGreen\">O</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/green/off\"><button class=\"button buttonLightGreen\">X</button></a></p>");
//             }
               
//             // If the outputBlueState is off, it displays the ON button       
//             if (outputBlueState=="off") {
//               client.println("<p><a href=\"/blue/on\"><button class=\"button buttonDarkBlue\">O</button></a></p>");
//             } else {
//               client.println("<p><a href=\"/blue/off\"><button class=\"button buttonLightBlue\">X</button></a></p>");
//             }

//             client.println("</body></html>");
            
//             // The HTTP response ends with another blank line
//             client.println();
//             // Break out of the while loop
//             break;
//           } else { // if you got a newline, then clear currentLine
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }
//       }
//     }
//     // Clear the header variable
//     header = "";
//     // Close the connection
//     client.stop();
//     Serial.println("Client disconnected.");
//     Serial.println("");
//   }
// }