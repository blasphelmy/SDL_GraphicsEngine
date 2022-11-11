#pragma once
#include <cstdint>
#include <SDL.h>
namespace blsp
{

    template<class TYPE>
    class vector2d {
    public:
        TYPE x, y;
        vector2d()                                { this->x = 0; this->y = 0; }
        vector2d(TYPE xy)                         { this->x = xy; this->y = xy; }
        vector2d(TYPE x, TYPE y)                  { this->x = x; this->y = y; }

        void setAll(TYPE xyz)                     { this->x = xyz; this->y = xyz; }

        vector2d operator + (vector2d const& obj) { vector2d result; result.x = x + obj.x; result.y = y + obj.y; return result; }
        vector2d operator - (vector2d const& obj) { vector2d result; result.x = x - obj.x; result.y = y - obj.y; return result; }
        vector2d operator * (vector2d const& obj) { vector2d result; result.x = x * obj.x; result.y = y * obj.y; return result; }
        vector2d operator / (vector2d const& obj) { vector2d result; result.x = x / obj.x; result.y = y / obj.y; return result; }

        template<class TYPE>
        vector2d<TYPE> cast_to() { return vector2d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y) }; };
    };

    template<class TYPE>
    class vector3d {
    public:
        TYPE x, y, z;
        vector3d()                                { this->x = 0; this->y = 0; this->z = 0; }
        vector3d(TYPE xyz)                        { this->x = xyz; this->y = xyz; this->z = xyz; }
        vector3d(TYPE x, TYPE y, TYPE z)          { this->x = x; this->y = y; this->z = z; }

        void setAll(TYPE xyz)                     { this->x = xyz; this->y = xyz; this->z = xyz; }

        vector3d operator + (vector3d const& obj) { vector3d result; result.x = x + obj.x; result.y = y + obj.y; result.z = z + obj.z; return result; }
        vector3d operator - (vector3d const& obj) { vector3d result; result.x = x - obj.x; result.y = y - obj.y; result.z = z - obj.z; return result; }
        vector3d operator * (vector3d const& obj) { vector3d result; result.x = x * obj.x; result.y = y * obj.y; result.z = z * obj.z; return result; }
        vector3d operator / (vector3d const& obj) { vector3d result; result.x = x / obj.x; result.y = y / obj.y; result.z = z / obj.z; return result; }

        template<class TYPE>
        vector3d<TYPE> cast_to() { return vector3d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y), static_cast<TYPE>(z) }; };
    };

    template<class TYPE>
    class vector4d {
    public:
        TYPE x, y, z, a;
        vector4d()                                { this->x = 0; this->y = 0; this->z = 0; this->a = 0; }
        vector4d(TYPE xyz)                        { this->x = xyz; this->y = xyz; this->z = xyz; }
        vector4d(TYPE x, TYPE y, TYPE z, TYPE a)  { this->x = x; this->y = y; this->z = z; this->a = a; }

        void setAll(TYPE xyz) { this->x = xyz; this->y = xyz; this->z = xyz; }

        vector4d operator + (vector4d const& obj) { vector4d result; result.x = x + obj.x; result.y = y + obj.y; result.z = z + obj.z; return result; }
        vector4d operator - (vector4d const& obj) { vector4d result; result.x = x - obj.x; result.y = y - obj.y; result.z = z - obj.z; return result; }
        vector4d operator * (vector4d const& obj) { vector4d result; result.x = x * obj.x; result.y = y * obj.y; result.z = z * obj.z; return result; }
        vector4d operator / (vector4d const& obj) { vector4d result; result.x = x / obj.x; result.y = y / obj.y; result.z = z / obj.z; return result; }

        template<class TYPE>
        vector4d<TYPE> cast_to() { return vector4d<TYPE>{ static_cast<TYPE>(x), static_cast<TYPE>(y), static_cast<TYPE>(z), static_cast<TYPE>(a) }; };
    };

    typedef vector2d<int>         vector2i;
    typedef vector2d<float>       vector2f;
    typedef vector2d<double>      vector2db;
    typedef vector2d<long double> vector2ld;
    typedef vector2d<uint16_t>    vector2i16t;
    typedef vector2d<uint8_t>     vector2di8t;

    typedef vector3d<int>         vector3i;
    typedef vector3d<float>       vector3f;
    typedef vector3d<long double> vector3ld;
    typedef vector3d<uint16_t>    vector3i16t;
    typedef vector3d<uint8_t>     vector3di8t;

    typedef vector4d<int>         vector4i;
    typedef vector4d<float>       vector4f;
    typedef vector4d<long double> vector4ld;
    typedef vector4d<uint16_t>    vector4i16t;
    typedef vector4d<uint8_t>     vector4di8t;

    class Color : public vector4di8t { public: Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { this->x = r; this->y = g; this->z = b; this->a = a; } };

    static const Color RED(204, 0, 0, 255), DARK_RED(102, 0, 0, 255), ORANGE(255, 128, 0, 255), BROWN(102, 51, 0, 255), DARK_BROWN(51, 25, 0, 255),
        GREEN(25, 51, 0, 255), LIME_GREEN(128, 255, 0, 255), FOREST_GREEN(0, 51, 0, 255), CYAN(0, 255, 255, 255), TEAL(0, 153, 153, 255);

    class SDL_GraphicsEngine {
    private:
        SDL_Renderer* renderer = nullptr;
        SDL_Window* window     = nullptr;
    public:
        SDL_GraphicsEngine() {}
        ~SDL_GraphicsEngine() {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        };

    public:
        bool done = false;
        std::string appName;
    public:
        virtual void OnUserCreate() = 0;
    public:
        void ConstructWindow(uint32_t width, uint32_t height) {
            SDL_Init(SDL_INIT_VIDEO);
            SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
            OnUserCreate();
            SDL_SetWindowTitle(window, appName.c_str());
        }
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
    public:
        void DrawPixel(Color color, uint32_t i, uint32_t j) {
            SetDrawColor(color);
            SDL_RenderDrawPoint(renderer, i, j);
        }
        void DrawLine(Color color, vector2i start, vector2i end) {
            SetDrawColor(color);
            SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
        }
        void DrawRectOutline(Color color, vector2i pos, vector2i size) {
            SetDrawColor(color);
            SDL_Rect rect = { pos.x, pos.y, size.x, size.y };
            SDL_RenderDrawRect(renderer, &rect);
        }
        void DrawRectFill(Color color, vector2i pos, vector2i size) {
            SetDrawColor(color);
            SDL_Rect rect = { pos.x, pos.y, size.x, size.y };
            SDL_RenderFillRect(renderer, &rect);
        }
    };
}