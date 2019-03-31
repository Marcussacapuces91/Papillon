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
 * @file ws2812b.h Lib ws2812b
 * @license Apache-2.0
 * @copyright 2019 par Marc SIBERT
 * @author Marc SIBERT <marc@sibert.fr>
 */

 /**
 * Classe modélisant une ligne de LEDs RGB WS2812.
 * 
 * @tparam PIN est le bit du PORT B utilisé pour commander la ligne de LEDs
 * @tparam LEN est la longueur de la ligne de LEDs
 */
template<byte PIN = 1, byte LEN = 1>
class WS2812b {

public:
/**
 * Structure regroupant les valeurs Red, Green, Blue.
 */
  struct rgb_t {
    byte r;
    byte g;
    byte b;
  };
  
/**
 * À appeler à l'initialisation de l'application.
 * 
 * Initialise le port de sortie.
 */
  void setup() {
    DDRB |= _BV(PIN);  
  }

/**
 * Définit le triplet de la LED indiquée.
 * 
 * @warning Il s'agit d'une copie en mémoire. Pour être effectivement appliquée, il faut utiliser la méthode flush()
 * 
 * @param pos Position de la LED dans la ligne [0..n-1]
 * @param rgb Une référence sur les valeurs à appliquer à cette LED
 */
  void setRGB(const byte pos, const rgb_t& rgb) {
    if (pos < LEN) data[pos] = rgb;
  }

/**
 * Applique l'ensemble des triplets mémorisés sur la ligne de LEDs.
 * 
 * @warning pendant la transmission, les interruptions sont désactivées.
 */
  void flush() const {
    PORTB &= !_BV(PIN);
    delayMicroseconds(50);
    for (unsigned i = 0; i < LEN; ++i) { 
      sendRGB(data[i]);
    }
  }

protected:
#define NOP1 "nop\n\t" 
#define NOP2 NOP1 NOP1 
#define NOP5 NOP2 NOP2 NOP1

/**
 * Transmet un bit modulé sur la ligne pré-définie.
 * 
 * @warning Cette méthode est définie pour une fréquence de processeur de 16 MHz.
 * 
 * @param b Un booléen indiqauant l'état du bit à transmettre.
 */
  inline 
  void sendBit(const bool b) const {
    PORTB |= _BV(PIN);
    __asm__(NOP2);
    PORTB &= b ? 0x0FF : !_BV(PIN);
    __asm__(NOP5);
    PORTB &= !_BV(PIN);
  }

/**
 * Transmet le triplet de couleurs vers la ligne.
 * 
 * @warning Cette méthode désactive ponctuellement les interruptions.
 * 
 * @param rgb Un triplet de couleurs.
 */
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
/// Zone mémoire concervant les triplets de couleur de chaque LED de la ligne.
  rgb_t data[LEN];
  
};
