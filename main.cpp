#include "substring.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <mgl2\mgl.h>

using std::string;
using std::ifstream;
using std::cout;
using std::chrono::high_resolution_clock;

string random_string(size_t size)
{
	string s;
	s.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		char c = rand() % 51;
		c += (c < 26) ? 'a' : 'A' - 26;
		s[i] = c;
	}

	return s;
}

template<class Func>
void test_random_text(Func func, const char *filename, const char *title)
{
	typedef std::chrono::duration<double, std::milli> duration;
	duration elapsed;

	const int num_iters = 100;
	const int start_len = 100000;
	const int d_len = 100000;
	double dts[num_iters] = { };

	int substr_len = 500;
	int text_len = start_len;

	string text = random_string(text_len);

	for (int i = 0; i < num_iters; i++)
	{
		int substr_pos = text_len - substr_len;
		string substr = text.substr(substr_pos, substr_len);

		double dt = 0.0;

		for (int k = 0; k < 5; k++)
		{
			auto start = high_resolution_clock::now();
			func(text, substr);
			elapsed = high_resolution_clock::now() - start;
			dt += elapsed.count();
		}

		dts[i] = dt / 5.0;

		text_len += d_len;
		text += random_string(d_len);

		system("cls");
		cout << (double)i  / num_iters * 100 << '%';
	}

	mglGraph gr;
	mglData y;
	y.Set(dts, num_iters);

    gr.Title(title);
	gr.SetRanges(start_len, text_len, 1, dts[num_iters - 1]);
	gr.Label('x', "text length");
	gr.Label('y', "time, ms");
	gr.Axis();
	gr.Plot(y);
	gr.WriteBMP(filename);
}

template<class Func>
void test_real_text(Func func, const char *filename, const char *title)
{
	typedef std::chrono::duration<double, std::milli> duration;
	duration elapsed;

	ifstream file("sample.txt");
	if (!file) return;
	
	string text((std::istreambuf_iterator<char>)file, std::istreambuf_iterator<char>());
	file.close();

	const int num_iters = 30;
	const int max_len = (int)text.size();
	const int start_len = 100000;
	const int d_len = (max_len - start_len) / num_iters;

	double dts[num_iters] = { };

	int substr_len = 500;
	int text_len = start_len;

	for (int i = 0; i < num_iters; i++)
	{
		int substr_pos = text_len - substr_len;
		string substr = text.substr(substr_pos, substr_len);

		double dt = 0.0;

		for (int k = 0; k < 5; k++)
		{
			auto start = high_resolution_clock::now();
			func(text, substr);
			elapsed = high_resolution_clock::now() - start;
			dt += elapsed.count();
		}

		dts[i] = dt / 5.0;

		text_len += d_len;
		text += random_string(d_len);

		system("cls");
		cout << (int)((double)i / num_iters * 100) << '%';
	}

	mglGraph gr;
	mglData y;
	y.Set(dts, num_iters);

    gr.Title(title);
	gr.SetRanges(start_len, text_len, 1, dts[num_iters - 1]);
	gr.Label('x', "text length");
	gr.Label('y', "time, ms");
	gr.Axis();
	gr.Plot(y);
	gr.WriteBMP(filename);
}

int main()
{
	srand((unsigned)time(NULL));

	auto rk_substr = [](const string &s, const string &ss) {
		return rabin_karp_substr(s, ss);
	};

	test_random_text(naive_substr, "plot/naive_rand.bmp", "Naive (random)");
	test_random_text(kmp_substr, "plot/kmp_rand.bmp", "KMP (random)");
	test_random_text(rk_substr, "plot/rk_rand.bmp", "Rabin-Karp (random)");
	test_real_text(naive_substr, "plot/naive_real.bmp", "Naive (real)");
	test_real_text(kmp_substr, "plot/kmp_real.bmp", "KMP (real)");
	test_real_text(rk_substr, "plot/rk_real.bmp", "Rabin-Karp (real)");

	cout << "\nend";

	getchar();
	return 0;
}