#include <Arduino.h>
#include "Midi.h"

Midi *Midi::object=0;
uint8_t Midi::_buffer[_SS_MAX_RX_BUFF];
uint8_t Midi::index=0;

bool Midi::begin(long speed){
    object=this;
    _bit_time = (F_CPU/speed)/4;
    _bit_delay =  _bit_time - (30/4);
    _bit_centre = _bit_time/2;
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW);
    blinking=false;
    return 0;
};

Midi::Midi(uint8_t receivePin){
    _receivePin = receivePin;
    set_Rx(_receivePin);
};

void Midi::set_Rx(uint8_t rx){
    // returns depending on rx; PD(pins 0-7), PB(pins 8-13) or PC(pins 14-19)
    port = digitalPinToPort(rx);
    pinMode(rx, INPUT);
    digitalWrite(rx, HIGH);
    //  Gives the adress of the where the binary data of ports PB, PC or PD 
    //are respectively. This is flash memory I beleive
    _receivePortRegister = portInputRegister(port);
    //Pin to mask, i.e pin 3 => 0b00000100 
    _receiveBitMask = digitalPinToBitMask(rx);
    pcint_maskreg = digitalPinToPCMSK(_receivePin);
    pcint_maskvalue = _BV(digitalPinToPCMSKbit(rx));
    *digitalPinToPCICR((int8_t)rx) |= _BV(digitalPinToPCICRbit(rx));
    Interrupt_handler(true);
};

inline void  Midi::recv(){
    //Since interrupts need to be statically defined or something,
    //as in before any code executes, this is the intermediary. 
    //object is a static pointer of type object, so it holds the address
    //of some Midi type object.
    if(object){
      //You get the error, "Midi::buffer cannot be called without object" if this
      //line was just 'buffer();'
      object->buffer();
    }
};

void Midi::buffer(){
    if(rx_read_pin()){
      /*while(true){
        digitalWrite(4, LOW);
        delay(100000);
        digitalWrite(4, HIGH);
        delay(100000);
        digitalWrite(4, LOW);
        delay(50000);
        digitalWrite(4, HIGH);
        delay(50000);
        digitalWrite(4, LOW);
        delay(50000);
        digitalWrite(4, HIGH);
        delay(50000);
      }*/
      return;
    }
    //disable interrupt while reading data
    Interrupt_handler(false);
    _delay_loop_2(_bit_centre);
    if(index<_SS_MAX_RX_BUFF-8){
      for(int i=8; i>0;i--){
        _delay_loop_2(_bit_delay);
        if(rx_read_pin()){
          _buffer[index] = 1;
          digitalWrite(4, HIGH);
        }else{
          _buffer[index] = 0;
          digitalWrite(4, LOW);
        }
        //digitalWrite(4, _buffer[index]);
        index++;
      }
    } else{
      return;
    }
    //use _delay_loop_2(), timer that has counter up to 65536. 
    //Each iteration is 4 cycles of F_CPU
    digitalWrite(4, LOW);
    for(int i=0; i<8; i++){
      digitalWrite(5, HIGH);
      if(_buffer[i]){
        digitalWrite(4, HIGH);
      } else{
        digitalWrite(4, LOW);
      }
      delay(150000);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
      delay(150000);
    }
    Interrupt_handler(true);
};



void Midi::Interrupt_handler(bool enable){
    if(enable){
      *pcint_maskreg |= pcint_maskvalue;
    } else{
      *pcint_maskreg &= ~pcint_maskvalue;
    }
};

uint8_t Midi::rx_read_pin(){
  return *_receivePortRegister & _receiveBitMask;
};


#if defined(PCINT0_vect)
ISR(PCINT0_vect)
{
  Midi::recv();
}
#endif

#if defined(PCINT1_vect)
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT2_vect)
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT3_vect)
ISR(PCINT3_vect, ISR_ALIASOF(PCINT0_vect));
#endif