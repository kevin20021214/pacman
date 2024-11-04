// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_SAMPLE* PACMAN_EAT_GHOST = NULL;
ALLEGRO_SAMPLE* transport = NULL;
ALLEGRO_SAMPLE* speedup = NULL;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;
bool gameWin = false;
/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
	PACMAN_EAT_GHOST = load_audio("Assets/Music/pacman_eatghost.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
	transport = load_audio("Assets/Music/transportation_music.ogg");
	speedup=load_audio("Assets/Music/speed.ogg");
}

void shared_destroy(void) {
	al_destroy_sample(transport);
	al_destroy_sample(speedup);
	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_EAT_GHOST);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
}