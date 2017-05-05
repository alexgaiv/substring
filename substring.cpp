#include "substring.h"

#include <vector>
#include <string>

using std::vector;
using std::string;
using std::size_t;

vector<int> prefix_function(const string &str)
{
	size_t n = str.size();
	vector<int> pi(n);

	pi[0] = 0;
	for (size_t i = 1; i < n; i++)
	{
		int j = pi[i - 1];
		while (j > 0 && str[i] != str[j])
			j = pi[j - 1];

		if (str[i] == str[j]) j++;
		pi[i] = j;
	}
	return pi;
}

int naive_substr(const string &str, const string &substr)
{
	size_t n = str.size();
	size_t m = substr.size();

	for (size_t i = 0; i < n - m + 1; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			if (str[i + j] != substr[j])
				break;
			else if (j == m - 1)
				return i;
		}
	}

	return -1;
}

int kmp_substr(const string &str, const string &substr)
{
	vector<int> pi = prefix_function(substr);

	size_t j = 0;
	size_t n = str.size();
	size_t m = substr.size();

	for (size_t i = 0; i < n; i++)
	{
		while (j > 0 && str[i] != substr[j]) {
			j = pi[j - 1];
		}

		if (str[i] == substr[j]) j++;

		if (j == m) {
			return i - m + 1;
		}
	}

	return -1;
}

int rabin_karp_substr(const std::string &str, const std::string &substr, int q)
{
	size_t n = str.size();
	size_t m = substr.size();

	const int d = 256;
	int h = 1;
	for (size_t i = 0; i < m - 1; i++) {
		h = (h * d) % q;
	}
	int hsubstr = 0;
	int hstr = 0;

	for (size_t i = 0; i < m; i++)
	{
		hsubstr = (hsubstr * d + substr[i]) % q;
		hstr = (hstr * d + str[i]) % q;
	}

	for (size_t i = 0; i < n - m + 1; i++)
	{
		if (hsubstr == hstr &&
			substr == str.substr(i, m))
		{
			return i;
		}

		hstr = (d * (hstr - str[i]*h) + str[i + m]) % q;
		if (hstr < 0) hstr += q;
	}

	return -1;
}
