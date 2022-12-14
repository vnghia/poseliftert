#include "poseliftert/input/from_body25.hpp"

#include <array>

#include "poseliftert/config.hpp"

namespace poseliftert {

static constexpr std::array<std::array<unsigned int, 2>, 14> BODY_25_INDEX_MAP =
    {{{0, 8},
      {1, 9},
      {2, 10},
      {3, 11},
      {4, 12},
      {5, 13},
      {6, 14},
      {9, 1},
      {11, 5},
      {12, 6},
      {13, 7},
      {14, 2},
      {15, 3},
      {16, 4}}};

inline float normalized(float input, float offset, float scale) {
  return (input - offset) / scale;
}

void from_body25(float* normalized_ptr, const float* const input_ptr,
                 unsigned int width, unsigned int height) {
  const auto scale = 0.5f * width;
  const auto offset_width = scale;
  const auto offset_height = 0.5f * height;

  auto l_shoulder_index = 5 * kNum2dDim;
  auto r_shoulder_index = 2 * kNum2dDim;
  auto thorax_x =
      (input_ptr[l_shoulder_index] + input_ptr[r_shoulder_index]) / 2;
  auto thorax_y =
      (input_ptr[l_shoulder_index + 1] + input_ptr[r_shoulder_index + 1]) / 2;

  {
    auto normalized_index = 7 * kNum2dDim;
    auto hip_index = 8 * kNum2dDim;
    normalized_ptr[normalized_index] =
        normalized((input_ptr[hip_index] + thorax_x) / 2, offset_width, scale);
    normalized_ptr[normalized_index + 1] = normalized(
        (input_ptr[hip_index + 1] + thorax_y) / 2, offset_height, scale);
  }

  {
    auto normalized_index = 8 * kNum2dDim;
    normalized_ptr[normalized_index] =
        normalized(thorax_x, offset_width, scale);
    normalized_ptr[normalized_index + 1] =
        normalized(thorax_y, offset_height, scale);
  }

  {
    auto normalized_index = 10 * kNum2dDim;
    auto l_eye_index = 16 * kNum2dDim;
    auto r_eye_index = 15 * kNum2dDim;
    normalized_ptr[normalized_index] =
        normalized((input_ptr[l_eye_index] + input_ptr[r_eye_index]) / 2,
                   offset_width, scale);
    normalized_ptr[normalized_index + 1] = normalized(
        (input_ptr[l_eye_index + 1] + input_ptr[r_eye_index + 1]) / 2,
        offset_height, scale);
  }

  for (const auto& idx_pair : BODY_25_INDEX_MAP) {
    auto normalized_index = idx_pair[0] * kNum2dDim;
    auto input_index = idx_pair[1] * kNum2dDim;
    normalized_ptr[normalized_index] =
        normalized(input_ptr[input_index], offset_width, scale);
    normalized_ptr[normalized_index + 1] =
        normalized(input_ptr[input_index + 1], offset_height, scale);
  }
}

}  // namespace poseliftert
