#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "realme"
#define WLAN_PASS       "qqqqqqqq"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "kantharuban"
#define AIO_KEY         "aio_GAWb49QTxZrwMo484xRha3P3AhIo"
#define Relay1            5
#define Relay2            6
#define Relay3            7
#define Relay4            8
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish exam = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/exam");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe pattern1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pattern1");
Adafruit_MQTT_Subscribe pattern2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pattern2");
Adafruit_MQTT_Subscribe pattern3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pattern3");

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
   pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);


  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&pattern1);
  mqtt.subscribe(&pattern2);
  mqtt.subscribe(&pattern3);
   
}
uint32_t x=0;

void loop() {
   MQTT_connect();

 Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &pattern1) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern1.lastread);
      digitalWrite(Relay1, HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4,HIGH);
      
    }
    if (subscription == &pattern2) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern2.lastread);
      int Light2_State = atoi((char *)pattern2.lastread);
      if(Light2_State>0)
      {
        digitalWrite(Relay4,LOW);
        digitalWrite(Relay1, HIGH);
        delay(300);
        digitalWrite(Relay1, LOW);
      digitalWrite(Relay2,HIGH);
      delay(300);
      digitalWrite(Relay2,LOW);
      digitalWrite(Relay3, HIGH);
      delay(300);
      digitalWrite(Relay3, LOW);
      digitalWrite(Relay4,HIGH);
      delay(300);
      }
    }
    if (subscription == &pattern3) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern3.lastread);
      
      digitalWrite(Relay1, LOW);
      digitalWrite(Relay2, LOW);
      digitalWrite(Relay3, LOW);
      digitalWrite(Relay4, LOW);
    }
    
  }
}
  void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
