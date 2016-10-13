/*
 Sketch which publishes temperature data from a DS1820 sensor to a MQTT topic.
 This sketch goes in deep sleep mode once the temperature has been sent to the MQTT
 topic and wakes up periodically (configure SLEEP_DELAY_IN_SECONDS accordingly).
 Hookup guide:
 - connect D0 pin to RST pin in order to enable the ESP8266 to wake up periodically
 - DS18B20:
     + connect VCC (3.3V) to the appropriate DS18B20 pin (VDD)
     + connect GND to the appopriate DS18B20 pin (GND)
     + connect D2 to the DS18B20 data pin (DQ)
     + connect a 4.7K resistor between DQ and VCC.
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Streaming.h>
#define SLEEP_DELAY_IN_SECONDS  1800
#define WLAN_SSID       "naranja"
#define WLAN_PASS       "8020980209802"
//#define ADAFRUIT_USERNAME  "caponata"
//#define AIO_KEY  "970561e3b9fb476c85d3796837c771ac"
//#define FEED_PATH ADAFRUIT_USERNAME "/temp/salon/"
#define FEED_PATH "/temp/salon"
#define ONE_WIRE_BUS            D2      // DS18B20 pin


//const char* ssid          = "naranja";
//const char* password      = "8020980209802";
//const char* mqtt_server   = "io.adafruit.com"
const char* mqtt_server   = "192.168.1.42";
/*const char* mqtt_username = "caponata";
const char* mqtt_password = "970561e3b9fb476c85d3796837c771ac";
const char* mqtt_topic    = "caponata/feeds/salon/";*/
WiFiClient espClient;
PubSubClient client(espClient);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);
char temperatureString[6];
void setup() {
  // setup serial port
  Serial.begin(115200);
  // setup WiFi
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // setup OneWire bus
  DS18B20.begin();
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", "", "")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
float getTemperature() {
  Serial << "Requesting DS18B20 temperature..." << endl;
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    Serial << "Reading temperature: " << temp << endl;
    delay(10000);
  } while (temp == 85.0 || temp == (-127.0));
    return temp;
}
void loop() {
  if (!client.connected()) {
    Serial.print("reconnect()");
    reconnect();
  }
  //client.loop();
  float temperature = getTemperature();
  // convert temperature to a string with two digits before the comma and 2 digits for precision
  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial << "Sending temperature: " << temperatureString << endl;
  // send temperature to the MQTT topic
    client.publish(FEED_PATH, temperatureString);
    Serial.println(FEED_PATH);
    Serial.println(temperatureString);
    Serial << "Closing MQTT connection..." << endl;
    client.disconnect();
    Serial << "Closing WiFi connection..." << endl;
    WiFi.disconnect();
    delay(100);
    Serial << "Entering deep sleep mode for " << SLEEP_DELAY_IN_SECONDS << " seconds..." << endl;
    ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
  //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
  delay(500);   // wait for deep sleep to happen
}


