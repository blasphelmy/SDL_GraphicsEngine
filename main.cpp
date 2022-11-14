#include <iostream>
#include <string> 
#include "SDL_GraphicsEngine.h"
#include <condition_variable>
#include <atomic>
#include <complex>
#include <cstdlib>
#include <immintrin.h>
#undef main

static const uint32_t WINDOW_WIDTH  = 1024;
static const uint32_t WINDOW_HEIGHT = 768;
static const int maxNumberOfThreads = 64;

//Example usage / testing
//copied and adapted from : https://github.com/OneLoneCoder/Javidx9/blob/master/PixelGameEngine/SmallerProjects/OneLoneCoder_PGE_Mandelbrot.cpp
//Formal thank you
class MandleBrotSet : public blsp::SDL_GraphicsEngine {
public:
    MandleBrotSet() {
        appName = "MandleBrotSet";
    }
public:
	struct WorkerThread
	{
		std::condition_variable cvStart;
		blsp::vector2i pix_tl = blsp::vector2i(0, 0);
		blsp::vector2i pix_br = blsp::vector2i(0, 0);
		blsp::vector2db frac_tl = blsp::vector2db(0, 0);
		blsp::vector2db frac_br = blsp::vector2db(0, 0);
		int iterations = 0;
		bool alive = true;
		std::mutex mux;
		int screen_width = 0;
		int* fractal = nullptr;
		std::thread thread;


		void Start(const blsp::vector2i ptl, const blsp::vector2i pbr, const blsp::vector2db ftl, const blsp::vector2db fbr, const int it)
		{
			pix_tl = ptl;
			pix_br = pbr;
			frac_tl = ftl;
			frac_br = fbr;
			iterations = it;
			std::unique_lock<std::mutex> lm(mux);
			cvStart.notify_one();
		}

		void CreateFractal()
		{
			while (alive)
			{
				std::unique_lock<std::mutex> lm(mux);
				cvStart.wait(lm);

				double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
				double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

				double y_pos = frac_tl.y;

				int y_offset = 0;
				int row_size = screen_width;

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

						fractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
						fractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
						fractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
						fractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
						_x_pos = _mm256_add_pd(_x_pos, _x_jump);
					}

					y_pos += y_scale;
					y_offset += row_size;
				}
				nWorkerComplete++;
			}
		}
	};
public:
	static std::atomic<int> nWorkerComplete;

    int iterations							= 64;
    int* pFractal							= nullptr;
	// Pan & Zoom variables
	blsp::vector2db vOffset					= blsp::vector2db(-1.5, -0.5);
	blsp::vector2db vStartPan				= blsp::vector2db(0.0, 0.0);
	blsp::vector2db vScale					= blsp::vector2db(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT);

	WorkerThread threadPool[maxNumberOfThreads];
