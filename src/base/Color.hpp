#pragma once

#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>

#include "./types.h"

namespace App {
/**
    @brief Represents a color with RGB attributes.
    The RGB attributes can be set using the following methods:
        r(uint value): Set the red channel value (0 to 255).
        g(uint value): Set the green channel value (0 to 255).
        b(uint value): Set the blue channel value (0 to 255).
        rgb(uint r, uint g, uint b): Set all RGB channels at once.

    Additionally, alternative methods are provided for setting color channels
    using floating-point values ranging from 0.0 to 1.0:
        red(float value): Set the red channel value (0.0 to 1.0).
        green(float value): Set the green channel value (0.0 to 1.0).
        blue(float value): Set the blue channel value (0.0 to 1.0).
        redGreenBlue(float red, float green, float blue): Set all RGB channels at once.

    The color attributes follow the convention where RGB values range from 0 to 255
    for integer format and from 0.0 to 1.0 for floating-point format.
*/
class Color {
 public:
  Color() { }

  Color(const char* colorStr)
  {
    parseStr(colorStr);
  }

  Color(ulong hex)
  {
    parseHex(hex);
  }

  Color& setColor(uint r, uint g, uint b)
  {
    _r = r;
    _g = g;
    _b = b;
    _red = _r / 255.0;
    _green = _g / 255.0;
    _blue = _b / 255.0;
    return *this;
  }

  Color& setColor(float red, float green, float blue)
  {
    _red = red;
    _green = green;
    _blue = blue;
    _r = _red * 255;
    _g = _green * 255;
    _b = _blue * 255;

    return *this;
  }

  Color& setColor(ulong hex)
  {
    parseHex(hex);
    return *this;
  }

  Color& setColor(const char* str)
  {
    parseStr(str);
    return *this;
  }

  Color& r(uint value)
  {
    _r = value;
    _red = _r / 255.0;
    return *this;
  }

  Color& g(uint value)
  {
    _g = value;
    _green = _g / 255.0;
    return *this;
  }

  Color& b(uint value)
  {
    _b = value;
    _blue = _b / 255.0;
    return *this;
  }

  Color& red(float value)
  {
    _red = value;
    _r = _red * 255;
    return *this;
  }

  Color& green(float value)
  {
    _green = value;
    _g = _green * 255;
    return *this;
  }

  Color& blue(float value)
  {
    _blue = value;
    _b = _blue * 255;
    return *this;
  }

  float red()
  {
    return this->_red;
  }

  float green()
  {
    return this->_green;
  }

  float blue()
  {
    return this->_blue;
  }

  uint r()
  {
    return this->_r;
  }

  uint g()
  {
    return this->_g;
  }

  uint b()
  {
    return this->_b;
  }

 private:
  uint _r = 0;
  uint _g = 0;
  uint _b = 0;
  float _red = 0;
  float _green = 0;
  float _blue = 0;

  void parseStr(const char* colorStr)
  {
    // Prepare Str
    char str[7] = "";
    if (std::strlen(colorStr) == 4) {
      for (int i = 0; i < 6; i++) {
        str[i] = colorStr[(i + 2) / 2];
      }
    } else if (std::strlen(colorStr) == 7) {
      for (int i = 0; i < 6; i++) {
        str[i] = colorStr[i + 1];
      }
    } else {
      throw std::runtime_error("Invalid color string size.");
    }

    // Convert to decimal
    char* endPtr;
    int hex = std::strtol(str, &endPtr, 16);

    if (*endPtr != '\0') {
      std::cerr << "Error: Invalid hexadecimal string." << std::endl;
      throw std::runtime_error("Color hex string conversion failed.");
    }

    // Parse color chanels
    parseHex(hex);
  }

  void parseHex(int hex)
  {
    _r = ((hex >> 16) & 0xFF);
    _g = ((hex >> 8) & 0xFF);
    _b = (hex & 0xFF);

    _red = _r / 255.0;
    _green = _g / 255.0;
    _blue = _b / 255.0;
  }
};

}  // namespace App