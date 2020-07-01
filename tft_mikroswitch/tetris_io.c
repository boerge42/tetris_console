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
	// Input (PC0...PC5)
	SWITCH_DDRX = 0xc0;
	// Pull-UPs (PC0...PC5)
	SWITCH_PORT = 0x3f;
}

// ********************************************
uint8_t mikroswitch_get_key(void)
{
	uint8_t ret = KEY_UNKNOWN;
	static uint8_t not_repeat = 0;
	
	// Taste gedrueckt (Low-aktiv...)?
	if ((SWITCH_PINX & 0x3f) == 0x3f) {
		// nein, dann nur Wiederholsperre zuruecksetzen
		not_repeat = 0;
	} else {
		// nur dann, wenn mindestens beim vorhergehenden Aufruf der 
		// Routine erkannt wurde, dass keine Taste gedrueckt ist
		if (!not_repeat) {
			// welche Taste (Low-aktiv)
			if (!(SWITCH_PINX & (1<<SWITCH_PIN0))) {
				ret = KEY_PAUSE_GAME;
			} else 
			if (!(SWITCH_PINX & (1<<SWITCH_PIN1))) {
				ret = KEY_NEW_GAME;
			} else 
			if (!(SWITCH_PINX & (1<<SWITCH_PIN2))) {
				ret = KEY_BRICK_LEFT;
			} else 
			if (!(SWITCH_PINX & (1<<SWITCH_PIN3))) {
				ret = KEY_BRICK_RIGHT;
			} else 
			if (!(SWITCH_PINX & (1<<SWITCH_PIN4))) {
				ret = KEY_BRICK_ROTATE;
			} else 
			if (!(SWITCH_PINX & (1<<SWITCH_PIN5))) {
				ret = KEY_BRICK_DROP;
			}	
			// Wiederholsperre setzen
			not_repeat = 1;
		}
	}
	return ret;
}
