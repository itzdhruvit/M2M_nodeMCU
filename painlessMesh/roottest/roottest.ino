#include "painlessMesh.h"

#include "BlynkSimpleEsp8266.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

char auth[] = "79f1ed12061c4e2691cac412397601bc";
#define   STATION_SSID     "Sadhak"
#define   STATION_PASSWORD "S79Pa420"
#define   STATION_PORT     5555
uint8_t   station_ip[4] =  {192,168,1,128}; // IP of the server

String node = "";

// prototypes
void receivedCallback( uint32_t from, String &msg );

painlessMesh  mesh;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, STATION_SSID, STATION_PASSWORD);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages


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

BLYNK_CONNECTED() {
  Blynk.virtualWrite(V0, node);

  Serial.println("Wrote on cloud");
  
}


void loop() {
  mesh.update();
  Blynk.run();
}

void receivedCallback( uint32_t from, String &msg ) {
  node = msg.c_str();
  Serial.println(node);
  Serial.printf("bridge: Received from %u msg=%s\n", from, msg.c_str());
}
