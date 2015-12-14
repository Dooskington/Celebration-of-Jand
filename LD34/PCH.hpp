#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

// SDL2
#if WINDOWS
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2_image/SDL_image.h>
    #include <SDL2_ttf/SDL_ttf.h>
    #include <SDL2_mixer/SDL_mixer.h>
#endif
