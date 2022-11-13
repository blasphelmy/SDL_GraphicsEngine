#include <iostream>
#include <string> 
#include "SDL_GraphicsEngine.h"
#include <condition_variable>
#include <atomic>
#include <complex>
#include <cstdlib>
#include <immintrin.h>
#undef main

static const uint32_t WINDOW_WIDTH  = 1280;
static const uint32_t WINDOW_HEIGHT = 720;

//Example usage / testing

class MandleBrotSet : public blsp::SDL_GraphicsEngine {
public:
    MandleBrotSet() {
        appName = "MandleBrotSet";
    }
public:
    int iterations			= 128;
    int* pFractal			= nullptr;
	blsp::vector2i pix_tl	= blsp::vector2i(0, 0);
	blsp::vector2i pix_br	= blsp::vector2i(WINDOW_WIDTH, WINDOW_HEIGHT);
	blsp::vector2db frac_tl = blsp::vector2db(-2.0, -1.0);
	blsp::vector2db frac_br = blsp::vector2db(1.0, 1.0);
public:
    void OnUserCreate() override {
        pFractal = (int*)_aligned_malloc(size_t(WINDOW_WIDTH) * size_t(WINDOW_HEIGHT) * sizeof(int), 128);
    }
    bool OnUserUpdate(float elaspedTimeMillS) override {
		ClearScreen(blsp::Color(0, 0, 0, 0));

		if (mouseButtonStates.scrolledUp) {
			frac_tl = frac_tl - blsp::vector2db(frac_tl.x * .1, frac_tl.y * .1);
			frac_br = frac_br - blsp::vector2db(frac_br.x * .1, frac_br.y * .1);
		}
		if (mouseButtonStates.scrolledDown) {
			frac_tl = frac_tl + blsp::vector2db(frac_tl.x * .1, frac_tl.y * .1);
			frac_br = frac_br + blsp::vector2db(frac_br.x * .1, frac_br.y * .1);
		}
		if (GetKey(blsp::KEYBOARD::Q).pressed) {
			iterations -= 16;
		}
		if (GetKey(blsp::KEYBOARD::W).pressed) {
			iterations += 16;
		}
		if (GetKey(blsp::ARROW_KEYS::LEFT_ARROW).pressed) {
			frac_tl = frac_tl - blsp::vector2db(.02, 0);
			frac_br = frac_br - blsp::vector2db(.02, 0);
		}
		if (GetKey(blsp::ARROW_KEYS::RIGHT_ARROW).pressed) {
			frac_tl = frac_tl + blsp::vector2db(.02, 0);
			frac_br = frac_br + blsp::vector2db(.02, 0);
		}
		if (GetKey(blsp::ARROW_KEYS::UP_ARROW).pressed) {
			frac_tl = frac_tl - blsp::vector2db(0, .02);
			frac_br = frac_br - blsp::vector2db(0, .02);
		}
		if (GetKey(blsp::ARROW_KEYS::DOWN_ARROW).pressed) {
			frac_tl = frac_tl + blsp::vector2db(0, .02);
			frac_br = frac_br + blsp::vector2db(0, .02);
		}
        createMandleBrot(pix_tl, pix_br, frac_tl, frac_br, iterations);
		 
		RenderPixels();

        DrawString(blsp::DARK_RED, std::to_string(elaspedTimeMillS / 1000.f), blsp::vector2i(0, 0));
        std::string mousePosition = std::to_string(mouseButtonStates.mousePOS.x) + ", " + std::to_string(mouseButtonStates.mousePOS.y);
        DrawString(blsp::DARK_RED, std::to_string(iterations), blsp::vector2i(0, 16));
        RenderScreen();
        return true;
    }
    bool OnUserDestroy() override {
        _aligned_free(pFractal);
        return true;
    }
	//ehh thanks again javid
    void createMandleBrot(const blsp::vector2i pix_tl, const blsp::vector2i pix_br, const blsp::vector2db frac_tl, const blsp::vector2db frac_br, const int iterations) {
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
		double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

		double y_pos = frac_tl.y;

		int y_offset = 0;
		int row_size = WINDOW_WIDTH;

		int x, y;

		__m256d _a, _b, _two, _four, _mask1;
		__m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
		__m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
		__m256i _one, _c, _n, _iterations, _mask2;

		_one = _mm256_set1_epi64x(1);
		_two = _mm256_set1_pd(2.0);
		_four = _mm256_set1_pd(4.0);
		_iterations = _mm256_set1_epi64x(iterations);

		_x_scale = _mm256_set1_pd(x_scale);
		_x_jump = _mm256_set1_pd(x_scale * 4);
		_x_pos_offsets = _mm256_set_pd(0, 1, 2, 3);
		_x_pos_offsets = _mm256_mul_pd(_x_pos_offsets, _x_scale);


		for (y = pix_tl.y; y < pix_br.y; y++)
		{
			// Reset x_position
			_a = _mm256_set1_pd(frac_tl.x);
			_x_pos = _mm256_add_pd(_a, _x_pos_offsets);

			_ci = _mm256_set1_pd(y_pos);

			for (x = pix_tl.x; x < pix_br.x; x += 4)
			{
				_cr = _x_pos;
				_zr = _mm256_setzero_pd();
				_zi = _mm256_setzero_pd();
				_n = _mm256_setzero_si256();


			repeat:
				_zr2 = _mm256_mul_pd(_zr, _zr);
				_zi2 = _mm256_mul_pd(_zi, _zi);
				_a = _mm256_sub_pd(_zr2, _zi2);
				_a = _mm256_add_pd(_a, _cr);
				_b = _mm256_mul_pd(_zr, _zi);
				_b = _mm256_fmadd_pd(_b, _two, _ci);
				_zr = _a;
				_zi = _b;
				_a = _mm256_add_pd(_zr2, _zi2);
				_mask1 = _mm256_cmp_pd(_a, _four, _CMP_LT_OQ);
				_mask2 = _mm256_cmpgt_epi64(_iterations, _n);
				_mask2 = _mm256_and_si256(_mask2, _mm256_castpd_si256(_mask1));
				_c = _mm256_and_si256(_one, _mask2); // Zero out ones where n < iterations													
				_n = _mm256_add_epi64(_n, _c); // n++ Increase all n
				if (_mm256_movemask_pd(_mm256_castsi256_pd(_mask2)) > 0)
					goto repeat;

				pFractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
				pFractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
				pFractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
				pFractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
				_x_pos = _mm256_add_pd(_x_pos, _x_jump);
			}

			y_pos += y_scale;
			y_offset += row_size;
		}

		for (int y = 0; y < WINDOW_HEIGHT; y++)
		{
			for (int x = 0; x < WINDOW_WIDTH; x++)
			{
				int i = pFractal[y * WINDOW_WIDTH + x];
				float n = (float)i;
				float a = 0.1f;
				// Thank you @Eriksonn - Wonderful Magic Fractal Oddball Man
				DrawPixel(blsp::ColorF(128 * sin(a * n) + 128, 128 * sin(a * n + 2.094f) + 128, 128 * sin(a * n + 4.188f) + 128, 255), x, y);
			}
		}
    }
};

int main() {
    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}