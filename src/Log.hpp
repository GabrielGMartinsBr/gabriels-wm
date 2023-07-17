#pragma once

#include <iostream>
#include <sstream>

class Log {
  struct Out {
    std::ostringstream* stream = nullptr;

    template <typename T>
    Out& operator<<(const T& value)
    {
      if (stream == nullptr) {
        stream = new std::ostringstream;
      }
      (*stream) << value;
      return *this;
    }

    ~Out()
    {
      std::cout << (*stream).str() << std::endl;
      delete stream;
    }
  };

  struct Err {
    std::ostringstream* stream = nullptr;

    template <typename T>
    Err& operator<<(const T& value)
    {
      if (stream == nullptr) {
        stream = new std::ostringstream;
      }
      (*stream) << value;
      return *this;
    }

    ~Err()
    {
      std::cerr << (*stream).str() << std::endl;
      delete stream;
    }
  };

 public:
  static Out out() { return Out(); }
  static Err err() { return Err(); }
};
