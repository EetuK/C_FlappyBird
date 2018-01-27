#include <asf.h>
//#include <string.h>
//#include "main.h"
#include "flappy_bird.h"
#include "ssd1306_i2c.h"
#include "ssd1306_oled.h"


// Counting game score and making game more difficult
void game_score_difficulty(struct Player *player,struct Wall *wall)
{
	bool one_time = false;
	char text_buffer[128];	// temp text buffer
	
	// Whenever wall position is play7er position, score + 1 
	if (wall->position[WALL1] == 8 || wall->position[WALL2] == 8 || wall->position[WALL3] == 8){
		player->score++;
		one_time = true;
	}
	
	
	// PRINTING SCORE BOX
	//FONT_ARIAL;	// change font
	//oled_drawRectangle_fill(110,0,18,10,2);
	//sprintf(text_buffer, "%02d",player->score);
	//oled_printf(false,113,1, text_buffer);
	//oled_drawRectangle(110,0,18,10);

	// As player score grows by 5, make hole minimum size smaller and
	// make the hole vertical range bigger
	if (player->score % 5 == 0 && one_time == true )
	{
		//if (Player.score % 10 == 0)
		wall->hole_min_size--;
		if (wall->hor_pos_change+wall->hole_min_size+10<=64)
		wall->hor_pos_change++;
		one_time = false;
	}
}

// Setting basic values for the came
void reset_game_values(struct Player *player, struct Wall *wall)
{
	// WALLS
	// wall settings
	wall->hole_min_size = 22;	// Minium hole size
	wall->speed = 1;		// Horizontal scrolling speed
	wall->hor_pos_change = 32;	// 
	
	// Set wall startup positions
	wall->position[WALL1] = WALL1_POSITION;
	wall->position[WALL2] = WALL2_POSITION;
	wall->position[WALL3] = WALL3_POSITION;
	
	// Set random hole sizes, except first
	wall->hole_size[WALL1] = wall->hole_min_size + 10;	// max hole size to make start easier
	wall->hole_size[WALL2] = wall->hole_min_size + random_between(0,10);	// Random between 0-10 + wall min size(18)
	wall->hole_size[WALL3] = wall->hole_min_size + random_between(0,10);	// Random between 0-10 + wall min size(18)
	
	// Set random hole positions, except first
	wall->hole_pos[WALL1] = 10;
	wall->hole_pos[WALL2] = random_between(0,22);
	wall->hole_pos[WALL3] = random_between(0,22);
	
	// PLAYER
	player->flap = 3.6;	// flap size value 3.5
	player->gravity = 0.3f;	// gravity value
	player->verticalPosition = 25;	
	player->horizontalPosition = 10;	
	player->fall_speed = 1;	
	player->score = 0;
	player->verticalSpeed = 0;
}

// generate random number
uint8_t random_between (uint8_t value_min, uint8_t value_max)
{
	return rand() % (value_max - value_min) + value_min;
}

// draw "player" to screen buffer
void draw_player(struct Player *player)
{
	oled_drawRectangle_fill(BOX_HOR_POS,player->verticalPosition,3,3,1);
}

// draw walls to screen buffer
void draw_walls(struct Wall *wall)
{
	
	//draw walls
	for (uint8_t i = 0; i<NRO_WALLS; i++)
	{
		//+11 x= wall[i] y=0 width=2, height=randomized hole + range(11), fill = 1
		oled_drawRectangle_fill(wall->position[i],0,2,wall->hole_pos[i],1); 
		oled_drawRectangle_fill(wall->position[i],wall->hole_pos[i]+wall->hole_size[i]+1,2,64-wall->hole_pos[i]-wall->hole_size[i],1);
	}
	// For all the walls: If wall horizontal position reaches 0, move it to 128(opposite side of the screen)
	for (uint8_t j = 0; j<3; j++)
	{
		// -2 = wall is 2 pixel wide
		if (wall->position[j] <= -2)
		{
			wall->hole_size[j] = wall->hole_min_size + random_between(0,10);	// new random hole size
			wall->hole_pos[j] = random_between(0,wall->hor_pos_change);	// new vertical hole place
			wall->position[j] = 128;	// set wall position
		}
	}
	// decrease walls horizontal place, so it moves toward the player
	for (uint8_t s=0; s<NRO_WALLS; s++)
	{
		wall->position[s] -= wall->speed;
	}
}

