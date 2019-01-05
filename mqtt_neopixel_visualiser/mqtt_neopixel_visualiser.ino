#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <FastLED.h>

#define NUM_LEDS 12
#define DATA_PIN 14

CRGB leds[NUM_LEDS];

// Replace the next variables with your SSID/Password combination

const char* ssid = "DoESLiverpool";
const char* password = "decafbad00";

// const char* mqtt_server = "10.0.39.179";

const char* mqtt_server = "broker.mqtt-dashboard.com";

char hexMAC[3+12+1] = "espxxxxxxxxxxxx"; // length of "esp" + 12 bytes for MAC + 1 byte '\0' terminator 
uint8_t mac[6];

uint8_t gPresHue = 240;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  WiFi.macAddress(mac);
  
  for (int i =0; i < 6; i++)
  {
    hexMAC[3+i*2] = hex_digit(mac[i]>>4);
    hexMAC[3+i*2+1] = hex_digit(mac[i] & 0x0f);
  } 
  Serial.println(hexMAC);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  //while (!client.connect("arduino", "try", "try")) {
    while (!client.connect(hexMAC)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  // Subscribe
  client.subscribe("mcqn/test");
}

void reconnect() {
  // Loop until we're reconnected
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(hexMAC)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("mcqn/test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  value = messageTemp.toInt();
  Serial.println(value); 
  gPresHue = value % 256;
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    reconnect();
  }

  for(int dot = 0; dot < NUM_LEDS; dot++) { 
      // leds[dot] = CRGB::Blue;
      leds[dot] = CHSV( gPresHue, 127, 127);
      // clear this led for the next time around the loop
      // leds[dot] = CRGB::Black;
  }
  
  FastLED.show();
}

char hex_digit(uint8_t aValue)
{
  char values[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
  if (aValue < 16)
  {
    return values[aValue];
  }
  else
  {
    return 'x';
  }
} 
