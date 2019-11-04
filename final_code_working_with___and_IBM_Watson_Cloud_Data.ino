#include <PubSubClient.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEsp.h>
//#include <WiFiEspUdp.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define ORG "bp3m8r"
#define DEVICE_TYPE "Arduino"
#define DEVICE_ID "Geu"
#define TOKEN "TnpNTv(MRy(!L-y5o0"
#define WIFI_AP "Moin Ahamad"
#define WIFI_PASSWORD "Khushnuda786@"
// Defines Tirg and Echo pins of the Ultrasonic Sensor
WiFiEspClient espClient;

SoftwareSerial soft(10, 11); // 10-tx,11-rx
unsigned long time_now = 0;
int period = 5000;
int status = WL_IDLE_STATUS; //esp initial status
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char publishTopic[] = "iot-2/evt/status/fmt/json";
const char responseTopic[] = "iotdm-1/response";
const char manageTopic[] = "iotdevice-1/mgmt/manage";
const char updateTopic[] = "iotdm-1/device/update";
const char rebootTopic[] = "iotdm-1/mgmt/initiate/device/reboot";
void callback(char* publishTopic, char* payload, unsigned int payloadLength);

PubSubClient client(server, 1883, callback, espClient);

int publishInterval = 30000; // 30 seconds
long lastPublishMillis;


int trigPin = 6;    // Trigger
int echoPin = 5;    // Echo
long duration, cm, inches;
float per, stat1, stat2;
 
void setup() {
  Serial.begin(115200);
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        InitWiFi();
        Serial.print(WiFi.localIP());
        if (!!!client.connected()) 
        {
                Serial.print("Reconnecting client to ");
                Serial.println(server);
                while (!!!client.connect(clientId, authMethod, token)) 
                {
                Serial.print(".");
                }
          Serial.println();
        }
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  if (cm > 0 && cm < 10)
  {
    stat1 = 50-cm;
    stat2 = stat1/50;
    per = stat2 * 100;
    send_data(per);
    Serial.println("Needs To Collect");
    Serial.println();
    
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  else if (cm >= 10 && cm < 20)
  {
    stat1 = 50-cm;
    stat2 = stat1/50;
    per = stat2 * 100;
    send_data(per);
    Serial.println("At Level 4");
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  else if (cm >= 20 && cm < 30)
  {
    stat1 = 50-cm;
    stat2 = stat1/50;
    per = stat2 * 100;
    send_data(per);
    Serial.println("At Level 3");
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  else if (cm >= 30 && cm < 40)
  {
    stat1 = 50-cm;
    stat2 = stat1/50;
    per = stat2 * 100;
    send_data(per);
    Serial.println("At Level 2");
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  else if (cm >= 40 && cm < 50)
  {
    stat1 = 50-cm;
    stat2 = stat1/50;
    per = stat2 * 100;
    send_data(per);
    Serial.println("At Level 1");
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  else if (cm >50)
  {
    per = 0;
    send_data(per);
    Serial.println("At Level 1");
    Serial.print(per);
    Serial.print("%");
    Serial.println();
  }
  
  delay(250);
}


void InitWiFi()
{
      // initialize serial for ESP module
      soft.begin(112500);
      // initialize ESP module
      WiFi.init(&soft);
      
      Serial.println("Connecting to AP …");
      // attempt to connect to WiFi network
      while ( status != WL_CONNECTED) {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(WIFI_AP);
            // Connect to WPA/WPA2 network
            status = WiFi.begin(WIFI_AP, WIFI_PASSWORD);
            delay(1000);
      }
      Serial.println("Connected to AP");
}

void callback(char* publishTopic, char* payload, unsigned int length) {
      Serial.println("callback invoked");
}

void send_data(float per)
{
  String payload = "{\"d\":{ \"temp\" :";
          payload += per;
          payload += "} }";
          Serial.print("Sending payload: ");
          Serial.println(payload);

          while (!!!client.publish(publishTopic, (char *)payload.c_str())) {
                  Serial.println("Publish ok");
                  if (!!!client.connected()) {
                        Serial.print("Reconnecting client to ");
                        Serial.println(server);
                        while (!!!client.connect(clientId, authMethod, token)) {
                              Serial.print(".");
                              delay(1000);
                        }
                        Serial.println();
                  }
          
          } 
            time_now = millis();           
            Serial.println("5 sec Delay");           
            while(millis() < time_now + period){
                //wait approx. [period] ms
            }  
}
