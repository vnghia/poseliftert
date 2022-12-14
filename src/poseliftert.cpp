#include "poseliftert/poseliftert.hpp"

#include "poseliftert/config.hpp"
#include "poseliftert/input/input.hpp"
#include "spdlog/spdlog.h"

namespace poseliftert {

Poseliftert::Poseliftert(const fs::path& engine_path, unsigned int width,
                         unsigned int height)
    : engine_(Engine::engine_from_path(engine_path)),
      width(width),
      height(height) {
  malloc_memory();
}

void Poseliftert::malloc_memory() {
  engine_.create_device_buffer(
      {{engine_.get_input_name(0), Engine::malloc_mode::managed},
       {engine_.get_output_name(0), Engine::malloc_mode::managed}});

  spdlog::info("malloc memory for Poseliftert");

  auto input_size = pose_2d_dim * kNum2DJoints * sizeof(float);
  input_data_.reset(new float[input_size]);
  spdlog::info("allocated {} byte for image input data dims=[{}, {}]",
               input_size * sizeof(float), kNum2DJoints, pose_2d_dim);

  net_input_data_ =
      std::static_pointer_cast<float[]>(engine_.get_input_device_owned_ptr(0));
  spdlog::info("use engine input buffer at {} for net input data",
               fmt::ptr(net_input_data_.get()));

  net_output_data_ =
      std::static_pointer_cast<float[]>(engine_.get_output_device_owned_ptr(0));
  spdlog::info("use engine output buffer at {} for net output data",
               fmt::ptr(net_output_data_.get()));

  input_ = Input(input_data_.get(), width, height, net_input_data_.get());
}

void Poseliftert::forward() {
  input_.process();

  engine_.forward();
  cudaDeviceSynchronize();
}

}  // namespace poseliftert
