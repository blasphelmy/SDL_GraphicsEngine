#pragma once
#include <cstdint>
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <unordered_map>
namespace blsp
{
    //utils, thanks javid..

    std::string hex(uint32_t n, uint8_t d)
    {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    };

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::microseconds;
    using std::chrono::seconds;

    template<class TYPE>
    struct vector2d {
        TYPE x, y;
        vector2d()                                { this->x = 0;  this->y = 0; }
        vector2d(TYPE xy)                         { this->x = xy; this->y = xy; }
        vector2d(TYPE x, TYPE y)                  { this->x = x;  this->y = y; }
        void setAll(TYPE xyz)                     { this->x = xyz; this->y = xyz; }
        void setXY(TYPE x, TYPE y)                { this->x = x; this->y = y; }

        vector2d operator + (vector2d const& obj) { vector2d result; result.x = x + obj.x; result.y = y + obj.y; return result; }
        vector2d operator - (vector2d const& obj) { vector2d result; result.x = x - obj.x; result.y = y - obj.y; return result; }
        vector2d operator * (vector2d const& obj) { vector2d result; result.x = x * obj.x; result.y = y * obj.y; return result; }
        vector2d operator / (vector2d const& obj) { vector2d result; result.x = x / obj.x; result.y = y / obj.y; return result; }

