#include "SDL.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <random>
#include <time.h>
#include <utility>
#include <memory>
#include <deque>
#include <vector>

using namespace std;

enum direction
{
        left, right, up, down
};
struct snake_node
{
        int x = 0, y = 0;
        int state = 0;
        bool operator==(const snake_node &right) const;
};
bool snake_node::operator==(const snake_node &right) const
{
        if (x == right.x && y == right.y && state == right.state)
                return true;
        return false;
}
SDL_Texture* CreateTexture(SDL_Renderer *ren, const string &file)
{
        SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
        if (!texture)
                cout << "Create Texture Error: " << SDL_GetError() << endl;
        return texture;
}
pair<int, int> generate_random_array(const deque<snake_node> &snake_v)
{
        static default_random_engine e(unsigned(time(0)));
        static uniform_int_distribution<int> u(0, 19);
        int i, j;
        bool flag = true;
        while (flag)
        {
                i = u(e) * 20;
                j = u(e) * 20;
                for (const auto &snake : snake_v)
                {
                        if (i == snake.x && j == snake.y)
                                break;
                        if (snake == snake_v.back())
                                flag = false;
                }
                /*for (auto &barrier : barrier_coordi)
                {
                        if (i == barrier.first && j == barrier.second)
                                break;
                        if (barrier == barrier_coordi.back())
                                flag = false;
                }*/
        }
        return { i, j };
}
void Init_map(SDL_Texture *&background, SDL_Texture *&barrier, SDL_Renderer *&ren)
{
        int map_array[][21] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                                        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
                                                  };
        SDL_RenderClear(ren);
        SDL_Rect dest;
        int w, h;
        SDL_QueryTexture(background, 0, 0, &w, &h);
        dest.w = w;
        dest.h = h;
        for (int i = 0; i != 20; ++i)
        {
                for (int j = 0; j != 21; ++j)
                {
                        dest.x = i * 20;
                        dest.y = j * 20;
                        if (!map_array[i][j])
                                SDL_RenderCopy(ren, background, 0, &dest);
                        else
                                SDL_RenderCopy(ren, barrier, 0, &dest);
                }
        }
        SDL_RenderPresent(ren);

}
template<typename T, typename... Args>
void destroy(T *val, Args&&... params)
{
        destroy(val);
        destroy(std::forward<Args>(params)...);
}
template<>
inline void destroy<SDL_Window>(SDL_Window *win)
{
        if (!win)
                return;
        SDL_DestroyWindow(win);
}
template<>
inline void destroy<SDL_Renderer>(SDL_Renderer *ren)
{
        if (!ren)
                return;
        SDL_DestroyRenderer(ren);
}
template<>
inline void destroy<SDL_Texture>(SDL_Texture *texture)
{
        if (!texture)
                return;
        SDL_DestroyTexture(texture);
}
void Init_snake(deque<snake_node> &snake_v)
{
        //shared_ptr<snake_node> snake_head = make_shared<snake_node>();
        snake_node snake_head, snake_body;
        snake_body.x = 3 * 20;
        snake_body.y = 4 * 20;
        snake_body.state = 1;
        snake_head.x = snake_body.x + 20;
        snake_head.y = snake_body.y;
        snake_head.state = 0;
        snake_v.push_back(snake_head);
        snake_v.push_back(snake_body);
}
void move_up(direction &d, deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&background)
{
        SDL_Rect dest;
        dest.w = dest.h = 20;
        dest.x = snake_v.back().x;
        dest.y = snake_v.back().y;
        SDL_RenderCopy(ren, background, 0, &dest);
        if (d != down)
        {
                for (int i = snake_v.size() - 1; i != 0; --i)
                {
                        snake_v[i].x = snake_v[i - 1].x;
                        snake_v[i].y = snake_v[i - 1].y;
                }
                snake_v[0].y -= 20;
                d = up;
        }
}
void move_down(direction &d, deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&background)
{
        SDL_Rect dest;
        dest.w = dest.h = 20;
        dest.x = snake_v.back().x;
        dest.y = snake_v.back().y;
        SDL_RenderCopy(ren, background, 0, &dest);
        if (d != up)
        {
                for (int i = snake_v.size() - 1; i != 0; --i)
                {
                        snake_v[i].x = snake_v[i - 1].x;
                        snake_v[i].y = snake_v[i - 1].y;
                }
                snake_v[0].y += 20;
                d = down;
        }
}
void move_right(direction &d, deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&background)
{
        SDL_Rect dest;
        dest.w = dest.h = 20;
        dest.x = snake_v.back().x;
        dest.y = snake_v.back().y;
        SDL_RenderCopy(ren, background, 0, &dest);
        if (d != direction::left)
        {
                for (int i = snake_v.size() - 1; i != 0; --i)
                {
                        snake_v[i].x = snake_v[i - 1].x;
                        snake_v[i].y = snake_v[i - 1].y;
                }
                snake_v[0].x += 20;
                d = direction::right;
        }
}
void move_left(direction &d, deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&background)
{
        SDL_Rect dest;
        dest.w = dest.h = 20;
        dest.x = snake_v.back().x;
        dest.y = snake_v.back().y;
        SDL_RenderCopy(ren, background, 0, &dest);
        if (d != direction::right)
        {
                for (int i = snake_v.size() - 1; i != 0; --i)
                {
                        snake_v[i].x = snake_v[i - 1].x;
                        snake_v[i].y = snake_v[i - 1].y;
                }
                snake_v[0].x -= 20;
                d = direction::left;
        }
}
void display_snake(const deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&snake_head, SDL_Texture *&snake_body)
{
        SDL_Rect dest;
        dest.w = dest.h = 20;
        for (const auto &snake : snake_v)
        {
                dest.x = snake.x;
                dest.y = snake.y;
                if (snake.state == 0)
                        SDL_RenderCopy(ren, snake_head, 0, &dest);
                else
                        SDL_RenderCopy(ren, snake_body, 0, &dest);
        }

}
pair<int, int> display_food(const deque<snake_node> &snake_v, SDL_Renderer *&ren, SDL_Texture *&snake_food)
{
        pair<int, int> food_coordin = generate_random_array(snake_v);
        SDL_Rect dest;
        dest.w = dest.h = 20;
        dest.x = food_coordin.first;
        dest.y = food_coordin.second;
        SDL_RenderCopy(ren, snake_food, 0, &dest);
        return food_coordin;
}
void destroy_food(SDL_Texture *&background, SDL_Renderer *&ren, pair<int, int> food_coordi)
{
        SDL_Rect dest;
        dest.x = food_coordi.first;
        dest.y = food_coordi.first;
        dest.w = dest.h = 20;
        SDL_RenderCopy(ren, background, 0, &dest);
}
bool eat_food(deque<snake_node> &snake_v, pair<int, int> food_coordi, SDL_Texture *&background, SDL_Renderer *&ren)
{
        snake_node snake_head = snake_v[0];
        if (snake_head.x == food_coordi.first && snake_head.y == food_coordi.second)
        {
                destroy_food(background, ren, food_coordi);
                return true;
        }
        return false;
}
void snake_longer(deque<snake_node> &snake_v, const direction &d)
{
        snake_node snake_new_node;
        snake_node snake_head = snake_v[0];
        snake_new_node.state = 1;
        snake_new_node.x = snake_head.x;
        snake_new_node.y = snake_head.y;
        snake_v.insert(snake_v.begin() + 1, snake_new_node);
        switch (d)
        {
        case direction::up:
                snake_v[0].y -= 20;
                break;
        case down:
                snake_v[0].y += 20;
                break;
        case direction::left:
                snake_v[0].x -= 20;
                break;
        case direction::right:
                snake_v[0].x += 20;
                break;
        default:
                break;
        }
}
bool hit_barrier(const deque<snake_node> &snake_v)
{
        snake_node snake_head = snake_v[0];
        for (auto val : barrier_coordi)
        {
                if (snake_head.x == val.first * 20 && snake_head.y == val.second * 20)
                        return true;
        }
        if (snake_head.x < 0 || snake_head.x >= 400 || snake_head.y < 0 || snake_head.y >= 400)
                return true;
        return false;
}
bool eat_self(const deque<snake_node> &snake_v)
{
        snake_node snake_head = snake_v[0];
        for (int i = 1; i != snake_v.size(); ++i)
        {
                if (snake_head.x == snake_v[i].x && snake_head.y == snake_v[i].y)
                        return true;
        }
        return false;
}
void display_game_over(SDL_Renderer *&ren, SDL_Texture *&game_over)
{
        SDL_RenderCopy(ren, game_over, 0, 0);
}
vector<SDL_Texture*> Init_TTF(const string &file, SDL_Renderer *ren)
{
        if (TTF_Init() == -1)
                cout << "Create TTF Error: " << TTF_GetError() << endl;
        vector<SDL_Texture*> texture_v;
        SDL_Color color = { 0, 0, 0, 255 };
        SDL_Surface *ttf_surface;
        SDL_Texture *ttf_texture;
        vector<const char *> char_v = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
        TTF_Font *font = TTF_OpenFont(file.c_str(), 20);
        if (!font)
        {
                cout << "open TTF Error: " << TTF_GetError() << endl;
        }
        for (auto char_variable : char_v)
        {
                ttf_surface = TTF_RenderText_Blended(font, char_variable, color);
                ttf_texture = SDL_CreateTextureFromSurface(ren, ttf_surface);
                texture_v.push_back(ttf_texture);
                SDL_FreeSurface(ttf_surface);
        }
        TTF_CloseFont(font);
        return texture_v;
}
void Init_food_count(SDL_Renderer *&ren, const vector<SDL_Texture*> &ttf_texture)
{
        SDL_Rect dest;
        int w, h;
        SDL_QueryTexture(ttf_texture[0], 0, 0, &w, &h);
        dest.w = w;
        dest.h = h;
        dest.x = 0;
        dest.y = 400;
        SDL_RenderCopy(ren, ttf_texture[0], 0, &dest);
        //SDL_RenderPresent(ren);
        dest.x += 20;
        SDL_RenderCopy(ren, ttf_texture[0], 0, &dest);
        dest.x += 20;
        SDL_RenderCopy(ren, ttf_texture[0], 0, &dest);
}
void display_food_count(int food_count, SDL_Renderer *&ren, const vector<SDL_Texture*> &ttf_texture, SDL_Texture *&background)
{

        SDL_Rect dest;
        int w, h;
        SDL_QueryTexture(ttf_texture[0], 0, 0, &w, &h);
        dest.w = w;
        dest.h = h;
        //Init_food_count(ren, ttf_texture);
        dest.y = 400;
        if (food_count < 10)
        {
                dest.x = 40;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count], 0, &dest);
        }
        else if(food_count < 100)
        {
                dest.x = 20;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count / 10], 0, &dest);
                dest.x = 40;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count % 10], 0, &dest);
        }
        else
        {
                dest.x = 0;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count / 100], 0, &dest);
                dest.x = 20;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count % 100 - food_count % 10], 0, &dest);
                dest.x = 40;
                SDL_RenderCopy(ren, background, 0, &dest);
                SDL_RenderCopy(ren, ttf_texture[food_count % 10], 0, &dest);
        }
        //SDL_RenderPresent(ren);
}
int main(int,char **)
{
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
                cout << "Init Error: " << SDL_GetError() << endl;
                return 1;
        }
        //create window
        SDL_Window *win = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 420, SDL_WINDOW_SHOWN);
        if (!win)
        {
                cout << "Create Window Error: " << SDL_GetError() << endl;
                return 1;
        }
                //create render
        SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        //SDL_Renderer *ren1 = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!ren)
        {
                cout << "Create Renderer Error: " << SDL_GetError() << endl;
                return 1;
        }
        SDL_Texture *background_png, *snake_body_png, *snake_head_png, *snake_food_png, *barrier_png, *game_over_png;
        background_png = CreateTexture(ren, "background.png");
        snake_body_png = CreateTexture(ren, "snake_body.png");
        snake_head_png = CreateTexture(ren, "snake_head.png");
        snake_food_png = CreateTexture(ren, "snake_food.png");
        game_over_png = CreateTexture(ren, "game_over.png");
        barrier_png = CreateTexture(ren, "barrier.png");


        direction d = direction::right;
        deque<snake_node> snake_v;
        pair<int, int> food_coordi;
        vector<SDL_Texture*> ttf_texture;
        string file = "abg.ttf";
        int food_count = 0;
        int flag = 1;
        Init_snake(snake_v);
        Init_map(background_png, barrier_png, ren);
        food_coordi = display_food(snake_v, ren, snake_food_png);
        ttf_texture = Init_TTF(file, ren);
        Init_food_count(ren, ttf_texture);
        ////display_snake(snake_v, ren, snake_head_png, snake_body_png);
        ////SDL_Delay(2000);
        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
                //move automatically ### this has the problem
                switch (d)
                {
                case direction::left:
                        move_left(d, snake_v, ren, background_png);
                        break;
                case direction::right:
                        move_right(d, snake_v, ren, background_png);
                        break;
                case up:
                        move_up(d, snake_v, ren, background_png);
                        break;
                case down:
                        move_down(d, snake_v, ren, background_png);
                        break;
                default:
                        break;
                }
                display_snake(snake_v, ren, snake_head_png, snake_body_png);
                display_food_count(food_count, ren, ttf_texture, background_png);
                SDL_RenderPresent(ren);
                SDL_Delay(150);
                if (eat_food(snake_v, food_coordi, background_png, ren))
                {
                        ++food_count;
                        snake_longer(snake_v, d);
                        food_coordi = display_food(snake_v, ren, snake_food_png);
                        //cout << food_coordi.first << ' ' << food_coordi.second << endl;
                }
                if (eat_self(snake_v))
                {
                        display_game_over(ren, game_over_png);
                        SDL_RenderPresent(ren);
                        SDL_Delay(5000);
                        quit = true;
                }
                while (SDL_PollEvent(&e))
                {
                        flag = 1;
                        if (e.type == SDL_QUIT)
                                quit = true;
                        if (e.type == SDL_KEYDOWN)
                        {
                                switch (e.key.keysym.sym)
                                {
                                case SDLK_SPACE:
                                        while (flag)
                                        {
                                                while (SDL_PollEvent(&e))
                                                {
                                                        if (e.type == SDL_KEYDOWN)
                                                        {
                                                                if (e.key.keysym.sym == SDLK_z)
                                                                {
                                                                        flag = 0;
                                                                        break;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                case SDLK_ESCAPE:
                                        quit = true;
                                        break;
                                case SDLK_UP:
                                        move_up(d, snake_v, ren, background_png);
                                        break;
                                case SDLK_DOWN:
                                        move_down(d, snake_v, ren, background_png);
                                        break;
                                case SDLK_LEFT:
                                        move_left(d, snake_v, ren, background_png);
                                        break;
                                case SDLK_RIGHT:
                                        move_right(d, snake_v, ren, background_png);
                                        break;
                                default:
                                        break;
                                }
                                if (eat_food(snake_v, food_coordi, background_png, ren))
                                {
                                        ++food_count;
                                        snake_longer(snake_v, d);
                                        food_coordi = display_food(snake_v, ren, snake_food_png);
                                        //cout << food_coordi.first << ' ' << food_coordi.second << endl;
                                }
                                if (eat_self(snake_v))
                                {
                                        display_game_over(ren, game_over_png);
                                        SDL_RenderPresent(ren);
                                        SDL_Delay(5000);
                                        quit = true;
                                }
                        }
                }


        }
        destroy(barrier_png, game_over_png, snake_food_png, snake_head_png, snake_body_png, background_png, ren, win);
        SDL_Quit();
        TTF_Quit();
        return 0;
}
