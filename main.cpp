#include <iostream>
#include <string> 
#include "SDL_GraphicsEngine.h"
#undef main

static const int WINDOW_WIDTH  = 1280;
static const int WINDOW_HEIGHT = 720;

//Example on usage

class MandleBrotSet : public blsp::SDL_GraphicsEngine {
public:
    MandleBrotSet() {
        appName = "MandleBrotSet";
    }
public:
    int i = 0;
    bool jiggletest = true;
public:
    void OnUserCreate() override {
    }
    bool OnUserUpdate(float elaspedTimeMS) override {
        ClearPixels(blsp::Color(0, 0, 0, 0));
        DrawPixel(blsp::LIME_GREEN, 50, 50);
        DrawRoundedRectFill(blsp::CYAN, 350, 350, 100, 50, 11);
        DrawLine(blsp::LIME_GREEN, blsp::vector2i(20, 20), blsp::vector2i(100, 120));
        if (i > 1280 - 200) jiggletest = false;
        if (i <= 0) jiggletest = true;
        if (jiggletest) {
            DrawRectFill(blsp::ORANGE, blsp::vector2i(i++, 50), blsp::vector2i(200, 100));
        }
        else {
            DrawRectFill(blsp::ORANGE, blsp::vector2i(i--, 50), blsp::vector2i(200, 100));
        }
        std::string time = std::to_string(elaspedTimeMS);
        DrawString(blsp::RED, time, blsp::vector2i(0, 0));
        RenderScreen();

        return true;
    }
};

int main() {
    SDL_Event event;

    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}