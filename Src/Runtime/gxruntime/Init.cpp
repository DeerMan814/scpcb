#include "Init.h"

#include <SDL.h>

namespace GX {

void InitEnv() {
    SDL_Init(SDL_INIT_VIDEO);
}

void QuitEnv() {
    SDL_Quit();
}

}
