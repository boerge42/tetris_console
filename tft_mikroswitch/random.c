/*
  
   Tetris (Zufallsroutinen)
  =========================
   Uwe Berger; 2015, 2016
 
 
  ---------
  Have fun! 
 
 
 *----------------------------------------------------------------------
 * Copyright (c) 2015 Uwe Berger - bergeruw(at)gmx.net
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *----------------------------------------------------------------------
*/

#include "random.h"

// ********************************************
// seed-Wert fuer srand() generieren; Quelle: 
// http://www.roboternetz.de/wissen/index.php/Zufallszahlen_mit_avr-gcc
uint16_t get_seed(void)
{
	uint16_t seed = 0;
	uint16_t *p = (uint16_t*) (RAMEND+1);
	extern uint16_t __heap_start;
	while (p >= &__heap_start + 1)
		seed ^= * (--p);
	return seed;
}

// ********************************************
void random_init(void)
{
	// Initialisierung Zufallsgenerator
	srand(get_seed());
}


// ********************************************
uint8_t generate_random_number(void)
{
	// eine Zufallszahl generieren...
	return (rand() % MAX_BRICKS);
}
