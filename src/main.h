
const char WiFiSSID[] = "Fruit453";
const char WiFiPSK[] = "RoryAndrewPeter";

const float SIT = 1.0;
const float STAND = 12.0;
const int LED_PIN = 5; // Thing's onboard LED
const int UPPER_LIMIT_PIN = 1;
const int LOWER_LIMIT_PIN = 2;
const int HEIGHT_POT_PIN = 3;

enum class State {
  UP,
  DOWN,
  STOP,
  SIT,
  STAND
} state;

void connectWiFi();

/** Call MDNS.begin(<domain>) to set up mDNS to point to "<domain>.local" */
void setupMDNS();

void initHardware();

State determine_state(char *url, int url_len);
