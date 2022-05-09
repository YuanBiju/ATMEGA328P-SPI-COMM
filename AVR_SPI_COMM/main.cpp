/*
 * AVR_SPI_COMM.cpp
 *
 * Created: 07-05-2022 16:45:04
 * Author : yuann
 */ 

#define F_CPU 16000000
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define MASTER 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//#include "addon_libs/SPI.h"

char data_in;

void usart_init(){
	UBRR0H |= (BAUDRATE >> 8);
	UBRR0L |= BAUDRATE; // load the baudrate to UBRR register
	
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); // enable transmitter and receiver
	UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); // set frame size as 8 bits
}

void usart_write(){
	while (!( UCSR0A & (1<<UDRE0))); // loop until data is available in UDR
	UDR0 = data_in;
}

void SPI_init(){
	if(MASTER){
		DDRB &= 0b11000110; // select the pins to be set
		DDRB |= 0b00101001; // set pins SCK|MOSI|CS as output and MISO as input
		
		//PORTB &= 0b11111110;
		
		SPCR &= 0b01011101; //setting SPI Control Register to 0x5D; SPI Enable|Master Select|Clock Pol|Clock Phase|SPR0
		
		data_in = 'M';
		usart_write();
	}
	else if(!MASTER){
		DDRB &= 0b11000011; // select the pins to be set
		DDRB |= 0b00110000; // set pins SCK|MOSI|CS as output and ~MISO as input
		
		PORTB |= 0b0000000;
		
		SPCR &= 0b01001101; //setting SPI Control Register to 0x5D; SPI Enable|~Slave Select|Clock Pol|Clock Phase|SPR0
		
		data_in = 'S';
		usart_write();
	}
}

char SPI_write(){
	char flush_buffer;
	if(MASTER){
		PORTB &= 0b11111110;
	}
	SPDR = 'a';			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF))){
		data_in = 'L';
		usart_write();
		};	/* Wait till transmission complete */
	
	if(MASTER){
		PORTB &= 0b11111111;
	}
	flush_buffer = SPDR;
	return flush_buffer;
}

int main(void)
{
	sei(); // enable global interrupts
	SPI_init();
	usart_init(); // initialize USART 
	
    /* Replace with your application code */
    while (1) 
    {
		data_in = SPI_write();
		usart_write();
		
    }
}

