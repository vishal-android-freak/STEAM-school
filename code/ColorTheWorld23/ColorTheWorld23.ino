// board config:
//https://github.com/Seeed-Studio/Wio_Link/wiki/Advanced-User-Guide
//WARNING: use only 3V3
//http://wiki.seeed.cc/Wio_Link/


// See the result in IoT here:
//
//https://dweet.io/dweet/for/ColorTheWorld23
//https://dweet.io/get/dweets/for/ColorTheWorld23
//https://dweet.io/follow/ColorTheWorld23

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define LED_PIN 2
#define LDR A0
#define ALIM 15
#define CFG_BUTTON 0

// WiFi network name and password:
char * networkName = "your_ssid";
char * networkPswd = "your_pass";
WiFiClient client;
String DweetRequest = "http://dweet.io/dweet/for/ColorTheWorld23";

// Internet domain to request from:

enum states {INIT, WIFI_CONNECT, DWEET_REQUEST, STANDBY};
int State = INIT;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
  pinMode(ALIM, OUTPUT);
  pinMode(CFG_BUTTON, INPUT_PULLUP); //si BP ON => level = LOW
  Serial.begin(9600);
  Serial.println();
  Serial.println("WIO LINK");
  digitalWrite(ALIM, HIGH);
}

// the loop function runs over and over again forever
void loop()
{
  int httpCode;
  int Sensor;
  String payload;
  HTTPClient http;  //Declare an object of class HTTPClient

  Sensor = analogRead(LDR);
  /*
    Serial.print(0);
    Serial.print(" ");
    Serial.print (1050);
    Serial.print(" ");
    Serial.println(Sensor);*/

  switch (State)
  {
    case INIT:
      // Connect to the WiFi network (see function below loop)
      connectToWiFi(networkName, networkPswd);
      State = WIFI_CONNECT ;
      break;

    case WIFI_CONNECT:
        digitalWrite( LED_PIN, LOW);
        State = DWEET_REQUEST;
      break;

    case DWEET_REQUEST:
      DweetRequest = "http://dweet.io/dweet/for/ColorTheWorld23?";
      DweetRequest += "Sensor=";
      DweetRequest += String(Sensor);
      DweetRequest += "&Sender=vishal";

      http.begin(DweetRequest);
      httpCode = http.GET();

      if (httpCode > 0)
      { //Check the returning code
        payload = http.getString();   //Get the request response payload
        Serial.println(payload);      //Print the response payload
      }
      payload = http.getString();   //Get the request response payload
      Serial.println(payload);  
      http.end();   //Close connection
      State = STANDBY;
      break;

    case STANDBY:
      delay(1000);
      State = DWEET_REQUEST;
      break;
  }
}

void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;
  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));
  WiFi.begin(networkName, networkPswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}



void printLine()
{
  Serial.println();
  for (int i = 0; i < 30; i++)
    Serial.print("-");
  Serial.println();
}
