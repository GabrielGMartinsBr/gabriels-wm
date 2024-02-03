#include <memory>
namespace App {

typedef unsigned int uint;
typedef unsigned long ulong;

template <typename T>
using Pointer = std::unique_ptr<T>;

}  // namespace App