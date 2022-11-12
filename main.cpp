#include <iostream>
#include <string> 
#include "SDL_GraphicsEngine.h"
#undef main

static const int WINDOW_WIDTH  = 1280;
static const int WINDOW_HEIGHT = 720;

//Example usage / testing

class MandleBrotSet : public blsp::SDL_GraphicsEngine {
public:
    MandleBrotSet() {
        appName = "MandleBrotSet";
    }
public:
    int i = 0;
    bool jiggletest = true;
    std::string keyboardtest = "";
public:
    void OnUserCreate() override {
    }
    bool OnUserUpdate(double elaspedTimeMS) override {
        ClearPixels(blsp::Color(0, 0, 0, 0));
        DrawPixel(blsp::LIME_GREEN, 50, 50);
        DrawRoundedRectFill(blsp::CYAN, 350, 350, 200, 350, 30);
        DrawLine(blsp::LIME_GREEN, blsp::vector2i(20, 20), blsp::vector2i(100, 120));
        DrawTriangleOutline(blsp::CYAN, 10, 10, 29, 19, 40, 50);
        if (i > 1280 - 200) jiggletest = false;
        if (i <= 0) jiggletest = true;
        if (jiggletest) {
            DrawRoundedRectFill(blsp::ORANGE, blsp::vector2f(i++ / 100, 50), blsp::vector2i(200, 100), 15);
        }
        else {
            DrawRoundedRectFill(blsp::ORANGE, blsp::vector2f(i-- / 100, 50), blsp::vector2i(200, 100), 15);
        }
        std::string time = std::to_string(elaspedTimeMS);
        DrawString(blsp::RED, time, blsp::vector2i(0, 0));

        if (GetKey(blsp::A).pressed) keyboardtest += "a";
        DrawString(blsp::RED, keyboardtest, blsp::vector2i(0, 10));
        
        RenderScreen();
        return true;
    }
};

int main() {
    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}