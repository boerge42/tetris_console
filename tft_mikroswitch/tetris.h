/*
  
     Tetris (Definitionen)
  ===========================
     Uwe Berger; 2015, 2020
 
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

#ifndef _TETRIS_H
#define _TETRIS_H


// Groesse Spielfeld 
#define GRID_DX		10
#define GRID_DY		20



// Farbdefinitionen
#define BLANK		0
#define GREEN		1
#define BLUE		2
#define YELLOW		3
#define RED			4
#define BROWN		5
#define MAGENTA		6
#define CYAN		7

// Struktur Spielstein
#define MAX_DXY		4
typedef struct {
	int8_t  x;
	int8_t  y;
	uint8_t dxy;
	uint8_t grid [4][4];
} brick_t;

// Spielsteine
#define MAX_BRICKS	7

static const brick_t default_bricks[MAX_BRICKS] = {
	// O
	{
		4, 0, 2,
		{
		{YELLOW, YELLOW, BLANK, BLANK},
		{YELLOW, YELLOW, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// J
	{
		4, 0, 3,
		{
		{BLUE, BLANK, BLANK, BLANK},
		{BLUE, BLUE, BLUE, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// L
	{
		4, 0, 3,
		{
		{BLANK, BLANK, BROWN, BLANK},
		{BROWN, BROWN, BROWN, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// S
	{
		4, 0, 3,
		{
		{BLANK, GREEN, GREEN, BLANK},
		{GREEN, GREEN, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// Z
	{
		4, 0, 3,
		{
		{RED, RED, BLANK, BLANK},
		{BLANK, RED, RED, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// T
	{
		4, 0, 3,
		{
		{MAGENTA, MAGENTA, MAGENTA, BLANK},
		{BLANK, MAGENTA, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	},
	// I
	{
		3, -1, 4,
		{
		{BLANK, BLANK, BLANK, BLANK},
		{CYAN, CYAN, CYAN, CYAN},
		{BLANK, BLANK, BLANK, BLANK},
		{BLANK, BLANK, BLANK, BLANK}
		}
	}
};

// Definition Spielfeld
typedef struct {
	uint8_t grid[GRID_DX][GRID_DY];
} grid_t;


// Spielstaende
typedef struct {
	uint8_t  level;
	uint16_t bricks;
	uint16_t lines;
	uint16_t points;
	uint32_t game_time; // in 100tel Sekunden...
	uint8_t  is_pause;
	uint8_t  game_over;
} score_t;


// Tastendefinitionen
#define KEY_UNKNOWN			0
#define KEY_QUIT_GAME		1
#define KEY_PAUSE_GAME		2
#define KEY_NEW_GAME		3
#define KEY_BRICK_DROP		4
#define KEY_BRICK_ROTATE	5
#define KEY_BRICK_LEFT		6
#define KEY_BRICK_RIGHT		7

// Forwards
void     timer_handler(void);
brick_t  generate_new_brick(void);
void     copy_brick2grid(brick_t *b, grid_t *g);
uint8_t  collision(brick_t *b, grid_t *g);
uint8_t  delete_full_lines(grid_t *g);
void     compute_level(score_t *s);
uint16_t compute_iteration_time(score_t *s);

#endif
