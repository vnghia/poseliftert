#include "poseliftert/output/output.hpp"

#include "poseliftert/config.hpp"

namespace poseliftert {

Output::Output(float* net_output_ptr, float* output_ptr)
    : net_output_ptr_(net_output_ptr), output_ptr_(output_ptr) {}

void Output::process() {
  for (int i = 0; i < kNum3DJoints; ++i) {
    output_ptr_[3 * i] = -net_output_ptr_[3 * i];
    output_ptr_[3 * i + 1] = net_output_ptr_[3 * i + 2];
    output_ptr_[3 * i + 2] = -net_output_ptr_[3 * i + 1];
  }
}

}  // namespace poseliftert
