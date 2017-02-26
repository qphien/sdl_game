#include "Game.h"
#include <iostream>
//init sdl, sdl_image, sdl_ttf, sdl_mixer
std::default_random_engine Game::e(unsigned(time(0)));
std::uniform_int_distribution<int> Game::u(0, 23);
Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
	if (TTF_Init() == -1)
		std::cout << "TTF Init Error: " << TTF_GetError() << std::endl;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) != 0)
		std::cout << "Mix Open Audio Error: " << Mix_GetError() << std::endl;
}
//loading picture, ttf, audio
SDL_Texture* Game::Load_Img_Texture(const std::string &img_path)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, img_path.c_str());
	if (!texture)
		std::cout << "Create Texture Error: " << SDL_GetError() << std::endl;
	return texture;
}
//loading picture, ttf, audio
void Game::Init_Game()
{
	//Create Window
	win = SDL_CreateWindow("MineSweper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		480, 480+40, SDL_WINDOW_SHOWN);
	if (!win)
		std::cout << "Create Window Error: " << SDL_GetError() << std::endl;
	//Create Renderer
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!ren)
		std::cout << "Create Renderer Error: " << SDL_GetError() << std::endl;
	//load image
	get_file_location();
	for (const auto &s : img_path)
		img_texture_v.push_back(Load_Img_Texture(s));
	//load ttf
	TTF_Font *font = TTF_OpenFont("./load_file/abg.ttf", 20);
	if (!font)
		std::cout << "Open TTF Error: " << TTF_GetError() << std::endl;
	SDL_Color color = { 255, 255, 255, 255 };
	std::vector<SDL_Color> color_v = { {0, 0, 0, 255}, {255, 0, 0, 255}, {0, 255, 0,255 },
	{0, 0, 255, 255}, {0, 255, 255, 255 }, {255, 0, 255, 255}, {255, 255, 0, 255}, {255, 128, 255, 255 } };
	std::vector<std::string> ttf_v = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	SDL_Surface *ttf_surface;
	SDL_Texture *ttf_texture;
	for (int i = 0; i != 10; ++i)
	{
		//load ttf for bomb
		if (i != 0 && i < 9)
		{
			ttf_surface = TTF_RenderText_Blended(font, ttf_v[i].c_str(), color_v[i - 1]);
			ttf_texture = SDL_CreateTextureFromSurface(ren, ttf_surface);
			ttf_texture_v.push_back(ttf_texture);
			SDL_FreeSurface(ttf_surface);
		}
		//load ttf for bomb number
		ttf_surface = TTF_RenderText_Blended(font, ttf_v[i].c_str(), color);
		ttf_texture = SDL_CreateTextureFromSurface(ren, ttf_surface);
		ttf_texture_bomb_v.push_back(ttf_texture);
		SDL_FreeSurface(ttf_surface);
	}
	//load music
	music = Mix_LoadMUS("./load_file/StartSnd.wav");
	if (!music)
		std::cout << "Load Music Error: " << std::endl;
}
//draw game picture
void Game::display_bomb_photo()
{
	SDL_Rect dst;
	dst.x = 40;
	dst.y = 485;
	SDL_QueryTexture(img_texture_v[5], 0, 0, &dst.w, &dst.h);
	SDL_RenderCopy(ren, img_texture_v[5], 0, &dst);
	//SDL_RenderPresent(ren);
}
void Game::display_total_bomb()
{
	SDL_Rect dst1, dst2;
	dst1.x = 65;
	dst1.y = 485;
	dst2.x = 85;
	dst2.y = 485;
	dst1.w = dst1.h = dst2.w = dst2.h = 20;
	int decade = total_bomb % 10;
	int unit = total_bomb / 10;
	SDL_RenderCopy(ren, img_texture_v[7], 0, &dst1);
	SDL_RenderCopy(ren, img_texture_v[7], 0, &dst2);
	SDL_RenderCopy(ren, ttf_texture_bomb_v[unit], 0, &dst1);
	SDL_RenderCopy(ren, ttf_texture_bomb_v[decade], 0, &dst2);
	//SDL_RenderPresent(ren);
}
void Game::Render_Map()
{
	SDL_Texture *Mine_Sweeper_Block = img_texture_v[0];
	SDL_Texture *background = img_texture_v[7];
	SDL_RenderClear(ren);
	SDL_Rect dst;
	SDL_QueryTexture(Mine_Sweeper_Block, 0, 0, &dst.w, &dst.h);
	for (int i = 0; i != 480; i += 20)
	{
		for (int j = 0; j != 520; j += 20)
		{
			dst.x = i;
			dst.y = j;
			if (j < 480)
				SDL_RenderCopy(ren, Mine_Sweeper_Block, 0, &dst);
			else
				SDL_RenderCopy(ren, background, 0, &dst);
		}
	}
	//SDL_RenderPresent(ren);
}
//play start sound
void Game::play_sound()
{
	//play sound once
	if (Mix_PlayMusic(music, 0) != 0)
		std::cout << "Play Music Error: " << Mix_GetError() << std::endl;
	if (Mix_PlayingMusic() == 0)
		std::cout << "Not Play Music: " << Mix_GetError() << std::endl;
	SDL_Delay(1000);
}
//destroy sdl, sdl_img, sdl_ttf, sdl_mixer
void Game::destroy_sdl()
{
	for (auto img_texture : img_texture_v)
		SDL_DestroyTexture(img_texture);
	for (auto ttf_texture : ttf_texture_bomb_v)
		SDL_DestroyTexture(ttf_texture);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	IMG_Quit();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}
