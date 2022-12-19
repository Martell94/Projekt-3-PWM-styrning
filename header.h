#ifndef HEADER_H_ //Header-guardens början.
#define HEADER_H_

#include "misc.h" //Inkluderar vår fusk-header.

#define POT1 PORTC0 /* Potentiometer 1 ansluten till analog pin A0 (PORTC0). */

#define BUTTON1 PORTD2 /*Knapp på pin 2*/
#define BUTTON2 PORTD3 /*Knapp på pin 3*/
#define BUTTON3 PORTB3 /*Knapp på pin 11*/
#define BUTTON4 PORTB4 /*Knapp på pin 12*/
#define BUTTON5 PORTB5 /*Knapp på pin 13*/

#define BUTTON1_PRESSED read(PIND, BUTTON1) //Skapar ett makro för att läsa tryckknappens pin
#define BUTTON2_PRESSED read(PIND, BUTTON2) //Etc, etc..
#define BUTTON3_PRESSED read(PINB, BUTTON3)
#define BUTTON4_PRESSED read(PINB, BUTTON4)
#define BUTTON5_PRESSED read(PINB, BUTTON5)

typedef struct { //Här skapar vi vår led-strukt, så slipper vi ha en ensam headerfil bara för detta.
	uint8_t pin;
	ioport_t ioport;
	bool enabled;
}led_t;

extern led_t led1, led2, led3, led4, led5; 
//^Likt ett funktionshuvud -- EJ deklaration, bara som vi synliggör dessa. 

void setup (); //Här har vi samtliga funktionshuvuden. 
void delay_us (const uint16_t delay_time);
uint16_t adc_read (const uint8_t pin);
void adc_set_pwm (const uint8_t pin,const uint16_t period_us,uint16_t* on_time_us,uint16_t* off_time_us);
void pwm_run (const uint8_t pot,led_t* self, const uint16_t delay_time);
void led_cycle (led_t* self,  const uint16_t on_time_us, const uint16_t off_time_us);
void toggle_led(led_t* self);
void led_init (led_t* self, const uint8_t pin);
void init_interrupts();
void led_on (led_t* self);
void led_off (led_t* self);

#endif /* HEADER_H_ */