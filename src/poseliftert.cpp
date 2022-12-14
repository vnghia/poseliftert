#include "poseliftert/poseliftert.hpp"

#include "poseliftert/config.hpp"
#include "poseliftert/input/input.hpp"
#include "poseliftert/output/output.hpp"
#include "spdlog/spdlog.h"

namespace poseliftert {

Poseliftert::Poseliftert(const fs::path& engine_path, unsigned int width,
                         unsigned int height, float* body25_output_ptr)
    : engine_(Engine::engine_from_path(engine_path)),
      width(width),
      height(height) {
  malloc_memory(body25_output_ptr);
}

void Poseliftert::malloc_memory(float* body25_output_ptr) {
  engine_.create_device_buffer(
      {{engine_.get_input_name(0), Engine::malloc_mode::managed},
       {engine_.get_output_name(0), Engine::malloc_mode::managed}});

  spdlog::info("malloc memory for Poseliftert");

  const bool use_body25_output = (body25_output_ptr != nullptr);
  if (!use_body25_output) {
    auto input_size = pose_2d_dim * kNum2DJoints;
    input_data_.reset(new float[input_size]);
    input_ptr_ = input_data_.get();
    spdlog::info("allocated {} byte for image input data dims=[{}, {}]",
                 input_size * sizeof(float), kNum2DJoints, pose_2d_dim);
  } else {
    input_ptr_ = body25_output_ptr;
    spdlog::info("use BODY_25 output from Openpose at {}",
                 fmt::ptr(input_ptr_));
  }

  net_input_data_ =
      std::static_pointer_cast<float[]>(engine_.get_input_device_owned_ptr(0));
  spdlog::info("use engine input buffer at {} for net input data",
               fmt::ptr(net_input_data_.get()));

  net_output_data_ =
      std::static_pointer_cast<float[]>(engine_.get_output_device_owned_ptr(0));
  spdlog::info("use engine output buffer at {} for net output data",
               fmt::ptr(net_output_data_.get()));

  input_ = Input(input_ptr_, width, height, net_input_data_.get(),
                 use_body25_output);

  auto output_size = pose_3d_dim * kNum3DJoints;
  output_data_.reset(new float[output_size]);
  spdlog::info("allocated {} byte for output data dims=[{}, {}]",
               output_size * sizeof(float), kNum3DJoints, pose_3d_dim);

  output_ = Output(net_output_data_.get(), output_data_.get());
}

void Poseliftert::forward() {
  input_.process();

  engine_.forward();
  cudaDeviceSynchronize();

  output_.process();
}

}  // namespace poseliftert
