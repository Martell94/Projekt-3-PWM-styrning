#include "header.h"

led_t led1, led2, led3, led4, led5; //Här deklareras dessa struktar för bruk.
static inline void init_interrupts();

/********************************************************************************
							*void setup();
Denna rutin initierar våra deklarerade led_struktar, samt ettställer div. portar.
********************************************************************************/
void setup(){
		
	led_init (&led1, 6); //Skickar vår ledstrukt samt dess pinnr till led_init.
	led_init (&led2, 7); //Etc, etc...
	led_init (&led3, 8);
	led_init (&led4, 9);
	led_init (&led5, 10);
	
	PORTD = (1 << BUTTON1) | (1 << BUTTON2); 
	//^Här skippar vi att använda vårt set-makro, då vi inte kan ettställa flera samtidigt med det. 
	PORTB = (1<<BUTTON3) | (1<<BUTTON4) | (1<<BUTTON5); //Samma här.
 
	init_interrupts(); //Hoppar till denna funktion.
	return;
}

/********************************************************************************
							*init_interrupts();
		Initierar våra interrupts, både i PCI samt EICRA-registret. 
		Denna har typen inline, vilket innebär att när den kallas på,
		så klistras den in i den funktion den kallas från istället för
		att vi hoppar hit. Detta sparar några moment i programexekverigen.
********************************************************************************/
static inline void init_interrupts(){
	
	asm("SEI"); //Assemblerinstruktion för att initierar nterrupts globalt.
	EICRA |= ((1<<ISC01) | (1<< ISC00) | (1<<ISC11) | (1<< ISC10)); //Här initierar vi vår avbrottsvektor.
	
	
	EIMSK |= (1<<INT0) |  (1<<INT1);
	/*^Initierar interrupts på stigande flank.*/
	
	PCICR |= (1<<PCIE0); //Initierar interrupts via PCI-registret. 
	PCMSK0 |= (1 << BUTTON3) | (1 << BUTTON4) | (1 << BUTTON5); //Initierar interrupts på specifikt knapp-pinsen.
	return;
}