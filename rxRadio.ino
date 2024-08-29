

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


const byte pipeIn[] = {0xEE,0xEE,0xEE,0xEE,0xEE};
RF24 radio(9, 10); 
uint8_t data[32];
uint16_t first_data = 0;
uint16_t second_data = 0;


void setup()
{
 
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.setChannel(100);
  radio.setAutoAck(false);
  radio.disableCRC();
  radio.disableDynamicPayloads();
  radio.setRetries(0, 0);
  radio.setDataRate(RF24_250KBPS);                          
  radio.setPALevel(RF24_PA_MAX);                           
  radio.startListening();                                   
  Serial.begin(9600);
}



void loop()
{
  if(radio.available()){
    radio.read(data, sizeof(data));
    first_data = (data[1]<<8) | data[0];
    second_data = (data[3]<<8) | data[2];
    Serial.print("First data: ");
    Serial.print(first_data);
    Serial.print(" ------- ");
    Serial.print("Second data: ");
    Serial.println(second_data);

  }

  
}
