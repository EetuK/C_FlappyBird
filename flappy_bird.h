
#ifndef FLAPPY_BIRD_H_
#define FLAPPY_BIRD_H_

#define WALL1 0
#define WALL2 1
#define WALL3 2
#define WALL1_POSITION 40
#define WALL2_POSITION 80
#define WALL3_POSITION 120
#define	NRO_WALLS 3
#define BOX_HOR_POS 10

struct Player{
	float verticalPosition;
	float horizontalPosition;
	float fall_speed;
	float flap;
	float verticalSpeed;
	float gravity;
	volatile int score;
};
struct Wall{
	uint8_t hole_min_size;
	uint8_t speed;
	int position[3];
	uint8_t	hole_size[3];
	uint8_t hole_pos[3];
	uint8_t hor_pos_change;
};


void game_score_difficulty (struct Player *player,struct Wall *wall);
void reset_game_values(struct Player *player,struct Wall *wall);
uint8_t random_between (uint8_t value_min, uint8_t value_max);
void draw_player(struct Player *player);
void draw_walls(struct Wall *wall);
bool alive(struct Player *player,struct Wall *wall);
void jump(struct Player *player, bool *button);
void auto_flap(struct Player *player, struct Wall *wall);
bool autoflap_rules(struct Player **player, struct Wall **wall);


#endif /* FLAPPY_BIRD_H_ */