#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Utils.hpp"

const char* getScore(const char* str, int score)
{
	std::string s = std::to_string(score);
	s = str + s;
	return s.c_str();
}

const char* getScore(int score)
{
	std::string s = std::to_string(score);
    size_t n = 6;
    int precision = n - std::min(n, s.size());
    s.insert(0, precision, '0');
    return s.c_str();
}

const char* getScore(int score, int totalscore)
{
	float scr = std::max(0.0, (float)score/totalscore*100.0);
    std::string s = std::to_string(scr);
    s.erase(s.begin()+5,s.end());
    s += "%";
	return s.c_str();
}

const char* getScore(int n300, int n100, int n0)
{
	float acc = std::max(0.0, (float)(n300*300+n100*100)/((n300+n100+n0)*300)*100.0);
    std::string s = std::to_string(acc);
    s.erase(s.begin()+5,s.end());
    s += "%";
	return s.c_str();
}

int getRank(int n300, int n100, int n0)
{
    float acc = std::max(0.0, (float)(n300*300+n100*100)/((n300+n100+n0)*300)*100.0);
    // 0 = S, 1 = A, 2 = B, 3 = C, 4 = D
    if (acc > 97.0) return 0;
    if (acc > 90.0) return 1;
    if (acc > 80.0) return 2;
    if (acc > 60.0) return 3;
    return 4;
}

int getRank(float acc) {
    if (acc > 97.0) return 0;
    if (acc > 90.0) return 1;
    if (acc > 80.0) return 2;
    if (acc > 60.0) return 3;
    return 4;
}

std::vector<const char*> getText(const char* str, int number, const char* str1)
{
    std::string s = std::to_string(number);
    size_t n = 3;
    int precision = n - std::min(n, s.size());
    s.insert(0, precision, '0');
    std::string result_str = str + s + str1;

    // Allocate memory for const char*
    const char* result_cstr = new char[result_str.size() + 1];

    // Copy string to const char*
    std::strcpy(const_cast<char*>(result_cstr), result_str.c_str());

    // Create vector of const char*
    std::vector<const char*> result_vector {result_cstr};

    return result_vector;
}

std::string getTextString(const char* str, int number, const char* str1)
{
    std::string s = std::to_string(number);
    size_t n = 3;
    int precision = n - std::min(n, s.size());
    s.insert(0, precision, '0');
    std::string result_str = str + s + str1;

    return result_str;
}

/*int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		std::cout << "Mouse X: " << mouseX << ", Mouse Y: " << mouseY << std::endl;*/

