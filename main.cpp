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
        ClearPixels(blsp::Color(40, 40, 40, 0));
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
        if (GetKey(blsp::B).pressed) keyboardtest += "b";
        if (GetKey(blsp::C).pressed) keyboardtest += "c";
        if (GetKey(blsp::D).pressed) keyboardtest += "d";
        if (GetKey(blsp::E).pressed) keyboardtest += "e";
        if (GetKey(blsp::F).pressed) keyboardtest += "f";
        if (GetKey(blsp::G).pressed) keyboardtest += "g";
        if (GetKey(blsp::H).pressed) keyboardtest += "h";
        if (GetKey(blsp::I).pressed) keyboardtest += "i";
        if (GetKey(blsp::J).pressed) keyboardtest += "j";
        if (GetKey(blsp::K).pressed) keyboardtest += "k";
        if (GetKey(blsp::L).pressed) keyboardtest += "l";
        if (GetKey(blsp::M).pressed) keyboardtest += "m";
        if (GetKey(blsp::N).pressed) keyboardtest += "n";
        if (GetKey(blsp::O).pressed) keyboardtest += "o";
        if (GetKey(blsp::P).pressed) keyboardtest += "p";
        if (GetKey(blsp::Q).pressed) keyboardtest += "q";
        if (GetKey(blsp::R).pressed) keyboardtest += "r";
        if (GetKey(blsp::S).pressed) keyboardtest += "s";
        if (GetKey(blsp::T).pressed) keyboardtest += "t";
        if (GetKey(blsp::U).pressed) keyboardtest += "u";
        if (GetKey(blsp::V).pressed) keyboardtest += "v";
        if (GetKey(blsp::W).pressed) keyboardtest += "w";
        if (GetKey(blsp::X).pressed) keyboardtest += "x";
        if (GetKey(blsp::Y).pressed) keyboardtest += "y";
        if (GetKey(blsp::Z).pressed) keyboardtest += "z";
        if (GetKey(blsp::SPACE).pressed) keyboardtest += " ";
        if (GetKey(blsp::ARROW_KEYS::RIGHT_ARROW).pressed) keyboardtest += ">";
        if (GetKey(blsp::BKSP).pressed) keyboardtest.pop_back();

        if (GetMouseState(blsp::MOUSE_EVENTS::LEFT_CLICK).pressed) {
            std::string clicked = "Left clicked!";
            DrawString(blsp::CYAN, clicked, blsp::vector2i(0, 32));
        }

        DrawString(blsp::CYAN, keyboardtest, blsp::vector2i(0, 16));
        
        RenderScreen();
        return true;
    }
};

int main() {
    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}