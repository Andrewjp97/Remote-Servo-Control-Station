#include <Servo.h>

#include <EthernetBonjour.h>

#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

#include <SPI.h>

#define servo_control_pin 4
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
Servo myServo;
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
 
  Serial.begin(115200);
  
  myServo.attach(9);
  
   Ethernet.begin(mac);
  Udp.begin(localPort);
  Serial.println(Ethernet.localIP());
   
   
    EthernetBonjour.begin("arduino");
    EthernetBonjour.addServiceRecord("Arduino Bonjour Webserver Example._udp",
                                   80,
                                   MDNSServiceTCP);

  
  
}

void loop() {
    EthernetBonjour.run();
    int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    String st = String(packetBuffer);
    int servoValue = st.toInt() - 5;
    Serial.println(servoValue);
    myServo.write(servoValue % 190);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  delay(10);

}
