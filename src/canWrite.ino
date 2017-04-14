#include <Arduino.h>

#include <FlexCAN.h>
#include <kinetis_flexcan.h>

FlexCAN CANbus(500000);

void setup()
{
  CANbus.begin();
  pinMode(13, OUTPUT);
}

void loop()
{

  digitalWrite(13, HIGH);
  CAN_message_t myMessage;
  myMessage.id = 0x711;
  myMessage.len = 8;
  myMessage.buf[0] = '0';
  myMessage.buf[1] = '1';
  myMessage.buf[2] = '2';
  myMessage.buf[3] = '3';
  myMessage.buf[4] = '0';
  myMessage.buf[5] = '0';
  myMessage.buf[6] = '0';
  myMessage.buf[7] = '0';
  myMessage.timeout = 0;


  int ret = CANbus.write(myMessage);
  Serial.println(ret);

  delay(100);
}
