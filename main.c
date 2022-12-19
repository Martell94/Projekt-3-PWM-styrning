/********************************************************************************
									* main.c:		
			Programmets huvudfil. Här har vi våra interrupt-rutiner,
			samt den while-loop som ska köras oändligt medan maskinen
			är igång.
********************************************************************************/
#include "header.h" //Inkluderar vår headerfil.

/********************************************************************************
								ISR(INT0_vect):
			Här bestämmer vi vad som ska hända när en interrupt sker på pin 
			2 och 3. Dessa har sina egna interruptrutiner.
********************************************************************************/
ISR(INT0_vect){ //Denna vektor styr interrupts på pin 2, dvs där knapp 1 sitter.
	toggle_led(&led1); //När knapp 1 trycks ned sker avbrott på led 1.
}

ISR (INT1_vect){  //Denna vektor styr interrupts på pin 3, där knapp 2 sitter.
   toggle_led(&led2); //Samma här. 
}

/********************************************************************************
								ISR(PCINT0_vect):	
			Här bestämmer vi vad som ska hända när en interrupt sker på 
			PCI-registret. Eftersom dessa inte har sina egna interrupts,
			utan interrupten sker på hela registret, får vi ha en 
			if-sats för att avgöra specifikt vilken lampa som ska tändas.
********************************************************************************/
ISR(PCINT0_vect){ //Här har vi vår PCI-avbrottrutin, där vi hamnar när avbrott sker på pin 
	if(BUTTON3_PRESSED){ //Ifall knapp 3 är nedtryckt, togglar vi enabled-boolen på led 3-strukten.
		toggle_led(&led3);
	}
	else if (BUTTON4_PRESSED){//Ifall knapp 4 är nedtryckt, etc etc...
		toggle_led(&led4);
	}
	else if (BUTTON5_PRESSED){
		toggle_led(&led5);
	}
}

/********************************************************************************
								int main(void):
			Programmets startpunkt. Här kör vi vår setup-funktion, samt vår
			while-loop. 
********************************************************************************/
int main(void){
	
	setup(); /* Initierar I/O-portar samt AD-omvandlaren vid start. */
	
	while (1){
		pwm_run(POT1, &led1, 1000); /* Styr lysdiod 1 via potentiometer 1, 1000 us periodtid. */
		pwm_run(POT1, &led2, 1000); /* Styr lysdiod 2 via potentiometer 1, 1000 us periodtid. */
		pwm_run(POT1, &led3, 1000); /*Etc, etc....  */
		pwm_run(POT1, &led4, 1000); 
		pwm_run(POT1, &led5, 1000); 
	}
	return 0; 
}