#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WiFiServer server(80);
IPAddress IP(192,168,4,1);
IPAddress mask = (255, 255, 255, 0);
byte ledPin = 2;
int output;
int data2 = 46;

/*String apiKey = "J7ARQXZJ9ZVTR3LZ";                                     //fill in the api key from thingspeak
const char* ssid = "itz_dhruvit   ";                                  //fill in your wifi name
const char* password = "1111111111";                              //fill in your wifi password
const char* server1 = "api.thingspeak.com";*/

char auth[] = "79f1ed12061c4e2691cac412397601bc";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "itz_dhruvit";
char pass[] = "1111111111";

BlynkTimer timer;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, BlynkServer);

}
void APsetup(){
 //Serial.begin(9600);
 WiFi.mode(WIFI_AP);
 WiFi.softAP("ESP8266", "qwertyuiop");
 WiFi.softAPConfig(IP, IP, mask);
 server.begin();
 pinMode(ledPin, OUTPUT);
 Serial.println();
 Serial.println("accesspoint_bare_01.ino");
 Serial.println("Server started.");
 Serial.print("IP: "); Serial.println(WiFi.softAPIP());
 Serial.print("MAC:"); Serial.println(WiFi.softAPmacAddress());  
}
int readrequest(){
 WiFiClient client = server.available();
 if (!client) {return 0;}
 digitalWrite(ledPin, LOW);
 String request = client.readStringUntil('\r');
 Serial.println("********************************");
 Serial.println("From the station: " + request);
 Serial.println(client.println(request + "asdfghjkl" + "\r"));
 client.flush();  
 output = request.toInt() + 46;
 Serial.println(output);
 
 return 1;
}
 void STAsetup(){
  //Serial.begin(9600);
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
  Serial.println("Connected to itz_dhruvit");  
 }

void BlynkServer(){
  Serial.println("Writing to Blynk");
  Blynk.virtualWrite(V0, output);
  
  //Serial.println("Connected to itz_dhruvit");
}


void loop() {
  APsetup();
  delay(100);
  while(!readrequest()){
    Serial.println("waiting for data");
    delay(1000);
  }
  delay(200);
  STAsetup();
  delay(1500);
  for(int i=0; i<1000; i++){
      Blynk.run();
      timer.run();
      delay(10);
  }
  delay(10000);
  WiFi.disconnect();
  delay(1500);

  Serial.print("Going back to APmode");
}
