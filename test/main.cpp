#include "littleGUI.h"

#include <iostream>

struct Vec2 {
  float x, y;
};

inline std::ostream& operator<<(std::ostream& stream, const Vec2& val)
{
  return stream << val.x << ", " << val.y;
}

int main() {
  LilArray<Vec2> vectors;

  for (float i = 0; i < 10; ++i)
  {
    vectors.PushBack({ i, i*i });
  }
  
  for (auto& val : vectors)
  {
    std::cout << val << std::endl;
  }

  return 0;
}