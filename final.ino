#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4
int d=0;

#define WLAN_SSID       "realme"             // Your SSID
#define WLAN_PASS       "qqqqqqqq"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "kantharuban"            // Username
#define AIO_KEY         "aio_GAWb49QTxZrwMo484xRha3P3AhIo"   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe pattern1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/pattern1"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe pattern2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pattern2");
Adafruit_MQTT_Subscribe pattern3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/pattern3");
//Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4");

void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  
      digitalWrite(Relay4, HIGH);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay2, HIGH);
      digitalWrite(Relay1, HIGH);
      
  
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
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&pattern1);
  mqtt.subscribe(&pattern2);
  mqtt.subscribe(&pattern3);
//  mqtt.subscribe(&Light4);
}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &pattern1) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern1.lastread);
      digitalWrite(Relay1, LOW);
      digitalWrite(Relay2,LOW);
      digitalWrite(Relay3, LOW);
      digitalWrite(Relay4,LOW);
       if(d==1)
      {
dancingled();
      }
      
  }
  if (subscription == &pattern2) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern2.lastread);
      int Light2_State = atoi((char *)pattern2.lastread);
      d=1;
       if(d==1)
      {
dancingled();
      }
     
    }
     if (subscription == &pattern3) {
      Serial.print(F("Got: "));
      Serial.println((char *)pattern3.lastread);
      d=0;
      digitalWrite(Relay1, HIGH);
      digitalWrite(Relay2, HIGH);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4, HIGH);
       if(d==1)
      {
dancingled();
      }
      
    }

  
}
 if(d==1)
      {
dancingled();
      }
}
void MQTT_connect() {
  int8_t ret;
 if(d==1)
      {
dancingled();
      }
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
void dancingled()
{

        digitalWrite(Relay4,HIGH);
        digitalWrite(Relay1, LOW);
        delay(300);
        digitalWrite(Relay1, HIGH);
      digitalWrite(Relay2,LOW);
      delay(300);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3, LOW);
      delay(300);
      digitalWrite(Relay3, HIGH);
      digitalWrite(Relay4,LOW);
      delay(300);
}
