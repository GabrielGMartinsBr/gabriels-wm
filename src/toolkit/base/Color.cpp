#include "Color.h"

Color::Color(int hexValue)
{
  hex = hexValue;
  r = ((hexValue >> 16) & 0xFF) / 255.0;
  g = ((hexValue >> 8) & 0xFF) / 255.0;
  b = (hexValue & 0xFF) / 255.0;
  hexStr = hexToStr(hexValue);
}

Color::Color(const char* str)
{
  hexStr[0] = '#';
  if (std::strlen(str) == 7) {
    for (int i = 0; i < 6; i++) {
      hexStr[i + 1] = str[i + 1];
    }
  }
  if (std::strlen(str) == 4) {
    for (int i = 0; i < 3; i++) {
      hexStr[i * 2 + 1] = str[i + 1];
      hexStr[i * 2 + 2] = str[i + 1];
    }
  }
  hexStr[7] = '\0';

  hex = hexToDec(str);
  r = ((hex >> 16) & 0xFF) / 255.0;
  g = ((hex >> 8) & 0xFF) / 255.0;
  b = (hex & 0xFF) / 255.0;
}

Color::Color(int red, int green, int blue)
{
  hex = (red << 16) | (green << 8) | blue;
  r = red / 255.0;
  g = green / 255.0;
  b = blue / 255.0;
  hexStr = hexToStr(hex);
}

char* Color::hexToStr(int hexValue)
{
  std::stringstream stream;
  stream << std::hex << std::setw(6) << std::setfill('0') << hexValue;
  std::string str = stream.str();
  char* cStr = new char[8];
  cStr[0] = '#';
  for (int i = 0; i < 6; i++) {
    cStr[i + 1] = str[i + 1];
  }
  cStr[7] = '\0';
  return cStr;
}

int Color::hexToDec(const char* hexStr)
{
  std::string str = std::string(hexStr).substr(1);
  int decimalValue;
  std::stringstream stream;
  stream << std::hex << str;
  stream >> decimalValue;
  return decimalValue;
}