#include <Adafruit_NeoPixel.h>
#define _SS_MAX_RX_BUFF 8 //lower SoftwareSerial's receive buffer to conserve some RAM
#include <SoftwareSerial.h>

#include "palette.h"
#include "anim.h"
#include "commands.h"
#include "small_timer.hpp"
#include "button_classes.hpp"

#define ANIMS 7 //number of animations
#define PALS 7 //number of palettes
#define INTERVAL 10000 //change interval, msec


//#define BTHS //whether to use hardware serial to communicate Bluetooth. Software serial is used otherwise
#define DEBUG //if defined, debug data is output to hardware serial port. REMEMBER TO REMOVE this definition once BTHS is set

Palette * pals[PALS] = {&PalRgb, &PalRainbow, &PalRainbowStripe, &PalParty, &PalHeat, &PalFire, &PalIceBlue};

Anim anim = Anim();

#ifndef BTHS
//11 and 12 are RX (from BT) and TX (to BT) pin numbers
SoftwareSerial bt(11,12);
#endif

bool commandComplete;  //flag indicating whether the command is complete or not
byte cmdPos;  //position inside command buffer; 0 means buffer is empty; 1 means command marker received; 2...5 means command data received. Value of 5 means the command is fully received


byte *command; //BT command buffer
byte commandLength;


constexpr bool disableAutoChangeEffects = true;

unsigned long ms = 10000;//startup animation duration, 10000 for "release" AnimStart

int paletteInd = random(PALS);
int animInd = -1;

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

extern Adafruit_NeoPixel pixels;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.print(F("RAM="));Serial.println(freeRam());
#endif
  pixels.begin();
  Serial3.begin(115200);
  randomSeed(analogRead(0)*analogRead(1));
  anim.setAnim(animInd);
  anim.setPeriod(20);
  anim.setPalette(pals[0]);
  anim.doSetUp();
#ifndef BTHS
  //Serial3.listen();
#endif

}

void loop() {
  /* this piece of code checks for looping while trying to find different colors
  for (int pi=0;pi<PALS;pi++) {
    int c = 0;

    Serial.print(F("pi="));Serial.print(pi);
    Color c1 = pals[pi]->getPalColor((float)rngb()/256);
    Color c2 = c1;
    while (c1.isCloseTo(c2)) {
      c = c + 1;
      c2 = pals[pi]->getPalColor((float)rngb()/256);
    }
    Serial.print(F(" c="));Serial.println(c);
  }
  /**/

  if (Serial3.available()) 
    {
        if (cmdPos == 0)
        {
            byte b;

            do
            {
                b = Serial3.read();
#ifdef DEBUG
                Serial.print(b); Serial.write(32);
#endif
            } while (Serial3.available() && b != COMMAND_MARKER);

            if (b == COMMAND_MARKER)
                cmdPos = 1;
        }

        if (cmdPos == 1)
        {
            while (Serial3.available() && cmdPos < 2)
            {
                byte b = Serial3.read();
#ifdef DEBUG
                Serial.print(b); Serial.write(32);
#endif
                commandLength = b; //Считываем длину команды
                cmdPos = 2;
            }
        }
        
        if (cmdPos > 1)
        {
            *command = new byte[commandLength];
            while (Serial3.available() && cmdPos < commandLength + 2)
            {
                byte b = Serial3.read();
#ifdef DEBUG
                Serial.print(b); Serial.write(32);
#endif
                command[(cmdPos++) - 2] = b;
            }

            if (cmdPos >= commandLength + 2)
            {
                byte cs = 0;
                for (byte i = 0; i < commandLength - 1; i++)
                {
                    cs += command[i];
                }
#ifdef DEBUG
                Serial.write('c'); Serial.println(cs);
#endif
                commandComplete = (cs == command[commandLength - 1]);
                cmdPos = 0;
            }
        }
    }
  
  if (anim.run()) 
  {
    if (commandComplete && command[0] == CMD_SETAP) 
    {
      if ((command[1] < ANIMS) && (command[2] < PALS)) 
      {
          anim.setAnim(command[1]);
          anim.setPalette(pals[command[2]]);
          Serial3.write(command, 3);
          ms = millis() + INTERVAL;
      }
    }

    if (commandComplete && command[0] == CMD_MAGIC) 
    {
      animInd = 7; //todo: why 7? (remove magik constant)
      anim.setAnim(animInd);
      anim.doSetUp();
      //todo: why 60000? (remove magik constant)
      ms = millis() + 60000;
      Serial3.print('!');
    }
    
    commandComplete = false;
    delete [] command;
  }

  // auto change effect
  if ((millis() > ms) && ( ! disableAutoChangeEffects)) {
    //TODO: rollover bug. after 49 days uptime this code will be broken
    //      (correct code in: https://www.arduino.cc/en/tutorial/BlinkWithoutDelay and https://arduino.stackexchange.com/questions/12587/how-can-i-handle-the-millis-rollover )
    ms = millis() + INTERVAL;
    switch ( (animInd < 0) ? 0 : random(2)) {
      case 0:
      {
        int prevAnimInd = animInd;
        while (prevAnimInd == animInd) animInd = random(ANIMS);
        anim.setAnim(animInd);
        anim.setPeriod(random(20, 40));
        anim.setPalette(pals[paletteInd]);
        anim.doSetUp();
        break;
      }
      case 1:
      {
        int prevPalInd = paletteInd;
        while (prevPalInd == paletteInd) paletteInd = random(PALS);
        anim.setPalette(pals[paletteInd]);
        break;
      }
    }
    Serial3.print(F(">"));Serial3.print(animInd);Serial3.print(F("\t"));Serial3.println(paletteInd);
#ifndef BTHS
    //Serial3.listen();
#endif
  }


}
