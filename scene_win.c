#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include"pacman_obj.h"
static ALLEGRO_FONT* win_size = NULL;
/* Internal Variables*/
static ALLEGRO_BITMAP* WinGame = NULL;
static ALLEGRO_SAMPLE_ID winBGM;
static ALLEGRO_SAMPLE_ID winmusic;
static int winTitleW ;
ALLEGRO_SAMPLE* winMusic = NULL;
static int winTitleH ;
ALLEGRO_TIMER* win_time;
// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
 static Button Return;


static void init() {
	win_time = al_create_timer(1.0f/28);
	al_start_timer(win_time);
	win_size = load_font("Assets/Minecraft.ttf", 100); 
	Return = button_create(730, 20, 50, 50, "Assets/Return.png", "Assets/Return2.png");
	WinGame = al_load_bitmap("Assets/win.png");
	winMusic = load_audio("Assets/Music/win_BGM.ogg");
	winBGM = play_bgm(winMusic, music_volume);
}
static void draw() {
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		300,
		680,
		ALLEGRO_ALIGN_CENTER,
		"PRESS\ ENTER\ TO\ RETURN\ GAME\ "
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		750,
		100,
		ALLEGRO_ALIGN_CENTER,
		"MENU"
	);
	al_draw_text(
		win_size,
		al_map_rgb(255, 255, 255),
		220,
		100,
		ALLEGRO_ALIGN_CENTER,
		" YOU\ WIN!"
	); 
	long long ti = al_get_timer_count(win_time);
	if (al_get_timer_count(win_time) == 140) {
		stop_bgm(winBGM);
		stop_bgm(winBGM);
		winMusic = load_audio("Assets/Music/win_music.ogg");
		winBGM = play_bgm(winMusic, music_volume);
	}
	if (al_get_timer_count(win_time) == 280) {
		stop_bgm(winBGM);
		winMusic = load_audio("Assets/Music/win_BGM.ogg");
		winBGM = play_bgm(winMusic, music_volume);
	}
	al_draw_scaled_bitmap(
		WinGame,
		(ti%9) *498, 0,
		498,280,
		130,250,
		534,300,
		0
	);
	 drawButton(Return);
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
		 Return.hovered = buttonHover(Return, mouse_x, mouse_y);
}
static void on_mouse_down() {
	if (Return.hovered)
		game_change_scene(scene_menu_create());
}
static void destroy() {
	stop_bgm(winBGM);
	al_destroy_bitmap(WinGame);
	al_destroy_font(win_size);
	al_destroy_sample(winMusic);
	al_destroy_timer(win_time);
	al_destroy_bitmap(Return.default_img);
	al_destroy_bitmap(Return.hovered_img);
}

static void on_key_down(int keycode) {
	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			game_change_scene(scene_main_create());
			break;
		default:
			break;
	}
}
Scene scene_win_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Win";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	game_log("Win scene created");
	return scene;
}