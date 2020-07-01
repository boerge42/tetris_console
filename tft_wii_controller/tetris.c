/*
  
          AVR-Tetris
  ===========================
     Uwe Berger; 2016, 2020
 
 
 Interessante Links zum Thema Tetris:
 ------------------------------------
 --> http://www.colinfahey.com/tetris/tetris.html
 
 
  ---------
  Have fun! 
 
 
 ToDo:
 -----
 --> Highscore
 
 *----------------------------------------------------------------------
 * Copyright (c) 2016 Uwe Berger - bergeruw(at)gmx.net
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

#include <avr/interrupt.h>
#include <avr/io.h>

#include "tetris.h"
#include "tetris_io.h"
#include "random.h"

// Timer...
volatile uint8_t key_tick  = 0;
volatile uint8_t sys_tick  = 0;

// Tetris...
static brick_t brick, next_brick;
static grid_t grid;
static score_t score = {1, 0, 0, 0, 0};
static uint8_t drop_key_pressed=0;

// **********************************************************
void timer_init(void)
{
    // Timer0 --> key_tick (10ms)
    // ...Timer Mode CTC
    TCCR0A |= (1<<WGM01);
    // ...Vorteiler 1024
    TCCR0B |= (1<<CS02) | (1<<CS00);
    // ...10ms -> ((F_CPU/1024)*0,01)-1
    OCR0A = 155;
    // ...ISR COMPA_vect
    TIMSK0 |= (1<<OCIE0A);    
	// Timer1 --> sys_tick (variable)
    // ...ISR COMPA_vect
    TIMSK1 |= (1<<OCIE1A);
	// Interrupts einschalten
    sei();
}

// **********************************************************
void timer_start(uint16_t mSec)
{

	cli();
    TCCR1B = 0;
    // -> ((F_CPU/256)*mSec/1000)-1
    OCR1A = (F_CPU/256*mSec/1000)-1;
    // Counter zuruecksetzen
    TCNT1 = 0;
	// ...Vorteiler 256
    TCCR1B |= (1<<WGM12) | (1<<CS12);
    sei();
}

// **********************************************************
void timer_stop(void)
{
    TCCR1B = 0;
}

// ********************************************
void timer_handler(void) 
{
	brick_t newbrick;
	static uint8_t free_iterations=0;
	// Spielzeit
	if (!drop_key_pressed) 
		score.game_time = score.game_time + compute_iteration_time(&score)/10;
	draw_score(&score);
	// Stein eine Zeile runter?
	newbrick=brick;
	newbrick.y++;
	if (!collision(&newbrick, &grid)) {
			// ...ja, passt hin
			clear_brick(&brick);
			draw_brick(&newbrick);
			brick=newbrick;
			if (!drop_key_pressed) free_iterations++;											
	} else {
		// ...nein, Endlage erreicht!
		timer_stop();
		// Spielstein in Spielfeld uebernehmen
		copy_brick2grid(&brick, &grid);
		// Spielfeld auf vollständige Zeilen pruefen, ausgeben, Spielstand
		score.lines = score.lines + delete_full_lines(&grid);
		draw_grid(&grid);
		// Spielstand berechnen/ausgeben, Schalter initialisieren
		score.points=score.points+((21+(3*score.level))-free_iterations);
		compute_level(&score);
		draw_score(&score);
		free_iterations=0;
		drop_key_pressed=0;
		// neuen Spielstein generieren/anzeigen
		brick=next_brick;
		score.bricks++;
		next_brick=generate_new_brick();
		draw_next_brick(&next_brick);	
		if (collision(&brick, &grid)) {
			score.game_over=1;
			draw_score(&score);
		} else {
			draw_brick(&brick);	
			// ...und Timer wieder starten
			timer_start(compute_iteration_time(&score));
		}
	}
}

// ********************************************
void game_init(void)
{
	uint8_t i, j;
	timer_stop();
	// Spielstaende nullen
	score.level=1;
	score.bricks=0;
	score.lines=0;
	score.points=0;
	score.game_time=0;
	score.is_pause=0;
	score.game_over=0;
	// Spielfeld initialisieren
	for (i=0; i<GRID_DX; i++) {
		for (j=0; j<GRID_DY; j++) {
			grid.grid[i][j]=BLANK;
		}	
	}
	// Flags initialisieren
	drop_key_pressed=0;
	// Spielfeld/Spielstand ausgeben
	draw_grid(&grid);
	draw_score(&score);
	// einen Spielstein generieren
	brick=generate_new_brick();
	score.bricks++;
	next_brick=generate_new_brick();
	draw_next_brick(&next_brick);
	draw_brick(&brick);
	// Timer starten
	timer_start(compute_iteration_time(&score));
}

// ********************************************
uint16_t compute_iteration_time(score_t *s) 
{
	return (500-(s->level-1)*50);	
}

// ********************************************
void compute_level(score_t *s)
{
	if (s->lines <= 0) {
		s->level = 1;
	} else if ((s->lines >= 1) && (s->lines <= 90)) {
				s->level = 1 + ((s->lines - 1) / 10);
			} else if (s->lines >= 91) {
  						s->level = 10;
					}	
}


// ********************************************
uint8_t delete_full_lines(grid_t *g)
{
	uint8_t count=0;
	uint8_t full;
	uint8_t i, j; 
	int8_t k;
	
	for (j=0; j<GRID_DY; j++) {
		full=1;
		for (i=0; i<GRID_DX; i++) { 
			if (g->grid[i][j]==BLANK) full=0;
		}
		// volle Zeile gefunden
		if (full) {
			count++;
			// drueber liegende Zeilen nach unten verschieben
			for (k=j; k>-1; k--) {
				for (i=0; i<GRID_DX; i++) {
					if (k>0) {
						g->grid[i][k]=g->grid[i][k-1];
					} else {
						g->grid[i][k]=BLANK;
					}
				}
			}
		}
	}
	return count;
}

// ********************************************
brick_t generate_new_brick(void) 
{
	return default_bricks[generate_random_number()];
}

// ********************************************
void copy_brick2grid(brick_t *b, grid_t *g)
{
	uint8_t i, j;
	for (i=0; i < b->dxy; i++) {
		for (j=0; j < b->dxy; j++) {
			// nur uebernehmen, wenn Spielsteinfeld belegt
			if (b->grid[i][j]!=BLANK) 
				g->grid[(j+b->x)][(i+b->y)] = b->grid[i][j];
		}	
	}
}



// ********************************************
brick_t rotate_brick_cw(brick_t b)
{
	brick_t new_b=b;
	uint8_t i, j;
	for (i=0; i<b.dxy; i++) {
		for (j=0; j<b.dxy; j++) {
			new_b.grid[i][j]=b.grid[b.dxy-j-1][i];
		}	
	}
	return new_b;	
}


// ********************************************
uint8_t collision(brick_t *b, grid_t *g)
{
	uint8_t i, j;
	// alle Positionen des Spielsteines pruefen
	for (i=0; i < b->dxy; i++) {
		for (j=0; j < b->dxy; j++) {
			if (b->grid[i][j]!=BLANK) {
				// ...linke Seite
				if ((b->x+j)<0) return 1;
				// ...rechte Seite ((b->x+b->dxy-(b->dxy-j-1))>GRID_DX)
				if ((b->x+j+1)>GRID_DX) return 1;
				// ...oben
				if ((b->y+i)<0) return 1;
				// ...unten
				if ((b->y+i+1)>GRID_DY) return 1;
				// ...ein Feld belegt
				if (g->grid[b->x+j][b->y+i]!=BLANK) return 1;
			}
		}
	}
	// theoretisch fehlt noch eine Pruefung, ob bei
	// der Rotation Steine im Weg sind...!
	// Bei Drehung im Uhrzeigersinn:
	// * alter/neuer Ziegel zusammen 
	// * in den dann freien Feldern rechts oben und
	//   links unten darf kein Stein auf dem Spielfeld
	//   sein 
	return 0;
} 

// ********************************************
void tetris_key_handler(void)
{
	brick_t newbrick;

	// Tastensteuerung		
	switch (tetris_get_key()) {
		// neues Spiel
		case KEY_NEW_GAME:
				game_init();
				create_game_screen();
				break;
		// Spielpause
		case KEY_PAUSE_GAME:
				if (score.is_pause) {
					score.is_pause=0;
					timer_start(compute_iteration_time(&score));
				} else {
					score.is_pause=1;
					timer_stop();
				}
				draw_score(&score);
				break;
		// Spielstein drehen
		case KEY_BRICK_ROTATE:
				if (score.is_pause || score.game_over) break;
				newbrick=rotate_brick_cw(brick);
				if (!collision(&newbrick, &grid)) {
					clear_brick(&brick);
					draw_brick(&newbrick);
					brick=newbrick;
				}
				break;
		// Spielstein runterfallen lassen
		case KEY_BRICK_DROP:
				if (score.is_pause || score.game_over) break;
				timer_stop();
				drop_key_pressed=1;
				// Schnelldurchlauf
				timer_start(5);
				break;
		// Spielstein nach links
		case KEY_BRICK_LEFT:
				if (score.is_pause || score.game_over) break;
				newbrick=brick;
				newbrick.x--;
				if (!collision(&newbrick, &grid)) {
					clear_brick(&brick);
					draw_brick(&newbrick);
					brick=newbrick;											
				};
				break;
		// Spielstein nach rechts
		case KEY_BRICK_RIGHT:
				if (score.is_pause || score.game_over) break;
				newbrick=brick;
				newbrick.x++;
				if (!collision(&newbrick, &grid)) {
					clear_brick(&brick);
					draw_brick(&newbrick);
					brick=newbrick;											
				};
				break;
	}
}

// ********************************************
ISR (TIMER0_COMPA_vect)
{
	static uint8_t tick1=0;
	// feuert alle 10ms...
	tick1++;
	// alle 80ms...
	if (tick1/8) {
		key_tick=1;
		tick1=0;
	}
}

// ********************************************
ISR (TIMER1_COMPA_vect)
{
	sys_tick=1;
	//PORTB ^= (1<<PB5);
}


// ********************************************
// ********************************************
// ********************************************
int main(void)
{
	
	DDRB  |=  (1<<PB5);
	PORTB &= ~(1<<PB5);
	
	// Eingabe-Hardware initialisieren
	input_init();
	
	// TFT iniialisieren
	tft_init();
	
	// Initialisierung Zufallsgenerator
	random_init();
	
	// Spiel aufbauen
	create_game_screen();
	
	// Spiel initialisieren
	game_init();
	
	timer_init();
	
	while (1) {
		
		// automatisches Zeugs fuer Tetris...
		if (sys_tick) {
			sys_tick=0;
			timer_handler();
		}
		
		// Tasten...
		if (key_tick) {
			key_tick=0;
			tetris_key_handler();
		}
		
	}
	
	timer_stop();
	destroy_game_screen();
	return 0;

}

