#include <asf.h>
#include <string.h>
#include <math.h>
#include "ssd1306_i2c.h"
#include "ssd1306_oled.h"
#include "flappy_bird.h"
#include "main.h"

// Statemachine states
#define START 0
#define GAME 1
#define GAMEOVER 2
#define AUTOPLAY 3


// Prototypes
void State_start(struct Player player,struct Wall wall);
void State_game(struct Player player,struct Wall wall);
void State_game_over(struct Player player,struct Wall wall);
void State_autoplay(struct Player player,struct Wall wall);


// Main
int main (void)
{
	system_init();	
	delay_init();		
	configure_i2c();	// config i2c
	init_oled();		// configure oled screen

	// Variables
	uint8_t state = 0;	// state machine
	bool button = false;	// bool for jump -function
	
	// Structures for player and wall data
	struct Wall wall;
	struct Player player;

	
	
	reset_game_values(&player, &wall);	// Game start values
	
	while (1)
	{
		// START
		if (state == START){
			oled_clear_no_update();		// Clear screen
			auto_flap(&player, &wall);			// Start screen animation (calculate player position)
			draw_player(&player);		// Draw player to screen buffer
			draw_walls(&wall);		// Calc and draw walls to screen buffer
			FONT_COURIER_NEW;	// Set font "Courier New"
			oled_printf(true,18,0, "FLAPPY");	// Print "Flappy Bird to screen"
			oled_printf(true,37,COURIER_R2, "BOX");
			oled_update();		// Update screen
			// If S1 pressed, clear screen, reset values and continue to game
			if (S1)
			{
				oled_clear();
				reset_game_values(&player, &wall);
				state = GAME;
			}
		}
		// GAME
		if (state == GAME){
			oled_clear_no_update();	
			oled_drawRectangle(0,0,128,64);	// Draw game boundaries
			jump(&player, &button);		// If S1 pressed, box jumps (calculating new coords for box)
			draw_player(&player);	// Draw player to screen buffer
			draw_walls(&wall);	// Calc and draw walls to screen buffer
			game_score_difficulty(&player, &wall);	// calculating passed walls and adding more difficulty
			oled_update();		// Update screen
			
			// Have player crashed any walls?
			if (!alive(&player, &wall)) // If alive return false
			{
				oled_clear();	// clear screen
				state = GAMEOVER;	// moving to game over -state
			}
		}
		
		// GAME OVER
		if (state == GAMEOVER)
		{
			char text_buffer[128];	// create temporary text buffer
									// for player score
				oled_clear_no_update();	// clear screen buffer
				FONT_COURIER_NEW;	// change font
				oled_printf(true,25,0, "GAME");	
				oled_printf(true,25,COURIER_R2, "OVER");	// printing "GAME OVER"
				FONT_ARIAL;	// change font
				sprintf(text_buffer,"YOUR SCORE: %d", player.score); // print score to temp buffer
				oled_printf(true,0, ARIAL_R7,text_buffer);	// print player score to screen buffer
				oled_update();	
				
				if (S1)	// New game if S1 pressed
				{
					reset_game_values(&player, &wall);
					state = GAME;
				}			
		}
	}
}
