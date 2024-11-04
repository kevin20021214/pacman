#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include <stdio.h>
#include"shared.h"
#include"scene_win.h"
#pragma warning(disable: 4996)
// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4 
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
int game_main_Score = 0;
bool game_over = false;
ALLEGRO_BITMAP* progress;
ALLEGRO_BITMAP* progress2;
/* Internal variables*/
ALLEGRO_TIMER* power_up_timer;
ALLEGRO_TIMER* speed_up_timer;
const int power_up_duration=10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;
static beaneat;
/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);
static int score[6];
static void init(void) {
	game_over = false;
	game_main_Score = 0;
	beaneat = 0;
	// create map
	basic_map = create_map("Assets/map_nthu.txt");//©ñÀÉ®×¦ì¸m
	// [TODO]
	// Create map from .txt file and design your own map !!
	// basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	progress = load_bitmap("Assets/progress.png");
	progress2 = load_bitmap("Assets/progress2.png");
	GAME_TICK = 0;
	render_init_screen();
	power_up_timer = al_create_timer(1.0f);// 1 tick / sec
	speed_up_timer = al_create_timer(1.0f);
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	return ;
}
static void step(void) {
	if (pman->objData.moveCD > pman->speed)
		pman->objData.moveCD -= pman->speed;
	else pman->objData.moveCD = 0;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman,'.');
		beaneat++;
		//printf("%d\n", beaneat);
		game_main_Score++;
		break;
	case'P':
		pacman_eatItem(pman, 'P');
		game_main_Score += 10;
		al_start_timer(power_up_timer);
		for (int i = 0; i < GHOST_NUM; i++)
			ghost_toggle_FLEE(ghosts[i], true);
		break;
	case'G':
		pacman_eatItem(pman, 'G');
		game_main_Score += 20;
		break;
	case'T':
		pacman_eatItem(pman, 'T');
		if (pman->objData.Coord.x == T[0][0] && pman->objData.Coord.y == T[0][1]) {
			pacman_Transport(pman, T[1][0]+1, T[1][1]);
			basic_map->map[T[1][1]][T[1][0]] = ' ';
		}
		else {
			pacman_Transport(pman, T[0][0]+1, T[0][1]);
			basic_map->map[T[0][1]][T[0][0]] = ' ';
		}
		break;
	case'S':
		al_start_timer(speed_up_timer);
		pacman_eatItem(pman, 'S');
		pman->speed = 3;
		break;
	default:
		break;
	}
	
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.

		basic_map->map[Grid_y][Grid_x]=' ';
	
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		
		if (basic_map->beansNum == beaneat) {
			gameWin = true;
			al_rest(1.0);
			break;
		}
		if(!cheat_mode && RecAreaOverlap(getDrawArea(pman->objData, GAME_TICK_CD),getDrawArea(ghosts[i]->objData,GAME_TICK_CD)))
		{
			game_log("collide with ghost\n");
			if (ghosts[i]->status != FLEE) {
				al_rest(1.0);
				pacman_die();
				game_over = true;
				break;
			}
			else {
				pacman_eatItem(pman, 'G');
				ghost_collided(ghosts[i]);
			}
		}
	}
}

static void update(void) {

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		al_start_timer(pman->death_anim_counter);
		long long int ti = al_get_timer_count(pman->death_anim_counter);
		if (ti >( 88 + 16)) {
			game_change_scene(scene_menu_create());
		}
		return;
	}
	if (gameWin) {
		gameWin = false;
		al_start_timer(pman->death_anim_counter);
		long long int ti = al_get_timer_count(pman->death_anim_counter);
		if (ti > 16) {
			game_change_scene(scene_win_create());
		}
		return;
	}
	if (al_get_timer_count(power_up_timer) == power_up_duration) {
		for (int i = 0; i < GHOST_NUM; i++)
			ghost_toggle_FLEE(ghosts[i], false);
		al_stop_timer(power_up_timer);
		al_set_timer_count(power_up_timer, 0);
	}
	if (al_get_timer_count(speed_up_timer) == power_up_duration) {
		pman->speed = 2;
		al_stop_timer(speed_up_timer);
		al_set_timer_count(speed_up_timer, 0);
	}
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}
static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	//	[TODO]
	//	Draw scoreboard, something your may need is sprinf();
	char a[1000];
	char b[1000];
	sprintf(b,"%.f %%",((float)beaneat/basic_map->beansNum)*100);
	sprintf(a, "score: %d", game_main_Score);
	al_draw_text(menuFont,al_map_rgb(255, 255, 0),100, 700,ALLEGRO_ALIGN_CENTER,a);
	al_draw_scaled_bitmap(progress2, 0, 0, 800, 676, 200, 680, 180, 100, 0);
	al_draw_scaled_bitmap(progress, 0, 0, 800*(1-(float)beaneat/basic_map->beansNum), 600, 200, 680, 180 * (1-(float)beaneat / basic_map->beansNum), 100, 0);
	al_draw_text(menuFont, al_map_rgb(255, 255, 0), 450, 720, ALLEGRO_ALIGN_CENTER,b);
	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
	FILE* stxt=fopen("Assets/score.txt", "r");
	for (int i = 0; i < 5; i++) {
		fscanf(stxt, "%d", &score[i]);
	}
		score[5] = game_main_Score;
		for (int i = 4; i >= 0; i--) {
			if (score[i] < score[i + 1]) {
				int tmp = score[i];
				score[i] = score[i + 1];
				score[i+1] = tmp;
			}
		}
		fclose(stxt);
		stxt=fopen("Assets/score.txt","w");
		for (int i = 0; i < 5; i++) {
			fprintf(stxt, "%d\n", score[i]);
		}
		fclose(stxt);
	al_destroy_bitmap(progress);
	al_destroy_bitmap(progress2);
	for (int i = 0; i < GHOST_NUM; i++) {
		free(*(ghosts + i));
	}
	free(ghosts);
	free(pman);
	al_destroy_timer(power_up_timer);
	al_destroy_timer(speed_up_timer);
}

static void on_key_down(int key_code) {
	switch (key_code)
	{
		// [HACKATHON 1-1]	
		// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
		// we provided you a function `pacman_NextMove` to set the pacman's next move direction.
		
		case ALLEGRO_KEY_W:
			pacman_NextMove(pman,UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman,RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else 
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;
	default:
		break;
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
