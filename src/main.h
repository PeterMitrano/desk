
const char WiFiSSID[] = "Fruit453";
const char WiFiPSK[] = "RoryAndrewPeter";

const float SIT = 1.0;
const float STAND = 12.0;
const int LED_PIN = 5; // Thing's onboard LED
const int MOTOR_PIN = 1;
const int HEIGHT_POT_PIN = 3;
const float MIN_H = 0.0; // volts?
const float MAX_H = 5.0; // volts?
const float kP = 0.25; // if we're off by 20 bits (.25"), output 5 (small non-zero correction)

enum class State {
  UP,
  DOWN,
  STOP,
  SIT,
  STAND,
  HEIGHT,
  INVALID
} state;
float current_height = 0.0;

void connectWiFi();

/** Call MDNS.begin(<domain>) to set up mDNS to point to "<domain>.local" */
void setupMDNS();

void initHardware();

State determine_state(char *url, int url_len);
