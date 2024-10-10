#include <SDL.h>
#include <iostream>
#include <math.h>

int main(int argc, char* argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to init SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Raycasting Game",
                                        10,
                                        30,
                                        800, 600,
                                        SDL_WINDOW_SHOWN);

    SDL_Window* devWindow = SDL_CreateWindow("Dev Window",
                                        900,
                                        30,
                                        800, 600,
                                        SDL_WINDOW_SHOWN);
                                                                              
    if(window == nullptr || devWindow == nullptr) {
        std::cerr << "Failed to init window." << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Renderer* devRenderer = SDL_CreateRenderer(devWindow, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr || devRenderer == nullptr) {
        std::cerr << "Failed to init renderer." << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool isRunning = true;
    SDL_Event event;

    const int mapWidth = 10;
    const int mapHeight = 10;
    int GameMap[mapWidth*mapHeight] = {1,1,1,1,1,1,1,1,1,1,
                                       1,0,0,0,0,0,0,0,0,1,
                                       1,0,0,0,0,0,0,0,0,1,
                                       1,0,0,0,0,0,0,0,0,1,
                                       1,0,0,1,1,1,0,0,0,1,
                                       1,0,0,0,1,1,0,0,0,1,
                                       1,0,0,0,0,1,0,0,0,1,
                                       1,0,0,0,0,0,0,0,0,1,
                                       1,0,0,0,0,0,0,0,0,1,
                                       1,1,1,1,1,1,1,1,1,1};

    const int wBlock = 50;
    const int hBlock = 50;

    const int renderDistance = 250;
    const int deltaRay = 10;
    const int viewAngle = 70;
    const int raysNumber = 10;

    int xPlayer = 100;
    int yPlayer = 270;
    int playerRotation = 0;

    while(isRunning) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_SetRenderDrawColor(devRenderer, 255,255,255,255);

        SDL_RenderClear(devRenderer);
        SDL_SetRenderDrawColor(devRenderer, 0,0,0,255);
        //---render-map---//
        for(int y = 0; y<mapHeight; y++) {
            for(int x = 0; x<mapWidth; x++) {
                if(GameMap[y*mapHeight + x] == 1) {
                    SDL_Rect rect = {x*wBlock, y*hBlock, wBlock, hBlock};
                    SDL_RenderDrawRect(devRenderer, &rect);
                }
            }
        }

        int xRayEnd = xPlayer;
        int yRayEnd = yPlayer;
        int rayLength = 0;
        bool wallCollision = false;
        
        while((rayLength <= renderDistance) && (!wallCollision)) {
            xRayEnd += deltaRay;
            yRayEnd = yRayEnd;
            rayLength += deltaRay;
            int mapBlockIndex = (int)(xRayEnd / wBlock) + ((int)(yRayEnd / hBlock))*mapWidth;
            
            if(GameMap[mapBlockIndex] == 1) {
                wallCollision = true;
            }
            
        }
            std::cout << rayLength << std::endl;


        //dx = singleBlockWidth - (xPlayer % singleBlockWidth)

        SDL_SetRenderDrawColor(devRenderer, 0,255,0,255);
        SDL_RenderDrawLine(devRenderer, xPlayer, yPlayer, xRayEnd, yRayEnd);

        SDL_SetRenderDrawColor(devRenderer, 255,0,0,255);
        SDL_Rect playerRect = {xPlayer - 5, yPlayer - 5, 10, 10};
        SDL_RenderFillRect(devRenderer, &playerRect);

        SDL_RenderPresent(devRenderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


//g++ -o build/main.exe src/main.cpp -I./lib/SDL2/include/SDL2 -L./lib/SDL2/lib -lmingw32 -lSDL2main -lSDL2