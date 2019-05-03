#include <ESP8266WiFi.h>
byte ledPin = 2;
char ssid[] = "node2";           // SSID of your AP
char pass[] = "qwertyuiop";         // password of your AP
IPAddress server1(192,168,4,2);     // IP address of the AP
WiFiClient client;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
  pinMode(ledPin, OUTPUT);
}



void loop() {
  client.connect(server1, 80);
  digitalWrite(ledPin, LOW);
  Serial.println("********************************");
  Serial.print("Byte sent to the AP: ");
  Serial.println(client.print("123\r"));
  //String answer = client.readStringUntil('\r');
  //Serial.println("From the AP: " + answer);
  client.flush();
  //digitalWrite(ledPin, HIGH);
  client.stop();
  delay(10000);
}
