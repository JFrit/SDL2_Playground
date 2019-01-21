#include <SDL2/SDL.h>
#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool quit= false;

SDL_Event e;

// Start up SDL and create a window
bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = nullptr;

SDL_Surface* gScreenSurface = nullptr;

SDL_Surface* gHelloWorld = nullptr;

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            while(!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }
                SDL_BlitSurface(gHelloWorld, nullptr, gScreenSurface, nullptr);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    close();
    return 0;
}

bool init() {
    // Initialization flag
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if(gWindow == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        } else {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia(){
    bool success = true;

    gHelloWorld = SDL_LoadBMP("Assets/Images/hello_world.bmp");
    if(gHelloWorld == nullptr) {
        printf("Unable to load image %s! SDL Error: %s\n", "hello_world.bmp", SDL_GetError());
        success = false;
    }
    return success;
}

void close() {
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}