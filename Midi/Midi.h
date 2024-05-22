#ifndef Midi_h
#define Midi_h
#include <inttypes.h>


#ifndef _SS_MAX_RX_BUFF
#define _SS_MAX_RX_BUFF 64 // RX buffer size
#endif

class Midi{   
    private:
        uint8_t _receivePin;
        uint8_t port;
        uint16_t _bit_time;
        uint16_t _bit_delay;
        uint16_t _bit_centre;
        uint8_t _receiveBitMask;
        volatile uint8_t *_receivePortRegister;
        volatile uint8_t *pcint_maskreg;
        static Midi *object;
        void set_Rx(uint8_t receive);
        
    public:
        bool blinking=false;
        uint8_t pcint_maskvalue;
        Midi(uint8_t receivePin);
        static uint8_t index;
        bool begin(long speed);
        void buffer();
        void Interrupt_handler(bool enable);
        static inline void recv();
        static void user_interrupt();
        uint8_t rx_read_pin();
        static uint8_t _buffer[_SS_MAX_RX_BUFF];
};

#endif