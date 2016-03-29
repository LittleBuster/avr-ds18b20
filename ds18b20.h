/* DS18B20 thermometer AVR library
 *
 * Copyright (C) 2016 Sergey Denisov.
 * Rewritten by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 */

#ifndef __DS18B20_H__
#define __DS18B20_H__

#include <stdio.h>


#define DDR_18B20 DDRB
#define PORT_18B20 PORTB
#define PIN_18B20 PINB


struct ds18b20 {
	uint8_t pin;
	uint8_t scratch[9];
};

/**
 * Thermometer initialization
 * @ds: ds18b20 structure
 * @pin: thermometer pin
 */
void ds18b20_init(struct ds18b20 *ds, uint8_t pin);

/**
 * Reding temperature from thermometer
 * @ds: ds18b20 structure
 * @temp: temperature pointer
 */
uint8_t ds18b20_read_temp(struct ds18b20 *ds, int8_t *temp);


#endif
