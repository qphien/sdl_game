#include "SDL.h"
#include <iostream>
#include "SDL_mixer.h"

using namespace std;
int main(int, char **)
{
        if (SDL_Init(SDL_INIT_AUDIO) != 0)
        {
                cout << "SDL_Init Error: " << SDL_GetError() << endl;
                return 1;
        }
        /*if (Mix_Init(MIX_INIT_MP3) != 0)
        {
                cout << "Mix_init Error: " << Mix_GetError() << endl;
                return 1;
        }*/
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 512) != 0)
        {
                cout << "Mix_openAudio Error: " << Mix_GetError() << endl;
                return 1;
        }
        Mix_Music *music;
        music = Mix_LoadMUS("2.wav");
        Mix_Chunk *sample = Mix_LoadWAV("ir_begin.wav");
        Mix_AllocateChannels(3);
        if (!music)
        {
                cout << "load music error: " << Mix_GetError() << endl;
                return 1;
        }
        cout << Mix_VolumeMusic(MIX_MAX_VOLUME / 2) << endl;
        if (Mix_PlayMusic(music, 1) != 0)
        {
                cout << "play music error: " << Mix_GetError() << endl;
                //return 1;
        }
        if (Mix_PlayChannel(-1, sample, 0) == -1)
        {
                cout << "play sample error: " << Mix_GetError() << endl;
                return 1;
        }
        cout << Mix_PlayingMusic() << endl;
        SDL_Delay(25000);
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        atexit(SDL_Quit);
        return 0;


}
