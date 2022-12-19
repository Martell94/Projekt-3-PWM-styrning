#include "header.h" //Inkluderingsdirektiv.

uint16_t on_time_us; //H�r deklarerar vi dessa f�r bruk senare.
uint16_t off_time_us;

/********************************************************************************
								*void pwm_run();
				Denna funktion utg�r knytpunkt f�r all PWM-relaterad kod. 
		
			const uint8_t pin : Pinnumret f�r potentiometern.
			led_t* self : V�r struktinstans.
			const uint16_t delay_time_us : V�r delaytid, i mikrosekunder.
********************************************************************************/
void pwm_run (const uint8_t pot,
			  led_t* self,
			  const uint16_t delay_time_us){
	
	uint16_t delay_time = delay_time_us; 
	//^ Konstaterar en delaytidsvariabel och s�tter v�rdet till det ing�ende argumentet.
	
	(void)adc_read(pot); 
	/*^ Initierar PWM p� v�r pot. S�tts till void, d� vi inte
	kommer f�r n�got vettigt returv�rde n�r f�rst den initieras.*/
	adc_set_pwm(pot, delay_time, &on_time_us, &off_time_us); 
	/*^Skickar all n�dv�ndig data till v�r AD-omvandlarfunktion.*/
	led_cycle(self, on_time_us, off_time_us);
	/*^Skickar v�r led-instans samt v�rd cykeltider till led_cycle.*/
	
	return;
}


/********************************************************************************
								*uint16_t adc_read();
		H�r initierar vi AD-omvandling via div. kommandon via relevanta register.
		
			const uint8_t pin : Den pin vi �nskar g�ra AD-omvandling p�.
********************************************************************************/

uint16_t adc_read (const uint8_t pin){
	
	ADMUX = (1<<REFS0) | pin; //Initierar ADC p� v�r ing�ende pin.
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Startar ad-omvandling, och definierar prescaler.
	while (read(ADCSRA, ADIF) == 0); // H�r v�ntar vi p� att ADIF visar ett, dvs p� att omvandlingen ska vara f�rdig.
	set(ADCSRA, ADIF); //H�r ettst�ller vi v�r flagga manuellt, vilket inneb�r att den nollst�lls n�sta cykel.
	
	return ADC; //Returnerar ADC-v�rdet.
}

/********************************************************************************
								*adc_set_pwm();
		H�r initierar vi led-struktarna, f�r bruk i t�nd/sl�ck-funktionerna.
		
	const uint8_t pin : Pinnumret f�r sj�lva lysdioden.
	const uint16_t period_us: Periodtiden i mikrosekunder.
	uint16_t* on_time_us: Tiden (i mikrosekunder) som v�r diod ska vara t�nd.
	uint16_t* off_time_us: Tiden (i mikrosekunder) som v�r diod ska vara sl�ckt.
********************************************************************************/
void adc_set_pwm(const uint8_t pin,
				 const uint16_t period_us,
				 uint16_t* on_time_us,
				 uint16_t* off_time_us){
	
	const double duty_cycle = adc_read(pin)/1023.0; 
	/*^ Definierar dutycycle beroende p� vilket v�rde som returneras fr�n 
	AD-omvandlaren, dividerat med uppl�sningen av v�r omvandlare.*/
	
	*on_time_us=(uint16_t)(period_us * duty_cycle + 0.5);
	/*^ S�tter on_time till periodtiden ggr dutycycle + 0.5, 
	vi plussar med 0.5 f�r att inte v�rdet ska trunkeras.*/
	
	*off_time_us=(period_us - *on_time_us);
	/*^ Eftersom vi redan r�knat ut f�rh�llandet mellan on_time gentemot dutycyclen 
	kan vi bara dra av on_time fr�n periodtiden f�r att f� off_time.*/
	
	return;
}