        template<class TYPE>
        vector2d<TYPE> cast_to()                  { return vector2d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y) }; };
    };

    template<class TYPE>
    struct vector3d {
        TYPE x, y, z;
        vector3d()                                { this->x = 0;   this->y = 0;   this->z = 0; }
        vector3d(TYPE xyz)                        { this->x = xyz; this->y = xyz; this->z = xyz; }
        vector3d(TYPE x, TYPE y, TYPE z)          { this->x = x;   this->y = y;   this->z = z; }
        void setAll(TYPE xyz)                     { this->x = xyz; this->y = xyz; this->z = xyz; }

        vector3d operator + (vector3d const& obj) { vector3d result; result.x = x + obj.x; result.y = y + obj.y; result.z = z + obj.z; return result; }
        vector3d operator - (vector3d const& obj) { vector3d result; result.x = x - obj.x; result.y = y - obj.y; result.z = z - obj.z; return result; }
        vector3d operator * (vector3d const& obj) { vector3d result; result.x = x * obj.x; result.y = y * obj.y; result.z = z * obj.z; return result; }
        vector3d operator / (vector3d const& obj) { vector3d result; result.x = x / obj.x; result.y = y / obj.y; result.z = z / obj.z; return result; }

        template<class TYPE>
        vector3d<TYPE> cast_to()                  { return vector3d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y), static_cast<TYPE>(z) }; };
    };

    template<class TYPE>
    struct vector4d {
        TYPE x, y, z, a;
        vector4d()                                { this->x = 0;   this->y = 0;   this->z = 0; this->a = 0; }
        vector4d(TYPE xyz)                        { this->x = xyz; this->y = xyz; this->z = xyz; }
        vector4d(TYPE x, TYPE y, TYPE z, TYPE a)  { this->x = x;   this->y = y;   this->z = z; this->a = a; }
        void setAll(TYPE xyz)                     { this->x = xyz; this->y = xyz; this->z = xyz; }

        vector4d operator + (vector4d const& obj) { vector4d result; result.x = x + obj.x; result.y = y + obj.y; result.z = z + obj.z; return result; }
        vector4d operator - (vector4d const& obj) { vector4d result; result.x = x - obj.x; result.y = y - obj.y; result.z = z - obj.z; return result; }
        vector4d operator * (vector4d const& obj) { vector4d result; result.x = x * obj.x; result.y = y * obj.y; result.z = z * obj.z; return result; }
        vector4d operator / (vector4d const& obj) { vector4d result; result.x = x / obj.x; result.y = y / obj.y; result.z = z / obj.z; return result; }

        template<class TYPE>
        vector4d<TYPE> cast_to()                  { return vector4d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y), static_cast<TYPE>(z), static_cast<TYPE>(a) }; };
    };

    typedef vector2d<int>         vector2i;
    typedef vector2d<float>       vector2f;
    typedef vector2d<double>      vector2db;
    typedef vector2d<long double> vector2ld;
    typedef vector2d<uint32_t>    vector2i32t;
    typedef vector2d<uint16_t>    vector2i16t;
    typedef vector2d<uint8_t>     vector2di8t;

    typedef vector3d<int>         vector3i;
    typedef vector3d<float>       vector3f;
    typedef vector3d<double>      vector3db;
    typedef vector3d<long double> vector3ld;
    typedef vector3d<uint16_t>    vector3i16t;
    typedef vector3d<uint8_t>     vector3di8t;

    typedef vector4d<int>         vector4i;
    typedef vector4d<float>       vector4f;
    typedef vector4d<double>      vector4db;
    typedef vector4d<long double> vector4ld;
    typedef vector4d<uint16_t>    vector4i16t;
    typedef vector4d<uint8_t>     vector4di8t;

    struct Color : public vector4di8t { Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { this->x = r; this->y = g; this->z = b; this->a = a; } };

    static const Color RED(204, 0, 0, 255), DARK_RED(102, 0, 0, 255), ORANGE(255, 128, 0, 255), BROWN(102, 51, 0, 255), DARK_BROWN(51, 25, 0, 255),
            GREEN(25, 51, 0, 255), LIME_GREEN(128, 255, 0, 255), FOREST_GREEN(0, 51, 0, 255), CYAN(0, 255, 255, 255), TEAL(0, 153, 153, 255);

    typedef enum KEYBOARD {
        A = 'a', B = 'b', C = 'c', D = 'd', E = 'e', F = 'f', G = 'g', H = 'h', I = 'i', J = 'j', K = 'k', L = 'l', M = 'm', 
        N = 'n', O = 'o', P = 'p', Q = 'q', R = 'r', S = 's', T = 't', U = 'u', V = 'v', W = 'w', X = 'x', Y = 'y', Z = 'z',
        NUM_0 = '0', NUM_1 = '1', NUM_2 = '2', NUM_3 = '3', NUM_4 = '4', NUM_5 = '5', NUM_6 = '6', NUM_7 = '7', NUM_8 = '8', NUM_9 = '9',
        SPACE = ' ', BKSP = '\b'
    };
    typedef enum ARROW_KEYS {
        RIGHT_ARROW = 0x4f, LEFT_ARROW = 0x50, DOWN_ARROW = 0x51, UP_ARROW = 0x52
    };

    typedef enum MOUSE_EVENTS {
        LEFT_CLICK = 0x01, SCROLL_BTN_CLICK = 0x02, RIGHT_CLICK = 0x03
    };

    int i = SDLK_0;

    struct keyState {
        bool pressed = false;
    };

    struct MouseButtonsState : public keyState {
        bool released = true;
    };

    /*
        SDL Graphics Engine Implementation
    */
    class SDL_GraphicsEngine {
    public:
        SDL_GraphicsEngine() {}
        ~SDL_GraphicsEngine() {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_CloseFont(font);
            SDL_Quit();
        };
    private:
        struct buttons {
            std::unordered_map<int, keyState> keyStates;
            void resetKeys() {
                for (auto& x : keyStates) {
                    if (x.first <= 0x03)
                        continue;

                    x.second.pressed = false;
                }
            }
        };
        struct mouseButtons {
            bool freezeMotion = false;
            std::unordered_map<short, MouseButtonsState> keyStates;
        };
    private:
        SDL_Renderer*       renderer = nullptr;
        SDL_Window*         window = nullptr;
        SDL_Event           event;
        TTF_Font*           font;
        buttons             buttonStates;
        mouseButtons        mouseButtonStates;

    public:
        bool done = false;
    protected:
        std::string appName;
        vector2f mousePOS;
    private:
        high_resolution_clock::time_point start;
        high_resolution_clock::time_point finish;
    public:
        virtual void OnUserCreate() = 0;
        virtual bool OnUserUpdate(double elaspedTimeMS) = 0;
    public:
        void ConstructWindow(uint32_t width, uint32_t height) {
            OnUserCreate();
            SetUpComponents(width, height);
            Tick();
        }
    private:
        bool keyListener() {
            bool isDragging = true;
            SDL_PollEvent(&event);
            if (event.type == SDL_QUIT) return true;
            if (event.type == SDL_KEYDOWN) {
                buttonStates.keyStates[event.key.keysym.sym].pressed = true;
                int i = event.key.keysym.sym;
                int keycodes = i;
                int arrow_keyMask = 0x000000ff;
                keycodes = i & arrow_keyMask;
                //79 right, 80 left, 81 down, 82 up
                if (keycodes >= 0x46 && keycodes <= 0x52) {
                    buttonStates.keyStates[keycodes].pressed = true;
                }
                return false;
            }
            if (event.type == SDL_MOUSEMOTION && !mouseButtonStates.freezeMotion) {
                mousePOS.setXY(event.button.x, event.button.y);
                return false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && mouseButtonStates.keyStates[event.button.button].pressed == false) {
                mouseButtonStates.freezeMotion = !mouseButtonStates.freezeMotion;
                mouseButtonStates.keyStates[event.button.button].pressed = mouseButtonStates.keyStates[event.button.button].pressed ? false : true;
                mouseButtonStates.keyStates[event.button.button].released = mouseButtonStates.keyStates[event.button.button].released ? false : true;
                return false;
            }
            if (event.type == SDL_MOUSEBUTTONUP && mouseButtonStates.keyStates[event.button.button].pressed == true) {
                //std::cout << "Button clicked! " + hex(event.button.button, 2) << std::endl;
                mouseButtonStates.keyStates[event.button.button].pressed = mouseButtonStates.keyStates[event.button.button].pressed ? false : true;
                mouseButtonStates.keyStates[event.button.button].released = mouseButtonStates.keyStates[event.button.button].released ? false : true;
                mouseButtonStates.freezeMotion = !mouseButtonStates.freezeMotion;
                return false;
            }
            return false;
        }
    private:
        void SetUpComponents(uint32_t width, uint32_t height) {
            SDL_Init(SDL_INIT_VIDEO);
            SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
            TTF_Init();
            font = TTF_OpenFont("./ubuntumono.ttf", 13);
            SDL_SetWindowTitle(window, appName.c_str());
            buttonStates.keyStates = {
                std::make_pair(KEYBOARD::A, keyState()), std::make_pair(KEYBOARD::B, keyState()), std::make_pair(KEYBOARD::C, keyState()), std::make_pair(KEYBOARD::D, keyState()), std::make_pair(KEYBOARD::E, keyState()), std::make_pair(KEYBOARD::F, keyState()), std::make_pair(KEYBOARD::G, keyState()),
                std::make_pair(KEYBOARD::H, keyState()), std::make_pair(KEYBOARD::I, keyState()), std::make_pair(KEYBOARD::J, keyState()), std::make_pair(KEYBOARD::K, keyState()), std::make_pair(KEYBOARD::L, keyState()), std::make_pair(KEYBOARD::M, keyState()), std::make_pair(KEYBOARD::N, keyState()),
                std::make_pair(KEYBOARD::O, keyState()), std::make_pair(KEYBOARD::P, keyState()), std::make_pair(KEYBOARD::Q, keyState()), std::make_pair(KEYBOARD::R, keyState()), std::make_pair(KEYBOARD::S, keyState()), std::make_pair(KEYBOARD::T, keyState()), std::make_pair(KEYBOARD::U, keyState()),
                std::make_pair(KEYBOARD::V, keyState()), std::make_pair(KEYBOARD::W, keyState()), std::make_pair(KEYBOARD::X, keyState()), std::make_pair(KEYBOARD::Y, keyState()), std::make_pair(KEYBOARD::Z, keyState()),

                std::make_pair(KEYBOARD::NUM_0, keyState()), std::make_pair(KEYBOARD::NUM_1, keyState()), std::make_pair(KEYBOARD::NUM_2, keyState()), std::make_pair(KEYBOARD::NUM_3, keyState()), std::make_pair(KEYBOARD::NUM_4, keyState()),
                std::make_pair(KEYBOARD::NUM_5, keyState()), std::make_pair(KEYBOARD::NUM_6, keyState()), std::make_pair(KEYBOARD::NUM_7, keyState()), std::make_pair(KEYBOARD::NUM_8, keyState()), std::make_pair(KEYBOARD::NUM_9, keyState()),
                std::make_pair(SPACE, keyState()), std::make_pair(BKSP, keyState()),
                std::make_pair(ARROW_KEYS::DOWN_ARROW, keyState()), std::make_pair(ARROW_KEYS::UP_ARROW, keyState()), std::make_pair(ARROW_KEYS::RIGHT_ARROW, keyState()), std::make_pair(ARROW_KEYS::LEFT_ARROW, keyState()),
            };
            mouseButtonStates.keyStates = {
                std::make_pair(MOUSE_EVENTS::LEFT_CLICK, MouseButtonsState()), std::make_pair(MOUSE_EVENTS::SCROLL_BTN_CLICK, MouseButtonsState()), std::make_pair(MOUSE_EVENTS::RIGHT_CLICK, MouseButtonsState()),
            };
        }
        void Tick() {
            start = high_resolution_clock::now();
            while (!keyListener()) {
                if (SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
                finish = high_resolution_clock::now();
                double elaspedTimeMS = ((double)(duration_cast<microseconds>(this->finish - this->start).count())) / 1000.f;
                if (OnUserUpdate(elaspedTimeMS)) {
                    start = high_resolution_clock::now();
                }
                buttonStates.resetKeys();
            }
        }
    protected:
        Color AdjustColorSat(Color colorVector, float f, float r_bias = 0.3, float g_bias = 0.6, float b_bias = 0.1) {
            float r = (float)colorVector.x;
            float g = (float)colorVector.y;
            float b = (float)colorVector.z;

            float L = r_bias * r
                + g_bias * g
                + b_bias * b;

            float new_r = r + f * (L - r);
            float new_g = g + f * (L - g);
            float new_b = b + f * (L - b);
            return Color((int)new_r, (int)new_g, (int)new_b, colorVector.a);
        };
    protected:
        keyState GetKey(char keyName) {
            return buttonStates.keyStates[keyName];
        }
        MouseButtonsState GetMouseState(short _MOUSE_EVENTS) {
            if (_MOUSE_EVENTS <= MOUSE_EVENTS::RIGHT_CLICK) {
                return mouseButtonStates.keyStates[_MOUSE_EVENTS];
            }
        }
    protected:
        void SetDrawColor(Color color) {
            SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, color.a);
        }
        void ClearPixels(Color color) {
            SetDrawColor(color);
            SDL_RenderClear(renderer);
        }
        void RenderScreen() {
            SDL_RenderPresent(renderer);
        }

    protected:
        void DrawPixel(Color color, float x, float y) {
            SetDrawColor(color);
            SDL_RenderDrawPoint(renderer, x, y);
        }
        void DrawPixel(Color color, int x, int y) {
            DrawPixel(color, (float)x, float(y));
        }
        void DrawPixel(Color color, uint32_t x, uint32_t y) {
            DrawPixel(color, (float)x, (float)y);
        }

        void DrawLine(Color color, float x, float y, float x2, float y2) {
            SetDrawColor(color);
            SDL_RenderDrawLineF(renderer, x, y, x2, y2);
        }
        void DrawLine(Color color, vector2i start, vector2i end) {
            DrawLine(color, (float)start.x, (float)start.y, (float)end.x, (float)end.y);
        }
        void DrawLine(Color color, vector2i32t start, vector2i32t end) {
            DrawLine(color, (float)start.x, (float)start.y, (float)end.x, (float)end.y);
        }
        void DrawLine(Color color, vector2f start, vector2f end) {
            DrawLine(color, start.x, start.y, end.x, end.y);
        }

        void DrawRectOutline(Color color, float x, float y, float x2, float y2) {
            SetDrawColor(color);
            SDL_FRect rect = { x, y, x2, y2 };
            SDL_RenderDrawRectF(renderer, &rect);
        }
        void DrawRectOutline(Color color, vector2i pos, vector2i size) {
            DrawRectOutline(color, (float)pos.x, (float)pos.y, (float)size.x, (float)size.y);
        }
        void DrawRectOutline(Color color, vector2i32t pos, vector2i32t size) {
            DrawRectOutline(color, (float)pos.x, (float)pos.y, (float)size.x, (float)size.y);
        }
        void DrawRectOutline(Color color, vector2f pos, vector2f size) {
            DrawRectOutline(color, pos.x, pos.y, size.x, size.y);
        }

        void DrawRectFill(Color color, float x, float y, float x2, float y2) {
            SetDrawColor(color);
            SDL_FRect rect = { x, y, x2, y2 };
            SDL_RenderFillRectF(renderer, &rect);
        }
        void DrawRectFill(Color color, vector2i pos, vector2i size) {
            DrawRectFill(color, (float)pos.x, (float)pos.y, (float)size.x, (float)size.y);
        }
        void DrawRectFill(Color color, vector2i32t pos, vector2i32t size) {
            DrawRectFill(color, (float)pos.x, (float)pos.y, (float)size.x, (float)size.y);
        }
        void DrawRectFill(Color color, vector2f pos, vector2f size) {
            DrawRectFill(color, pos.x, pos.y, size.x, size.y);
        }

        void DrawRoundedRectFill(Color color, float posx, float posy, int sizex, int sizey, int radius) {
            SetDrawColor(color);
            if (sizex % 2 == 0) sizex = sizex - 1;
            if (sizey % 2 == 0) sizey = sizey - 1;

            float x0 = posx + (sizex / 2);
            float y0 = posy + (sizey / 2);
            float x = radius - 1;
            float y = 0;
            float dx = 1;
            float dy = 1;
            int err = dx - (radius << 1);
            while (x >= y) {
                SDL_RenderDrawLineF(renderer, x0 + y + (sizex / 2) - radius, y0 + x + (sizey / 2) - radius, x0 - y - (sizex / 2) + radius, y0 + x + (sizey / 2) - radius);
                SDL_RenderDrawLineF(renderer, x0 + x + (sizex / 2) - radius, y0 + y + (sizey / 2) - radius, x0 - x - (sizex / 2) + radius, y0 + y + (sizey / 2) - radius);
                SDL_RenderDrawLineF(renderer, x0 + x + (sizex / 2) - radius, y0 - y - (sizey / 2) + radius, x0 - x - (sizex / 2) + radius, y0 - y - (sizey / 2) + radius);
                SDL_RenderDrawLineF(renderer, x0 + y + (sizex / 2) - radius, y0 - x - (sizey / 2) + radius, x0 - y - (sizex / 2) + radius, y0 - x - (sizey / 2) + radius);
                if (err <= 0)
                {
                    y++;
                    err += dy;
                    dy += 2;
                }
                if (err > 0)
                {
                    x--;
                    dx += 2;
                    err += dx - (radius << 1);
                }
            }
            SDL_FRect rect = { posx + 1, posy + radius, sizex - 2, sizey - (radius * 2) };
            SDL_RenderFillRectF(renderer, &rect);
        }
        void DrawRoundedRectFill(Color color, vector2f pos, vector2i size, int radius) {
            DrawRoundedRectFill(color, pos.x, pos.y, size.x, size.y, radius);
        }


        //https://en.wikipedia.org/w/index.php?title=Midpoint_circle_algorithm&oldid=889172082#C_example
        void DrawCircleOutline(Color color, float x0, float y0, int radius) {
            SetDrawColor(color);
            float x = radius - 1;
            float y = 0;
            float dx = 1;
            float dy = 1;
            float err = dx - (radius << 1);
            while (x >= y) {
                SDL_RenderDrawPoint(renderer, x0 + x, y0 + y);
                SDL_RenderDrawPoint(renderer, x0 + y, y0 + x);
                SDL_RenderDrawPoint(renderer, x0 - y, y0 + x);
                SDL_RenderDrawPoint(renderer, x0 - x, y0 + y);
                SDL_RenderDrawPoint(renderer, x0 - x, y0 - y);
                SDL_RenderDrawPoint(renderer, x0 - y, y0 - x);
                SDL_RenderDrawPoint(renderer, x0 + y, y0 - x);
                SDL_RenderDrawPoint(renderer, x0 + x, y0 - y);
                if (err <= 0)
                {
                    y++;
                    err += dy;
                    dy += 2;
                }

                if (err > 0)
                {
                    x--;
                    dx += 2;
                    err += dx - (radius << 1);
                }
            }
        }
        void DrawCircleOutline(Color color, vector2f pos, int radius) {
            DrawCircleOutline(color, pos.x, pos.y, radius);
        }

        void DrawCircleFill(Color color, float x0, float y0, int radius) {
            SetDrawColor(color);
            float x = radius - 1;
            float y = 0;
            float dx = 1;
            float dy = 1;
            int err = dx - (radius << 1);
            while (x >= y) {
                SDL_RenderDrawLineF(renderer, x0 + y, y0 + x, x0 - y, y0 + x);
                SDL_RenderDrawLineF(renderer, x0 + x, y0 + y, x0 - x, y0 + y);
                SDL_RenderDrawLineF(renderer, x0 + x, y0 - y, x0 - x, y0 - y);
                SDL_RenderDrawLineF(renderer, x0 + y, y0 - x, x0 - y, y0 - x);
                if (err <= 0)
                {
                    y++;
                    err += dy;
                    dy += 2;
                }

                if (err > 0)
                {
                    x--;
                    dx += 2;
                    err += dx - (radius << 1);
                }
            }
        }
        void DrawCircleFill(Color color, vector2f pos, int radius) {
            DrawCircleFill(color, pos.x, pos.y, radius);
        }

        void DrawString(uint8_t r, uint8_t g, uint8_t b, std::string& text, float x, float y) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), { r, g, b });
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = { x, y, text.size() * 7, 13 };
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);
            SDL_FreeSurface(surface);
        }
        void DrawString(Color color, std::string& text, vector2i pos) {
            DrawString(color.x, color.y, color.z, text, (float)pos.x, (float)pos.y);
        }

        void DrawTriangleFill(float ax, float ay, float bx, float by, float cx, float cy, Color a, Color b, Color c) {
            const std::vector<SDL_Vertex> verts =
            {
                { SDL_FPoint{ ax, ay }, SDL_Color{ a.x, a.y, a.z, a.a }, SDL_FPoint{ 0 }, },
                { SDL_FPoint{ bx, by }, SDL_Color{ b.x, b.y, b.z, b.a }, SDL_FPoint{ 0 }, },
                { SDL_FPoint{ cx, cy }, SDL_Color{ c.x, c.y, c.z, c.a }, SDL_FPoint{ 0 }, },
            };
            SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
        }
        void DrawTriangleFill(vector2f apos, vector2f bpos, vector2f cpos, Color a, Color b, Color c) {
            DrawTriangleFill(apos.x, apos.y, bpos.x, bpos.y, cpos.x, cpos.y, a, b, c);
        }

        void DrawTriangleOutline(Color color, float ax, float ay, float bx, float by, float cx, float cy) {
            SetDrawColor(color);
            SDL_RenderDrawLineF(renderer, ax, ay, bx, by);
            SDL_RenderDrawLineF(renderer, ax, ay, cx, cy);
            SDL_RenderDrawLineF(renderer, cx, cy, bx, by);
        }
        void DrawTriangleOutline(Color color, vector2f a, vector2f b, vector2f c) {
            DrawTriangleOutline(color, a.x, a.y, b.x, b.y, c.x, c.y);
        }

        SDL_Vertex CreatePoint(float x, float y, Color color) {
            return { SDL_FPoint{x, y}, SDL_Color{color.x, color.y, color.z, color.a}, SDL_FPoint{0} };
        }
        void DrawGeometry(std::vector<SDL_Vertex> mesh) {
            SDL_RenderGeometry(renderer, nullptr, mesh.data(), mesh.size(), nullptr, 0);
        }
    };
}