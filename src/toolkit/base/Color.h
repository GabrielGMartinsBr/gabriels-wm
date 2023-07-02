#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

struct Color {
  char* hexStr;
  int hex;
  double r;
  double g;
  double b;

  Color(int hex);
  Color(const char* str);
  Color(int r, int g, int b);

  static char* hexToStr(int hexValue);
  static int hexToDec(const char* hexStr);
};
