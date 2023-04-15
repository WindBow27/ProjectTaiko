#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

const char* getScore(const char* str, int score);
const char* getScore(int score);
const char* getScore(int score, int totalscore);
const char* getScore(int n300, int n100, int n0);
int getRank(int n300, int n100, int n0);
std::vector<const char*> getText(const char* str, int number, const char* str1);
std::string getTextString(const char* str, int number, const char* str1);

