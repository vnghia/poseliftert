#pragma once

#include "poseliftert/config.hpp"

namespace poseliftert {

void normalization(float* normalized_ptr, const float* const input_ptr,
                   unsigned int width, unsigned int height);

}  // namespace poseliftert
