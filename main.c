# Copyright (c) 2010, Bjoern Heller <tec@hellercom.de>. All rights reserved
# This code is licensed under GNU/ GPL

#include <avr/io.h>
#include <avr/delay.h>
#include <util/delay.h>
#include <inttypes.h>

#ifndef F_CPU
#define F_CPU 11059200UL
#endif

unsigned char i;                //Laufvariable für Warteschleife
unsigned char run;                //Startvariable für Lauflicht
unsigned char bit;                //Aufwärtslaufendes bit
unsigned char bit2;                //Voraussetzung für Abwärtsschleife
unsigned char bit3;                //Abwärtslaufendes bit
unsigned char speed;


void wait(uint8_t speed){               //Warteschleife, beeinflusst Lauflichtgeschwindigkeit
    for (i=1; i<=speed; i++)         
              _delay_ms(10);}

int main (void){
  
  DDRB = 0xFF;                //Port B als Ausgang
  PORTB = 0xFF;                //alle LED's aus
  DDRA = 0x00;                //Port D als Eingang

  bit2 = 0;                  //bit2 vordefinieren
  bit3 = 128;                  //bit3 vordefinieren
  while(1){
  
  switch(PIND){                //Startbefehl abfragen

    case 0xFE: run=1; speed=64; break;      //SW0: Start, Warteschleife wird 64 mal durchlaufen
    case 0xFD: run=1; speed=32; break;      //SW1: Start, Warteschleife wird 32 mal durchlaufen
    case 0xFB: run=1; speed=16; break;      //SW2: Start, Warteschleife wird 16 mal durchlaufen
    case 0xF7: run=1; speed=8; break;      //SW3: Start, Warteschleife wird 8 mal durchlaufen
    case 0xEF: run=1; speed=4; break;       //SW4: Start, Warteschleife wird 4 mal durchlaufen
    case 0xDF: run=1; speed=2; break;       //SW5: Start, Warteschleife wird 2 mal durchlaufen
    case 0xBF: run=1; speed=1; break;       //SW6: Start, Warteschleife wird 1 mal durchlaufen
    case 0x7F: run=0; break;          //SW7: Stopp
    default: break;                //Wenn keine Taste gedrückt: Schleife ohne Veränderung verlassen
    }



  if(run){                  //Lauflicht einschalten bei run=1
  
    if(bit2==0){
    PORTB =~bit;              //bit komplementär auf PORTB ausgeben
    bit = bit<<1;}              //aufwärts
    if (bit==0){              //ganz oben:  
      bit2=1;                //Bedingung für Abwärtsschleife
      bit=1;}                //bit wieder auf Anfangswert
    else wait(speed);            //Warteschleife aufwärts
    if (bit2==1){              //Abwärtsschleife
      PORTB =~bit3;            //bit3 komplementär auf PORTB ausgeben
      bit3 = bit3>>1;}          //abwärts
    if (bit3==0){              //ganz unten:
        bit2=0;              //Schleife verlassen
        bit3=128;}            //bit3 wieder auf Anfangswert
    else wait(speed);            //Warteschleife abwärts
    
    }  
      
    
  else {                    //bei run=0:
    PORTB=0xFF;                //alle LED's aus
    bit=1;
    bit3=128;
    bit2=0;}}                //bits auf Anfangswert, damit immer bei LED0 begonnen wird

return 0;                       //never reached
}


