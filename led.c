#include "header.h" //Inkluderar vår headerfil. 

/********************************************************************************
								*void led_init();
		Här initierar vi led-struktarna, för bruk i tänd/släck-funktionerna.
		
				led_t* self : Den struktinstans vi önskar tilldela värden.
				const uint8_t pin : Pinnumret för själva lysdioden.
********************************************************************************/
void led_init (led_t* self, const uint8_t pin){ 
	
	self->pin = pin; //Sätter struktens pin till den vi tar emot som ingående argument.
	
	/*Denna if-sats ställer struktens port beroende på pin-variabelns värde.*/
	if (pin >= 0 && pin <= 7){ 
		self->ioport = IO_PORTD; //Om pin >=0 eller <7 tillskrivs den PORTD.
		set(DDRD, self->pin); //Här använder vi vårt set-makro för att ettställa pinen.
	}
	else if (pin >=8 && pin <= 13){ // Om värdert överstiger 8 sätts den på PORTB.
		self->ioport = IO_PORTB; //Etc, etc...
		set(DDRB, self->pin - 8);
	}
	self->enabled = false; //Defaulttillståndet för lysdioden, dvs av. 
	return;
}

/********************************************************************************
								*void toggle_led();
		Här togglar vi lysdioderna genom att invertera deras bool-värde.
		
				led_t* self : Den struktinstans vi vill toggla.
********************************************************************************/
void toggle_led(led_t* self){
	self->enabled = !self->enabled;
	return;
}

/********************************************************************************
								*void led_on();
	 Här tänder vi lysdioderna beroende på vilket värde vår enabled-bool har.
		
				led_t* self : Den struktinstans vi önskar tända.
********************************************************************************/
void led_on (led_t* self){
	
	if (!self->enabled) return; //Ifall boolvärdet är false skickas vi tillbaka.
	
	/*Här ettställs pinen beroende på vilken pin den inskickade led-instansen har*/
	if (self->pin >= 0 && self->pin <= 7){ 
		set(PORTD, self->pin); //Här använder vi vårt set-makro.
	}
	else if (self->pin >=8 && self->pin <=13){
	
	/*Eftersom pin-variablen kan ha värden överstigande 8 behöver 
	vi reducera det med åtta för PORTB-registret.*/	
	set(PORTB, self->pin - 8); 
	}
	return;
}

/********************************************************************************
								*void led_off();
		Här släcker vi lysdioderna beroende på vilket värde vår enabled-bool
		har, i övrigt är den funktionellt samma som funktionen ovan. 
		
				led_t* self : Den struktinstans vi önskar släcka.
********************************************************************************/
void led_off (led_t* self){
	
	if (self->pin >= 0 && self->pin <= 7){
		clr(PORTD, self->pin); 
		//^Här använder vi vårt clear-makro för att nollställa relevant bit.
	}
	else if (self->pin >=8 && self->pin <=13){ //Samma här, men för pinnr överstigande 8. 
		clr(PORTB, self->pin - 8);
	}
	return;
}

/********************************************************************************
								*void led_off();
		Här släcker vi lysdioderna beroende på vilket värde vår enabled-bool
		har, i övrigt är den funktionellt samma som funktionen ovan. 
		
			led_t* self : Den struktinstans vi önskar styra.
			on_time_us : Den tid som lysdioden ska vara tänd i vår PWM-cykel.
			off_time_us : Den tid som lysdioden ska vara släckt.
********************************************************************************/
void led_cycle (led_t* self, const uint16_t on_time_us, const uint16_t off_time_us){

	led_on(self); //Här skickas vår led-instans till led_on-funktionen.
	delay_us(on_time_us); //Här skickar vi on_time till vår mikrosekundsdelay-funktion.
	led_off(self); //Etc, etc...
	delay_us(off_time_us);
	
	return;
}