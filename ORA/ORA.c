/*
 * ORA.c
 *
 * Created: 2015.09.03. 20:59:55
 *  Author: Zoli
 */ 

#define F_CPU 6000000UL
#define ANTIPRELL 30

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

/*
* init függvényben történnek meg a beállítások
* inicializálás közben a megszakítások globálisan ki vannak kapcsolva!
*
*/

//						0			1			2		3		4			5		6			7		8			9
//#define SZAMOK [10] = {00111111, 00000110, 01011011, 01001111, 01100110, 01101101, 01011111, 00000111, 01111111, 01100111 };
//#define SZAMOK [10] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10100000, 0b11111000, 0b10000000, 0b10011000 };
//						0			1			2			3			4			5			6			7			8			9
const uint8_t SZAMOK[10] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000010, 0b11111000, 0b10000000, 0b10010000 };
const uint8_t DIGIT[4] = {0b01000000, 0b00100000, 0b00010000, 0b00001000 };

volatile uint8_t ciklus=0;
volatile uint8_t masodperc=0;
volatile uint8_t kijelzes=0;
volatile uint8_t prell=ANTIPRELL;
uint8_t t_sec=0;
uint8_t perc=0;
uint8_t t_perc=0;
uint8_t kijelzes_Seged=0;

void init(void)
{
	cli();   //globális megszakítás kezelés kikapcsolása
	DDRB =  0xFF;
	DDRD =  0x78; // 0b1111000; utolsó három pd6,pd5,pd4,pd3 kimenet | pd2,pd1, pd0 bemenet
	PORTD |=  (1<<PD2);	//felhúzó ellenállás bekapcsolása hogy GND-re lehúzva lehessen használni
	PORTD |=  (1<<PD1);//felhúzó ellenállás bekapcsolása hogy GND-re lehúzva lehessen használni
	PORTD |=  (1<<PD0);//felhúzó ellenállás bekapcsolása hogy GND-re lehúzva lehessen használni
	
	TCCR0B |=(1<<CS02);  //256-elõosztó elvileg egy ciklus 1 464,84375
	TIMSK |= (1<<TOIE0);
	sei();   //globális megszakítás kezelés bekapcsolása

}

ISR(TIMER0_OVF_vect)
{
	
	ciklus++;
	if (ciklus == 92) {ciklus=0; masodperc++;}
	if (prell < ANTIPRELL) prell++;  //prellezésmentesítés 30
	
	
}



	
int main(void)
{
	init();

	

    while(1)
    {
		//PORTD |= (1<<PORTD6); // a másodperc digit fixen tartása (ez majd nem kell)
		//PORTB = SZAMOK[masodperc]; // portb szegmenseinek kijelzése
		switch (kijelzes)
		{
		case 0 :
			PORTD = DIGIT[kijelzes];
			PORTB = SZAMOK[masodperc];
			break;
		case 1 :
			PORTD = DIGIT[kijelzes];
			PORTB = SZAMOK[t_sec];
			break;
		case 2 :
			PORTD = DIGIT[kijelzes];
			PORTB = SZAMOK[perc];
			break;
		case 3 :
			PORTD = DIGIT[kijelzes];
			PORTB = SZAMOK[t_perc];
			break;
		
		default:
			break;
		};
		
       if (masodperc==10) 
	   {
		   masodperc=0;
		   t_sec++; 
		   if (t_sec==6) 
		   {
			   t_sec=0;
			   perc++;
			   if (perc==10) 
			   {
					perc=0;
					t_perc++;
					if(t_perc==6)
					{
						t_perc=0;
					}
			   }
			   
		   }
	   }
	   kijelzes_Seged++;
	   if (kijelzes_Seged==100)
	   {
			kijelzes++;
			if (kijelzes == 4) kijelzes=0;
			kijelzes_Seged=0;
	   }
	   
	   if (PIND2==0) 
	   {	
		   if(prell == ANTIPRELL)
		   {
			   t_perc++;
			   prell=0;
		   }
	  
	   }
	   else { prell=0; }
	   
		//TODO:: Please write your application code 
    }
}