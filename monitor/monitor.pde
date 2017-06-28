/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
long lastMessageAt=0;

int  incom[]=new int[32];
NodeVis nodes[]=new NodeVis[32];
int truncateTimeout=10;
void setup() 
{
  size(500,500);
  for (int a=0; a<nodes.length; a++) {
    nodes[a]=new NodeVis(a);
  }
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  for (int a=0; a<Serial.list().length; a++) {
    println(Serial.list()[a]);
  }
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
}
int BN_ORIGIN=0;
int BN_HEADER=1;
void draw()
{
  background(0);
  
  for (int a=0; a<nodes.length; a++) {
    nodes[a].dr();
  }
  if ( myPort.available() > 0) {
    //truncated message timeout mechanism
    int incomCount=0;
    int expectedLen=0;
    long lastByteStarted=millis();
    print("\nrx:");
    int currentOrigin=-1;
    while ((incomCount<expectedLen)||expectedLen==0) {
      if (myPort.available()>0) {

        lastByteStarted=millis();
        int i = (int)(myPort.read());
        if (incomCount==BN_ORIGIN) {
          currentOrigin=i;
        }
        incom[incomCount%incom.length]=i;
        if (expectedLen==0) {
          if (incomCount==BN_HEADER) {
            expectedLen=(i&0xf)+2;//+2 for the two header bytes
          }
          if (expectedLen==0 & incomCount>BN_HEADER) {
            print("string?");
          }
        }
        print(i);
        print('\t');
        incomCount++;
      }
      if (millis()>lastByteStarted+truncateTimeout) {
        print("truncated");
      }
    }
    applyMessage();
    //actually only the node zero needs to car about lat message time
    lastMessageAt=millis();
  }
}
void applyMessage() {
  boolean found=false;
  int lastActive=0;
  int setNode=0;
  //find nodeVis that has it
  for (int a=0; a<nodes.length; a++) {
    if (nodes[a].active) {
      lastActive=a;
      if (nodes[a].represents==incom[BN_ORIGIN]) {
        setNode=a;
        found=true;
        break;
      }
    }
  }
  //if none, create new and apply
  if (!found) {
    nodes[lastActive+1].activate(incom[BN_ORIGIN]);
    setNode=lastActive+1;
  }
  int msglen=(incom[BN_HEADER]&0xf);
  String newData="\nlen: "+(msglen-2)+"";
  for (int b=2; b<msglen; b++) {
    newData+="\n"+incom[msglen];
  }
  nodes[setNode].updateData(newData);
}
int cols=8;
int space=2;
class NodeVis { 
  int index; 
  int represents;
  String myData="";
  boolean active=false;
  NodeVis (int i) {  
    index=i;
  }
  void activate(int id) {
    represents=id;
    active=true;
  }
  void updateData(String newData) {
    myData=newData;
  }
  void dr() {
    float w=(width/cols);
    if (active) {
      pushMatrix();
      translate(w*(index%cols)+space,0);
      fill(0);
      stroke(124);
      rect(space, space, w-space, 32-space);
      fill(230);
      text(represents+'\n'+myData, 8, 16);
      popMatrix();
    }
  }
} 