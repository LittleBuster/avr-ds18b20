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

#include "ds18b20.h"
#include <avr/io.h>
#include <util/delay.h>


static uint8_t read_byte(uint8_t pin)
{
	uint8_t dat = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		DDR_18B20 |= (1 << pin);
		_delay_us(2);
		DDR_18B20 &= ~(1 << pin);
		_delay_us(4);
		dat = dat >> 1;
		if (PIN_18B20 & (1 << pin))
			dat |= 0x80;
		_delay_us(62);
	}
	return dat;
}

static void write_byte(uint8_t data, uint8_t pin)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		DDR_18B20 |= (1 << pin);
		_delay_us(2);
		if (data & 0x01)
			DDR_18B20 &= ~(1 << pin);
		else
			DDR_18B20 |= (1 << pin);

		data = data >> 1;
		_delay_us(62);
		DDR_18B20 &= ~(1 << pin);
		_delay_us(2);
	}
}

static uint8_t calc_crc(uint16_t adress)
{
	uint8_t crc = 0;
	for (uint8_t i = 0; i < 8; i++)
	{
		crc = crc ^ (*(uint16_t *)(adress + i));
		for (uint8_t j = 0; j < 8; j++)
		{
			if (crc & 0x01)
				crc = (crc >> 1) ^ 0x8C;
			else
				crc >>= 1;
		}
	}
	return crc;
}

static uint8_t start_sensor(uint8_t pin)
{
	PORT_18B20 &= ~(1 << pin);
	DDR_18B20 |= (1 << pin);
	_delay_us(490);
	DDR_18B20 &= ~(1 << pin);
	_delay_us(68);
	if ((PIN_18B20 & (1 << pin)) > 0)
		return 0;
	return 1;
}

void ds18b20_init(struct ds18b20 *ds, uint8_t pin)
{
	ds->pin = pin;
}

uint8_t ds18b20_read_temp(struct ds18b20 *ds, int8_t *temp)
{
	start_sensor(ds->pin);
	_delay_us(422);
	write_byte(0xCC, ds->pin); // skip S/N
	write_byte(0x44, ds->pin); // temp convert
	_delay_ms(1000);
	start_sensor(ds->pin);
	_delay_us(422);
	write_byte(0xCC, ds->pin); // skip S/N
	write_byte(0xBE, ds->pin);

	for (uint8_t i = 0; i < 9; i++)
		ds->scratch[i] = read_byte(ds->pin);

	if (ds->scratch[8] == calc_crc((uint16_t)&ds->scratch[0]))
		*temp = (int8_t)((ds->scratch[1] << 4)|(ds->scratch[0] >> 4));
	else
		return 0;
	return 1;
}
