/*
  
       Tetris (Ein-/Ausgaben)
  ================================
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


#include "tetris_io.h"


static uint16_t wii_controller_typ;


const uint16_t tetris_tft_colors[] =
{
	TFT_BLACK,    // black
    TFT_GREEN,    // green
    TFT_BLUE,     // blue
    TFT_YELLOW,   // yellow
    TFT_RED,      // red
    TFT_BROWN,    // brown
    TFT_MAGENTA,  // magenta
    TFT_CYAN      // lightblue/cyan
};

// ********************************************
void tft_create_game_screen(void)
{
	uint8_t y = SCORE_Y;
	
	fillScreen(TFT_BLACK);
	// Ego befriedigen...
	setTextColor(TFT_GRAY, TFT_BLACK);
	print_xy(0, 152, "U.B.; 2020");

	// Spielfeld zeichnen...
	drawFastVLine(GRID_X-1, GRID_Y, GRID_DY*BRICK_DY+1, TFT_WHITE);
	drawFastVLine(GRID_X+GRID_DX*BRICK_DX, GRID_Y, GRID_DY*BRICK_DY+1, TFT_WHITE);
	drawFastHLine(GRID_X-1, GRID_Y+GRID_DY*BRICK_DY, GRID_DX*BRICK_DX+2, TFT_WHITE);

	setTextColor(TFT_BLUE, TFT_BLACK);
	//print_xy(SCORE_X, y, "Next");
    y=y+37;
	print_xy(SCORE_X, y, "Level");
    y=y+21;
	print_xy(SCORE_X, y, "Bricks");
    y=y+21;
	print_xy(SCORE_X, y, "Lines");
    y=y+21;
	print_xy(SCORE_X, y, "Points");
    y=y+21;
	print_xy(SCORE_X, y, "Time [s]");
	
}

// ********************************************
void tft_destroy_game_screen(void)
{
	// ...
}

// ********************************************
void tft_draw_score(score_t *s)
{
	char buf[10];
    
    setTextColor(TFT_WHITE, TFT_BLACK);

	//...Zeit liegt in 100tel Sekunden vor
	sprintf(buf, "%u,%02u", (unsigned int)(s->game_time/100), (unsigned int)(s->game_time%100));
	print_xy(SCORE_X+6, SCORE_Y+130, buf);

	// Level
	sprintf(buf, "%u", s->level);
	print_xy(SCORE_X+6, SCORE_Y+46, buf);

	// Anzahl versenkte Spielsteine
	sprintf(buf, "%u", s->bricks);
	print_xy(SCORE_X+6, SCORE_Y+67, buf);

	// Anzahl abgeraumte Zeilen
	sprintf(buf, "%u", s->lines);
	print_xy(SCORE_X+6, SCORE_Y+88, buf);

	// Punkte
	sprintf(buf, "%u", s->points);
	print_xy(SCORE_X+6, SCORE_Y+109, buf);

	// Ausgabe Pause bzw. Spielende
	if (s->is_pause||s->game_over) {
		setTextColor(TFT_WHITE, TFT_RED);
		if (s->is_pause) {
			print_xy(0, 0, "Play pause!");
		} else {
			print_xy(0, 0, "Game over!");
		}
	} else {
		setTextColor(TFT_WHITE, TFT_BLACK);
		print_xy(0, 0, "           ");		
	}
}

// ********************************************
void tft_clear_brick(brick_t *b) 
{
	uint8_t i, j;
	for (i=0; i < b->dxy; i++) {
		for (j=0; j < b->dxy; j++) {
			if (b->grid[i][j]) 
				//mvaddch((b->y+GRID_Y+i), (b->x+GRID_X+j), ' '); 
				fillRect(GRID_X+b->x*BRICK_DX+j*BRICK_DX, GRID_Y+b->y*BRICK_DY+i*BRICK_DY, BRICK_DX, BRICK_DY, tetris_tft_colors[0]);
		}	
	}
}

// ********************************************
void tft_draw_brick(brick_t *b) 
{
	uint8_t i, j;
	for (i=0; i < b->dxy; i++) {
		for (j=0; j < b->dxy; j++) {
			if (b->grid[i][j]) 
				fillRect(GRID_X+b->x*BRICK_DX+j*BRICK_DX, GRID_Y+b->y*BRICK_DY+i*BRICK_DY, BRICK_DX, BRICK_DY, tetris_tft_colors[b->grid[i][j]]);
		}	
	}
}

// ********************************************
void tft_draw_next_brick(brick_t *b)
{
	uint8_t i, j;
	for (i=0; i<MAX_DXY; i++) {
		for (j=0; j<MAX_DXY; j++) {
				fillRect(SCORE_X+j*BRICK_DX, SCORE_Y+i*BRICK_DY, BRICK_DX, BRICK_DY, tetris_tft_colors[b->grid[i][j]]);
		}	
	}
}

// ********************************************
void tft_draw_grid(grid_t *g) 
{
	uint8_t i, j;
	fillRect(GRID_X, GRID_Y, GRID_DX*BRICK_DX, GRID_DY*BRICK_DY, tetris_tft_colors[0]);
	for (i=0; i<GRID_DX; i++) {
		for (j=0; j<GRID_DY; j++) {
			if (g->grid[i][j])
				fillRect(GRID_X+i*BRICK_DX, GRID_Y+j*BRICK_DY, BRICK_DX, BRICK_DY, tetris_tft_colors[g->grid[i][j]]);
		}	
	}
}

// ********************************************
void input_init(void)
{
	i2c_init();
	wii_controller_typ = wii_controller_init();	
}


// ********************************************
uint8_t wii_nunchuck_get_key(void)
{
	wii_nunchuck_controls_t c;
	uint8_t ret=KEY_UNKNOWN;
	
	c = wii_nunchuck_get_data();
	// ...ausser KEY_BRICK_LEFT/_RIGHT, jeweils 
	// mit "Tastenwiederholsperre"
	if (c.button_xl) ret = KEY_BRICK_LEFT;
	else if (c.button_xr) ret =  KEY_BRICK_RIGHT;
		 else if (c.button_yd && !(c.button_yd_repeat)) ret = KEY_BRICK_DROP;
		 	  else if (c.button_yu && !(c.button_yu_repeat)) ret = KEY_PAUSE_GAME;
	 		  	   else if (c.button_z && !(c.button_z_repeat)) ret = KEY_BRICK_ROTATE;
	 	  		   	    else if (c.button_c && !(c.button_c_repeat)) ret = KEY_NEW_GAME;	
	return ret;
}

// ********************************************
uint8_t wii_classic_get_key(void)
{
	wii_classic_controls_t c;
	uint8_t ret=KEY_UNKNOWN;
	
	c = wii_classic_get_data();
	// ...Tetris-Tasten
	if (c.button_bdl) ret = KEY_BRICK_LEFT;
	else if (c.button_bdr) ret =  KEY_BRICK_RIGHT;
	// ...der Rest der Tasten mit "Tastenwiederholsperre"
    else if (c.button_bdd && !(c.button_bdd_repeat)) ret = KEY_BRICK_DROP;
 	else if (c.button_bh && !(c.button_bh_repeat)) ret = KEY_PAUSE_GAME;
  	else if (c.button_bx && !(c.button_bx_repeat)) ret = KEY_BRICK_ROTATE;
   	else if (c.button_bplus && !(c.button_bplus_repeat)) ret = KEY_NEW_GAME;	
   	
	return ret;
}

// ********************************************
uint8_t wii_controller_tetris_get_key(void)
{
	switch (wii_controller_typ) {
		case WII_CONTROLLER_NUNCHUCK:
			return wii_nunchuck_get_key();
			break;
		case WII_CONTROLLER_CLASSIC:
			return wii_classic_get_key();
			break;
		default:
			return KEY_UNKNOWN;
			break;
	}
}
