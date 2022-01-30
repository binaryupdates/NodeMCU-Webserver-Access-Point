#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";    // Enter SSID here
const char* password = "12345";  // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

bool D1Pinstatus = LOW;
bool D2Pinstatus = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);  // green led
  pinMode(D2, OUTPUT);  // red led

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/d1on", handle_d1on);
  server.on("/d1off", handle_d1off);
  server.on("/d2on", handle_d2on);
  server.on("/d2off", handle_d2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();
  
  if(D1Pinstatus)
  {digitalWrite(D1, HIGH);}
  else
  {digitalWrite(D1, LOW);}
  
  if(D2Pinstatus)
  {digitalWrite(D2, HIGH);}
  else
  {digitalWrite(D2, LOW);}
}

void handle_OnConnect() {
  D1Pinstatus = LOW;
  D2Pinstatus = LOW;
  Serial.println("LED D1: OFF | LED D2: OFF");
  server.send(200, "text/html", updateWebpage(D1Pinstatus,D2Pinstatus)); 
}

void handle_d1on() {
  D1Pinstatus = HIGH;
  Serial.println("LED D1: ON");
  server.send(200, "text/html", updateWebpage(true,D2Pinstatus)); 
}

void handle_d1off() {
  D1Pinstatus = LOW;
  Serial.println("LED D1: OFF");
  server.send(200, "text/html", updateWebpage(false,D2Pinstatus)); 
}

void handle_d2on() {
  D2Pinstatus = HIGH;
  Serial.println("LED D2: ON");
  server.send(200, "text/html", updateWebpage(D1Pinstatus,true)); 
}

void handle_d2off() {
  D2Pinstatus = LOW;
  Serial.println("LED D2: OFF");
  server.send(200, "text/html", updateWebpage(D1Pinstatus,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String updateWebpage(uint8_t d1Pinstate,uint8_t d2Pinstate){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(d1Pinstate){
    ptr +="<p>GREEN LED: ON</p><a class=\"button button-off\" href=\"/d1off\">OFF</a>\n";
   }else{
    ptr +="<p>GREEN LED: OFF</p><a class=\"button button-on\" href=\"/d1on\">ON</a>\n";
  }
  
  if(d2Pinstate){
    ptr +="<p>RED LED: ON</p><a class=\"button button-off\" href=\"/d2off\">OFF</a>\n";
  }else{
    ptr +="<p>RED LED: OFF</p><a class=\"button button-on\" href=\"/d2on\">ON</a>\n";
  }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