//destroy sdl, sdl_img, sdl_ttf, sdl_mixer
Game::~Game()
{
	destroy_sdl();
}
//add prefix to file location
void Game::get_file_location()
{
	for (auto &s : img_path)
		s = "./load_file/" + s;
}
//get random bomb coordinate
std::pair<int, int> Game::get_bomb_coordi()
{
	return { u(e) * 20, u(e) * 20 };
}
void Game::get_information()
{
	int bomb_adjacent_x, bomb_adjacent_y;
	int flag;
	for (int i = 0; i != 10; ++i)
		bomb_coordi.push_back(get_bomb_coordi());
	for (const auto &bomb : bomb_coordi)
	{
		for (int i = -1; i != 2; ++i)
		{
			for (int j = -1; j != 2; ++j)
			{
				flag = 0;
				if (i != 0 && j != 0)
				{
					bomb_adjacent_x = bomb.first + i * 20;
					bomb_adjacent_y = bomb.second + j * 20;
					//determine whether out of space 
					if (bomb_adjacent_x < 0 || bomb_adjacent_y > 460 || bomb_adjacent_y < 0 || bomb_adjacent_y > 460)
						continue;
					for (const auto &bomb2 : bomb_coordi)          //determine whether adjacnet point is bomb 
					{
						if (bomb_adjacent_x == bomb2.first && bomb_adjacent_y == bomb2.second)
						{
							flag = 1;
							break;
						}
					}
					if (flag)
						continue;
					++bomb_number_v[{bomb_adjacent_x, bomb_adjacent_y}];
				}
			}
		}
	}

}
//display the block you click
bool Game::display(int x, int y)
{
	SDL_Rect dst;
	dst.w = dst.h = 20;
	//click the bomb block
	for (const auto &bomb : bomb_coordi)
	{
		if (bomb.first == x && bomb.second == y)
		{
			for (const auto &bomb1 : bomb_coordi)    //display all bombs;
			{
				dst.x = bomb1.first;
				dst.y = bomb1.second;
				SDL_RenderCopy(ren, img_texture_v[4], 0, &dst);
			}
			return true;
		}
	}
	for (const auto &bomb_number : bomb_number_v)        //display bomb number
	{
		if (bomb_number.first.first == x && bomb_number.first.second == y)
		{
			dst.x = x;
			dst.y = y;
			SDL_RenderCopy(ren, ttf_texture_v[bomb_number.second - 1], 0, &dst);
			return false;
		}
		
	}
	
	std::queue<std::pair<int ,int>> que;
	std::map<std::pair<int, int>, bool> flag_v; //determine whether the block has been accessed
	bool flag;
	bool flag_2;
	int temp_x, temp_y;
	int adjacent_x, adjacent_y;
	que.push({ x, y });
	flag_v[{ x, y }] = true;
	while (!que.empty())    //problem: do not push
	{
		temp_x = que.front().first;
		temp_y = que.front().second;
		que.pop();
		dst.x = temp_x;
		dst.y = temp_y;
		flag_2 = false;
		for (const auto &bomb : bomb_number_v)
		{
			if (temp_x == bomb.first.first && temp_y == bomb.first.second)
			{
				flag_2 = true;
				SDL_RenderCopy(ren, img_texture_v[1], 0, &dst);
				SDL_RenderCopy(ren, ttf_texture_v[bomb.second - 1], 0, &dst);
			}
		}
		if (!flag_2)
		{
			SDL_RenderCopy(ren, img_texture_v[1], 0, &dst);
			//SDL_RenderPresent(ren);
			//SDL_Delay(1000);
			for (int i = -1; i != 2; ++i)
			{
				adjacent_x = temp_x + i * 20;
				if (adjacent_x < 0 || adjacent_x > 460)
					continue;
				for (int j = -1; j != 2; ++j)
				{
					flag = false;
					adjacent_y = temp_y + j * 20;
					if (adjacent_y < 0 || adjacent_y > 460)
						continue;
					if (flag_v[{adjacent_x, adjacent_y}])
						continue;
					//dst.x = adjacent_x;
					//dst.y = adjacent_y;
					for (const auto &bomb : bomb_coordi)
					{
						if (adjacent_x == bomb.first && adjacent_y == bomb.second)
						{
							flag = true;
							break;
						}
					}
					if (flag)
						continue;
					que.push({ adjacent_x, adjacent_y });
					flag_v[{adjacent_x, adjacent_y}] = true;
					block_been_clicked.push_back({ adjacent_x, adjacent_y });
				}
			}
		}
	}
	return false;

}
void Game::game_start()
{
	Init_Game();
	get_information();
	Render_Map();
	display_bomb_photo();
	display_total_bomb();
	SDL_RenderPresent(ren);
	play_sound();

	SDL_Event e;
	bool quit = false;
	bool flag;
	bool flag2;
	SDL_Rect dst, last_dst;
	dst.w = dst.h = 20;
	last_dst.x = last_dst.y = 600;
	last_dst.w = last_dst.h = 20;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_MOUSEMOTION)
			{
				flag = true;
				flag2 = true;
				if (e.motion.x < 480 && e.motion.y < 480)
				{
					dst.x = e.motion.x / 20 * 20;
					dst.y = e.motion.y / 20 * 20;
					for (const auto &block : block_been_clicked)
					{
						if (block.first == dst.x && block.second == dst.y)
						{
							flag = false;
							break;
						}
					}
					if (flag)
					{
						for (const auto &block : block_been_clicked)
						{
							if (block.first == last_dst.x && block.second == last_dst.y)
							{
								flag2 = false;
								break;
							}
						}
						if (!(last_dst.x == dst.x && last_dst.y == dst.y) && flag2 == true)
							SDL_RenderCopy(ren, img_texture_v[0], 0, &last_dst);
						last_dst.x = dst.x;
						last_dst.y = dst.y;
						SDL_RenderCopy(ren, img_texture_v[6], 0, &dst);
					}
				}
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					SDL_RenderCopy(ren, img_texture_v[1], 0, &dst);
					break;
				case SDL_BUTTON_RIGHT:
					bool flag = false;
					if (flag_information.empty())
						flag = true;
					for (auto &infor : flag_information)
					{
						if (infor.x == dst.x && infor.y == dst.y)
						{
							infor.click_number = (infor.click_number + 1) % 3;
							std::cout << infor.click_number << std::endl;
							switch (infor.click_number)
							{
							case 0:
								SDL_RenderCopy(ren, img_texture_v[2], 0, &dst);
								--total_bomb;
								display_total_bomb();
								break;
							case 1:
								SDL_RenderCopy(ren, img_texture_v[3], 0, &dst);
								++total_bomb;
								display_total_bomb();
								break;
							case 2:
								SDL_RenderCopy(ren, img_texture_v[0], 0, &dst);
								break;
							}
							break;
						}
						if (infor == flag_information.back())
							flag = true;
					}
					if (flag)
					{
						I_F temp;
						temp.x = dst.x;
						temp.y = dst.y;
						++temp.click_number;
						flag_information.push_back(temp);
						--total_bomb;
						display_total_bomb();
						SDL_RenderCopy(ren, img_texture_v[2], 0, &dst);
					}
				}
								
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				block_been_clicked.push_back({ dst.x, dst.y });
				if (e.button.button == SDL_BUTTON_LEFT)
				{
					quit = display(dst.x, dst.y);
					if (quit == true)
					{
						SDL_RenderPresent(ren);
						SDL_Delay(5000);
						//SDL_RenderCopy(ren, img_texture_v.back(), 0, 0);
					}
				}
			}
		}
		SDL_RenderPresent(ren);
	}
	//destroy_sdl();
	


}