public:
	void InitialiseThreadPool()
	{
		for (int i = 0; i < maxNumberOfThreads; i++)
		{
			threadPool[i].alive = true;
			threadPool[i].fractal = pFractal;
			threadPool[i].screen_width = WINDOW_WIDTH;
			threadPool[i].thread = std::thread(&WorkerThread::CreateFractal, &threadPool[i]);
		}
	}
    void OnUserCreate() override {
        pFractal = (int*)_aligned_malloc(size_t(windowWidth) * size_t(WINDOW_HEIGHT) * sizeof(int), 128);
		InitialiseThreadPool();
    }
    bool OnUserUpdate(float elaspedTimeMillS) override {
		ClearScreen(blsp::Color(0, 0, 0, 0));

		blsp::vector2db mousePOS = mouseButtonStates.mousePOS.cast_to<double>();

		if (GetMouseState(blsp::MOUSE_EVENTS::LEFT_CLICK).pressed)
		{
			//vStartPan = mousePOS;
			vOffset =  vOffset - ((mousePOS - vStartPan) / vScale);
			vStartPan = mousePOS;
		}
		if (GetMouseState(blsp::MOUSE_EVENTS::LEFT_CLICK).released)
		{
			vStartPan = mousePOS;
		}
		
		blsp::vector2db vMouseBeforeZoom = ScreenToWorld(mousePOS.cast_to<int>());
		
		if (mouseButtonStates.scrolledUp || GetKey(blsp::W).pressed) {
			vScale *= 1.1;
		}
		if (mouseButtonStates.scrolledDown || GetKey(blsp::Q).pressed) {
			vScale *= .9;
		}

		blsp::vector2db vMouseAfterZoom = ScreenToWorld(mousePOS.cast_to<int>());
		vOffset = vOffset + (vMouseBeforeZoom - vMouseAfterZoom);

		blsp::vector2i pix_tl(0, 0);
		blsp::vector2i pix_br(windowWidth, windowHeight);
		blsp::vector2db frac_tl(-2.0, -1.0);
		blsp::vector2db frac_br(1.0, 1.0);

		frac_tl = ScreenToWorld(pix_tl);
		frac_br = ScreenToWorld(pix_br);

		if (GetKey(blsp::KEYBOARD::NUM_1).pressed) {
			iterations -= 16;
		}
		if (GetKey(blsp::KEYBOARD::NUM_2).pressed) {
			iterations += 16;
		}
		CreateFractalThreadPool(pix_tl, pix_br, frac_tl, frac_br, iterations);
		for (int y = 0; y < windowHeight; y++)
		{
			for (int x = 0; x < windowWidth; x++)
			{
				int i = pFractal[y * windowWidth + x];
				float n = (float)i;
				float a = 0.1f;
				// Thank you @Eriksonn - Wonderful Magic Fractal Oddball Man
				DrawPixel(blsp::ColorF(128 * sin(a * n) + 128, 128 * sin(a * n + 2.094f) + 128, 128 * sin(a * n + 4.188f) + 128, 255), x, y);
			}
		}
		RenderPixels();

        DrawString(blsp::DARK_RED, std::to_string(elaspedTimeMillS / 1000.f), blsp::vector2i(0, 0));
        DrawString(blsp::DARK_RED, std::to_string(iterations), blsp::vector2i(0, 16));
		DrawString(blsp::DARK_BROWN, "MousePOS: " +  std::to_string(mousePOS.x) + " " + std::to_string(mousePOS.y), blsp::vector2i(0, 32));
		DrawString(blsp::DARK_BROWN, "VScale: " + std::to_string(vScale.x) + " " + std::to_string(vScale.y), blsp::vector2i(0, 48));
		DrawString(blsp::DARK_BROWN, "vOffset: " + std::to_string(vOffset.x) + " " + std::to_string(vOffset.y), blsp::vector2i(0, 96));
		DrawString(blsp::DARK_BROWN, "pixel_tl: " + std::to_string(pix_tl.x) + " " + std::to_string(pix_tl.y), blsp::vector2i(0, 64));
		DrawString(blsp::DARK_BROWN, "pixel_br: " + std::to_string(pix_br.x) + " " + std::to_string(pix_br.y), blsp::vector2i(0, 80));
        RenderScreen();
        return true;
    }
    bool OnUserDestroy() override {
        _aligned_free(pFractal);
        return true;
    }

	// Convert coordinates from Screen Space --> World Space
	blsp::vector2db ScreenToWorld(blsp::vector2i n)
	{
		blsp::vector2db v(0.0, 0.0);
		v.x = ((double)n.x / vScale.x) + vOffset.x;
		v.y = ((double)n.y / vScale.y) + vOffset.y;
		return v;
	}

	void CreateFractalThreadPool(const blsp::vector2i pix_tl, const blsp::vector2i pix_br, const blsp::vector2db frac_tl, const blsp::vector2db frac_br, const int iterations)
	{
		int nSectionWidth = (pix_br.x - pix_tl.x) / maxNumberOfThreads;
		double dFractalWidth = (frac_br.x - frac_tl.x) / double(maxNumberOfThreads);
		nWorkerComplete = 0;

		for (size_t i = 0; i < maxNumberOfThreads; i++)
			threadPool[i].Start(
				blsp::vector2i(pix_tl.x + nSectionWidth * i, pix_tl.y),
				blsp::vector2i(pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
				blsp::vector2db(frac_tl.x + dFractalWidth * double(i), frac_tl.y),
				blsp::vector2db(frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
				iterations);


		while (nWorkerComplete < maxNumberOfThreads) // Wait for all workers to complete
		{
		}
	}
};
std::atomic<int> MandleBrotSet::nWorkerComplete = 0;
int main() {
    MandleBrotSet mdset;
    mdset.ConstructWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	return 0;
}