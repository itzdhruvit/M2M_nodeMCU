#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WiFiServer server1(80);
IPAddress IP(192,168,4,2);
IPAddress mask = (255, 255, 255, 0);
byte ledPin = 2;
String request = "7";
String answer = "";
int node0;

char ssid[] = "node3";
char pass[] = "qwertyuiop";
IPAddress server2(192,168,4,3);
int count = 0;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  //Blynk.begin(auth, ssid, pass);

}
void APsetup(){
 //Serial.begin(9600);
 WiFi.mode(WIFI_AP);
 WiFi.softAP("node2", "qwertyuiop");
 WiFi.softAPConfig(IP, IP, mask);
 server1.begin();
 pinMode(ledPin, OUTPUT);
 Serial.println();
 Serial.println("AP_node2 started");
 Serial.print("IP: "); Serial.println(WiFi.softAPIP());
 Serial.print("MAC:"); Serial.println(WiFi.softAPmacAddress());  
}
int readrequest(){
 WiFiClient client = server1.available();
 if (!client) {return 0;}
 digitalWrite(ledPin, LOW);
 String request = client.readStringUntil('\r');
 Serial.println("********************************");
 Serial.println("From the station: " + request);
 //Serial.println(client.println(request + "asdfghjkl" + "\r"));
 client.flush();  
 
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
  //Serial.println("Connected");
  Serial.println("STA_Node2");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
  pinMode(ledPin, OUTPUT);
  Serial.println("Connected to NODE3");  
 }

 int send_req_node3(){
  client.connect(server2, 80);
  Serial.println("node1 data sending to node3" + request);
  Serial.println(client.print(request + "999" + "\r"));
  String answer = client.readStringUntil('\r'); 
  return answer.toInt();

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
  send_req_node3();
  
  delay(1000);
  while(!send_req_node3()){
    Serial.println("waiting to receive data");
    delay(1000);
  }
  WiFi.disconnect();
  delay(1500);

  Serial.print("Going back to APmode");
}