// Have player hit the wall?
bool alive(struct Player *player,struct Wall *wall)
{
	// check for all the walls
	for (uint8_t i = 0; i < NRO_WALLS; i++)
	{
		// check for whole wall (width=3)
		for (int x=-1; x<3; x++)
		{
			// Is player upper than lowest edge of the upper wall?
			if (wall->position[i] == BOX_HOR_POS+x && player->verticalPosition <= wall->hole_pos[i]) 
			return false;
			// Is player  lower than highest edge of th wall
			if(wall->position[i] == BOX_HOR_POS+x && player->verticalPosition + 3 >= wall->hole_pos[i]+wall->hole_size[i]+1) 
			return false;
		}
	}
	// If player is out of game boundaries, return false
	if (player->verticalPosition >= 1 && player->verticalPosition+3 <= 63)
		return true;
	else
		return false;
}

// Players jump movement
void jump(struct Player *player, bool *button)
{	
	// If S1 pressed
	if (S1 && *button == false)
	{
		// button variable makes sure, that button is pressed
		// only once
		*button = true;
		// add vertical speed for player
		if (player->verticalSpeed<=0)
			player->verticalSpeed-=(player->flap+player->verticalSpeed);
		else
			player->verticalSpeed-=player->flap;
	}
	// making sure that button is pressed only one
	if (*button == true && !(S1))
		*button = false;
	// add gravity
	player->verticalSpeed+=player->gravity;
	// add vertical speed to position
	player->verticalPosition+= player->verticalSpeed;
}



void auto_flap(struct Player *player,struct Wall *wall)
{
	bool bounce = false;
	//oled_clear_no_update();
	uint8_t pieninseina = 128;
	uint8_t pieninseinanumero = 0;
	for (uint8_t i = 0; i<3; i++)
	{
		if (wall->position[i]>10 && wall->position[i] < pieninseina)
		{
			pieninseina = wall->position[i];
		}
	}
	for (uint8_t j= 0; j<3; j++)
	{
		if (wall->position[j] == pieninseina)
		{
			pieninseinanumero = j;
		}
	}
	
	bool asd = true;
	asd = autoflap_rules(&player, &wall); 
	if (player->verticalPosition > wall->hole_pos[pieninseinanumero]+wall->hole_size[pieninseinanumero])
	{
		bounce = true;
	}
	if (asd == false)
	{
		bounce = true;
	}
	
	if (bounce == true)
	{
		if (player->verticalSpeed<=0)
		player->verticalSpeed-=(player->flap+player->verticalSpeed);
		else
		player->verticalSpeed-=player->flap;
		bounce = false;
	}
	player->verticalSpeed+=player->gravity;
	player->verticalPosition+= player->verticalSpeed;
	player->horizontalPosition++;
	//Draw_Walls();
	//Draw_player();
	////FONT_COURIER_NEW;
	////oled_printf(true,18,0, "FLAPPY");
	////oled_printf(true,37,COURIER_R2, "BOX");
	//oled_update();
}

bool autoflap_rules(struct Player **player, struct Wall **wall)
{
	for (uint8_t i = 0; i < NRO_WALLS; i++)
	{
		for (int x=-3; x<6; x++)
		{
			//if (Wall.position[i] == BOX_HOR_POS+x && Player.verticalPosition < Wall.hole_pos[i]) // Onko boxi ylempänä kuin yläseinä?
			//return false;
			if((*wall)->position[i] == BOX_HOR_POS+x && (*player)->verticalPosition + 6 > (*wall)->hole_pos[i]+(*wall)->hole_size[i]) // Onko boxi alempana kuin alaseinä(yläseinä+reikä)?
			return false;
		}
	}
	if ((*player)->verticalPosition >= 10 && (*player)->verticalPosition+3 <= 59)
		return true;
	else
		return false;
}
