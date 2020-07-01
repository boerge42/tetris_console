/*
  
  Tetris (Ein-/Ausgaben; Definitionen)
  ====================================
      Uwe Berger; 2015, 2016, 2020


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

#ifndef _TETRIS_IO_H
#define _TETRIS_IO_H

#include <string.h>
#include <stdio.h>
#include "tetris.h"
#include "uTFT_ST7735.h"
#include "colors.h"
#include "wii_controller.h"

/* mcurses....
// Bildschirmposition Spielfeld
#define GRID_X		5
#define GRID_Y		3
// Bildschirmposition Spielstaende
#define SCORE_X		20
#define SCORE_Y		3
*/

// TFT-Dimensionen
// Bildschirmposition Spielfeld
#define GRID_X		1
#define GRID_Y		9
#define BRICK_DX    7
#define BRICK_DY    7
// Bildschirmposition Spielstaende
#define SCORE_X		80
#define SCORE_Y		9


#define create_game_screen	tft_create_game_screen
#define destroy_game_screen	tft_destroy_game_screen
#define draw_brick			tft_draw_brick
#define clear_brick			tft_clear_brick
#define draw_next_brick		tft_draw_next_brick
#define draw_grid			tft_draw_grid
#define draw_score			tft_draw_score
#define tetris_get_key		wii_controller_tetris_get_key


// Forwards
void input_init(void);
void create_game_screen(void);
void destroy_game_screen(void);	
void draw_brick(brick_t *b);
void clear_brick(brick_t *b);
void draw_next_brick(brick_t *b);
void draw_grid(grid_t *g);
void draw_score(score_t *s);
uint8_t  tetris_get_key(void);

#endif
