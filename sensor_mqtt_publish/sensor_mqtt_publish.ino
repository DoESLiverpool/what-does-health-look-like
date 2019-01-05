#include <PubSubClient.h>
// Include the correct WiFi header file for the board we're running on
// This code will work for ESP8266, ESP32 and Arduino MKRWIFI1010
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
#include <WiFiNINA.h>
#include <SFU.h>
#endif

// Replace the next variables with your SSID/Password combination
char* ssid = "DoESLiverpool";
char* password = "decafbad00";

//const char* mqtt_server = "10.0.39.179";

const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

char hexMAC[3+12+1] = "espxxxxxxxxxxxx"; // length of "esp" + 12 bytes for MAC + 1 byte '\0' terminator 
uint8_t mac[6];

char msg[50];
int value = 0;
unsigned long lastMillis = 0;

const int presPin = A0; //36;
long presValue = 0;

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

  client.subscribe("/hello");
  // client.unsubscribe("/hello");
}


void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(presPin, INPUT);
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
      //client.subscribe("presPin/pressure");
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


long lastMsg = 0;

void loop() {
    
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
  
  long now = millis();
  if (now - lastMillis > 50) {
    lastMillis = now;
    presValue = analogRead(presPin);
    Serial.println(presValue);
   // Convert the value to a char array
    char tempString[8];    
    dtostrf(presValue, 1, 0, tempString);
    client.publish("mcqn/test", tempString);
  }
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
