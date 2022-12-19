#include "header.h" //Inkluderar v�r headerfil. 

/********************************************************************************
								*void led_init();
		H�r initierar vi led-struktarna, f�r bruk i t�nd/sl�ck-funktionerna.
		
				led_t* self : Den struktinstans vi �nskar tilldela v�rden.
				const uint8_t pin : Pinnumret f�r sj�lva lysdioden.
********************************************************************************/
void led_init (led_t* self, const uint8_t pin){ 
	
	self->pin = pin; //S�tter struktens pin till den vi tar emot som ing�ende argument.
	
	/*Denna if-sats st�ller struktens port beroende p� pin-variabelns v�rde.*/
	if (pin >= 0 && pin <= 7){ 
		self->ioport = IO_PORTD; //Om pin >=0 eller <7 tillskrivs den PORTD.
		set(DDRD, self->pin); //H�r anv�nder vi v�rt set-makro f�r att ettst�lla pinen.
	}
	else if (pin >=8 && pin <= 13){ // Om v�rdert �verstiger 8 s�tts den p� PORTB.
		self->ioport = IO_PORTB; //Etc, etc...
		set(DDRB, self->pin - 8);
	}
	self->enabled = false; //Defaulttillst�ndet f�r lysdioden, dvs av. 
	return;
}

/********************************************************************************
								*void toggle_led();
		H�r togglar vi lysdioderna genom att invertera deras bool-v�rde.
		
				led_t* self : Den struktinstans vi vill toggla.
********************************************************************************/
void toggle_led(led_t* self){
	self->enabled = !self->enabled;
	return;
}

/********************************************************************************
								*void led_on();
	 H�r t�nder vi lysdioderna beroende p� vilket v�rde v�r enabled-bool har.
		
				led_t* self : Den struktinstans vi �nskar t�nda.
********************************************************************************/
void led_on (led_t* self){
	
	if (!self->enabled) return; //Ifall boolv�rdet �r false skickas vi tillbaka.
	
	/*H�r ettst�lls pinen beroende p� vilken pin den inskickade led-instansen har*/
	if (self->pin >= 0 && self->pin <= 7){ 
		set(PORTD, self->pin); //H�r anv�nder vi v�rt set-makro.
	}
	else if (self->pin >=8 && self->pin <=13){
	
	/*Eftersom pin-variablen kan ha v�rden �verstigande 8 beh�ver 
	vi reducera det med �tta f�r PORTB-registret.*/	
	set(PORTB, self->pin - 8); 
	}
	return;
}

/********************************************************************************
								*void led_off();
		H�r sl�cker vi lysdioderna beroende p� vilket v�rde v�r enabled-bool
		har, i �vrigt �r den funktionellt samma som funktionen ovan. 
		
				led_t* self : Den struktinstans vi �nskar sl�cka.
********************************************************************************/
void led_off (led_t* self){
	
	if (self->pin >= 0 && self->pin <= 7){
		clr(PORTD, self->pin); 
		//^H�r anv�nder vi v�rt clear-makro f�r att nollst�lla relevant bit.
	}
	else if (self->pin >=8 && self->pin <=13){ //Samma h�r, men f�r pinnr �verstigande 8. 
		clr(PORTB, self->pin - 8);
	}
	return;
}

/********************************************************************************
								*void led_off();
		H�r sl�cker vi lysdioderna beroende p� vilket v�rde v�r enabled-bool
		har, i �vrigt �r den funktionellt samma som funktionen ovan. 
		
			led_t* self : Den struktinstans vi �nskar styra.
			on_time_us : Den tid som lysdioden ska vara t�nd i v�r PWM-cykel.
			off_time_us : Den tid som lysdioden ska vara sl�ckt.
********************************************************************************/
void led_cycle (led_t* self, const uint16_t on_time_us, const uint16_t off_time_us){

	led_on(self); //H�r skickas v�r led-instans till led_on-funktionen.
	delay_us(on_time_us); //H�r skickar vi on_time till v�r mikrosekundsdelay-funktion.
	led_off(self); //Etc, etc...
	delay_us(off_time_us);
	
	return;
}