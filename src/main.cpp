#include <Arduino.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include "main.h"

WiFiServer server(80);
Servo motor;

State global_state = State::STOP;
State last_global_state = State::INVALID;
bool pressing_button = false;

void setup()
{
  Serial.begin(9600);

  initHardware();
  connectWiFi();
  server.begin();
  setupMDNS();
}

String state_to_string(State state) {
  switch(state) {
    case State::UP:
      return "Up";
    case State::DOWN:
      return "Down";
    case State::STOP:
      return "Stop";
    case State::SIT:
      return "Sitting Position";
    case State::STAND:
      return "Standing Position";
    case State::HEIGHT:
      return "Custom Height";
    default:
      return "invalid (interpreted as Stop)";
  }
}

State determine_state(char *url, int url_len) {
  if (strncmp(url, "/up", url_len) == 0) {
    return State::UP;
  }
  else if (strncmp(url, "/down", url_len) == 0) {
    return State::DOWN;
  }
  else if (strncmp(url, "/stop", url_len) == 0) {
    return State::STOP;
  }
  else if (strncmp(url, "/position/stand", url_len) == 0) {
    return State::STAND;
  }
  else if (strncmp(url, "/position/sit", url_len) == 0) {
    return State::SIT;
  }
  else if (strncmp(url, "/height", 7) == 0) {
    current_height = atoi(&url[8]);
    return State::HEIGHT;
  }
  return State::INVALID;
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    // check buttons
    int manual_up = !digitalRead(UP_PIN);
    int manual_down = !digitalRead(DOWN_PIN);

    if (manual_up) {
      global_state = State::UP;
      pressing_button = true;
    }
    else if (manual_down) {
      global_state = State::DOWN;
      pressing_button = true;
    }
    else {
      if (pressing_button) {
        pressing_button = false;
        global_state = State::STOP;
      }
    }

    // just act based on our current state
    switch(global_state) {
      case State::UP:
        motor.write(180);
        break;
      case State::DOWN:
        motor.write(0);
        break;
      case State::INVALID: //fall through
      case State::STOP:
        motor.write(90);
        break;
      case State::SIT:
        //motor.write(0);
        break;
      case State::STAND:
        //motor.write(180);
        break;
      case State::HEIGHT:
        int pos = analogRead(HEIGHT_POT_PIN);
        int error = pos - current_height;
        //motor.write(90 + error * kP);
        break;
    }
    last_global_state = global_state;
  }
  else { // we've recieved something new, handle it.
    // Read the first line of the request
    char request_line[50];
    String pretty_string = client.readStringUntil('\n');
    client.flush();
    pretty_string.toCharArray(request_line, 50);
    char *save_ptr;

    char *method = strtok_r(request_line, " ", &save_ptr);
    char *fullurl = strtok_r(NULL, " ", &save_ptr);
    char *url = strtok_r(fullurl, "?", &save_ptr);
    int url_len = sizeof(*url * sizeof(char));
    char *params = strtok_r(NULL, "", &save_ptr);
    //Serial.println(url);

    global_state = determine_state(url, url_len);

    client.flush();

    // do stuff...
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/html\r\n\r\n";
    response += state_to_string(global_state);

    // Send the response to the client
    client.print(response);
    delay(1);

    // The client will actually be disconnected
    // when the function returns and 'client' object is detroyed
  }
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
  if (!MDNS.begin("desk"))
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
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);
  motor.attach(MOTOR_PIN);
}

