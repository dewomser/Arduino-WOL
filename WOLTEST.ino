#include <SPI.h>         // needed for Ethernet library communication with the W5100 (Arduino ver>0018)
#include <Ethernet.h>
#include <utility/w5100.h>

SOCKET s; // our socket that will be opened in RAW mode
byte buf[1500+14]; // buffer to send through socket
byte mac[] = { 0x00, 0x1a, 0x2b, 0x3c,0x77,0x8f }; 
byte targetmac[] = { 0x00, 0x26, 0x22, 0x39,0xa3,0xca };
int bufIDX=0;

void setup() {

  // initialize the w5100 chip and open a RAW socket  
  W5100.init();
  W5100.writeSnMR(s, SnMR::MACRAW); 
  W5100.execCmdSn(s, Sock_OPEN);
  delay(20000);
}

void loop() {
  


   bufIDX=0;
  // filling buffer with some data ...
  for (int i=0; i<=5; i++) {
    buf[bufIDX]=0xFF;
    bufIDX++;
  }
  for (int i=0;i<=5;i++){
     buf[bufIDX]=mac[i];
    bufIDX++; 
  }
  
  buf[bufIDX]=0x08;
  bufIDX++;
  buf[bufIDX]=0x42;
  bufIDX++;

  for (int i=0; i<=5; i++) {
    buf[bufIDX]=0xFF;
    bufIDX++;
  }

  for (int repeats=0;repeats<=15;repeats++){
    for (int i =0; i <=5;i++){
      buf[bufIDX]=targetmac[i];
      bufIDX++;
    }  
  }  
  // sending buffer
  W5100.send_data_processing(s, buf, bufIDX);
  W5100.execCmdSn(s, Sock_SEND_MAC);
 
  for(;;){
    
  }
}
