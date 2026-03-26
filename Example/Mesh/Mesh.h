#pragma once
#include <vector>


namespace Mesh{
  std::vector<float> vertices = {
    -1.0f,  1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 1.0f,
    1.0f,  1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f, 1.0f,
    -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f, 1.0f,
    1.0f,  1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
  };

  std::vector<unsigned int> indicies = {
    0, 1, 2,
    1, 3, 2,
    4, 5, 6,
    5, 7, 6,
    0, 1, 4,
    1, 5, 4,
    2, 3, 6,
    3, 7, 6,
    0, 2, 4,
    2, 6, 4,
    1, 3, 5,
    3, 7, 5
  };

  std::vector<float> colors = {
    0.1f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f,  1.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    0.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
  };

};
