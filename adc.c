#include "header.h" //Inkluderingsdirektiv.

uint16_t on_time_us; //Här deklarerar vi dessa för bruk senare.
uint16_t off_time_us;

/********************************************************************************
								*void pwm_run();
				Denna funktion utgör knytpunkt för all PWM-relaterad kod. 
		
			const uint8_t pin : Pinnumret för potentiometern.
			led_t* self : Vår struktinstans.
			const uint16_t delay_time_us : Vår delaytid, i mikrosekunder.
********************************************************************************/
void pwm_run (const uint8_t pot,
			  led_t* self,
			  const uint16_t delay_time_us){
	
	uint16_t delay_time = delay_time_us; 
	//^ Konstaterar en delaytidsvariabel och sätter värdet till det ingående argumentet.
	
	(void)adc_read(pot); 
	/*^ Initierar PWM på vår pot. Sätts till void, då vi inte
	kommer får något vettigt returvärde när först den initieras.*/
	adc_set_pwm(pot, delay_time, &on_time_us, &off_time_us); 
	/*^Skickar all nödvändig data till vår AD-omvandlarfunktion.*/
	led_cycle(self, on_time_us, off_time_us);
	/*^Skickar vår led-instans samt vård cykeltider till led_cycle.*/
	
	return;
}


/********************************************************************************
								*uint16_t adc_read();
		Här initierar vi AD-omvandling via div. kommandon via relevanta register.
		
			const uint8_t pin : Den pin vi önskar göra AD-omvandling på.
********************************************************************************/

uint16_t adc_read (const uint8_t pin){
	
	ADMUX = (1<<REFS0) | pin; //Initierar ADC på vår ingående pin.
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //Startar ad-omvandling, och definierar prescaler.
	while (read(ADCSRA, ADIF) == 0); // Här väntar vi på att ADIF visar ett, dvs på att omvandlingen ska vara färdig.
	set(ADCSRA, ADIF); //Här ettställer vi vår flagga manuellt, vilket innebär att den nollställs nästa cykel.
	
	return ADC; //Returnerar ADC-värdet.
}

/********************************************************************************
								*adc_set_pwm();
		Här initierar vi led-struktarna, för bruk i tänd/släck-funktionerna.
		
	const uint8_t pin : Pinnumret för själva lysdioden.
	const uint16_t period_us: Periodtiden i mikrosekunder.
	uint16_t* on_time_us: Tiden (i mikrosekunder) som vår diod ska vara tänd.
	uint16_t* off_time_us: Tiden (i mikrosekunder) som vår diod ska vara släckt.
********************************************************************************/
void adc_set_pwm(const uint8_t pin,
				 const uint16_t period_us,
				 uint16_t* on_time_us,
				 uint16_t* off_time_us){
	
	const double duty_cycle = adc_read(pin)/1023.0; 
	/*^ Definierar dutycycle beroende på vilket värde som returneras från 
	AD-omvandlaren, dividerat med upplösningen av vår omvandlare.*/
	
	*on_time_us=(uint16_t)(period_us * duty_cycle + 0.5);
	/*^ Sätter on_time till periodtiden ggr dutycycle + 0.5, 
	vi plussar med 0.5 för att inte värdet ska trunkeras.*/
	
	*off_time_us=(period_us - *on_time_us);
	/*^ Eftersom vi redan räknat ut förhållandet mellan on_time gentemot dutycyclen 
	kan vi bara dra av on_time från periodtiden för att få off_time.*/
	
	return;
}
