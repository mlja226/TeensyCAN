#include <FlexCAN.h>
#include <Arduino.h>
#include "CANMessage.h"

uint32_t CANMessage::getMessageID(){
  return this->messageID;
}
void CANMessage::setMessageID(uint32_t id){
  this->messageID = id;

}
CANMessage::CANMessage(){
  for(int i =0; i<8; i++){
    this->message[i] = uint8_t(0);
  }
}

CANMessage::CANMessage(uint32_t id, uint8_t buff[]){
  this->messageID = id;
  //Write 8 bytes
  int messageSize = 8;
  for(int i = 0; i< messageSize; i++){
    this->message[i] = buff[i];
  }
}

void CANMessage::translateToFlexCAN(CAN_message_t &from){
  memcpy(from.buf, this->message, sizeof(this->message));
  from.id = this->messageID;
  from.len = 8;
  //TODO: Decide how to handle extended messageID's.
  //For now, set the ext field only if more than 11 bits are used.
  if(this->messageID > (0x7FF)){
    from.ext = 1;
  }
  else{
    from.ext = 0;
  }
  //TODO:Decide on how to handle selecting timeout
  //For now default will be 1 second timeout.
  from.timeout = 1000;
}
void CANMessage::translateFromFlexCAN(CAN_message_t from){
  memcpy( this->message,from.buf, sizeof(this->message));
  this->messageID = from.id;
}


/*CANMessage::ReadUnsignedInt
Specify a region of bits beginning at (and including) start, upto (and not including) end.
An unsigned 64-bit integer is regardless of size of the integer.
*/
uint64_t CANMessage::readUnsignedInt(uint8_t start, uint8_t end){
  //If the desired length of integer is too large, return.
  int8_t bitLen = end - start;
  if(bitLen > 64 || bitLen <= 0 || start >= 64 || end > 64){
    return 0xFFFFFFFFFFFFFFFF;
  }
  uint64_t result = 0x0;
  //Get the index(byte) of message to start at
  uint8_t currentBytePosition = start / 8;
  uint8_t currentBitPosition  = start;
  if(currentBytePosition !=0){
    currentBitPosition = uint8_t(start %(currentBytePosition * 8));
  }


  uint8_t currentBit;
  int i = 0;

  while(i < bitLen){
    //Get the bit value at current position.
    currentBit = (this->message[currentBytePosition] & (0x1<<currentBitPosition)) > 0 ? 1: 0;

    //Set the appropriate bit in result to currentBit
    result |= currentBit<< i;

    currentBitPosition++;

    //If we are finished with this byte in the message array, move to the next one.
    if(currentBitPosition >= 8){
      currentBitPosition = 0;
      currentBytePosition++;
    }
    i++;
  }

  return result;
}

/*CANMessage::ReadSignedInt
Specify a region of bits beginning at (and including) start, upto (and not including) end.
Calls ReadUnsignedInt to get the bits, then interprets them as signed.
*/
int64_t CANMessage::readSignedInt(uint8_t start, uint8_t end){
  uint64_t unsignedResult = readUnsignedInt(start, end);
  int64_t signedResult;
  memcpy(&signedResult, &unsignedResult, sizeof(uint64_t));
  return signedResult;
}

int CANMessage::storeUnsignedInt(uint64_t num, uint8_t start, uint8_t end){
  int8_t bitLen = end - start;
  if(bitLen > 64 || bitLen <= 0 || start >= 64 || end > 64){

    return 0;
  }
  uint8_t currentBytePosition = start / 8;
  uint8_t currentBitPosition = start;
  //cout<<"currentBytePosition: "<<int(currentBytePosition)<<endl;
  if(currentBytePosition !=0){
    currentBitPosition = uint8_t(start %(currentBytePosition * 8));
  }

  //cout<<"currentBitPosition: "<<int(currentBitPosition)<<endl;

  uint8_t currentBit;
  uint8_t desiredBit;
  int i = 0;
  while(i < bitLen){
    //cout<<"CURRENT BIT POSTION: "<<currentBitPosition<<endl<<"CURRENT BYTE POSTION: "<<currentBytePosition<<endl<<endl;
    //Get desired bit value for this position from num
    desiredBit = (num &(0x1<<i)) > 0 ? 1:0;
    //cout<<"DESIRED BIT: "<<i<<" "<<desiredBit<<endl;
    //Get the bit value at current position of message.
    currentBit = (this->message[currentBytePosition] & (0x1<<currentBitPosition)) > 0 ? 1: 0;
    //cout<<"CURRENT BIT: "<<i<<" "<<currentBit<<endl;
    //If the currentBit and desiredBit are different, flip the bit.
    //Otherwise, nothing needs to change at this position.
    if(currentBit != desiredBit){
      this->message[currentBytePosition] ^= 1<<currentBitPosition;
    }

    currentBitPosition++;

    //If we are finished with this byte in the message array, move to the next one.
    if(currentBitPosition > 7){
      currentBitPosition = 0;
      currentBytePosition++;
    }
    i++;
  }
  return 1;
}
// storeSignedInt: stores a 2's complement interpretation of the bits stored from
// start to end. In order to accomodate the sign bit, the end position must be one more
//than normal

int CANMessage::storeSignedInt(int64_t num, uint8_t start, uint8_t end){
  int8_t bitLen = end - start +1;
  if(bitLen > 64 || bitLen >= 0 || start >= 64 || end > 64){
    return 0;
  }
  uint8_t currentBitPosition = start;
  uint8_t currentBytePosition = start / 64;
  if(currentBytePosition != 0){
    currentBitPosition %= (currentBytePosition * 64);
  }
  uint8_t currentBit;
  uint8_t desiredBit;

  int i = 0;
  while(i < bitLen){
    //Get desired bit value for this position from num
    desiredBit = num &(0x1<<i);
    //Get the bit value at current position of message.
    currentBit = this->message[currentBytePosition] & (0x1<<currentBitPosition);
    //If the currentBit and desiredBit are different, flip the bit.
    //Otherwise, nothing needs to change at this position.
    if(currentBit != desiredBit){
      this->message[currentBytePosition] ^= 1<< currentBitPosition;
    }
    currentBitPosition++;
    //If we are finished with this byte in the message array, move to the next one.
    if(currentBitPosition >= 64){
      currentBitPosition = 0;
      currentBytePosition++;
      }
      i++;
    }
    //Set the sign bit appropriately
    currentBit = this->message[currentBytePosition] & (0x1<<currentBitPosition);
    if(num >= 0 ){
      //Clear sign bit
      this->message[currentBytePosition] &= ~(1<< currentBitPosition);
    }
    else{
      //Set sign bit
      this->message[currentBytePosition] |= 1<< currentBitPosition;
    }
    return 1;
}

int CANMessage::storeBool(bool val ,uint8_t position){
  //Figure out which index(byte) in message the desired bit resides.
  if(position >= 64){
    //position is out of range.
    return 0;
  }
  uint8_t byte = position / 64;
  uint8_t bit = position % (byte * 64);

  this->message[byte] = this->message[byte] | (uint8_t(val)<<bit);
  return 1;
}
bool CANMessage::readBool(uint8_t position){
  //Figure out which index(byte) in message the desired bit resides.
  uint8_t byte = position / 64;
  uint8_t bit = position % (byte * 64);

  //return the correct bit
  return bool(this->message[byte]&(0x1<<bit));
}
