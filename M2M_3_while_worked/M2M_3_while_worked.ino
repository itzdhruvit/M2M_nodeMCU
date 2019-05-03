#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WiFiServer server(80);
IPAddress IP(192,168,4,1);
IPAddress mask = (255, 255, 255, 0);
byte ledPin = 2;
int output;
int data2 = 46;
bool write_2_blynk = false;
char auth[] = "79f1ed12061c4e2691cac412397601bc";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "itz_dhruvit";
char pass[] = "1111111111";

int count = 0;

BlynkTimer timer;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

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
 
 output = request.toInt() + count;
 Serial.println(output);
 count++;
 Serial.println(count); 
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

BLYNK_CONNECTED() {//This function is called only when nodeMCU is connected one time.
  Serial.println(output);
  //output+=100;
  //Serial.println(output);  
  Blynk.virtualWrite(V0, output);
  write_2_blynk = true; 

  Serial.println("Wrote on cloud");
  
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
  write_2_blynk = false;
  while(!write_2_blynk){
    Blynk.run();    
    //Serial.println("Hey, we are in Blynk");
  }
  delay(10000);
  WiFi.disconnect();
  delay(1500);

  Serial.print("Going back to APmode");
}
