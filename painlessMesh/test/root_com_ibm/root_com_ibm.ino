#include "painlessMesh.h"


#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555


#define   STATION_SSID     "itz_dhruvit"
#define   STATION_PASSWORD "1111111111"
#define   STATION_PORT     5555
uint8_t   station_ip[4] =  {192,168,1,128}; // IP of the server

String node = "";

 
Scheduler userScheduler;

painlessMesh  mesh;

#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

//-------- Customise these values -----------
const char* ssid = "itz_dhruvit";
const char* password = "1111111111";

#define ORG "duxwpn"
#define DEVICE_TYPE "water_meter"
#define DEVICE_ID "m_1"
#define TOKEN "ssVyHw0fjNFX7*8ql!"
//-------- Customise the above values --------

#define DEVICE_ID_2 "m_2"
#define TOKEN_2 "Node2@02"

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

char token_2[] = TOKEN_2;
char clientId_2[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID_2;

char topic_1[] = "iot-2/cmd/test/fmt/String";

int i=0;
int j=0;
int k=0;
int l=0;
int m=0;
int o=0;

void callback(char* topic_1, byte* payload, unsigned int length);

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);


void setup_ibm(){
 Serial.println();

 Serial.print("Connecting to "); Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(300);
 Serial.print(".");
 } 
 Serial.println("");

 Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

int counter = 0;
int counter_2 = 0;
String send_msg = "";

void ibm_1(){

  
 if (!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(300);
 }
 Serial.println();
 }

 String payload = node;

 
 Serial.print("Sending payload for device 1: ");
 Serial.println(payload);
 
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 i=1;
 } else {
 Serial.println("Publish failed");
 }
 
} 

void ibm_2(){
  
 if (!client.connected()) {
 Serial.print("Reconnecting client_2 to ");
 Serial.println(server);
 while (!client.connect(clientId_2, authMethod, token_2)) {
 Serial.print(".");
 delay(300);
 }
 Serial.println();
 }

 String payload = "{\"d\":{\"Name\":\"Second Device\"";
 payload += ",\"counter_2\":";
 payload += counter_2++;
 payload += "}}";
 
 Serial.print("Sending payload to 2nd device: ");
 Serial.println(payload);
 
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok for device 2 ");
 } else {
 Serial.println("Publish failed");
 }
 j=1;
}
void ibm_con_setup(){
  wifiConnect();
  mqttConnect();
}

  

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}

void initManagedDevice() {
  if (client.subscribe(topic_1)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic_1, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic_1);

  for (int i = 0; i < payloadLength; i++) {
    Serial.print((char)payload[i]);
    
  }
  
  o=1;
}



// prototypes
void receivedCallback( uint32_t from, String &msg );
void sendMessage() ;

Task taskSendMessage(TASK_SECOND * 1, TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = send_msg;
  msg += mesh.getNodeId();
  Serial.println(msg);
  mesh.sendBroadcast( msg);
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 2 ));
}

// Needed for painless library


void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  setup_root();
  setup_con();
}


void setup_con(){
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  //mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable(); 
}

void setup_root(){

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages
  pinMode(LED_BUILTIN, OUTPUT);
  // Channel set to 6. Make sure to use the same channel for your mesh and for you other

  // network (STATION_SSID)
  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.stationManual(STATION_SSID, STATION_PASSWORD, STATION_PORT, station_ip);
  // Bridge node, should (in most cases) be a root node. See [the wiki](https://gitlab.com/painlessMesh/painlessMesh/wikis/Possible-challenges-in-mesh-formation) for some background
  mesh.setRoot(true);
  // This and all other mesh should ideally now the mesh contains a root
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
}

void ibm_loop(){
  setup_ibm();
  ibm_1();
  client.disconnect();
  //WiFi.disconnect();
  Serial.println("value of i is:");
  Serial.print(i);
  while(i==1){
    //setup_ibm();
    ibm_2();
    break;
  }
  i=0;
  client.disconnect();
  //WiFi.disconnect();
}

void loop() {  
  userScheduler.execute();
  mesh.update();
  while(k==1){
      mesh.stop();
      k=0;
      l=1;
  }

  while(l==1){
    ibm_loop();
    l=0;
    m=1;
  }
  while(m==1){
    ibm_con_setup();
    if(!client.loop()){
      mqttConnect();      
    }
    m=0; 
    Serial.println("Waiting for reverse control");
    while(!o){
      if(!client.loop()){
        mqttConnect(); 
      }
      Serial.print(".");
      delay(100);
    }
    o=0;
    
  }
 
}
 

void receivedCallback( uint32_t from, String &msg ) {
  node = msg.c_str();
  Serial.println(node);
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
  k=1;
  mesh.stop();
  }
