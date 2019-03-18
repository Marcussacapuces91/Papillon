struct rgb_t {
  byte r;
  byte g;
  byte b;
};

rgb_t barre[8];
  
#define NOP1 "nop\n\t" 
#define NOP2 NOP1 NOP1 
#define NOP3 NOP2 NOP1
#define NOP4 NOP2 NOP2
#define NOP5 NOP4 NOP1
#define NOP6 NOP4 NOP2
#define NOP7 NOP4 NOP2 NOP1
#define NOP8 NOP4 NOP4
#define NOP10 NOP8 NOP2
#define NOP12 NOP8 NOP4
#define NOP15 NOP8 NOP4 NOP2 NOP1
#define NOP16 NOP8 NOP8

inline void sendBit(const byte out, const bool b) {
  PORTB = out;
  __asm__(NOP1);
  PORTB = b ? out : 0;
  __asm__(NOP6);
  PORTB = 0;
}

void sendRGB(const byte out, const rgb_t rgb) {
  cli();
  sendBit(out, 0x80 & rgb.r); 
  sendBit(out, 0x40 & rgb.r); 
  sendBit(out, 0x20 & rgb.r); 
  sendBit(out, 0x10 & rgb.r); 
  sendBit(out, 0x08 & rgb.r); 
  sendBit(out, 0x04 & rgb.r); 
  sendBit(out, 0x02 & rgb.r); 
  sendBit(out, 0x01 & rgb.r); 
  sei();

  cli();
  sendBit(out, 0x80 & rgb.g); 
  sendBit(out, 0x40 & rgb.g); 
  sendBit(out, 0x20 & rgb.g); 
  sendBit(out, 0x10 & rgb.g); 
  sendBit(out, 0x08 & rgb.g); 
  sendBit(out, 0x04 & rgb.g); 
  sendBit(out, 0x02 & rgb.g); 
  sendBit(out, 0x01 & rgb.g); 
  sei();

  cli();
  sendBit(out, 0x80 & rgb.b); 
  sendBit(out, 0x40 & rgb.b); 
  sendBit(out, 0x20 & rgb.b); 
  sendBit(out, 0x10 & rgb.b); 
  sendBit(out, 0x08 & rgb.b); 
  sendBit(out, 0x04 & rgb.b); 
  sendBit(out, 0x02 & rgb.b); 
  sendBit(out, 0x01 & rgb.b); 
  sei();
}

byte f(const int x) {
  return 255 * exp( -sq( x / 2.0f / 0.5f ) );
}

void setup() {
  Serial.begin(250000);
  while (!Serial) ;
  Serial.println(F("Setup"));
  Serial.flush();

  pinMode(11, OUTPUT);

}

void loop() {
  const byte out = 8;
  static int t = 0;

  for (byte i = 0; i < 8; ++i) {
    barre[i] = { 
      f((i + t) % 7 - 4) / 8,
      f((i - t + 65535)*2/3 % 7 - 4) / 8,
      16,
    }; 
  };
  
  PORTB = 0;
  delay(100);
  for (byte i = 0; i < sizeof(barre) / sizeof(rgb_t); ++i) { 
    sendRGB(out, barre[i]);
  }

  ++t;
}
