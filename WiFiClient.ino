#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#ifndef STASSID
#define STASSID ""
#define STAPSK  ""
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

String estadoLed = "off";

ESP8266WebServer server(80);

IPAddress ip(192,168,1,240);
IPAddress gateway(192,168,1,1);
IPAddress netmask(255,255,255,0);

void handleRoot() {

  String content = "<!DOCTYPE html><html>";
        content += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
        content += "<link rel=\"icon\" href=\"data:,\">";
        content += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
        content += ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;";
        content += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
        content += ".button2 {background-color: #77878A;}</style></head>";
        content += "<body><h1> Web Server</h1>";
        content += "<p>LED : " + estadoLed + "</p>";   
        if (estadoLed=="off") {
          content += "<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>";
        } else {
          content += "<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>";
        }           
        content += "</body></html>";
  server.send(200, "text/html", content);

}

void enciendeLed() {
  digitalWrite(2, LOW);
  //server.send(200, "text/plain", "Enciendo Led");
  estadoLed = "on";
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}
void apagaLed() {
  digitalWrite(2, HIGH);
  //server.send(200, "text/plain", "Apago Led");
  estadoLed = "off";
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleNotFound() {
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
 
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  WiFi.hostname("IoT-Station");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }
 // WiFi.config(ip,gateway,netmask) ;
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());



  server.on("/", handleRoot);
  server.on("/2/on", enciendeLed);
  server.on("/2/off", apagaLed);



  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  
}

void loop(void) {
  server.handleClient();
}
