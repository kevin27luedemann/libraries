/*
 * Created by Kevin Luedemann
 * 09.09.2017 12:27
 */

#ifndef __SPI_H__
#define __SPI_H__

#include <avr/io.h>

class SPI
{
public:
	SPI();
	~SPI();
	inline static uint8_t transfer(uint8_t d);
	inline static void transfer(void *buf, size_t count);

private:

};

#endif
