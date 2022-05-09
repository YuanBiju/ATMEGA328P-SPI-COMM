#pragma once

#define SPI_MASTER 1;
#define SPI_SLAVE 0;

#include <avr/io.h>

class SPI 
{
public:
	void init();
	char write();
};