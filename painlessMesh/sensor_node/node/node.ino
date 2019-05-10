
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

int pulse_count = 0;
float litres = 0.0; 

const char interruptPin = D3;
 
//const byte sensorInterrupt =  0 ;
void count_pulse () {
pulse_count++;
Serial.println("I'm in count_pluse ");  

if( pulse_count == 48 ) {
    pulse_count = 0; 
    litres = litres + 0.1;
    Serial.print("Water Meter Reading: ");
    Serial.println(litres);
  }
}


//void flow_loop(){

//}





void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = String(litres);
  msg += mesh.getNodeId();
  Serial.println(msg);
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}


void setup(){
  Serial.begin(115200);
  pinMode (interruptPin, INPUT);
  //pinMode (d2, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin), count_pulse, RISING);
  sei();
  Serial.println("setup");

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

}


void loop() {
    
  userScheduler.execute(); // it will run mesh scheduler as well
  mesh.update();
}
