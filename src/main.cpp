#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "main.h"

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "Fruit453";
const char WiFiPSK[] = "RoryAndrewPeter";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 5; // Thing's onboard, green LED

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);

  initHardware();
  connectWiFi();
  server.begin();
  setupMDNS();
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  char request_line[50];
  String pretty_string = client.readStringUntil('\n');
  pretty_string.toCharArray(request_line, 50);
  char *save_ptr;

  char *method = strtok_r(request_line, " ", &save_ptr);
  char *fullurl = strtok_r(NULL, " ", &save_ptr);
  char *url = strtok_r(fullurl, "?", &save_ptr);
  char *params = strtok_r(NULL, "", &save_ptr);
  Serial.println(pretty_string);
  Serial.println(url);
  Serial.println(params);
  client.flush();

  // do stuff...
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: text/html\r\n\r\n";
  response += "Ok.";

  // Send the response to the client
  client.print(response);
  delay(1);

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

void connectWiFi()
{
  byte ledStatus = LOW;
  Serial.println();
  Serial.println("Connecting to: " + String(WiFiSSID));
  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMDNS()
{
  if (!MDNS.begin("thing"))
  {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

void initHardware()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

