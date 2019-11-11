/*
 * ESP-01 sketch
 * Control an LED from a web page
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "***REMOVED***";
const char* password = "***REMOVED***";

const int led = 2;
int ledState = HIGH;
String toggleCmd = "toggle";

ESP8266WebServer server(80);

// An HTML form with a button
static const char formText[] =
  "<form action=\"/\">\n"
  "<input type=\"hidden\" name=\"toggle\"/>\n"
  "<button type=\"submit\">Toggle LED</button>\n"
  "</form>\n";

// Handle requests for the root document (/)
void handleRoot() 
{
  // If the server got the "toggle" argument, toggle the LED.
  if (server.hasArg("toggle"))
  {
    ledState = !ledState;
    digitalWrite(led, ledState);
  }

  // Display the form 
  server.send(200, "text/html", formText);
}

// Error message for unrecognized file requests
void handleNotFound() {
  server.send(404, "text/plain", "File not found\n\n");
}

void setup() 
{
  Serial.begin(9600);

  pinMode(led, OUTPUT);

  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Set up handlers for the root page (/) and everything else
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin(); // Start the server
}

#define MSG_DELAY 10000
void loop() 
{
  static unsigned long nextMsgTime = millis() + MSG_DELAY;
  
  server.handleClient(); // Process requests from HTTP clients

  if (millis() > nextMsgTime)
  {
    // Show the URL in the serial port
    Serial.print("Visit me at http://");
    Serial.println(WiFi.localIP());
    nextMsgTime = millis() + MSG_DELAY;
  }
  
}
