#include <avr/io.h>
#include <SPI.h>

void SPI::init(){
	if(SPI_MASTER){
		DDRB &= 0b11000110; // select the pins to be set
		DDRB |= 0b00101001; // set pins SCK|MOSI|CS as output and MISO as input
		
		SPCR |= 0b01011101; //setting SPI Control Register to 0x5D; SPI Enable|Master Select|Clock Pol|Clock Phase|SPR0
	}
	else if(SPI_SLAVE){
		DDRB &= 0b11000111; // select the pins to be set
		DDRB |= 0b00110000; // set pins SCK|MOSI|CS as output and ~MISO as input
		
		SPCR |= 0b01001101; //setting SPI Control Register to 0x5D; SPI Enable|~Slave Select|Clock Pol|Clock Phase|SPR0	
	}
	
	SPSR &= ~(1<<SPI2X);
}

char SPI::write(){
	if(SPI_MASTER){
		char flush_buffer;
		SPDR = 'a';			/* Write data to SPI data register */
		while(!(SPSR & (1<<SPIF)));	/* Wait till transmission complete */
		flush_buffer = SPDR;
		return flush_buffer;
	}
}