#define F_CPU 1000000L
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

volatile uint8_t sec = 0;
uint8_t mode = 0;
bool ini = false;


uint8_t* decToBin(uint8_t dec) {
	static uint8_t bin[6];
 	int dig = 0;

 	bin[0] = 0;
  	bin[1] = 0;
  	bin[2] = 0;
  	bin[3] = 0;
  	bin[4] = 0;
  	bin[5] = 0;
  
  	while(dec != 0) {
		bin[dig] = dec % 2;
    	dec = dec / 2;
    	dig++;
	}
 	return bin;

}

void showSec(uint8_t bin[6]) {
	if(bin[0] == 1)
    	PORTC |= (1 << 0);
  	else
    	PORTC &= ~(1 << 0);

  	if(bin[1] == 1)
    	PORTC |= (1 << 1);
  	else
    	PORTC &= ~(1 << 1);

  	if(bin[2] == 1)
    	PORTC |= (1 << 2);
  	else
    	PORTC &= ~(1 << 2);

  	if(bin[3] == 1)
    	PORTC |= (1 << 3);
  	else
    	PORTC &= ~(1 << 3);

  	if(bin[4] == 1)
    	PORTC |= (1 << 4);
  	else
    	PORTC &= ~(1 << 4);

  	if(bin[5] == 1)
    	PORTC |= (1 << 5);
  	else
    	PORTC &= ~(1 << 5);
  
}

void showMin(uint8_t bin[6]) {
	if(bin[0] == 1)
    	PORTD |= (1 << 1);
  	else
    	PORTD &= ~(1 << 1);

  	if(bin[1] == 1)
    	PORTD |= (1 << 5);
  	else
    	PORTD &= ~(1 << 5);

  	if(bin[2] == 1)
    	PORTB |= (1 << 0);
  	else
    	PORTB &= ~(1 << 0);

  	if(bin[3] == 1)
    	PORTB |= (1 << 1);
  	else
    	PORTB &= ~(1 << 1);

  	if(bin[4] == 1)
    	PORTB |= (1 << 2);
  	else
    	PORTB &= ~(1 << 2);

}

void onSec() {

    	PORTC |= (1 << 0);
    	PORTC |= (1 << 1);
    	PORTC |= (1 << 2);
    	PORTC |= (1 << 3);
    	PORTC |= (1 << 4);
    	PORTC |= (1 << 5);
  
}

void onMin() {

    	PORTD |= (1 << 5);
    	PORTD |= (1 << 1);
    	PORTB |= (1 << 0);
    	PORTB |= (1 << 1);
    	PORTB |= (1 << 2);
}

void countSec() {
  	sec ++;
  	if(sec > 63)
    	sec = 0;
    
}

uint8_t prell() {
	_delay_ms(100);  
	return (PIND & (1 << PD2)); 
    
}

ISR(INT0_vect) {
  	if(prell() == 0 && ini) {
    	switch(mode) {
      		case 0: {
				showSec(decToBin(0));
				showMin(decToBin(0));
        		mode = 1;
        		break;
      		}
      		case 1: {
				showSec(decToBin(0));
				showMin(decToBin(0));
        		mode = 2;
        		break;
      		}
			case 2: {
        		mode = 0;
        		break;
      		}
    	}
	}  
  
}


int main() {
  	DDRC |= (1 << 5);
  	DDRC |= (1 << 4);
  	DDRC |= (1 << 3);
  	DDRC |= (1 << 2);
  	DDRC |= (1 << 1);
  	DDRC |= (1 << 0);

  	DDRD |= (1 << 5);
  	DDRD |= (1 << 1);
  	DDRB |= (1 << 0);
  	DDRB |= (1 << 1);
  	DDRB |= (1 << 2);
  
  	DDRD &= ~(1 << 0);
  	DDRD &= ~(1 << 7);
  	DDRD &= ~(1 << 2);
  
  	PORTD |= (1 << 0);
  	PORTD |= (1 << 7);
  	PORTD |= (1 << 2);

  	EICRA |= (1 << ISC00);
  	EIMSK |= (1 << INT0);
  	sei();
  	ini = true;

	while(1) {
    	countSec();
    
  		switch(mode) {
    		case 0: {
      			showSec(decToBin(sec));
      			_delay_ms(650);
      			break;
    		} 
    		case 1: {
      			showMin(decToBin(sec));
      			_delay_ms(650);
      			break;
    		}
			case 2: {
				showSec(decToBin(0));
				showMin(decToBin(0));

      			while((PIND & (1 << PD0)) == 0)
					onSec();
				while((PIND & (1 << PD7)) == 0)
					onMin();		
      			break;
    		}
  		}
	}
	return 0;

}
