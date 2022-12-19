/********************************************************************************
									* main.c:		
			Programmets huvudfil. H�r har vi v�ra interrupt-rutiner,
			samt den while-loop som ska k�ras o�ndligt medan maskinen
			�r ig�ng.
********************************************************************************/
#include "header.h" //Inkluderar v�r headerfil.

/********************************************************************************
								ISR(INT0_vect):
			H�r best�mmer vi vad som ska h�nda n�r en interrupt sker p� pin 
			2 och 3. Dessa har sina egna interruptrutiner.
********************************************************************************/
ISR(INT0_vect){ //Denna vektor styr interrupts p� pin 2, dvs d�r knapp 1 sitter.
	toggle_led(&led1); //N�r knapp 1 trycks ned sker avbrott p� led 1.
}

ISR (INT1_vect){  //Denna vektor styr interrupts p� pin 3, d�r knapp 2 sitter.
   toggle_led(&led2); //Samma h�r. 
}

/********************************************************************************
								ISR(PCINT0_vect):	
			H�r best�mmer vi vad som ska h�nda n�r en interrupt sker p� 
			PCI-registret. Eftersom dessa inte har sina egna interrupts,
			utan interrupten sker p� hela registret, f�r vi ha en 
			if-sats f�r att avg�ra specifikt vilken lampa som ska t�ndas.
********************************************************************************/
ISR(PCINT0_vect){ //H�r har vi v�r PCI-avbrottrutin, d�r vi hamnar n�r avbrott sker p� pin 
	if(BUTTON3_PRESSED){ //Ifall knapp 3 �r nedtryckt, togglar vi enabled-boolen p� led 3-strukten.
		toggle_led(&led3);
	}
	else if (BUTTON4_PRESSED){//Ifall knapp 4 �r nedtryckt, etc etc...
		toggle_led(&led4);
	}
	else if (BUTTON5_PRESSED){
		toggle_led(&led5);
	}
}

/********************************************************************************
								int main(void):
			Programmets startpunkt. H�r k�r vi v�r setup-funktion, samt v�r
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