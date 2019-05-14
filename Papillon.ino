/**
 * Copyright 2019 par Marc SIBERT
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file Papillon.ino Sketch pour IDE Arduino
 * @license Apache-2.0
 * @copyright 2019 par Marc SIBERT
 * @author Marc SIBERT <marc@sibert.fr>
 */
#include <avr/sleep.h>
#include "ws2812b.h"

long readVcc() { 

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1); // Read 1.1V reference against AVcc 
  ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);  // ADC on, Auto Triggering on, prescaler = 1 / 128
  ADCSRB = 0;

  delay(2); // Wait for Vref to settle 
  
  unsigned long a = 0;
  ADCSRA |= _BV(ADSC);
  for (byte i = 0; i < 8; ++i) {
    while (!bit_is_set(ADCSRA, ADIF));
    ADCSRA |= _BV(ADIF);
    a += ADCL + 256U * ADCH;
  }
  ADCSRA &= !_BV(ADEN);
  
  //return a;
  return 1126400L * 8 / a; // Back-calculate AVcc in mV
}


/**
 * Distribution normale centrée.
 * 
 * @param x le paramètre
 * @return l'amplitude
 */
byte f(const float x) {
  return 255 * exp( -sq( x / 2.0f / 0.5f ) );
}

#define NB_LEDS 24

WS2812b<0,24> line0;
WS2812b<1,6> line1;
WS2812b<2,6> line2;
WS2812b<3,24> line3;


void setup() {
#if 0
  Serial.begin(250000);
  while (!Serial) ;
  Serial.println(F("Setup"));
  Serial.println(readVcc());
#endif

  line0.setup();
  line1.setup();
  line2.setup();
  line3.setup();
}

void loop() {
//  Serial.println(readVcc());

  static unsigned long long t = 0;
  
  for (byte i = 0; i < NB_LEDS; ++i) {
    line0.setRGB(i, { 
      f(((i*10L + t*10) % 70) / 10.0f - 4),
      f(((i*10L - t*10 + 65535L*65536) % 100) / 10.0f - 4),
      0
    });
    line3.setRGB(23-i, { 
      f(((i*10L + t*10) % 70) / 10.0f - 4),
      f(((i*10L - t*10 + 65535L*65536) % 100) / 10.0f - 4),
      0
    });
  };
  line0.flush();
  line3.flush();

  
  for (byte i = 0; i < 6; ++i) {
    line1.setRGB(i, {
        (sin(t/25.0) + 1) * 127,
        (cos(t/25.0) + 1) * 127,
        0
    });
    line2.setRGB(i, {
        (sin(t/25.0) + 1) * 127,
        (cos(t/25.0) + 1) * 127,
        0
    });
  }
  line1.flush();
  line2.flush();

  ++t;
  delay(10);
}
