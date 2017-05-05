#ifndef _KMP_H_
#define _KMP_H_

#include <string>

int naive_substr(const std::string &str, const std::string &substr);

int kmp_substr(const std::string &str, const std::string &substr);

int rabin_karp_substr(
	const std::string &str, const std::string &substr,
	int q = 101);

#endif // _KMP_H_