#include "header.h"

led_t led1, led2, led3, led4, led5; //H�r deklareras dessa struktar f�r bruk.
static inline void init_interrupts();

/********************************************************************************
							*void setup();
Denna rutin initierar v�ra deklarerade led_struktar, samt ettst�ller div. portar.
********************************************************************************/
void setup(){
		
	led_init (&led1, 6); //Skickar v�r ledstrukt samt dess pinnr till led_init.
	led_init (&led2, 7); //Etc, etc...
	led_init (&led3, 8);
	led_init (&led4, 9);
	led_init (&led5, 10);
	
	PORTD = (1 << BUTTON1) | (1 << BUTTON2); 
	//^H�r skippar vi att anv�nda v�rt set-makro, d� vi inte kan ettst�lla flera samtidigt med det. 
	PORTB = (1<<BUTTON3) | (1<<BUTTON4) | (1<<BUTTON5); //Samma h�r.
 
	init_interrupts(); //Hoppar till denna funktion.
	return;
}

/********************************************************************************
							*init_interrupts();
		Initierar v�ra interrupts, b�de i PCI samt EICRA-registret. 
		Denna har typen inline, vilket inneb�r att n�r den kallas p�,
		s� klistras den in i den funktion den kallas fr�n ist�llet f�r
		att vi hoppar hit. Detta sparar n�gra moment i programexekverigen.
********************************************************************************/
static inline void init_interrupts(){
	
	asm("SEI"); //Assemblerinstruktion f�r att initierar nterrupts globalt.
	EICRA |= ((1<<ISC01) | (1<< ISC00) | (1<<ISC11) | (1<< ISC10)); //H�r initierar vi v�r avbrottsvektor.
	
	
	EIMSK |= (1<<INT0) |  (1<<INT1);
	/*^Initierar interrupts p� stigande flank.*/
	
	PCICR |= (1<<PCIE0); //Initierar interrupts via PCI-registret. 
	PCMSK0 |= (1 << BUTTON3) | (1 << BUTTON4) | (1 << BUTTON5); //Initierar interrupts p� specifikt knapp-pinsen.
	return;
}