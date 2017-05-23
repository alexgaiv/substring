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

struct TestParams
{
    int num_iters;
    int start_len;
    int d_len;
    int substr_len;
};

template<class Func>
void TestRandomText(Func func, const TestParams &params, const char *title, mglGraph &gr, const char *color)
{
	typedef std::chrono::duration<double, std::milli> duration;
	duration elapsed;

    const int num_iters = params.num_iters;
    const int start_len = params.start_len;
    const int d_len = params.d_len;
    const int substr_len = params.substr_len;

    std::vector<double> dts(num_iters);
	
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
			int result = func(text, substr);
			elapsed = high_resolution_clock::now() - start;
            if (result != substr_pos) __debugbreak();
			dt += elapsed.count();
		}

		dts[i] = dt / 5.0;

		text_len += d_len;
		text += random_string(d_len);

		system("cls");
		cout << (double)i  / num_iters * 100 << '%';
	}

	mglData y;
	y.Set(dts);
    gr.AddLegend(title, color);
	gr.Plot(y, color);
}

template<class Func>
void TestRealText(Func func, const std::string &text, const TestParams &params, const char *title, mglGraph &gr, const char *color)
{
	typedef std::chrono::duration<double, std::milli> duration;
	duration elapsed;

	const int num_iters = params.num_iters;
	const int start_len = params.start_len;
	const int d_len = params.d_len;
    const int substr_len = params.substr_len;

    std::vector<double> dts(num_iters);
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

		system("cls");
		cout << (int)((double)i / num_iters * 100) << '%';
	}

    mglData y;
    y.Set(dts);
    gr.AddLegend(title, color);
    gr.Plot(y, color);
}

void BuiltRandomTextPlot()
{
    mglGraph gr;
    TestParams p;

    p.num_iters = 10;
    p.start_len = 10000000;
    p.d_len = 10000000;
    p.substr_len = 30;

    gr.Title("Random text");
    gr.SetRanges(p.start_len, p.start_len + p.num_iters * p.d_len, 60, 700);
    gr.Label('x', "text length");
    gr.Label('y', "time, ms");
    gr.Axis();

    auto rk_substr = [](const string &s, const string &ss) {
        return rabin_karp_substr(s, ss);
    };

    TestRandomText(naive_substr, p, "Naive", gr, "r");
    TestRandomText(kmp_substr, p, "KMP", gr, "g");
    //TestRandomText(rk_substr, p, "Rabin-Karp", gr, "b");

    gr.Legend();
    gr.WriteBMP("plot/rand.bmp");
}

void BuildRealTextPlot()
{
    ifstream file("sample.txt");
    if (!file) return;
    string text((std::istreambuf_iterator<char>)file, std::istreambuf_iterator<char>());
    file.close();

    mglGraph gr;
    TestParams p;

    p.num_iters = 20;
    p.start_len = 100000;
    p.d_len = (text.size() - p.start_len) / p.num_iters;
    p.substr_len = 500;

    gr.Title("Real text");
    gr.SetRanges(p.start_len, p.start_len + p.num_iters * p.d_len, 0, 100);
    gr.Label('x', "text length");
    gr.Label('y', "time, ms");
    gr.Axis();

    auto rk_substr = [](const string &s, const string &ss) {
        return rabin_karp_substr(s, ss);
    };

    TestRealText(naive_substr, text, p, "Naive", gr, "r");
    TestRealText(kmp_substr, text, p, "KMP", gr, "g");
    //TestRealText(rk_substr, text, p, "Rabin-Karp (real)", gr, "b");

    gr.Legend();
    gr.WriteBMP("plot/real.bmp");
}

int main()
{
	srand((unsigned)time(NULL));

    BuiltRandomTextPlot();
    BuildRealTextPlot();

	cout << "\nend";

	getchar();
	return 0;
}