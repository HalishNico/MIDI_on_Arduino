#include "Midi.h"

uint8_t rx=0;
Midi instance(0);
volatile uint8_t *_port_port;
uint8_t _port; 
bool global_bool = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  while(instance.begin(9600)){};
  instance.blinking=false;
  _port = digitalPinToPort(rx);
  _port_port=portInputRegister(_port);
}

void loop() {
   //Serial.println(instance.rx_read_pin());
}