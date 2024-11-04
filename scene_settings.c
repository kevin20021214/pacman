// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#pragma warning(disable: 4996)
bool bu=false;
bool check = false;
static ALLEGRO_BITMAP* volume = NULL;
static Button volume_down;
static Button volume_muse;
static Button effect_up;
static Button effect_down;
static Button Return;
static ALLEGRO_SAMPLE_ID menuBGM;
static ALLEGRO_FONT* volume_size = NULL;
static ALLEGRO_SAMPLE* Max_volume = NULL;
static ALLEGRO_SAMPLE* setting_music = NULL;
static ALLEGRO_SAMPLE* effect_music = NULL;
float re;
static int score[6];
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.
/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
static void init() {
	FILE* stxt = fopen("Assets/score.txt", "r");
	for (int i = 0; i < 5; i++) {
		fscanf(stxt, "%d", &score[i]);
	}
	fclose(stxt);
	volume = al_load_bitmap("Assets/volume.png");
	Max_volume = load_audio("Assets/Music/volume_max.ogg");
	setting_music = load_audio("Assets/Music/setting_music.ogg");
	effect_music = load_audio("Assets/Music/effect_music.ogg");
	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	volume_muse = button_create(180,400, 80,80, "Assets/muse.png", "Assets/muse2.png");
	volume_down= button_create((music_volume*96)+160, 150, 30, 130, "Assets/volume_down.png", "Assets/volume_down2.png");//改圖
	effect_down = button_create(160, 300, 80, 80, "Assets/effect_down.png", "Assets/effect_down2.png");//改圖
	effect_up = button_create(590, 300, 80, 80, "Assets/volume_up.png", "Assets/volume_up2.png");
	//stop_bgm(menuBGM);
	Return = button_create(680, 0, 80, 80, "Assets/Return.png", "Assets/Return2.png");
	menuBGM = play_bgm(setting_music, music_volume);
	volume_size = load_font("Assets/Minecraft.ttf", 50);
}
static void draw(void ){
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_scaled_bitmap(volume,0,0,40,40,165,220,505,10,1);
	drawButton(volume_down);
	if (check) {
		al_draw_scaled_bitmap(volume_muse.hovered_img,0,0,1725,931,180,400,80,80,0);
	}
	else {
		al_draw_scaled_bitmap(volume_muse.default_img, 0, 0, 1725, 931, 180, 400, 80, 80, 0);
	}
	drawButton(Return);
	char a[1000], b[10000];
	sprintf(a, "VOLUME: %2.f %%", (music_volume/5)*100);
	al_draw_text(volume_size, al_map_rgb(20, 67, 88), 410, 170, ALLEGRO_ALIGN_CENTRE, a);
	drawButton(effect_down);
	drawButton(effect_up);
	char s[100];
	for (int i = 0; i < 5; i++) {
		sprintf(s,"TOP   %d:      %d",i+1,score[i]);
		al_draw_text(volume_size, al_map_rgb(20, 227,220), 480, 420+i*80, ALLEGRO_ALIGN_CENTRE, s);
	}
	sprintf(b, "EFFECT: %2.f %%", (effect_volume / 5 )* 100);
	al_draw_text(volume_size, al_map_rgb(20, 67, 88), 420, 320, ALLEGRO_ALIGN_CENTRE, b);
}
static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_font(volume_size);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below
	al_destroy_bitmap(volume_down.default_img);
	al_destroy_bitmap(volume_muse.default_img);
	al_destroy_bitmap(Return.default_img);
	al_destroy_bitmap(volume_down.hovered_img);
	al_destroy_bitmap(volume_muse.hovered_img);
	al_destroy_bitmap(Return.hovered_img);
	al_destroy_bitmap(effect_down.default_img);
	al_destroy_bitmap(effect_up.default_img);
	al_destroy_bitmap(effect_down.hovered_img);
	al_destroy_bitmap(effect_up.hovered_img);
	al_destroy_bitmap(volume);
}
static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	if (bu == true) {
		volume_down.body.x = mouse_x - volume_down.body.w / 2;
		if (volume_down.body.x < 160) {
			music_volume = 0;
			volume_down.body.x = 160;
		}
		else if (volume_down.body.x> 640) {
			volume_down.body.x = 640;
			music_volume = 5;
		}
		else music_volume = ((mouse_x-160.0f) / 480)*5;
	}
	volume_down.hovered = buttonHover(volume_down, mouse_x, mouse_y);
	volume_muse.hovered = buttonHover(volume_muse, mouse_x, mouse_y);
	Return.hovered = buttonHover(Return, mouse_x, mouse_y);
	effect_down.hovered = buttonHover(effect_down, mouse_x, mouse_y);
	effect_up.hovered = buttonHover(effect_up, mouse_x, mouse_y);
}
static void on_mouse_up() {
	if (bu) {
		stop_bgm(menuBGM);
		volume_down.body.x = (music_volume * 96) + 160;
		menuBGM = play_bgm(setting_music, music_volume);
	}
		bu = false;
}
static void on_mouse_down() {//改功能
	if (volume_down.hovered) {
		if (check) {
			re = 0;
			check = !check;
		}
		bu = true;
	}
	if (volume_muse.hovered) {
		check = !check;
		if (check) {
			volume_down.body.x = 160;
			stop_bgm(menuBGM);
			re = music_volume;
			music_volume = 0;
		}
		else {
			music_volume = re;
			volume_down.body.x = (music_volume * 96) + 160;
			menuBGM=play_bgm(setting_music, music_volume);
		}
	}
	if (effect_down.hovered) {
		if (effect_volume > 0) {
			effect_volume-=0.5;
			stop_bgm(menuBGM);
			menuBGM = play_bgm(effect_music, effect_volume);
			al_rest(0.5);
			stop_bgm(menuBGM);
			menuBGM = play_bgm(setting_music, music_volume);
		}
	}
	if (effect_up.hovered) {
		if (effect_volume < 5) {
			effect_volume+=0.5;
			stop_bgm(menuBGM);
			menuBGM = play_bgm(effect_music, effect_volume);
			al_rest(0.5);
			stop_bgm(menuBGM);
			menuBGM = play_bgm(setting_music, music_volume);
		}
		else if (effect_volume == 5) {
			stop_bgm(menuBGM);
			menuBGM = play_bgm(Max_volume, effect_volume);
			al_rest(1);
			stop_bgm(menuBGM);
			menuBGM = play_bgm(setting_music, effect_volume);
		}
	}
	if (Return.hovered) {
		game_change_scene(scene_menu_create());
	}
//game_change_scene(scene_settings_create());
}
// The only function that is shared across files.
Scene scene_settings_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_mouse_move = &on_mouse_move;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_up = &on_mouse_up;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");

	return scene;
}