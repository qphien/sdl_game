#ifndef __GAME_H__
#define __GAME_H__
#include "Corresponding_Sdl.h"
#include <vector>
#include <string>
#include <utility>
#include <random>
#include <ctime>
#include <map>
#include <queue>
#include "flag_informaiton.h"
class Game
{
public:
	Game(); //init sdl, sdl_image, sdl_ttf, sdl_mixer
	SDL_Texture* Load_Img_Texture(const std::string &); // load img_texture
	void Init_Game();  //loading picture, ttf, audio
	void Render_Map(); //draw game picture
	void play_sound(); //play game sound
	void game_start();
	void destroy_sdl(); //destroy sdl, sdl_image, sdl_ttf, sdl_mixer;
	void get_file_location();
	~Game(); //destroy sdl, sdl_image, sdl_ttf, sdl_mixer;
private:
	std::pair<int, int> get_bomb_coordi();
	void get_information();   //get bomb coordinate and bomb number
	void display_bomb_photo();
	void display_total_bomb();
	bool display(int, int); //display the block you click;
	static std::default_random_engine e;
	static std::uniform_int_distribution<int> u;
	SDL_Window *win;
	SDL_Renderer *ren;
	std::vector<SDL_Texture*> img_texture_v;
	std::vector<SDL_Texture*> ttf_texture_bomb_v;  //display bomb_for_count
	std::vector<SDL_Texture*> ttf_texture_v;     //display bomb
	std::vector<std::pair<int, int>> bomb_coordi;
	std::vector<std::pair<int, int>> block_been_clicked;
	std::vector<I_F> flag_information;
	std::map<std::pair<int, int>, int> bomb_number_v;
	int total_bomb = 10;
	TTF_Font *font;
	Mix_Music *music;
	std::vector<std::string> img_path = { "MineSweeper_block.png", "MineSweeper_opened_block.png",
		 "MineSweeper_flag.png","MineSweeper_confused_flag.png", "MineSweeper_bomb.png",
		"MineSweeper_bomb_for_count.png", "MineSweeper_mouse.png", "background.png", "game_over.png"};
};
#endif


