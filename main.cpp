#include <iostream>
#include "SDL_GraphicsEngine.h"
#undef main

#define WINDOW_WIDTH 1280   
#define WINDOW_HEIGHT 720

//Example on usage

class MandleBrotSet : public blsp::SDL_GraphicsEngine {
public:
    MandleBrotSet() {
        appName = "MandleBrotSet";
    }
    void OnUserCreate() override {
    }
};

int main() {
    SDL_Event event;

    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    mdset.ClearPixels(blsp::Color(0, 0, 0, 0));
    mdset.DrawPixel(blsp::LIME_GREEN, 50, 50);
    mdset.DrawLine(blsp::LIME_GREEN, blsp::vector2i(20, 20), blsp::vector2i(100, 120));
    mdset.DrawRectFill(blsp::GREEN, blsp::vector2i(40, 50), blsp::vector2i(200, 100));
    mdset.RenderScreen();

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
	return 0;
}