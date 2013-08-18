#include <SPI.h> // needed for Ethernet library communication with the W5100 (Arduino ver>0018)
#include <Ethernet.h>
#include <utility/w5100.h>



const int ledPin = 9; // LED connected to digital pin 13 9 for arduino Ethernet
const int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;     // variable to store the read value


SOCKET s; // our socket that will be opened in RAW mode
byte buf[1500+14]; // buffer to send through socket
byte mac[] = { 0x90, 0xA2, 0xD9, 0xD0, 0xF4, 0xB1  };
byte targetmac[] = { 0x20,0xCF,0x31,0x95,0x32,0xE4 };
int bufIDX=0;

void setup() 
{
 pinMode(inPin, INPUT_PULLUP);      // sets the digital pin 7 as input
 pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
 //digitalWrite(ledPin, LOW);
    // initialize the w5100 chip and open a RAW socket
  W5100.init();
  W5100.writeSnMR(s, SnMR::MACRAW); 
  W5100.execCmdSn(s, Sock_OPEN);
//  delay(20000);
}

void loop() {

 val = digitalRead(inPin);   // read the input pin
 digitalWrite(ledPin, val);    // Startup wol if 1
 if(val == 1)
 {
  
   
   
   bufIDX=0;
  // filling buffer with some data ...

//start out, 6 bytes of 0xFF
  for (int i=0; i<=5; i++) {
    buf[bufIDX]=0xFF;
    bufIDX++;
  }

//followed by the "senders" mac
//which is made up
  for (int i=0;i<=5;i++){
     buf[bufIDX]=mac[i];
     bufIDX++;
  }
  
//ether type
  buf[bufIDX]=0x08;
  bufIDX++;
  buf[bufIDX]=0x42;
  bufIDX++;

//antoher series of 0xFF
  for (int i=0; i<=5; i++) {
    buf[bufIDX]=0xFF;
    bufIDX++;
  }

//repeat target mac 16 times
  for (int repeats=0;repeats<=15;repeats++){
    for (int i =0; i <=5;i++){
      buf[bufIDX]=targetmac[i];
      bufIDX++;
    }
  }
  // sending buffer
  W5100.send_data_processing(s, buf, bufIDX);
  W5100.execCmdSn(s, Sock_SEND_MAC);
  
 //pin 9  Starts WOL therefor no more need for this loop 
 
 //for(;;){
 //lalalalalalalalalala Endlosschleife
 //}
 }
 else {
    // turn LED off:
    val=0; 
  }
 
}
