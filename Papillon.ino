/**
 * 
 */

byte f(const float x) {
  return 255 * exp( -sq( x / 2.0f / 0.5f ) );
}


template<byte PIN = 1, byte LEN = 1>
class Line {

public:
/**
 * Red, Green, Blue byte values.
 */
  struct rgb_t {
    byte r;
    byte g;
    byte b;
  };
  
/**
 * Should be called in arduino setup func.
 * Initialize ports
 */
  void setup() {
    DDRB |= _BV(PIN);  
  }

  void setRGB(const byte pos, const rgb_t& rgb) {
    if (pos < LEN) data[pos] = rgb;
  }

  void flush() const {
    PORTB = 0;
    delayMicroseconds(50);
    for (unsigned i = 0; i < LEN; ++i) { 
      sendRGB(data[i]);
    }
  }

protected:
#define NOP1 "nop\n\t" 
#define NOP2 NOP1 NOP1 
#define NOP5 NOP2 NOP2 NOP1

  inline 
  void sendBit(const bool b) const {
    PORTB |= _BV(PIN);
    __asm__(NOP2);
    PORTB &= b ? 0x0FF : !_BV(PIN);
    __asm__(NOP5);
    PORTB &= !_BV(PIN);
  }

  void sendRGB(const rgb_t& rgb) const {
    cli();
    sendBit(0x80 & rgb.r); 
    sendBit(0x40 & rgb.r); 
    sendBit(0x20 & rgb.r); 
    sendBit(0x10 & rgb.r); 
    sendBit(0x08 & rgb.r); 
    sendBit(0x04 & rgb.r); 
    sendBit(0x02 & rgb.r); 
    sendBit(0x01 & rgb.r); 
    sei();
  
    cli();
    sendBit(0x80 & rgb.g); 
    sendBit(0x40 & rgb.g); 
    sendBit(0x20 & rgb.g); 
    sendBit(0x10 & rgb.g); 
    sendBit(0x08 & rgb.g); 
    sendBit(0x04 & rgb.g); 
    sendBit(0x02 & rgb.g); 
    sendBit(0x01 & rgb.g); 
    sei();
  
    cli();
    sendBit(0x80 & rgb.b); 
    sendBit(0x40 & rgb.b); 
    sendBit(0x20 & rgb.b); 
    sendBit(0x10 & rgb.b); 
    sendBit(0x08 & rgb.b); 
    sendBit(0x04 & rgb.b); 
    sendBit(0x02 & rgb.b); 
    sendBit(0x01 & rgb.b); 
    sei();
  }
  
private:
  rgb_t data[LEN];
  
};


Line<0,10> line1;
Line<1,10> line2;


void setup() {
#if 0  
  Serial.begin(250000);
  while (!Serial) ;
  Serial.println(F("Setup"));
  Serial.flush();
#endif

  line1.setup();
  line2.setup();
}

void loop() {
  static unsigned t = 0;

  for (byte i = 0; i < 10; ++i) {
    line1.setRGB(i, { 
      f(((i*10L + t) % 70) / 10.0f - 4) / 8,
      f(((i*10L - t + 65535) % 100) / 15.0f - 4) / 8,
      16,
    });
    line2.setRGB(i, { 
      f(((i*10L + t + 50) % 70) / 10.0f - 4) / 8,
      f(((i*10L - t + 50 + 65535) % 100) / 15.0f - 4) / 8,
      16,
    });
  };

  line1.flush();
  line2.flush();

  ++t;
  delay(10);
}
